#include"arraylib.h"
#include<malloc.h>


Array *init_array(int n)
{
	int i=0;
	Array *arr=(Array*)malloc(sizeof(Array));
	
	//内存分类失败
	if(arr==0)
		return 0;

	arr->data=(void**)malloc(sizeof(void*)*n);
	arr->n=n;

	return arr;
}
