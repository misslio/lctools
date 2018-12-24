#include <muduo/base/AsyncLogging.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Timestamp.h>
#include <muduo/utils/ConfFileRead.h>

using namespace muduo;

#ifndef _cplusplus  
#define _cplusplus  
#include "muduo_clog.h"  
#endif  


#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

//off_t kRollSize = 500*1000*1000;

muduo::AsyncLogging* g_asyncLog = NULL;
int muduo_clog_level = 0;

void asyncOutput(const char* msg, int len)
{
  g_asyncLog->append(msg, len);
}

int muduo_clog_init(char *proc_name)
{
  char name[256] = "/var/log/iridium/run/";
  int ret;
  if(!proc_name){
	return -1;
  }
  if(access(name, F_OK) < 0){
	ret = mkdir(name, 0655);
	if(ret < 0){
		return -1;
	}
  }
  
  snprintf(name + strlen(name),  256 - strlen(name), "%s", proc_name);
  ConfFileRead conf("/iridium/config/iridiumlog_muduo.conf");
  off_t kRollSize = conf.GetFileSize();
  static muduo::AsyncLogging log(name, kRollSize);
  log.start();
  
  g_asyncLog = &log;

  muduo::Logger::setOutput(asyncOutput);
  
  muduo_clog_level = muduo::Logger::logLevel();
  
  return 0;
}


int muduo_clog_get_level()
{
	return muduo_clog_level;
}

void muduo_clog_trace(const char *c_file, const char *func, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_TRACE(muduo::Logger::SourceFile(c_file), line, func) << vaArgsBuf;
}

void muduo_clog_debug(const char *c_file, const char *func, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_DEBUG(muduo::Logger::SourceFile(c_file), line, func) << vaArgsBuf;
}

void muduo_clog_info(const char *c_file, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_INFO(muduo::Logger::SourceFile(c_file), line) << vaArgsBuf;
}

void muduo_clog_warn(const char *c_file, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_WARN(muduo::Logger::SourceFile(c_file), line) << vaArgsBuf;
}

void muduo_clog_error(const char *c_file, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_ERROR(muduo::Logger::SourceFile(c_file), line) << vaArgsBuf;
}

void muduo_clog_fatal(const char *c_file, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_FATAL(muduo::Logger::SourceFile(c_file), line) << vaArgsBuf;
}

void muduo_clog_syserr(const char *c_file, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_SYSERR(muduo::Logger::SourceFile(c_file), line) << vaArgsBuf;
}

void muduo_clog_sysfatal(const char *c_file, int line, const char* fmt, ...)
{
	char vaArgsBuf[LOGINFO_MAX_LEN];
	va_list args;
	memset(vaArgsBuf, 0, LOGINFO_MAX_LEN);
	
	va_start(args, fmt);
	vsnprintf(vaArgsBuf, LOGINFO_MAX_LEN, fmt, args);
	va_end(args);
	
	LOG_SYSFATAL(muduo::Logger::SourceFile(c_file), line) << vaArgsBuf;
}









