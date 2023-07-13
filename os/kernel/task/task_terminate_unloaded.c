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
/************************************************************************
 * kernel/task/task_terminate_unloaded.c
 *
 *   Copyright (C) 2007, 2009, 2012-2014 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <sched.h>
#include <queue.h>
#include <arch/irq.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_ARM_MPU)
#include <tinyara/mpu.h>
#endif

#include "task/task.h"
#include "sched/sched.h"
#include "signal/signal.h"
#ifdef CONFIG_TASK_MONITOR
#include "task_monitor/task_monitor.h"
#endif
#ifdef CONFIG_PREFERENCE
#include "preference/preference.h"
#endif
#ifdef HAVE_TASK_GROUP
#include "group/group.h"
#endif

/************************************************************************
 * Public Functions
 ************************************************************************/
/****************************************************************************
 * Name: task_terminate_unloaded
 *
 * Description:
 *   This function is almost same as task_terminate(),
 *   but it performs essential operations except like recovery for resources, task_recover.
 *   or signaling to other tasks, task_signalparent.
 *   That's because it is called after binary manager recovers resources.
 *   Also, it doesn't need to signaling because all task/threads would be terminated.
 *
 * Parameters:
 *   tcb - The TCB to be released
 *
 * Return Value:
 *   OK on success; ERROR on failure
 *
 ****************************************************************************/
int task_terminate_unloaded(FAR struct tcb_s *tcb)
{
	irqstate_t saved_state;

	/* Suppress context changes for a bit so that the task is unloaded seemlessly.
	 */

	sched_lock();

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_ARM_MPU)
	/* Disable mpu regions when the binary is unloaded if its own mpu registers are set in mpu h/w. */
	if (IS_BINARY_MAINTASK(tcb) && up_mpu_check_active(&tcb->mpu_regs[0])) {
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		for (int i = 0; i < MPU_REG_NUMBER * NUM_APP_REGIONS; i += MPU_REG_NUMBER) {
			up_mpu_disable_region(&tcb->mpu_regs[i]);
		}
#else
		up_mpu_disable_region(&tcb->mpu_regs[0]);
#endif
	}
#endif

#if defined(CONFIG_SCHED_ATEXIT) || defined(CONFIG_SCHED_ONEXIT)
	task_atexit(tcb);
	task_onexit(tcb, EXIT_SUCCESS);
#endif
#ifdef HAVE_TASK_GROUP
	group_leave(tcb);
#endif
	sig_cleanup(tcb);

	saved_state = irqsave();
	dq_rem((FAR dq_entry_t *)tcb, (dq_queue_t *)g_tasklisttable[tcb->task_state].list);
	irqrestore(saved_state);

#ifdef CONFIG_TASK_MONITOR
	/* Unregister this pid from task monitor */
	task_monitor_unregester_list(tcb->pid);
#endif
#ifdef CONFIG_PREFERENCE
	preference_clear_callbacks(tcb->pid);
#endif
	tcb->flags |= TCB_FLAG_EXIT_PROCESSING;

	/* No need to release the unloading thread's stack,
	 * because whole heap memory will be released at one go.
	 * So marking alloc pointer to NULL for skipping to release.
	 */
	tcb->stack_alloc_ptr = NULL;

	sched_unlock();

	/* Deallocate its TCB */

	return sched_releasetcb(tcb, tcb->flags & TCB_FLAG_TTYPE_MASK);
}
