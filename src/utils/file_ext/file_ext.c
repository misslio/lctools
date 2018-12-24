/****
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ file_ext.c
 * @brief ‐ 文件类型后缀定义与匹配模块代码实现
 * @author ‐ lc
 * @date ‐ 2018‐09-06
 ****/
#include <stdio.h>
#include <glib.h>
#include <string.h>

#include "file_ext.h"


#if 0
/**
 * @brief - 默认支持的还原文件类型，数据来自沙箱
 * @author - lc
 * @date - 2018-09-07
 **/

static char *default_file_ext[] = {
	"EXE",
	"DLL",
	"SCR",
	"DOC",
	"DOCX",
	"XLS",
	"XLSX",
	"PPT",
	"PPTX",
	"RTF",
	"PDF",
	"BAT",
	"CMD",
	"VBS",
	"JS",
	"PS1",
	"HTML",
	"HTA",
	"URL"
};
#endif

/**
 * @brief - 初始化文件类型, 会将default_file_ext中默认的文件类型
 *			添加进来
 * @params - None
 * @ret - 含有默认值的file_ext_t类型指针
 * @author - lc
 * @date - 2018-09-07
 **/
file_ext_t* init_file_ext()
{
	file_ext_t *file_ext = NULL;
	
	file_ext = (file_ext_t *)g_malloc(sizeof(file_ext_t));
	memset(file_ext, 0, sizeof(file_ext_t));

	file_ext->ext_hs = g_hash_table_new_full(g_str_hash, g_str_equal,
					g_free, NULL);

	/* 这里默认不添加文件类型，由还原解析配置模块通过读取数据库调用 add_file_ext 接口添加文件类型 */
	/*
	int i = 0;
	for(i = 0; i < sizeof(default_file_ext)/sizeof(default_file_ext[0]); i++){
		add_file_ext(file_ext, default_file_ext[i]);
	}
	*/

	file_ext->max_str_len = 5;
	file_ext->max_str = g_malloc0(file_ext->max_str_len + 1);
	
	return file_ext;
}




/**
 * @brief - 清除file_ext中hash表的所有数据
 * @params - file_ext : 待销毁指针
 * @ret - None
 * @author - lc
 * @date - 2018-09-13
 **/
void clear_file_ext(file_ext_t *file_ext)
{
	if(!file_ext){
		return;
	}

	if(file_ext->max_str){
		g_free(file_ext->max_str);
		file_ext->max_str = NULL;
	}
	
	if(file_ext->ext_hs){
		g_hash_table_remove_all(file_ext->ext_hs);
	}
	
	file_ext->count = 0;
	file_ext->max_str_len = 0;
	
}


/**
 * @brief - 销毁file_ext_t指针
 * @params - file_ext : 待销毁指针
 * @ret - None
 * @author - lc
 * @date - 2018-09-07
 **/
void destroy_file_ext(file_ext_t *file_ext)
{
	if(!file_ext){
		return;
	}
	
	if(file_ext->max_str){
		g_free(file_ext->max_str);
		file_ext->max_str = NULL;
	}
	if(file_ext->ext_hs){
		g_hash_table_destroy(file_ext->ext_hs);
		file_ext->ext_hs = NULL;
	}

	g_free(file_ext);
}

/**
 * @brief - 将后缀类型ext添加进file_ext存储，类型统一转小写存储
 * @params - file_ext : 存储所有ext的指针
 *			 ext : 文件后缀类型，例如"exe", "Docx", 不对大小写做
 *				   要求
 * @ret - -1 : 失败
 *		  0 : 成功
 * @author - lc
 * @date - 2018-09-07
 **/
int add_file_ext(file_ext_t *file_ext, char *ext)
{
	if(!file_ext || !ext){
		return -1;
	}

	int len = 0;
	char *ext_key = NULL;

	/* 拷贝ext并转小写 */
	len = strlen(ext);
	ext_key = g_ascii_strdown(ext, -1);
	file_ext->count++;

	g_hash_table_insert(file_ext->ext_hs, ext_key, ext_key);

	if(len > file_ext->max_str_len){
		file_ext->max_str_len = len;
		if(file_ext->max_str){
			g_free(file_ext->max_str);
			file_ext->max_str = NULL;
		}
		file_ext->max_str = g_malloc(file_ext->max_str_len + 1);
	}

	
	return 0;
}


/**
 * @brief - 检测file_name是否包含在file_ext中，注意file_ext->is_black
 *			会反转检测结果
 * @params - file_ext : 存储所有ext的指针
 *			 file_name : 要检测的文件名
 * @ret - -1 : 出错,
 *		  下面两个宏的值必定不是负数
 *		  FILE_EXT_MATCH : 	匹配成功
 *		  FILE_EXT_NOT_MATCH : 匹配失败
 * @author - lc
 * @date - 2018-09-07
 **/
int is_need_file_ext(file_ext_t *file_ext, char *file_name)
{	
	int match_result = FILE_EXT_NOT_MATCH;
	int len = strlen(file_name);
	int i = 0, ext_len = 0;
	int have_ext = 0;

	if(!file_ext || !file_name){
		return -1;
	}

	file_ext->max_str[0] = '\0';

	/* 提取文件后缀名       
	 * i > 0 ,表示i不能为0，即".exe"这种文件名定为不匹配
	 */
	for(i = len - 1; i > 0; i--){
		if(file_name[i] == '.'){
			have_ext = 1;
			break;
		}
		file_ext->max_str[ext_len++] = file_name[i];
		if(ext_len > file_ext->max_str_len){
			goto is_need_file_ext_exit;
		}
	}

	if(!have_ext){
		goto is_need_file_ext_exit;
	}
	
	file_ext->max_str[ext_len] = '\0';

	char *new_ext = g_strreverse(file_ext->max_str);
	char *down_ext = g_ascii_strdown(new_ext, -1);

	if(down_ext){
		if(g_hash_table_lookup(file_ext->ext_hs, down_ext)){
			match_result = FILE_EXT_MATCH;
		}

		g_free(down_ext);
		down_ext = NULL;
	}
	
is_need_file_ext_exit:
	
	if(file_ext->is_black){
		return FILE_EXT_MATCH == match_result ? FILE_EXT_NOT_MATCH : FILE_EXT_MATCH;
	}else{
		return match_result;
	}
}


/**
 * @brief - 设置file_ext是黑名单还是白名单
 * @params - file_ext : 存储所有ext的指针
 *			 is_black : 0 表示白名单，1表示黑名单
 * @ret - None
 * @author - lc
 * @date - 2018-09-07
 **/
void set_file_ext_black_white(file_ext_t *file_ext, int is_black)
{
	if(file_ext){
		file_ext->is_black = is_black;
	}
}


#if 0	/* 测试main函数 */
/**
 * @brief - 测试数据
 * @author - lc
 * @date - 2018-09-07
 **/
static char *test_file_name[] = {
	"name",			/* no match */
	"name.exe",		/* match */
	"name.Exe",		/* match */
	"name.ExE",		/* match */
	"name.PPTX",	/* match */
	"name.Pptx",	/* match */
	".exe"
};

int main()
{
	int i; 
	file_ext_t *file_ext = NULL;
	
	file_ext = init_file_ext();
	
	for(i = 0; i < sizeof(test_file_name)/sizeof(test_file_name[0]); i++){
		printf("%s->%d\n", test_file_name[i], is_need_file_ext(file_ext, test_file_name[i]));
	}
	
	destroy_file_ext(file_ext);
}
#endif /* 0 */

