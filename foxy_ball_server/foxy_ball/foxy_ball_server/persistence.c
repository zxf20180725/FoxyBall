#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include"persistence.h"
#include"global.h"
#include"server.h"

int save_hash_table()
{
	FILE *file = 0;
	file = fopen(DB_FILE_PATH, "wb");
	console_log("开始持久化...");

	if (file == 0)
	{
		console_log("持久化失败！db文件可能被占用！");
		return 0;
	}

	//文件头4个字节代表array的长度
	fwrite(&hash_table->n, sizeof(int), 1, file);


	//遍历hash_table
	for (int i = 0; i < hash_table->n; i++)
	{
		Entry *head = (Entry *)hash_table->data[i];
		//此处为空，不写入
		if (head->k == 0)
			continue;
		//不为空，遍历entry链表
		do
		{
			//4字节key长度
			int key_len = (strlen(head->k) + 1)*sizeof(char);
			//4字节value长度
			int value_len = (strlen(head->v) + 1)*sizeof(char);
			fwrite(&key_len, sizeof(int), 1, file);				//写入key_len
			fwrite(head->k, key_len, 1, file);					//写入key
			fwrite(&value_len, sizeof(int), 1, file);			//写入value_len
			fwrite(head->v, value_len, 1, file);				//写入value
		} while (head = head->next);
	}
	fclose(file);

	console_log("持久化成功！");


	return 1;
}


int load_hash_table()
{
	FILE *file = 0;
	file = fopen(DB_FILE_PATH, "rb");
	if (file == 0)
		return 0;

	int array_length = 0;
	//读取hashtable长度
	fread(&array_length, sizeof(int), 1, file);
	hash_table = new_hash_table(array_length);	//创建hash_table
	//读取entry数据
	while (1)
	{
		int key_len = 0;
		int value_len = 0;
		//读取key_len
		if (!fread(&key_len, sizeof(int), 1, file))return 1;
		char *key = (char *)malloc(key_len);
		//读取key
		if (!fread(key, key_len, 1, file))return 0;
		//读取value_len
		if (!fread(&value_len, sizeof(int), 1, file))return 0;
		char *value = (char *)malloc(value_len);
		//读取value
		if (!fread(value, value_len, 1, file))return 0;
		//添加数据
		add_data(hash_table, key, value);
	}
	return 1;
}