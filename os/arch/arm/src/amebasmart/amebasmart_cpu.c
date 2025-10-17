/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>

#include <tinyara/arch.h>
#include <tinyara/cpu_state.h>
#include <arch/irq.h>

#include "up_internal.h"
#include "sctlr.h"
#include "smp.h"
#include "scu.h"
#include "gic.h"
#include "mmu.h"
#include "amebasmart_cpu.h"
#include "amebasmart_boot.h"
#include "barriers.h"
#include "psci.h"

#include <tinyara/fs/fs.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>



#ifdef CONFIG_SMP

/****************************************************************************
 * Private Types
 ****************************************************************************/


/****************************************************************************
 * Private Data
 ****************************************************************************/


/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Symbols defined via the linker script */


/****************************************************************************
 * Public Functions
 ****************************************************************************/


/****************************************************************************
 * Name: cpu__disable
 *
 * Description:
 *   Disables a specified CPU by performing a complete power-down sequence.
 *   This function is called from CPU0 to power off a secondary CPU completely,
 *   cutting power to its domain.
 *
 * Input Parameters:
 *   cpu_id - The ID of the CPU to disable
 *
 * Returned Value:
 *   0 on success
 *
 * Assumptions:
 *   - Called from CPU0 (primary processor)
 *   - Target CPU is currently running and can be safely powered down
 *   - All tasks on target CPU have been migrated away
 *
 ****************************************************************************/

static int cpu__disable(int cpu_id)
{
	up_cpu_off(cpu_id);
	lldbg("CPU%d: disable\n", cpu_id);
	return 0;
}

/****************************************************************************
 * Name: cpu__enable
 *
 * Description:
 *   Enables a previously disabled CPU by bringing it back online through
 *   a hot-plug sequence. This function powers on the CPU domain and
 *   boots the secondary core.
 *
 * Input Parameters:
 *   cpu_id - The ID of the CPU to enable
 *
 * Returned Value:
 *   0 on success
 *
 * Assumptions:
 *   - Called from CPU0 (primary processor)
 *   - Target CPU is currently powered off
 *   - System is in a state to safely bring additional CPU online
 *
 ****************************************************************************/

static int cpu__enable(int cpu_id)
{
	up_cpu_on(cpu_id);
	lldbg("CPU%d: enable\n", cpu_id);
	return 0;
}

/****************************************************************************
 * Name: cpu__hotplug
 *
 * Description:
 *   Performs a hot-plug operation on a CPU without full power-down. This
 *   function temporarily takes a CPU offline for power management purposes
 *   by migrating tasks away and putting the CPU in a halt state. The CPU
 *   remains powered but inactive, allowing for faster resume compared to
 *   full power cycling.
 *
 * Input Parameters:
 *   cpu_id - The ID of the CPU to hot-plug
 *
 * Returned Value:
 *   0 on success
 *
 * Assumptions:
 *   - Called from CPU0 (primary processor)
 *   - Target CPU is currently running and can be safely halted
 *   - Tasks on target CPU can be migrated to other CPUs
 *
 ****************************************************************************/

static int cpu__hotplug(int cpu_id)
{
	smp_hotplug_cpu(cpu_id);
	lldbg("CPU%d: hotplug\n", cpu_id);
	return 0;
}


const struct cpu_arch_ops_s cpu_arch_ops = {
    .enable                  = cpu__enable,
    .disable                 = cpu__disable,
    .hotplug                 = cpu__hotplug,
};

/****************************************************************************
 * Name: up_cpuinitialize
 *
 * Description:
 *   Initializes the CPU architecture operations and returns the operation
 *   table for both CPU driver framework and CPU state management. This function
 *   is called during system startup to register the AmebaSmart-specific CPU
 *   management capabilities with the generic CPU driver subsystem and set up
 *   the CPU state management infrastructure.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Pointer to const struct cpu_arch_ops_s containing function pointers for
 *   CPU operations. Returns NULL on failure.
 *
 * Assumptions:
 *   - Called during system initialization
 *   - SMP subsystem is ready for CPU management
 *   - CPU state management system is ready to be initialized
 *
 ****************************************************************************/

const struct cpu_arch_ops_s * up_cpuinitialize(void)
{
    lldbg("[cpu hal] initialized\n");
    return &cpu_arch_ops;
}



#endif /* CONFIG_SMP */
