/****************************************************************************
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
 * kernel/sched/sched_cpuoff.c
 *
 * Description:
 *   This file implements the CPU control's power off function  which handles
 *   CPU off operations both from user applications (via CPU driver) and
 *   from the PM idle logic.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sched.h>
#include <debug.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>

#include <tinyara/arch.h>
#include <tinyara/spinlock.h>
#include <tinyara/irq.h>
#include <tinyara/sched.h>

#include "sched/sched.h"


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
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
 * Name: sched_cpuoff
 *
 * Description:
 *   Request to bring a CPU to power off state.
 *
 * Input Parameters:
 *   cpu - The CPU index to turn off.
 *
 * Returned Value:
 *   OK (0) on success, Negative value on failure.
 *
 * Preconditions(Checks):
 *   1. CPU cannot turn itself OFF.
 *   2. CPU number must be valid (1 ... CONFIG_SMP_NCPUS).
 *   3. CPU 0 cannot be turned OFF.
 *   4. If task migration is required i.e. cpu is executing task other than
 *      IDLE task, don't call this API in critical_section.
 *
 ****************************************************************************/

int sched_cpuoff(int cpu)
{
	FAR struct tcb_s *tcb;
	irqstate_t flags;
	int ret = OK;
	
	if (cpu == this_cpu()) {
		slldbg("Cannot turn off CPU from itself: %d\n", cpu);
		return -EINVAL;
	}

	/* Validate CPU index */
	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		slldbg("Invalid CPU number: %d\n", cpu);
		return -EINVAL;
	}

	tcb = current_task(cpu);
	if (tcb->pid != cpu) {
		//Trying to turnoff a busy CPU, error case
		return -EBUSY;
	}

	if (spin_trylock(&g_state_transition_lock) == SP_LOCKED) {
		return -EBUSY;
	}

	if (CPU_ISSET(cpu, &g_active_cpus_mask)) {
		/* Lock the scheduler and disable interrupts to ensure atomicity
		 * throughout this critical operation.
		 */
		flags = enter_critical_section();
		/* Update the active CPUs mask to indicate that the offline CPU is no longer active */
		CPU_CLR(cpu, &g_active_cpus_mask);
		//Call up_cpu_off function
		ret = up_cpu_off(cpu);
		if (ret != 0) {
			CPU_SET(cpu, &g_active_cpus_mask);
		}
		slldbg("CPU%d is turned off successfully\n", cpu);
		leave_critical_section(flags);
	} else {
		slldbg("Already desired state (OFF), operation not supported\n");
		ret = -ENOTSUP;
	}
errout_with_global_lock:
	spin_unlock(&g_state_transition_lock);
	return ret;
}

