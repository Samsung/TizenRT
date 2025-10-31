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

#ifndef __INCLUDE_TINYARA_CPU_STATE_H
#define __INCLUDE_TINYARA_CPU_STATE_H


typedef enum {
    CPU_RUNNING = 0, /* CPU is in task scheduer or boot from reset */
    CPU_HOTPLUG = 1, /* CPU is offline */
    CPU_WAKE_FROM_SLEEP = 2, /* CPU just wake from sleep but not in task scheduler */
    CPU_HALTED = 3, /* CPU has lost power and equivalent to cold boot when powered on */
    CPU_SCHED_OFF = 4, /* CPU is powered on but not in task scheduler */
} cpu_state_t;

/****************************************************************************
 * CPU Arch Ops
 ****************************************************************************/

struct cpu_arch_ops_s {
    int (*enable)(int cpu); /* To bring CPU online from HALTED state */
    int (*disable)(int cpu); /* To Poweredown the CPU completely(HALT state)*/
    int (*hotplug)(int cpu); /* To make the CPU offline (HOTPLUG state)*/
};


/* Global pointer to architecture-specific CPU operations.
 * It Should be initialized by board-specific code
 */
extern const struct cpu_arch_ops_s *g_cpu_arch_ops;


/****************************************************************************
 * Exported Function Prototypes for Kernel CPU Control
 ****************************************************************************/
int cpu_disable(int cpu);
int cpu_enable(int cpu);
int cpu_hotplug(int cpu);
cpu_state_t cpu_get_state(int cpu);
int cpu_set_state(int cpu, cpu_state_t state);

#endif /* __INCLUDE_TINYARA_CPU_STATE_H */
