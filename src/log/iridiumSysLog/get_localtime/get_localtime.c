#include "get_localtime.h"

struct tm iridium_tm_now;


/* 获取锁失败的时候一个值 */
//static struct tm default_iridium_tm;
struct tm iridium_tm_last;
/*
 * Be wary of readers/writer locks. If there is a novice error when trying to break up a lock, 
 * it is this: seeing that a data structure is frequently accessed for reads and infrequently 
 * accessed for writes, one may be tempted to replace a mutex guarding the structure with a 
 * readers/writer lock to allow for concurrent readers. This seems reasonable, but unless the 
 * hold time for the lock is long, this solution will scale no better (and indeed, may scale 
 * worse) than having a single lock. Why? Because the state associated with the readers/writer
 * lock must itself be updated atomically, and in the absence of a more sophisticated (and less 
 * space-efficient) synchronization primitive, a readers/writer lock will use a single word of 
 * memory to store the number of readers. Because the number of readers must be updated atomically, 
 * acquiring the lock as a reader requires the same bus transaction—a read-to-own—as acquiring 
 * a mutex, and contention on that line can hurt every bit as much.
 * URL - http://blog.csdn.net/ysu108/article/details/39343295
 */
pthread_mutex_t localtime_lock = PTHREAD_MUTEX_INITIALIZER;

#if 0
/**
 * @desc - this is a test function. Test two types of lock.
 * @date - 2017-06-06 by lc
 **/

void *get_localtime_func(void *arg)
{
	time_t now;

	
	time(&now);
	
//	if(pthread_mutex_trylock(&localtime_lock) == 0){
		if(pthread_mutex_lock(&localtime_lock) == 0){

		localtime_r(&now, &iridium_tm_now);
		pthread_mutex_unlock(&localtime_lock);
	}
	return NULL;
}

#endif

/**
 * @desc - thread function to get localtime and store it in a global variable.
 * @date - 2017-06-06 by lc
 * @note - we use a pthread_mutex_trylock to lock this global variable to avoid dead lock. 
 * 		   Because if we can't update the time for limited times, it's ok.
 **/
void *get_localtime_func(void *arg)
{
	time_t now;
	
	while(1){
		if(pthread_mutex_trylock(&localtime_lock) == 0){
			time(&now);
			localtime_r(&now, &iridium_tm_now);
			pthread_mutex_unlock(&localtime_lock);
		}
		/* 500 ms */
		usleep(500000);
	}

	return NULL;
}


/**
 * @desc - user interface. 
 * @date - 2017-06-06 by lc
 **/
void get_localtime(struct tm *tm_in)
{
	if(!tm_in)
		return;
	memset(tm_in, 0, sizeof(struct tm));
	
	if(pthread_mutex_trylock(&localtime_lock) == 0){
		memcpy(tm_in, &iridium_tm_now, sizeof(iridium_tm_now));
		memcpy(&iridium_tm_last, &iridium_tm_now, sizeof(iridium_tm_now));
		pthread_mutex_unlock(&localtime_lock);
		return;
	}else{
		memcpy(tm_in, &iridium_tm_last, sizeof(iridium_tm_last));
	}
}

/**
 * @desc - initial function. Start a thread to update iridium_tm_now on every 10ms
 * @date - 2017-06-06 by lc.
 **/
pthread_t start_getlocaltime_thread()
{
	pthread_t pth_get_localtime;
	int err;

	memset(&iridium_tm_now, 0, sizeof(iridium_tm_now));
	memset(&iridium_tm_last, 0, sizeof(iridium_tm_last));
	
	/* init our iridium_tm_now on start */
	time_t now;
	time(&now);
	localtime_r(&now, &iridium_tm_now);
	memcpy(&iridium_tm_last, &iridium_tm_now, sizeof(iridium_tm_last));
	
	err = pthread_create(&pth_get_localtime, NULL, get_localtime_func, (void *)1);
	if(0 != err){
		fprintf(stderr, "pthread create error!");
		return -1;
	}

	return pth_get_localtime;
}
