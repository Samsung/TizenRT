/*******************************************************************
 *
 * @file      ThingsLogger.h
 * @brief     Describes about the ThingsLogger header
 *
 *******************************************************************/

#ifndef THINGS_LOGGER_H_
#define THINGS_LOGGER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "oicdef.h"

#ifdef __TIZEN__
#include <dlog.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* Log Value Macro's */
#define THINGS_FUNC_ENTRY "IN"
#define THINGS_FUNC_EXIT "OUT"
#define THINGS_MEMORY_ERROR "Memory Error"
#define THINGS_INVALID_PARAM "Invalid Parameter"

#define THINGS_OIC "OIC"

#define PCF(str) str

// Max buffer size used in variable argument log function
#define MAX_LOG_V_BUFFER_SIZE (256)


// Log levels

typedef enum
{
    THINGS_DEBUG = 0, THINGS_INFO, THINGS_WARNING, THINGS_ERROR, THINGS_FATAL
} DALogLevel;

/**
 * Initialize the logger.
 */
void DALogInit();

/**
 * Called to Free dyamically allocated resources used with custom logging.
 * Not necessary if default logging is used
 *
 */
void DALogShutdown();

/**
 * Set the Version.
 */
void DALogSetVersion(char* version);

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param logStr - log string
 */
void DALog(DALogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *logStr);

/**
 * Output a variable argument list log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void DALogv(DALogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *format, ...);
//#endif //#ifdef __TIZEN__

const char* __get_timestamp__();

#ifdef TB_LOG

    #ifdef __TIZEN__

        //#define LOG_TAG_OCF "OIC_0.9.1.D.17.02.09_1_d7e99e8"
        #define THINGS_LOG(level,tag,mes) dlog_print(DLOG_INFO, OIC_STACK_FIRMWARE_VERSION, "%s : [%s:%d]:" mes, __get_timestamp__(), __FUNCTION__, __LINE__ )
        #define THINGS_LOG_V(level,tag,fmt,args...) dlog_print(DLOG_INFO, OIC_STACK_FIRMWARE_VERSION, "%s : [%s:%d]:" fmt, __get_timestamp__(), __FUNCTION__, __LINE__, ##args )
        #define THINGS_LOG_D(level,tag,fmt,args...) dlog_print(DLOG_DEBUG, OIC_STACK_FIRMWARE_VERSION, "%s : [%s:%d]:" fmt, __get_timestamp__(),__FUNCTION__, __LINE__, ##args )
        #define PROFILING_TIME(fmt,args...) dlog_print(DLOG_DEBUG, OIC_STACK_FIRMWARE_VERSION, "%s : [%s:%d]:[PROFILING_TIME] " fmt, __get_timestamp__(),__FUNCTION__, __LINE__, ##args )

    #else // These macros are defined for Linux, Android, and Arduino*/
        #define THINGS_LOG_INIT()   DALogInit()
        #define THINGS_LOG_SHUTDOWN()   DALogShutdown()
        #define THINGS_LOG(level, tag, logStr)  DALog((level), (tag), __FUNCTION__, __LINE__, (logStr))
        #define THINGS_LOG_D(level, tag, ...)   DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
        #define THINGS_LOG_V(level, tag, ...)   DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
        #define PROFILING_TIME(...)
    #endif //__TIZEN__
#else //TB_LOG
/*
 * Merged RELEASE & LOGGING option into RELEASE
 * THINGS_LOG_D will be disabled in RELEASE mode
 * THINGS_LOG & THINGS_LOG_V will be enabled in RELEASE mode
 *
 */
     #ifdef __TIZEN__
        #define THINGS_LOG_CONFIG(ctx)
        #define THINGS_LOG_SHUTDOWN()
        #define THINGS_LOG(level, tag, logStr)
        #define THINGS_LOG_V(level, tag, ...)
        #define THINGS_LOG_D(level, tag, ...)
        #define PROFILING_TIME(...)
        #define THINGS_LOG_INIT()
     #else
        #define THINGS_LOG_CONFIG(ctx)
        #define THINGS_LOG_SHUTDOWN()

#if 1
        #define THINGS_LOG(level, tag, logStr)  DALog((level), (tag), __FUNCTION__, __LINE__, (logStr))
        #define THINGS_LOG_V(level, tag, ...)   DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
        #define THINGS_LOG_D(level, tag, ...)   DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)

// Fot set loglevel
#else	
        #define THINGS_LOG(level, tag, logStr)  
        #define THINGS_LOG_V(level, tag, ...)   
        #define THINGS_LOG_D(level, tag, ...)   
#endif
		#define THINGS_LOG_DEBUG(level, tag, logStr)  DALog((level), (tag), __FUNCTION__, __LINE__, (logStr))
		#define THINGS_LOG_INFO(level, tag, logStr)  DALog((level), (tag), __FUNCTION__, __LINE__, (logStr))
		#define THINGS_LOG_WARNING(level, tag, logStr)  DALog((level), (tag), __FUNCTION__, __LINE__, (logStr))
		#define THINGS_LOG_ERROR(level, tag, logStr)  DALog((level), (tag), __FUNCTION__, __LINE__, (logStr))
		#define THINGS_LOG_FATAL(level, tag, logStr)  DALog((level), (tag), __FUNCTION__, __LINE__, (logStr))

		#define THINGS_LOG_V_DEBUG(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_V_INFO(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_V_WARNING(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_V_ERROR(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_V_FATAL(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)

		#define THINGS_LOG_D_DEBUG(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_D_INFO(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_D_WARNING(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_D_ERROR(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
		#define THINGS_LOG_D_FATAL(level, tag, ...)  DALogv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
// Fot set loglevel //

        #define PROFILING_TIME(...)
        #define THINGS_LOG_INIT()
       // #define ENABLE_MALLOC_DEBUG
    #endif
#endif // TB_LOG


#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* THINGS_LOGGER_H_ */
