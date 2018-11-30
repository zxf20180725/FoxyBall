#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include"arraylib.h"
#include"expire.h"

#define VERSION "0.5.1"
#define UPDATE_TIME "2018年11月29日16:14:44"
#define AUTHOR "狡猾的皮球"

extern Array *hash_table;				//数据库核心结构
extern Expires *expires_head;			//过期表
extern unsigned long memory_amount;		//内存使用量(单位字节)


//初始化数据库
int init_foxy_ball();
#endif
