/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include "binfmt_arch_apis.h"

/****************************************************************************
* Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private functions
 ****************************************************************************/
#ifdef CONFIG_ARM_MPU
static inline void binfmt_set_mpu(struct binary_s *binp)
{
	uint32_t *regs = NULL;
	uint8_t nregion = 0;

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (binp->islibrary) {
		regs = binp->cmn_mpu_regs;
		nregion = mpu_get_nregion_info(MPU_REGION_COMMON_BIN);
	} else
#endif
	{
		regs = sched_self()->mpu_regs;
		nregion = mpu_get_nregion_info(MPU_REGION_APP_BIN);
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	/* Configure text section as RO and executable region */
	mpu_get_register_config_value(&regs[0], nregion - 3, (uintptr_t)binp->sections[BIN_TEXT], binp->sizes[BIN_TEXT], true, true);
	/* Configure ro section as RO and non-executable region */
	mpu_get_register_config_value(&regs[3], nregion - 2, (uintptr_t)binp->sections[BIN_RO], binp->sizes[BIN_RO], true, false);
	/* Complete RAM partition will be configured as RW region */
	mpu_get_register_config_value(&regs[6], nregion - 1, (uintptr_t)binp->sections[BIN_DATA], binp->ramsize, false, false);
#else
	/* Complete RAM partition will be configured as RW region */
	mpu_get_register_config_value(&regs[0], nregion - 1, (uintptr_t)binp->ramstart, binp->ramsize, false, true);
#endif

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (binp->islibrary) {
		/* Set MPU register values to real MPU h/w */
		for (int i = 0; i < MPU_REG_NUMBER * NUM_APP_REGIONS; i += MPU_REG_NUMBER) {
			up_mpu_set_register(&regs[i]);
		}
	}
#endif
}
#endif

/****************************************************************************
 * Public functions
 ****************************************************************************/

void *binfmt_arch_allocate_section(uint32_t size)
{
	void *mem = NULL;
#if defined(CONFIG_ARMV7M_MPU)
	mem = kmm_memalign_at(CONFIG_HEAP_INDEX_LOADED_APP, size, size);
#elif defined(CONFIG_ARMV8M_MPU)
	mem = kmm_memalign_at(CONFIG_HEAP_INDEX_LOADED_APP, MPU_ALIGNMENT_BYTES, size);
#elif defined(CONFIG_ARCH_USE_MMU)
	mem = kmm_memalign_at(CONFIG_HEAP_INDEX_LOADED_APP, MMU_ALIGNMENT_BYTES, size);
#else
	mem = kmm_malloc_at(CONFIG_HEAP_INDEX_LOADED_APP, size);
#endif

	return mem;
}

uint32_t binfmt_arch_align_mem(uint32_t val)
{
#if defined(CONFIG_ARMV7M_MPU) || defined(CONFIG_ARMV8M_MPU)
	return MPU_ALIGN_UP(val);
#elif defined(CONFIG_ARCH_USE_MMU)
	return MMU_ALIGN_UP(val);
#endif
}

void binfmt_arch_init_mem_protect(struct binary_s *binp)
{
#if defined(CONFIG_ARM_MPU)
	binfmt_set_mpu(binp);
#elif defined(CONFIG_ARCH_USE_MMU)
	binfmt_setup_app_pgtable(binp);
#endif
}

void binfmt_arch_deinit_mem_protect(struct binary_s *binp)
{
#if (defined(CONFIG_ARMV7M_MPU) || defined(CONFIG_ARMV8M_MPU))
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (binp->islibrary) {
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		for (int i = 0; i < MPU_REG_NUMBER * NUM_APP_REGIONS; i += MPU_REG_NUMBER) {
			up_mpu_disable_region(&binp->cmn_mpu_regs[i]);
		}
#else
		up_mpu_disable_region(&binp->cmn_mpu_regs[0]);
#endif
	}
#endif
#elif defined(CONFIG_ARCH_USE_MMU)
	mmu_clear_app_pgtbl(binp->binary_idx);
#endif
}
