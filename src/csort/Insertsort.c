/**
 * Copyright 2018‐2018 LICAN. All Rights Reserved
 * @author - lc
 * @date - 2018-12-06
 **/

#include "Insertsort.h"
 
/**
 * @brief - 插入排序,升序方式
 * @params - ori : 待排序数组
 *			 len	: 数组中元素个数
 * @ret - None
 * @author - lc
 * @date - 2018-12-06
 * @Note - 稳定性:稳定
 *		   时间复杂度: O(N^2)
 **/
void Insertsort(ElementType ori[], int len)
{
	int i, j;
	ElementType tmp;
	for(i = 1; i < len; i++){
		tmp = ori[i];
		for(j = i; j > 0 && ori[j-1] > tmp; j--){
			ori[j] = ori[j-1];	
		}
		ori[j] = tmp;
	}
	
	return;
}