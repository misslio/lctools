/**
 * Copyright 2018‐2018 LICAN. All Rights Reserved
 * @author - lc
 * @date - 2018-12-06
 **/
#include <stdlib.h>
#include "Mergesort.h"

/**
 * @brief - 升序方式，归并排序的merge实现
 * @params - ori : 待排序数组
 *			 tmpA : 临时转存数组
 *			 left : ori中的左侧数据开始下标
 *			 right : ori中的右侧数据开始下标
 *			 rightEnd : ori中右侧数据结束下标
 * @ret - None
 * @author - lc
 * @date - 2018-12-06
 **/
void Merge(ElementType ori[], ElementType tmpA[], 
		int left, int right, int rightEnd)
{
	int leftEnd = right - 1;
	int count = 0,j;
	
	while(left <= leftEnd && right <= rightEnd){
		if(ori[left] < ori[right]){
			tmpA[count++] = ori[left++];
		}else{
			tmpA[count++] = ori[right++];
		}
	}
	
	//检查剩余数据
	while(left <= leftEnd){
		tmpA[count++] = ori[left++];
	}
	while(right <= rightEnd){
		tmpA[count++] = ori[right++];
	}

	//将数据拷贝回原数组
	for(j = count-1; j >=0; j--){
		ori[rightEnd--] = tmpA[j];
	}
}

void Msort(ElementType ori[], ElementType tmpA[], int left, int right)
{
	int centor;
	
	if(left < right){
		centor = (left + right)/2;
		#if 1
		Msort(ori, tmpA, left, centor);
		Msort(ori, tmpA, centor+1, right);
		Merge(ori, tmpA, left, centor+1, right);
		#else	//为何下面的方法不行
		Msort(ori, tmpA, left, centor-1);
		Msort(ori, tmpA, centor, right);
		Merge(ori, tmpA, left, centor, right);	
		#endif
	}
}

/**
 * @brief - 升序方式，归并排序的merge实现
 * @params - ori : 待排序数组
 *			 len : ori数组长度
 * @ret - None
 * @author - lc
 * @date - 2018-12-06
 * @Note - 稳定性:稳定
 **/
void Mergesort(ElementType ori[], int len)
{
	ElementType *tmpA = (ElementType *)malloc(len * sizeof(ElementType));
	
	if(tmpA){
		Msort(ori, tmpA, 0, len - 1);
	}
	
	free(tmpA);	
}