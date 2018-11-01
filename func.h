#ifndef _FUNC_
#define _FUNC_
#include"ArrayLib.h"

//kv结构
typedef struct kv
{
	struct kv *next;
    char *k;
    char *v;
} Entry;

//创建hashtable
Array *new_hash_table(int length);

//初始化Entry
void init_entry(Entry *entry);

//创建Entry
Entry *new_entry();

//hash计算下标
int hash(char *k,int length);

//DJB Hash Function
unsigned int times33(char *key);

//向hash表中添加数据
int add_data(Array *arr,char *k,char *v);

//光速查找数据
char *get_data(Array *arr,char *k);

#endif