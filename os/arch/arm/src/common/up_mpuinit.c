/****************************************************************************
 * arch/arm/src/common/up_mpuinit.c
 *
 *   Copyright (C) 2017 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_ARM_MPU
#include <tinyara/mpu.h>

#include <assert.h>

#include "mpu.h"
#include "up_mpuinit.h"
#include "cache.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
static struct mpu_usages_s g_mpu_region_usages;

#ifdef CONFIG_ARMV7M_MPU
#define MPU_MAX_NREGION		CONFIG_ARMV7M_MPU_NREGIONS
#elif defined(CONFIG_ARMV8M_MPU)
#define MPU_MAX_NREGION		CONFIG_ARMV8M_MPU_NREGIONS
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mpu_region_initialize
 *
 * Description:
 *   Initialize the MPU region numbers.
 *
 ****************************************************************************/

void mpu_region_initialize(struct mpu_usages_s *mpu)
{
	uint8_t offset = mpu->nregion_board_specific;

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	offset += NUM_APP_REGIONS;
	mpu->nregion_common_bin = offset;
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
	offset += NUM_APP_REGIONS;
	mpu->nregion_app_bin = offset;
#endif

#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
	offset += 1;
	mpu->nregion_stackovf = offset;
#endif
	mpu->max_nregion = offset;
}

/****************************************************************************
 * Name: mpu_get_nregion_info
 *
 * Description:
 *   Get the MPU region numbers.
 *
 ****************************************************************************/

uint8_t mpu_get_nregion_info(enum mpu_region_usages_e usages)
{
	uint8_t nregion;
	switch (usages) {
	case MPU_REGION_BOARD_SPECIFIC:
		nregion = g_mpu_region_usages.nregion_board_specific;
		break;
	case MPU_REGION_COMMON_BIN:
		nregion = g_mpu_region_usages.nregion_common_bin;
		break;
	case MPU_REGION_APP_BIN:
		nregion = g_mpu_region_usages.nregion_app_bin;
		break;
	case MPU_REGION_STACKOVF:
		nregion = g_mpu_region_usages.nregion_stackovf;
		break;
	case MPU_REGION_MAX:
		nregion = g_mpu_region_usages.max_nregion;
		break;
	default:
		nregion = 0;
		break;
	}

	return nregion;
}

/****************************************************************************
 * Name: mpu_set_nregion_board_specific
 *
 * Description:
 *   Initialize the number of MPU regions for board specific purpose
 *
 ****************************************************************************/

void mpu_set_nregion_board_specific(uint8_t num)
{
	/* Initialize number of mpu regions for board-specific purpose */
	g_mpu_region_usages.nregion_board_specific = num;
}

/****************************************************************************
 * Name: up_mpuinitialize
 *
 * Description:
 *   Configure the MPU to permit user-space access to only restricted
 *   resources.
 *
 ****************************************************************************/
void up_mpuinitialize(void)
{
	/* Initialize MPU register numbers information */
	mpu_region_initialize(&g_mpu_region_usages);

	/* Maximum MPU region number sanity check */
	DEBUGASSERT(g_mpu_region_usages.max_nregion <= MPU_MAX_NREGION);

	/* Show MPU information */
	mpu_showtype();

#ifdef CONFIG_ARMV7M_DCACHE
	/* Memory barrier */
	ARM_DMB();

#ifdef CONFIG_IMXFT_QSPI
	/* Make QSPI memory region strongly ordered */
	mpu_priv_stronglyordered(IMXRT_QSPIMEM_BASE, IMXRT_QSPIMEM_SIZE);
#endif
#endif

	/* Then enable the MPU */
	mpu_control(true, false, true);
}

#endif // CONFIG_ARM_MPU
