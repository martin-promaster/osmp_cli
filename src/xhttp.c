#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/xhttp.h"

static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
{
    
    size_t realsize = size * nmemb;
    http_response *mem = (http_response *)clientp;

    //X_LOG_DEBUG("Entering cb %d..........................................", mem->size);

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
        return 0;  /* out of response_st! */
 
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
    //X_LOG_DEBUG("CALL BACK realsize is:%d", realsize);
    //X_LOG_DEBUG("CALL BACK mem body data is:%s", mem->response);
    //X_LOG_DEBUG("CALL BACK mem body len is:%d", mem->size);

    return realsize;
}

http_response* http_post(char* url, char* token, char* payload) 
{
    http_response chunk = {0};
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    CURLcode res;
    if(curl) 
    {
        X_LOG_DEBUG("%s", "libcurl curl_easy_setopt() start ...");
        /* set url */
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        X_LOG_DEBUG("%s", "libcurl curl_easy_setopt() CURLOPT_HTTPHEADER start ...");
        struct curl_slist *list = {0};
        list = curl_slist_append(NULL, "Content-Type: application/json;charset=utf8");
        list = curl_slist_append(list, "routeurl: /project/project-list");
        if (token != NULL)
        {
            char* x_access_token_str = (char*)malloc( MAX_UTMP_TOKEN_SIZE );
            sprintf(x_access_token_str, "x-access-token: %s", token);
            X_LOG_DEBUG("%s", x_access_token_str);
            list = curl_slist_append(list, x_access_token_str);
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        X_LOG_DEBUG("%s", "libcurl curl_easy_setopt() CURLOPT_USERAGENT start ...");
        /* some servers do not like requests that are made without a user-agent 
            field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:82.0) Gecko/20100101 Firefox/82.0");

        /* POST data */
        X_LOG_DEBUG("%s", "libcurl curl_easy_setopt() CURLOPT_POSTFIELDSIZE/CURLOPT_POSTFIELDS start ...");
        X_LOG_DEBUG("request body data is:%s", payload);
        X_LOG_DEBUG("request body len is:%d", strlen(payload));
        /* size of the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(payload));
        /* pass in a pointer to the data - libcurl will not copy */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);

        /* RECEIVE DATA */
        X_LOG_DEBUG("%s", "libcurl curl_easy_setopt() CURLOPT_WRITEFUNCTION/CURLOPT_WRITEDATA start ...");
        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        X_LOG_DEBUG("%s", "libcurl curl_easy_setopt() successfully complete ...");


        X_LOG_DEBUG("%s", "libcurl curl_easy_perform() start ...");
        res = curl_easy_perform(curl);
        X_LOG_DEBUG("%s", "libcurl curl_easy_perform() successfully complete ...");
    
        if(res != CURLE_OK) {
            fprintf(stderr, "libcurl curl_easy_perform() is failed: %s", curl_easy_strerror(res));
            curl_slist_free_all(list); /* free the list again */
            curl_easy_cleanup(curl);
        } else {
            X_LOG_DEBUG("libcurl chunk size: %d", chunk.size);
            // X_LOG_DEBUG("chunk resp %s", chunk.response);

            http_response* response = (http_response*)malloc(sizeof(http_response*));
            response->response = (char*)malloc(1048576*10);
            memset(response->response, 0, chunk.size);
            memcpy(response->response, chunk.response, chunk.size);
            response->size = chunk.size;

            /* remember to free the buffer */
            free(chunk.response);
            X_LOG_DEBUG("%s", "free chunk response ...");

            curl_slist_free_all(list); /* free the list again */ 
            X_LOG_DEBUG("%s", "libcurl curl_slist_free_all() ...");
            curl_global_cleanup();
            X_LOG_DEBUG("%s", "libcurl curl_global_cleanup() ...");
            
            return response;
        }
    } 
}

// int http_post_deprecated(char* url, http_response *response) 
// {
//     http_response chunk = {0};
//     curl_global_init(CURL_GLOBAL_ALL);
//     CURL *curl = curl_easy_init();
//     CURLcode res;
//     if(curl) 
//     {
//         struct curl_slist *list;
//         list = curl_slist_append(NULL, "Content-Type: application/json;charset=utf8");
//         list = curl_slist_append(list, "routeurl: /project/project-list");
        
//         const char *data = "{\"code\":\"\",\"codeUuid\":\"\",\"loginName\":\"dongjin@utry.cn\",\"loginPwd\":\"123456\"}";
//         size_t data_len = strlen(data)+1;
//         printf("request data is:%s\nrequest data length is:%d\n", data, data_len);

//         /* set url */
//         curl_easy_setopt(curl, CURLOPT_URL, "https://utmpapi.utry.cn/utmp-admin-api/session/login");
//         curl_easy_setopt(curl, CURLOPT_POST, 1L);
//         /* size of the POST data */
//         curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data_len);
//         /* pass in a pointer to the data - libcurl will not copy */
//         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
//         /* send all data to this function  */
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
//         /* we pass our 'chunk' struct to the callback function */
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
//         /* some servers do not like requests that are made without a user-agent 
//             field, so we provide one */
//         curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:82.0) Gecko/20100101 Firefox/82.0");
//         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

//         // printf("libcurl start ...\n");
//         res = curl_easy_perform(curl);
//         // printf("http_response is: %s\n", chunk.response);
//         if(response->response == NULL) {
//             response->response = (char*)malloc(chunk.size*sizeof(char*));
//             strcpy(response->response, chunk.response);
//             response->size = chunk.size;
//         }
//         // printf("libcurl end ...\n");

//         if(res != CURLE_OK) {
//             fprintf(stderr, "curl_easy_perform() is failed: %s", curl_easy_strerror(res));
//             curl_slist_free_all(list); /* free the list again */
//             curl_easy_cleanup(curl);
//         }
//         curl_slist_free_all(list); /* free the list again */
//         curl_global_cleanup();

//         return CURLE_OK;
//     } 
// }