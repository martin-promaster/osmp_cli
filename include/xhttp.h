#ifndef __X_HTTP_H__
#define __X_HTTP_H__

#include <stdlib.h>
#include <curl/curl.h>
#include "../include/xdefine.h"

typedef struct http_response_s {
  char *response;
  size_t size;
} http_response;

typedef struct http_request_s {
    char *request;
    size_t size;
} http_request;

http_response* http_post(char* url, char* token, char* payload);

#endif