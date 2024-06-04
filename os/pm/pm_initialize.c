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
 * pm/pm_initialize.c
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
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
#include <semaphore.h>
#include <debug.h>
#include <tinyara/pm/pm.h>
#include <time.h>
#include "pm_test.h"
#include "pm.h"
#include "pm_metrics.h"
#include "pm_timer/pm_timer.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* All PM global data: */

/* Initialize the registry and the PM global data structures.  The PM
 * global data structure resides in .data which is zeroed at boot time.  So
 * it is only required to initialize non-zero elements of the PM global
 * data structure here.
 */

struct pm_global_s g_pmglobals;

/* The g_pm_timer_freelist is a singly linked list of pm timers available
 * to the system */

sq_queue_t g_pm_timer_freelist;

/* The g_pm_timer_activelist data structure is a singly linked list ordered by
 * pm wakeup timer expiration time.
 */

sq_queue_t g_pm_timer_activelist;

/* This is the number of free, pre-allocated pm wakeup timer structures in the
 * g_pm_timer_freelist. 
 */

uint16_t g_pm_timer_nfree;

/* g_pm_list is a list of pre-allocated pm wakeup timers. The number of pm timers
 * in the list is a configuration item.
 */

pm_timer_t g_pm_timer_pool[CONFIG_PM_MAX_STATIC_TIMER];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_initialize
 *
 * Description:
 *   This function is called by MCU-specific one-time at power on reset in
 *   order to initialize the power management capabilities.  This function
 *   must be called *very* early in the initializeation sequence *before* any
 *   other device drivers are initialize (since they may attempt to register
 *   with the power management subsystem).
 *
 * Input parameters:
 *   None.
 *
 * Returned value:
 *    None.
 *
 ****************************************************************************/

void pm_initialize(void)
{
	sem_init(&g_pmglobals.regsem, 0, 1);

	g_pmglobals.stime = clock_systimer();
	g_pmglobals.btime = clock_systimer();

#ifdef CONFIG_PM_TICKSUPPRESS
	/* Registers a handler to be called when the core wakes up */
	up_register_wakehandler(pm_wakehandler);
#endif

#ifdef CONFIG_PM_METRICS
	struct timespec cur_time;

	/* Get current time */
	clock_gettime(CLOCK_REALTIME, &cur_time);

	struct pm_statechange_s *initnode = NULL;

	/* Initialize the domain's state history queue */

	sq_init(&g_pmglobals.history);

	/* Create an initial state change node with NORMAL state and bootup time */

	initnode = (struct pm_statechange_s *)pm_alloc(1, sizeof(struct pm_statechange_s));

	initnode->state = PM_NORMAL;
	initnode->timestamp = cur_time.tv_sec;

	/* Add the initial state change node to the head of the history queue */

	sq_addlast((&initnode->entry), &g_pmglobals.history);
#endif

	/* Initialize pm timer list */
	sq_init(&g_pm_timer_freelist);
	sq_init(&g_pm_timer_activelist);

	/* The g_pm_timer_freelist must be initiated */
	pm_timer_t *timer = g_pm_timer_pool;
	for (int i = 0; i < CONFIG_PM_MAX_STATIC_TIMER; i++) {
		sq_addlast((pm_timer_t *)timer++, &g_pm_timer_freelist);
	}

	/* All pm timers are free */
	g_pm_timer_nfree = CONFIG_PM_MAX_STATIC_TIMER;

	pmtest_init();
}
#endif							/* CONFIG_PM */
