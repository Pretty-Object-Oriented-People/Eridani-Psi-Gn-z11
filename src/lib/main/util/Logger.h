/*
 * (Re)defines the logging macros (from Logging.h) with logger information formatting.
 * To use this, use #define LOGGER_? to set logger settings, then include this file.
 * You should redefine the NAME for the logger.
 * 
 * Examples,
 * Basic Usage:
 * ```
 * #define LOGGER_NAME "A simple logger"
 * #include Logger.h
 * ```
 * Usage with different time format logging:
 * ```
 * #define LOGGER_NAME "I don't care about the day"
 * #define LOGGER_TIMEFMT "%d:%d:%d"
 * #define LOGGER_TIMEFMTARGS(ttm) ttm->tm_hour, ttm->tm_min, ttm->tm_sec
 * #include Logger.h
 * ```
 * You can also take full control of the formatting by defining FORMAT and FORMAT_VA
 * 
 * It should be obvious that Loggers can only be used [concurrently] correctly inside compiled files.
 * 
 */

#ifndef __LOGGERH_IMPORTS
#define __LOGGERH_IMPORTS
#include "Logging.h"
#include <time.h>
#endif

//Settings

#ifndef LOGGER_FORMAT

#ifndef LOGGER_NAME
#define LOGGER_NAME "Random Logger"
#endif

#ifndef LOGGER_TIMEFMT
#define LOGGER_TIMEFMT "%d.%d.%d %d:%d:%d"
#endif

#define LOGGER_FORMAT(info) "[" LOGGER_NAME "][" LOGGER_TIMEFMT "] " info
#endif

//VA Settings

#ifndef LOGGER_FORMAT_VA

#ifndef LOGGER_TIMEFMTARGS
#define LOGGER_TIMEFMTARGS(ttm) ttm->tm_mday, ttm->tm_mon+1, ttm->tm_year+1900, ttm->tm_hour, ttm->tm_min, ttm->tm_sec
#endif

#define LOGGER_FORMAT_VA(ttm, ...) LOGGER_TIMEFMTARGS(_lt), ##__VA_ARGS__
#endif

//Logging

#define loggerPrintTo(fd, info, ...) do { time_t _t; time(&_t); let _lt = localtime(&_t); fprintf(fd, LOGGER_FORMAT(info), LOGGER_FORMAT_VA(ttm, ##__VA_ARGS__)); } while(0)

#undef logInfoInline
#undef logErrorInline

#define logInfoInline(info, ...) loggerPrintTo(stdout, info, ##__VA_ARGS__)
#define logErrorInline(info, ...) loggerPrintTo(stderr, info, ##__VA_ARGS__)

#ifdef __DEBUG_MODE
#undef logDebugInline
#define logDebugInline(info, ...) loggerPrintTo(stdout, info, ##__VA_ARGS__)
#endif