#include<string.h>
#include<stdio.h>
#include"command_parser.h"
#include"utils.h"

int parse_cmd(char *cmd)
{
	char **sub_cmd = split(cmd, " ");		//切割命令
	int length = get_split_length(sub_cmd);	//命令长度
	if (length <= 0)
		return 0;

	//set命令
	if (strcmp(sub_cmd[0], "set") && length == 3)
	{
		add_data(hash_table,sub_cmd[1],sub_cmd[2]);
		free_split(sub_cmd);
		printf("添加数据成功！\n");
		return 1;
	}

	printf("命令非法！\n");
	return 0;
}