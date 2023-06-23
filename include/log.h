#pragma once

#include <memory>
#include <ctime>
#include <cstring>

using namespace std;

#define GL_LOG_I(format, ...)                                                              \
{                                                                                          \
    time_t t = time(0);                                                                    \
    struct tm* ptm = localtime(&t);                                                                 \
    fprintf(stderr, "[ INFO ] [%4d-%02d-%02d %02d:%02d:%02d ] [ %s:%s:%d ] " format "\n",    \
            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour,                  \
            ptm->tm_min, ptm->tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
}

#define GL_LOG_E(format, ...)                                                              \
{                                                                                          \
    time_t t = time(0);                                                                    \
    struct tm* ptm = localtime(&t);                                                                 \
    fprintf(stderr, "[ ERROR] [%4d-%02d-%02d %02d:%02d:%02d ] [ %s:%s:%d ] " format "\n",    \
            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour,                  \
            ptm->tm_min, ptm->tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
}

#define GL_LOG_D(format, ...)                                                              \
{                                                                                          \
    time_t t = time(0);                                                                    \
    struct tm* ptm = localtime(&t);                                                                 \
    fprintf(stderr, "[ DEBUG] [%4d-%02d-%02d %02d:%02d:%02d ] [ %s:%s:%d ] " format "\n",    \
            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour,                  \
            ptm->tm_min, ptm->tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
}

#define GL_LOG_W(format, ...)                                                              \
{                                                                                          \
    time_t t = time(0);                                                                    \
    struct tm* ptm = localtime(&t);                                                                 \
    fprintf(stderr, "[ WARN ] [%4d-%02d-%02d %02d:%02d:%02d ] [ %s:%s:%d ] " format "\n",    \
            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour,                  \
            ptm->tm_min, ptm->tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
}


