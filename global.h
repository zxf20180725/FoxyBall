#ifndef _GLOBAL_H
#define _GLOBAL_H
#include"arraylib.h"
#include"expire.h"

#define VERSION "0.0.1"
#define UPDATE_TIME "2018��11��16�� 16:13:39"
#define AUTHOR "�ƻ���Ƥ��"

extern Array *hash_table;		//���ݿ���Ľṹ
extern Expires *expires_head;	//���ڱ�

//��ʼ�����ݿ�
int init_foxy_ball();
#endif
