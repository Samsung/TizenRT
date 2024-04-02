/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * pm/pm_timer/pm_timer_activity.c
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

#include <tinyara/pm/pm.h>
#include "pm_timer.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Public Variables
 ************************************************************************/

/************************************************************************
 * Private Functions
 ************************************************************************/

void pm_timer_expire(void) 
{
	pm_wakeup_timer_t *head;

	while (g_pmTimer_activeList.head && ((pm_wakeup_timer_t *)g_pmTimer_activeList.head)->delay <= 0) {
		
    		head = (pm_wakeup_timer_t *)g_pmTimer_activeList.head;

		/* Remove the pm timer from active list */
        	sq_remfirst(&g_pmTimer_activeList);

		/* Make the pm timer free */
		pm_timer_delete(head);
	}
}

/************************************************************************
 * Public Functions
 ************************************************************************/

/****************************************************************************
 * Name: pm_set_wakeup_timer
 *
 * Description:
 *   This function is called just before sleep to start the required PM wake up
 *   timer. It will start the first timer from the g_pmTimer_activeList with the
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

int pm_set_wakeup_timer(void)
{
	/* if no timer present , do we need to wake up board ??? */
	if (g_pmTimer_activeList.head == NULL) {
		pmdbg("the g_pmTimer_activeList list is empty!!!\n");
		return PM_TIMER_SUCCESS;
	}

	pm_wakeup_timer_t *curr;
    	pm_wakeup_timer_t *prev;
    	prev = curr = (pm_wakeup_timer_t *)g_pmTimer_activeList.head;

	/* Remove the wakeup timers that are already expired. 
	 * Although this must have already handled by pm_timer_update() */
	while (curr->delay <= 0 && curr->next) {
		prev = curr;
		curr = curr->next;
	}

	if (curr->delay > 0) {
		pmdbg("Setting timer and Board will wake up after %d millisecond\n", curr->delay);
		pm_set_timer(PM_WAKEUP_TIMER, curr->delay * 1000);
	} 
	
	return PM_TIMER_SUCCESS;
}

/****************************************************************************
 * Name: pm_timer_update
 *
 * Description:
 *   This function decreases the delay of head pm timer in the 
 *   g_pmTimer_activeList by given ticks. If the delay becomes 0,
 *   It expires the pm timer.
 *
 * Input Parameters:
 *   Ticks to decrease
 *
 * Returned Value:
 *   None.
 * 
 * Assumption: This should be also implemented for CONFIG_SCHED_TICKLESS and
 * CONFIG_SCHED_TICKSUPRESS.
 *
 ****************************************************************************/

void pm_timer_update(int ticks)
{
	pm_wakeup_timer_t *timer;
	int decr;

	/* Check if there are any active pm timers to process */
	while (g_pmTimer_activeList.head && ticks > 0) {

		/* Get the pm timer at the head of the list */
		timer = (pm_wakeup_timer_t *)g_pmTimer_activeList.head;

		decr = MIN(timer->delay, ticks);
		timer->delay -= decr;
		ticks -= decr;

		/* Check if the timer at the head of the list is ready to run */
		pm_timer_expire();
	}
}

