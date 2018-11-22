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

//在数据包中取int32(会改变传入的data和len)
int get_int32(unsigned char *data, int *len);

//在数据包中取str(会改变传入的data和len),返回值一定要free
char * get_str(unsigned char *data, int *len);

//数据包分发(返回1正常 0不正常)
int dispatch_data(unsigned char *data, int len);

#endif