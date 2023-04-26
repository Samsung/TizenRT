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
 * kernel/sched/sched_removereadytorun.c
 *
 *   Copyright (C) 2007-2009, 2012 Gregory Nutt. All rights reserved.
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
#include <assert.h>

#include "sched/sched.h"

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
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_removereadytorun
 *
 * Description:
 *   This function removes a TCB from the ready to run list.
 *
 * Inputs:
 *   rtcb - Points to the TCB that is ready-to-run
 *
 * Return Value:
 *   true if the currently active task (the head of the
 *     g_readytorun list) has changed.
 *
 * Assumptions:
 * - The caller has established a critical section before calling this
 *   function (calling sched_lock() first is NOT a good idea -- use irqsave()).
 * - The caller handles the condition that occurs if the
 *   the head of the g_readytorun list is changed.
 *
 ****************************************************************************/

#ifndef CONFIG_SMP
bool sched_removereadytorun(FAR struct tcb_s *rtcb)
{
	FAR struct tcb_s *ntcb = NULL;
	bool ret = false;

	/* Check if the TCB to be removed is at the head of the ready to run list.
	 * In this case, we are removing the currently active task.
	 */

	if (!rtcb->blink) {
		/* There must always be at least one task in the list (the idle task) */

		ntcb = (FAR struct tcb_s *)rtcb->flink;
		DEBUGASSERT(ntcb != NULL);

		ntcb->task_state = TSTATE_TASK_RUNNING;
		ret = true;
	}

	/* Remove the TCB from the ready-to-run list */

	dq_rem((FAR dq_entry_t *)rtcb, (FAR dq_queue_t *)&g_readytorun);

	/* Since the TCB is not in any list, it is now invalid */

	rtcb->task_state = TSTATE_TASK_INVALID;
	return ret;
}

#else
bool sched_removereadytorun(FAR struct tcb_s *rtcb)
{
	FAR dq_queue_t *tasklist;
	bool ret = false;
	int cpu;

	/* Which CPU (if any) is the task running on? Which task list holds
	 * the TCB
	 */
	cpu      = rtcb->cpu;
	tasklist = TLIST_HEAD(rtcb->task_state, cpu);

	/* Check if the TCB to be removed is at the head of a ready-to-run list.
	 * For the case of SMP, there are two lists involved: (1) the g_readytrun
	 * list that holds non-running tasks that have not been assigned to a CPU,
	 * and (2) the g_assignedtasks[] lists which hold tasks assigned to a CPU
	 * including the task that is currently running on that CPU. Only the latter
	 * list contains the currently active task and removing the head of only
	 * this list can result in a context switch.
	 *
	 * rtcb->blink == NULL will tell us if the TCB is at the head of the
	 * ready-to-run list and hence, a candidate for the new running task.
	 *
	 * If so, then the tasklist RUNNABLe attribute will inform us if the list
	 * holds the currently executing task and, hence, if a context switch should
	 * occur.
	 */

	if (rtcb->blink == NULL) {	//PORTNOTE: Check if required. Removed because we dont use this attribute in our tlist...  && TLIST_ISRUNNABLE(rtcb->task_state)) {
		FAR struct tcb_s *ntcb;
		FAR struct tcb_s *rtrtcb = NULL;
		int current_cpu;

		/* There must always be one task in the list, i.e. the idle task
		 * after the TCB being removed.
		 */

		ntcb = (FAR struct tcb_s *)rtcb->flink;
		DEBUGASSERT(ntcb != NULL);

		/* If we are modifying the head of some assigned task list other
		 * than our own, we will need to stop that CPU.
		 */

		current_cpu = this_cpu();
		if (cpu != current_cpu) {
			DEBUGVERIFY(up_cpu_pause(cpu));
		}

		/* The task is running but the CPU that it was running on has been
		 * paused. We can now safely remove its TCB from the ready-to-run
		 * task list, In the SMP case, this may either be the g_readytorun()
		 * or the g_assignedtasks[cpu] list.
		 */

		dq_rem((FAR dq_entry_t *)rtcb, tasklist);

		/* Which task will go at the head of the list? It will either be
		 * the next tcb in the assigned task list (ntcb) or a TCB in the
		 * g_readytorun list. We can only select a task from that list if
		 * the affinity mask includes the current CPU.
		 *
		 * If pre-emption is locked of another CPU is in a critical section,
		 * then use the 'ntcb' which will probably be the IDLE thread.
		 * REVISIT: What if it is not the IDLE thread?
		 */

		if (!sched_islocked_global() && !irq_cpu_locked(current_cpu)) {
			/* Search for the highest priority task that can run on
			 * this CPU
			 */

			for (rtrtcb = (FAR struct tcb_s *)g_readytorun.head; \
					rtrtcb != NULL && !CPU_ISSET(cpu, &rtrtcb->affinity); \
					rtrtcb = (FAR struct tcb_s *)rtrtcb->flink);
		}

		/* Did we find a task in the g_readytorun list?  Which task should
		 * we use?  We decide strictly by the priority of the two tasks:
		 * Either (1) the task currently at the head of the
		 * g_assignedtasks[cpu] list (ntcb) or (2) the highest priority
		 * task from the g_readytorun list with matching affinity (rtrtcb).
		 */

		if (rtrtcb != NULL && rtrtcb->sched_priority >= ntcb->sched_priority) {
			/* The TCB rtrtcb has the higher priority and it can be run
			 * on the target CPU. Remove that task (rtrtcb) from the
			 * g_readytorun list and add it to the head of the
			 * g_assignedtasks[cpu] list.
			 */

			dq_rem((FAR dq_entry_t *)rtrtcb, (FAR dq_queue_t *)&g_readytorun);
			dq_addfirst((FAR dq_entry_t *)rtrtcb, tasklist);
			rtrtcb->cpu = cpu;
			ntcb = rtcb;
		}

		/* Will pre-emption be disabled after the switch? If the lockcount
		 * is greater tahn zero, then this task/CPU holds the scheduler lock.
		 */

		if (ntcb->lockcount > 0) {
			/* Yes, make sure that the scheduling logic knows about this */
			spin_setbit(&g_cpu_lockset, cpu, &g_cpu_locksetlock, \
					&g_cpu_schedlock);
		} else {
			/* No, we may need to release our hold on the lock. */
			spin_clrbit(&g_cpu_lockset, cpu, &g_cpu_locksetlock,
                      &g_cpu_schedlock);
		}

		/* NOTE: If the task runs on another CPU(cpu), adjusting global
		 * IRQ controls will be done in the pause handler on the new CPU(cpu).
		 * If the task is scheduled on this CPU(current_cpu), do nothing
		 * because this CPU already has a critical section.
		 */

		ntcb->task_state = TSTATE_TASK_RUNNING;

		/* All done, restart the other CPU (if it was paused). */

		ret = true;
		if (cpu != current_cpu) {
			/* In this we will not want to report a context switch to this
			 * CPU.  Only the other CPU is affected.
			 */
			DEBUGVERIFY(up_cpu_resume(cpu));
			ret = false;
		}
	} else {
		/* The task is not running.  Just remove its TCB from the ready-to-run
		 * list.  In the SMP case this may be either the g_readytorun() or the
		 * g_assignedtasks[cpu] list.
		 */

		dq_rem((FAR dq_entry_t *)rtcb, tasklist);
	}

	/* Since the TCB is no longer in any list, it is now invalid */
	rtcb->task_state = TSTATE_TASK_INVALID;

	return ret;
}
#endif /* ifndef CONFIG_SMP */
