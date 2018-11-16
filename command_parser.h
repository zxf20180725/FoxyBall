#ifndef  _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_
#include"hashmap.h"
#include"arraylib.h"
#include"expire.h"
#include"global.h"
/*
	�����������FoxyBall�������
	
	Ȩ�ޣ�
		��¼��				login <username> <password>

	��kv������
		���һ��key��		set <key> <value>		
		ɾ��һ��key��		del <key>
		��ѯkey��value��	get <key>
		���ù���ʱ�䣺		expire <key> <second>
		�鿴ʣ��ʱ�䣺		ttl <key>
		�ж�key�Ƿ���ڣ�	exists <key>
		�������key��		flush

	�־û�������
		�������ݣ�			save

	������
		������				help <cmd>
		��ʾ����kv			show
*/

//��������
void parse_cmd(char *cmd);

#endif