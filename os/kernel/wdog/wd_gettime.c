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
 * kernel/wdog/wd_gettime.c
 *
 *   Copyright (C) 2007, 2009, 2014 Gregory Nutt. All rights reserved.
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

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>

#include <tinyara/wdog.h>

#include "wdog/wdog.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

/********************************************************************************
 * Private Types
 ********************************************************************************/

/********************************************************************************
 * Global Variables
 ********************************************************************************/

/********************************************************************************
 * Private Variables
 ********************************************************************************/

/********************************************************************************
 * Private Functions
 ********************************************************************************/

/********************************************************************************
 * Public Functions
 ********************************************************************************/

/********************************************************************************
 * Name: wd_gettime
 *
 * Description:
 *   This function returns the time remaining before the specified watchdog
 *   expires.
 *
 * Parameters:
 *   wdog = watchdog ID
 *
 * Return Value:
 *   The time in system ticks remaining until the watchdog time expires.  Zero
 *   means either that wdog is not valid or that the wdog has already expired.
 *
 * Assumptions:
 *
 ********************************************************************************/

int wd_gettime(WDOG_ID wdog)
{
	irqstate_t flags;

	/* Verify the wdog */

	flags = enter_critical_section();
	if (wdog && WDOG_ISACTIVE(wdog)) {
		/* Traverse the watchdog list accumulating lag times until we find the wdog
		 * that we are looking for
		 */

		FAR struct wdog_s *curr;
		int delay = 0;

		for (curr = (FAR struct wdog_s *)g_wdactivelist.head; curr; curr = curr->next) {
			delay += curr->lag;
			if (curr == wdog) {
				leave_critical_section(flags);
				return delay;
			}
		}
	}

	leave_critical_section(flags);
	return 0;
}

#ifdef CONFIG_SCHED_TICKSUPPRESS
/********************************************************************************
 * Name: wd_getdelay
 *
 * Description:
 *  This function returns delay (in system ticks) from head of wdog active list.
 *  This function is provided by RTOS and called from platform-specific code.
 *
 * Parameters:
 *	None
 *
 * Return Value:
 *  wdog delay in system ticks
 *
 * Assumptions:
 *
 ********************************************************************************/

int wd_getdelay(void)
{
	return (g_wdactivelist.head) ? ((FAR struct wdog_s *)g_wdactivelist.head)->lag : 0;
}
#endif
