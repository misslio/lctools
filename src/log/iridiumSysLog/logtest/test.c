/**********
 * desc : V3版日志使用示例，参考使用
 *
 *  链接选项 : -lpthread -liridiumlog
 *  环境配置：1. 拷贝libiridiumlog.so到/usr/lib64目录下
 *			  2. 在/usr/include/下新建irididum目录，然后将iridiumlog.h wrapfcntl.h
 *				 两个文件拷贝到/usr/include/iridium目录下。
 * date : 2016-10-27 by lc
 **********/

#include <stdio.h>
#include <unistd.h>
#include "./../iridiumlog.h"

#define LOG_NAME "example_default"


/**
 * @desc - 测试默认的日志文件
 * @date - 2017-12-18 created by lc
 **/
static void test_default_log_file()
{
	iridium_write_log(NULL, IRILOG_DEBUG, "Test the filename debug log");
	iridium_write_log(NULL, IRILOG_INFO, "Test the filename info log");
	iridium_write_log(NULL, IRILOG_FATAL, "Test the filename warning log");
	iridium_write_log(NULL, IRILOG_ERR, "Test the filename err log");
}

/**
 * @desc - 测试配置文件中的日志
 * @date - 2017-12-18 created by lc
 **/
static void test_conf_log_file()
{
	iridium_write_log("your_log_file_name", IRILOG_DEBUG, "Test the filename debug log");
	iridium_write_log("your_log_file_name", IRILOG_INFO, "Test the filename debug log");
	iridium_write_log("your_log_file_name", IRILOG_FATAL, "Test the filename warning log");
	iridium_write_log("your_log_file_name", IRILOG_ERR, "Test the filename err log");
}


/**
 * @desc - 测试配置文件中不存在的日志写入
 * @date - 2017-12-18 created by lc
 **/
static void test_no_conf_log_file()
{
	iridium_write_log(LOG_NAME, IRILOG_DEBUG, "Test the filename debug log");
	iridium_write_log(LOG_NAME, IRILOG_INFO, "Test the filename debug log");
	iridium_write_log(LOG_NAME, IRILOG_FATAL, "Test the filename warning log");
	iridium_write_log(LOG_NAME, IRILOG_ERR, "Test the filename err log");
}


void test_handle_processing()
{
	iridium_write_log(LOG_NAME, IRILOG_ERR, "Test the filename err log");

	iridium_write_log("your_log_file_name", IRILOG_DEBUG, "Test the filename debug log");

	iridium_write_log(NULL, IRILOG_DEBUG, "Test the filename debug log");

	iridium_write_log("xxx", IRILOG_DEBUG, "Test the filename err log");
	iridium_write_log("xxxx", IRILOG_DEBUG, "Test the filename err log");
	iridium_write_log("444", IRILOG_DEBUG, "Test the filename err log");
	iridium_write_log("434", IRILOG_DEBUG, "Test the filename err log");
	iridium_write_log("4674", IRILOG_DEBUG, "Test the filename err log");
	iridium_write_log("123", IRILOG_ERR, "Test the filename err log");
}

void test_open_file_size()
{
	while(1)
	iridium_write_log("open_file_size", IRILOG_ERR, "Test the filename err log");
}


int main()
{

	printf("log version:%s\n", iridium_log_version());
	
	test_handle_processing();
	test_default_log_file();
	test_conf_log_file();
	test_no_conf_log_file();

	iridium_destroy_log();

#if 0
//	test_open_file_size();

	int i = 0;
	struct timespec tp;
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
//	printf("%ld.%09ld\n", tp.tv_sec, tp.tv_nsec);

	int j = 0;
	for(j = 0; j < 1; j++){
		for(i = 0; i < 10000; i++){
			iridium_write_log("open_file_size.log", IRILOG_ERR, \
				"1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGH");
		}
//		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
//		printf("%d, %ld.%09ld, per: %ld(ns)\n", j, tp.tv_sec, tp.tv_nsec, (tp.tv_sec * 1000000000 + tp.tv_nsec)/((j + 1) * 1000000));
	}

#endif

	return 0;
}
