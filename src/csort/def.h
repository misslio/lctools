/**
 * Copyright 2018‐2018 LICAN. All Rights Reserved
 * @file - def.h
 * @brief - 为本目录代码提供共用的宏定义
 * @author - lc
 * @date - 2018-12-06
 **/

#ifndef LCTOOLS_DEF_H
#define LCTOOLS_DEF_H

#define PRINT_INT_ARRAY(A, LEN, index)	do{for(index = 0; index < LEN; index++){printf("%d ", A[index]);} printf("\n");}while(0)
	
typedef int ElementType;

#endif /* LCTOOLS_DEF_H */