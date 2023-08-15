#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
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
enum JSON_Type {
    JNull=0,
    JString=1,
    JNumber=2,
    JObject=3,
    JArray=4,
    JBool=5
};

typedef struct JSON_Object_st {
    enum JSON_Type j_type;
    char* j_key;
    char* s_value;
    int i_value;
    bool b_value;
    void* next;
} JSON_object;


int help()
{
    printf(MENU_MAIN);
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
        printf("OSMP CLI>>>");
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
            char* payload = "{\"code\":\"\",\"codeUuid\":\"\",\"loginName\":\"dongjin@utry.cn\",\"loginPwd\":\"123456\"}";
            http_response *resp = http_post("https://utmpapi.utry.cn/utmp-admin-api/session/login", NULL, payload);
            X_LOG_DEBUG("http_response [%d] is: %s", resp->size, resp->response);
            char *tokenVal = strstr(resp->response, "xaccessToken");
            tokenVal--;
            tokenVal = strtrim(tokenVal, '}');

            // Parsing json
            // +
            char* json_head = (char*)malloc(sizeof(char)*(resp->size+1));
            char* json = json_head;
            memset(json, 0, resp->size+1);
            memcpy(json, resp->response, resp->size+1);
            
            char** json_objects_head = (char**)calloc(sizeof(char*), MAX_OSMP_JSON_SIZE);
            char** json_objects = json_objects_head;

            *json_objects_head = json_head;
            // X_LOG_DEBUG(">>>>>>>>>>>>> %s", *json_objects_head);
            
            int a = 0;
            while(*json) {
                if(*json == '{') {
                    a++;
                    *json_objects = json;
                    json_objects++;
                } 
                else if (*json == '[')
                {
                    while (1)
                    {
                        json++;
                        if (*json != ']')
                        {
                            continue;
                        } else {
                            json++;
                            break;
                        }
                    }
                    X_LOG_DEBUG("exit [] branch.");
                    X_LOG_DEBUG(">>>>>>>>>>> %s", json);
                }
                // X_LOG_DEBUG(">>>>>>>>>>> %s", json);
                json++;
            }

            json_objects = json_objects_head;

            int abiii=0;
            while (*json_objects)
            {
                /* code */
                X_LOG_DEBUG("+++++++++++++++++++++++++++++++++++++++++++++++");
                X_LOG_DEBUG("%s", *json_objects);
                json_objects++;
                X_LOG_DEBUG("-----------------------------------------------");
                abiii++;
                if (abiii>10)
                {
                    break;
                    /* code */
                }
            }
            
            X_LOG_DEBUG("%d", a);

            // -

            char** p0 = (char**)malloc(sizeof(char**));
            *p0 = tokenVal;
            
            char* x_access_key = strsep(p0, ":");
            x_access_key = strsep(p0, ":");
            x_access_key = strtrim(x_access_key, '\"');
            X_LOG_DEBUG("xaccessToken is [%d]%s\n", strlen(x_access_key), x_access_key);

            // payload = "{\"orders\":[],\"pageNum\":1,\"pageSize\":1000,\"queryLike\":\"\",\"ssascription\":[\"2097164\",\"2097123\",\"2097173\",\"2097177\",\"2097179\",\"2097180\",\"2341059\",\"2920991\",\"3167706\"],\"status\":[]}";
            // http_response *resp1 = http_post("https://utmpapi.utry.cn//utmp-admin-api/project/page/query", x_access_key, payload);
            // printf("response [%d]%s\n", resp1->size, resp1->response);
            // free(resp1->response);
            // free(resp1);

            // payload = "{\"orders\":[],\"pageNum\":2,\"pageSize\":1000,\"queryLike\":\"\",\"ssascription\":[\"2097164\",\"2097123\",\"2097173\",\"2097177\",\"2097179\",\"2097180\",\"2341059\",\"2920991\",\"3167706\"],\"status\":[]}";
            // http_response *resp2 = http_post("https://utmpapi.utry.cn//utmp-admin-api/project/page/query", x_access_key, payload);
            // printf("response [%d]%s\n", resp2->size, resp2->response);
            // free(resp2->response);
            // free(resp2);
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
