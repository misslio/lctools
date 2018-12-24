

#ifndef MUDUO_CLOG_H
#define MUDUO_CLOG_H


#ifdef _cplusplus  
extern "C" {  
#endif  

#define LOGINFO_MAX_LEN	(1024 * 32)

enum MUDUO_CLOG_LEVEL{
    MUDUO_CLOG_LEVEL_TRACE,
    MUDUO_CLOG_LEVEL_DEBUG,
    MUDUO_CLOG_LEVEL_INFO,
    MUDUO_CLOG_LEVEL_WARN,
    MUDUO_CLOG_LEVEL_ERROR,
    MUDUO_CLOG_LEVEL_FATAL,
    MUDUO_CLOG_LEVEL_NUM_LOG_LEVELS,
};
int muduo_clog_init(char *proc_name);
	
int muduo_clog_get_level();

void muduo_clog_trace(const char *c_file, const char *func, int line, const char* fmt, ...);

void muduo_clog_debug(const char *c_file, const char *func, int line, const char* fmt, ...);

void muduo_clog_info(const char *c_file, int line, const char* fmt, ...);
void muduo_clog_warn(const char *c_file, int line, const char* fmt, ...);
void muduo_clog_error(const char *c_file, int line, const char* fmt, ...);

void muduo_clog_fatal(const char *c_file, int line, const char* fmt, ...);
void muduo_clog_syserr(const char *c_file, int line, const char* fmt, ...);

void muduo_clog_sysfatal(const char *c_file, int line, const char* fmt, ...);

#define MUDUO_CLOG_TRACE(fmt, ...)	if(muduo_clog_get_level() <= MUDUO_CLOG_LEVEL_TRACE) muduo_clog_trace(__FILE__, __func__, __LINE__, (fmt) , ##__VA_ARGS__)
#define MUDUO_CLOG_DEBUG(fmt, ...)	if(muduo_clog_get_level() <= MUDUO_CLOG_LEVEL_DEBUG) muduo_clog_debug(__FILE__, __func__, __LINE__, (fmt) , ##__VA_ARGS__)
#define MUDUO_CLOG_INFO(fmt, ...)	if(muduo_clog_get_level() <= MUDUO_CLOG_LEVEL_INFO) muduo_clog_info(__FILE__, __LINE__, (fmt) , ##__VA_ARGS__)
#define MUDUO_CLOG_WARN(fmt, ...)		muduo_clog_warn(__FILE__, __LINE__, (fmt) , ##__VA_ARGS__)
#define MUDUO_CLOG_ERROR(fmt, ...)		muduo_clog_error(__FILE__, __LINE__, (fmt) , ##__VA_ARGS__)
#define MUDUO_CLOG_FATAL(fmt, ...)		muduo_clog_fatal(__FILE__, __LINE__, (fmt) , ##__VA_ARGS__)
#define MUDUO_CLOG_SYSERR(fmt, ...)		muduo_clog_syserr(__FILE__, __LINE__, (fmt) , ##__VA_ARGS__)
#define MUDUO_CLOG_SYSFATAL(fmt, ...)	muduo_clog_sysfatal(__FILE__, __LINE__, (fmt) , ##__VA_ARGS__)
		
#ifdef _cplusplus  
}  
#endif  

#endif /* MUDUO_CLOG_H */
