#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//校验接收到的数据是否合法（数据包是否以DATA_START开头）
int validate_pck(unsigned char *data, int length);

//4字节数据转换成int
int char_to_int(unsigned char *data);

//切割字节数组(返回值一定要free)
unsigned char *split_bytes(unsigned char *bytes, int start, int len);

//获取数据包长度
int get_pck_len(unsigned char *data);

#endif