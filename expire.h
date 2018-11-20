#ifndef  _EXPIRE_H_
#define  _EXPIRE_H_
#include"arraylib.h"
#include"hashmap.h"

//使用该结构时，需要头结点，头结点不包含数据
typedef struct expires
{
	struct expires *next;	//下一个
	struct expires *last;	//上一个
	Entry *entry;			//有过期时间的kv
	long expire;			//过期时间(过期时的时间戳)
}Expires;

//设置过期时间
int set_expire(Array *arr, Expires *expires_head, char *k, long time);

//获取key的剩余过期时间(返回0不会过期)
int get_expire(Array *arr, Expires *expires_head, char *k);

//获取当前时间戳
long get_timestamp();

//删除过期键 （惰性删除）
int expire_if_needed(Entry *dest_entry, Expires *expires);

//删除键之前清理expires表
int del_key_before(Entry *dest_entry, Expires *expires);


#endif