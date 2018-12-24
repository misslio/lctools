/*
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ get_trace.c
 * @brief ‐ 打印当前函数调用堆栈
 * @author ‐ lc
 * @date ‐ 2018-06-10
 */

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define STACK_SIZE 100

/**
 * @brief ‐ 打印当前函数调用堆栈，但static的函数没有函数名称
 * @params ‐ stack_info : 栈信息存储缓冲区
 *			 len : stack_info的长度
 * @ret ‐ None
 * @TO DO - 支持static函数符号打印
 * @author ‐ lc
 * @date ‐ 2018-06-11
 **/
void get_trace(char *stack_info, int len)
{
    int j, nptrs;

    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, STACK_SIZE);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

	int offset = 0;
    for (j = 0; j < nptrs; j++){
		if(stack_info){
			offset += snprintf(stack_info + offset, len - offset, "%d	%s\n", j + 1, strings[j]);
		}else{
			printf("%d	%s\n", j + 1, strings[j]);
		}
	}

    free(strings);
	strings = NULL;
	return;
}
