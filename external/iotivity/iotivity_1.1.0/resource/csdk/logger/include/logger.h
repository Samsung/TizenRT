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

#define IOTIVITY_VERSION "1.1.0"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "logger_types.h"
#include "iotivityopts.h"

#ifdef __ANDROID__
#include <android/log.h>
#elif defined(__TIZEN__)
#include <dlog.h>
#elif defined(ARDUINO)
#include "Arduino.h"
#include "avr/pgmspace.h"
#endif

#ifdef __TINYARA__

#ifdef ERROR
#undef ERROR
#endif

#endif

#ifdef __cplusplus
extern "C"
{
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

// Log levels
#ifdef __TIZEN__
  typedef enum {
    DEBUG = DLOG_DEBUG,
    INFO = DLOG_INFO,
    WARNING = DLOG_WARN,
    ERROR = DLOG_ERROR,
    FATAL = DLOG_ERROR
  } LogLevel;
#else
  typedef enum
  {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
  } LogLevel;
#endif

#ifdef __TIZEN__
#define OCLog(level,tag,mes)
#define OCLogv(level,tag,fmt,args...)
#elif defined(ANDROID) || defined(__linux__) || defined(__APPLE__) || defined(__TINYARA__)
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
  void OCLogv(LogLevel level, const char *tag, const char *format, ...)
#if defined(__GNUC__)
  __attribute__((format(printf, 3, 4)))
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
  void OCLog(LogLevel level, const char *tag, const char *logStr);

  /**
   * Output the contents of the specified buffer (in hex) with the specified priority level.
   *
   * @param level      - DEBUG, INFO, WARNING, ERROR, FATAL
   * @param tag        - Module name
   * @param buffer     - pointer to buffer of bytes
   * @param bufferSize - max number of byte in buffer
   */
  void OCLogBuffer(LogLevel level, const char *tag, const uint8_t *buffer, uint16_t bufferSize);
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
  void OCLog(LogLevel level, PROGMEM const char *tag, const int lineNum,
             PROGMEM const char *logStr);

  /**
   * Output the contents of the specified buffer (in hex) with the specified priority level.
   *
   * @param level      - DEBUG, INFO, WARNING, ERROR, FATAL
   * @param tag        - Module name
   * @param buffer     - pointer to buffer of bytes
   * @param bufferSize - max number of byte in buffer
   */
  void OCLogBuffer(LogLevel level, const char *tag, const uint8_t *buffer, size_t bufferSize);

  /**
   * Output a variable argument list log string with the specified priority level.
   *
   * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
   * @param tag    - Module name
   * @param lineNum- line Number
   * @param format - variadic log string
   */
  void OCLogv(LogLevel level, PROGMEM const char *tag, const int lineNum,
              PROGMEM const char *format, ...)
#if defined(__GNUC__)
  __attribute__((format(printf, 4, 5)))
#endif
  ;
#endif

#ifdef TB_LOG

#ifdef __TIZEN__

#define OIC_LOG(level,tag,mes) LOG_(LOG_ID_MAIN, (level), (tag), mes)
#define OIC_LOG_V(level,tag,fmt,args...) LOG_(LOG_ID_MAIN, level, tag, fmt, ##args)
#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize)

#else // These macros are defined for Linux, Android, and Arduino

#define OIC_LOG_INIT()    OCLogInit()

#ifdef ARDUINO

#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize)  OCLogBuffer((level), PCF(tag), (buffer), (bufferSize))
// Don't define variable argument log function for Arduino
#define OIC_LOG_V(level, tag, format, ...) OCLogv((level), PCF(tag), __LINE__, PCF(format),__VA_ARGS__)

#define OIC_LOG_CONFIG(ctx)
#define OIC_LOG_SHUTDOWN()
#define OIC_LOG(level, tag, logStr) OCLog((level), PCF(tag), __LINE__, PCF(logStr))
#define OIC_LOG_V(level, tag, ...)

#else

#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize)  OCLogBuffer((level), (tag), (buffer), (bufferSize))
#define OIC_LOG_CONFIG(ctx)    OCLogConfig((ctx))
#define OIC_LOG_SHUTDOWN()     OCLogShutdown()
#define OIC_LOG(level, tag, logStr)  OCLog((level), (tag), (logStr))
// Define variable argument log function for Linux and Android
#define OIC_LOG_V(level, tag, ...)  OCLogv((level), (tag), __VA_ARGS__)

#endif //ARDUINO
#endif //__TIZEN__

#else //TB_LOG

#define OIC_LOG_CONFIG(ctx)
#define OIC_LOG_SHUTDOWN()
#define OIC_LOG(level, tag, logStr)
#define OIC_LOG_V(level, tag, ...)
#define OIC_LOG_BUFFER(level, tag, buffer, bufferSize)
#define OIC_LOG_INIT()
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* LOGGER_H_ */
