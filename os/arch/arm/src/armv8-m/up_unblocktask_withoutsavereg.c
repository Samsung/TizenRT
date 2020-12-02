/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 *  arch/arm/src/armv8-m/up_unblock_task_without_savereg.c
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
#include <tinyara/arch.h>

#include "sched/sched.h"
#include "clock/clock.h"
#include "up_internal.h"
#ifdef CONFIG_ARMV8M_MPU
#include "mpu.h"
#include <tinyara/mpu.h>
#endif

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
#ifdef CONFIG_SUPPORT_COMMON_BINARY
extern uint32_t *g_umm_app_id;
#endif
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_unblock_task_without_savereg
 *
 * Description:
 *   Move the TCB to the ready-to-run list and Switch context to the context
 *   of the task at the head of the ready to run list without saving old context.
 *   If it is in interrupt context, it doesn't copy the current_regs into the OLD tcb.
 *   If not, it calls up_fullcontextrestore to perform the user context switch.
 *
 * Inputs:
 *   tcb: Refers to the tcb to be unblocked.  This tcb is
 *     in one of the waiting tasks lists.  It must be moved to
 *     the ready-to-run list and, if it is the highest priority
 *     ready to run taks, executed.
 *
 ****************************************************************************/
void up_unblock_task_without_savereg(struct tcb_s *tcb)
{
	struct tcb_s *rtcb;

	/* Remove the task from the blocked task list */
	dq_rem((FAR dq_entry_t *)tcb, (dq_queue_t *)g_tasklisttable[tcb->task_state].list);

	/* Reset its timeslice.  This is only meaningful for round
	* robin tasks but it doesn't here to do it for everything
	*/

#if CONFIG_RR_INTERVAL > 0
	tcb->timeslice = MSEC2TICK(CONFIG_RR_INTERVAL);
#endif

	/* Add the task in the correct location in the prioritized
	* g_readytorun task list
	*/

	if (sched_addprioritized(tcb, (FAR dq_queue_t *)&g_readytorun)) {
		/* The new btcb was added at the head of the ready-to-run list.	It
		* is now to new active task!
		*/

		ASSERT(tcb->flink != NULL);
		tcb->task_state = TSTATE_TASK_RUNNING;
		tcb->flink->task_state = TSTATE_TASK_READYTORUN;

	} else {
		/* The new btcb was added in the middle of the ready-to-run list */

		tcb->task_state = TSTATE_TASK_READYTORUN;
	}

	/*
	* Are we in an interrupt handler?
	*/
	if (current_regs) {

		/* Restore the exception context of the rtcb at the (new) head
		 * of the g_readytorun task list.
		 */

		rtcb = this_task();

#ifdef CONFIG_TASK_SCHED_HISTORY
		/* Save the task name which will be scheduled */
		save_task_scheduling_status(rtcb);
#endif

		/* Restore the MPU registers in case we are switching to an application task */
#ifdef CONFIG_ARMV8M_MPU
		/* Condition check : Update MPU registers only if this is not a kernel thread. */
		if ((rtcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
#if defined(CONFIG_APP_BINARY_SEPARATION)
			for (int i = 0; i < MPU_REG_NUMBER * MPU_NUM_REGIONS; i += MPU_REG_NUMBER) {
				up_mpu_set_register(&rtcb->mpu_regs[i]);
			}
#endif
		}
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
		up_mpu_set_register(rtcb->stack_mpu_regs);
#endif
#endif

#ifdef CONFIG_SUPPORT_COMMON_BINARY
		if (g_umm_app_id) {
			*g_umm_app_id = rtcb->app_id;
		}
#endif
#ifdef CONFIG_TASK_MONITOR
		/* Update rtcb active flag for monitoring. */
		rtcb->is_active = true;
#endif

#ifdef CONFIG_ARMV8M_TRUSTZONE
		if (rtcb->tz_context) {
			TZ_LoadContext_S(rtcb->tz_context);
		}
#endif
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
		/* Then switch contexts */

		up_fullcontextrestore(nexttcb->xcp.regs);
	}
}
