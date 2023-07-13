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
 * kernel/task/task_terminate.c
 *
 *   Copyright (C) 2007-2009, 2011-2014 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <assert.h>
#include <queue.h>
#include <errno.h>

#include <tinyara/sched.h>
#include <tinyara/ttrace.h>
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_ARM_MPU)
#include <tinyara/mpu.h>
#endif

#ifndef CONFIG_DISABLE_SIGNALS
#ifndef CONFIG_DISABLE_PTHREAD
#include <pthread.h>
#endif
#include <unistd.h>
#endif
#include <arch/irq.h>

#include "sched/sched.h"
#ifndef CONFIG_DISABLE_SIGNALS
#include "signal/signal.h"
#endif
#include "task/task.h"

#ifdef CONFIG_TASK_MONITOR
#include "task_monitor/task_monitor_internal.h"
#endif
#ifdef CONFIG_PREFERENCE
#include "preference/preference.h"
#endif
/****************************************************************************
 * Definitions
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
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_terminate
 *
 * Description:
 *   This function causes a specified task to cease to exist. Its  stack and
 *   TCB will be deallocated.  This function is the internal implementation
 *   of the task_delete() function.  It includes and additional parameter
 *   to determine if blocking is permitted or not.
 *
 *   This function is the final function called all task termination
 *   sequences.  task_terminate() is called only from task_delete() (with
 *   nonblocking == false) and from task_exit() (with nonblocking == true).
 *
 *   The path through task_exit() supports the final stops of the exit(),
 *   _exit(), and pthread_exit
 *
 *   - pthread_exit().  Calls _exit()
 *   - exit(). Calls _exit()
 *   - _exit().  Calls task_exit() making the currently running task
 *     non-running. task_exit then calls task_terminate() (with nonblocking
 *     == true) to terminate the non-running task.
 *
 *   NOTE: that the state of non-blocking is irrelevant when called through
 *   exit() and pthread_exit().  In those cases task_exithook() has already
 *   been called with nonblocking == false;
 *
 * Inputs:
 *   pid - The task ID of the task to delete.  A pid of zero
 *         signifies the calling task.
 *   nonblocking - True: The task is an unhealthy, partially torn down
 *         state and is not permitted to block.
 *
 * Return Value:
 *   OK on success; or ERROR on failure
 *
 *   This function can fail if the provided pid does not correspond to a
 *   task (errno is not set)
 *
 ****************************************************************************/

int task_terminate(pid_t pid, bool nonblocking)
{
	FAR struct tcb_s *dtcb;
	irqstate_t saved_state;
	trace_begin(TTRACE_TAG_TASK, "task_terminate");

	/* Make sure the task does not become ready-to-run while we are futzing with
	 * its TCB by locking ourselves as the executing task.
	 */

	sched_lock();

	/* Find for the TCB associated with matching PID */

	dtcb = sched_gettcb(pid);
	if (!dtcb) {
		/* This PID does not correspond to any known task */

		sched_unlock();
		trace_end(TTRACE_TAG_TASK);
		return -ESRCH;
	}

	/* Verify our internal sanity */

	if (dtcb->task_state == TSTATE_TASK_RUNNING || dtcb->task_state >= NUM_TASK_STATES) {
		sched_unlock();
		PANIC();
	}

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_ARM_MPU)
	/* Disable mpu regions when the binary is unloaded if its own mpu registers are set in mpu h/w. */
	if (IS_BINARY_MAINTASK(dtcb) && up_mpu_check_active(&dtcb->mpu_regs[0])) {
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		for (int i = 0; i < MPU_REG_NUMBER * NUM_APP_REGIONS; i += MPU_REG_NUMBER) {
			up_mpu_disable_region(&dtcb->mpu_regs[i]);
		}
#else
		up_mpu_disable_region(&dtcb->mpu_regs[0]);
#endif
	}
#endif

	/* Perform common task termination logic (flushing streams, calling
	 * functions registered by at_exit/on_exit, etc.).  We need to do
	 * this as early as possible so that higher level clean-up logic
	 * can run in a healthy tasking environment.
	 *
	 * In the case where the task exits via exit(), task_exithook()
	 * may be called twice.
	 *
	 * I suppose EXIT_SUCCESS is an appropriate return value???
	 */

	task_exithook(dtcb, EXIT_SUCCESS, nonblocking);

	/* Remove the task from the OS's tasks lists. */

	saved_state = irqsave();
	dq_rem((FAR dq_entry_t *)dtcb, (dq_queue_t *)g_tasklisttable[dtcb->task_state].list);
	dtcb->task_state = TSTATE_TASK_INVALID;
#ifdef CONFIG_TASK_MONITOR
	/* Unregister this pid from task monitor */
	task_monitor_unregester_list(pid);
#endif
#ifdef CONFIG_PREFERENCE
	preference_clear_callbacks(pid);
#endif
	irqrestore(saved_state);

	/* At this point, the TCB should no longer be accessible to the system */

	sched_unlock();

	trace_end(TTRACE_TAG_TASK);

	/* Deallocate its TCB */

	return sched_releasetcb(dtcb, dtcb->flags & TCB_FLAG_TTYPE_MASK);
}

/****************************************************************************
 * Name: thread_termination_handler
 *
 * Description:
 *   The handler called when SIGKILL is receive. Terminate the thread
 *   that received the signal immediately.
 *
 * Inputs:
 *   none
 *
 * Return Value:
 *   none
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_SIGNALS
void thread_termination_handler(void)
{
	struct tcb_s *rtcb = sched_self();
	if (!rtcb) {
		set_errno(ESRCH);
		return;
	}

	switch ((rtcb->flags & TCB_FLAG_TTYPE_MASK) >> TCB_FLAG_TTYPE_SHIFT) {
	case TCB_FLAG_TTYPE_TASK:
	case TCB_FLAG_TTYPE_KERNEL:
		/* tasks and kernel threads has to use this interface */
		(void)task_delete(rtcb->pid);
		break;
#ifndef CONFIG_DISABLE_PTHREAD
	case TCB_FLAG_TTYPE_PTHREAD:
		(void)pthread_cancel(rtcb->pid);
		(void)pthread_join(rtcb->pid, NULL);
		break;
#endif
	default:
		set_errno(EINVAL);
		break;
	}
	return;
}
#endif
