#include <stdio.h>

#include "def.h"

#include "Mergesort.h"

int main()
{
	ElementType a[] = {34, 8, 64, 51, 32, 21};
	int i = 0;
	
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	Mergesort(a, sizeof(a)/sizeof(a[0]));
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	
	return 0;
}