#include <string.h>
#include<malloc.h>
#include"utils.h"

char** split(char *str, char *delimiter) {

	int len = strlen(str);
	char *strCopy = (char*)malloc((len + 1) * sizeof(char)); //额外分配多一个字符空间来存储'\0'

	strcpy(strCopy, str); //复制str字符串

	//将strCopy中的每个分隔符赋值为'\0'
	for (int i = 0; strCopy[i] != '\0'; i++) {
		for (int j = 0; delimiter[j] != '\0'; j++) {
			if (strCopy[i] == delimiter[j]) {
				strCopy[i] = '\0';
				break;
			}
		}
	}

	//为字符串数组分配空间,额外分配多一个字符串指针并赋值为NULL来作为字符串结束标志
	char** res = (char**)malloc((len + 2) * sizeof(char*));
	len++; //遍历到strCopy最后的'\0'才结束
	int resI = 0; //每一个分隔符和原字符串的'\0'标志依次作为数组中的字符串的结束标志

	for (int i = 0; i < len; i++) {
		res[resI++] = strCopy + i;
		while (strCopy[i] != '\0') {
			i++;
		}
	}
	res[resI] = 0; //字符串数组中结束标志

	return res;
}

int get_split_length(char** src)
{
	int i = 0;
	for (; src[i] != 0; i++);
	return i;
}

void free_split(char** src)
{
	for (int i = 0; src[i] != 0; i++)
		free(src[i]);
	free(src);
}