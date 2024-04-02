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
 * pm/pm_activity.c
 *
 *   Copyright (C) 2011-2012, 2016 Gregory Nutt. All rights reserved.
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

#include <stdint.h>
#include <assert.h>

#include <tinyara/pm/pm.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>

#include "pm.h"
#include "pm_timer/pm_timer.h"

#ifdef CONFIG_PM

/****************************************************************************
 * External Definitons
 ****************************************************************************/

extern struct pm_timer_s g_pm_timer;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_activity
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   performing meaningful activities (non-idle).  This increments an activity
 *   count and/or will restart a idle timer and prevent entering reduced
 *   power states.
 *
 * Input Parameters:
 *   domain - The domain of the PM activity
 *   priority - Activity priority, range 0-9.  Larger values correspond to
 *     higher priorities.  Higher priority activity can prevent the system
 *     from entering reduced power states for a longer period of time.
 *
 *     As an example, a button press might be higher priority activity because
 *     it means that the user is actively interacting with the device.
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *   This function may be called from an interrupt handler (this is the ONLY
 *   PM function that may be called from an interrupt handler!).
 *
 ****************************************************************************/

void pm_activity(int domain, int priority)
{
	FAR struct pm_domain_s *pdom;
	clock_t now;
	uint32_t accum;
	irqstate_t flags;

	/* Get a convenience pointer to minimize all of the indexing */

	DEBUGASSERT(domain >= 0 && domain < CONFIG_PM_NDOMAINS);
	pdom = &g_pmglobals.domain[domain];

	/* Just increment the activity count in the current time slice. The priority
	 * is simply the number of counts that are added.
	 */

	if (priority > 0) {
		/* Add the priority to the accumulated counts in a critical section. */

		flags = enter_critical_section();
		accum = (uint32_t)pdom->accum + priority;

		/* Make sure that we do not overflow the underlying uint16_t representation */

		if (accum > INT16_MAX) {
			accum = INT16_MAX;
		}

		/* Save the updated count */

		pdom->accum = (int16_t)accum;

		/* Check the elapsed time.  In periods of low activity, time slicing is
		 * controlled by IDLE loop polling; in periods of higher activity, time
		 * slicing is controlled by driver activity.  In either case, the duration
		 * of the time slice is only approximate; during times of heavy activity,
		 * time slices may be become longer and the activity level may be over-
		 * estimated.
		 */

		now = clock_systimer();
		if (now - pdom->stime >= TIME_SLICE_TICKS) {
			int16_t tmp;

			/* Sample the count, reset the time and count, and assess the PM
			 * state.  This is an atomic operation because interrupts are
			 * still disabled.
			 */

			tmp         = pdom->accum;
			pdom->stime = now;
			pdom->accum = 0;

			/* Reassessing the PM state may require some computation.  However,
			 * the work will actually be performed on a worker thread at a user-
			 * controlled priority.
			 */

			(void)pm_update(domain, tmp);
		}

		leave_critical_section(flags);
	}
}

/****************************************************************************
 * Name: pm_stay
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   performing meaningful activities (non-idle), needs the power at kept
 *   last the specified level.
 *
 * Input Parameters:
 *   domain - The domain of the PM activity
 *   state - The state want to stay.
 *
 *     As an example, media player might stay in normal state during playback.
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

void pm_stay(int domain, enum pm_state_e state)
{
	FAR struct pm_domain_s *pdom;
	irqstate_t flags;

	/* Get a convenience pointer to minimize all of the indexing */

	DEBUGASSERT(domain >= 0 && domain < CONFIG_PM_NDOMAINS);
	pdom = &g_pmglobals.domain[domain];

	flags = enter_critical_section();
	DEBUGASSERT(state < PM_COUNT);
	DEBUGASSERT(pdom->stay[state] < UINT16_MAX);
	pdom->stay[state]++;
	leave_critical_section(flags);
}

/****************************************************************************
 * Name: pm_set_timer
 *
 * Description:
 *   This function is called to set a timed callback to an intended function.
 *   It may be used to invoke pm_relax() after a fixed time duration of
 *   locking using pm_stay().
 *
 * Input Parameters:
 *   timer_type - the type of the timer which determines the callback
 *   timer_interval - duration of the timer
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void pm_set_timer(int pm_timer_type, size_t timer_interval)
{
	/* Set the global variables to trigger the appropriate timer */
	g_pm_timer.timer_type = pm_timer_type;
	g_pm_timer.timer_interval = timer_interval;

	/* Wakeup timer will be triggered at the time of sleep, otherwise trigger now. */
	if (pm_timer_type != PM_WAKEUP_TIMER) {
		up_set_pm_timer();
	}
	return;
}

/****************************************************************************
 * Name: pm_timer_process
 *
 * Description:
 *   This function decides which pm wakeup timer to be set just before sleep.
 *   It finds the wakeup timer with nearest future expiration time and sets it.
 *   When the board goes to sleep , it starts the timer and sleeps.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void pm_timer_process(void)
{
	pmdbg("pm_timer_process: inside pm_timer_process\n");

	/* if no timer present , do we need to wake up board ??? */
	if (g_pmTimer_activeList.head == NULL) {
		pmdbg("pm_timer_process: the g_pmTimer_activeList list is empty!!!\n");
		return;
	}

	pm_wakeup_timer_t *curr;
    	pm_wakeup_timer_t *prev;
    	prev = curr = (pm_wakeup_timer_t *)g_pmTimer_activeList.head;

	unsigned int now = clock_systimer();

	/* Remove the wakeup timers that are already expired. 
	 * There will be two cases here:
	 * 1. We remove it after app completes it timer work and just before setting another timer.
	 *    this case is fine.
	 * 2. We remove it before app completes it timer work and now pm lock will not happen
	 *    for the app to do its timer work ??? should we apply pm lock here ?? .
	 * */
	while (curr->expire_timetick < now && curr->next) {
		prev = curr;
		curr = curr->next;
		pm_timer_stop(prev->id);
		pmdbg("pm_timer_process: removed timer with id %d from the list \n", prev->id);
	}

	if (curr->expire_timetick > now) {
		pmdbg("pm_timer_process: setting timer with id %d\n", curr->id);
		pmdbg("pm_timer_process: Board will wake up after %d millisecond\n", (curr->expire_timetick - now));
		pm_set_timer(PM_WAKEUP_TIMER, (curr->expire_timetick - now) * 1000);

		/* There can be multiple wakeup timers with RUNNING state. 
		 * Need to handle that. This will usually happen when before expiration of
		 * timer1, we get some uart interrupt and we add timer2 with lower expiration time
		 * than timer1. Now just before sleep, we will start timer2. So we have timer1 and
		 * timer2 with state RUNNING but actually timer2 is running. */
		curr->status = RUNNING;
	} 
	/* Case where every timer expire time is less than current time. 
	 * No need to start any timer, But do we need to wake up after a fixed timer? */
	else {
		pmdbg("pm_timer_process: every timer expire time is less than current time, last timer id is %d\n", curr->id);
		pm_timer_stop(curr->id);
	}

}

/****************************************************************************
 * Name: pm_timer_callback
 *
 * Description:
 *   This function runs after any of the wakeup timers expire . 
 *   It will be used to remove the latest running wakeup timer from the 
 *   g_pmTimer_activeList.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void pm_timer_callback(void)
{
	pmdbg("pm_timer_callback: inside pm_timer_callback!!!\n");

	/* Check to see if the list is empty which should not be the case */
	if (g_pmTimer_activeList.head == NULL) {
		pmdbg("pm_timer_callback: the linked list is empty!!!\n");
		return;
	}
	pm_wakeup_timer_t *head;
    	head = (pm_wakeup_timer_t *)g_pmTimer_activeList.head;

	/* Removing the first timer from the list because that is the one we started
	 * before sleep. */
	sq_remfirst(&g_pmTimer_activeList);
	head->status = INACTIVE;

	/* Making sure to lock pm until the app completes its timer related work */
	if (head->is_periodic) {
		pm_stay(PM_IDLE_DOMAIN, PM_NORMAL);
		head->is_pm_lock = true;
		pmdbg("pm_timer_callback: setting pm lock for timer with id %d\n", head->id);
	}
}

/****************************************************************************
 * Name: pm_relax
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   idle now, could relax the previous requested power level.
 *
 * Input Parameters:
 *   domain - The domain of the PM activity
 *   state - The state want to relax.
 *
 *     As an example, media player might relax power level after playback.
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

void pm_relax(int domain, enum pm_state_e state)
{
	FAR struct pm_domain_s *pdom;
	irqstate_t flags;

	/* Get a convenience pointer to minimize all of the indexing */

	DEBUGASSERT(domain >= 0 && domain < CONFIG_PM_NDOMAINS);
	pdom = &g_pmglobals.domain[domain];

	flags = enter_critical_section();
	DEBUGASSERT(state < PM_COUNT);
	DEBUGASSERT(pdom->stay[state] > 0);
	pdom->stay[state]--;
	leave_critical_section(flags);
}

/****************************************************************************
 * Name: pm_staycount
 *
 * Description:
 *   This function is called to get current stay count.
 *
 * Input Parameters:
 *   domain - The domain of the PM activity
 *   state - The state want to relax.
 *
 * Returned Value:
 *   Current pm stay count
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

uint32_t pm_staycount(int domain, enum pm_state_e state)
{
	FAR struct pm_domain_s *pdom;

	/* Get a convenience pointer to minimize all of the indexing */

	DEBUGASSERT(domain >= 0 && domain < CONFIG_PM_NDOMAINS);
	pdom = &g_pmglobals.domain[domain];

	return pdom->stay[state];
}


#endif							/* CONFIG_PM */
