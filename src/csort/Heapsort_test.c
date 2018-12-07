#include <stdio.h>

#include "def.h"

#include "Heapsort.h"

int main()
{
	ElementType a[] = {34, 8, 64, 51, 32, 21, 3};
	int i = 0;
	
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	Heapsort(a, sizeof(a)/sizeof(a[0]));
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	
	return 0;
}