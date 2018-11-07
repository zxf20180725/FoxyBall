#ifndef _GLOBAL_H
#define _GLOBAL_H
#include"arraylib.h"
#include"expire.h"

extern Array *hash_table;		//数据库核心结构
extern Expires *expires_head;	//过期表

//初始化数据库
int init_foxy_ball();
#endif
