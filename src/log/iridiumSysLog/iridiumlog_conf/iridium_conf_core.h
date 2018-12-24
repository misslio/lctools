/******************************************************************
 * filename : iridiumlog.h
 * des : a simple log
 *
 * date	: 2016-05-06
 ******************************************************************/

#ifndef IRIDIUM_CONF_CORE_H
#define IRIDIUM_CONF_CORE_H

#include <stdio.h>
#include <string.h>

#include <sys/types.h> /* about files */
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdarg.h>

#include <time.h>

#include <errno.h>
#include <stdlib.h>

#include <pthread.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <glib.h>
#include <ctype.h>
#include "../com_def.h"


/**
 * @desc - 以下错误是允许出现的，所以值均未正数
 * @date - 2017-12-29 changed by lc
 **/
#define PARAMETER_ERR	-2
#define REPEAT_INIT_ERR		3
#define OPEN_FILE_ERR	4
#define READ_FILE_ERR	5
#define CONF_FILE_MAP_ERR	6
#define CONF_CLOSE_ALL	9

#define MAX_CONF_FILE_SIZE	(1024 * 1024 * 2)	 /* 2M */
#define MAX_LINE_LEN 		1024				/* 1k */
#define CONF_STRING_MAX_LEN		128
#define CONF_MAX_NODE_NUM		64				

#define IRI_CONF_FINISH_INIT	1

typedef struct _log_single_conf_pair {
    char name[CONF_STRING_MAX_LEN];
    char value[CONF_STRING_MAX_LEN];
}log_single_conf_pair_t;

typedef struct _log_conf_pairs{
	char mode_name[CONF_STRING_MAX_LEN];
	log_single_conf_pair_t pairs[CONF_MAX_NODE_NUM];
	int cur_pos;
}log_conf_pairs_t;

typedef struct _log_iri_conf_manager{
	GHashTable *iri_conf;	/* key - mode name, value - log_conf_pairs_t */
	int mode_count;
	int init_flag;
	int log_switch;
}log_iri_conf_manager_t;

char *log_get_value(log_conf_pairs_t * conf_pairs, char *name);
log_conf_pairs_t * log_get_conf_pairs(log_iri_conf_manager_t *conf_manager, char *mode_name);
void log_destroy_iri_conf(log_iri_conf_manager_t *conf_manager);
void log_go_through_conf_manager(log_iri_conf_manager_t *conf_manager);


int log_get_conf_info_from_file(char *conf_file, log_iri_conf_manager_t *conf_manager);
void log_conf_manager_foreach(log_iri_conf_manager_t *conf_manager, void(*user_func)(gpointer key, gpointer value, gpointer user_data), gpointer user_data);


#endif
