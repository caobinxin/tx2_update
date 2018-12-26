#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG
#ifdef DEBUG

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERR 3
 
#define printf_error(s) \
    printf("\e[1;31m[%s:%d    func:%s]%s\e[0m\n", __FILE__, __LINE__, __FUNCTION__, s);  
  
#define printf_run(s) \
    printf("\e[1;32m[%s:%d    func:%s]%s\e[0m\n", __FILE__, __LINE__, __FUNCTION__, s);  
  
#define printf_warn(s) \
    printf("\e[1;33m[%s:%d    func:%s]%s\e[0m\n", __FILE__, __LINE__, __FUNCTION__, s);  
  
#define printf_log(t,s) \
        switch (t) \
        { \
            case LOG_ERR: \
                printf_error(s); \
                break; \
            case LOG_DEBUG: \
            case LOG_INFO: \
                printf_run(s); \
                break; \
            case LOG_WARNING: \
                printf_warn(s); \
            default: \
                printf(s); \
                break; \
        }

#endif

#endif