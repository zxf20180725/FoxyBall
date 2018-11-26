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

//向数据包添加int32，返回新的数据包,返回值一定要free
unsigned char *add_int32(unsigned char *data, int value, int *len);

//向数据包添加str，返回新的数据包,返回值一定要free
unsigned char *add_str(unsigned char *data, char *str, int *len);

//给数据包添加包头，返回新的数据包,返回值一定要free
unsigned char *add_head(unsigned char *data,int *len);

//创建4字节空间(作为数据包基础)
unsigned char *create_pck();

//数据包分发
unsigned char *dispatch_data(unsigned char *data, int len, int *return_len);

/*************组装相关数据包函数*************/

//结果,ret=0失败,ret=1成功,len为返回的数据包长度
unsigned char *set_result(char* protocol_name, int ret, int *len);

//get命令结果
unsigned char *get_data_result(char *ret,int *len);

//ttl命令结果
unsigned char *ttl_result(int ret, int *len);


/********************************************/
#endif