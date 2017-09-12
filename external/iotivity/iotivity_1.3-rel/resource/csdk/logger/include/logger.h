//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "logger_types.h"

#ifdef __ANDROID__
#include <android/log.h>
#elif defined(__TIZEN__)
#include <dlog.h>
#elif defined(ARDUINO)
#include <inttypes.h>
#include "Arduino.h"
#include "avr/pgmspace.h"
#endif

#ifdef __cplusplus
#ifndef ARDUINO
#include <cinttypes>
#endif
extern "C"
{
#else
#include <inttypes.h>
#endif

// Use the PCF macro to wrap strings stored in FLASH on the Arduino
// Example:  OIC_LOG(INFO, TAG, PCF("Entering function"));
#ifdef ARDUINO
#ifdef __cplusplus
#define PCF(str)  ((PROGMEM const char *)(F(str)))
#else
#define PCF(str)  ((PROGMEM const char *)(PSTR(str)))
#endif //__cplusplus
#else
    #define PCF(str) str
#endif

// Max buffer size used in variable argument log function
#define MAX_LOG_V_BUFFER_SIZE (256)

// Setting this flag for a log level means that the corresponding log message
// contains private data. This kind of message is logged only when a call to
// OCSetLogLevel() enabled private data logging.
#define OC_LOG_PRIVATE_DATA (1 << 31)

// Log levels
#ifdef __TIZEN__
typedef enum {
    DEBUG = DLOG_INFO,    // DLOG_INFO : 4
    INFO = DLOG_INFO,
    WARNING = DLOG_WARN,  // DLOG_WARN : 5
    ERROR = DLOG_ERROR,   // DLOG_ERROR : 6
    FATAL = DLOG_ERROR,
    DEBUG_LITE = DLOG_INFO,
    INFO_LITE = DLOG_INFO,
} LogLevel;
#else

/** @todo temporary work-around until better names with prefixes are used for the enum values. */
#ifdef ERROR
#undef ERROR
#endif

typedef enum {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL,
    DEBUG_LITE,       // The DEBUG log for Lite device
    INFO_LITE,        // The INFO log for Lite device
} LogLevel;

#endif // __TIZEN__

#define DEBUG_PRIVATE       ((OC_LOG_PRIVATE_DATA) | (DEBUG))
#define INFO_PRIVATE        ((OC_LOG_PRIVATE_DATA) | (INFO))
#define WARNING_PRIVATE     ((OC_LOG_PRIVATE_DATA) | (WARNING))
#define ERROR_PRIVATE       ((OC_LOG_PRIVATE_DATA) | (ERROR))
#define FATAL_PRIVATE       ((OC_LOG_PRIVATE_DATA) | (FATAL))

#ifndef OC_LOG_LEVEL
#define OC_MINIMUM_LOG_LEVEL    (DEBUG)
#else
#define OC_MINIMUM_LOG_LEVEL    (OC_LOG_LEVEL)
#endif

// Perform signed comparison here, to avoid compiler warnings caused by
// unsigned comparison with DEBUG (i.e., with value 0 on some platforms).
#define IF_OC_PRINT_LOG_LEVEL(level) \
    if (((int)OC_MINIMUM_LOG_LEVEL) <= ((int)(level & (~OC_LOG_PRIVATE_DATA))))

/**
 * Set log level and privacy log to print.
 *
 * @param level                   - log level.
 * @param hidePrivateLogEntries   - Hide Private Log.
 */
void OCSetLogLevel(LogLevel level, bool hidePrivateLogEntries);

#ifdef __TIZEN__
/**
 * Output the contents of the specified buffer (in hex) with the specified priority level.
 *
 * @param[in]    level      DEBUG, INFO, WARNING, ERROR, FATAL
 * @param[in]    tag        Module name
 * @param[in]    buffer     pointer to buffer of bytes
 * @param[in]    bufferSize max number of byte in buffer
 */
void OCLogBuffer(int level, const char* tag, const uint8_t* buffer, size_t bufferSize);

#define OCLog(level,tag,mes) LOG_(LOG_ID_MAIN, ((level) & (~OC_LOG_PRIVATE_DATA)), (tag), mes)
#define OCLogv(level,tag,fmt,args...) LOG_(LOG_ID_MAIN, ((level) & (~OC_LOG_PRIVATE_DATA)),tag,fmt,##args)
#elif !defined(ARDUINO)
    /**
     * Configure logger to use a context that defines a custom logger function
     *
     * @param ctx - pointer to oc_log_ctx_t struct that defines custom logging functions
     */
    void OCLogConfig(oc_log_ctx_t *ctx);

    /**
     * Initialize the logger.  Optional on Android and Linux.  Configures serial port on Arduino
     */
    void OCLogInit();

    /**
     * Called to Free dyamically allocated resources used with custom logging.
     * Not necessary if default logging is used
     *
     */
    void OCLogShutdown();

    /**
     * Output a variable argument list log string with the specified priority level.
     * Only defined for Linux and Android
     *
     * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
     * @param tag    - Module name
     * @param format - variadic log string
     */
    void OCLogv(int level, const char * tag, const char * format, ...)
#if defined(__GNUC__)
    __attribute__ ((format(printf, 3, 4)))
#endif
    ;

    /**
     * Output a log string with the specified priority level.
     * Only defined for Linux and Android
     *
     * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
     * @param tag    - Module name
     * @param logStr - log string
     */
    void OCLog(int level, const char * tag, const char * logStr);

    /**
     * Output the contents of the specified buffer (in hex) with the specified priority level.
     *
     * @param level      - DEBUG, INFO, WARNING, ERROR, FATAL
     * @param tag        - Module name
     * @param buffer     - pointer to buffer of bytes
     * @param bufferSize - max number of byte in buffer
     */
    void OCLogBuffer(int level, const char* tag, const uint8_t* buffer, size_t bufferSize);
#else  // For arduino platforms
    /**
     * Initialize the serial logger for Arduino
     * Only defined for Arduino
     */
    void OCLogInit();

    /**
     * Output a log string with the specified priority level.
     * Only defined for Arduino.  Uses PROGMEM strings
     *
     * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
     * @param tag    - Module name
     * @param lineNum- line Number
     * @param logStr - log string
     */
    void OCLog(int level, PROGMEM const char *tag, const int lineNum,
               PROGMEM const char *logStr);

    /**
     * Output the contents of the specified buffer (in hex) with the specified priority level.
     *
     * @param level      - DEBUG, INFO, WARNING, ERROR, FATAL
     * @param tag        - Module name
     * @param buffer     - pointer to buffer of bytes
     * @param bufferSize - max number of byte in buffer
     */
    void OCLogBuffer(int level, const char* tag, const uint8_t* buffer, size_t bufferSize);

    /**
     * Output a variable argument list log string with the specified priority level.
     *
     * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
     * @param tag    - Module name
     * @param lineNum- line Number
     * @param format - variadic log string
     */
    void OCLogv(int level, PROGMEM const char *tag, const int lineNum,
                PROGMEM const char *format, ...)
#if defined(__GNUC__)
    __attribute__ ((format(printf, 4, 5)))
#endif
;
#endif

#ifdef TB_LOG

#ifdef __TIZEN__
#define OIC_LOG(level,tag,mes) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            LOG_(LOG_ID_MAIN, (level), (tag), mes); \
    } while(0)

#define OIC_LOG_V(level,tag,fmt,args...) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            LOG_(LOG_ID_MAIN, (level), (tag), fmt, ##args); \
    } while(0)

#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCLogBuffer((level), (tag), (buffer), (bufferSize)); \
    } while(0)

#define OIC_LOG_CA_BUFFER(level, tag, buffer, bufferSize, isHeader) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCPrintCALogBuffer((level), (tag), (buffer), (bufferSize), (isHeader)); \
    } while(0)

#else // NO __TIZEN__ - These macros are defined for Linux, Android, Win32, and Arduino
#define OIC_LOG_INIT()    OCLogInit()

#ifdef ARDUINO
#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCLogBuffer((level), PCF(tag), (buffer), (bufferSize)); \
    } while(0)

// Don't define variable argument log function for Arduino
#define OIC_LOG_V(level, tag, format, ...) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCLogv((level), PCF(tag), __LINE__, PCF(format),__VA_ARGS__); \
    } while(0)

#define OIC_LOG_CONFIG(ctx)
#define OIC_LOG_SHUTDOWN()
#define OIC_LOG(level, tag, logStr) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCLog((level), PCF(tag), __LINE__, PCF(logStr)); \
    } while(0)

#define OIC_LOG_V(level, tag, ...)

#else // NO ARDUINO
#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCLogBuffer((level), (tag), (buffer), (bufferSize)); \
    } while(0)

#define OIC_LOG_CA_BUFFER(level, tag, buffer, bufferSize, isHeader) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCPrintCALogBuffer((level), (tag), (buffer), (bufferSize), (isHeader)); \
    } while(0)

#define OIC_LOG_CONFIG(ctx)    OCLogConfig((ctx))
#define OIC_LOG_SHUTDOWN()     OCLogShutdown()
#define OIC_LOG(level, tag, logStr) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCLog((level), (tag), (logStr)); \
    } while(0)

// Define variable argument log function for Linux, Android, and Win32
#define OIC_LOG_V(level, tag, ...) \
    do { \
        IF_OC_PRINT_LOG_LEVEL((level)) \
            OCLogv((level), (tag), __VA_ARGS__); \
    } while(0)

#endif // ARDUINO
#endif // __TIZEN__

#else // TB_LOG
#define OIC_LOG_CONFIG(ctx)
#define OIC_LOG_SHUTDOWN()
#define OIC_LOG(level, tag, logStr)
#define OIC_LOG_V(level, tag, ...)
#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize)
#define OIC_LOG_CA_BUFFER(level, tag, buffer, bufferSize, isHeader)
#define OIC_LOG_INIT()
#endif // TB_LOG

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* LOGGER_H_ */
