#ifndef IRIDIUMLOG_CONF_H
#define IRIDIUMLOG_CONF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include "iridium_conf_core.h"
#include "../com_def.h"

#define DEFAULT_LOG_CONF_FILES_MEM		50

#define DEFAULT_LOG_ROLLBACK_NUMBER (5)
#define DEFAULT_LOG_SINGLE_MAX_SIZE (30 * 1024 *1024 )	/* 30 M */

#define TOTAL_SWITCH_MODE_NAME "TOTAL_SWITCH"
#define TOTAL_SWITCH_IS_VALID	"IS_VALID"
#define TOTAL_SWITCH_TIMER		"TIMMER"
#define DEFAULT_LOG_FILE_NAME "iridium_default"

enum log_file_line_level{
	INDEX_LOG_LEVEL = 0,
	INDEX_LOG_FILE,
	INDEX_LOG_TERMINAL,
	INDEX_LOG_ROLLBAKC_NUM,
	INDEX_LOG_SINGLE_MAX_SIZE,
	INDEX_LOG_IS_PROCESS_LOCK,
	INDEX_LOG_END
};

enum infos_special_index{
	INFOS_ID_CTL = 0,
	INFOS_ID_TOTAL_SWITCH,
	INFOS_ID_DEFAULT,
	INFOS_ID_START
};

typedef struct _value_string {
    int	value;
    const char *strptr;
} value_string;


typedef struct _irilog_switch{
	unsigned short p_file:1;
	unsigned short p_func:1;
	unsigned short p_line:1;
	unsigned short p_date:1;

	unsigned short f_file:1;
	unsigned short f_func:1;
	unsigned short f_line:1;
	unsigned short f_date:1;

	unsigned short log_terminal:1;
	unsigned short log_file:1;

	unsigned short debug:1;
	unsigned short info:1;
	unsigned short err:1;
	unsigned short fatal:1;
}irilog_switch_t;

typedef struct _single_log_conf_info{
	int log_switch;				/* only in INFOS_ID_CTL，默认打开 */
	int is_valid; 				/* IS_VALID of TOTAL_SWITCH, only in INFOS_ID_CTL, 默认关闭*/
	char name[FILE_NAME_LEN];	/* in all except INFOS_ID_CTL*/	
	int timer;					/* in INFOS_ID_TOTAL_SWITCH */
	irilog_switch_t irilog_switch;	/* in all except INFOS_ID_CTL */
	int loginfo_max_len;			/* in all except INFOS_ID_CTL */
	int rollback_num;				/* in all except INFOS_ID_CTL */
	unsigned int single_max_size;	/* in all except INFOS_ID_CTL */
	int is_process_lock;
}single_log_conf_info_t;

/**
 * @desc - 存储配置文件中静态的配置信息，初始化完成后该结构体不再更改，默认支持的数组个数由宏
 *		   DEFAULT_LOG_CONF_FILES_MEM
 *		   来定义,但不做限制，会随着个数增大动态增长。
 * 		数组前面三个元素有特殊用处，初始即有值
 *		1. INFOS_ID_CTL,即控制信息，存储日志是否开启，总开关是否打开等全局控制变量
 *		2. INFOS_ID_DEFAULT, 默认的配置信息，所有未在配置文件中设置的日志文件，
 *			均会被分配到这个id号.但日志名会根据用户是否传入而决定使用自定义还是默认的
 *		3. INFOS_ID_TOTAL_SWITCH, 配置文件中TOTAL_SWITCH中的所有内容
 * @date - 2017-12-18 created by lc
 **/
typedef struct _log_conf_infos{
	guint32 len;
	guint32 allocated_len;
	single_log_conf_info_t **sinfo;	
	GHashTable *names; 
}log_conf_infos_t;

log_conf_infos_t * get_log_conf_infos(char *conf_file);
void go_through_log_conf_infos(log_conf_infos_t *infos);

void log_conf_infos_destroy(log_conf_infos_t *infos);

single_log_conf_info_t * get_log_conf_info(log_conf_infos_t *infos, int);

#endif
