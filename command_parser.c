#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"command_parser.h"
#include"utils.h"

char *help_text[22] = {
	"�����������FoxyBall�������",
	"",
	"Ȩ�ޣ�",
	"	��¼��              login <username> <password>",
	"",
	"��kv������",
	"	���һ��key��       set <key> <value>",
	"	ɾ��һ��key��       del <key>",
	"	��ѯkey��value��    get <key>",
	"	���ù���ʱ�䣺      expire <key> <second>",
	"	�鿴ʣ��ʱ�䣺      ttl <key>",
	"	�ж�key�Ƿ���ڣ�   exists <key>",
	"	�������key��       flush",
	"",
	"�־û�������",
	"	�������ݣ�          save",
	"",
	"������",
	"	������              help",
	"	��ʾ����kv��        show",
	"	������              cls",
	"	�汾��Ϣ��          version"
};

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

	//help����
	if (strcmp(sub_cmd[0], "help") == 0 && length == 1)
	{
		for (int len = 0; len < 22; len++)
		{
			printf("%s\n", help_text[len]);
		}
		free_split(sub_cmd);
		return;
	}

	//version����
	if (strcmp(sub_cmd[0], "version") == 0 && length == 1)
	{
		printf("     FoxyBall��v%s��%s��\n     ���ߣ�%s\n", VERSION, UPDATE_TIME, AUTHOR);
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

	//del����
	if (strcmp(sub_cmd[0], "del") == 0 && length == 2)
	{
		int ret = del_key(hash_table, sub_cmd[1]);
		if (ret)
			printf("ɾ���ɹ���key��%s\n",sub_cmd[1]);
		else
			printf("ɾ��ʧ�ܣ�����key�Ƿ���ڣ�\n");
		free_split(sub_cmd);
		return;
	}

	printf("����Ƿ���\n");
	return;
}