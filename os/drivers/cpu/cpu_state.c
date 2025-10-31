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
#include <tinyara/cpu_driver.h>
#include <tinyara/cpu_state.h>
#include <tinyara/spinlock.h>
#include <debug.h>
#include <errno.h>


/* Global pointer to architecture-specific CPU operations for CPU1 */
const struct cpu_arch_ops_s *g_cpu_arch_ops = NULL;

#ifdef CONFIG_CPU_HOTPLUG
/* cpu hotplug flag for each core */
static volatile cpu_state_t g_cpuhp_flag[CONFIG_SMP_NCPUS];

int cpu_set_state(int cpu, cpu_state_t NewStatus)
{
	irqstate_t flags;

	if (cpu < 0 || cpu >= CONFIG_SMP_NCPUS) {
                return -EINVAL;
        }

	flags = enter_critical_section();
        g_cpuhp_flag[cpu] = NewStatus;
	leave_critical_section(flags);

	return OK;
}

cpu_state_t cpu_get_state(int cpu)
{
	cpu_state_t ret;
        irqstate_t flags;

	if (cpu < 0 || cpu >= CONFIG_SMP_NCPUS) {
                return -EINVAL;
        }

	flags = enter_critical_section();
        ret = g_cpuhp_flag[cpu];
        leave_critical_section(flags);

	return ret;
}
#endif


/*
 Export functions for kernel PM modules
 1. cpu_enable();
 2. cpu_disable();
 3. cpu_hotplug();
*/

int cpu_disable(int cpu)
{
        const struct cpu_arch_ops_s *ops;

        /* Currently, only cpu1 is supported for hotplug/disable */
        if (cpu < 1 || cpu >= CONFIG_SMP_NCPUS) {
                return -EINVAL;
        }

	ops = g_cpu_arch_ops;

	if (!ops) {
                return -ENODEV;
        }

        return ops->disable(cpu);
}

int cpu_enable(int cpu)
{
        const struct cpu_arch_ops_s *ops;

        /* Currently, only cpu1 is supported for hotplug/disable */
        if (cpu < 1 || cpu >= CONFIG_SMP_NCPUS) {
                return -EINVAL;
        }

        ops = g_cpu_arch_ops;
        if (!ops) {
                return -ENODEV;
        }

        return ops->enable(cpu);
}

int cpu_hotplug(int cpu)
{
        const struct cpu_arch_ops_s *ops;

        /* Currently, only cpu1 is supported for hotplug/disable */
        if (cpu < 1 || cpu >= CONFIG_SMP_NCPUS) {
                return -EINVAL;
        }

        ops = g_cpu_arch_ops;

        if (!ops) {
                return -ENODEV;
        }

        return ops->hotplug(cpu);
}

