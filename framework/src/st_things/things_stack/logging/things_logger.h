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
void things_log_init();

/**
 * Called to Free dyamically allocated resources used with custom logging.
 * Not necessary if default logging is used
 *
 */
void things_log_shutdown();

/**
 * Set the Version.
 */
void things_log_set_version(char *version);

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param log_str - log string
 */
void things_log(things_log_level_e level, const char *tag, const char *funcName, const int16_t line_num, const char *log_str);

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

const char *__get_timestamp__();

#ifdef TB_LOG
#define THINGS_LOG_INIT()   things_log_init()
#define THINGS_LOG_SHUTDOWN()   things_log_shutdown()
#define THINGS_LOG(level, tag, log_str)  things_log((level), (tag), __FUNCTION__, __LINE__, (log_str))
#define THINGS_LOG_D(level, tag, ...)   things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_V(level, tag, ...)   things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define PROFILING_TIME(...)
#else							//TB_LOG
/*
 * Merged RELEASE & LOGGING option into RELEASE
 * THINGS_LOG_D will be disabled in RELEASE mode
 * THINGS_LOG & THINGS_LOG_V will be enabled in RELEASE mode
 *
 */
#define THINGS_LOG_CONFIG(ctx)
#define THINGS_LOG_SHUTDOWN()

#define THINGS_LOG(level, tag, log_str)  things_log((level), (tag), __FUNCTION__, __LINE__, (log_str))
#define THINGS_LOG_V(level, tag, ...)   things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_D(level, tag, ...)   things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)

#define THINGS_LOG_DEBUG(level, tag, log_str)  things_log((level), (tag), __FUNCTION__, __LINE__, (log_str))
#define THINGS_LOG_INFO(level, tag, log_str)  things_log((level), (tag), __FUNCTION__, __LINE__, (log_str))
#define THINGS_LOG_WARNING(level, tag, log_str)  things_log((level), (tag), __FUNCTION__, __LINE__, (log_str))
#define THINGS_LOG_ERROR(level, tag, log_str)  things_log((level), (tag), __FUNCTION__, __LINE__, (log_str))
#define THINGS_LOG_FATAL(level, tag, log_str)  things_log((level), (tag), __FUNCTION__, __LINE__, (log_str))

#define THINGS_LOG_V_DEBUG(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_V_INFO(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_V_WARNING(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_V_ERROR(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_V_FATAL(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)

#define THINGS_LOG_D_DEBUG(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_D_INFO(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_D_WARNING(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_D_ERROR(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
#define THINGS_LOG_D_FATAL(level, tag, ...)  things_logv((level), (tag), __FUNCTION__, __LINE__, __VA_ARGS__)
// Fot set loglevel //

#define PROFILING_TIME(...)
#define THINGS_LOG_INIT()
// #define ENABLE_MALLOC_DEBUG
#endif							// TB_LOG

#ifdef __cplusplus
}
#endif							// __cplusplus
#endif							/* THINGS_LOGGER_H_ */
