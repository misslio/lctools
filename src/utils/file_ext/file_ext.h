/****
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ file_type.h
 * @brief ‐ 文件类型后缀定义与匹配模块
 * @author ‐ lc
 * @date ‐ 2018‐09-06
 ****/
#ifndef FILE_TYPE_H
#define FILE_TYPE_H

#include <glib.h>


#define FILE_EXT_NOT_MATCH	0
#define FILE_EXT_MATCH		1

typedef struct _file_ext{
	int count;			/* type_hs中实际存储的文件后缀字符串数量 */
	GHashTable *ext_hs;	/* key值为文件扩展名的，value值无用的hash表，字符串小写存储 */
	
	char *max_str;		/* 为最长的后缀名预先申请空间，内部函数使用 */
	int max_str_len;
	
	int is_black;		/* 非0值表示为黑名单，0表示为白名单[默认值] */
}file_ext_t;


/**
 * 初始化文件类型, 会将default_file_ext中默认的文件类型
 * 添加进来
 **/
file_ext_t * init_file_ext();


/**
 * 清除file_ext中hash表的所有数据
 **/
void clear_file_ext(file_ext_t *file_ext);


/**
 * 销毁file_ext_t指针
 **/
void destroy_file_ext(file_ext_t *);


/**
 * 将后缀类型ext添加进file_ext存储，类型统一转小写存储
 **/
int add_file_ext(file_ext_t *file_ext, char *ext);


/**
 * 检测file_name是否包含在file_ext中，注意file_ext->is_black
 * 会反转检测结果
 **/
int is_need_file_ext(file_ext_t *file_ext, char *file_name);



/**
 * 设置file_ext是黑名单还是白名单
 **/
void set_file_ext_black_white(file_ext_t *, int);

#endif /*  */
