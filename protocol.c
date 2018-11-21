﻿#include<malloc.h>
#include<string.h>
#include"protocol.h"

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