#ifndef __SLQ_H__
#define __SLQ_H__

#include "/usr/include/mysql/mysql.h"
#include "../include/xdefine.h"

#define MAX_TABLE_NAME_LENGTH 256
#define SQL_NO_QUERY_RESULT 0
typedef char** SQL_FIELD_NAME_FMT;

MYSQL* sql_open(char* host, char* user, char* pwd, char* schema);
MYSQL_RES* sql_real_query(MYSQL* my_conn, char* my_sql);

SQL_FIELD_NAME_FMT sql_field_fmt_str(MYSQL_RES* res);

int print_table_information(char* table_name);

#endif