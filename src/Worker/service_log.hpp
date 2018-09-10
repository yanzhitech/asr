#ifndef _SERVICE_LOG_HPP_
#define _SERVICE_LOG_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION <= 30400
//#pragma GCC system_header
//#warning use high version gcc
#endif
#define NEED_DEBUG

#ifdef NEED_DEBUG
	#define _DEBUG(fmt, ...)\
	({\
	 time_t __time_buf__;\
	 tm __localtime_buf__;\
	 char __strftime_buf__[sizeof("19820429 23:59:59")];\
	 time(&__time_buf__);\
	 localtime_r(&__time_buf__, &__localtime_buf__);\
	 strftime(__strftime_buf__, sizeof("19820429 23:59:59"), "%Y%m%d %H:%M:%S", &__localtime_buf__);\
	 fprintf(stderr, "[DEBUG]  [%s]  "  fmt ,__strftime_buf__, ##__VA_ARGS__);\
	 })
#else
	#define _DEBUG(fmt, ...)
#endif


#define _INFO(fmt, ...)\
	({\
	 timeval __time_buf__;\
	 tm __localtime_buf__;\
	 char __strftime_buf__[sizeof("19820429 23:59:59")];\
	 gettimeofday(&__time_buf__, NULL);\
	 localtime_r(&(__time_buf__.tv_sec), &__localtime_buf__);\
	 strftime(__strftime_buf__, sizeof("19820429 23:59:59"), "%Y%m%d %H:%M:%S", &__localtime_buf__);\
	 fprintf(stderr, "[INFO] [%s]  "  fmt ,__strftime_buf__, ##__VA_ARGS__);\
	 })

#define _LOG(_buf, fmt, ...)\
	({\
	 timeval __time_buf__;\
	 tm __localtime_buf__;\
	 char __strftime_buf__[sizeof("19820429 23:59:59")];\
	 gettimeofday(&__time_buf__, NULL);\
	 localtime_r(&(__time_buf__.tv_sec), &__localtime_buf__);\
	 strftime(__strftime_buf__, sizeof("19820429 23:59:59"), "%Y%m%d %H:%M:%S", &__localtime_buf__);\
	 snprintf(_buf, sizeof(_buf), "[INFO] [%s]  " fmt  ,__strftime_buf__,##__VA_ARGS__);\
	 })

#define _ERROR(fmt, ...)\
	({\
	 timeval __time_buf__;\
	 tm __localtime_buf__;\
	 char __strftime_buf__[sizeof("19820429 23:59:59")];\
	 gettimeofday(&__time_buf__, NULL);\
	 localtime_r(&(__time_buf__.tv_sec), &__localtime_buf__);\
	 strftime(__strftime_buf__, sizeof("19820429 23:59:59"), "%Y%m%d %H:%M:%S", &__localtime_buf__);\
	 fprintf(stderr, "[ERROR] [%s]  " fmt ,__strftime_buf__, ##__VA_ARGS__);\
	 })

#define _ERROR_RETURN(ret, fmt, ...)\
	({\
	 _ERROR(fmt, ##__VA_ARGS__);\
	 return ret;\
	 })

#define _ERROR_EXIT(ret, fmt, ...)\
	({\
	 _ERROR(fmt, ##__VA_ARGS__);\
	 exit(ret);\
	 })

#define _ERROR_CONTINUE(fmt, ...)\
	({\
	 _ERROR(fmt, ##__VA_ARGS__);\
	 continue;\
	 })

#define _ERROR_BREAK(fmt, ...)\
	({\
	 _ERROR(fmt, ##__VA_ARGS__);\
	 break;\
	 })

#define WASTE_TIME_US(past)\
	({\
	 timeval __now__;\
	 gettimeofday(&__now__, NULL);\
	 (__now__.tv_sec - past.tv_sec) * 1000000 + (__now__.tv_usec - past.tv_usec);\
	 })

#define WASTE_TIME_US_INTERVAL(start, end)\
	({\
	 (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);\
	 })

#define WASTE_TIME_MS(past)\
	({\
	 timeval __now__;\
	 gettimeofday(&__now__, NULL);\
	 (__now__.tv_sec - past.tv_sec) * 1000 + (__now__.tv_usec - past.tv_usec) / 1000;\
	 })

#define SILENT_STDERR(x)\
	({\
	 int __err_fd__ = dup(STDERR_FILENO);\
	 close(STDERR_FILENO);\
	 int __ret__ = (x);\
	 dup2(__err_fd__, STDERR_FILENO);\
	 close(__err_fd__);\
	 __ret__;\
	 })


#define INFO(fmt, arg...) \
    do { \
	LOG(INFO, fmt, ##arg); \
    } while(0)

#define DEBUG(fmt, arg...) \
    do { \
	LOG(DEBUG, fmt, ##arg); \
    } while(0)

#define TRACE(fmt, arg...) \
    do { \
	LOG(TRACE, fmt, ##arg); \
    } while(0)

#define WARNING(fmt, arg...) \
    do { \
	LOG(WARNING, fmt, ##arg); \
    } while(0)

#define ERROR(fmt, arg...) \
    do { \
	LOG(ERROR, fmt, ##arg); \
    } while(0)

#define FATAL(fmt, arg...) \
    do { \
	LOG(FATAL, fmt, ##arg); \
    } while(0)

#define CRITICAL(fmt, arg...) \
    do { \
	LOG(CRITICAL, fmt, ##arg); \
    } while(0)

#define LOG(level, fmt, arg...) \
    do { \
        time_t timestamp = time(NULL); \
        struct tm sts; \
	localtime_r(&timestamp, &sts); \
        fprintf(stderr, "["#level"][%4d%02d%02d %02d:%02d:%02d][%x][%s:%d] "fmt"\n", \
		sts.tm_year + 1900, sts.tm_mon + 1, sts.tm_mday, \
		sts.tm_hour, sts.tm_min, sts.tm_sec, \
		(unsigned int) pthread_self(), \
		__FILE__, __LINE__, ##arg); \
    } while (0)

#endif /* _SERVICE_LOG_HPP_ */

