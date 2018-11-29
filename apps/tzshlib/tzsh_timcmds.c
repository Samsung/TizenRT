/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * apps/tzshlib/dbg_timcmds.c
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

#include <tinyara/config.h>

#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MAX_TIME_STRING 80

#ifdef CONFIG_CLOCK_MONOTONIC
#define TIME_CLOCK CLOCK_MONOTONIC
#else
#define TIME_CLOCK CLOCK_REALTIME
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifndef  CONFIG_TZSH_DISABLE_DATE
static FAR const char *const g_datemontab[] = {
	"jan", "feb", "mar", "apr", "may", "jun",
	"jul", "aug", "sep", "oct", "nov", "dec"
};
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: date_month
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_DATE
static inline int date_month(FAR const char *abbrev)
{
	int i;

	for (i = 0; i < 12; i++) {
		if (strncasecmp(g_datemontab[i], abbrev, 3) == 0) {
			return i;
		}
	}
	return ERROR;
}
#endif

/****************************************************************************
 * Name: date_gettime
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_DATE
static inline int date_showtime(FAR struct tzsh_vtbl_s *vtbl, FAR const char *name)
{
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	static const char format[] = "%a, %b %d %H:%M:%S %Y";
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
		tzsh_output(vtbl, g_fmtcmdfailed, name, "clock_gettime", TZSH_ERRNO);
		return ERROR;
	}

	/* Break the current time up into the format needed by strftime */

	if (gmtime_r((FAR const time_t *)&ts.tv_sec, &tm) == NULL) {
		tzsh_output(vtbl, g_fmtcmdfailed, name, "gmtime_r", TZSH_ERRNO);
		return ERROR;
	}

	/* Show the current time in the requested format */

	ret = strftime(timbuf, MAX_TIME_STRING, format, &tm);
	if (ret < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, name, "strftime", TZSH_ERRNO);
		return ERROR;
	}

	tzsh_output(vtbl, "%s\n", timbuf);
	return OK;
}
#endif

/****************************************************************************
 * Name: date_settime
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_DATE
static inline int date_settime(FAR struct tzsh_vtbl_s *vtbl, FAR const char *name, FAR char *newtime)
{
	struct timespec ts;
	struct tm tm;
	FAR char *token;
	FAR char *saveptr;
	long result;
	int ret;

	/* Only this date format is supported: MMM DD HH:MM:SS YYYY */
	/* Get the month abbreviation */

	token = strtok_r(newtime, " \t", &saveptr);
	if (token == NULL) {
		goto errout_bad_parm;
	}

	tm.tm_mon = date_month(token);
	if (tm.tm_mon < 0) {
		goto errout_bad_parm;
	}

	/* Get the day of the month.  NOTE: Accepts day-of-month up to 31 for all months */

	token = strtok_r(NULL, " \t", &saveptr);
	if (token == NULL) {
		goto errout_bad_parm;
	}

	result = strtol(token, NULL, 10);
	if (result < 1 || result > 31) {
		goto errout_bad_parm;
	}

	tm.tm_mday = (int)result;

	/* Get the hours */

	token = strtok_r(NULL, " \t:", &saveptr);
	if (token == NULL) {
		goto errout_bad_parm;
	}

	result = strtol(token, NULL, 10);
	if (result < 0 || result > 23) {
		goto errout_bad_parm;
	}

	tm.tm_hour = (int)result;

	/* Get the minutes */

	token = strtok_r(NULL, " \t:", &saveptr);
	if (token == NULL) {
		goto errout_bad_parm;
	}

	result = strtol(token, NULL, 10);
	if (result < 0 || result > 59) {
		goto errout_bad_parm;
	}

	tm.tm_min = (int)result;

	/* Get the seconds */

	token = strtok_r(NULL, " \t:", &saveptr);
	if (token == NULL) {
		goto errout_bad_parm;
	}

	result = strtol(token, NULL, 10);
	if (result < 0 || result > 61) {
		goto errout_bad_parm;
	}

	tm.tm_sec = (int)result;

	/* And finally the year */

	token = strtok_r(NULL, " \t", &saveptr);
	if (token == NULL) {
		goto errout_bad_parm;
	}

	result = strtol(token, NULL, 10);
	if (result < 1900 || result > 2100) {
		goto errout_bad_parm;
	}

	tm.tm_year = (int)result - 1900;

	/* Convert this to the right form, then set the timer */

	ts.tv_sec = mktime(&tm);
	ts.tv_nsec = 0;

	ret = clock_settime(CLOCK_REALTIME, &ts);
	if (ret < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, name, "clock_settime", TZSH_ERRNO);
		return ERROR;
	}

	return OK;

errout_bad_parm:
	tzsh_output(vtbl, g_fmtarginvalid, name);
	return ERROR;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cmd_time
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_TIME
int cmd_time(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	struct timespec start;
#ifndef CONFIG_TZSH_DISABLEBG
	bool bgsave;
#endif
#if CONFIG_NFILE_STREAMS > 0
	bool redirsave;
#endif
	int ret;

	/* Get the current time */

	ret = clock_gettime(TIME_CLOCK, &start);
	if (ret < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "clock_gettime", TZSH_ERRNO);
		return ERROR;
	}

	/* Save state */

#ifndef CONFIG_TZSH_DISABLEBG
	bgsave = vtbl->np.np_bg;
#endif
#if CONFIG_NFILE_STREAMS > 0
	redirsave = vtbl->np.np_redirect;
#endif

	/* Execute the command */

	ret = tzsh_parse(vtbl, argv[1]);
	if (ret >= 0) {
		struct timespec end;
		struct timespec diff;

		/* Get and print the elapsed time */

		ret = clock_gettime(TIME_CLOCK, &end);
		if (ret < 0) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "clock_gettime", TZSH_ERRNO);
			ret = ERROR;
		} else {
			diff.tv_sec = end.tv_sec - start.tv_sec;
			if (start.tv_nsec > end.tv_nsec) {
				diff.tv_sec--;
				end.tv_nsec += 1000000000;
			}

			diff.tv_nsec = end.tv_nsec - start.tv_nsec;
			tzsh_output(vtbl, "\n%lu.%04lu sec\n", (unsigned long)diff.tv_sec, (unsigned long)diff.tv_nsec / 100000);
		}
	}

	/* Restore state */

#ifndef CONFIG_TZSH_DISABLEBG
	vtbl->np.np_bg = bgsave;
#endif
#if CONFIG_NFILE_STREAMS > 0
	vtbl->np.np_redirect = redirsave;
#endif

	return ret;
}
#endif

/****************************************************************************
 * Name: cmd_date
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_DATE
int cmd_date(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR char *newtime = NULL;
	FAR const char *errfmt;
	bool badarg = false;
	int option;
	int ret;

	/* Get the date options:  date [-s time] [+FORMAT] */

	while ((option = getopt(argc, argv, "s:")) != ERROR) {
		if (option == 's') {
			/* We will be setting the time */

			newtime = optarg;
		} else {				/* option = '?' */

			/* We need to parse to the end anyway so that getopt stays healthy */

			badarg = true;
		}
	}

	/* If a bad argument was encountered then exit with an error */

	if (badarg) {
		errfmt = g_fmtarginvalid;
		goto errout;
	}

	/* optind < argc-1 means that there are additional, unexpected arguments on
	 * th command-line
	 */

	if (optind < argc) {
		errfmt = g_fmttoomanyargs;
		goto errout;
	}

	/* Display or set the time */

	if (newtime) {
		ret = date_settime(vtbl, argv[0], newtime);
	} else {
		ret = date_showtime(vtbl, argv[0]);
	}

	return ret;

errout:
	tzsh_output(vtbl, errfmt, argv[0]);
	return ERROR;
}
#endif
