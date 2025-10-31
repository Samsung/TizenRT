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
 * kernel/power/pm_hotplug.c
 *
 * Description:
 *   This file implements the CPU hotplug manager module which handles CPU
 *   hotplug operations both from user applications (via syscalls) and from
 *   the PM idle logic.
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
#include <tinyara/cpu_state.h>
#include <power/hotplug_manager.h>
#include <sched.h>
#include <debug.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>

#ifdef CONFIG_SMP
#include <../arch/arm/src/armv7-a/smp.h>
#include <../arch/arm/src/armv7-a/barriers.h>
#endif

#include "pm_hotplug.h"
#include "sched/sched.h"

#ifdef CONFIG_CPU_HOTPLUG_MANAGER
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* CPU hotplug state flags for each core */
static volatile cpu_state_t g_cpuhp_state[CONFIG_SMP_NCPUS];

/* Spinlock for synchronizing access to CPU hotplug states */
static volatile spinlock_t g_cpuhp_lock[CONFIG_SMP_NCPUS];

/* Flag to indicate if user-initiated hotplug operations are in progress */
static volatile bool g_user_hotplug_in_progress = false;

/* Spinlock for synchronizing access to global hotplug state */
static volatile spinlock_t g_global_hotplug_lock = SP_UNLOCKED;

/* Define the missing g_cpu_tasklistlock */
volatile spinlock_t g_cpu_tasklistlock = SP_UNLOCKED;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cpu_hotplug_lock
 *
 * Description:
 *   Acquire the hotplug lock for a specific CPU.
 *
 * Input Parameters:
 *   cpu - The CPU index to lock
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void cpu_hotplug_lock(int cpu)
{
	DEBUGASSERT(cpu >= 0 && cpu < CONFIG_SMP_NCPUS);
	spin_lock(&g_cpuhp_lock[cpu]);
}

/****************************************************************************
 * Name: cpu_hotplug_unlock
 *
 * Description:
 *   Release the hotplug lock for a specific CPU.
 *
 * Input Parameters:
 *   cpu - The CPU index to unlock
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void cpu_hotplug_unlock(int cpu)
{
	DEBUGASSERT(cpu >= 0 && cpu < CONFIG_SMP_NCPUS);
	spin_unlock(&g_cpuhp_lock[cpu]);
}

/****************************************************************************
 * Name: cpu_hotplug_global_lock
 *
 * Description:
 *   Acquire the global hotplug lock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void cpu_hotplug_global_lock(void)
{
	spin_lock(&g_global_hotplug_lock);
}

/****************************************************************************
 * Name: cpu_hotplug_global_unlock
 *
 * Description:
 *   Release the global hotplug lock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void cpu_hotplug_global_unlock(void)
{
	spin_unlock(&g_global_hotplug_lock);
}

/****************************************************************************
 * Name: cpu_hotplug_manager_init
 *
 * Description:
 *   Initialize the CPU hotplug manager.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/
int cpu_hotplug_manager_init(void)
{
	int cpu;

	svdbg("Initializing CPU hotplug manager\n");
	pmlldbg("Initializing CPU hotplug manager\n");
	//printf("[Vivek Jain] Initializing CPU hotplug manager\n");

	/* Initialize the CPU states */
	for (cpu = 0; cpu < CONFIG_SMP_NCPUS; cpu++) {
		g_cpuhp_state[cpu] = up_get_cpu_state(cpu);
	//	printf("[Vivek Jain] CPU state for CPU%d is %d\n", cpu, g_cpuhp_state[cpu]);
	}

	/* CPU 0 is always online */
	g_cpuhp_state[0] = CPU_ONLINE;

	/* Initialize the task list lock */
	sem_init(&g_cpu_tasklistlock, 0, 1);

	svdbg("CPU hotplug manager initialized successfully\n");
	pmlldbg("CPU hotplug manager initialized successfully\n");
	return OK;
}

/****************************************************************************
 * Name: perform_hotplug
 *
 * Description:
 *   Request to take a CPU offline (user-initiated).
 *
 * Input Parameters:
 *   cpu - The CPU index to take offline
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/
int perform_hotplug(int cpu)
{
    if(g_user_hotplug_in_progress)
    {
        //Already in progress don't raise another request
        return -EINVAL;
    }
	printf("[Vivek Jain] ENTER - perform_hotplug for CPU%d\n", cpu);
	int ret = OK;
	//irqstate_t flags;

	/* Validate CPU index */
	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		printf("[Vivek Jain] CPU index invalid CPU%d, CONFIG_SMP_NCPUS: %d\n", cpu, CONFIG_SMP_NCPUS);
		return -EINVAL;
	}

	/* Check if CPU is already offline */
	if ((g_cpuhp_state[cpu] = up_get_cpu_state(cpu)) == CPU_OFFLINE) {
		printf("[Vivek Jain] CPU state offline for CPU%d\n", cpu);
		return OK;
	}

	if (cpu == this_cpu()) {
		printf("Cannot Hotplug current CPU from itself\n");
		return -1;
	}

	/* Acquire global lock to indicate user operation in progress */
	cpu_hotplug_global_lock();

	/* Set flag to indicate user-initiated operation */
	g_user_hotplug_in_progress = true;

	cpu_hotplug_global_unlock();

	/* Acquire CPU-specific lock */
	cpu_hotplug_lock(cpu);

	/* Check if CPU is available for hotplug */
	if (!cpu_hotplug_is_available(cpu)) {
		printf("[Vivek Jain] Hotplug is unavailable for CPU%d\n", cpu);
		ret = -EBUSY;
		goto errout_with_lock;
	}

	/* Set state to indicate hotplug in progress */
	cpu_hotplug_set_state(cpu, CPU_HOTPLUG_IN_PROGRESS);

	/* Call architecture-specific hotplug function */
    if (up_get_cpu_state(cpu) == CPU_RUNNING) {
		//Must stop all running task on the CPU before taking it offline.
		//Otherwise the hotplug is not possible.
		printf("[Vivek Jain] Migrating tasks for CPU%d\n", cpu);
		sched_lock();
		ret = sched_migrate_cpu_tasks(cpu);
		if (ret < 0) {
			sdbg("Failed to migrate tasks off CPU%d: %d\n", cpu, ret);
			goto errout_with_state;
		}
		
		printf("[Vivek Jain] CPU state for CPU%d is running, CPU hotplug is called\n", cpu);

	    ret = cpu_hotplug(cpu);
		printf("[Vivek Jain] cpu_hotplug return: %d\n", ret);
    }
	/* Update state to OFFLINE */
	cpu_hotplug_set_state(cpu, CPU_OFFLINE);

errout_with_state:
	if (ret < 0) {
		/* Restore state on failure */
		cpu_hotplug_set_state(cpu, up_get_cpu_state(cpu));
	}
	sched_unlock();

errout_with_lock:
	cpu_hotplug_unlock(cpu);

	/* Release global lock */
	cpu_hotplug_global_lock();
	g_user_hotplug_in_progress = false;
	cpu_hotplug_global_unlock();
	
	return ret;
}

/****************************************************************************
 * Name: perform_hotplug_online
 *
 * Description:
 *   Request to bring a CPU online.
 *
 * Input Parameters:
 *   cpu - The CPU index to bring online
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/
int perform_hotplug_online(int cpu)
{
    //TODO: Implementation pending. Complete after getting API from Device team.
    return 0;
}

/****************************************************************************
 * Name: is_hotplug_available
 *
 * Description:
 *   Check if a CPU is available for hotplug operations.
 *
 * Input Parameters:
 *   cpu - The CPU index to check
 *
 * Returned Value:
 *   true if CPU is available, false otherwise.
 *
 ****************************************************************************/
bool is_hotplug_available(int cpu)
{
    /* CPU 0 (primary CPU) is never available for hotplug */
	if (cpu == 0) {
		return false;
	}

	/* Check current state */
	cpu_state_t state = cpu_hotplug_get_state(cpu);
	
	/* CPU is available if it's online or waking from sleep */
	return (state == CPU_ONLINE || state == CPU_WAKE_FROM_SLEEP);
}

/****************************************************************************
 * Name: cpu_hotplug_get_state
 *
 * Description:
 *   Get the current hotplug state of a CPU.
 *
 * Input Parameters:
 *   cpu - The CPU index to check
 *
 * Returned Value:
 *   Current state of the CPU.
 *
 ****************************************************************************/
cpu_state_t cpu_hotplug_get_state(int cpu)
{
	DEBUGASSERT(cpu >= 0 && cpu < CONFIG_SMP_NCPUS);
	return g_cpuhp_state[cpu];
}

/****************************************************************************
 * Name: cpu_offline
 *
 * Description:
 *   Syscall function to take a CPU offline.
 *
 * Input Parameters:
 *   cpu - The CPU index to take offline
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/
int cpu_offline(int cpu)
{
	return perform_hotplug(cpu);
}

/****************************************************************************
 * Name: cpu_online
 *
 * Description:
 *   Syscall function to bring a CPU online.
 *
 * Input Parameters:
 *   cpu - The CPU index to bring online
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/
int cpu_online(int cpu)
{
	return perform_hotplug_online(cpu);
}

#endif