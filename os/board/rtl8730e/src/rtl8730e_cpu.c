/**************************************************************
	board_cpu_initialize()
	    +- ops = cpu_arch_initialize()
	    +- cpu_driver_register(1, ops)
		   +- alloc cpu_dev_s {cpu=1, ops=ops}
		   +- register_driver("/dev/cpu1")
***************************************************************/

#include <tinyara/config.h>
#include <tinyara/cpu_driver.h>
#include <tinyara/cpu_state.h>
#include <debug.h>
#include <errno.h>

#include "board_cpu.h"

int board_cpu_initialize(void)
{
    int ret;

    const struct cpu_arch_ops_s *ops = up_cpuinitialize();
    if (!ops) {
        lldbg("Arch ops not available\n");
        return -ENODEV;
    }

    /* Initialize the global pointer to architecture-specific CPU operations */
    g_cpu1_arch_ops = ops;

#ifdef CONFIG_CPU_DRIVER
    /* Register only secondary CPU (cpu1) with its ops */
    ret = cpu_driver_register(1, ops);
    if (ret < 0) {
        lldbg("Failed to register cpu1: %d\n", ret);
        return ret;
    }

    lldbg("CPU lifecycle ready for cpu1\n");
#endif

    return ops;
}
