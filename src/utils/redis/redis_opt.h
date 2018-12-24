/****
* Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
* @file ‐ redis_opt.h
* @brief ‐ redis订阅消息功能，步骤，初始化redis连接，初始化订阅消息，销毁redis连接
* @author ‐ qxq
* @date ‐ 2017‐04-25
* @[NOTE]‐
* @[change log] ‐ 2018-07-18 by lwd
****/


#ifndef __REDIS_OPT_H__
#define __REDIS_OPT_H__


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <pthread.h>
#include <hiredis/hiredis.h>


#define SUB_OK 		0
#define SUB_ERR 	(-1)
#define CHANNEL_NAME_LEN 	128
#define DEFINE_IP_LEN 		16
#define FUNC_MAX_COUNT		50


/**
 * @brief - 订阅消息的回调函数格式
 * @params -channel_name:订阅消息的通道名
 			message:订阅通道接受到的消息内容
 			len:消息内容的长度
 			
 * @ret - None
 * @author - lwd
 * @date - 2018-07-19
 **/
typedef void (*message_func_t)(const char *channel_name, const char *message, int len);


/**
 * @brief - 初始化redis连接
 * @params -host:redis服务器ip
 			port:redis服务器端口
 * @ret - SUB_OK:redis连接初始化成功，SUB_ERR:redis连接初始化失败
 * @author - lwd
 * @date - 2018-07-19
 **/
int init_redis_conn(char *host, guint32 port);


/**
 * @brief - 初始化订阅消息
 * @params -channel_name:订阅消息通道
 			func:处理订阅消息的回调函数
 * @ret - SUB_OK:消息订阅成功，SUB_ERR:消息订阅失败
 * @author - lwd
 * @date - 2018-07-19
 **/
int init_subscribe(const char *channel_name, message_func_t func);


/**
 * @brief ‐ 发送redis的set数据类型消息，并设置过期时间
 * @params ‐data_key:redis的set类型的key
 			data_value:redis的set类型的value
 			ttl:redis的set类型key的过期时间，单位秒
 * @ret ‐ 0:设置成功，-1:设置失败data_key 或 data_value无效
 * @author - lwd
 * @date ‐ 2018‐07-24
 **/
int set_ttl_key(char *data_key, char *data_value, int ttl);


/**
 * @brief - 销毁redis连接
 * @params -None
 * @ret - None
 * @author - lwd
 * @date - 2018-07-19
 **/
void destory_redis_conn();


#endif	/* __REDIS_OPT_H__ */