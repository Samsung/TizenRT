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
/************************************************************************
 * kernel/sched/sched_mergepending.c
 *
 *   Copyright (C) 2007, 2009, 2012 Gregory Nutt. All rights reserved.
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <sched.h>
#include <queue.h>
#include <assert.h>

#ifdef CONFIG_SMP
#include <tinyara/spinlock.h>
#endif

#include "sched/sched.h"

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

#define ALL_CPUS ((cpu_set_t)-1)

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Global Variables
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/************************************************************************
 * Private Function Prototypes
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: sched_mergepending
 *
 * Description:
 *   This function merges the prioritized g_pendingtasks list into the
 *   prioritized g_readytorun task list.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   true if the head of the g_readytorun task list has changed.
 *
 * Assumptions:
 * - The caller has established a critical section before
 *   calling this function (calling sched_lock() first is NOT
 *   a good idea -- use enter_critical_section()).
 * - The caller handles the condition that occurs if the
 *   the head of the sched_mergTSTATE_TASK_PENDINGs is changed.
 *
 ************************************************************************/

#ifndef CONFIG_SMP
bool sched_mergepending(void)
{
	FAR struct tcb_s *pndtcb;
	FAR struct tcb_s *pndnext;
	FAR struct tcb_s *rtrtcb;
	FAR struct tcb_s *rtrprev;
	bool ret = false;

	/* Initialize the inner search loop */

	rtrtcb = this_task();

	/* Process every TCB in the g_pendingtasks list */

	for (pndtcb = (FAR struct tcb_s *)g_pendingtasks.head; pndtcb; pndtcb = pndnext) {
		pndnext = pndtcb->flink;

		/* Search the g_readytorun list to find the location to insert the
		 * new pndtcb. Each is list is maintained in ascending sched_priority
		 * order.
		 */

		for (; (rtrtcb && pndtcb->sched_priority <= rtrtcb->sched_priority); rtrtcb = rtrtcb->flink) ;

		/* Add the pndtcb to the spot found in the list.  Check if the
		 * pndtcb goes at the ends of the g_readytorun list. This would be
		 * error condition since the idle test must always be at the end of
		 * the g_readytorun list!
		 */

		ASSERT(rtrtcb);

		/* The pndtcb goes just before rtrtcb */

		rtrprev = rtrtcb->blink;
		if (!rtrprev) {
			/* Special case: Inserting pndtcb at the head of the list */

			/* Then insert at the head of the list */
			pndtcb->flink = rtrtcb;
			pndtcb->blink = NULL;
			rtrtcb->blink = pndtcb;
			g_readytorun.head = (FAR dq_entry_t *)pndtcb;
			rtrtcb->task_state = TSTATE_TASK_READYTORUN;
			pndtcb->task_state = TSTATE_TASK_RUNNING;
			ret = true;
		} else {
			/* Insert in the middle of the list */

			pndtcb->flink = rtrtcb;
			pndtcb->blink = rtrprev;
			rtrprev->flink = pndtcb;
			rtrtcb->blink = pndtcb;
			pndtcb->task_state = TSTATE_TASK_READYTORUN;
		}

		/* Set up for the next time through */

		rtrtcb = pndtcb;
	}

	/* Mark the input list empty */

	g_pendingtasks.head = NULL;
	g_pendingtasks.tail = NULL;

	return ret;
}

#else /* !CONFIG_SMP */
bool sched_mergepending(void)
{
	FAR struct tcb_s *rtcb;
	FAR struct tcb_s *ptcb;
	FAR struct tcb_s *tcb;
	bool ret = false;
	int cpu;
	int me;

	/* Remove and process every TCB in the g_pendingtasks list.
	 *
	 * Do nothing if (1) pre-emption is still disabled (by any CPU), or (2) if
	 * some CPU other than this one is in a critical section.
	 */

	me = this_cpu();
	if (!sched_islocked_global() && !irq_cpu_locked(me)) {
		/* Find the CPU that is executing the lowest priority task */

		ptcb = (FAR struct tcb_s *)dq_peek((FAR dq_queue_t *)&g_pendingtasks);
		if (ptcb == NULL) {
			/* The pending task list is empty */

			goto errout;
		}

		cpu  = sched_select_cpu(ALL_CPUS); /* REVISIT:  Maybe ptcb->affinity */
		rtcb = current_task(cpu);

		/* Loop while there is a higher priority task in the pending task list
		 * than in the lowest executing task.
		 *
		 * Normally, this loop should execute no more than CONFIG_SMP_NCPUS
		 * times.  That number could be larger, however, if the CPU affinity
		 * sets do not include all CPUs. In that case, the excess TCBs will
		 * end up in the g_readytorun list.
		 */

		while (ptcb->sched_priority > rtcb->sched_priority) {
			/* Remove the task from the pending task list */

			tcb = (FAR struct tcb_s *)
			dq_remfirst((FAR dq_queue_t *)&g_pendingtasks);

			/* Add the pending task to the correct ready-to-run list. */

			ret |= sched_addreadytorun(tcb);

			/* This operation could cause the scheduler to become locked.
			 * Check if that happened.
			 */

			if (sched_islocked_global() || irq_cpu_locked(me)) {
				/* Yes.. then we may have incorrectly placed some TCBs in the
				 * g_readytorun list (unlikely, but possible).  We will have to
				 * move them back to the pending task list.
				 */

				sched_merge_prioritized((FAR dq_queue_t *)&g_readytorun, \
						(FAR dq_queue_t *)&g_pendingtasks, \
						TSTATE_TASK_PENDING);

				/* And return with the scheduler locked and tasks in the
				 * pending task list.
				 */

				goto errout;
			}

			/* Set up for the next time through the loop */

			ptcb = (FAR struct tcb_s *)
			dq_peek((FAR dq_queue_t *)&g_pendingtasks);
			if (ptcb == NULL) {
				/* The pending task list is empty */

				goto errout;
			}

			cpu  = sched_select_cpu(ALL_CPUS); /* REVISIT:  Maybe ptcb->affinity */
			rtcb = current_task(cpu);
		}

		/* No more pending tasks can be made running.  Move any remaining
		 * tasks in the pending task list to the ready-to-run task list.
		 */

		sched_merge_prioritized((FAR dq_queue_t *)&g_pendingtasks, \
				(FAR dq_queue_t *)&g_readytorun, \
				TSTATE_TASK_READYTORUN);
	}

errout:

	return ret;
}
#endif /* !CONFIG_SMP */
