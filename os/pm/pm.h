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

/* Function-like macros *****************************************************/
/****************************************************************************
 * Name: pm_lock
 *
 * Descripton:
 *   Lock the power management registry.  NOTE: This function may return
 *   an error if a signal is received while what (errno == EINTR).
 *
 ****************************************************************************/

#define pm_lock() sem_wait(&g_pmglobals.regsem);

/****************************************************************************
 * Name: pm_unlock
 *
 * Descripton:
 *   Unlock the power management registry.
 *
 ****************************************************************************/

#define pm_unlock() sem_post(&g_pmglobals.regsem);

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Forward declaration - struct pm_domain_s is now in the global header */

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

	/* domains is a doubly-linked list of registered power management domains */
	dq_queue_t domains;

	/* suspended_domains is a doubly-linked list of domains with suspend_count > 0 */
	dq_queue_t suspended_domains;

	/* state       - The current state for this PM domain (as determined by an
	 *               explicit call to pm_changestate())
	 */

	uint8_t state;

	/* Timer to decrease state */

	WDOG_ID wdog;

	/* No. of Registered Domains */
	uint16_t ndomains;

	/* Indicates Board is Ready to State Change */
	bool is_running;

	/* Sleep ops structure */
	struct pm_sleep_ops *sleep_ops;

#ifdef CONFIG_PM_DVFS
	/* DVFS ops structure */
	struct pm_clock_ops *dvfs_ops;
#endif
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

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/
/****************************************************************************
 * Name: pm_check_domain
 *
 * Description:
 *   This function is called inside PM internal APIs to check whether the
 *   domain pointer is valid.
 * 
 * Input Parameters:
 *   domain - Pointer to the domain structure
 *
 * Returned Value:
 *   0 (OK) - If domain is valid
 *  -1 (ERROR) - If domain is not valid
 *
 ****************************************************************************/
int pm_check_domain(FAR struct pm_domain_s *domain);

/****************************************************************************
 * Name: pm_checkstate
 *
 * Description:
 *   This function is called from the MCU-specific IDLE loop to monitor the
 *   the power management conditions.  This function returns the "recommended"
 *   power management state based on the PM configuration and activity
 *   reported in the last sampling periods.  The power management state is
 *   not automatically changed, however.  The IDLE loop must call
 *   pm_changestate() in order to make the state change.
 *
 *   These two steps are separated because the platform-specific IDLE loop may
 *   have additional situational information that is not available to the
 *   the PM sub-system.  For example, the IDLE loop may know that the
 *   battery charge level is very low and may force lower power states
 *   even if there is activity.
 *
 *   NOTE: That these two steps are separated in time and, hence, the IDLE
 *   loop could be suspended for a long period of time between calling
 *   pm_checkstate() and pm_changestate().  The IDLE loop may need to make
 *   these calls atomic by either disabling interrupts until the state change
 *   is completed.
 *
 * Input Parameters:
 *   None (This function operates on all registered domains or global state)
 *
 * Returned Value:
 *   The recommended power management state.
 *
 ****************************************************************************/

enum pm_state_e pm_checkstate(void);

/****************************************************************************
 * Name: pm_changestate
 *
 * Description:
 *   This function is used to platform-specific power management logic.  It
 *   will announce the power management power management state change to all
 *   drivers that have registered for power management event callbacks.
 *
 * Input Parameters:
 *   newstate - Identifies the new PM state
 *
 * Returned Value:
 *   0 (OK) means that the callback function for all registered drivers
 *   returned OK (meaning that they accept the state change).  Non-zero
 *   means that one of the drivers refused the state change.  In this case,
 *   the system will revert to the preceding state.
 *
 * Assumptions:
 *   It is assumed that interrupts are disabled when this function is
 *   called.  This function is probably called from the IDLE loop... the
 *   lowest priority task in the system.  Changing driver power management
 *   states may result in renewed system activity and, as a result, can
 *   suspend the IDLE thread before it completes the entire state change
 *   unless interrupts are disabled throughout the state change.
 *
 ****************************************************************************/

int pm_changestate(enum pm_state_e newstate);

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
 *   domain - Pointer to the domain structure
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_domain(FAR struct pm_domain_s *domain);

/****************************************************************************
 * Name: pm_metrics_update_suspend
 *
 * Description:
 *   This function is called inside pm_suspend. It note the timestamp (in ticks) of
 *   suspended domain.
 *
 * Input parameters:
 *   domain - Pointer to the domain structure
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_suspend(FAR struct pm_domain_s *domain);

/****************************************************************************
 * Name: pm_metrics_update_resume
 *
 * Description:
 *   This function is called inside pm_resume. Before resuming domain, it counts
 *   amount of time (in ticks) the given domain was suspended.
 * 
 * Input parameters:
 *   domain - Pointer to the domain structure
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_resume(FAR struct pm_domain_s *domain);


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
 * Name: pm_metrics_update_wakeup_reason
 *
 * Description:
 *   This function is called inside pm_wakehandler. It counts the frequency of wakeup
 *   sources, which are waking up the board.
 * 
 * Input parameters:
 *   wakeup_src   - the wakeup reason code.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_wakeup_reason(pm_wakeup_reason_code_t wakeup_src);

/****************************************************************************
 * Name: pm_metrics_update_missing_tick
 *
 * Description:
 *   This function is called inside pm_wakehandler. It checks the amount of time board
 *   was in sleep.
 * 
 * Input parameters:
 *   missing_tick - the amount of time the board was in sleep.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_missing_tick(clock_t missing_tick);
#else 
#define pm_metrics_update_domain(domain)
#define pm_metrics_update_suspend(domain)
#define pm_metrics_update_resume(domain)
#define pm_metrics_update_changestate()
#define pm_metrics_update_idle()
#define pm_metrics_update_wakeup_reason(wakeup_src)
#define pm_metrics_update_missing_tick(missing_tick)
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* CONFIG_PM */
#endif /* #define __DRIVERS_POWER_PM_H */
