/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#include <tinyara/fs/ioctl.h>

/* IOCTLs for /dev/cpuX */
#define IOC_CPU_ENABLE              _IOC(_CPUSTATEBASE, 1)
#define IOC_CPU_DISABLE             _IOC(_CPUSTATEBASE, 2)
#define IOC_CPU_GET_STATE           _IOC(_CPUSTATEBASE, 3)

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef enum {
	CPU_STATE_RUNNING = 0,
	CPU_STATE_OFFLINE = 1	// OFFLINE = HOTPLUG
} cpu_drv_state_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: cpu_driver_init
 *
 * Description:
 *   Initialize the CPU driver by registering device nodes for all secondary
 *   CPUs. This function creates /dev/cpuX device files for each secondary CPU.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK (0) on success, negative errno on failure
 *
 ****************************************************************************/

void cpu_driver_init(void);

#endif /* __INCLUDE_TINYARA_CPU_DRIVER_H */
