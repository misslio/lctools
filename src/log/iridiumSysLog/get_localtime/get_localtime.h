/****
 * @desc - To use localtime_r safely, for thread and signal
 * @date - 2017-06-06 by lc
 * @status - 
 ****/

 #ifndef GET_LOCALTIME_H
 #define GET_LOCALTIME_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>


extern struct tm iridium_tm_now;

/**
 * 因为目前在日志模块调用，所以名字改为与log相关
 **/
#define init_iridium_log_thread	start_getlocaltime_thread

void *get_localtime_func(void *arg);
void get_localtime(struct tm *tm_in);
pthread_t start_getlocaltime_thread();


#endif
