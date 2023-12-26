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
 * kernel/wdog/wd_create.c
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
#include <queue.h>

#include <tinyara/arch.h>
#include <tinyara/wdog.h>
#include <tinyara/kmalloc.h>

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
 * Name: wd_create
 *
 * Description:
 *   The wd_create function will create a watchdog by allocating it from the
 *   list of free watchdogs.
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   Pointer to watchdog (i.e., the watchdog ID), or NULL if insufficient
 *   watchdogs are available.
 *
 * Assumptions:
 *
 ****************************************************************************/

WDOG_ID wd_create(void)
{
	FAR struct wdog_s *wdog;
	irqstate_t state;

	/* These actions must be atomic with respect to other tasks and also with
	 * respect to interrupt handlers that may be allocating or freeing watchdog
	 * timers.
	 */

	state = enter_critical_section();

	/* If we are in an interrupt handler -OR- if the number of pre-allocated
	 * timer structures exceeds the reserve, then take the next timer from
	 * the head of the free list.
	 */

	if (g_wdnfree > CONFIG_WDOG_INTRESERVE || up_interrupt_context()) {
		/* Remove the watchdog timer from the free list and decrement the
		 * count of free timers all with interrupts disabled.
		 */

		wdog = (FAR struct wdog_s *)sq_remfirst(&g_wdfreelist);

		/* Did we get one? */

		if (wdog) {
			DEBUGASSERT(g_wdnfree > 0);
			g_wdnfree--;

			/* Yes.. Clear the forward link and all flags */

			wdog->next = NULL;
			wdog->flags = 0;
		} else {
			/* If wdog is Null, g_wdnfree must be zero, else assert */
			DEBUGASSERT(g_wdnfree == 0);
		}
		leave_critical_section(state);
	}

	/* We are in a normal tasking context AND there are not enough unreserved,
	 * pre-allocated watchdog timers.  We need to allocate one from the kernel
	 * heap.
	 */

	else {
		/* We do not require that interrupts be disabled to do this. */

		leave_critical_section(state);
		wdog = (FAR struct wdog_s *)kmm_malloc(sizeof(struct wdog_s));

		/* Did we get one? */

		if (wdog) {
			/* Yes.. Clear the forward link and set the allocated flag */

			wdog->next = NULL;
			wdog->flags = WDOGF_ALLOCED;
		}
	}

	return (WDOG_ID)wdog;
}
