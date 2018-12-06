#include <stdio.h>

#include "Shellsort.h"


#define PRINT_INT_ARRAY(A, LEN, index)	do{for(index = 0; index < LEN; index++){printf("%d ", A[index]);} printf("\n");}while(0)
int main()
{
	ElementType a[] = {34, 8, 64, 51, 32, 21};
	int i = 0;
	
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	Shellsort(a, sizeof(a)/sizeof(a[0]));
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	
	return 0;
}