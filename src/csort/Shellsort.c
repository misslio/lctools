/**
 * Copyright 2018‐2018 LICAN. All Rights Reserved
 * @author - lc
 * @date - 2018-12-05
 **/
 
#include "Shellsort.h"

/**
 * @brief - 希尔排序,升序方式
 * @params - ori : 待排序数组
 *			 len	: 数组中元素个数
 * @ret - None
 * @author - lc
 * @date - 2018-12-05
 * @Note - 稳定性:不稳定
 * @分析- 希尔增量不一定互素
 *		  Hibbard提出的增量序列：1,3,7,...,2^k - 1,最坏运行时间: delta(N^(3/2))
 * 		  Sedgewick提出了几种增量序列，最好的序列是{1,5,19,41,109,...},更好些
 **/
void Shellsort(ElementType ori[], int len)
{
	int i,j,increment;
	ElementType tmp;
	
	for(increment = len/2; increment > 0; increment /=2)
		for(i = increment; i<len; i++){
			tmp = ori[i];
			for(j = i; j >= increment; j-= increment){
				if(tmp > ori[j-increment])
					ori[j] = ori[j-increment];
				else
					break;
			}
			ori[j] = tmp;
		}
}
