#include<malloc.h>
#include"global.h"
#include"hashmap.h"

Array *hash_table = 0;
Expires *expires_head = 0;
unsigned long memory_amount = 0;

int init_foxy_ball()
{
	//��ʼ�����ݿ��ϣ��
	hash_table = new_hash_table(1024);
	//��ʼ������key��
	expires_head = (Expires *)malloc(sizeof(Expires));
	expires_head->entry = 0;
	expires_head->next = 0;
	expires_head->last = 0;
	expires_head->expire = 0;
	if (hash_table == 0 || expires_head == 0)
		return 0;
	return 1;
}