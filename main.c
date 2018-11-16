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
#include"command_parser.h"

int main()
{
	int i = 0;
	char *cmd = (char *)malloc(1024);
	char *test = "set aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa 12313213213213213213213213213213213213213213212313213132132132";
	char **ret=split(test, " ");

	if (!init_foxy_ball())
		exit(0);

	while (1)
	{
		printf("FoxyBall>>>");
		gets(cmd);
		parse_cmd(cmd);
	}
	free(cmd);
	return 0;
}