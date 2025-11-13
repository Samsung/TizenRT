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

#include "up_internal.h"
#include "cp15_cacheops.h"
#include "gic.h"
#include "sched/sched.h"
#include "barriers.h"
#include "smp.h"
#include "arm.h"


#ifdef CONFIG_CPU_HOTPLUG

/****************************************************************************
 * Name: up_cpu_off
 *
 * Description:
 *   Power down a CPU core by turning off its power domain. This function
 *   waits for the target CPU to enter OFF state via PSCI affinity info,
 *   then completely cuts power to the CPU domain.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to power down
 *
 * Returned Value:
 *   Zero (OK) on success, negated error no on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU has been hot-plugged out
 *   - All tasks have been migrated away from target CPU
 *
 ****************************************************************************/

int up_cpu_off(int cpu)
{
	int ret;

	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		lldbg("cpu number out of range\n");
		return -EINVAL;
	}

	lldbg("Starting core powerdown for CPU%d\n", cpu);

	ret = cpu_core_powerdown(cpu);
	if (ret < 0) {
		lldbg("Failed to powerdown CPU%d, errno: %d\n", cpu, ret);
		return ret;
	}

	lldbg("Secondary core CPU%d powerdown complete\n", cpu);
	return OK;
}

/****************************************************************************
 * Name: up_cpu_on
 *
 * Description:
 *   Power on and boot a CPU core that was previously powered down. This function
 *   performs the complete power-on sequence: enables the CPU power domain,
 *   reinitializes the SCU for cache coherency, and cold-boots the secondary
 *   core via PSCI to the specified entry point.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to power on
 *
 * Returned Value:
 *   Zero (OK) on success, negated error no on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU is currently powered off
 *   - System is ready to bring additional CPU online
 *
 ****************************************************************************/

int up_cpu_on(int cpu)
{
	int ret;

	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		lldbg("cpu number out of range\n");
		return -EINVAL;
	}

	/* Power on the CPU core */
	ret = cpu_core_poweron(cpu);
	if (ret != OK) {
		lldbg("Failed to power on CPU%d: errno = %d\n", cpu, ret);
		return ret;
	}

	lldbg("Booting secondary core... \n");

	/* cold-boot the core via PSCI */
	ret = vPortSecondaryBoot(cpu);
	if (ret != OK) {
		lldbg("Failed to boot CPU%d: %d\n", cpu, ret);
		return ret;
	}

	lldbg("Secondary core booted successfully\n");
	return OK;
}

#endif /* CONFIG_CPU_HOTPLUG */
