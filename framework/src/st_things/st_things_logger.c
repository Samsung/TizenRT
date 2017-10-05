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

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "st_things_logger.h"
#include "string.h"

#ifndef __TIZEN__
static const uint16_t LINE_BUFFER_SIZE = 255;
static const char *LEVEL[] __attribute__ ((unused)) = {
"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};

static char *p_version = NULL;

void st_things_log_init()
{
	/* Initialize p_version variable */
	p_version = NULL;
}

void st_things_log_shutdown()
{
	/* Free the memory allocated for p_version */
	free(p_version);
}

void st_things_log_set_version(char *version)
{
	if (NULL == version) {
		return;
	}

	if (NULL != p_version) {
		free(p_version);
	}

	/* Allocate memory for p_version and set the version information */
	p_version = (char *)malloc(strlen(version) + 1);
	if (NULL == p_version) {
		printf("Error while allocating memory for version");
		return;
	}
	memset(p_version, 0, strlen(version) + 1);
	strncpy(p_version, version, strlen(version) + 1);
}

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - ST_DEBUG, ST_INFO, ST_WARNING, ST_ERROR, ST_FATAL
 * @param tag    - Module name
 * @param log_str - log string
 */
void st_things_log(st_things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *log_str)
{
	if (!log_str || !tag) {
		return;
	}

	struct timeval tv;
	time_t now_time = 0;
	struct tm *now_tm = NULL;
	char tm_buf[64] = { 0 };

	gettimeofday(&tv, NULL);
	now_time = tv.tv_sec;
	now_tm = localtime(&now_time);
	strftime(tm_buf, sizeof tm_buf, "%m/%d %H:%M:%S", now_tm);

	if (p_version) {
		printf("[%s %s] %s %s %s %d %s\n", tm_buf, p_version, LEVEL[level], tag, func_name, line_num, log_str);
	} else {
		printf("[%s] %s %s %s %d %s\n", tm_buf, LEVEL[level], tag, func_name, line_num, log_str);
	}
}

/**
 * Output a variable argument list log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - ST_DEBUG, ST_INFO, ST_WARNING, ST_ERROR, ST_FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void st_things_log_v(st_things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *format, ...)
{
	if (!format || !tag) {
		return;
	}

	/* Using Variable arguments to extract each argument and passing it st_things_log API */
	char buffer[MAX_LOG_V_BUFFER_SIZE] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof buffer - 1, format, args);
	va_end(args);
	st_things_log(level, tag, func_name, line_num, buffer);
}
#endif							//#ifndef __TIZEN__
