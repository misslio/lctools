/**
 * Copyright 2018‐2018 LICAN. All Rights Reserved
 * @author - lc
 * @date - 2018-12-06
 **/
 
#include "Bubblesort.h"

/**
 * @brief - 冒泡排序,升序方式
 * @params - ori : 待排序数组
 *			 len	: 数组中元素个数
 * @ret - None
 * @author - lc
 * @date - 2018-12-06
 * @Note - 稳定性:稳定
 **/
void Bubblesort(ElementType ori[], int len)
{
	int i,j;
	int tmp = 0;

	for(i = len-1; i > 0; i--){
		for(j = 0; j < i; j++){
			if(ori[j] > ori[j+1]){
				tmp = ori[j+1];
				ori[j+1] = ori[j];
				ori[j] = tmp;
			}
		}
	}

	return;
}
