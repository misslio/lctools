/****
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ iridium_conf.c
 * @brief ‐ 提供不同类型的配置文件项的值的转换接口的实现代码
 * @author ‐ lc
 * @date ‐ 2018‐04-21
 ****/


#include "iridium_conf_core.h"
#include "include/log.h"

#include "iridium_conf.h"

enum _TYPE_VALUE{
	INT_VALUE = 1,
	DOUBLE_VALUE,
 	CHAR_STR_VALUE,
	GINT8_VALUE,
	GINT16_VALUE,
	GINT32_VALUE,
	GINT64_VALUE,
	GUINT8_VALUE,
	GUINT16_VALUE,
	GUINT32_VALUE,
	GUINT64_VALUE
};


/* global variable, default values are given here. comment added by lc on 2017-09-21 */
static iri_conf_manager_t conf_manager;


static char* get_iridium_config_value(char* pair, char* key)
{	
	conf_pairs_t *conf_pairs;
	conf_pairs = get_conf_pairs(&conf_manager, pair);
	
	if(NULL == conf_pairs) {
		return NULL;
	}
	
	return get_value(conf_pairs, key);
}


static void READ_IRIDIUM_CONFIG_AND_LOG(char* pair, char* key, void* src_value, int value_type)
{
	char *tmp_value = NULL;

	int tmp_int = 0;
	long int tmp_long = 0;
	char *stop;
	tmp_value = get_iridium_config_value(pair, key);
	if(tmp_value){
		switch(value_type) {
		case INT_VALUE:
			sscanf(tmp_value, "%d", (int*)src_value);
			break;
		case DOUBLE_VALUE:
			sscanf(tmp_value, "%lf", (double*)src_value);
			break;
 		case CHAR_STR_VALUE:
			*(char**)src_value = tmp_value;
			break;
		case GINT8_VALUE:
			tmp_int = atoi(tmp_value);
			*(gint8*)src_value = (gint8)(tmp_int & 0xff);
			break;
		case GINT16_VALUE:
			tmp_int = atoi(tmp_value);
			*(gint16*)src_value = (gint16)(tmp_int & 0xffff);
			break;
		case GINT32_VALUE:
			sscanf(tmp_value, "%d", (gint32*)src_value);
			break;
		case GINT64_VALUE:
			sscanf(tmp_value, "%lu", (gint64*)src_value);
			break;
		case GUINT8_VALUE:
			tmp_long = strtoul(tmp_value, &stop, 10);
			*(guint8*)src_value = (guint16)(tmp_long & 0xff);
			break;
		case GUINT16_VALUE:
			tmp_long = strtoul(tmp_value, &stop, 10);
			*(guint16*)src_value = (guint16)(tmp_long & 0xffff);
			break;
		case GUINT32_VALUE:
			sscanf(tmp_value, "%u", (guint32*)src_value);
			break;
		case GUINT64_VALUE:
			sscanf(tmp_value, "%lu", (guint64*)src_value);
			break;
		default:
			IRIDIUM_LOG_FATAL("Read conf fail[%s-%s]: should not reach here!", pair, key);
			break;
		}

		return;
	}

	IRIDIUM_LOG_FATAL("Read conf fail[%s-%s]: should not reach here!", pair, key);
}

int init_global_config(const char *conf_file)
{
	memset(&conf_manager, 0, sizeof(conf_manager));
	return get_conf_info_from_file(conf_file, &conf_manager);	
}

void destroy_global_config() 
{	
	destroy_iri_conf(&conf_manager);
	memset(&conf_manager, 0, sizeof(conf_manager));
}

void go_through_global_config()
{
	go_through_conf_manager(&conf_manager);
}

int get_iri_conf_int(char *mode_name, char *item_name)
{
	int get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, INT_VALUE);
	
	return get_value;
}

double get_iri_conf_double(char *mode_name, char *item_name)
{
	double get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, DOUBLE_VALUE);
	
	return get_value;
}


char * get_iri_conf_str(char *mode_name, char *item_name)
{	
	char * get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, CHAR_STR_VALUE);
	
	return get_value;
}

gint8 get_iri_conf_gint8(char *mode_name, char *item_name)
{
	gint8 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GINT8_VALUE);
	
	return get_value;
}
gint16 get_iri_conf_gint16(char *mode_name, char *item_name)
{
	gint16 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GINT16_VALUE);
	
	return get_value;
}

gint32 get_iri_conf_gint32(char *mode_name, char *item_name)
{
	gint32 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GINT32_VALUE);
	
	return get_value;	
}

gint64 get_iri_conf_gint64(char *mode_name, char *item_name)
{
	gint64 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GINT64_VALUE);
	
	return get_value;		
}

guint8 get_iri_conf_guint8(char *mode_name, char *item_name)
{
	guint8 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GUINT8_VALUE);
	
	return get_value;		
}


guint16 get_iri_conf_guint16(char *mode_name, char *item_name)
{
	guint16 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GUINT16_VALUE);
	
	return get_value;		
}
guint32 get_iri_conf_guint32(char *mode_name, char *item_name)
{
	guint32 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GUINT32_VALUE);
	
	return get_value;		
}
guint64 get_iri_conf_guint64(char *mode_name, char *item_name)
{
	guint64 get_value = 0;
	
	READ_IRIDIUM_CONFIG_AND_LOG(mode_name, item_name, &get_value, GUINT64_VALUE);
	
	return get_value;		
}





