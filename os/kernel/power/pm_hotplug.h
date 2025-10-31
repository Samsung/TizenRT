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

#ifndef CPU_HOTPLUG_MANAGER_H
#define CPU_HOTPLUG_MANAGER_H
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/cpu_state.h>
#include <sys/types.h>
#include <stdbool.h>

#ifdef CONFIG_CPU_HOTPLUG_MANAGER
#define CPU_HOTPLUG_IN_PROGRESS     3  /* hotplug in progress */
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* CPU states for hotplug management */
/* Use the existing cpu_state_t enum from smp.h */
typedef cpu_state_t cpu_hotplug_state_t;

/* Define the missing CPU states that are used in the code */
#define CPU_ONLINE                  CPU_RUNNING
#define CPU_OFFLINE                 CPU_HOTPLUG

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: cpu_hotplug_manager_init
 *
 * Description:
 *   Initialize the CPU hotplug manager module.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/
int cpu_hotplug_manager_init(void);

/****************************************************************************
 * Name: perform_hotplug
 *
 * Description:
 *   Request to take a CPU offline.
 *
 * Input Parameters:
 *   cpu - The CPU index to take offline
 *
 * Returned Value:
 *   OK (0) on success, negative value on failure.
 *
 ****************************************************************************/
int perform_hotplug(int cpu);

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
int perform_hotplug_online(int cpu);

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
bool is_hotplug_available(int cpu);

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
cpu_state_t cpu_hotplug_get_state(int cpu);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_CPU_HOTPLUG_MANAGER */
#endif /* __INCLUDE_TINYARA_CPU_HOTPLUG_MANAGER_H */