/**
 * Copyright 2018‐2018 LICAN. All Rights Reserved
 * @author - lc
 * @date - 2018-12-05
 **/
 
#include "Bucketsort.h"

ElementType GetMax(ElementType A[], int N)
{
	int i = 0;
	ElementType max = A[0];
	for(i = 1; i < N; i++){
		if(max < A[i]) 
			max = A[i];
	}
	return max;
}

/* 假设待排序数组都是正整数 */
void Bucketsort(ElementType A[], int N)
{
	ElementType *buckets, max;

	max = GetMax(A, N);
	buckets = (ElementType *)malloc(max * sizeof(ElementType));
	memset(buckets, -1, max * sizeof(ElementType));
	
	int i = 0;
	for(i = 0; i < N; i++){
		buckets[A[i]] = A[i];
	}
	int j = 0;
	for(i = 0; i < max; i++){
		if(buckets[i] != -1)
			A[j++] = buckets[i];
	}
	
	return;
}