#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include"global.h"
#include"command_parser.h"

int main()
{
	int i = 0;
	char *cmd = (char *)malloc(4096);

	if (!init_foxy_ball())
		exit(0);

	while (1)
	{
		printf("FoxyBall>>>");
		gets(cmd);
		parse_cmd(cmd);
	}
	free(cmd);

	//char *cmd = (char *)malloc(4096);

	//gets(cmd);

	//FILE *fp = 0;

	//fp = fopen("./test.dat", "wb");

	//fwrite(cmd, (strlen(cmd)+1)*sizeof(char), 1, fp);

	//printf("%d\n", strlen(cmd));

	//fclose(fp);

	return 0;
}