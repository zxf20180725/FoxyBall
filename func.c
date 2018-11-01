#include<malloc.h>
#include <string.h>
#include"func.h"
#include"ArrayLib.h"

unsigned int times33(char *key)
{
    unsigned int hash = 5381;
    while(*key){
        hash += (hash << 5 ) + (*key++);
    }
    return (hash & 0x7FFFFFFF);
}

void init_entry(Entry *entry)
{
	entry->k=0;
	entry->v=0;
	entry->next=0;
}

Array *new_hash_table(int length)
{
	Array *arr=init_array(length);
	int i;
	for(i=0;i<arr->n;i++)
	{
		Entry *entry=new_entry();
		arr->data[i]=entry;
	}
	return arr;
}

Entry *new_entry()
{
	Entry *entry=(Entry*)malloc(sizeof(Entry));
	entry->k=0;
	entry->v=0;
	entry->next=0;
	return entry;
}

int hash(char *k,int length)
{
	int ret=times33(k)&(length-1);
	return ret;
}

int add_data(Array *arr,char *k,char *v)
{
	//计算index
	int index=hash(k,arr->n);
	//获取entry
	Entry* entry=(Entry*)arr->data[index];
	//hash冲突
	if(entry->k==0)
	{
		entry->k=k;
		entry->v=v;
	}
	else
	{
		while(1)
		{
			if (entry->next==0)
			{
				Entry *e_t=new_entry();
				e_t->k=k;
				e_t->v=v;
				e_t->next=0;
				entry->next=e_t;
				break;
			}
			else
			{
				entry=entry->next;
			}
		}
	}
	return 1;
}

char *get_data(Array *arr,char *k)
{
	int index=hash(k,arr->n);
	//获取entry
	Entry *entry=(Entry*)arr->data[index];
	//没有查询到
	if(entry->k==0)
		return 0;
	//链表查询
	do
	{
		if(strcmp(k,entry->k)==0)
			return entry->v;
	}while(entry=entry->next);
	//没查到
	return 0;
}