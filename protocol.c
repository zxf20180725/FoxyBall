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