/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#ifndef __ARCH_ARM_SRC_IMXRT_IMXRT_MPUREG_H
#define __ARCH_ARM_SRC_IMXRT_IMXRT_MPUREG_H

/****************************************************************************
 * Included FilesG
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/irq.h>

#ifndef __ASSEMBLY__
#include <stdint.h>
#include <arch/arch.h>
#endif

#if defined(CONFIG_ARMV7M_MPU) && defined(CONFIG_BUILD_PROTECTED)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*
 * Region definitions for our platform
 * To provide MPU based memory protection in protected build, below four
 * definations should be defined. But they can be assigned any region
 * depending on the availability of the mpu regions in the specific chip. 
 *
 * For application memory protection we will have single mpu region with
 * app's memory range, during context switch the corresponding app's
 * memory range is set.
 */
#define MPU_REG_KERN_CODE	MPU_REG0
#define MPU_REG_KERN_DATA	MPU_REG1
#define MPU_REG_USER_CODE	MPU_REG2
#define MPU_REG_USER_DATA	MPU_REG3

#ifndef __ASSEMBLY__
typedef void (*fptr)(uint8_t region, uintptr_t base, size_t size);
struct mpu_region_info {
	fptr	call;
	unsigned int	base;
	unsigned int	size;
	unsigned int	rgno;
};

extern const struct mpu_region_info regions_info[];
#endif
#endif
#endif							/* __ARCH_ARM_SRC_IMXRT_IMXRT_MPUREG_H */
