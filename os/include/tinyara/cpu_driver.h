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

#ifndef __INCLUDE_TINYARA_CPU_DRIVER_H
#define __INCLUDE_TINYARA_CPU_DRIVER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/cpu_state.h>
#include <tinyara/fs/ioctl.h>



/* IOCTLs for /dev/cpuX */
#define IOC_CPU_ENABLE              _IOC(_CPUSTATEBASE, 1)
#define IOC_CPU_DISABLE             _IOC(_CPUSTATEBASE, 2)
#define IOC_CPU_HOTPLUG             _IOC(_CPUSTATEBASE, 3)
#define IOC_CPU_GET_STATE           _IOC(_CPUSTATEBASE, 4)

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


