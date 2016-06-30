/**
 * desc - Print info to console with file, func and line number. 
 **/

#ifdef SIMPLE_DEBUG

#define PRINT_DEBUG(format, args...)	printf("[\033[33mDEBUG INFO\033[0m]\033[33mfunc\033[0m: %s \033[33mline\033[0m: %d :"format, __func__, __LINE__, ##args)
#else
#define PRINT_DEBUG(format, args...)	do{;}while(0)
#endif

#define PRINT_ERR(format, args...) printf("[\033[31mERR INFO\033[0m]\033[31mfunc\033[0m: %s \033[31mline\033[0m: %d :"format, __func__, __LINE__, ##args)

#ifndef LC_FREE_MALLOC
#define LC_FREE_MALLOC(p) do\
	{\
		if((p) != NULL)\
		{\
			free(p);\
			(p) = NULL;\
		}\
	}while(0)
#endif




