/****************************************************************************
 * sched/sched/sched_backtrace.c
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/sched.h>
#include <tinyara/arch.h>
#include <tinyara/spinlock.h>

#include "sched/sched.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_backtrace
 *
 * Description:
 *   Get thread backtrace from specified tid.
 *   Store up to SIZE return address of the current program state in
 *   the array pointed to by BUFFER.
 *
 * Input Parameters:
 *   tid   - Task ID to get backtrace
 *   buffer - Buffer to store backtrace
 *   size   - Size of buffer
 *   skip   - Number of frames to skip
 *
 * Returned Value:
 *   Number of frames captured, or negative error code on failure
 *
 ****************************************************************************/

int sched_backtrace(pid_t tid, FAR void **buffer, int size, int skip)
{
	FAR struct tcb_s *tcb;
	irqstate_t flags;
	int ret = 0;

	if (size <= 0 || !buffer) {
		return -EINVAL;
	}

	/* Get the TCB with critical section to prevent race with task exit */
	flags = enter_critical_section();

	tcb = sched_gettcb(tid);
	if (!tcb) {
		leave_critical_section(flags);
		return -ESRCH;
	}

	/* Validate TCB state - avoid racing with task exit */
	if (tcb->task_state == TSTATE_TASK_INVALID ||
	    (tcb->flags & TCB_FLAG_EXIT_PROCESSING) != 0) {
		leave_critical_section(flags);
		return -ESRCH;
	}

#ifdef CONFIG_SMP
	/* If the task is running on another CPU, we need to use IPI */

	if (tcb->cpu != this_cpu() && tcb->task_state == TSTATE_TASK_RUNNING) {
		/* IPI-based backtrace not supported in TizenRT */
		lldbg("Backtrace: Task %d running on CPU %d, IPI not supported\n", tid, tcb->cpu);
		leave_critical_section(flags);
		return 0;
	}
#endif
	leave_critical_section(flags);

	/* Task is not running or on the same CPU, we can safely get the backtrace */
	/* asserted_location=0 for normal backtrace (not from ASSERT) */
	ret = up_backtrace(tcb, buffer, size, skip, 0);

	return ret;
}

#ifdef CONFIG_ARCH_STACKDUMP
/****************************************************************************
 * Name: sched_dumpstack
 *
 * Description:
 *   Dump the stack of the specified task.
 *
 ****************************************************************************/

void sched_dumpstack(pid_t tid)
{
	FAR struct tcb_s *tcb;
	irqstate_t flags;

	/* Common validation for both paths */
	flags = enter_critical_section();

	tcb = sched_gettcb(tid);
	if (!tcb) {
		leave_critical_section(flags);
		_lldbg("Task %d not found\n", tid);
		return;
	}

	/* Validate TCB state */
	if (tcb->task_state == TSTATE_TASK_INVALID ||
	    (tcb->flags & TCB_FLAG_EXIT_PROCESSING) != 0) {
		leave_critical_section(flags);
		_lldbg("Task %d not found (invalid state)\n", tid);
		return;
	}

	leave_critical_section(flags);

#ifdef CONFIG_ARCH_BACKTRACE_MAX_FRAMES
	void *buffer[CONFIG_ARCH_BACKTRACE_MAX_FRAMES];
	int size;
	int i;

	size = sched_backtrace(tid, buffer, CONFIG_ARCH_BACKTRACE_MAX_FRAMES, 0);
	if (size < 0) {
		_lldbg("Failed to get backtrace for task %d: %d\n", tid, size);
		return;
	}

	_lldbg("Backtrace for task %d (%s):\n", tid, tcb->name);
	for (i = 0; i < size; i++) {
		_lldbg("  [%d]: %p\n", i, buffer[i]);
	}
#else
	/* Fallback when CONFIG_ARCH_BACKTRACE_MAX_FRAMES is not defined */
	_lldbg("Backtrace for task %d (%s) not available (CONFIG_ARCH_BACKTRACE_MAX_FRAMES undefined)\n", tid, tcb->name);
#endif
}
#endif							/* CONFIG_ARCH_STACKDUMP */

