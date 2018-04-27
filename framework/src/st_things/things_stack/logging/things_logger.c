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

// if we have unistd.h, we're a Unix system
#include <time.h>
#include <sys/time.h>
#include <debug.h>

#include "things_logger.h"
#include "utils/things_malloc.h"
#include "string.h"

static const char *LEVEL[] __attribute__((unused)) = {
	"DEBUG", "INFOR", "WARNING", "ERROR", "FATAL"
};
static char ts_buf[64] = { 0 };

char *p_version = NULL;

void things_log_init()
{
	/*
	 * Initialize p_version variable
	 */
	p_version = NULL;
}

void things_log_shutdown()
{
	/*
	 * Free the memory allocated for p_version
	 */
	things_free(p_version);
}

void things_log_set_version(char *version)
{
	if (NULL != p_version) {
		things_free(p_version);
	}

	/*
	 * Allocate memory for p_version and set the version information
	 */
	p_version = (char *)things_malloc(strlen(version) + 1);
	if (NULL == p_version) {
		dbg("Error while allocating memory for version");
		return;
	}
	memset(p_version, 0, strlen(version) + 1);
	things_strncpy(p_version, version, strlen(version));
}

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param logStr - log string
 */
void things_log(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *logStr)
{
	struct timeval tv;
	time_t nowtime = 0;
	struct tm *nowtm = NULL;
	char buf[64] = { 0 };

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(buf, sizeof buf, "%m/%d %H:%M:%S", nowtm);

	/* If Version information is not NULL, display the Version Information
	 * Logger Format is given below
	 * [<MM/YY H:M:S.uS> <LOGGER_VERSION>] <LOG_LEVEL> <TAG> <API> <LINE_NO> <LOG_STR>
	 */
	if (p_version) {
		dbg("[%s %s] %s %s %s %d %s\n", buf, p_version, LEVEL[level], tag, func_name, line_num, logStr);
	} else {
		dbg("T%d [%s] %s %s %s %d %s\n", getpid(), buf, LEVEL[level], tag, func_name, line_num, logStr);
	}
}

/**
 * Output a variable argument list log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void things_logv(things_log_level_e level, const char *tag, const char *func_name, const int16_t line_num, const char *format, ...)
{
	if (!format || !tag) {
		return;
	}

	char buffer[MAX_LOG_V_BUFFER_SIZE] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof buffer - 1, format, args);
	va_end(args);
	things_log(level, tag, func_name, line_num, buffer);
}