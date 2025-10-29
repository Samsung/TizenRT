/****************************************************************************
 * kernel/sched/sched_migrate_tasks_online.c
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
 * Name: sched_migrate_tasks_online
 *
 * Description:
 *   Brings back an offline CPU by updating the active CPUs mask to indicate
 *   that the specified CPU is active again. This function is used to restore
 *   a previously offline CPU to the active CPU pool.
 *
 *   IMPORTANT: This function must be called from a running CPU while the
 *   target 'offline_cpu' is being brought back online. The caller must
 *   ensure preemption is locked or this is called in a context where it
 *   cannot be preempted.
 *
 * Inputs:
 *   offline_cpu - The index of the CPU that is being brought back online.
 *
 * Return Value:
 *   OK (0) on success, or a negated errno value on failure.
 *
 ****************************************************************************/

int sched_migrate_tasks_online(int offline_cpu)
{
	irqstate_t flags;
	int ret = OK;

	/* Basic sanity checks */
	if (offline_cpu < 0 || offline_cpu >= CONFIG_SMP_NCPUS) {
		lldbg("ERROR: Invalid offline_cpu: %d\n", offline_cpu);
		return -EINVAL;
	}

	if (offline_cpu == this_cpu()) {
		lldbg("ERROR: Cannot bring back the current running CPU: %d\n", offline_cpu);
		return -EINVAL;
	}

	/* Lock the scheduler and disable interrupts to ensure atomicity
	 * throughout this critical operation.
	 */
	flags = enter_critical_section();

	/* Update the active CPUs mask to indicate that the offline CPU is active again */
	g_active_cpus_mask |= ((cpu_set_t)1 << offline_cpu);


	leave_critical_section(flags);

	return ret;
}

#endif							/* CONFIG_SCHED_MIGRATE */
