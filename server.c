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

	int remain_bytes;					//需要继续在缓冲区取多少字节的数据(0代表当前有一个完整的data，并且还没处理；-1代表当前数据包为空；)
	unsigned char *data;				//单次封包数据
	int data_length;					//data的字节数
}Client;

static Client client_list[MAX_CLIENT_NUMS] = { 0 };		//客户端列表
static SOCKET server_socket;							//用于接收新client的socket
static struct sockaddr_in server_info;					//用于存储本地创建socket的基本信息

static void analysis(char* data, int datal, Client *client);							//解析数据
DWORD WINAPI client_handler(LPVOID lpParameter);										//处理client
void clear_client(Client *client);														//清理client
void set_client_pck(Client *client, unsigned char *pck, int len);						//给client设置完整数据包
void set_client_lack_pck(Client *client, unsigned char *pck, int size, int len);		//设置不完整的数据包
void append_client_lack_pck(Client *client, unsigned char *pck, int len);				//给数据包追加数据
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

static void analysis(unsigned char *data, int length, Client *client)
{
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	//TODO:该处理粘包流程有严重bug，当包头被缓冲区分割时导致，等以后智商有所提高时再修复
	while (length != 0)
	{
		//根据client的数据来判断这次是从头拆包还是接收剩余数据
		if (client->remain_bytes == -1)	//从头拆包
		{
			//校验(校验标识符4字节，包长度4字节)
			if (!validate_pck(data, length))
				return;

			//获取数据包长度
			int pck_len = get_pck_len(data);

			//判断 （8+数据包长度）<= length 就是单个数据包小于等于缓冲区的情况
			if ((8 + pck_len) <= length)
			{
				//取数据包数据，把data中剩余数据前移
				unsigned char *pck_bytes = split_bytes(data, 8, pck_len);
				int remain_len = length - 8 - pck_len;		//数据包剩余长度
				if (remain_len > 0)
					memcpy(data, &data[8 + pck_len], remain_len);
				length = remain_len;
				//把一个完整的数据包给client
				set_client_pck(client, pck_bytes, pck_len);
			}
			else//否则 就是单个数据包大于缓冲区的情况
			{
				unsigned char *pck_bytes = split_bytes(data, 8, length - 8);

				//把一个不完整的数据包给client
				set_client_lack_pck(client, pck_bytes, pck_len, length - 8);

				length = 0;
			}
		}
		else if (client->remain_bytes > 0)//接收剩余数据
		{
			//剩余数据在缓冲区范围内
			if (client->remain_bytes <= length)
			{
				//取剩余的数据
				unsigned char *remain_bytes = split_bytes(data, 0, client->remain_bytes);
				//合并到client中去
				append_client_lack_pck(client, remain_bytes, client->remain_bytes);
				//数据前移
				int remain_len = length - (client->remain_bytes);
				if (remain_len > 0)
					memcpy(data, &data[client->remain_bytes], remain_len);
				length = remain_len;
			}
			else//剩余数据大于缓冲区范围
			{
				//取整个缓冲区的数据合并到client中
				unsigned char *remain_bytes = split_bytes(data, 0, length);
				append_client_lack_pck(client, remain_bytes, length);
				length = 0;
			}
		}

		//解析client中的数据包
		if (client->remain_bytes == 0)
		{
			//printf("开头：%x\n", client->data[0]);
			//printf("结尾：%x\n", client->data[client->data_length-1]);
			//printf("长度：%d\n", client->data_length);
			//free(client->data);
			//client->data = 0;
			//client->data_length = -1;
			//client->remain_bytes = -1;
			int length = client->data_length;
			unsigned char * data = client->data;
			printf("%d\n", get_int32(data, &length));
			printf("%d\n", get_int32(data, &length));
			printf("%s\n", get_str(data, &length));
			printf("%s\n", get_str(data, &length));
			printf("%d\n", get_int32(data, &length));
		}
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

void set_client_pck(Client *client, unsigned char *pck, int len)
{
	client->data = pck;
	client->data_length = len;
	client->remain_bytes = 0;
}

void set_client_lack_pck(Client *client, unsigned char *pck, int size, int len)
{
	client->data = pck;
	client->data_length = size;
	client->remain_bytes = size - len;
}

void append_client_lack_pck(Client *client, unsigned char *pck, int len)
{
	int data_length = client->data_length;
	int remain_length = client->remain_bytes;
	//分配空间为之前的长度+本次补丁的长度
	int pck_size = sizeof(char)*(data_length - remain_length + len);
	unsigned char *patch_pck = (char *)malloc(pck_size);
	//复制之前的数据进来
	memcpy(patch_pck, client->data, data_length - remain_length);
	//合并本次数据
	memcpy(&patch_pck[data_length - remain_length], pck, len);
	//释放之前数据
	free(client->data);
	free(pck);
	client->data = patch_pck;
	client->remain_bytes -= len;
}

/*
void append_client_lack_pck(Client *client, unsigned char *pck, int len)
{
int data_length = client->data_length;
int remain_length = client->remain_bytes;
//分配空间为之前的长度+本次补丁的长度
int pck_size = sizeof(char)*(data_length - remain_length + len);
unsigned char *patch_pck = (char *)malloc(pck_size);
for (int i = 0; i < pck_size; i++)
{
if (i < data_length - remain_length)
patch_pck[i] = client->data[i];
else
patch_pck[i] = pck[i - data_length + remain_length];
}
//释放之前数据
free(client->data);
free(pck);
client->data = patch_pck;
client->remain_bytes -= len;
}
*/

#else



#endif

int main(int argc, char* argv[])
{
	if (run_server() == 0)
		return 0;
	return 0;
}