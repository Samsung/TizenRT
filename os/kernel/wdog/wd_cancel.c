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
 * kernel/wdog/wd_cancel.c
 *
 *   Copyright (C) 2007-2009, 2014 Gregory Nutt. All rights reserved.
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

#include <stdbool.h>
#include <assert.h>

#include <tinyara/arch.h>
#include <tinyara/wdog.h>

#include "sched/sched.h"
#include "wdog/wdog.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wd_cancel
 *
 * Description:
 *   This function cancels a currently running watchdog timer. Watchdog
 *   timers may be cancelled from the interrupt level.
 *
 * Parameters:
 *   wdog - ID of the watchdog to cancel.
 *
 * Return Value:
 *   OK or ERROR
 *
 * Assumptions:
 *
 ****************************************************************************/

int wd_cancel(WDOG_ID wdog)
{
	FAR struct wdog_s *curr;
	FAR struct wdog_s *prev;
	irqstate_t state;
	int ret = ERROR;

	/* Prohibit timer interactions with the timer queue until the
	 * cancellation is complete
	 */

	state = enter_critical_section();

	/* Make sure that the watchdog is initialized (non-NULL) and is still
	 * active.
	 */

	if (wdog && WDOG_ISACTIVE(wdog)) {
		/* Search the g_wdactivelist for the target FCB.  We can't use sq_rem
		 * to do this because there are additional operations that need to be
		 * done.
		 */

		prev = NULL;
		curr = (FAR struct wdog_s *)g_wdactivelist.head;

		while ((curr) && (curr != wdog)) {
			prev = curr;
			curr = curr->next;
		}

		/* Check if the watchdog was found in the list.  If not, then an OS
		 * error has occurred because the watchdog is marked active!
		 */

		ASSERT(curr);

		/* If there is a watchdog in the timer queue after the one that
		 * is being cancelled, then it inherits the remaining ticks.
		 */

		if (curr->next) {
			curr->next->lag += curr->lag;
		}

		/* Now, remove the watchdog from the timer queue */

		if (prev) {
			/* Remove the watchdog from mid- or end-of-queue */

			(void)sq_remafter((FAR sq_entry_t *)prev, &g_wdactivelist);
		} else {
			/* Remove the watchdog at the head of the queue */

			(void)sq_remfirst(&g_wdactivelist);

			/* Reassess the interval timer that will generate the next
			 * interval event.
			 */

			sched_timer_reassess();
		}

		/* Mark the watchdog inactive */

		wdog->next = NULL;
		WDOG_CLRACTIVE(wdog);

		/* Return success */

		ret = OK;
	}

	leave_critical_section(state);
	return ret;
}
