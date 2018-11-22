#include<malloc.h>
#include"global.h"
#include"hashmap.h"
#include"persistence.h"
Array *hash_table = 0;
Expires *expires_head = 0;
unsigned long memory_amount = 0;

int init_foxy_ball()
{
	////初始化数据库哈希表
	//hash_table = new_hash_table(1024);

	//从数据库文件中加载hash_table
	if(!load_hash_table())
		hash_table = new_hash_table(1024*1024);		//加载失败则重新创建hash_table


	//初始化过期key表
	expires_head = (Expires *)malloc(sizeof(Expires));
	expires_head->entry = 0;
	expires_head->next = 0;
	expires_head->last = 0;
	expires_head->expire = 0;
	if (hash_table == 0 || expires_head == 0)
		return 0;
	return 1;
}