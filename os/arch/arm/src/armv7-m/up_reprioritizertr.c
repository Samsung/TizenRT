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
 *  arch/arm/src/armv7-m/up_reprioritizertr.c
 *
 *   Copyright (C) 2007-2009, 2012-2013 Gregory Nutt. All rights reserved.
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
#include <stdbool.h>
#include <sched.h>
#include <debug.h>
#include <tinyara/arch.h>

#include "sched/sched.h"
#include "up_internal.h"

#ifdef CONFIG_TASK_SCHED_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_reprioritize_rtr
 *
 * Description:
 *   Called when the priority of a running or
 *   ready-to-run task changes and the reprioritization will
 *   cause a context switch.  Two cases:
 *
 *   1) The priority of the currently running task drops and the next
 *      task in the ready to run list has priority.
 *   2) An idle, ready to run task's priority has been raised above the
 *      the priority of the current, running task and it now has the
 *      priority.
 *
 * Inputs:
 *   tcb: The TCB of the task that has been reprioritized
 *   priority: The new task priority
 *
 ****************************************************************************/

void up_reprioritize_rtr(struct tcb_s *tcb, uint8_t priority)
{
	/* Verify that the caller is same */

	if (tcb->task_state < FIRST_READY_TO_RUN_STATE || tcb->task_state > LAST_READY_TO_RUN_STATE
#if SCHED_PRIORITY_MIN > 0
		|| priority < SCHED_PRIORITY_MIN
#endif
#if SCHED_PRIORITY_MAX < UINT8_MAX
		|| priority > SCHED_PRIORITY_MAX
#endif
	   ) {
		PANIC();
	} else {
		struct tcb_s *rtcb = this_task();
		bool switch_needed;

		sllvdbg("TCB=%p PRI=%d\n", tcb, priority);

		/* Remove the tcb task from the ready-to-run list.
		 * sched_removereadytorun will return true if we just removed the head
		 * of the ready to run list.
		 */

		switch_needed = sched_removereadytorun(tcb);

		/* Setup up the new task priority */

		tcb->sched_priority = (uint8_t)priority;

		/* Return the task to the ready-to-run task list. sched_addreadytorun
		 * will return true if the task was added to the head of ready-to-run
		 * list.  We will need to perform a context switch only if the
		 * EXCLUSIVE or of the two calls is non-zero (i.e., one and only one
		 * the calls changes the head of the ready-to-run list).
		 */

		switch_needed ^= sched_addreadytorun(tcb);

		/* Now, perform the context switch if one is needed (i.e. if the head
		 * of the ready-to-run list is no longer the same).
		 */

		if (switch_needed) {
			/* If we are going to do a context switch, then now is the right
			 * time to add any pending tasks back into the ready-to-run list.
			 * task list now
			 */

			if (g_pendingtasks.head) {
				sched_mergepending();
			}

			/* Are we in an interrupt handler? */

			if (current_regs) {
				/* Yes, then we have to do things differently.
				 * Just copy the current_regs into the OLD rtcb.
				 */

				up_savestate(rtcb->xcp.regs);

				/* Restore the exception context of the rtcb at the (new) head
				 * of the g_readytorun task list.
				 */

				rtcb = this_task();
				sllvdbg("New Active Task TCB=%p\n", rtcb);

				/* Restore rtcb data for context switching */
				
				up_restoretask(rtcb);
				
				/* Then switch contexts */
				
				up_restorestate(rtcb->xcp.regs);

			}

			/* No, then we will need to perform the user context switch */

			else {
				/* Switch context to the context of the task at the head of the
				 * ready to run list.
				 */

				struct tcb_s *nexttcb = this_task();
#ifdef CONFIG_TASK_SCHED_HISTORY
				/* Save the task name which will be scheduled */
				save_task_scheduling_status(nexttcb);
#endif
				up_switchcontext(rtcb->xcp.regs, nexttcb->xcp.regs);

				/* up_switchcontext forces a context switch to the task at the
				 * head of the ready-to-run list.  It does not 'return' in the
				 * normal sense.  When it does return, it is because the blocked
				 * task is again ready to run and has execution priority.
				 */
			}
		}
	}
}
