#ifndef  _ARRAY_LIB_H_
#define  _ARRAY_LIB_H_

typedef struct 
{
	int  n;     // 动态数组的元素个数
	//int  size;	// 动态数组的元素空间大小
	void **data; // 动态数组的数据起始地址
} Array;

/*
	功能：初始化动态数组
	参数：
		int n:数组长度
		void(*init_obj)(void*):初始化元素的回调函数
	返回值：
		Array指针或0
*/
Array *init_array(int n);

#endif  /* _ARRAY_LIB_H_ */