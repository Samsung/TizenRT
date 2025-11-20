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
 * kernel/sched/sched_cpucontroloff.c
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
#include <tinyara/arch.h>
#include <tinyara/spinlock.h>
#include <tinyara/irq.h>
#include <tinyara/sched.h>
#include <sched.h>
#include <debug.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>

#ifdef CONFIG_SMP
#include <../arch/arm/src/armv7-a/smp.h>
#include <../arch/arm/src/armv7-a/barriers.h>
#endif

#include "sched/sched.h"

#ifdef CONFIG_CPU_MANAGER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Spinlock for synchronizing access to CPU hotplug states */
volatile spinlock_t g_cpuhp_lock[CONFIG_SMP_NCPUS];

/* Spinlock for synchronizing access to global hotplug state */
volatile spinlock_t g_state_transition_lock = SP_UNLOCKED;

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
 *   cpu - The CPU index to turn of
 *   migrate - flag to enable task migration.
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/

int sched_cpuoff(int cpu, bool migrate)
{
    if(spin_trylock(&g_cpuhp_lock[cpu])  == SP_LOCKED) {
        slldbg("CPU state change is already in progress\n");
	return -EBUSY;
    }

    spin_lock(&g_state_transition_lock);

    int ret = OK;

    /* Validate CPU index */
    if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
	slldbg("Invalid CPU number: %d\n", cpu);
        ret = -EINVAL;
	goto errout_with_global_lock;
    }

    if (cpu == this_cpu()) {
	slldbg("Cannot turn off this CPU from itself: %d\n", cpu);
	ret = -EINVAL;
	goto errout_with_global_lock;
    }

    if(CPU_ISSET(cpu, &g_active_cpus_mask)) {
        //check migrate flag migrate all task off of CPU if set.
	if(migrate) {
	    ret = sched_migrate_tasks(cpu);
	    if(ret != 0) {
	        goto errout_with_global_lock;
	    }
	}
        //Call up_cpu_off function
	ret = up_cpu_off(cpu);
    } else {
	slldbg("Already desired state (OFF), operation not supported\n");
        ret = -ENOTSUP;
    }
errout_with_global_lock:
    spin_unlock(&g_state_transition_lock);
    spin_unlock(&g_cpuhp_lock[cpu]);
    return ret;
}
#endif


