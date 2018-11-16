#include <string.h>
#include<malloc.h>
#include"utils.h"

char** split(char *str, char *delimiter) {

	int len = strlen(str);
	char *strCopy = (char*)malloc((len + 1) * sizeof(char)); //��������һ���ַ��ռ����洢'\0'

	strcpy(strCopy, str); //����str�ַ���

	//��strCopy�е�ÿ���ָ�����ֵΪ'\0'
	for (int i = 0; strCopy[i] != '\0'; i++) {
		for (int j = 0; delimiter[j] != '\0'; j++) {
			if (strCopy[i] == delimiter[j]) {
				strCopy[i] = '\0';
				break;
			}
		}
	}

	//Ϊ�ַ����������ռ�,��������һ���ַ���ָ�벢��ֵΪNULL����Ϊ�ַ���������־
	char** res = (char**)malloc((len + 2) * sizeof(char*));
	len++; //������strCopy����'\0'�Ž���
	int resI = 0; //ÿһ���ָ�����ԭ�ַ�����'\0'��־������Ϊ�����е��ַ����Ľ�����־

	for (int i = 0; i < len; i++) {
		res[resI++] = strCopy + i;
		while (strCopy[i] != '\0') {
			i++;
		}
	}
	res[resI] = 0; //�ַ��������н�����־

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