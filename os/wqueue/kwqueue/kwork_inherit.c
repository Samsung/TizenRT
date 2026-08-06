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
 * wqueue/kwqueue/kwork_inherit.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <sched.h>

#include <tinyara/wqueue.h>

#include "sched/sched.h"
#include "semaphore/semaphore.h"
#include "wqueue.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lpwork_boostworker
 *
 * Description:
 *   Called by the work queue client to assure that the priority of the low-
 *   priority worker thread is at least at the requested level, reqprio. This
 *   function would normally be called just before calling work_queue().
 *
 * Parameters:
 *   reqprio - Requested minimum worker thread priority
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

static void lpwork_boostworker(pid_t wpid, uint8_t reqprio)
{
	FAR struct tcb_s *wtcb;

	/* Get the TCB of the low priority worker thread from the process ID. */

	wtcb = sched_gettcb(wpid);
	DEBUGASSERT(wtcb);

	/* REVISIT: Priority multi-boost is not supported */

	DEBUGASSERT(wtcb->boost_priority == 0);

	/* If the priority of the client thread is greater than the base priority
	 * of the worker thread, then we may need to adjust the worker thread's
	 * priority now or later to that priority.
	 */

	if (reqprio > wtcb->base_priority) {
		/* Save the boost priority value as it might be needed in case of
		 * multiple re-prioritizations; the priority of the thread can't go
		 * below the boost priority value until the priority boost is canceled.
		 */

		wtcb->boost_priority = reqprio;

		/* If the priority of the client thread is less than or equal to the
		 * priority of the worker thread, then do nothing because the thread is
		 * already running at a sufficient priority.
		 */

		if (reqprio > wtcb->sched_priority) {
			/* Raise the priority of the worker thread.  This cannot cause a
			 * context switch because we have preemption disabled.  The task
			 * will be marked "pending" and the switch will occur during
			 * sched_unlock() processing.
			 */

			(void)sched_setpriority(wtcb, reqprio);
		}
	}
}

/****************************************************************************
 * Name: lpwork_restoreworker
 *
 * Description:
 *   This function is called to restore the priority after it was previously
 *   boosted.  This is often done by client logic on the worker thread when
 *   the scheduled work completes.  It will check if we need to drop the
 *   priority of the worker thread.
 *
 * Parameters:
 *   reqprio - Previously requested minimum worker thread priority to be
 *     "unboosted"
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

static void lpwork_restoreworker(pid_t wpid, uint8_t reqprio)
{
	FAR struct tcb_s *wtcb;

	/* Get the TCB of the low priority worker thread from the process ID. */

	wtcb = sched_gettcb(wpid);
	DEBUGASSERT(wtcb);

	/* REVISIT: Priority multi-boost is not supported. */

	DEBUGASSERT(wtcb->boost_priority == reqprio);

	/* Clear the thread's boost priority. */

	wtcb->boost_priority = 0;

	/* Was the priority of the worker thread boosted? If so, then drop its
	 * priority back to the correct level.  What is the correct level?
	 */

	if (wtcb->sched_priority != wtcb->base_priority) {
		FAR struct semholder_s *pholder;
		FAR struct tcb_s *stcb;
		uint8_t wpriority;

		/* We attempt to restore the thread's priority to its base priority.
		 * If there is any task with a higher priority waiting for a semaphore
		 * held by wtcb, then this value will be overwritten.
		 */

		wpriority = wtcb->base_priority;

		/* Try to find the highest priority across all the tasks that are
		 * waiting for any semaphore held by wtcb.
		 */

		for (pholder = wtcb->holdsem; pholder != NULL; pholder = pholder->tlink) {
			stcb = sem_findhighestwaiter(pholder->sem, NULL);
			if (stcb != NULL && stcb->sched_priority > wpriority) {
				wpriority = stcb->sched_priority;
			}
		}

		/* Apply the selected priority to the worker thread (hopefully back to
		 * the thread's base priority).
		 */

		sched_setpriority(wtcb, wpriority);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lpwork_boostpriority
 *
 * Description:
 *   Called by the work queue client to assure that the priority of the low-
 *   priority worker thread is at least at the requested level, reqprio. This
 *   function would normally be called just before calling work_queue().
 *
 * Parameters:
 *   reqprio - Requested minimum worker thread priority
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

void lpwork_boostpriority(uint8_t reqprio)
{
	irqstate_t flags;
	int wndx;

	/* Clip to the configured maximum priority */

	if (reqprio > CONFIG_SCHED_LPWORKPRIOMAX) {
		reqprio = CONFIG_SCHED_LPWORKPRIOMAX;
	}

	/* Prevent context switches until we get the priorities right */

	flags = enter_critical_section();
	sched_lock();

	/* Adjust the priority of every worker thread */

	struct lp_wqueue_s *lwq = get_lpwork();
	for (wndx = 0; wndx < CONFIG_SCHED_LPNTHREADS; wndx++) {
		lpwork_boostworker(lwq->worker[wndx].pid, reqprio);
	}

	sched_unlock();
	leave_critical_section(flags);
}

/****************************************************************************
 * Name: lpwork_restorepriority
 *
 * Description:
 *   This function is called to restore the priority after it was previously
 *   boosted.  This is often done by client logic on the worker thread when
 *   the scheduled work completes.  It will check if we need to drop the
 *   priority of the worker thread.
 *
 * Parameters:
 *   reqprio - Previously requested minimum worker thread priority to be
 *     "unboosted"
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

void lpwork_restorepriority(uint8_t reqprio)
{
	irqstate_t flags;
	int wndx;

	/* Clip to the configured maximum priority */

	if (reqprio > CONFIG_SCHED_LPWORKPRIOMAX) {
		reqprio = CONFIG_SCHED_LPWORKPRIOMAX;
	}

	/* Prevent context switches until we get the priorities right */

	flags = enter_critical_section();
	sched_lock();

	/* Adjust the priority of every worker thread */

	struct lp_wqueue_s *lwq = get_lpwork();
	for (wndx = 0; wndx < CONFIG_SCHED_LPNTHREADS; wndx++) {
		lpwork_restoreworker(lwq->worker[wndx].pid, reqprio);
	}

	sched_unlock();
	leave_critical_section(flags);
}
