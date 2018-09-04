/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * apps/system/utils/kdbg_date.c
 *
 *   Copyright (C) 2011-2012, 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include <tinyara/time.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define MAX_TIME_STRING         80
#define STR_LEN_OF_MONTH_WEEK   3
#define MAX_EPOCH_YEAR_OF_UINT  2106
#define EPOCH_YEAR_CONST		(EPOCH_YEAR - TM_YEAR_BASE)

#define DATE_ERROUT(errmsg) printf("date: cannot set date: %s\n", errmsg); goto errout;

/****************************************************************************
 * Private Data
 ****************************************************************************/

static FAR const char *const g_datemontab[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
static FAR const char *const g_dayofweek[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
#endif

static const unsigned int g_dayofmonth[] = {
	31, 28, 31, 30, 31, 30,
	31, 31, 30, 31, 30, 31
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline int date_month(FAR const char *abbrev)
{
	int month;

	for (month = 0; month < 12; month++) {
		if (strncasecmp(g_datemontab[month], abbrev, STR_LEN_OF_MONTH_WEEK) == 0) {
			return month;
		}
	}

	return ERROR;
}

#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
static inline int day_of_week(FAR const char *abbrev)
{
	int wday;

	for (wday = 0; wday < 7; wday++) {
		if (strncasecmp(g_dayofweek[wday], abbrev, STR_LEN_OF_MONTH_WEEK) == 0) {
			return wday;
		}
	}

	return ERROR;
}
#endif

static inline int date_showtime(void)
{
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	static const char format[] = "%b %d %H:%M:%S %Y %a %j";
#else
	static const char format[] = "%b %d %H:%M:%S %Y";
#endif
	struct timespec ts;
	struct tm tm;
	char timbuf[MAX_TIME_STRING];
	int ret;

	/* Get the current time */

	ret = clock_gettime(CLOCK_REALTIME, &ts);
	if (ret < 0) {
		printf("clock_gettime failed\n");
		return ERROR;
	}

	/* Break the current time up into the format needed by strftime */

	(void)gmtime_r((FAR const time_t *)&ts.tv_sec, &tm);

	/* Show the current time in the requested format */

	(void)strftime(timbuf, MAX_TIME_STRING, format, &tm);
	printf("%s\n", timbuf);

	return OK;
}

static inline int date_settime(const char *tbuf)
{
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	const char format[] = "%b %d %H:%M:%S %Y %a %j";
#else
	const char format[] = "%b %d %H:%M:%S %Y";
#endif
	struct timespec ts;
	struct tm tm = {0};
	char *res = NULL;

	res = strptime(tbuf, format, &tm);
	if (NULL == res) {
		DATE_ERROUT("Invalid date-time argument!\n");
	} else if (tm.tm_year < EPOCH_YEAR_CONST || tm.tm_year > MAX_EPOCH_YEAR_OF_UINT) {
		DATE_ERROUT("Invalid year in the argument!\n");
	} else if ((tm.tm_mon != 1) ?
	(tm.tm_mday > g_dayofmonth[tm.tm_mon]) :
	(clock_isleapyear(tm.tm_year + TM_YEAR_BASE) ?
	tm.tm_mday > (g_dayofmonth[tm.tm_mon] + 1) :
	tm.tm_mday > g_dayofmonth[tm.tm_mon])) {
		DATE_ERROUT("Invalid day of the month in the argument!\n");
	}
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	int dst_val = 0;
	char *dst = (char *)tbuf;
	while ((dst_val < 6) && (dst = strchr((const char*)dst, ' '))) {
		dst++;
		dst_val++;
	}

	if (NULL != dst) {
		dst_val = (int)strtol(dst, NULL, 10);
		if (dst_val < 0 || dst_val > 1) {
			DATE_ERROUT("Invalid day light savings flag in the argument!\n");
		}
		tm.tm_isdst = dst_val;
	}
#endif

	ts.tv_sec = mktime(&tm);
	ts.tv_nsec = 0;

	if (clock_settime(CLOCK_REALTIME, &ts) < 0) {
		DATE_ERROUT("clock_settime failed\n");
	}

	return OK;

errout:
	return ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int kdbg_date(int argc, char **args)
{
	int ret;

	/* Get the date options:  date [-s "MMM DD HH:MM:SS YYYY"] */

	if (argc == 1) {
		ret = date_showtime();
	} else if (!strncmp(args[1], "-s", strlen("-s") + 1) && argc == 3) {
		ret = date_settime(args[2]);
	} else {

		printf("\nUsage: date\n");
		printf("   or: date [-s \"FORMAT\"]\n");
		printf("Display, or Set system time and date information\n");
		printf("\nOptions:\n");
		printf(" -s \"FORMAT\"     Set system time in the given \"FORMAT\"\n");
#if !(defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED))
		printf("               FORMAT: \"MMM DD HH:MM:SS YYYY\"\n");
		printf("                'month', 'day', 'hour':'minute':'second' 'year'\n");
		printf("                Example: \"Apr 21 10:35:22 1991\"\n");
#else
		printf("               FORMAT: \"MMM DD HH:MM:SS YYYY DWR DYR D\"\n");
		printf("                'month', 'day', 'hour':'minute':'second' 'year' 'day_of_week' 'day_of_year' 'daylight_savings_flag'\n");
		printf("                Example: \"Apr 21 10:35:22 1991 Sun 175 1\"\n");

		printf("                Day of Week Range(DWR) = [Sunday = Sun, Monday = Mon, and so on...]\n");
		printf("                Day of Year Range(DYR) = [0,365], \n");
		printf("                daylight_savings_flag: +ve(in effect), 0(low), -ve(information unavailable)\n");
#endif
		printf("                Year valid range = [1970,2106]\n");

		ret = ERROR;
	}
	return ret;
}
