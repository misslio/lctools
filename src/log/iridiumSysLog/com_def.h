#ifndef COM_DEF_H
#define COM_DEF_H

#include "config.h"

#ifndef LC_FREE_MALLOC
#define LC_FREE_MALLOC(p) do	{\
		if((p) != NULL)\
			{\
				free(p);\
				(p) = NULL;\
			}\
	}while(0)
#endif


#ifdef LOG_DEBUG
#ifndef PRINT_DEBUG
#define PRINT_DEBUG(format, args...)	printf("[\033[33mDEBUG INFO\033[0m]\033[33mfunc\033[0m: %s \033[33mline\033[0m: %d :"format, __func__, __LINE__, ##args)
#endif
#ifndef PRINT_ERR
#define PRINT_ERR(format, args...) do{\
	printf("[\033[31mERR INFO\033[0m]\033[31mfunc\033[0m: %s \033[31mline\033[0m: %d :"format, __func__, __LINE__, ##args);\
	exit(0);}while(0)
#endif
#else
#ifndef PRINT_DEBUG
#define PRINT_DEBUG(format, args...)	(void)0
#endif
#ifndef PRINT_ERR
#define PRINT_ERR(format, args...)	(void)0
#endif
#endif

#ifndef PRINT_ERR_FATAL
#define PRINT_ERR_FATAL(format, args...) do{\
	printf("[\033[31mERR INFO\033[0m]\033[31mfunc\033[0m: %s \033[31mline\033[0m: %d :"format, __func__, __LINE__, ##args);\
	exit(0);}while(0)
#endif

#ifndef FILE_NAME_LEN
#define FILE_NAME_LEN 	128
#endif

#endif
