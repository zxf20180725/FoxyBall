#ifndef _UTILS_H
#define _UTILS_H


/*
用delimiter中的每一个分隔符分割str字符串,这并不会改变str中的字符,然后返回一个字符串数组.
字符串数组中的字符串有可能是以'\0'开头的空串(出现在str首尾或连续的分隔符会产生这样的空串).

注意：
	一定要手动free返回值
*/
char** split(char *str, char *delimiter);

//获取split函数的返回值的元素个数
int get_split_length(char** src);

//释放split函数的返回值
void free_split(char** src);

#endif