#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"hashmap.h"
#include"arraylib.h"
#include"expire.h"
#include"global.h"
#include"utils.h"

int main()
{
	int i = 0;
	char *cmd = (char *)malloc(1024);

	//char *cmd_text = "set abc 6666";
	//char **ret = split(cmd_text, " ");

	//printf("%d\n", get_split_length(ret));
	//for (int j = 0; ret[j] != 0; j++) {
	//	printf("%s\n", ret[j]);
	//}

	if (!init_foxy_ball())
		exit(0);

	for (i = 0; i < 2048; i++)
	{
		char *k = (char *)malloc(1024);
		char *v = (char *)malloc(1024);
		sprintf(k, "key%d", i);
		sprintf(v, "value%d", i);
		add_data(hash_table, k, v);
	}

	//for (i = 0; i < hash_table->n; i++)
	//{
	//	Entry *entry = (Entry*)hash_table->data[i];
	//	if (entry->k != 0)
	//	{
	//		do
	//		{
	//			printf("index:%3d,value:%s,key:%s\n", i, entry->v, entry->k);
	//		} while (entry = entry->next);
	//	}
	//}
	while (1)
	{
		printf("FoxyBall>>>");
		gets(cmd);
		printf("success!\n", cmd);
	}
	free(cmd);
	return 0;
}