#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"command_parser.h"
#include"utils.h"

char *help_text[22] = {
	"命令解析器，FoxyBall所有命令：",
	"",
	"权限：",
	"	登录：              login <username> <password>",
	"",
	"对kv操作：",
	"	添加一个key：       set <key> <value>",
	"	删除一个key：       del <key>",
	"	查询key的value：    get <key>",
	"	设置过期时间：      expire <key> <second>",
	"	查看剩余时间：      ttl <key>",
	"	判断key是否存在：   exists <key>",
	"	清空所有key：       flush",
	"",
	"持久化操作：",
	"	保存数据：          save",
	"",
	"其他：",
	"	帮助：              help",
	"	显示所有kv：        show",
	"	清屏：              cls",
	"	版本信息：          version"
};

void parse_cmd(char *cmd)
{
	char **sub_cmd = split(cmd, " ");		//切割命令
	int length = get_split_length(sub_cmd);	//命令长度
	if (length <= 0)
		return;

	//show命令
	if (strcmp(sub_cmd[0], "show") == 0 && length == 1)
	{
		show_all();
		free_split(sub_cmd);
		return;
	}

	//cls命令
	if (strcmp(sub_cmd[0], "cls") == 0 && length == 1)
	{
		system("cls");
		free_split(sub_cmd);
		return;
	}

	//help命令
	if (strcmp(sub_cmd[0], "help") == 0 && length == 1)
	{
		for (int len = 0; len < 22; len++)
		{
			printf("%s\n", help_text[len]);
		}
		free_split(sub_cmd);
		return;
	}

	//version命令
	if (strcmp(sub_cmd[0], "version") == 0 && length == 1)
	{
		printf("     FoxyBall（v%s，%s）\n     作者：%s\n", VERSION, UPDATE_TIME, AUTHOR);
		free_split(sub_cmd);
		return;
	}

	//set命令
	if (strcmp(sub_cmd[0], "set")==0 && length == 3)
	{
		int ret=add_data(hash_table,sub_cmd[1],sub_cmd[2]);
		free_split(sub_cmd);
		if (ret)
			printf("添加数据成功！\n");
		else
			printf("添加数据失败！请检查key是否已存在！\n");
		return;
	}

	//del命令
	if (strcmp(sub_cmd[0], "del") == 0 && length == 2)
	{
		int ret = del_key(hash_table, sub_cmd[1]);
		if (ret)
			printf("删除成功，key：%s\n",sub_cmd[1]);
		else
			printf("删除失败！请检查key是否存在！\n");
		free_split(sub_cmd);
		return;
	}

	printf("命令非法！\n");
	return;
}