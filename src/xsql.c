#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "/usr/include/mysql/mysql.h"
#include "../include/xsql.h"

MYSQL* sql_open(char* host, char* user, char* pwd, char* schema) 
{
    MYSQL* my_conn = mysql_init(NULL);
    if ( NULL == my_conn )
    {
        printf("%s: %d : error : %s\n",__FILE__,  __LINE__, mysql_error(my_conn));
        exit(1);
    }
    if ( NULL == mysql_real_connect(my_conn, host, user, pwd, schema, 6033, NULL, 0) )
    {
        printf("%s: %d : error : %s\n", __FILE__, __LINE__, mysql_error(my_conn));
        exit(1);
    }
    if ( 0 != mysql_set_character_set(my_conn, "utf8") )
    {
        printf("%s: %d : error : %s\n", __FILE__, __LINE__, mysql_error(my_conn));
        exit(1);
    }
    return my_conn;
}

MYSQL_RES* sql_real_query(MYSQL* my_conn, char* my_sql)
{
    if ( SQL_NO_QUERY_RESULT != mysql_real_query(my_conn, my_sql, strlen(my_sql)) )
    {
        printf("%s: %d : error : %s\n", __FILE__, __LINE__, mysql_error(my_conn));
        return NULL;
        // exit(1);
    }

    MYSQL_RES* res = mysql_store_result(my_conn);
    if (NULL == res )
    {
        printf("%s : %d : error : %s\n", __FILE__, __LINE__, mysql_error(my_conn));
        exit(1);
    }
    return res;
}

SQL_FIELD_NAME_FMT sql_field_fmt_str(MYSQL_RES* res)
{
    int cols = mysql_num_fields(res);
    // MYSQL_FIELD_OFFSET a = mysql_field_tell(res);
    // printf("Preparing to fetch %d fields. Cursor now offset to %d\n", cols, a);
    mysql_field_seek(res, 0);

    SQL_FIELD_NAME_FMT field_name_fmt = (char**)malloc(sizeof(SQL_FIELD_NAME_FMT)*cols);
    int idx = 0;
    MYSQL_FIELD* myfields;
    while (myfields = mysql_fetch_field(res))
    {
        field_name_fmt[idx] = (char*)malloc(sizeof(char)*8);
        memset(field_name_fmt[idx], 0, 8);
        int field_name_length = strlen(myfields->name)+2;
        sprintf(field_name_fmt[idx], "%%%ds", myfields->name_length>field_name_length?myfields->name_length:field_name_length);
        // printf("[%d]name length -->%d name format --> %s;", idx, myfields->name_length, field_name_fmt[idx]);
        idx++;
    }
    // reset cursor.
    mysql_field_seek(res, 0);
    return field_name_fmt;
}

#define OSMP_SEPARATOR_LINE(s, n) \
    memset(s, 45, n); s[n-1] = 0; \
    printf("%s\n", s);


int print_table_information(char* table_name) 
{
    char* host = "rm-bp18m16y76g8d8ww2fo.mysql.rds.aliyuncs.com";
    char* user = "pms_user";
    char* pwd = "pms_user@123";
    char* schema = "pms_db";
    char* sql;

    if (table_name != NULL)
    {
        sql = (char*)malloc(sizeof(char)*MAX_TABLE_NAME_LENGTH);
        memset(sql, 0, MAX_TABLE_NAME_LENGTH);
        sprintf(sql, "SELECT * FROM %s", table_name);
        printf("SQL in query is: %s;\n", sql);
    }
    else
    {
        printf("No table name, exit.\n");
        return 0;
    }

    MYSQL* my_conn = sql_open(host, user, pwd, schema);
    MYSQL_RES* my_res = sql_real_query(my_conn, sql);

    if (my_res == NULL)
    {
        return 100;
    }
    

    // Retrieve meta data information.
    int cols = mysql_num_fields(my_res);

    char* field_names = (char*)malloc(sizeof(char)*1024);
    memset(field_names, 0, 1024);
    
    MYSQL_FIELD* myfields;
    while (myfields = mysql_fetch_field(my_res))
    {
        // printf("%s %d\n", myfields->name, strlen(myfields->name));
        char* field_name = (char*)malloc(sizeof(char)*(strlen(myfields->name)+4));
        sprintf(field_name, "| %s ", myfields->name);
        // printf("%s %d\n", field_name, strlen(field_name));   
        strcat(field_names, field_name);
        // printf("%s", field_names);
        // printf("%s", myfields->name);
        // printf(" | ");
        free(field_name);
    }
    strcat(field_names, "|\n");
    int separate_length = strlen(field_names);
    char separate_line[separate_length];

    // Begin to print table
    OSMP_SEPARATOR_LINE(separate_line, separate_length);
    printf("%s", field_names);
    OSMP_SEPARATOR_LINE(separate_line, separate_length);


    // Retrieve cell value of each row.
    SQL_FIELD_NAME_FMT fmt = sql_field_fmt_str(my_res);
    MYSQL_ROW row;
    while ( row = mysql_fetch_row(my_res) )
    {
        for ( int i = 0; i < cols; i++ )
        {
            printf("|");
            row[i]?printf(fmt[i], row[i]):printf(" Null ");
        }
        printf("|\n");
    }
    OSMP_SEPARATOR_LINE(separate_line, separate_length);

    free(field_names);
    free(row);
    free(fmt);
    free(sql);
    free(myfields);

    mysql_free_result(my_res);
    mysql_close(my_conn);

    return 1;
}