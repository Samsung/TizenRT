/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

///@file tinyara/mpu.h

#ifndef __INCLUDE_TINYARA_MPU_H
#define __INCLUDE_TINYARA_MPU_H

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>

/********************************************************************************
 * Pre-processor Definitions
 ********************************************************************************/

enum {
	REG_RNR,
	REG_RBAR,
	REG_RASR,
	REG_MAX
};

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
#define MPU_NUM_REGIONS		3
#else
#define MPU_NUM_REGIONS		1
#endif

/* Enum for MPU region numbers */

enum MPU_REG_NUM {
#ifdef CONFIG_APP_BINARY_SEPARATION
#ifdef CONFIG_SUPPORT_COMMON_BINARY
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	MPU_REG_NUM_COM_LIB_TXT,	/* Common Binary */
	MPU_REG_NUM_COM_LIB_RO,
	MPU_REG_NUM_COM_LIB_DATA,
#else
	MPU_REG_NUM_COM_LIB,
#endif
#endif
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	MPU_REG_NUM_APP_TXT,		/* Apps */
	MPU_REG_NUM_APP_RO,
	MPU_REG_NUM_APP_DATA,
#else
	MPU_REG_NUM_APP,
#endif
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
	MPU_REG_NUM_STK,		/* Stack */
#endif
#else
	MPU_REG_NUM_UFLASH,
	MPU_REG_NUM_URAM,
#endif
	MPU_REG_NUM_MAX
};

#ifdef CONFIG_ARMV7M_MPU
#define CUR_MPU_REG_NUM CONFIG_ARMV7M_MPU_NREGIONS
#elif defined(CONFIG_ARMV8M_MPU)
#define CUR_MPU_REG_NUM CONFIG_ARMV8M_MPU_NREGIONS
#endif

#if defined(CONFIG_ARMV7M_MPU) || defined(CONFIG_ARMV8M_MPU)
#if (MPU_REG_NUM_MAX >= CUR_MPU_REG_NUM)
#error "MPU region numbers exceeded !!"
#endif
#endif


/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

void mpu_get_register_value(uint32_t *regs, uint32_t region, uintptr_t base, size_t size, uint8_t readonly, uint8_t execute);

#endif
