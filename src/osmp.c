#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "../include/cJSON.h"
#include "../include/cJSON_Utils.h"
#include "/usr/include/mysql/mysql.h"
#include "../include/pmscore.h"
#include "../include/xsql.h"
#include "../include/xhttp.h"


#define MENU_MAIN "\
-----------------  OSMP utils -----------------\n\
Welcome to use OSMP utils. Any problem, please \n\
feel free to contact Administrator and send \n\
email to <martin.dong@139..com> for help. \n\
---------------- Function Menu ----------------\n\
1. Query and print table.\n\
2. Print linked list information.\n\
3. UTMP test.\n\
4. Run default test cases.\n\
Type 'q' or 'quit' to exit the program.\n\
------------------------------------------------\n"

// Please make your choice:"
enum json_type_e {
    JNull=0,
    JString=1,
    JNumber=2,
    JObject=3,
    JArray=4,
    JBool=5
};

typedef struct json_object_t {
    enum json_type_e j_type;
    char* j_key;
    void* j_value;
    void* next;
} JSON_object;

char* osmp_retrival_utmp_access_token() 
{
    char* payload = "{\"code\":\"\",\"codeUuid\":\"\",\"loginName\":\"dongjin@utry.cn\",\"loginPwd\":\"123456\"}";
    http_response *resp = http_post("https://utmpapi.utry.cn/utmp-admin-api/session/login", NULL, payload);
    //X_LOG_DEBUG("http_response [%d] is: %s", resp->size, resp->response);

    // Retrival xaccessToken
    char *x_access_token = NULL;
    cJSON *json = cJSON_Parse(resp->response);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            return NULL;
        }
    }
    cJSON *jdata = cJSON_GetObjectItemCaseSensitive(json, "data");
    //X_LOG_DEBUG("Parsed by cJSON: %s", cJSON_Print(jdata));
    cJSON *jaccessToken = cJSON_GetObjectItemCaseSensitive(jdata, "xaccessToken");
    if (cJSON_IsString(jaccessToken) && (jaccessToken->valuestring != NULL))
    {
        size_t len = strlen(jaccessToken->valuestring);
        x_access_token=(char*)malloc(sizeof(char*)*len);
        memcpy(x_access_token, jaccessToken->valuestring, len);
    }
    free(json);
    free(jdata);
    free(jaccessToken);
    return x_access_token;
}

int osmp_utmp_project_data_to_db(const http_response* resp)
{
    cJSON *json = NULL;
    cJSON *jdata = NULL;
    cJSON* jlists = NULL;
    cJSON* jlist = NULL;
    json = cJSON_Parse(resp->response);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return OSMP_ERR;
    }
    jdata = cJSON_GetObjectItemCaseSensitive(json, "data");
    jlists = cJSON_GetObjectItemCaseSensitive(jdata, "list");
    jlist = NULL;
    cJSON_ArrayForEach(jlist, jlists)
    {
        cJSON *id = cJSON_GetObjectItemCaseSensitive(jlist, "id");
        cJSON *projectnum = cJSON_GetObjectItemCaseSensitive(jlist, "projectnum");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(jlist, "name");
        cJSON *status = cJSON_GetObjectItemCaseSensitive(jlist, "status");
        cJSON *contractamount = cJSON_GetObjectItemCaseSensitive(jlist, "contractamount");
        cJSON *income = cJSON_GetObjectItemCaseSensitive(jlist, "income");
        cJSON *actualpayment = cJSON_GetObjectItemCaseSensitive(jlist, "actualpayment");
        cJSON *managerName = cJSON_GetObjectItemCaseSensitive(jlist, "managerName");
        cJSON *nextmilestone = cJSON_GetObjectItemCaseSensitive(jlist, "nextmilestone");
        cJSON *plancomplettime = cJSON_GetObjectItemCaseSensitive(jlist, "plancomplettime");
        cJSON *countdown = cJSON_GetObjectItemCaseSensitive(jlist, "countdown");

        char* sql = (char*)malloc(sizeof(char*)*10240);
        sprintf(sql, 
            "insert into utmp_projectpageqrydetailinformation(id, projectnum, name ,status, contractamount, income, actualpayment, manager_name, nextmilestone, plancomplettime, countdown) values(%d, '%s', '%s', '%s', %d,  %d,  %d, '%s', '%s', '%s', '%s');", 
            id->valueint, projectnum->valuestring, name->valuestring, status->valuestring, 
            contractamount->valueint, income->valueint, actualpayment->valueint, managerName->valuestring, 
            nextmilestone->valuestring, plancomplettime->valuestring, countdown->valuestring);
        //X_LOG_DEBUG("SQL string is:\n%s", sql);
        
        // Handing mysql error 1062: Duplicate entry '' for key 'PRIMARY'
        if (1062 == osmp_mysql_real_query(sql))
        {
            memset(sql, 0, sizeof(char*)*10240);
            sprintf(sql, 
                "UPDATE utmp_projectpageqrydetailinformation set status='%s', contractamount=%d, income=%d, actualpayment=%d, manager_name='%s', nextmilestone='%s', plancomplettime='%s', countdown='%s' WHERE id=%d;", 
                status->valuestring, contractamount->valueint, income->valueint, actualpayment->valueint, 
                managerName->valuestring, nextmilestone->valuestring, plancomplettime->valuestring, 
                countdown->valuestring, id->valueint);
            X_LOG_DEBUG("SQL string is:\n%s", sql);
            osmp_mysql_real_query(sql);
        }
    }
}

int help()
{
    return printf(MENU_MAIN);
}

int main(int argc, char** argv) {
    
    
    if (argc > 1)
    {
        printf("Total argc is %d. No parameter is needed, exit.\n", argc);
        exit(0);
    }

    help();

    char* in_selection = (char*)malloc(sizeof(char)*1);
    while(1) 
    {
        printf("\nOSMP CLI>>> ");
        char* in_selection = (char*)malloc(sizeof(char)*1);
        scanf("%s", in_selection);

        /* Select which to to next. */
        if (*in_selection == '1')
        {
            printf("Please input the schema name:");
            char* schema_name = (char*)calloc(MAX_TABLE_NAME_LENGTH, sizeof(char));
            scanf("%s", schema_name);
            while (schema_name)
            {
                if(print_table_information(schema_name) == 100) {
                    // return to the command line.
                    break;
                }
                printf("Please input the schema name:");
                scanf("%s", schema_name);
                if (!strcmp(schema_name, "q"))
                {
                    printf("User quit. Osmp terminated normally.\n");
                    break;
                }
            }
            free(schema_name);
        } 
        else if (*in_selection == '2') 
        {
            welcomeMessage();
        } 
        else if (*in_selection == '3') 
        {
            char* payload = NULL;
            // Retrival xaccessToken
            char *x_access_token = osmp_retrival_utmp_access_token();

            payload = "{\"orders\":[],\"pageNum\":1,\"pageSize\":1000,\"queryLike\":\"\",\"ssascription\":[\"2097164\",\"2097123\",\"2097173\",\"2097177\",\"2097179\",\"2097180\",\"2341059\",\"2920991\",\"3167706\"],\"status\":[]}";;
            http_response *resp1 = http_post("https://utmpapi.utry.cn//utmp-admin-api/project/page/query", x_access_token, payload);
            osmp_utmp_project_data_to_db(resp1);
            //X_LOG_DEBUG("response [%d]%s\n", resp1->size, resp1->response);
            free(resp1->response);
            free(resp1);

            // payload = "{\"orders\":[],\"pageNum\":2,\"pageSize\":1000,\"queryLike\":\"\",\"ssascription\":[\"2097164\",\"2097123\",\"2097173\",\"2097177\",\"2097179\",\"2097180\",\"2341059\",\"2920991\",\"3167706\"],\"status\":[]}";
            // http_response *resp2 = http_post("https://utmpapi.utry.cn//utmp-admin-api/project/page/query", x_access_token, payload);
            // printf("response [%d]%s\n", resp2->size, resp2->response);
            // free(resp2->response);
            // free(resp2);
            payload = "{\"orders\":[],\"pageNum\":2,\"pageSize\":1000,\"queryLike\":\"\",\"ssascription\":[\"2097164\",\"2097123\",\"2097173\",\"2097177\",\"2097179\",\"2097180\",\"2341059\",\"2920991\",\"3167706\"],\"status\":[]}";;
            http_response *resp2 = http_post("https://utmpapi.utry.cn//utmp-admin-api/project/page/query", x_access_token, payload);
            osmp_utmp_project_data_to_db(resp2);
            //X_LOG_DEBUG("response [%d]%s\n", resp1->size, resp1->response);
            free(resp2->response);
            free(resp2);
        }
        else if (*in_selection == '4') 
        {
            char* newchar = (char*)malloc(sizeof(char)*MAX_PASSWORD);
            int len = GenerateMailSecurityInfo(newchar);
            printf("Generated passoword is %s [%d].\n\n", newchar, len);
        }
        else if (*in_selection == '?') 
        {
            help();
        }
        else 
        {
            printf("Unrecognised command, exiti the shell, bye!\n");
            exit(0);
        }
    }

}
