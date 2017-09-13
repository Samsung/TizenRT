#ifndef SDKAPI_LOGGER_H_
#define SDKAPI_LOGGER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __TIZEN__
#include <dlog.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* Log Value Macro's */
#define SDKAPI_FUNC_ENTRY "IN"
#define SDKAPI_FUNC_EXIT "OUT"
#define SDKAPI_MEMORY_ERROR "Memory Error"
#define SDKAPI_INVALID_PARAM "Invalid Parameter"
#define SDKAPI_OCF_THINGS_TAG "OCF-THINGS-SDKAPI"

// Max buffer size used in variable argument log function
#define MAX_LOG_V_BUFFER_SIZE (256)

// Log levels
#ifdef __TIZEN__
typedef enum
{
    SDKAPI_DEBUG = DLOG_DEBUG,
    SDKAPI_INFO = DLOG_INFO,
    SDKAPI_WARNING = DLOG_WARN,
    SDKAPI_ERROR = DLOG_ERROR,
    SDKAPI_FATAL = DLOG_FATAL
} SDKAPI_LogLevel;
#endif

#ifndef __TIZEN__ /* Linux */
typedef enum
{
    SDKAPI_DEBUG,
    SDKAPI_INFO,
    SDKAPI_WARNING,
    SDKAPI_ERROR,
    SDKAPI_FATAL
} SDKAPI_LogLevel;

/**
 * Initialize the logger.
 */
void SDKAPI_LogInit();

/**
 * Called to Free dyamically allocated resources used with custom logging.
 * Not necessary if default logging is used
 */
void SDKAPI_LogShutdown();

/**
 * Set the Version.
 */
void SDKAPI_LogSetVersion(char* version);

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param logStr - log string
 */
void SDKAPI_Log(SDKAPI_LogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *logStr);

/**
 * Output a variable argument list log string with the specified priority level.
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void SDKAPI_LogV(SDKAPI_LogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *format, ...);
#endif //#ifndef __TIZEN__

#ifdef FEATURE_LOGGING
    #ifdef __TIZEN__
        #define SDKAPI_LOG_INIT()
        #define SDKAPI_LOG_DEINIT()
        #define SDKAPI_LOG(level, logStr) dlog_print(level, SDKAPI_OCF_THINGS_TAG, "%s : %s(%d) > " logStr, __FILE__, __FUNCTION__, __LINE__ )
        #define SDKAPI_LOG_V(level, fmt, ...) dlog_print(level, SDKAPI_OCF_THINGS_TAG, "%s : %s(%d) > " fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
        #define SDKAPI_LOG_ENTRY(level) dlog_print(level, SDKAPI_OCF_THINGS_TAG, "%s : %s(%d) > " SDKAPI_FUNC_ENTRY, __FILE__, __FUNCTION__, __LINE__ )
        #define SDKAPI_LOG_EXIT(level) dlog_print(level, SDKAPI_OCF_THINGS_TAG, "%s : %s(%d) > " SDKAPI_FUNC_EXIT, __FILE__, __FUNCTION__, __LINE__ )
    #else /* Linux */
        #define SDKAPI_LOG_INIT()   SDKAPI_LogInit()
        #define SDKAPI_LOG_DEINIT()   SDKAPI_LogShutdown()
        #define SDKAPI_LOG(level, logStr)  SDKAPI_Log((level), SDKAPI_OCF_THINGS_TAG, __FUNCTION__, __LINE__, (logStr))
        #define SDKAPI_LOG_V(level, fmt, ...)   SDKAPI_LogV((level), SDKAPI_OCF_THINGS_TAG, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
        #define SDKAPI_LOG_ENTRY(level) SDKAPI_Log((level), SDKAPI_OCF_THINGS_TAG, __FUNCTION__, __LINE__, (SDKAPI_FUNC_ENTRY))
        #define SDKAPI_LOG_EXIT(level) SDKAPI_Log((level), SDKAPI_OCF_THINGS_TAG, __FUNCTION__, __LINE__, (SDKAPI_FUNC_EXIT))
    #endif
#else //DISABLE LOGGING
    #define SDKAPI_LOG_INIT()
    #define SDKAPI_LOG_DEINIT()
    #define SDKAPI_LOG(level, logStr)
    #define SDKAPI_LOG_V(level, fmt, ...)
    #define SDKAPI_LOG_ENTRY(level)
    #define SDKAPI_LOG_EXIT(level)
#endif // FEATURE_LOGGING

#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* SDKAPI_LOGGER_H_ */
