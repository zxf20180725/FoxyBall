#include<string.h>
#include<stdio.h>
#include"command_parser.h"
#include"utils.h"

int parse_cmd(char *cmd)
{
	char **sub_cmd = split(cmd, " ");		//�и�����
	int length = get_split_length(sub_cmd);	//�����
	if (length <= 0)
		return 0;

	//set����
	if (strcmp(sub_cmd[0], "set") && length == 3)
	{
		add_data(hash_table,sub_cmd[1],sub_cmd[2]);
		free_split(sub_cmd);
		printf("������ݳɹ���\n");
		return 1;
	}

	printf("����Ƿ���\n");
	return 0;
}