#include <stdio.h>

#include "def.h"

#include "Quicksort.h"

int main()
{
	ElementType a[] = {34, 8, 64, 51, 32, 21, 34, 2, 50, 100, 29, 98, 103, 64, 76, 90, 78, 78,145, 10};
	int i = 0;
	
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	Quicksort(a, sizeof(a)/sizeof(a[0]));
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	
	return 0;
}