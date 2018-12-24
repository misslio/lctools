#include "./../iridiumlog.h"


#define THREAD_NUM	70

#define LOG_C	"iridium_analysis_start"
#define LOG_D	"iridium_analysis_fatal"

void *thread_function(void *arg);  
void *thread_function_second(void *arg);  
void *thread_function_three(void *arg);  

  
int main()  
{  

//	iridium_init_log("lc/");

    int res;  
    pthread_t thread_group[THREAD_NUM];  
	void *(*thread_func_group[THREAD_NUM])(void *);

	int i;

	char info[THREAD_NUM][20];
	char temp[20];
	for(i = 0; i< THREAD_NUM; i++){
		sprintf(temp, "%d%d%d%d%d", i, i, i, i, i);
		strcpy(info[i], temp);
	}
	
	for( i = 0; i < THREAD_NUM/3; i++){
		thread_func_group[i] = thread_function;
	}

	for( i = THREAD_NUM/3; i < (THREAD_NUM / 3) * 2; i++)
		thread_func_group[i] = thread_function_second;
	
	for( i = (THREAD_NUM / 3) * 2; i < THREAD_NUM; i++)
		thread_func_group[i] = thread_function_three;
	
	
  	for(i = 0 ; i < THREAD_NUM; i++){
	    res = pthread_create(&(thread_group[i]), NULL, thread_func_group[i], (void*)(info[i]));  
	    if (res != 0)  
	    {  
	       perror("Thread creation failed!");  
		   return -1;
	    }  
  	}


	/* 忽略多个线程同时终止的问题，代码不会执行到此，线程随进程终止 */
 	for(i = 0; i < THREAD_NUM; i++){
		pthread_join(thread_group[i], NULL);
	}

   	return 0;
}  
  
void *thread_function(void *arg)  
{  	  
	while(1){
	   if(iridium_write_log(LOG_C,IRILOG_ERR, "%s--%ld", (const char*)arg, (unsigned long)pthread_self()) < 0){
		   printf("write error\n");
	   	}
	   usleep(1);
	}

	return NULL;  
}  

  
void *thread_function_second(void *arg)  
{  
	while(1){
	   if(iridium_write_log(LOG_C,IRILOG_INFO, "%s--%ld", (const char*)arg, (unsigned long)pthread_self()) < 0){
		   printf("write error\n");
	   	}
	   usleep(1);
	}
	
	return NULL;  
}  


void *thread_function_three(void *arg)  
{  

	while(1){
	   if(iridium_write_log(LOG_D,IRILOG_DEBUG, "%s--%ld", (const char*)arg, (unsigned long)pthread_self()) < 0){
		   printf("write error\n");
	   }
	   usleep(1);
	}
	
	return NULL;  
}  

