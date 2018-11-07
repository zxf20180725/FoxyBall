#include<malloc.h>
#include <string.h>
#include"hashmap.h"
#include"arraylib.h"
#include"expire.h"
#include"global.h"

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

Entry *exist_key(Array *arr, char *k)
{
	int index = hash(k, arr->n);
	Entry *current = (Entry *)arr->data[index];
	do
	{
		if (strcmp(current->k, k) == 0)
			return current;
	} while (current = current->next);
	return 0;
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

int del_key(Array *arr, char *k)
{
	int index = hash(k, arr->n);
	Entry *current = (Entry *)arr->data[index];
	Entry *last = 0;
	do
	{
		if (strcmp(current->k, k) == 0)
		{
			//要删除的键是链表头
			if (last == 0)
			{
				//有下一个元素
				if (current->next)
				{
					Entry *next = current->next;
					del_key_before(current, expires_head->next);
					free(current->k);
					free(current->v);
					free(current);
					arr->data[index] = next;
					return 1;
				}
				//没下一个元素
				else
				{
					del_key_before(current, expires_head->next);
					free(current->k);
					free(current->v);
					current->k = 0;
					current->v = 0;
					return 1;
				}
			}
			//删除的不是表头
			else
			{
				//有下一个元素
				if (current->next)
				{
					Entry *next = current->next;
					last->next = current->next;
					del_key_before(current, expires_head->next);
					free(current->k);
					free(current->v);
					free(current);
					return 1;
				}
				//没有下一个元素
				else
				{
					del_key_before(current, expires_head->next);
					free(current->k);
					free(current->v);
					free(current);
					last->next = 0;
					return 1;
				}
			}
		}
		last = current;
	} while (current = current->next);
	return 0;
}

char *get_data(Array *arr, char *k)
{
	Entry *entry = exist_key(arr, k);
	int signal;
	//没有查询到
	if (entry == 0)
		return 0;
	signal = expire_if_needed(entry, expires_head->next);
	if (signal)
		return 0;
	return entry->v;
}