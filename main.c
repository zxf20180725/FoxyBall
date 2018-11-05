#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include"hashmap.h"
#include"arraylib.h"


int main()
{
	//char *key = (char *)malloc(1024);
	//sprintf(key, "hello%d", 1);
	//free(key);


	char *key[] = {
		"key1", "狡猾的皮球", "狡猾的球球",
		"key4", "foxyball", "2018bf05a16eb3749b3e91397309035506e0f8ff",
		"b765a065d8e64dec67daa4506a7bbd1a9d6401d2", "卧槽", "冲突测试",
		"key1","key1","key1"
	};
	int len = sizeof(key) / sizeof(char*);
	int i = 0;

	char *ret = 0;

	Array *hash_table = new_hash_table(3);

	for (i = 0; i < len; i++)
	{
		char *k = (char *)malloc(1024);
		char *v = (char *)malloc(1024);

		sprintf(k,"key%d", i);
		sprintf(v,"值：%d", i);
		add_data(hash_table, k, v);
	}
	del_key(hash_table, "key10");
	//free_entry(hash_table->data[0]);

	for (i = 0; i < hash_table->n; i++)
	{
		Entry *entry = (Entry*)hash_table->data[i];
		if (entry->k != 0)
		{
			do
			{
				printf("index:%3d,value:%s,key:%s\n", i, entry->v, entry->k);
			} while (entry = entry->next);
		}
	}

	while (1)
	{
		char key[100];
		printf("---------------------------------\n");
		printf("请输入要查询的key:");
		gets(key);
		printf("\n");
		ret = get_data(hash_table, key);
		if (ret == 0)
			printf("没有查询到！\n");
		else
			printf("%s\n", ret);
	}

	ret = get_data(hash_table, "key1");
	if (ret == 0)
		printf("没有查询到！\n");
	else
		printf("查询结果：%s\n", ret);

	system("pause");
	return 0;
}