/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __ST_THINGS_LOGGER_H__
#define __ST_THINGS_LOGGER_H__

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __TIZEN__
#include <dlog.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif							/* __cplusplus */

/* Log Value Macro's */
#define ST_THINGS_FUNC_ENTRY "IN"
#define ST_THINGS_FUNC_EXIT "OUT"
#define ST_THINGS_MEMORY_ERROR "Memory Error"
#define ST_THINGS_INVALID_PARAM "Invalid Parameter"
#define ST_THINGS_TAG "ST_THINGS_SDK"

// Max buffer size used in variable argument log function
#define MAX_LOG_V_BUFFER_SIZE (256)

// Log levels
#ifdef __TIZEN__
typedef enum {
	ST_DEBUG = DLOG_DEBUG,
	ST_INFO = DLOG_INFO,
	ST_WARNING = DLOG_WARN,
	ST_ERROR = DLOG_ERROR,
	ST_FATAL = DLOG_FATAL
}
st_things_log_level_e;
#endif

#ifndef __TIZEN__				/* Linux */
typedef enum {
	ST_DEBUG,
	ST_INFO,
	ST_WARNING,
	ST_ERROR,
	ST_FATAL
} st_things_log_level_e;

/**
 * Initialize the logger.
 */
void st_things_log_init();

/**
 * Called to Free dyamically allocated resources used with custom logging.
 * Not necessary if default logging is used
 */
void st_things_log_shutdown();

/**
 * Set the Version.
 */
void st_things_log_set_version(char *version);

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level   - ST_DEBUG, ST_INFO, ST_WARNING, ST_ERROR, ST_FATAL
 * @param tag     - Module name
 * @param log_str - log string
 */
void st_things_log(st_things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *log_str);

/**
 * Output a variable argument list log string with the specified priority level.
 * @param level  - ST_DEBUG, ST_INFO, ST_WARNING, ST_ERROR, ST_FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void st_things_log_v(st_things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *format, ...);
#endif							//#ifndef __TIZEN__

#ifdef FEATURE_LOGGING
#ifdef __TIZEN__
#define ST_LOG_INIT()
#define ST_LOG_DEINIT()
#define ST_LOG(level, log_str) dlog_print(level, ST_THINGS_TAG, "%s : %s(%d) > " log_str, __FILE__, __FUNCTION__, __LINE__)
#define ST_LOG_V(level, fmt, ...) dlog_print(level, ST_THINGS_TAG, "%s : %s(%d) > " fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define ST_LOG_ENTRY(level) dlog_print(level, ST_THINGS_TAG, "%s : %s(%d) > " ST_THINGS_FUNC_ENTRY, __FILE__, __FUNCTION__, __LINE__)
#define ST_LOG_EXIT(level) dlog_print(level, ST_THINGS_TAG, "%s : %s(%d) > " ST_THINGS_FUNC_EXIT, __FILE__, __FUNCTION__, __LINE__)
#else							/* Linux */
#define ST_LOG_INIT()   st_things_log_init()
#define ST_LOG_DEINIT()   st_things_log_shutdown()
#define ST_LOG(level, log_str)  st_things_log((level), ST_THINGS_TAG, __FUNCTION__, __LINE__, (log_str))
#define ST_LOG_V(level, fmt, ...)   st_things_log_v((level), ST_THINGS_TAG, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define ST_LOG_ENTRY(level) st_things_log((level), ST_THINGS_TAG, __FUNCTION__, __LINE__, (ST_THINGS_FUNC_ENTRY))
#define ST_LOG_EXIT(level) st_things_log((level), ST_THINGS_TAG, __FUNCTION__, __LINE__, (ST_THINGS_FUNC_EXIT))
#endif
#else							//DISABLE LOGGING
#define ST_LOG_INIT()
#define ST_LOG_DEINIT()
#define ST_LOG(level, log_str)
#define ST_LOG_V(level, fmt, ...)
#define ST_LOG_ENTRY(level)
#define ST_LOG_EXIT(level)
#endif							// FEATURE_LOGGING

#ifdef __cplusplus
}
#endif							/* __cplusplus */
#endif							/* __ST_THINGS_LOGGER_H__ */
