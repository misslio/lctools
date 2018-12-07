/**
 * Copyright 2018‐2018 LICAN. All Rights Reserved
 * @author - lc
 * @date - 2018-12-06
 **/

#include "Heapsort.h"


//从0开始，所以左孩子是2i+1,而不是2i
#define LeftChild(i) (2*(i)+1)	

/**
 * @brief - 下滤方式构建最大堆
 * @params - ori : 待排序数组
 *			 pos : 开始下滤的父节点
 *			 len : 数组中元素个数
 * @ret - None
 * @author - lc
 * @date - 2018-12-06
 **/
void perceDownMax(ElementType ori[], int pos, int len)
{
	ElementType tmp = ori[pos];
	int i;
	int child;
	//注意循环和if中的边界处理
	for(i = pos; LeftChild(i) < len; i = child){
		child = LeftChild(i);
		if(child < len-1 && ori[child] < ori[child+1])
			child++;
		if(tmp < ori[child])
			ori[i] = ori[child];
		else
			break;
	}
	ori[i] = tmp;	
}

/**
 * @brief - 下滤方式构建最小堆
 * @params - ori : 待排序数组
 *			 pos : 开始下滤的父节点
 *			 len : 数组中元素个数
 * @ret - None
 * @author - lc
 * @date - 2018-12-06
 **/
void perceDownMin(ElementType ori[], int pos, int len)
{
	ElementType tmp = ori[pos];
	int i;
	int child;
	//注意循环和if中的边界处理
	for(i = pos; LeftChild(i) < len; i = child){
		child = LeftChild(i);
		if(child < len-1 && ori[child] > ori[child+1])
			child++;
		if(tmp > ori[child])
			ori[i] = ori[child];
		else
			break;
	}
	ori[i] = tmp;	
}

/**
 * @brief - 堆排序
 * @params - ori : 待排序数组
 *			 len : 数组中元素个数
 * @ret - None
 * @author - lc
 * @date - 2018-12-06
 * @Note - 不需要使用额外的数组
 *		   稳定性:不稳定
 *		   时间复杂度: O(N*logN)
 **/
void Heapsort(ElementType ori[], int len)
{
	int i;
	//构建堆
	for(i = len/2; i >= 0; i--){
		perceDownMax(ori, i, len);
	}
	//删除最小
	ElementType tmp;
	for(i = len - 1; i>0; i--){
		tmp = ori[0];
		ori[0] = ori[i];
		ori[i] = tmp;
		perceDownMax(ori, 0, i);
	}
	
	return;
}