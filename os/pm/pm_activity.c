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
 * Name: pm_adjust_sleep_duration
 *
 * Description:
 *   This function is called just before sleep to calculate exact required
 *   sleep duration "if needed". This function is used to timely wake up board
 *   so that we can sent the next wifi keep alive signal.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void pm_adjust_sleep_duration(void)
{
	/* This part calculates the new time interval of sleep just before sleep. This part is added
	*  for correct functioning of sending of wifi keep alive signal. Basically, last_wifi_alive_send_time
	*  variable stores the last time tick when wifi keep alive signal was sent. Now before sleep, 
	*  it will calculate how much time has passed after that signal sent and accordingly it will 
	*  set duration of sleep. So that board can wake up when it is requried to send signal again.
	* 
	*  Also, somehow we are unable to send wifi alive signal in this iteration ( because of other priority task
	*  or very less time for wifi send action). Then also we need to wakeup the board , so that
	*  system will send wifi alive signal in next iteration. 
	*/
	   
	if (g_pm_timer.timer_type == PM_WAKEUP_TIMER && g_pm_timer.last_wifi_alive_send_time > 0) {
		uint32_t elapsed_time_after_last_wakeup = (clock_systimer() - g_pm_timer.last_wifi_alive_send_time) * 1000;
		if (elapsed_time_after_last_wakeup < g_pm_timer.timer_interval) {
			g_pm_timer.timer_interval -= elapsed_time_after_last_wakeup;
		}
	} else if (g_pm_timer.timer_type == PM_NO_TIMER) {
		pm_set_timer(PM_WAKEUP_TIMER, DEFAULT_PM_SLEEP_DURATION);
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
