/****
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ iridium_conf_core.h
 * @brief ‐ 读取配置文件的核心代码
 * @author ‐ lc
 * @date ‐ 2018‐04-21
 ****/

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

#ifndef LC_FREE_MALLOC
#define LC_FREE_MALLOC(p) do	{\
		if((p) != NULL)\
			{\
				free(p);\
				(p) = NULL;\
			}\
	}while(0)
#endif

/**************** debug print *********************/
#define DEBUG_MODE

#ifdef DEBUG_MODE	/*********open debug print*******/

#ifndef PRINT_DEBUG
#define PRINT_DEBUG(format, args...)	printf("[\033[33mDEBUG INFO\033[0m]\033[33mfunc\033[0m: %s \033[33mline\033[0m: %d :"format, __func__, __LINE__, ##args)
#endif
#ifndef PRINT_ERR
#define PRINT_ERR(format, args...) printf("[\033[31mERR INFO\033[0m]\033[31mfunc\033[0m: %s \033[31mline\033[0m: %d :"format, __func__, __LINE__, ##args)
#endif

#else /********  close debug print*********/

#ifndef PRINT_DEBUG
#define PRINT_DEBUG(format, args...)	do{;}while(0)
#endif

#ifndef PRINT_ERR
#define PRINT_ERR(format, args...)	do{;}while(0)
#endif


#endif
/**************** debug print end line *********************/

/** ERR DEFINE **/
#define PARAMETER_ERR	-2
#define REPEAT_INIT_ERR	-3
#define OPEN_FILE_ERR	-4
#define READ_FILE_ERR	-5
#define MALLOC_BUF_ERR	-6
#define CONF_CLOSE_ALL	-9


#define CONF_FILE_PATH		"./iridium_all.conf"
#define MAX_CONF_FILE_SIZE	(1024 * 1024 * 2)	 /* 2M */
#define MAX_LINE_LEN 		1024				/* 1k */
#define CONF_STRING_MAX_LEN		128
#define CONF_MAX_NODE_NUM		64				

#define IRI_CONF_FINISH_INIT	1

typedef struct _single_conf_pair {
    char name[CONF_STRING_MAX_LEN];
    char value[CONF_STRING_MAX_LEN];
}single_conf_pair_t;

typedef struct _conf_pairs{
	char mode_name[CONF_STRING_MAX_LEN];
	single_conf_pair_t pairs[CONF_MAX_NODE_NUM];
	int cur_pos;
}conf_pairs_t;

typedef struct _iri_conf_manager{
	GHashTable *iri_conf;	/* key - mode name, value - conf_pairs_t */
	int mode_count;
	int init_flag;
}iri_conf_manager_t;


int get_conf_info_from_file(const char *conf_file, iri_conf_manager_t *conf_manager);
char *get_value(conf_pairs_t * conf_pairs, char *name);
conf_pairs_t * get_conf_pairs(iri_conf_manager_t *conf_manager, char *mode_name);
void destroy_iri_conf(iri_conf_manager_t *conf_manager);

void go_through_conf_manager(iri_conf_manager_t *conf_manager);


#endif
