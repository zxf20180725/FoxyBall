#include<time.h>
#include<malloc.h>
#include<string.h>
#include"hashmap.h"
#include"expire.h"
#include"global.h"

long get_timestamp()
{
	time_t t;
	long ret;
	ret = time(&t);
	return ret;
}

int set_expire(Array *arr, Expires *expires_head, char *k, long time)
{
	Entry *dest_entry = exist_key(arr, k);
	//key不存在
	if (dest_entry == 0)
		return 0;
	if (time < 0)
		time = 0;
	while (1)
	{
		//如果已存在这个过期键，直接更新过期时间
		if (expires_head->entry == dest_entry)
		{
			expires_head->expire = get_timestamp() + time;
			return 1;
		}

		if (expires_head->next == 0)
		{
			Expires *expire = (Expires *)malloc(sizeof(Expires));
			expire->last = expires_head;
			expire->next = 0;
			expire->entry = dest_entry;
			expire->expire = get_timestamp() + time;
			expires_head->next = expire;
			return 1;
		}
		else
		{
			expires_head = expires_head->next;
		}
	}
	return 0;
}

long get_expire(Array *arr, Expires *expires_head, char *k)
{
	if (expires_head->next == 0)
		return 0;

	expires_head = expires_head->next;

	do
	{
		if (strcmp(expires_head->entry->k, k) == 0)
		{
			long ret = expires_head->expire - get_timestamp();
			if (ret < 0)
				return 0;
			else
				return ret;
		}
	} while (expires_head = expires_head->next);

	return 0;
}

int expire_if_needed(Entry *dest_entry, Expires *expires)
{
	if (expires == 0)
		return 0;

	do
	{
		//找到了需要删除的entry
		if (dest_entry == expires->entry)
		{
			//判断是否过期
			if (expires->expire < get_timestamp())
			{
				//没有下一个节点
				if (expires->next == 0)
				{
					//在hash_table中删除
					del_key(hash_table, dest_entry->k);
					//expires->last->next = 0;
					//free(expires);
					return 1;
				}
				//有下一个节点
				else
				{
					//在hash_table中删除
					del_key(hash_table, dest_entry->k);
					//expires->last->next = expires->next;
					//free(expires);
					return 1;
				}
			}
		}
	} while (expires = expires->next);
	return 0;
}

int del_key_before(Entry *dest_entry, Expires *expires)
{
	if (expires == 0)
		return 0;

	do
	{
		//找到了需要删除的entry
		if (dest_entry == expires->entry)
		{
			//删除当前节点
			if (expires->next == 0)		//没有下一个节点
			{
				expires->last->next = 0;
				free(expires);
				return 1;
			}
			//有下一个节点
			else
			{
				expires->last->next = expires->next;
				free(expires);
				return 1;
			}
		}
	} while (expires = expires->next);
	return 0;
}