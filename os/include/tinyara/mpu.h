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

/* Structure for MPU region numbers */

struct mpu_usages_s {
	uint8_t nregion_board_specific;
	uint8_t nregion_common_bin;
	uint8_t nregion_app_bin;
	uint8_t nregion_stackovf;
	uint8_t max_nregion;
};

enum mpu_register_type_e {
	MPU_REG_RNR,
	MPU_REG_RBAR,
	MPU_REG_RASR,
	MPU_REG_NUMBER
};

/* Enum to to get MPU region number info for MPU usages */
enum mpu_region_usages_e {
	MPU_REGION_BOARD_SPECIFIC,
	MPU_REGION_COMMON_BIN,
	MPU_REGION_APP_BIN,
	MPU_REGION_STACKOVF,
	MPU_REGION_MAX
};

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
/* Separate three MPU regions (text, ro and rw) to optimize reloading time */
#define MPU_NUM_REGIONS     3
#else
/* Just a MPU region for all of section data */
#define MPU_NUM_REGIONS     1
#endif

#ifdef CONFIG_ARMV8M_MPU
#define MPU_ALIGNMENT_BYTES    32
#define MPU_ALIGN_UP(a)                (((a) + MPU_ALIGNMENT_BYTES - 1) & ~(MPU_ALIGNMENT_BYTES - 1))
#endif

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

void mpu_get_register_config_value(uint32_t *regs, uint32_t region, uintptr_t base, size_t size, uint8_t readonly, uint8_t execute);

void up_mpu_set_register(uint32_t *mpu_regs);

bool up_mpu_check_active(uint32_t *mpu_regs);

void up_mpu_disable_region(uint32_t *mpu_regs);

void mpu_region_initialize(struct mpu_usages_s *mpu);

void mpu_set_nregion_board_specific(uint8_t num);

uint8_t mpu_get_nregion_info(enum mpu_region_usages_e usages);

#endif
