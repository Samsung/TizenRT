/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __ARCH_ARM_INCLUDE_CHIP_MPU_H
#define __ARCH_ARM_INCLUDE_CHIP_MPU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/irq.h>

#ifndef __ASSEMBLY__
#include <stdint.h>
#include <arch/arch.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*
 * Region definitions for our platform
 */
#define MPU_REG_ENTIRE_MAP     MPU_REG0
#define MPU_REG_USER_DATA      MPU_REG1
#define MPU_REG_USER_HEAP      MPU_REG2
#define MPU_REG_USER_CONFIG0   MPU_REG3
#define MPU_REG_TASK_STACK     MPU_REG4
#define MPU_REG_KERN_DATA      MPU_REG5
#define MPU_REG_KERN_REG1      MPU_REG6
#define MPU_REG_KERN_REG0      MPU_REG7
#if (CONFIG_ARMV7M_MPU_NREGIONS == 12)
#define MPU_REG_KERN_PERI      MPU_REG8
#define MPU_REG_KERN_CODE      MPU_REG9
#ifdef CONFIG_MPU_STACKGUARD
#define MPU_REG_STK_GUARD      MPU_REG10
#endif
#define MPU_REG_KERN_VEC       MPU_REG11
#endif

#ifndef __ASSEMBLY__
typedef void (*fptr)(uintptr_t base, size_t size);

struct mpu_region_info
{
	fptr	call;
	unsigned int	base;
	unsigned int	size;
	unsigned int	rgno;
};

extern const struct mpu_region_info regions_info[];
#endif

#endif /* __ARCH_ARM_INCLUDE_CHIP_MPU_H */
