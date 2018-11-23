#ifndef _SERVER_H_
#define _SERVER_H_

#define MAX_CLIENT_NUMS 256			//最大连接数
#define PORT 45007					//端口号
#define DATA_START 0xABCDEFAB		//数据包头部校验
#define	BUFFER_SIZE 4096			//缓冲区大小4kb

//控制台输出
void console_log(char *str);

//控制台输出时间头
void console_log_head();

//输出爱心logo
void start_logo();


#endif