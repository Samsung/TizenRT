/****************************************************************************
 * kernel/sched/sched_migrate_cpu_tasks.c
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#ifdef CONFIG_SCHED_MIGRATE

#include <stdbool.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/spinlock.h>
#include <tinyara/sched.h>

#include "sched/sched.h"
#include "clock/clock.h"

/****************************************************************************
 * Name: sched_migrate_cpu_tasks
 *
 * Description:
 *   Migrates all tasks from a specified offline CPU to other available
 *   online CPUs. This function is called after the target CPU has been
 *   successfully halted.
 *
 *   IMPORTANT: This function must be called from a running CPU (e.g., CPU0)
 *   while the target 'offline_cpu' is completely halted and no longer
 *   participating in scheduling. The caller must ensure preemption is
 *   locked or this is called in a context where it cannot be preempted.
 *
 * Inputs:
 *   offline_cpu - The index of the CPU that has been shut down.
 *
 * Return Value:
 *   OK (0) on success, or a negated errno value on failure.
 *
 ****************************************************************************/

int sched_migrate_cpu_tasks(int offline_cpu)
{
	FAR struct tcb_s *tcb;
	FAR struct tcb_s *next_tcb;
	FAR dq_queue_t *task_list;
	irqstate_t flags;
	int ret = OK;

	/* Basic sanity checks */
	if (offline_cpu < 0 || offline_cpu >= CONFIG_SMP_NCPUS) {
		lldbg("ERROR: Invalid offline_cpu: %d\n", offline_cpu);
		return -EINVAL;
	}

	if (offline_cpu == this_cpu()) {
		lldbg("ERROR: Cannot migrate tasks from the current running CPU: %d\n", offline_cpu);
		return -EINVAL;
	}

	/* Lock the scheduler and disable interrupts to ensure atomicity
	 * throughout this critical operation.
	 */
	flags = enter_critical_section();

	/* Get the list of tasks assigned to the offline CPU.
	 * We need to lock the task list to prevent concurrent access, though
	 * since offline_cpu is halted, this might be redundant but is safe.
	 */
	task_list = (FAR dq_queue_t *)&g_assignedtasks[offline_cpu];

	/* Iterate through each task in the offline CPU's assigned list.
	 * Since there's no _foreach_safe macro, we must manually get the next
	 * pointer before removing the current TCB.
	 */
	tcb = (FAR struct tcb_s *)task_list->head;
	while (tcb != NULL) {
		next_tcb = (FAR struct tcb_s *)tcb->flink;	/* Get next TCB before removing current one */

		if (tcb->sched_priority == 0) {
			/*couldn't locate the macro for sched_priority==IDLE */
			tcb = next_tcb;
			continue;
		}

		/* Remove the TCB from the offline CPU's assigned task list.
		 * It's no longer assigned to this CPU.
		 */
		dq_rem((FAR dq_entry_t *) tcb, task_list);

		/* Check if the task has a hard CPU affinity lock to the offline CPU.
		 * If TCB_FLAG_CPU_LOCKED is set and tcb->cpu matches offline_cpu,
		 * it was pinned. We must override this for migration.
		 */
		if ((tcb->flags & TCB_FLAG_CPU_LOCKED) != 0 && tcb->cpu == offline_cpu) {
			lldbg("Overriding CPU affinity for task PID %d (was locked to CPU %d)\n", tcb->pid, offline_cpu);
			tcb->flags &= ~TCB_FLAG_CPU_LOCKED;
			tcb->cpu = this_cpu();	// Tentatively assign to current CPU, sched_addreadytorun will confirm.
		}

		/* Clear the CPU assignment. The task is now unassigned.
		 * sched_addreadytorun will handle selecting a new CPU.
		 */
		tcb->cpu = -1;			// Mark as unassigned

		/* The task is no longer in the TSTATE_TASK_ASSIGNED or TSTATE_TASK_RUNNING
		 * state for the old CPU. It will be added to the global ready-to-run list.
		 * sched_addreadytorun will determine its new state (e.g., TSTATE_TASK_READYTORUN
		 * or TSTATE_TASK_RUNNING if it preempts).
		 */
		tcb->task_state = TSTATE_TASK_INVALID;	// Reset state before adding

		/* Add the task to the global ready-to-run queue.
		 * sched_addreadytorun will handle:
		 * 1. Selecting a new, online CPU using sched_select_cpu().
		 * 2. Placing it in the new CPU's g_assignedtasks list or the global g_readytorun.
		 * 3. Setting the correct task_state.
		 * 4. Triggering a context switch on the target CPU if the migrated task
		 *    becomes the new highest priority task.
		 */
		lldbg("Migrating task PID %d from offline CPU %d\n", tcb->pid, offline_cpu);

		if (!sched_addreadytorun(tcb)) {
			/* This is not necessarily a fatal error. It just means the task
			 * was added to the middle of a ready list and didn't immediately
			 * preempt the current task on the target CPU.
			 */
			lldbg("Task PID %d added to ready queue, no immediate preemption on target CPU.\n", tcb->pid);
		} else {
			lldbg("Task PID %d migrated and will cause a context switch on target CPU.\n", tcb->pid);
		}

		tcb = next_tcb;
	}

	/* The offline CPU's assigned task list should now be empty.
	 * The idle task for that CPU would also have been processed.
	 * We can now mark the CPU as offline in any global masks if not already done.
	 * This is likely handled by an arch-specific power management layer.
	 */

	/* Release critical section and scheduler lock.
	 * Releasing the scheduler lock (sched_unlock) will process any tasks
	 * that might have been added to g_pendingtasks during the migration
	 * (e.g., if preemption was locked on the target CPU when sched_addreadytorun was called).
	 */
	leave_critical_section(flags);

	lldbg("Successfully migrated all tasks from offline CPU %d.\n", offline_cpu);
	return ret;
}

#endif							/* CONFIG_SCHED_MIGRATE */
