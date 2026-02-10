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
 * kernel/sched/sched_cpuon.c
 *
 * Description:
 *   This file implements the CPU control's power on function  which handles
 *   CPU on operations both from user applications (via CPU driver) and
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

 /* Spinlock for synchronizing access to global hotplug state */
volatile spinlock_t g_state_transition_lock = SP_UNLOCKED;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_cpuon
 *
 * Description:
 *   Request to bring a CPU to power on state.
 *
 * Input Parameters:
 *   cpu - The CPU index to turn on
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 * Preconditions(Checks):
 *   1. CPU cannot turn itself ON.
 *   2. CPU number must be valid (1 ... CONFIG_SMP_NCPUS).
 *   3. CPU 0 cannot be turned ON.
 *
 ****************************************************************************/

int sched_cpuon(int cpu)
{
	irqstate_t flags;
	int ret = OK;
	
	if (cpu == this_cpu()) {
		smplldbg("Cannot turn on this CPU from itself: %d\n", cpu);
		return -EINVAL;
	}

	/* Validate CPU index */
	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		smplldbg("Invalid CPU number: %d\n", cpu);
		return -EINVAL;
	}

	if (spin_trylock(&g_state_transition_lock) == SP_LOCKED) {
		return -EBUSY;
	}


	if (!CPU_ISSET(cpu, &g_active_cpus_mask)) {
		/* Lock the scheduler and disable interrupts to ensure atomicity
		 * throughout this critical operation.
		 */
		flags = enter_critical_section();
		ret = up_cpu_on(cpu);
		if (ret == 0) {
			smpllvdbg("CPU%d is turned on succesfully\n", cpu);
			/* Update the active CPUs mask to indicate that the offline CPU is active again */
			CPU_SET(cpu, &g_active_cpus_mask);
		} else {
			smplldbg("Failed to turn on CPU%d\n", cpu);
		}
		leave_critical_section(flags);
	} else {
		smplldbg("Already desired state (ON), operation not supported\n");
		ret = -ENOTSUP;
	}
	spin_unlock(&g_state_transition_lock);
	return ret;
}

