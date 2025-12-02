/****************************************************************************
 * kernel/sched/sched_migrate_tasks.c
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
 * Name: sched_migrate_tasks
 *
 * Description:
 *   Migrates all tasks from a specified offline CPU to other available
 *   online CPUs.
 *
 *   IMPORTANT: This function must be called from a running CPU (e.g., CPU0)
 *   The caller must ensure preemption is locked or this is called
 *   in a context where it cannot be preempted. And the cpu from which the
 *   tasks need to be migrated should cleared from the g_active_cpus_mask.
 *
 * Inputs:
 *   offline_cpu - The index of the CPU that needs to be shut down.
 *
 * Return Value:
 *   OK (0) on success, or a negated errno value on failure.
 *
 ****************************************************************************/

int sched_migrate_tasks(int offline_cpu)
{
	FAR struct tcb_s *tcb;
	FAR struct tcb_s *priv_tcb;
	FAR dq_queue_t *task_list;
	irqstate_t flags;
	int ret = OK;

	/* Basic sanity checks */
	if (offline_cpu < 0 || offline_cpu >= CONFIG_SMP_NCPUS) {
		slldbg("ERROR: Invalid offline_cpu: %d\n", offline_cpu);
		return -EINVAL;
	}

	if (offline_cpu == this_cpu()) {
		slldbg("ERROR: Cannot migrate tasks from the current running CPU: %d\n", offline_cpu);
		return -EINVAL;
	}

	if (CPU_ISSET(offline_cpu, &g_active_cpus_mask)) {
		slldbg("ERROR: CPU_%d is not cleared from the active cpus mask\n", offline_cpu);
		return -EINVAL;
	}

	/* Lock the scheduler to prevent preemption during this operation.
	 * This ensures that no other tasks can be scheduled while we're
	 * migrating tasks from the offline CPU.
	 */
	sched_lock();

	/* Disable interrupts to ensure atomicity throughout this critical operation. */
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

	tcb = (FAR struct tcb_s *)task_list->tail;

	DEBUGASSERT(tcb->flink == NULL);
	tcb = tcb->blink;	/* Skipping the idle task */

	while (tcb != NULL) {
		priv_tcb = (FAR struct tcb_s *)tcb->blink;	/* Get previous TCB before removing current one */

		/* Remove the tcb from the appropriate task list
		 * and assign it an appropriate task state
		 */
		ASSERT(!sched_removereadytorun(tcb));

		sllvdbg("Migrating task PID %d from offline CPU %d\n", tcb->pid, offline_cpu);

		/* Add the tcb to the appropriate task list */
		ASSERT(!sched_addreadytorun(tcb))

		tcb = priv_tcb;
	}

	/* The offline CPU's assigned task list should now be empty.
	 * The idle task for that CPU would also have been processed.
	 * We can now mark the CPU as offline in any global masks if not already done.
	 */
	DEBUGASSERT((task_list)->head->flink == NULL);	/* Assert if the head of assingned list is not idle */

	/* Release critical section and scheduler lock.
	 * Releasing the scheduler lock (sched_unlock) will process any tasks
	 * that might have been added to g_pendingtasks during the migration
	 * (e.g., if preemption was locked on the target CPU when sched_addreadytorun was called).
	 */
	leave_critical_section(flags);

	sched_unlock();

	sllvdbg("Successfully migrated all tasks from offline CPU %d.\n", offline_cpu);
	return ret;
}

#endif							/* CONFIG_SCHED_MIGRATE */
