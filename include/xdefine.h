#ifndef __X_DEFINE_H__
#define __X_DEFINE_H__

/* 更新宏定义 1. 使用可变参数，支持多项输出; 2. 去除Z中默认加上的双引号; */
#define X_LOG_DEBUG(Z, X...) \
    printf("[%s %s] [%s.%d] [%s] [DEBUG] " Z "\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, ##X)

#define MAX_UTMP_TOKEN_SIZE     8192
#define MAX_OSMP_JSON_SIZE      1048576

#endif