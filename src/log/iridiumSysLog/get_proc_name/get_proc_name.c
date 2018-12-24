/**
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ get_proc_name.c
 * @brief - 获取当前运行进程的名称
 * @author - lc
 * @date - 2018-04-26
 **/
 
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/**
 * @brief - 获取当前进程的目录和进程名称
 * @params - proc_dir	存储进程目录，长度需定义为 PATH_MAX,该参数可以为NULL
 *			 proc_name 	存储进程名称，该参数不可为NULL
 * 			 len		proc_name缓冲区的长度
 * @ret - 0 : 失败; >0 : 成功
 * @author - lc
 * @date - 2018-04-26
 **/
int get_proc_path_name(char* proc_dir, char* proc_name, size_t len)
{	
        char* path_end = NULL;
		char *real_dir;
		if(!proc_name){
			return 0;
		}

		if(!proc_dir){
			real_dir = (char *)malloc(PATH_MAX);
			memset(real_dir, 0, PATH_MAX);
		}else{
			real_dir = proc_dir;
		}

		memset(proc_name, 0, len);
		
        if(readlink("/proc/self/exe", real_dir, PATH_MAX) <=0){
                return 0;
        }
		
        path_end = strrchr(real_dir,  '/');
        if(path_end == NULL)
                return 0;
        ++path_end;
        strncpy(proc_name, path_end, len);
        *path_end = '\0';
		
		if(!proc_dir){
			free(real_dir);
			real_dir = NULL;
		}
		
        return path_end - real_dir;
}
#if 0
int main()
{
        char path[PATH_MAX];
        char proc_name[1024];
        get_proc_path_name(path, proc_name, sizeof(proc_name));
        printf("path:%s, proc_name--->%s\n", path, proc_name);
}
#endif