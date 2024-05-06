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
 * kernel/clock/clock_initialize.c
 *
 *   Copyright (C) 2007, 2009, 2011-2012 Gregory Nutt. All rights reserved.
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
#include <tinyara/compiler.h>

#include <stdint.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <debug.h>

#ifdef CONFIG_RTC
#include <arch/irq.h>
#endif

#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/time.h>
#include <tinyara/rtc.h>
#ifdef CONFIG_INIT_SYSTEM_TIME
#include <tinyara/version.h>
#endif

#include "clock/clock.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
/* Standard time definitions (in units of seconds) */

#define SEC_PER_MIN  ((time_t)60)
#define SEC_PER_HOUR ((time_t)60 * SEC_PER_MIN)
#define SEC_PER_DAY  ((time_t)24 * SEC_PER_HOUR)

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/**************************************************************************
 * Public Constant Data
 **************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

#ifndef CONFIG_SCHED_TICKLESS
volatile clock_t g_system_timer;
#endif

struct timespec g_basetime;

/**************************************************************************
 * Private Variables
 **************************************************************************/

/**************************************************************************
 * Private Functions
 **************************************************************************/

/****************************************************************************
 * Name: clock_basetime
 *
 * Description:
 *   Get the initial time value from the best source available.
 *
 ****************************************************************************/

#ifdef CONFIG_RTC
#if defined(CONFIG_RTC_DATETIME)
/* Initialize the system time using a broken out date/time structure */

static inline void clock_basetime(FAR struct timespec *tp)
{
	struct tm rtctime;

	/* Get the broken-out time from the date/time RTC. */
	(void)up_rtc_getdatetime(&rtctime);

	/* And use the broken-out time to initialize the system time */

	tp->tv_sec  = mktime(&rtctime);
	tp->tv_nsec = 0;
}

#elif defined(CONFIG_RTC_HIRES)

/* Initialize the system time using a high-resolution structure */

static inline void clock_basetime(FAR struct timespec *tp)
{
	/* Get the complete time from the hi-res RTC. */

	(void)up_rtc_gettime(tp);
}

#else

/* Initialize the system time using seconds only */

static inline void clock_basetime(FAR struct timespec *tp)
{
	/* Get the seconds (only) from the lo-resolution RTC */

	tp->tv_sec  = up_rtc_time();
	tp->tv_nsec = 0;
}

#endif							/* CONFIG_RTC_HIRES */
#else							/* CONFIG_RTC */

static inline void clock_basetime(FAR struct timespec *tp)
{
	time_t jdn = 0;

	/* Get the EPOCH-relative julian date from the calendar year,
	 * month, and date
	 */

	jdn = clock_calendar2utc(CONFIG_START_YEAR, CONFIG_START_MONTH - 1, CONFIG_START_DAY);

	/* Set the base time as seconds into this julian day. */

	tp->tv_sec  = jdn * SEC_PER_DAY;
	tp->tv_nsec = 0;
}

#endif							/* CONFIG_RTC */

/****************************************************************************
 * Name: clock_inittime
 *
 * Description:
 *   Get the initial time value from the best source available.
 *
 ****************************************************************************/

static void clock_inittime(void)
{
	/* (Re-)initialize the time value to match the RTC */

	clock_basetime(&g_basetime);
#ifndef CONFIG_SCHED_TICKLESS
	g_system_timer = 0;
#endif
}

/****************************************************************************
 * Name: initialize_system_time
 *
 * Description:
 *   Initialize the system time based on VERSION_BUILD_TIME.
 *
 ****************************************************************************/

#ifdef CONFIG_INIT_SYSTEM_TIME
#ifdef CONFIG_INIT_SYSTEM_TIME_WITH_MIDNIGHT
#define SYSTIME_INIT_TIME_FORMAT "%Y-%m-%d"
#else
#define SYSTIME_INIT_TIME_FORMAT "%Y-%m-%d %T"
#endif

static void initialize_system_time(void)
{
	struct tm init_time;
	struct timespec ts;
	char *ret;

	/* Initialize the members not used in strptime */

#ifdef CONFIG_INIT_SYSTEM_TIME_WITH_MIDNIGHT
	init_time.tm_sec = 0;
	init_time.tm_min = 0;
	init_time.tm_hour = 0;
#endif
	init_time.tm_wday = 0;
	init_time.tm_yday = 0;
	init_time.tm_isdst = 0;

	/* Get build date */

	ret = strptime(CONFIG_VERSION_BUILD_TIME, SYSTIME_INIT_TIME_FORMAT, &init_time);
	if (ret == NULL) {
		return;
	}

	/* Convert struct tm to struct timespec */

	ts.tv_sec = mktime(&init_time);
	ts.tv_nsec = 0;

	/* Set system init time for rtc */

	(void)up_rtc_settime(&ts);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: clock_initialize
 *
 * Description:
 *   Perform one-time initialization of the timing facilities.
 *
 ****************************************************************************/

void clock_initialize(void)
{
	/* Initialize the RTC hardware */

#if defined(CONFIG_RTC)
	up_rtc_initialize();
#ifdef CONFIG_INIT_SYSTEM_TIME
	initialize_system_time();
#endif
#endif

	/* Initialize the time value to match the RTC */

	clock_inittime();
}

/****************************************************************************
 * Name: clock_synchronize
 *
 * Description:
 *   Synchronize the system timer to a hardware RTC.  This operation is
 *   normally performed automatically by the system during clock
 *   initialization.  However, the user may also need to explicitly re-
 *   synchronize the system timer to the RTC under certain conditions where
 *   the system timer is known to be in error.  For example, in certain low-
 *   power states, the system timer may be stopped but the RTC will continue
 *   keep correct time.  After recovering from such low-power state, this
 *   function should be called to restore the correct system time.
 *
 *   Calling this function could result in system time going "backward" in
 *   time, especially with certain lower resolution RTC implementations.
 *   Time going backward could have bad consequences if there are ongoing
 *   timers and delays.  So use this interface with care.
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef CONFIG_RTC
void clock_synchronize(void)
{
	irqstate_t flags;

	/* Re-initialize the time value to match the RTC */

	flags = enter_critical_section();
	clock_inittime();
	leave_critical_section(flags);
}
#endif

/****************************************************************************
 * Name: clock_timer
 *
 * Description:
 *   This function must be called once every time the real time clock
 *   interrupt occurs.  The interval of this clock interrupt must be
 *   USEC_PER_TICK
 *
 ****************************************************************************/

#ifndef CONFIG_SCHED_TICKLESS
void clock_timer(void)
{
	/* Increment the per-tick system counter */

	g_system_timer++;
}

#ifdef CONFIG_SCHED_TICKSUPPRESS
void clock_timer_nohz(clock_t ticks)
{
	g_system_timer += ticks;
}
#endif
#endif
