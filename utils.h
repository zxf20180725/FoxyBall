#ifndef _UTILS_H
#define _UTILS_H


/*
��delimiter�е�ÿһ���ָ����ָ�str�ַ���,�Ⲣ����ı�str�е��ַ�,Ȼ�󷵻�һ���ַ�������.
�ַ��������е��ַ����п�������'\0'��ͷ�Ŀմ�(������str��β�������ķָ�������������Ŀմ�).

ע�⣺
	һ��Ҫ�ֶ�free����ֵ
*/
char** split(char *str, char *delimiter);

//��ȡsplit�����ķ���ֵ��Ԫ�ظ���
int get_split_length(char** src);

//�ͷ�split�����ķ���ֵ
void free_split(char** src);

#endif