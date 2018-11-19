#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_

#define DB_FILE_PATH "./db"

//保存hash_table到本地
int save_hash_table();

//从文件加载hash_table
int load_hash_table();

#endif