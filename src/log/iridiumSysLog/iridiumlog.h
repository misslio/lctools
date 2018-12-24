#ifndef IRIDIUM_LOG_H
#define IRIDIUM_LOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <pthread.h>

#include "wrapfcntl.h"
//#include "com_def.h"

#define IRILOG_PATH "/var/log/iridium/"

#define LOGPATH_MAX_LEN 256


#define NO_INIT			0
#define HAS_INITED		1


#define  PROCESS_WLOCK()		do{if(info->is_process_lock) node->wlock(node->fd, 0, SEEK_SET, 0);}while(0)
#define  PROCESS_UNLOCK()		do{if(info->is_process_lock) node->unlock(node->fd, 0, SEEK_SET, 0);}while(0)

#define FILE_NAME_LEN 	128


/**
 * des	:  log level
 *
 * IRILOG_START and IRILOG_END are just flags.
 **/
typedef enum _loglevl{
	IRILOG_STRART = 0,

	IRILOG_DEBUG,
	IRILOG_INFO,
	IRILOG_ERR,
	IRILOG_FATAL,

	IRILOG_END
}loglevl;

/**
 * @desc - dynamic information for a log file.
 * @date - 2017-12-16 created by lc
 **/
typedef struct _log_file_node{
	char name[FILE_NAME_LEN];
	char date[32];		/* 记录文件创建日期 */

	int fd;
	int times;
	guint64 file_size;
	int id;

	int rollback_use;

	pthread_rwlock_t logfile_pthread_rwlock;
	int (*wlock)(int, off_t, int, off_t);
	int (*unlock)(int, off_t, int, off_t);
}log_file_node_t;

int transparent_iridium_write_log(const char* fileName, const char* funcName, int line, const char *log_file, int level, const char *fmt, ...);



#define iridium_write_log(log_file, level, fmt, ...) (transparent_iridium_write_log(__FILE__, __func__, __LINE__, (log_file), (level) , (fmt) , ##__VA_ARGS__))
/* destroy the whole system log */

int open_iridiumlog();

void iridium_destroy_log(void);

const char * iridium_log_version();

int iridium_init_log(char *logpath);

#endif
