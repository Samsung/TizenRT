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
/************************************************************************
 * kernel/clock/clock_settime.c
 *
 *   Copyright (C) 2007, 2009, 2011, 2014 Gregory Nutt. All rights reserved.
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <time.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <arch/irq.h>

#include "clock/clock.h"

/************************************************************************
 * Definitions
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
 * Name: clock_settime
 *
 * Description:
 *   Clock Functions based on POSIX APIs
 *
 ************************************************************************/

int clock_settime(clockid_t clock_id, FAR const struct timespec *tp)
{
	struct timespec bias;
	irqstate_t flags;
	int ret = OK;

	svdbg("clock_id=%d\n", clock_id);
	DEBUGASSERT(tp != NULL);

	/* CLOCK_REALTIME - POSIX demands this to be present. This is the wall
	 * time clock.
	 */

	if (clock_id == CLOCK_REALTIME) {
		/* Interrupts are disabled here so that the in-memory time
		 * representation and the RTC setting will be as close as
		 * possible.
		 */

		flags = enter_critical_section();

		/* Save the new base time. */

		g_basetime.tv_sec  = tp->tv_sec;
		g_basetime.tv_nsec = tp->tv_nsec;

		/* Get the elapsed time since power up (in milliseconds).  This is a
		 * bias value that we need to use to correct the base time.
		 */

		(void)clock_systimespec(&bias);

		/* Subtract that bias from the basetime so that when the system
		 * timer is again added to the base time, the result is the current
		 * time relative to basetime.
		 */

		if (g_basetime.tv_nsec < bias.tv_nsec) {
			g_basetime.tv_nsec += NSEC_PER_SEC;
			g_basetime.tv_sec--;
		}

		/* Result could be negative seconds */

		g_basetime.tv_nsec -= bias.tv_nsec;
		g_basetime.tv_sec  -= bias.tv_sec;

		leave_critical_section(flags);

		svdbg("basetime=(%ld,%lu) bias=(%ld,%lu)\n", (long)g_basetime.tv_sec, (unsigned long)g_basetime.tv_nsec, (long)bias.tv_sec, (unsigned long)bias.tv_nsec);
	} else {
		sdbg("Returning ERROR\n");
		set_errno(EINVAL);
		ret = ERROR;
	}

	return ret;
}
