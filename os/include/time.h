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
/********************************************************************************
 * include/time.h
 *
 *   Copyright (C) 2007-2011, 2013-2014 Gregory Nutt. All rights reserved.
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
 ********************************************************************************/
/**
 * @defgroup TIME_KERNEL TIME
 * @brief Provides APIs for Time
 * @ingroup KERNEL
 */

///@file time.h
///@brief Time APIs

#ifndef __INCLUDE_TIME_H
#define __INCLUDE_TIME_H

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>

/********************************************************************************
 * Pre-processor Definitions
 ********************************************************************************/

/* Clock tick of the system (frequency Hz).
 *
 * NOTE: This symbolic name CLK_TCK has been removed from the standard.  It is
 * replaced with CLOCKS_PER_SEC.  Both are defined here.
 *
 * The default value is 100Hz, but this default setting can be overridden by
 * defining the clock interval in microseconds as CONFIG_USEC_PER_TICK in the
 * board configuration file.
 */
/**
 * @ingroup TIME_KERNEL
 * @{
 */
#ifdef CONFIG_USEC_PER_TICK
#define CLK_TCK           (1000000/CONFIG_USEC_PER_TICK)
#define CLOCKS_PER_SEC    (1000000/CONFIG_USEC_PER_TICK)
#else
#define CLK_TCK           (100)
#define CLOCKS_PER_SEC    (100)
#endif

/* CLOCK_REALTIME refers to the standard time source.  For most
 * implementations, the standard time source is the system timer interrupt.
 * However, if the platform supports an RTC, then the standard time source
 * will be the RTC for the clock_gettime() and clock_settime() interfaces
 * (the system timer is still the time source for all of the interfaces).
 *
 * CLOCK_REALTIME represents the machine's best-guess as to the current
 * wall-clock, time-of-day time. This means that CLOCK_REALTIME can jump
 * forward and backward as the system time-of-day clock is changed.
 */

#define CLOCK_REALTIME     0

/* Clock that cannot be set and represents monotonic time since some
 * unspecified starting point. It is not affected by changes in the
 * system time-of-day clock.
 */

#ifdef CONFIG_CLOCK_MONOTONIC
#define CLOCK_MONOTONIC  1
#endif

/* This is a flag that may be passed to the timer_settime() function */

#define TIMER_ABSTIME      1

#ifndef CONFIG_LIBC_LOCALTIME
/* Local time is the same as gmtime in this implementation */

/**
 * @ingroup TIME_KERNEL
 * @brief  POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define localtime(c)       gmtime(c)
/**
 * @ingroup TIME_KERNEL
 * @brief  POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define localtime_r(c, r)  gmtime_r(c, r)
#endif

/* tm_year of struct tm means years since 1900 */
#define TM_YEAR_BASE       1900

/********************************************************************************
 * Public Types
 ********************************************************************************/

/* Some libc represents time_t intenally as long type.
 * However, type long supports year value within the interval (1901, 2038),
 * whereas  type unsigned int supports the interval (1970, 2106).
 * Therefore, we represent time_t internally as type uint32_t.
 */
typedef uint32_t time_t;		/* Holds time in seconds */
typedef uint8_t clockid_t;		/* Identifies one time base source */
typedef FAR void *timer_t;		/* Represents one POSIX timer */

/**
 * @ingroup TIME_KERNEL
 * @brief structure represents an elapsed time */
struct timespec {
	time_t tv_sec;				/* Seconds */
	long tv_nsec;				/* Nanoseconds */
};

/**
 * @ingroup TIME_KERNEL
 * @brief structure represents an elapsed time */
struct timeval {
	time_t tv_sec;				/* Seconds */
	long tv_usec;				/* Microseconds */
};

struct timezone {
	int tz_minuteswest;			/* minutes west of Greenwich */
	int tz_dsttime;				/* type of DST correction */
};

/**
 * @ingroup TIME_KERNEL
 * @brief Structure containing a calendar date and time */
struct tm {
	int tm_sec;				/* second (0-61, allows for leap seconds) */
	int tm_min;				/* minute (0-59) */
	int tm_hour;				/* hour (0-23) */
	int tm_mday;				/* day of the month (1-31) */
	int tm_mon;				/* month (0-11) */
	int tm_year;				/* years since 1900, TM_YEAR_BASE */
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	int tm_wday;				/* day of the week (0-6) */
	int tm_yday;				/* day of the year (0-365) */
	int tm_isdst;				/* non-0 if daylight savings time is in effect */
#endif
};

/**
 * @ingroup TIME_KERNEL
 * @brief Struct itimerspec is used to define settings for an interval timer */
struct itimerspec {
	struct timespec it_value;	/* First time */
	struct timespec it_interval;	/* and thereafter */
};

/* forward reference (defined in signal.h) */

struct sigevent;

/**
 * @}
 */
/********************************************************************************
 * Public Data
 ********************************************************************************/
#ifdef CONFIG_LIBC_LOCALTIME
/* tzname[] - Timezone strings
 * Setup by tzset()
 */

extern char *tznames[2];
#endif

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * @ingroup TIME_KERNEL
 * @brief clock and timer functions
 * @details @b #include <time.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int clock_settime(clockid_t clockid, FAR const struct timespec *tp);
/**
 * @ingroup TIME_KERNEL
 * @brief clock and timer functions
 * @details @b #include <time.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int clock_gettime(clockid_t clockid, FAR struct timespec *tp);
/**
 * @ingroup TIME_KERNEL
 * @brief clock and timer functions
 * @details @b #include <time.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int clock_getres(clockid_t clockid, FAR struct timespec *res);

/**
 * @ingroup TIME_KERNEL
 * @brief returns the implementation's best approximation to the
 * processor time used by the process since the beginning of a
 * implementation-defined era related only to the process invocation.
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.0
 */
clock_t clock(void);
/**
 * @ingroup TIME_KERNEL
 * @brief convert broken-down time into time since the Epoch
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
time_t mktime(FAR struct tm *tp);
/**
 * @ingroup TIME_KERNEL
 * @brief convert a time value to a broken-down UTC time
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR struct tm *gmtime(FAR const time_t *timer);
/**
 * @ingroup TIME_KERNEL
 * @brief convert a time value to a broken-down UTC time
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR struct tm *gmtime_r(FAR const time_t *timer, FAR struct tm *result);

#ifdef CONFIG_LIBC_LOCALTIME
/**
 * @ingroup TIME_KERNEL
 * @brief convert a time value to a broken-down local time
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR struct tm *localtime(FAR const time_t *timer);
/**
 * @ingroup TIME_KERNEL
 * @brief convert a time value to a broken-down local time
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR struct tm *localtime_r(FAR const time_t *timer, FAR struct tm *result);

#endif
/**
 * @ingroup TIME_KERNEL
 * @brief convert date and time to a string
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
size_t strftime(char *s, size_t max, FAR const char *format, FAR const struct tm *tm);

/**
 * @cond
 * @internal
 */
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
/**
 * @internal
 */
FAR char *asctime(FAR const struct tm *tp);
/**
 * @internal
 */
FAR char *asctime_r(FAR const struct tm *tp, FAR char *buf);
/**
 * @internal
 */
FAR char *ctime(FAR const time_t *timep);
/**
 * @internal
 */
FAR char *ctime_r(FAR const time_t *timep, FAR char *buf);
#endif
/**
 * @endcond
 */

/**
 * @ingroup TIME_KERNEL
 * @brief convert a time string to a time tm structure
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.0
 */
char *strptime(const char *buf, const char *fmt, struct tm *tm);

/**
 * @ingroup TIME_KERNEL
 * @brief calculate time difference
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.0
 */
#ifdef CONFIG_HAVE_DOUBLE
double difftime(time_t time1, time_t time0);
#else
float difftime(time_t time1, time_t time0);
#endif

/**
 * @ingroup TIME_KERNEL
 * @brief get time
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
time_t time(FAR time_t *tloc);

/**
 * @ingroup TIME_KERNEL
 * @brief create a per-process timer
 * @details @b #include <time.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int timer_create(clockid_t clockid, FAR struct sigevent *evp, FAR timer_t *timerid);
/**
 * @ingroup TIME_KERNEL
 * @brief delete a per-process timer
 * @details @b #include <time.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int timer_delete(timer_t timerid);
/**
 * @ingroup TIME_KERNEL
 * @brief per-process timers
 * @details @b #include <time.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int timer_settime(timer_t timerid, int flags, FAR const struct itimerspec *value, FAR struct itimerspec *ovalue);
/**
 * @ingroup TIME_KERNEL
 * @brief per-process timers
 * @details @b #include <time.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int timer_gettime(timer_t timerid, FAR struct itimerspec *value);
/**
 * @cond
 * @internal
 */
int timer_getoverrun(timer_t timerid);
/**
 * @endcond
 */

/**
 * @ingroup TIME_KERNEL
 * @brief high-resolution sleep
 * @details @b #include <time.h> \n
 * SYSTEM CALL API
 *
 * @param[in] rqtp The amount of time to be suspended from execution.
 * @param[in] rmtp If the rmtp argument is non-NULL, the timespec structure
 *                 referenced by it is updated to contain the amount of time
 *                 remaining in the interval (the requested time minus the time
 *                 actually slept)
 * @return On success, remaining time is returned. On failure, ERROR is returned.
 * @since TizenRT v1.0
 */
int nanosleep(FAR const struct timespec *rqtp, FAR struct timespec *rmtp);

/**
 * @cond
 * @internal
 * @ingroup TIME_KERNEL
 * @brief set time conversion information
 * @details @b #include <time.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 */
#ifdef CONFIG_LIBC_LOCALTIME
void tzset(void);
#endif
/**
 * @endcond
 */
#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_TIME_H */
