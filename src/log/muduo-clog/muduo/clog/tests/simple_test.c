#include <muduo/base/muduo_clog.h>
#include <stdio.h>
//#include <string.h>
#include <unistd.h>
#include <time.h>

#define LOG_WRITE_NUM	1000000
void get_time_gap(struct timespec *old_tp, struct timespec *new_tp, struct timespec *out_tp)
{
	if(new_tp->tv_nsec < old_tp->tv_nsec){
		new_tp->tv_sec - 1;
		new_tp->tv_nsec += 999999999;
	}
	
	out_tp->tv_sec = new_tp->tv_sec - old_tp->tv_sec;
	out_tp->tv_nsec = new_tp->tv_nsec - old_tp->tv_nsec;
	
}

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("err input\n./simple_test 1 or ./simple_test 30\n");
		return -1;
	}
	muduo_clog_init("simple_test_fake");

	int i = 0, j = 0;
	struct timespec old_tp, new_tp, out_tp;
	for(j = 0; j < atoi(argv[1]); j++){	
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &old_tp);
		for(i = 0; i < LOG_WRITE_NUM; i++){
			MUDUO_CLOG_TRACE("1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGH");
			MUDUO_CLOG_DEBUG("1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGH");
			MUDUO_CLOG_INFO("1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGH");
			MUDUO_CLOG_WARN("1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGH");
			MUDUO_CLOG_ERROR("1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGH");
//			MUDUO_CLOG_SYSFATAL("1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGH");
		}
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &new_tp);
		get_time_gap(&old_tp, &new_tp, &out_tp);
		printf("inner : %ld.%09ld, per: %ld(ns)\n", out_tp.tv_sec, out_tp.tv_nsec, (out_tp.tv_sec * 1000000000 + out_tp.tv_nsec)/ LOG_WRITE_NUM);
	}
	printf("total %ld.%09ld\n", new_tp.tv_sec, new_tp.tv_nsec);
	return 0;
}
