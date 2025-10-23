#include <tinyara/config.h>
#include <tinyara/cpu_driver.h>
#include <tinyara/cpu_state.h>
#include <tinyara/spinlock.h>
#include <debug.h>
#include <errno.h>
#include <tinyara/barriers.h>



/* Global pointer to architecture-specific CPU operations for CPU1 */
const struct cpu_arch_ops_s *g_cpu1_arch_ops = NULL;


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

        ARM_DSB();

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
 4. cpu_wakeup();
 5. cpu_teardown();
*/

int cpu_disable(int cpu)
{
        const struct cpu_arch_ops_s *ops;

        /* Currently, only cpu1 is supported for hotplug/disable */
        if (cpu != 1) {
                return -EINVAL;
        }

	ops = g_cpu1_arch_ops;

	if (!ops) {
                return -ENODEV;
        }

        return ops->disable(cpu);
}

int cpu_enable(int cpu)
{
        const struct cpu_arch_ops_s *ops;

	/* Currently, only cpu1 is supported for hotplug/disable */
        if (cpu != 1) {
                return -ENOTSUP;
        }

        ops = g_cpu1_arch_ops;
        if (!ops) {
                return -ENODEV;
        }

        return ops->enable(cpu);
}

int cpu_hotplug(int cpu)
{
        const struct cpu_arch_ops_s *ops;

        if (cpu != 1) {
                return -EINVAL;
        }

        ops = g_cpu1_arch_ops;

        if (!ops) {
                return -ENODEV;
        }

        return ops->hotplug(cpu);
}

int cpu_wakeup(int cpu)
{
        const struct cpu_arch_ops_s *ops;

        if (cpu != 1) {
                return -EINVAL;
        }

        ops = g_cpu1_arch_ops;

        if (!ops) {
                return -ENODEV;
        }

        return ops->wakeup(cpu);
}

int cpu_teardown(int cpu)
{
        const struct cpu_arch_ops_s *ops;

        if (cpu != 1) {
                return -EINVAL;
        }

        ops = g_cpu1_arch_ops;

        if (!ops) {
                return -ENODEV;
        }

        return ops->teardown(cpu);
}


