#ifndef __INCLUDE_TINYARA_CPU_DRIVER_H
#define __INCLUDE_TINYARA_CPU_DRIVER_H

#include <tinyara/cpu_state.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Register a CPU device node (e.g. /dev/cpu1) with its associated arch ops.
 *
 * Input:
 *   cpu  - logical CPU ID (only secondary CPU1 supported in current design)
 *   ops  - pointer to arch ops table (from board)
 *
 * Return:
 *   OK (0) on success, negative errno on failure
 */
int cpu_driver_register(int cpu, const struct cpu_arch_ops_s *ops);

#endif /* __INCLUDE_TINYARA_CPU_DRIVER_H */


