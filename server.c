#include<stdio.h>
#include"server.h"
#include"protocol.h"

static int client_nums = 0;			//当前连接服务器的客户端的个数

#if ((defined _WIN32) || (defined _WIN64))		

#include <winsock2.h>
#include <windows.h>  
#pragma comment(lib,"ws2_32.lib")
typedef struct client
{
	SOCKET socket;						//客户端socket
	struct sockaddr_in socket_info;		//socket的基本信息（ip、port之类的）
	int is_run;							//该client是否正在被使用
	HANDLE  h;							//为这个client服务的线程句柄

	int remain_bytes;					//需要继续在缓冲区取多少字节的数据
	unsigned char *data;				//单次封包数据
	int data_length;					//data的字节数
}Client;

static Client client_list[MAX_CLIENT_NUMS] = { 0 };		//客户端列表
static SOCKET server_socket;							//用于接收新client的socket
static struct sockaddr_in server_info;					//用于存储本地创建socket的基本信息

static void analysis(char* data, int datal, Client *client);		//解析数据
DWORD WINAPI client_handler(LPVOID lpParameter);					//处理client
void clear_client(Client *client);									//清理client

//启动server
int run_server()
{
	printf("FoxyBall服务端启动中......\n");
	WORD socket_version = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(socket_version, &wsadata) != 0)
	{
		return 0;
	}

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//创建socket 并判断是否创建成功
	if (server_socket == INVALID_SOCKET)
	{
		printf("服务端创建socket失败！\n");
		return 0;
	}

	server_info.sin_family = AF_INET;					//定义协议族为IPV4
	server_info.sin_port = htons(PORT);					//规定端口号
	server_info.sin_addr.S_un.S_addr = INADDR_ANY;		//在0.0.0.0启动服务

	if (bind(server_socket, (LPSOCKADDR)&server_info, sizeof(server_info)) == SOCKET_ERROR)//绑定
	{
		printf("端口绑定异常，请检查%d端口是否被占用！\n", PORT);
		return 0;
	}

	if (listen(server_socket, 5) == SOCKET_ERROR)		//监听
	{
		printf("socket监听失败！\n");
		return 0;
	}

	printf("FoxyBall服务启动成功！ 端口：%d\n", PORT);

	while (1)
	{
		SOCKET client_socket;			//客户端（远程）的socket
		struct sockaddr_in client_info; //用于存储已连接的客户端的socket基本信息
		int c_sin_len;					//函数accept的第三个参数，c_sin的大小。

		c_sin_len = sizeof(client_info);

		//accept会阻塞线程
		client_socket = accept(server_socket, (SOCKADDR *)&client_info, &c_sin_len);

		if (client_socket == INVALID_SOCKET)
		{
			//printf("accept error\n");
			continue; //继续等待下一次连接
		}

		if (client_nums + 1 > MAX_CLIENT_NUMS)
		{
			//send(client_socket, "连接超限制，您已被断开 \n", strlen("连接超限制，您已被断开 \n"), 0);
			printf("客户端IP：%s PORT：%d连接被拒绝，因为当前连接数已达上限！\n", inet_ntoa(client_info.sin_addr), client_info.sin_port);
			closesocket(client_socket);
			continue;
		}

		int j = 0;
		for (j = 0; j < MAX_CLIENT_NUMS; j++)
		{
			if (client_list[j].is_run == 0)
			{
				client_list[j].is_run = 1;
				client_list[j].socket = client_socket;
				client_list[j].remain_bytes = -1;
				client_list[j].data = 0;
				client_list[j].data_length = -1;
				memcpy(&(client_list[j].socket_info), &client_info, sizeof(client_info));
				if (client_list[j].h)
				{
					CloseHandle(client_list[j].h);
				}
				client_list[j].h = CreateThread(NULL, 0, client_handler, &(client_list[j]), 0, NULL);
				client_nums++;
				break;
			}
		}
	}
}

static void analysis(unsigned char* data, int length, Client *client)
{
	printf("客户端（%s:%d）发来数据:%s  数据长度:%d\n", inet_ntoa(client->socket_info.sin_addr), client->socket_info.sin_port, data, length);


	//根据client的数据来判断这次是从头拆包还是接收剩余数据
	if (client->remain_bytes==-1)	//从头拆包
	{
		//校验(校验标识符4字节，包长度4字节)
		if (length < 8)
			return;
		//获取数据包长度
		//判断 （8+数据包长度）<= length 就是单个数据包小于等于缓冲区的情况
		//否则 就是单个数据包大于缓冲区的情况
	}
	else//接收剩余数据
	{
			
	}

}

DWORD WINAPI client_handler(LPVOID lpParameter)
{
	unsigned char revData[BUFFER_SIZE];		//缓冲区
	int  ret;								//recv函数的返回值

	Client *client = (Client *)lpParameter;

	printf("有新客户端连入服务器 , IP = %s PORT = %d \n", inet_ntoa(client->socket_info.sin_addr), client->socket_info.sin_port);

	printf("最多可连接%d个客户端，当前已连接%d个客户端\n", MAX_CLIENT_NUMS, client_nums);

	send(client->socket, "hello i am server \n", strlen("hello i am server \n"), 0);

	while (1)
	{
		//接收来自 这个客户端的消息
		ret = recv(client->socket, revData, BUFFER_SIZE, 0);

		if (ret > 0)
		{
			analysis(revData, ret, client);
		}
		else if (ret == 0)
		{
			//当ret == 0 说明客户端已断开连接。这里我们直接跳出循环去等待下次连接即可。
			printf("客户端断开连接, IP = %s PORT = %d \n", inet_ntoa(client->socket_info.sin_addr), client->socket_info.sin_port);
			closesocket(client->socket);
			break;
		}
		else//ret < 0
		{
			//当ret < 0 说明出现了异常 例如阻塞状态解除，或者读取数据时出现指针错误等。
			//所以我们这里要主动断开和客户端的链接，然后跳出循环去等待新的连接。
			printf("在与客户端通信是发生异常 IP = %s PORT = %d \n", inet_ntoa(client->socket_info.sin_addr), client->socket_info.sin_port);
			closesocket(client->socket);
			break;
		}
	}
	clear_client(client);
	client_nums--;
	printf("最多可连接%d个客户端，当前已连接%d个客户端\n", MAX_CLIENT_NUMS, client_nums);
	return;
}

void clear_client(Client *client)
{
	if (client->data != 0)
		free(client->data);
	client->data = 0;
	client->data_length = -1;
	client->remain_bytes = -1;
	client->is_run = 0;
}

#else



#endif

int main(int argc, char* argv[])
{
	//if (run_server() == 0)
	//	return 0;

	unsigned char data[] = { 0x00, 0x04, 0x00, 0x00 };

	
	printf("%d\n", char_to_int(data));


	return 0;
}