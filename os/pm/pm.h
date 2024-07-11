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
 * pm/pm.h
 *
 *   Copyright (C) 2011-2012, 2016, 2018 Gregory Nutt. All rights reserved.
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

#ifndef __DRIVERS_POWER_PM_H
#define __DRIVERS_POWER_PM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <semaphore.h>
#include <queue.h>
#include <tinyara/clock.h>
#include <tinyara/wqueue.h>
#include <tinyara/pm/pm.h>
#include <tinyara/wdog.h>

#ifdef CONFIG_PM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
#define pm_alloc(num, size) kmm_calloc(num, size)
#else
#define pm_alloc(num, size) calloc(num, size)
#endif

/* Convert the time slice interval into system clock ticks.
 *
 * CONFIG_PM_SLICEMS provides the duration of one time slice in milliseconds.
 * CLOCKS_PER_SEC provides the number of timer ticks in one second.
 *
 * slice ticks = (CONFIG_PM_SLICEMS msec / 1000 msec/sec) /
 *               (CLOCKS_PER_SEC ticks/sec)
 */

#define TIME_SLICE_TICKS ((CONFIG_PM_SLICEMS * CLOCKS_PER_SEC) /  1000)

/* Function-like macros *****************************************************/
/****************************************************************************
 * Name: pm_lock
 *
 * Descripton:
 *   Lock the power management registry.  NOTE: This function may return
 *   an error if a signal is received while what (errno == EINTR).
 *
 ****************************************************************************/

#define pm_lock()	sem_wait(&g_pmglobals.regsem);

/****************************************************************************
 * Name: pm_unlock
 *
 * Descripton:
 *   Unlock the power management registry.
 *
 ****************************************************************************/

#define pm_unlock()	sem_post(&g_pmglobals.regsem);

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This structure encapsulates all of the global data used by the PM module */

struct pm_global_s {
	/* This semaphore manages mutually exclusive access to the power management
	 * registry.  It must be initialized to the value 1.
	 */

	sem_t regsem;

	/* registry is a doubly-linked list of registered power management
	 * callback structures.  To ensure mutually exclusive access, this list
	 * must be locked by calling pm_lock() before it is accessed.
	 */

	dq_queue_t registry;

	/* The power state lock count */

	uint16_t suspend_count[CONFIG_PM_NDOMAINS];

	/* state       - The current state for this PM domain (as determined by an
	 *               explicit call to pm_changestate())
	 * recommended - The recommended state based on the PM algorithm.
	 */

	uint8_t state;
	uint8_t recommended;

	/* stime - The time (in ticks) at the start of the current time slice */

	clock_t stime;

	/* btime - The time (in ticks) at the start of the current state */

	clock_t btime;

	/* Timer to decrease state */

	WDOG_ID wdog;
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* All PM global data: */

EXTERN struct pm_global_s g_pmglobals;
EXTERN char *pm_domain_map[CONFIG_PM_NDOMAINS];
/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/
/****************************************************************************
 * Name: pm_set_wakeup_timer
 *
 * Description:
 *   This function is called just before sleep to start the required PM wake up
 *   timer. It will start the first timer from the g_pm_timer_activelist with the
 *   required delay.(delay should be positive)
 * 
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   0 - system can go to sleep
 *   -1 - system should not go to sleep
 *
 ****************************************************************************/
int pm_set_wakeup_timer(void);

/****************************************************************************
 * Name: pm_wakehandler
 *
 * Description:
 *   This function is called when the core wakes up. The operations are that
 *   should be reflected in the kernel immediately after the core wakes up.
 *	 This behavior is only for the IDLE domain.
 *
 * Input parameters:
 *   missing_tick - Missed ticks while the core was sleeping.
 *
 * Returned value:
 *   None
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/
#ifdef CONFIG_PM_TICKSUPPRESS
void pm_wakehandler(clock_t missing_tick, pm_wakeup_reason_code_t wakeup_src);
#endif

#ifdef CONFIG_PM_METRICS
/****************************************************************************
 * Name: pm_metrics_update_domain
 *
 * Description:
 *   This function is called when new domain got registered during pm_monitoring
 *   or during pm_metrics initialization. It initialize the PM Metrics for given
 *   domain.
 *
 * Input parameters:
 *   domain_id - the ID of domain registered with PM.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_domain(int domain_id);

/****************************************************************************
 * Name: pm_metrics_update_suspend
 *
 * Description:
 *   This function is called inside pm_suspend. It note the timestamp (in ticks) of
 *   suspended domain.
 *
 * Input parameters:
 *   domain_id - the ID of domain registered with PM.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_suspend(int domain_id);

/****************************************************************************
 * Name: pm_metrics_update_resume
 *
 * Description:
 *   This function is called inside pm_resume. Before resuming domain, it counts
 *   amount of time (in ticks) the given domain was suspended.
 * 
 * Input parameters:
 *   domain_id - the ID of domain registered with PM.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_resume(int domain_id);


/****************************************************************************
 * Name: pm_metrics_update_changestate
 *
 * Description:
 *   This function is called inside pm_changestate. Before changing state, it counts
 *   amount of time (in ticks) was in that state.
 * 
 * Input parameters:
 *   None
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_changestate(void);


/****************************************************************************
 * Name: pm_metrics_update_idle
 *
 * Description:
 *   This function is called inside pm_idle. It counts the frequency of domain, which
 *   make board unable to go into sleep during idle cpu time.
 * 
 * Input parameters:
 *   None
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_idle(void);


/****************************************************************************
 * Name: pm_metrics_update_wakehandler
 *
 * Description:
 *   This function is called inside pm_wakehandler. It counts the frequency of wakeup
 *   sources, which are waking up the board. It also checks the amount of time board
 *   was in sleep.
 * 
 * Input parameters:
 *   missing_tick - the amount of time the board was in sleep.
 *   wakeup_src   - the wakeup reason code.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_wakehandler(uint32_t missing_tick, pm_wakeup_reason_code_t wakeup_src);

/****************************************************************************
 * Name: pm_metrics_update_sleep
 *
 * Description:
 *   This function is called inside pm_sleep's callback. It counts the frequency of
 *   thread which wakeup the board. It also checks the minimum amount of time board
 *   was in sleep because of given thread.
 * 
 * Input parameters:
 *   pid - the ID of thread
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_sleep(pid_t pid);

/****************************************************************************
 * Name: pm_metrics_update_recover
 *
 * Description:
 *   This function is called inside pm_recover. It resets the wakeup_counts and 
 *   sleep_ticks of given thread for consistent PM Metrics result.
 * 
 * Input parameters:
 *   pid - the ID of thread
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_recover(pid_t pid);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* CONFIG_PM */
#endif							/* #define __DRIVERS_POWER_PM_H */
