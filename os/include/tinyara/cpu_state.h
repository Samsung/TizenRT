#ifndef __INCLUDE_TINYARA_CPU_STATE_H
#define __INCLUDE_TINYARA_CPU_STATE_H

#include <tinyara/fs/ioctl.h>


/* IOCTLs for /dev/cpuX */
#define IOC_CPU_ENABLE              _IOC(_CPUSTATEBASE, 1)
#define IOC_CPU_DISABLE             _IOC(_CPUSTATEBASE, 2)
#define IOC_CPU_HOTPLUG             _IOC(_CPUSTATEBASE, 3)
#define IOC_CPU_WAKEUP              _IOC(_CPUSTATEBASE, 4)
#define IOC_CPU_TEARDOWN            _IOC(_CPUSTATEBASE, 5)
#define IOC_CPU_GET_STATE           _IOC(_CPUSTATEBASE, 6)
#define IOC_CPU_SET_STATE           _IOC(_CPUSTATEBASE, 7)

typedef enum {
    CPU_RUNNING = 0, /* CPU is in task scheduer or boot from reset */
    CPU_HOTPLUG = 1, /* CPU is offline */
    CPU_WAKEUP = 2, /* CPU just wake from sleep but not in task scheduler */
    CPU_HALTED = 3, /* CPU has lost power and equivalent to cold boot when powered on */
    CPU_TEARDOWN = 4, /* CPU is preparing to go offfline */
} cpu_state_t;

/****************************************************************************
 * CPU Arch Ops
 ****************************************************************************/

struct cpu_arch_ops_s {
    int (*enable)(int cpu);
    int (*disable)(int cpu);
    int (*hotplug)(int cpu);
    int (*wakeup)(int cpu);
    int (*teardown)(int cpu);

};


const struct cpu_arch_ops_s * up_cpuinitialize(void);


/* Global pointer to architecture-specific CPU operations for CPU1.
 * It Should be initialized by board-specific code
 */
extern const struct cpu_arch_ops_s *g_cpu1_arch_ops;


/****************************************************************************
 * Exported Function Prototypes for Kernel CPU Control
 ****************************************************************************/
int cpu_disable(int cpu);
int cpu_enable(int cpu);
int cpu_hotplug(int cpu);
int cpu_wakeup(int cpu);
int cpu_teardown(int cpu);
cpu_state_t cpu_get_state(int cpu);
int cpu_set_state(int cpu, cpu_state_t state);

#endif /* __INCLUDE_TINYARA_CPU_STATE_H */
