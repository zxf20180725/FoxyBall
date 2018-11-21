#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//校验接收到的数据是否合法（数据包是否以DATA_START开头）
int validate_pck(unsigned char *data, int length);

//4字节数据转换成int
int char_to_int(unsigned char *data);

#endif