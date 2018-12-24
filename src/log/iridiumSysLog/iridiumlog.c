#include <sys/types.h>
#include <unistd.h>
#include "iridiumlog.h"
#include "iridiumlog_conf/iridiumlog_conf.h"
#include "get_localtime/get_localtime.h"
#include "get_proc_name/get_proc_name.h"

#include "config.h"


#define WRITE_TIMES	500

#define LOGINFO_MAX_LEN	(1024 * 32)

#if 0
#define CONF_FILE_PATH		"./iridiumlog_common2.conf"
#else
#define CONF_FILE_PATH		"/iridium/config/iridiumlog_muduo.conf"
#endif
#define MONTH_ARRAY_MAX_INDEX	12

static char log_version[128];

static char *logInfo[] = {"NOUSE","DEBUG", "INFO", "ERR", "FATAL" ,"NOUSE"};

static const char month_array[][4] = {"Jan",  "Feb",  "Mar",  "Apr",  "May",  "Jun", "Jul", "Aug", "Sep", "Oct", "Nov",  "Dec", "ABC"};

static time_t debug_dead_time = 0;
static int close_log_flag = 0;
static volatile  int init_flag = NO_INIT;
static char global_log_path[LOGPATH_MAX_LEN] = {0};

static pthread_mutex_t init_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_rwlock_t hash_lock = PTHREAD_RWLOCK_INITIALIZER;

static GHashTable *log_file_nodes = NULL;
static GHashTable *conf_log_file_names = NULL;
static log_conf_infos_t *infos = NULL;

static pthread_t getlocaltime_pth = 0;

/* 定义日志名称需要的信息 */
static char *proc_name = NULL;
static pid_t proc_pid = 0;
static char *host_name = NULL;
/**
 * @desc - version info
 * @date - 2017-06-06 created by lc
 **/
 const char * iridium_log_version()
 {
 	snprintf(log_version, 128, "iridium syslog -V%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	return log_version;
 }


/**
 * @desc - initial funtion of iridiumlog
 * @date - 2017-12-16 created by lc
 **/
int iridium_init_log(char *logpath)
{
	PRINT_DEBUG("Enter this func\n");
	pthread_mutex_lock(&init_lock);
	int ret = -1;
	if(NO_INIT != init_flag){
		ret = 0;
		goto iridium_init_log_exit;
	}

	if(logpath){
		if(logpath[0] == '/'){
			PRINT_ERR_FATAL("para[%s] should not start with \"/\"\n", logpath);
		}
		snprintf(global_log_path, LOGPATH_MAX_LEN - 1, "%s%s%s", IRILOG_PATH, logpath, logpath[strlen(logpath) - 1] != '/'?"/":"");
	}else{
		strcpy(global_log_path, IRILOG_PATH);
	}

	int ret2 = 0;
	if(access(global_log_path, F_OK) < 0){
		if((ret2 = mkdir(global_log_path, 0655)) < 0){
			goto iridium_init_log_exit;
		}
	}

	infos = get_log_conf_infos(CONF_FILE_PATH);
	PRINT_DEBUG("get infos. count:%d\n", infos->len);
	if(0 == infos->sinfo[INFOS_ID_CTL]->log_switch){
		close_log_flag = 1;
		log_conf_infos_destroy(infos);
		infos = NULL;
		ret = 0;
		goto iridium_init_log_exit;
	}else if(2 == infos->sinfo[INFOS_ID_CTL]->log_switch){
		//to do -deletle all files
	}

	if(infos->sinfo[INFOS_ID_CTL]->is_valid){
		time_t now;
		time(&now);
		if(infos->sinfo[INFOS_ID_TOTAL_SWITCH]->timer != 0){
			debug_dead_time = now + infos->sinfo[INFOS_ID_TOTAL_SWITCH]->timer;
		}
	}

	conf_log_file_names = infos->names;

	/* 初始化 log_file_nodes */
	log_file_nodes = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	getlocaltime_pth = init_iridium_log_thread();

	/* 获取日志名称需要的信息 */
	if(!proc_name){
		proc_name = (char *)malloc(64);
		if(!proc_name){
			PRINT_ERR("get proc_name failed\n");
		}else{
			proc_name[0] = '\0';
			get_proc_path_name(NULL, proc_name, 64);
			/* 避免proc_name为空的字符串 */
			if(strlen(proc_name) < 1){
				strcpy(proc_name, "default");
			}
		}
	}

	proc_pid = getpid();

	if(!host_name){
		host_name = (char *)malloc(64);
		if(!host_name){
			PRINT_ERR("get host_name failed\n");
		}else{
			gethostname(host_name, 64);
		}
	}
	
	ret = 0;

iridium_init_log_exit:
	/* 赋值必须放在初始化完毕，锁释放之前，否则多线程同步初始化会出现错误
	 * 因为这样即使在锁未释放前init_flag的值已改变，初始化工作也已经完成
	 */
	init_flag = HAS_INITED;
	pthread_mutex_unlock(&init_lock);
	PRINT_DEBUG("Leave this func\n");
	return ret;
}

/**
 * @date - 2017-12-16 created by lc
 **/
void iridium_destroy_log()
{
	log_conf_infos_destroy(infos);
	if(log_file_nodes)
		g_hash_table_destroy(log_file_nodes);
	conf_log_file_names = NULL;

	if(proc_name){
		free(proc_name);
		proc_name = NULL;
	}

	if(host_name){
		free(host_name);
		host_name = NULL;
	}

	if(getlocaltime_pth > 0){
		pthread_cancel(getlocaltime_pth);
		pthread_join(getlocaltime_pth, NULL);
	}

}

/**
 * @desc - this function can be an external function for uesers
 * @date - 2017-12-16 created by lc
 **/
int open_iridiumlog()
{	
	if(NO_INIT == init_flag)
		return iridium_init_log(NULL);
	
	return 0;
}

/**
 * @desc - 根据规则新建日志文件
 * @date - 2017-06-29 created by lc
 **/
void create_log_file_fd(log_file_node_t *node)
{
	if(!node)
		return;

	if(node->fd > 0){
		close(node->fd);
	}

	struct tm tm_now;
	get_localtime(&tm_now);

	snprintf(node->date, 32, "%04d%02d%02d-%02d%02d%02d", tm_now.tm_year+1900, tm_now.tm_mon + 1, \
			tm_now.tm_mday, tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);

	
	char new_file_name[256];
	int name_len;
	int fd = -1;
	int ori_len = 0;
	char logPath[LOGPATH_MAX_LEN + 256];
	
	name_len = snprintf(new_file_name, 256, "%s.%s.%s.%s.%d.log", proc_name, node->name, node->date, host_name, proc_pid);

	strcpy(logPath, global_log_path);
	ori_len = strlen(logPath);

	name_len = strlen(new_file_name);
	memcpy(logPath + ori_len, new_file_name, name_len);
	logPath[ori_len + name_len] = '\0';

	fd = open(logPath, O_CREAT|O_APPEND|O_RDWR, 0644);
	node->fd = fd > 0 ? fd : -1;

	return;
}


/**
 * @date - 2017-12-16 created by lc
 **/
log_file_node_t* new_log_file_node(single_log_conf_info_t *info, int id, char *logPath, const char *log_file)
{
	PRINT_DEBUG("Enter this func\n");
	
	log_file_node_t* new_node = (log_file_node_t *)g_malloc(sizeof(log_file_node_t));
	memset(new_node, 0, sizeof(log_file_node_t));

	if(log_file){
		snprintf(new_node->name, FILE_NAME_LEN, "%s", log_file);
	}else{
		strcpy(new_node->name, info->name);
	}
	
	new_node->id = id;
	pthread_rwlock_init(&(new_node->logfile_pthread_rwlock), NULL);
	new_node->wlock = writew_lock;
	new_node->unlock = un_lock;

	create_log_file_fd(new_node);

#if 0	/* 关闭初次打开的检测，每次都必定为新文件 */
	/* 检测初次打开时的文件大小 */
	if(new_node->fd > 0){
		struct stat statbuff;
		if(fstat(new_node->fd, &statbuff) == 0){	/* 调用失败，继续写入 */
			if(statbuff.st_size >= info->single_max_size){
				ftruncate(new_node->fd, 0);
				lseek(new_node->fd, 0, SEEK_SET);
			}else{
				new_node->file_size = statbuff.st_size;
			}
		}	
	}
#endif
	PRINT_DEBUG("Leave this func\n");

	return new_node;
}


/**
 * @desc - search a log file node from hashtable, if not existed, create new one
 * @params - log_file : must be a valid string
 *			 info : must not be NULL
 *			 id : must be a valid id
 * @date - 2017-12-17 created by lc
 **/
log_file_node_t* get_log_file_node(const char *log_file, single_log_conf_info_t *info, int id)
{
	PRINT_DEBUG("Enter this func, file_name:%s\n", log_file);
	if(!log_file_nodes || !info)
		return NULL;
	
	log_file_node_t *new_node = NULL;
	log_file_node_t *ret = NULL;

	char logPath[LOGPATH_MAX_LEN];
	strcpy(logPath, global_log_path);
	int ori_len = strlen(logPath);

	int name_len;

	name_len = strlen(log_file);
	memcpy(logPath + ori_len, log_file, name_len);
	logPath[ori_len + name_len] = '\0';

	ret = (log_file_node_t *)g_hash_table_lookup(log_file_nodes, log_file);
	if(ret){
		if(ret->fd < 0){
			#if 0
			ret->fd = open(logPath, O_CREAT|O_APPEND|O_RDWR, 0644);
            if(ret->fd <= 0) {
                PRINT_ERR("open file[%s] fail!\n", logPath);
                goto get_log_file_node_exit;
            }else{
				struct stat statbuff;
				if(fstat(new_node->fd, &statbuff) == 0){
					if(statbuff.st_size >= info->single_max_size){
						ftruncate(new_node->fd, 0);
						lseek(new_node->fd, 0, SEEK_SET);
					}
				}
			}
			#else
			create_log_file_fd(ret);	/*一般不会发生，否则是发生错误，文件描述符不明原因无效 */
			if(ret->fd < 0){
				PRINT_ERR("open file[%s] fail!\n", logPath);
                goto get_log_file_node_exit;
			}
			#endif
		}

		goto get_log_file_node_exit;
	}

	

	char *key_name = (char *)g_malloc(strlen(log_file) + 1);
	memcpy(key_name, log_file, strlen(log_file));
	key_name[strlen(log_file)] = '\0';
	
	pthread_rwlock_wrlock(&hash_lock);
	/* new file node. create and insert! */
	new_node = new_log_file_node(info, id , logPath, log_file);
	g_hash_table_insert(log_file_nodes, key_name, new_node);
	pthread_rwlock_unlock(&hash_lock);
	
	ret = new_node;
		
get_log_file_node_exit:
	PRINT_DEBUG("Leave this func\n");
    return ret;	
}

/**
 * @desc - 日志写入的调用接口
 *         处理流程：
 *				1 日志是否已初始化
 *				2 日志功能是否关闭
 *				3 定时器是否开启，若开启，则检测，时间到并且为debug级别，则直接返回
 *				4 如果总开关 ---开启，则使用INFOS_ID_TOTAL_SWITCH为info_id，
 *						     ---未开启，检测配置文件日志名称的hash表
 *										--- 找到，则使用对应info_id
 *										--- 未找到，则使用INFOS_ID_DEFAULT
 *				5 根据info_id获取响应配置信息sw
 *				6 根据sw生成相应日志内容
 *				...
 *				   
 * @date - 2017-12-18 created by lc
 **/
int transparent_iridium_write_log(const char* fileName, const char* funcName, int line, const char *log_file, int level,  const char* fmt, ...)
{	
	PRINT_DEBUG("Enter this func\n");
	int ret = 0;
	int file_size = 0;

	/* 初始化 */
	ret = open_iridiumlog();
	if(ret < 0){
		PRINT_DEBUG("open iridiumlog fail\n");
		return -1;
	}

	/* 是否关闭 */
	if(1 == close_log_flag){
		PRINT_DEBUG("log is closed!\n");
		return 0;
	}

	/* debug定时器是否开启，开启则检测*/
	if(IRILOG_DEBUG == level && debug_dead_time > 0){
		PRINT_DEBUG("DEAD_TIME: %ld\n", debug_dead_time);
		time_t now;
		time(&now);
		PRINT_DEBUG("now: %ld\n", now);
		if(debug_dead_time < now){
			PRINT_DEBUG("DEBUG IS CLOSED BECAUSE OF TIMER!\n");
			return 0;
		}		
	}

	/* 如果总开关开启，则所有id均指向INFOS_ID_TOTAL_SWITCH*/
	int info_id = INFOS_ID_TOTAL_SWITCH;	
	char real_log_file[FILE_NAME_LEN];
	if(!infos->sinfo[INFOS_ID_CTL]->is_valid){	
		int *search_id;
		if(log_file){
			search_id = (int *)g_hash_table_lookup(conf_log_file_names, log_file);
			if(search_id){
				PRINT_DEBUG("get %s in configure file, id = %d\n", log_file, *search_id);
				info_id = *search_id;
			}else{
				PRINT_DEBUG("NO %s in configure file\n", log_file);
				info_id = INFOS_ID_DEFAULT;	/* 配置文件中没有，则使用默认值 */
			}
		}else{
			PRINT_DEBUG("use default log file: %s", infos->sinfo[INFOS_ID_DEFAULT]->name);
			info_id = INFOS_ID_DEFAULT;	/* 为空，使用默认值，则使用默认值 */
		}
	}

	if(!log_file){
		strcpy(real_log_file, infos->sinfo[INFOS_ID_DEFAULT]->name);
	}else{
		strncpy(real_log_file, log_file, FILE_NAME_LEN - 1);
	}

	PRINT_DEBUG("log_file name : %s, info_id:%d\n", real_log_file, info_id);

	single_log_conf_info_t *info = get_log_conf_info(infos, info_id);
	if(!info){
		PRINT_ERR("info_id[%d] is invalid!\n", info_id);
	}

	irilog_switch_t *sw = &(info->irilog_switch);
	
	if(!(sw->log_file || sw->log_terminal)){
		PRINT_DEBUG("total closed\n");
		return -1;
	}

	ret = -1;
	switch(level){
		case IRILOG_DEBUG:
			if(1 == sw->debug)
				ret = 0;
			break;
		case IRILOG_INFO:
			if(1 == sw->info)
				ret = 0;
			break;
		case IRILOG_ERR:
			if(1 == sw->err)
				ret = 0;
			break;
		case IRILOG_FATAL:
			if(1 == sw->fatal)
				ret = 0;
			break;
		default:
			break;
	}

	if(-1 == ret){
		PRINT_DEBUG("level[%d] is closed or invalid\n", level);
		return 0;
	}
	
	PRINT_DEBUG("start log information handling\n");

	/****
	 * 以上内容除初始化，无锁，无内存操作，方便无效日志的快速退出
	 * 以下内容，新增日志有线程锁，日志内容组建有内存操作，文件写入有进程和线程锁
	 * 2017-12-18 written by lc
	 ****/
	char infobuf[LOGINFO_MAX_LEN], pinfobuf[LOGINFO_MAX_LEN];
	char date_buf[32], blank[2];
	char file_buf[1024], func_buf[1024], line_buf[64], level_buf[64];
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
#if 1
	memset(infobuf, 0, LOGINFO_MAX_LEN);
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);

	memset(infobuf, 0, LOGINFO_MAX_LEN);
	memset(pinfobuf, 0, LOGINFO_MAX_LEN); 
#endif
	blank[0] = '\0';
#if 1
	memset(date_buf, 0, 32);
	memset(file_buf, 0, 1024);
	memset(func_buf, 0, 1024);
	memset(line_buf, 0, 64);
#endif
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);

	if(sw->f_date || sw->p_date){
		struct tm tm_now;
		get_localtime(&tm_now);

		snprintf(date_buf, 32, "%s %02d %04d %02d:%02d:%02d ", month_array[tm_now.tm_mon < MONTH_ARRAY_MAX_INDEX ? tm_now.tm_mon : MONTH_ARRAY_MAX_INDEX], \
			tm_now.tm_mday, tm_now.tm_year+1900, tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);
	}
	
	if(sw->p_file || sw->p_file){
		char* tmp_f = NULL;
		tmp_f = rindex(fileName, '/');
		if((NULL != tmp_f) && (0 != *tmp_f)){
			snprintf(file_buf, 1024, "File:%s ", tmp_f + 1);
		} else {
			snprintf(file_buf, 1024, "File:%s ", fileName);
		}
	}
	
	if(sw->f_func || sw->p_func)
		snprintf(func_buf, 1024, "Func:%s ", funcName);
	if(sw->f_line || sw->p_line)
		snprintf(line_buf, 64, "Line:%d", line);
	
	if(level == IRILOG_ERR)
		snprintf(level_buf, 64, "\033[31m[%s]:\033[0m", logInfo[level]);
	else
		snprintf(level_buf, 64, "\033[33m[%s]:\033[0m", logInfo[level]);


	date_buf[31] = '\0';
	file_buf[1023] = '\0';
	func_buf[1023] = '\0';
	line_buf[63] = '\0';
	level_buf[63] = '\0';

	PRINT_DEBUG("before terminal printf\n");
	if(sw->log_terminal && 0x01){
		PRINT_DEBUG("terminal printf\n");
		file_size = snprintf(pinfobuf, LOGINFO_MAX_LEN, "%s%s%s%s%s %s\n",\
				(sw->p_date == 1) ? date_buf : blank,\
				(sw->p_file == 1) ? file_buf : blank,\
				(sw->p_func == 1) ? func_buf : blank,\
				(sw->p_line == 1) ? line_buf : blank, level_buf,  vaArgsBuf);
		pinfobuf[LOGINFO_MAX_LEN - 2] = '\0';
		fprintf(stderr, "%s", pinfobuf);
	}

	PRINT_DEBUG("before log file write\n");
	
	if(sw->log_file && 0x01){
		PRINT_DEBUG("log file write\n");
		log_file_node_t *node = NULL;
		/** 根据文件名，获取 log_file_node **/
		node = get_log_file_node(real_log_file, info, info_id);

		/** 检测文件有效性 **/	
		if(node->fd <= 0){
			PRINT_ERR("File <%s> is not opened!\n", node->name);
			return -1;
		}

		ret = pthread_rwlock_wrlock(&(node->logfile_pthread_rwlock));
		if(ret < 0)
			printf("pthread_rwlock_wrlock error\n");
		if(info->is_process_lock)
		PROCESS_WLOCK();

		/* 检测文件大小 */
		struct stat statbuff;
		if(node->times > WRITE_TIMES){
			if(node->file_size > info->single_max_size){
				if(fstat(node->fd, &statbuff) == 0){	/* 调用失败，继续写入 */	
					create_log_file_fd(node);
					node->file_size = 0;
					node->times = 0;
				}
			}
		}

		/* Write log information to file */
		snprintf(infobuf, LOGINFO_MAX_LEN, "%s%s%s%s[%s] %s\n",\
				(sw->f_date == 1) ? date_buf : blank,\
				(sw->f_file == 1) ? file_buf : blank,\
				(sw->f_func == 1) ? func_buf : blank,\
				(sw->f_line == 1) ? line_buf : blank, logInfo[level],  vaArgsBuf);
		infobuf[LOGINFO_MAX_LEN - 2] = '\0';
		infobuf[strlen(infobuf) - 1] = '\n';

		if((file_size = write(node->fd, infobuf, strlen(infobuf))) < 0){
			PRINT_ERR("Write error to log info!\n");
		}else{
			node->times++;
			node->file_size += file_size;
			PRINT_DEBUG("%s\n", infobuf);
		}
		
		PROCESS_UNLOCK();
		
		pthread_rwlock_unlock(&(node->logfile_pthread_rwlock));
	}
	PRINT_DEBUG("Leave this func\n");

	return 0;
}

