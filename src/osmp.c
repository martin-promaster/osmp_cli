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
            // X_LOG_DEBUG("http_response [%d] is: %s\n", resp->size, resp->response);
            char *tokenVal = strstr(resp->response, "xaccessToken");
            tokenVal--;
            tokenVal = strtrim(tokenVal, '}');

            char** p0 = (char**)malloc(sizeof(char**));
            *p0 = tokenVal;
            
            char* x_access_key = strsep(p0, ":");
            x_access_key = strsep(p0, ":");
            x_access_key = strtrim(x_access_key, '\"');
            X_LOG_DEBUG("xaccessToken is [%d]%s\n", strlen(x_access_key), x_access_key);

            payload = "{\"orders\":[],\"pageNum\":1,\"pageSize\":1000,\"queryLike\":\"\",\"ssascription\":[\"2097164\",\"2097123\",\"2097173\",\"2097177\",\"2097179\",\"2097180\",\"2341059\",\"2920991\",\"3167706\"],\"status\":[]}";
            http_response *resp1 = http_post("https://utmpapi.utry.cn//utmp-admin-api/project/page/query", x_access_key, payload);
            printf("response [%d]%s\n", resp1->size, resp1->response);
            free(resp1->response);
            free(resp1);

            payload = "{\"orders\":[],\"pageNum\":2,\"pageSize\":1000,\"queryLike\":\"\",\"ssascription\":[\"2097164\",\"2097123\",\"2097173\",\"2097177\",\"2097179\",\"2097180\",\"2341059\",\"2920991\",\"3167706\"],\"status\":[]}";
            http_response *resp2 = http_post("https://utmpapi.utry.cn//utmp-admin-api/project/page/query", x_access_key, payload);
            printf("response [%d]%s\n", resp2->size, resp2->response);
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
