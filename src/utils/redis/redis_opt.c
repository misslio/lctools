/****
* Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
* @file ‐ redis_opt.c
* @brief ‐ redis订阅消息功能，步骤，初始化redis连接，初始化订阅消息，取消订阅消息，销毁redis连接
* @author ‐ qxq
* @date ‐ 2017‐04-25
* @[NOTE]‐
* @[change log] ‐ 2018-07-18 by lwd
****/


#include <string.h>
#include <sys/prctl.h>

#include "redis_opt.h"
#include "include/log.h"
#include "include/def.h"


typedef struct _redis_conn_info{
	char host[DEFINE_IP_LEN];
	guint32 port;
}redis_conn_info_t;


typedef struct _sub_msg{
	pthread_t pt;
	redisContext *c;	
	const char *channel_name;
	message_func_t message_func[FUNC_MAX_COUNT];
	int size;
}sub_msg_t;


static redis_conn_info_t *redis_conn_info = NULL;
redisContext *redis_conn_hd = NULL;
static GHashTable *sub_hash = NULL;
static pthread_mutex_t mut_hash;


/* 订阅回调 */
static void *subscribe_func(void *data)
{
	sub_msg_t *sub_msg = (sub_msg_t *)data;
	
	if(sub_msg->channel_name == NULL || sub_msg->c == NULL){
		goto PTHREAD_EXIT;
	}
	redisReply *reply = NULL;
	reply = (redisReply*)redisCommand(sub_msg->c, "SUBSCRIBE %s", sub_msg->channel_name);
	if(reply != NULL){
		freeReplyObject(reply);
		IRIDIUM_LOG_INFO_MC("subscribe success:%s",sub_msg->channel_name);
	}

	/* 记录模块线程id */
	pid_t pid, tid;
	pid = getpid();
	tid = gettid_v2();
	IRIDIUM_LOG_START("pid = %d, name = redis_sub, tid = %d",pid, tid);
	
	char tname[128];
	snprintf(tname, 128, "redis_sub_%s_%d", sub_msg->channel_name, tid);
	prctl(PR_SET_NAME, tname);
	
	for(;;){
		int ret = redisGetReply(sub_msg->c,(void*)&reply);
		if(ret == REDIS_OK){
			if(reply->type == REDIS_REPLY_ARRAY && reply->elements == 3){
				if(reply->element[2]->str){
					int i;
					for(i = 0; i < sub_msg->size; i++){
						IRIDIUM_LOG_INFO_MC("channel %s callback count %d.", sub_msg->channel_name, i);
						sub_msg->message_func[i](reply->element[1]->str, reply->element[2]->str, (int)reply->element[2]->len);
						IRIDIUM_LOG_INFO_MC("info: %s; %s; %d", reply->element[1]->str, reply->element[2]->str, (int)reply->element[2]->len);
					}
				}
			}
			freeReplyObject(reply);
			IRIDIUM_LOG_INFO_MC("subscribe listen thread running.");
		}else{
			IRIDIUM_LOG_ERROR_MC("<%04d>subscribe reply error %d.", DB_R_INVALID_RE, ret);
			sleep(30);
		}
	}

PTHREAD_EXIT:
	IRIDIUM_LOG_START("subscribe listen thread exit.");
	pthread_exit((void *)0);
}


/* 取消订阅 */
static int unsubscribe(const char *channel_name)
{
	if(redis_conn_hd == NULL){
		IRIDIUM_LOG_START("<%04d>redis_conn_hd is NULL.", DB_R_CONN_FAIL);
		return SUB_ERR;
	}
	
	if(channel_name == NULL){
		IRIDIUM_LOG_START("<%04d>unsubscribe fail channel %s is NULL.", DB_INVALID_ARG, channel_name);
		return SUB_ERR;
	}
	
	redisReply *reply = NULL;
	
	reply = (redisReply*)redisCommand(redis_conn_hd, "UNSUBSCRIBE %s", channel_name);
	
	if(reply == NULL){
		IRIDIUM_LOG_START("<%04d>unsubscribe fail reply is NULL.", DB_R_INVALID_RE);
		return SUB_ERR;
	}
	freeReplyObject(reply);
	
	IRIDIUM_LOG_START("unsubscribe success:%s", channel_name);

	return SUB_OK;
}


/* key注销函数 */
static void sub_key_destroy(gpointer *data)
{
	g_free(data);
}


/* value注销函数 */
static void sub_value_destroy(gpointer *data)
{
	sub_msg_t *value = (sub_msg_t *)data;
	if(value){
		int re;
		re  = pthread_cancel(value->pt);
		pthread_join(value->pt, NULL);
		
		IRIDIUM_LOG_START("channel %s pthread_cancel %d", value->channel_name, re);
		
		redisFree(value->c);
		g_free(value);
	}
}


/**
 * @brief - 初始化redis连接
 * @params -host:redis服务器ip
 			port:redis服务器端口
 * @ret - SUB_OK:redis连接初始化成功，SUB_ERR:redis连接初始化失败
 * @author - lwd
 * @date - 2018-07-19
 **/
int init_redis_conn(char *host, guint32 port)
{
	if(redis_conn_info != NULL){
		IRIDIUM_LOG_START("redis already init.");
		return SUB_OK;
	}else{
		redis_conn_info = (redis_conn_info_t *)g_malloc(sizeof(redis_conn_info_t));
		if(redis_conn_info == NULL){
			IRIDIUM_LOG_START("<%04d>malloc redis_conn_info_t fail.", DB_MALLOC_FAIL);
			return SUB_ERR;
		}
	}
	
	snprintf(redis_conn_info->host, DEFINE_IP_LEN, host);
	redis_conn_info->port = port;
	
	sub_hash = g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify)sub_key_destroy, (GDestroyNotify)sub_value_destroy);
	
	struct timeval timeout = {1, 500000}; // 1.5 seconds {s,us}
	IRIDIUM_LOG_START("connection info %s %u.", redis_conn_info->host, redis_conn_info->port);
	redis_conn_hd = redisConnectWithTimeout(redis_conn_info->host, redis_conn_info->port, timeout);
	if (redis_conn_hd->err) {
		g_free(redis_conn_info);
		redis_conn_info = NULL;
	
		redisFree(redis_conn_hd);
		IRIDIUM_LOG_START("<%04d>Redis connection error: %s.", DB_R_CONN_FAIL, redis_conn_hd->errstr);
		return SUB_ERR;
	}
	return SUB_OK;
}


 /**
 * @brief - 初始化订阅消息
 * @params -channel_name:订阅消息通道
 			func:处理订阅消息的回调函数
 * @ret - SUB_OK:消息订阅成功，SUB_ERR:消息订阅失败
 * @author - lwd
 * @date - 2018-07-19
 **/
int init_subscribe(const char *channel_name, message_func_t func)
{	
	pthread_mutex_lock(&mut_hash);
	
	char *key = NULL;
	sub_msg_t *sub_msg = NULL;
	
	sub_msg = (sub_msg_t *)g_hash_table_lookup(sub_hash, channel_name);
	if(sub_msg){
		if(sub_msg->size >= FUNC_MAX_COUNT){
			IRIDIUM_LOG_START("<%04d>add channel %s fail,over max callback func %d.", DB_R_EXEC_SQL, channel_name, FUNC_MAX_COUNT);
			goto init_subscribe_exit;
		}
		
		sub_msg->message_func[sub_msg->size] = func;
		sub_msg->size++;
		IRIDIUM_LOG_START("add channel %s func size %d.", channel_name, sub_msg->size);
		pthread_mutex_unlock(&mut_hash);
		return SUB_OK;
	}else{
		struct timeval timeout = {1, 500000}; // 1.5 seconds {s,us}
		redisContext *c = redisConnectWithTimeout(redis_conn_info->host, redis_conn_info->port, timeout);

		if (c->err){
			redisFree(c);
			IRIDIUM_LOG_START("<%04d>Redis connection error: %s.", DB_R_CONN_FAIL, c->errstr);
			
			goto init_subscribe_exit;
		}
		
		key = (char *)g_malloc(sizeof(char)*CHANNEL_NAME_LEN);
		sub_msg = (sub_msg_t *)g_malloc(sizeof(sub_msg_t));
		
		if(key != NULL && sub_msg != NULL){
			memset(key, 0, CHANNEL_NAME_LEN);
			memset(sub_msg, 0, sizeof(sub_msg_t));
			
			snprintf(key, CHANNEL_NAME_LEN, channel_name);
			
			sub_msg->c = (redisContext *)c;
			sub_msg->channel_name = channel_name;
			sub_msg->message_func[sub_msg->size] = func;
			sub_msg->size = 1;
			
			IRIDIUM_LOG_START("init channel %s func size %d.", sub_msg->channel_name, sub_msg->size);
			
			int ret = pthread_create(&(sub_msg->pt), NULL, subscribe_func, sub_msg);
			if(ret != 0){
				IRIDIUM_LOG_START("<%04d>create new sub thread fail!", DB_R_THREAD_FAIL);
				goto init_subscribe_exit;
			}
			pthread_detach(sub_msg->pt);
			
			g_hash_table_insert(sub_hash, key, sub_msg);
			
			pthread_mutex_unlock(&mut_hash);
			return SUB_OK;
		}else{
			g_free(key);
			g_free(sub_msg);
		}
	}
	
init_subscribe_exit:
	pthread_mutex_unlock(&mut_hash);
	return SUB_ERR;
}


/**
 * @brief ‐ 发送redis的set数据类型消息，并设置过期时间
 * @params ‐data_key:redis的set类型的key
 			data_value:redis的set类型的value
 			ttl:redis的set类型key的过期时间，单位秒
 * @ret ‐ 0:设置成功，-1:设置失败data_key 或 data_value无效
 * @author - lwd
 * @date ‐ 2018‐07-24
 **/
int set_ttl_key(char *data_key, char *data_value, int ttl)
{
	if(data_key == NULL || data_value == NULL)
	{
		IRIDIUM_LOG_ERROR_MC("<%04d>data_key or data_value is NULL.", DB_INVALID_ARG);
		return -1;
	}
	
	redisReply *reply = NULL;

	reply = (redisReply*)redisCommand(redis_conn_hd, "SET %s %s", data_key, data_value);
	if(reply != NULL){
		freeReplyObject(reply);
	}
	
	reply = (redisReply*)redisCommand(redis_conn_hd, "EXPIRE %s %d", data_key, ttl);
	if(reply != NULL){
		freeReplyObject(reply);
	}

	return 0;
}


/**
 * @brief - 销毁redis连接
 * @params -
 * @ret - None
 * @author - lwd
 * @date - 2018-07-19
 **/
void destory_redis_conn()
{
	GHashTableIter iter;
	gpointer key, value;
	g_hash_table_iter_init(&iter, sub_hash);
	while(g_hash_table_iter_next(&iter, &key, &value)){
		char *channel_name = (char *)key;
		
		unsubscribe(channel_name);
	}
	
	g_hash_table_destroy(sub_hash);
	sub_hash = NULL;
	redisFree(redis_conn_hd);
	g_free(redis_conn_info);
}



