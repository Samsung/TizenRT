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
 *  arch/arm/src/armv7-r/arm_releasepending.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
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
#include <tinyara/sched.h>

#include "sched/sched.h"
#include "group/group.h"
#include "up_internal.h"
#ifdef CONFIG_TASK_SCHED_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_release_pending
 *
 * Description:
 *   Release and ready-to-run tasks that have
 *   collected in the pending task list.  This can call a
 *   context switch if a new task is placed at the head of
 *   the ready to run list.
 *
 ****************************************************************************/

void up_release_pending(void)
{
	struct tcb_s *rtcb = this_task();

	sllvdbg("From TCB=%p\n", rtcb);

	/* Merge the g_pendingtasks list into the g_readytorun task list */

	/* sched_lock(); */
	if (sched_mergepending()) {
		/* The currently active task has changed!  We will need to
		 * switch contexts.
		 */

		/* Are we operating in interrupt context? */

		if (current_regs) {
			/* Yes, then we have to do things differently.
			 * Just copy the current_regs into the OLD rtcb.
			 */

			up_savestate(rtcb->xcp.regs);

			/* Restore the exception context of the rtcb at the (new) head
			 * of the g_readytorun task list.
			 */

			rtcb = this_task();

#ifdef CONFIG_TASK_SCHED_HISTORY
			/* Save the task name which will be scheduled */
			save_task_scheduling_status(rtcb);
#endif

			/* Then switch contexts.  Any necessary address environment
			 * changes will be made when the interrupt returns.
			 */
			up_restorestate(rtcb->xcp.regs);
		}

		/* Copy the exception context into the TCB of the task that
		 * was currently active. if up_saveusercontext returns a non-zero
		 * value, then this is really the previously running task
		 * restarting!
		 */

		else if (!up_saveusercontext(rtcb->xcp.regs)) {
			/* Restore the exception context of the rtcb at the (new) head
			 * of the g_readytorun task list.
			 */

			rtcb = this_task();

#ifdef CONFIG_TASK_SCHED_HISTORY
			/* Save the task name which will be scheduled */
			save_task_scheduling_status(rtcb);
#endif

			/* Then switch contexts */
			up_fullcontextrestore(rtcb->xcp.regs);
		}
	}
}
