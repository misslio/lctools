#include <stdio.h>

#include "def.h"

#include "Insertsort.h"

int main()
{
	ElementType a[] = {34, 8, 64, 51, 32, 21};
	int i = 0;
	
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	Insertsort(a, sizeof(a)/sizeof(a[0]));
	PRINT_INT_ARRAY(a, sizeof(a)/sizeof(a[0]), i);
	
	return 0;
}