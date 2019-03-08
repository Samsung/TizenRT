/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef THINGS_LOGGER_H_
#define THINGS_LOGGER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "things_def.h"
#include "ocpayload.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Value Macro's */
#define THINGS_FUNC_ENTRY "IN"
#define THINGS_FUNC_EXIT "OUT"
#define THINGS_MEMORY_ERROR "Memory Error"
#define THINGS_INVALID_PARAM "Invalid Parameter"

// Max buffer size used in variable argument log function
#define MAX_LOG_V_BUFFER_SIZE (256)

// Log levels

typedef enum {
	THINGS_DEBUG = 0, THINGS_INFO, THINGS_WARNING, THINGS_ERROR, THINGS_FATAL
} things_log_level_e;

/**
 * Initialize the logger.
 */
void things_log_init(void);

/**
 * Called to Free dyamically allocated resources used with custom logging.
 * Not necessary if default logging is used
 *
 */
void things_log_shutdown(void);

/**
 * Set the Version.
 */
void things_log_set_version(char *version);

/**
 * Output a variable argument list log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void things_logv(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *format, ...);
//#endif //#ifdef __TIZEN__

const char *__get_timestamp__(void);

#define THINGS_LOG_INIT()   things_log_init()
#define THINGS_LOG_SHUTDOWN()   things_log_shutdown()

#ifdef CONFIG_DEBUG_ST_THINGS

#ifdef CONFIG_DEBUG_ST_THINGS_WARN
#define THINGS_LOG_W(tag, ...)   things_logv((THINGS_WARNING), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#else //CONFIG_DEBUG_ST_THINGS_WARN
#define THINGS_LOG_W(tag, ...)
#endif

#ifdef CONFIG_DEBUG_ST_THINGS_ERROR
#define THINGS_LOG_E(tag, ...)   things_logv((THINGS_ERROR), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#else //CONFIG_DEBUG_ST_THINGS_ERROR
#define THINGS_LOG_E(tag, ...)
#endif

#ifdef CONFIG_DEBUG_ST_THINGS_DEBUG
#define THINGS_LOG_D(tag, ...)   things_logv((THINGS_DEBUG), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_REQUEST(tag, flag, payload)     things_log_request((THINGS_DEBUG), (tag), __FUNCTION__, __LINE__, flag, payload);
#define THINGS_LOG_RESPONSE(tag, payload)    things_log_response((THINGS_DEBUG), (tag), __FUNCTION__, __LINE__, payload);
#else //CONFIG_DEBUG_ST_THINGS_DEBUG
#define THINGS_LOG_D(tag, ...)
#define THINGS_LOG_REQUEST(tag, flag, payload)
#define THINGS_LOG_RESPONSE(tag, payload)
#endif

#ifdef CONFIG_DEBUG_ST_THINGS_INFO
#define THINGS_LOG_V(tag, ...)   things_logv((THINGS_INFO), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#else //CONFIG_DEBUG_ST_THINGS_INFO
#define THINGS_LOG_V(tag, ...)
#endif

#else //CONFIG_DEBUG_ST_THINGS

#define THINGS_LOG_W(tag, ...)
#define THINGS_LOG_E(tag, ...)
#define THINGS_LOG_D(tag, ...)
#define THINGS_LOG_V(tag, ...)

#endif //CONFIG_DEBUG_ST_THINGS

#define PROFILING_TIME(...)

#ifdef __cplusplus
}
#endif							// __cplusplus
#endif							/* THINGS_LOGGER_H_ */
