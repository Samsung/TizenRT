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
 * kernel/wdog/wd_start.c
 *
 *   Copyright (C) 2007-2009, 2012, 2014 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/wdog.h>

#include "sched/sched.h"
#include "wdog/wdog.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

typedef void (*wdentry0_t)(int argc);
#if CONFIG_MAX_WDOGPARMS > 0
typedef void (*wdentry1_t)(int argc, uint32_t arg1);
#endif
#if CONFIG_MAX_WDOGPARMS > 1
typedef void (*wdentry2_t)(int argc, uint32_t arg1, uint32_t arg2);
#endif
#if CONFIG_MAX_WDOGPARMS > 2
typedef void (*wdentry3_t)(int argc, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif
#if CONFIG_MAX_WDOGPARMS > 3
typedef void (*wdentry4_t)(int argc, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
#endif

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
 * Name: wd_expiration
 *
 * Description:
 *   Check if the timer for the watchdog at the head of list is ready to
 *   run.  If so, remove the watchdog from the list and execute it.
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline void wd_expiration(void)
{
	FAR struct wdog_s *wdog;

	/* Check if the watchdog at the head of the list is ready to run */

	if (((FAR struct wdog_s *)g_wdactivelist.head)->lag <= 0) {
		/* Process the watchdog at the head of the list as well as any
		 * other watchdogs that became ready to run at this time
		 */

		while (g_wdactivelist.head && ((FAR struct wdog_s *)g_wdactivelist.head)->lag <= 0) {
			/* Remove the watchdog from the head of the list */

			wdog = (FAR struct wdog_s *)sq_remfirst(&g_wdactivelist);

			/* If there is another watchdog behind this one, update its
			 * its lag (this shouldn't be necessary).
			 */

			if (g_wdactivelist.head) {
				((FAR struct wdog_s *)g_wdactivelist.head)->lag += wdog->lag;
			}

			/* Indicate that the watchdog is no longer active. */

			WDOG_CLRACTIVE(wdog);

			/* Execute the watchdog function */

			up_setpicbase(wdog->picbase);
			switch (wdog->argc) {
			default:
				wd_corruption_dbg(wdog);
				DEBUGPANIC();
				break;

			case 0:
				(*((wdentry0_t)(wdog->func)))(0);
				break;

#if CONFIG_MAX_WDOGPARMS > 0
			case 1:
				(*((wdentry1_t)(wdog->func)))(1, wdog->parm[0]);
				break;
#endif
#if CONFIG_MAX_WDOGPARMS > 1
			case 2:
				(*((wdentry2_t)(wdog->func)))(2, wdog->parm[0], wdog->parm[1]);
				break;
#endif
#if CONFIG_MAX_WDOGPARMS > 2
			case 3:
				(*((wdentry3_t)(wdog->func)))(3, wdog->parm[0], wdog->parm[1], wdog->parm[2]);
				break;
#endif
#if CONFIG_MAX_WDOGPARMS > 3
			case 4:
				(*((wdentry4_t)(wdog->func)))(4, wdog->parm[0], wdog->parm[1], wdog->parm[2], wdog->parm[3]);
				break;
#endif
			}
		}
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wd_start
 *
 * Description:
 *   This function adds a watchdog to the timer queue.  The specified
 *   watchdog function will be called from the interrupt level after the
 *   specified number of ticks has elapsed. Watchdog timers may be started
 *   from the interrupt level.
 *
 *   Watchdog timers execute in the address environment that was in effect
 *   when wd_start() is called.
 *
 *   Watchdog timers execute only once.
 *
 *   To replace either the timeout delay or the function to be executed,
 *   call wd_start again with the same wdog; only the most recent wdStart()
 *   on a given watchdog ID has any effect.
 *
 * Parameters:
 *   wdog     = watchdog ID
 *   delay    = Delay count in clock ticks
 *   wdentry  = function to call on timeout
 *   parm1..4 = parameters to pass to wdentry
 *
 * Return Value:
 *   OK or ERROR
 *
 * Assumptions:
 *   The watchdog routine runs in the context of the timer interrupt handler
 *   and is subject to all ISR restrictions.
 *
 ****************************************************************************/

int wd_start(WDOG_ID wdog, int delay, wdentry_t wdentry, int argc, ...)
{
	va_list ap;
	FAR struct wdog_s *curr;
	FAR struct wdog_s *prev;
	FAR struct wdog_s *next;
	int32_t now;
	irqstate_t state;
	int i;

	/* Verify the wdog */

	if (!wdog || argc > CONFIG_MAX_WDOGPARMS || argc < 0 || delay < 0) {
		set_errno(EINVAL);
		return ERROR;
	}

#ifdef CONFIG_DEBUG
	/* Store the pid of process to keep track which process is responsible for wdog expiration */
	wdog->pid = getpid();
#endif

	/* Check if the watchdog has been started. If so, stop it.
	 * NOTE:  There is a race condition here... the caller may receive
	 * the watchdog between the time that wd_start is called and
	 * the critical section is established.
	 */

	state = enter_critical_section();
	if (WDOG_ISACTIVE(wdog)) {
		wd_cancel(wdog);
	}

	/* Save the data in the watchdog structure */

	wdog->func = wdentry;		/* Function to execute when delay expires */
	up_getpicbase(&wdog->picbase);
	wdog->argc = argc;

	va_start(ap, argc);
	for (i = 0; i < argc; i++) {
		wdog->parm[i] = va_arg(ap, uint32_t);
	}
#ifdef CONFIG_DEBUG
	for (; i < CONFIG_MAX_WDOGPARMS; i++) {
		wdog->parm[i] = 0;
	}
#endif
	va_end(ap);

	/* Calculate delay+1, forcing the delay into a range that we can handle */

	if (delay <= 0) {
		delay = 1;
	} else if (++delay <= 0) {
		delay--;
	}
#ifdef CONFIG_SCHED_TICKLESS
	/* Cancel the interval timer that drives the timing events.  This will cause
	 * wd_timer to be called which update the delay value for the first time
	 * at the head of the timer list (there is a possibility that it could even
	 * remove it).
	 */

	(void)sched_timer_cancel();
#endif

	/* Do the easy case first -- when the watchdog timer queue is empty. */

	if (g_wdactivelist.head == NULL) {
		/* Add the watchdog to the head == tail of the queue. */

		sq_addlast((FAR sq_entry_t *)wdog, &g_wdactivelist);
	}

	/* There are other active watchdogs in the timer queue */

	else {
		now = 0;
		prev = curr = (FAR struct wdog_s *)g_wdactivelist.head;

		/* Advance to positive time */

		while ((now += curr->lag) < 0 && curr->next) {
			prev = curr;
			curr = curr->next;
		}

		/* Advance past shorter delays */

		while (now <= delay && curr->next) {
			prev = curr;
			curr = curr->next;
			now += curr->lag;
		}

		/* Check if the new wdog must be inserted before the curr. */

		if (delay < now) {
			/* The relative delay time is smaller or equal to the current delay
			 * time, so decrement the current delay time by the new relative
			 * delay time.
			 */

			delay -= (now - curr->lag);
			curr->lag -= delay;

			/* Insert the new watchdog in the list */

			if (curr == (FAR struct wdog_s *)g_wdactivelist.head) {
				/* Insert the watchdog at the head of the list */

				sq_addfirst((FAR sq_entry_t *)wdog, &g_wdactivelist);
			} else {
				/* Insert the watchdog in mid- or end-of-queue */

				sq_addafter((FAR sq_entry_t *)prev, (FAR sq_entry_t *)wdog, &g_wdactivelist);

			}
		}

		/* The new watchdog delay time is greater than the curr delay time,
		 * so the new wdog must be inserted after the curr. This only occurs
		 * if the wdog is to be added to the end of the list.
		 */

		else {
			delay -= now;
			if (!curr->next) {
				sq_addlast((FAR sq_entry_t *)wdog, &g_wdactivelist);
			} else {
				next = curr->next;
				next->lag -= delay;
				sq_addafter((FAR sq_entry_t *)curr, (FAR sq_entry_t *)wdog, &g_wdactivelist);
			}
		}
	}

	/* Put the lag into the watchdog structure and mark it as active. */

	wdog->lag = delay;
	WDOG_SETACTIVE(wdog);

#ifdef CONFIG_SCHED_TICKLESS
	/* Resume the interval timer that will generate the next interval event.
	 * If the timer at the head of the list changed, then this will pick that
	 * new delay.
	 */

	sched_timer_resume();
#endif

	leave_critical_section(state);
	return OK;
}

/****************************************************************************
 * Name: wd_timer
 *
 * Description:
 *   This function is called from the timer interrupt handler to determine
 *   if it is time to execute a watchdog function.  If so, the watchdog
 *   function will be executed in the context of the timer interrupt
 *   handler.
 *
 * Parameters:
 *   ticks - If CONFIG_SCHED_TICKLESS is defined then the number of ticks
 *     in the interval that just expired is provided.  Otherwise,
 *     this function is called on each timer interrupt and a value of one
 *     is implicit.
 *
 * Return Value:
 *   If CONFIG_SCHED_TICKLESS is defined then the number of ticks for the
 *   next delay is provided (zero if no delay).  Otherwise, this function
 *   has no returned value.
 *
 * Assumptions:
 *   Called from interrupt handler logic with interrupts disabled.
 *
 ****************************************************************************/

#ifdef CONFIG_SCHED_TICKLESS
unsigned int wd_timer(int ticks)
{
	FAR struct wdog_s *wdog;
	int decr;

	/* Check if there are any active watchdogs to process */

	while (g_wdactivelist.head && ticks > 0) {
		/* Get the watchdog at the head of the list */

		wdog = (FAR struct wdog_s *)g_wdactivelist.head;

#ifndef CONFIG_SCHED_TICKLESS_ALARM
		/* There is logic to handle the case where ticks is greater than
		 * the watchdog lag, but if the scheduling is working properly
		 * that should never happen.
		 */

		DEBUGASSERT(ticks <= wdog->lag);
#endif
		/* Decrement the lag for this watchdog. */

		decr = MIN(wdog->lag, ticks);

		/* There are.  Decrement the lag counter */

		wdog->lag -= decr;
		ticks -= decr;

		/* Check if the watchdog at the head of the list is ready to run */

		wd_expiration();
	}

	/* Return the delay for the next watchdog to expire */

	return g_wdactivelist.head ? ((FAR struct wdog_s *)g_wdactivelist.head)->lag : 0;
}

#else
void wd_timer(void)
{
	/* Check if there are any active watchdogs to process */

	if (g_wdactivelist.head) {
		/* There are.  Decrement the lag counter */

		--(((FAR struct wdog_s *)g_wdactivelist.head)->lag);

		/* Check if the watchdog at the head of the list is ready to run */

		wd_expiration();
	}

}
#endif							/* CONFIG_SCHED_TICKLESS */

#ifdef CONFIG_SCHED_TICKSUPPRESS
void wd_timer_nohz(clock_t ticks)
{
	FAR struct wdog_s *wdog;
	int decr;

	for (wdog = g_wdactivelist.head; ticks > 0 && wdog; wdog = wdog->next) {

		/* Decrement the lag for this watchdog. */

		decr = MIN(wdog->lag, ticks);

		/* There are.  Decrement the lag counter */

		wdog->lag -= decr;
		ticks -= decr;

		/* Expires when the next wd_timer is called.*/
	}
}
#endif
