/**
 * Copyright 2018©\2018 LICAN. All Rights Reserved
 * @author - lc
 * @date - 2018-12-05
 **/
 
#include "Shellsort.h"

void Shellsort(ElementType A[], int N)
{
	int i,j,increment;
	ElementType tmp;
	
	for(increment = N/2; increment > 0; increment /=2)
		for(i = increment; i<N; i++){
			tmp = A[i];
			for(j = i; j >= increment; j-= increment){
				if(tmp > A[j-increment])
					A[j] = A[j-increment];
				else
					break;
			}
			A[j] = tmp;
		}
}
