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
 * arch/arm/src/armv7-a/arm_cpustop.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/sched_note.h>
#include <tinyara/cpu_state.h>

#include "up_internal.h"
#include "cp15_cacheops.h"
#include "gic.h"
#include "sched/sched.h"
#include "barriers.h"
#include "smp.h"
#include "arm.h"

#ifdef CONFIG_SMP

/****************************************************************************
 * Name: up_cpu_off
 *
 * Description:
 *   Stop and power down a CPU completely. This function performs a full
 *   power down sequence including hot-plug operations and core power down.
 *   The CPU will be completely powered off and can be restarted later
 *   with up_cpu_on().
 *
 * Input Parameters:
 *   cpu - The index of the CPU to stop
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU is currently running and can be safely powered down
 *   - Task migration for target CPU has been completed
 *
 ****************************************************************************/

int up_cpu_off(int cpu)
{
	int ret;

	svdbg("Disabling CPU%d\n", cpu);

	DEBUGASSERT(cpu > 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

#ifdef CONFIG_CPU_HOTPLUG
	/* Hot-plug the CPU */
	ret = smp_hotplug_cpu(cpu);
	if (ret != OK) {
		lldbg("Failed to hotplug CPU%d: %d\n", cpu, ret);
		return ret;
	}

	svdbg("Secondary core powerdown?... %d\n", cpu_get_state(cpu));

	/* Power down the core completely */
	cpu_core_powerdown(cpu);
#else
	/* If hot-plug is not enabled, we cannot safely stop the CPU */
	return -ENOSYS;
#endif

	return OK;
}

/****************************************************************************
 * Name: up_cpu_on
 *
 * Description:
 *   Enable and start a CPU that was previously powered down. This function
 *   performs the complete power on sequence by first powering on the
 *   CPU domain reinitializes the SCU for cache coherency, and boots the
 *   secondary core.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to enable
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU is currently powered off and can be safely started
 *   - System is in a state to safely bring additional CPU online
 *
 ****************************************************************************/

int up_cpu_on(int cpu)
{
	int ret = OK;

	svdbg("Enabling CPU%d\n", cpu);

	DEBUGASSERT(cpu > 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

#ifdef CONFIG_CPU_HOTPLUG
	/* Validate that the target CPU is in a state that can be started */
	if (cpu_get_state(cpu) != CPU_HALTED) {
		lldbg("CPU%d is not in halted state, current state: %d\n", cpu, cpu_get_state(cpu));
		return -EINVAL;
	}

	/* take the lock, as we are about to put cpu1 idle task back active */
	sched_lock();

	/* Power on the CPU core */
	ret = cpu_core_poweron(cpu);
	if (ret != OK) {
		lldbg("Failed to power on CPU%d: %d\n", cpu, ret);
		goto out_unlock;
	}

	/* Start the CPU */
	ret = vPortSecondaryStart(cpu);
	if (ret != OK) {
		lldbg("Failed to boot CPU%d: %d\n", cpu, ret);
		goto out_unlock;
	}
#else
	/* If hot-plug is not enabled, we cannot safely start the CPU */
	return -ENOSYS;
#endif

out_unlock:
	/* unlock the scheduler */
	sched_unlock();
	return ret;
}

#endif /* CONFIG_SMP */
