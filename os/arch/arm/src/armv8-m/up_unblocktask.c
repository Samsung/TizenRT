/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 *  arch/arm/src/armv8-m/up_unblocktask.c
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

#include <sched.h>
#include <debug.h>
#include <tinyara/arch.h>

#include "sched/sched.h"
#include "clock/clock.h"
#include "up_internal.h"

#ifdef CONFIG_TASK_SCHED_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

#ifdef CONFIG_ARMV8M_TRUSTZONE
#include <tinyara/tz_context.h>
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
 * Name: up_unblock_task
 *
 * Description:
 *   A task is currently in an inactive task list
 *   but has been prepped to execute.  Move the TCB to the
 *   ready-to-run list, restore its context, and start execution.
 *
 * Inputs:
 *   tcb: Refers to the tcb to be unblocked.  This tcb is
 *     in one of the waiting tasks lists.  It must be moved to
 *     the ready-to-run list and, if it is the highest priority
 *     ready to run taks, executed.
 *
 ****************************************************************************/

void up_unblock_task(struct tcb_s *tcb)
{
	struct tcb_s *rtcb = this_task();

	/* Verify that the context switch can be performed */

	ASSERT((tcb->task_state >= FIRST_BLOCKED_STATE) && (tcb->task_state <= LAST_BLOCKED_STATE));

	/* Remove the task from the blocked task list */

	sched_removeblocked(tcb);

	/* Reset its timeslice.  This is only meaningful for round
	 * robin tasks but it doesn't here to do it for everything
	 */

#if CONFIG_RR_INTERVAL > 0
	tcb->timeslice = MSEC2TICK(CONFIG_RR_INTERVAL);
#endif

	/* Add the task in the correct location in the prioritized
	 * g_readytorun task list
	 */

	if (sched_addreadytorun(tcb)) {
#ifdef CONFIG_ARMV8M_TRUSTZONE
		if (rtcb->tz_context) {
			TZ_StoreContext_S(rtcb->tz_context);
		}
#endif
		/* The currently active task has changed! We need to do
		 * a context switch to the new task.
		 *
		 * Are we in an interrupt handler?
		 */

		if (current_regs) {
			/* Yes, then we have to do things differently.
			 * Just copy the current_regs into the OLD rtcb.
			 */

			up_savestate(rtcb->xcp.regs);

			/* Restore the exception context of the rtcb at the (new) head
			 * of the g_readytorun task list.
			 */

			rtcb = this_task();

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
