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
 * kernel/clock/clock_gettime.c
 *
 *   Copyright (C) 2007, 2009, 2011, 2014, 2016 Gregory Nutt. All rights reserved.
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

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>
#include <tinyara/rtc.h>
#include <tinyara/spinlock.h>

#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <arch/irq.h>

#include "clock/clock.h"

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Private Function Prototypes
 ************************************************************************/

/**********************************************************************
 * Public Constant Data
 **********************************************************************/

/************************************************************************
 * Public Variables
 ************************************************************************/

/**********************************************************************
 * Private Variables
 **********************************************************************/

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: clock_gettime
 *
 * Description:
 *   Clock Functions based on POSIX APIs
 *
 ************************************************************************/

int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
	struct timespec ts;
	uint32_t carry;
	int ret = OK;

	svdbg("clock_id=%d\n", clock_id);
	DEBUGASSERT(tp != NULL);

#ifdef CONFIG_CLOCK_MONOTONIC
	/* CLOCK_MONOTONIC is an optional under POSIX: "If the Monotonic Clock
	 * option is supported, all implementations shall support a clock_id
	 * of CLOCK_MONOTONIC defined in <time.h>. This clock represents the
	 * monotonic clock for the system. For this clock, the value returned
	 * by clock_gettime() represents the amount of time (in seconds and
	 * nanoseconds) since an unspecified point in the past (for example,
	 * system start-up time, or the Epoch). This point does not change
	 * after system start-up time. The value of the CLOCK_MONOTONIC clock
	 * cannot be set via clock_settime(). This function shall fail if it
	 * is invoked with a clock_id argument of CLOCK_MONOTONIC."
	 */

	if (clock_id == CLOCK_MONOTONIC) {
		/* The the time elapsed since the timer was initialized at power on
		 * reset.
		 */

		ret = clock_systimespec(tp);
	} else
#endif

		/* CLOCK_REALTIME - POSIX demands this to be present.  CLOCK_REALTIME
		 * represents the machine's best-guess as to the current wall-clock,
		 * time-of-day time. This means that CLOCK_REALTIME can jump forward and
		 * backward as the system time-of-day clock is changed.
		 */

		if (clock_id == CLOCK_REALTIME) {
			/* Get the elapsed time since the time-of-day was last set.
			 * clock_systimespec() provides the time since power was applied;
			 * the bias value corresponds to the time when the time-of-day was
			 * last set.
			 */

			ret = clock_systimespec(&ts);
			if (ret == OK) {
				/* Add the base time to this.  The base time is the time-of-day
				 * setting.  When added to the elapsed time since the time-of-day
				 * was last set, this gives us the current time.
				 */
				irqstate_t flags;
				flags = spin_lock_irqsave(NULL);
				ts.tv_sec  += (uint32_t)g_basetime.tv_sec;
				ts.tv_nsec += (uint32_t)g_basetime.tv_nsec;
				spin_unlock_irqrestore(NULL, flags);

				/* Handle carry to seconds. */

				if (ts.tv_nsec >= NSEC_PER_SEC) {
					carry       = ts.tv_nsec / NSEC_PER_SEC;
					ts.tv_sec  += carry;
					ts.tv_nsec -= (carry * NSEC_PER_SEC);
				}

				/* And return the result to the caller. */

				tp->tv_sec  = ts.tv_sec;
				tp->tv_nsec = ts.tv_nsec;
			}
		} else {
			ret = -EINVAL;
		}

	/* Check for errors and set the errno value if necessary */

	if (ret < 0) {
		sdbg("Returning ERROR\n");

		set_errno(-ret);
		ret = ERROR;
	} else {
		svdbg("Returning tp=(%d,%d)\n", (int)tp->tv_sec, (int)tp->tv_nsec);
	}

	return ret;
}
