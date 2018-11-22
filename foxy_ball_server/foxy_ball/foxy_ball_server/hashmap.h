#ifndef _HASHMAP_H_
#define _HASHMAP_H_
#include"arraylib.h"

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

//释放Entry(不会释放其本身)
void free_entry(Entry *entry);

//创建Entry
Entry *new_entry();

//hash计算下标
int hash(char *k,int length);

//times33算法
unsigned int times33(char *key);

//判断key是否存在(返回0不存在)
Entry *exist_key(Array *arr, char *k);

//向hash表中添加数据(返回1成功 0失败)
int add_data(Array *arr,char *k,char *v);

//删除key以及其数据(返回1成功 0失败)
int del_key(Array *arr, char *k);

//光速查找数据(返回0失败)
char *get_data(Array *arr,char *k);

//标准输出所有kv
void show_all();
#endif