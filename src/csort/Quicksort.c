#include "def.h"

#include "Insertsort.h"

static inline void swap(ElementType *a, ElementType *b)
{
	ElementType tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

ElementType median3(ElementType ori[], int left, int right)
{
	int centor = (left + right)/2;
	//插入排序
	if(ori[centor] < ori[left]){
		swap(&ori[left], &ori[right]);
	}
	if(ori[right] < ori[centor]){
		swap(&ori[right], &ori[centor]);
	}
	if(ori[centor] < ori[left]){
		swap(&ori[centor], &ori[left]);
	}
	
	swap(&ori[centor], &ori[right - 1]);
	return ori[right - 1];
}


#define CUTOFF (3)

void Qsort(ElementType ori[], int left, int right)
{
	int i, j;
	ElementType pivot;
	
	//对于较小的数组，采用插入排序会比快速排序更优，
	//但此处为何是小于等于，而不是大于?
	if(left + CUTOFF <= right){
		pivot = median3(ori, left, right);
		
		i = left;
		j = right - 1;
		for(;;){
			while(ori[++i] < pivot);
			while(ori[--j] > pivot);
			if(i < j){
				swap(&ori[i], &ori[j]);
			}else{
				break;
			}
		}
		swap(&ori[i], &ori[right-1]);
		
		Qsort(ori, left, i-1);
		Qsort(ori, i+1, right);
	}else{
		Insertsort(ori + left, right - left + 1);
	}
}

void Quicksort(ElementType ori[], int len)
{
	Qsort(ori, 0, len - 1);
}