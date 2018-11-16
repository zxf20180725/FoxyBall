#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"command_parser.h"
#include"utils.h"

void parse_cmd(char *cmd)
{
	char **sub_cmd = split(cmd, " ");		//�и�����
	int length = get_split_length(sub_cmd);	//�����
	if (length <= 0)
		return;

	//show����
	if (strcmp(sub_cmd[0], "show") == 0 && length == 1)
	{
		show_all();
		free_split(sub_cmd);
		return;
	}

	//cls����
	if (strcmp(sub_cmd[0], "cls") == 0 && length == 1)
	{
		system("cls");
		free_split(sub_cmd);
		return;
	}

	//set����
	if (strcmp(sub_cmd[0], "set")==0 && length == 3)
	{
		int ret=add_data(hash_table,sub_cmd[1],sub_cmd[2]);
		free_split(sub_cmd);
		if (ret)
			printf("������ݳɹ���\n");
		else
			printf("�������ʧ�ܣ�����key�Ƿ��Ѵ��ڣ�\n");
		return;
	}

	printf("����Ƿ���\n");
	return;
}