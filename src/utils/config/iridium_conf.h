/****
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ iridium_conf.h
 * @brief ‐ 提供不同类型的配置文件项的值的转换接口
 * @author ‐ lc
 * @date ‐ 2018‐04-21
 ****/


#ifndef IRIDIUM_CONF_H
#define IRIDIUM_CONF_H

#include <glib.h>

#define STR_LEN 128

int init_global_config(const char *);

void destroy_global_config();

int get_iri_conf_int(char *mode_name, char *item_name);
double get_iri_conf_double(char *mode_name, char *item_name);


gint8 get_iri_conf_gint8(char *mode_name, char *item_name);
gint16 get_iri_conf_gint16(char *mode_name, char *item_name);
gint32 get_iri_conf_gint32(char *mode_name, char *item_name);
gint64 get_iri_conf_gint64(char *mode_name, char *item_name);

guint8 get_iri_conf_guint8(char *mode_name, char *item_name);
guint16 get_iri_conf_guint16(char *mode_name, char *item_name);
guint32 get_iri_conf_guint32(char *mode_name, char *item_name);
guint64 get_iri_conf_guint64(char *mode_name, char *item_name);

char * get_iri_conf_str(char *mode_name, char *item_name);

void go_through_global_config();


#endif /* INCLUDE_GLOBAL_VARIABLE_H_ */
