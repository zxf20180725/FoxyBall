#include<malloc.h>
#include <string.h>
#include"hashmap.h"
#include"arraylib.h"

unsigned int times33(char *key)
{
	unsigned int hash = 5381;
	while (*key){
		hash += (hash << 5) + (*key++);
	}
	return (hash & 0x7FFFFFFF);
}

void init_entry(Entry *entry)
{
	entry->k = 0;
	entry->v = 0;
	entry->next = 0;
}

void free_entry(Entry *entry)
{
	/*
	if (entry == 0)
	return;

	free(entry->k);
	free(entry->v);

	entry->k = 0;
	entry->v = 0;

	free_entry(entry->next);

	free(entry);

	entry = 0;*/
	Entry *p = entry->next, *q = 0;
	//传入的是空指针
	if (entry == 0)
		return;
	//不释放头结点
	if (entry->k != 0)
	{
		free(entry->k);
		entry->k = 0;
	}
	if (entry->v != 0)
	{
		free(entry->v);
		entry->v = 0;
	}
	//没有下一个节点
	if (entry->next == 0)
		return;
	//释放单链表
	while (p)
	{
		q = p;
		p = p->next;
		if (q->k != 0)
			free(q->k);
		if (q->v != 0)
			free(q->v);
		free(q);
	}
}

Array *new_hash_table(int length)
{
	Array *arr = init_array(length);
	int i;
	for (i = 0; i < arr->n; i++)
	{
		Entry *entry = new_entry();
		arr->data[i] = entry;
	}
	return arr;
}

Entry *new_entry()
{
	Entry *entry = (Entry*)malloc(sizeof(Entry));
	entry->k = 0;
	entry->v = 0;
	entry->next = 0;
	return entry;
}

int hash(char *k, int length)
{
	int ret = times33(k)&(length - 1);
	return ret;
}

int add_data(Array *arr, char *k, char *v)
{
	//计算index
	int index = hash(k, arr->n);
	//获取entry
	Entry* entry = (Entry*)arr->data[index];
	//hash冲突
	if (entry->k == 0)
	{
		entry->k = k;
		entry->v = v;
	}
	else
	{
		while (1)
		{
			if (strcmp(entry->k, k) == 0)
				return 0;

			if (entry->next == 0)
			{
				Entry *e_t = new_entry();
				e_t->k = k;
				e_t->v = v;
				e_t->next = 0;
				entry->next = e_t;
				return 1;
			}

			entry = entry->next;
		}
	}
}

char *get_data(Array *arr, char *k)
{
	int index = hash(k, arr->n);
	//获取entry
	Entry *entry = (Entry*)arr->data[index];
	//没有查询到
	if (entry->k == 0)
		return 0;
	//链表查询
	do
	{
		if (strcmp(k, entry->k) == 0)
			return entry->v;
	} while (entry = entry->next);
	//没查到
	return 0;
}