#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<string.h>
#include"protocol.h"
#include"hashmap.h"
#include"global.h"
#include"server.h"

int validate_pck(unsigned char *data, int length)
{
	if (length < 8)
		return 0;

	if (data[0] != 0xAB || data[1] != 0xCD || data[2] != 0xEF || data[3] != 0xAB)
		return 0;

	return 1;
}

int char_to_int(unsigned char *data)
{
	return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
}

unsigned char *split_bytes(unsigned char *bytes, int start, int len)
{
	unsigned char *result = 0;
	result = (char *)malloc(sizeof(char)*len);
	memcpy(result, &bytes[start], len);
	return result;
}

int get_pck_len(unsigned char *data)
{
	unsigned char *pck_len_bytes = split_bytes(data, 4, 4);
	int pck_len = char_to_int(pck_len_bytes);
	free(pck_len_bytes);
	return pck_len;
}

int get_int32(unsigned char *data, int *len)
{
	unsigned char int32[4] = { data[0], data[1], data[2], data[3] };
	//改变源数据
	memcpy(data, &data[4], *len - 4);
	*len -= 4;
	return char_to_int(int32);
}

char * get_str(unsigned char *data, int *len)
{
	unsigned char int32[4] = { data[0], data[1], data[2], data[3] };
	int str_len = char_to_int(int32);
	char *str = (char *)malloc(sizeof(char)*str_len);
	memcpy(str, &data[4], str_len);	//截取字符串
	//改变源数据
	memcpy(data, &data[4 + str_len], *len - str_len);
	*len = *len - 4 - str_len;
	return str;
}

unsigned char *add_int32(unsigned char *data,int value,int *len)
{
	//数据包不为空
	if (data != 0)
	{
		unsigned char *new_pck = (char *)malloc(sizeof(char)*(*len + 4));		//分配一个新的空间
		//把之前数据复制过去
		memcpy(new_pck, data, *len);
		//追加int32
		memcpy(&new_pck[*len], &value, 4);
		*len += 4;
		//释放之前的data
		free(data);
		return new_pck;
	}
	else//数据包为空
	{
		unsigned char *new_pck = (char *)malloc(sizeof(char)*4);		//分配一个新的空间
		memcpy(new_pck, &value, 4);
		*len = 4;
		return new_pck;
	}
}

unsigned char *add_str(unsigned char *data, char *str, int *len)
{
	int str_len = strlen(str) + 1;	//str所占字节长度，包括\0

	if (data != 0)
	{
		unsigned char *new_pck = (char *)malloc(sizeof(char)*(*len + str_len + 4));		//分配一个新的空间
		//把之前数据复制过去
		memcpy(new_pck, data, *len);
		//追加字符串长度位
		memcpy(&new_pck[*len], &str_len, 4);
		//追加字符串
		memcpy(&new_pck[*len + 4], str, str_len);
		//释放之前的data
		free(data);
		*len = *len + 4 + str_len;
		return new_pck;
	}
	else//没有数据包
	{
		unsigned char *new_pck = (char *)malloc(sizeof(char)*(str_len + 4));		//分配一个新的空间
		//追加字符串长度位
		memcpy(new_pck, &str_len, 4);
		//追加字符串
		memcpy(&new_pck[4], str, str_len);
		*len = str_len + 4;
		return new_pck;
	}
}

unsigned char *add_head(unsigned char *data, int *len)
{
	unsigned char *new_pck = (char *)malloc(sizeof(char)*(*len + 4));		//分配一个新的空间
	//追加包长度位
	memcpy(new_pck, len, 4);
	//追加之前的数据
	memcpy(&new_pck[4], data, *len);
	//释放之前的数据
	free(data);
	*len += 4;
	return new_pck;
}

unsigned char *create_pck()
{
	unsigned char *pck = (char *)malloc(sizeof(char)* 4);
	return pck;
}

unsigned char *dispatch_data(unsigned char *data, int len,int *return_len)
{
	//获取协议名称
	char *protocol_name = get_str(data, &len);
	unsigned char *return_pck = 0;
	//添加kv
	if (strcmp(protocol_name, "set") == 0)
	{
		char *key = get_str(data, &len);
		char *value = get_str(data, &len);
		int ret = add_data(hash_table, key, value);
		return_pck = get_kv_result(ret,return_len);
		free(key);
		free(value);
	}
	else
	{
		console_log("接收到了一个不存在的协议！");
	}
	free(protocol_name);

	//返回要发送给客户端的数据包
	return return_pck;
}

unsigned char *get_kv_result(int ret,int *len)
{
	unsigned char *pck=add_str(0, "set_result", len);
	pck = add_int32(pck, ret, len);
	pck = add_head(pck, len);
	return pck;
}