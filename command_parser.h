#ifndef  _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_
#include"hashmap.h"
#include"arraylib.h"
#include"expire.h"
#include"global.h"
/*
	命令解析器，FoxyBall所有命令：
	
	权限：
		登录：				login <username> <password>

	对kv操作：
		添加一个key：		set <key> <value>		
		删除一个key：		del <key>
		查询key的value：	get <key>
		设置过期时间：		expire <key> <second>
		查看剩余时间：		ttl <key>
		判断key是否存在：	exists <key>
		清空所有key：		flush

	持久化操作：
		保存数据：			save

	其他：
		帮助：				help <cmd>
		显示所有kv			show
*/

//解析命令
void parse_cmd(char *cmd);

#endif