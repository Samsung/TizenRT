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
/****************************************************************************
 * arch/arm/src/armv8-m/up_mpu.c
 *
 *   Copyright (C) 2011, 2013 Gregory Nutt. All rights reserved.
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

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include <tinyara/mpu.h>

#include "mpu.h"
#include "up_internal.h"
#include "up_arch.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_ARMV8M_MPU_NREGIONS
#define CONFIG_ARMV8M_MPU_NREGIONS 8
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mpu_log2regionceil
 *
 * Description:
 *   Determine the smallest value of l2size (log base 2 size) such that the
 *   following is true:
 *
 *   size <= (1 << l2size)
 *
 ****************************************************************************/

uint8_t mpu_log2regionceil(uintptr_t base, size_t size)
{
	uint8_t l2size;

	/* The minimum permitted region size is 32 bytes (log2(32) = 5. */

	for (l2size = 5; l2size < 32; l2size++) {
		if ((base & ((1 << l2size) - 1)) + size <= (1 << l2size)) {
			break;
		}
	}
	return l2size;
}

/****************************************************************************
 * Name: mpu_log2regionfloor
 *
 * Description:
 *   Determine the largest value of l2size (log base 2 size) such that the
 *   following is true:
 *
 *   size >= (1 << l2size)
 *
 ****************************************************************************/

uint8_t mpu_log2regionfloor(uintptr_t base, size_t size)
{
	uint8_t l2size = mpu_log2regionceil(base, size);

	if (l2size > 4 && (base & ((1 << l2size) - 1)) + size < (1 << l2size)) {
		l2size--;
	}

	return l2size;
}

/****************************************************************************
 * Name: mpu_get_register_config_value
 *
 * Description:
 *   Configure the user application SRAM mpu settings into the tcb variables
 *
 ****************************************************************************/

void mpu_get_register_config_value(uint32_t *regs, uint32_t region, uintptr_t base, size_t size, uint8_t readonly, uint8_t execute)
{
	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */
	regs[0] = region & MPU_RNR_MASK;

	/* Select the region base address */
	regs[1] = (base & MPU_RBAR_ADDR_MASK);
#ifdef CONFIG_APPS_RAM_REGION_SHAREABLE
	regs[1] |= MPU_RBAR_SH_OUT;
#else
	regs[1] |= MPU_RBAR_SH_NON;
#endif
	if (readonly) {
		regs[1] |= MPU_RBAR_AP_RORO;
	} else {
		regs[1] |= MPU_RBAR_AP_RWRW;
	}

	if (!execute) {
		regs[1] |= MPU_RBAR_XN;
	}

	regs[2] = (base + size - 1) & MPU_RLAR_LIMIT_MASK;
#ifdef CONFIG_ARMV8M_DCACHE
	regs[2] |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WB_T_RWA);
#else
	regs[2] |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_NC);
#endif
	regs[2] |= MPU_RLAR_ENABLE;
}

/****************************************************************************
 * Name: up_mpu_set_register
 *
 * Description:
 *   Set MPU register values to real mpu h/w
 *
 ****************************************************************************/
void up_mpu_set_register(uint32_t *mpu_regs)
{
	/* We update MPU registers only if there is non zero value of
	 * base address (This ensures valid MPU settings)
	 */
	if (mpu_regs[MPU_REG_RBAR]) {
		putreg32(mpu_regs[MPU_REG_RNR], MPU_RNR);
		putreg32(mpu_regs[MPU_REG_RBAR], MPU_RBAR);
		putreg32(mpu_regs[MPU_REG_RASR], MPU_RLAR);
	}
}

/****************************************************************************
 * Name: up_mpu_check_active
 *
 * Description:
 *   Checks if mpu registers passed as argument are the ones set in
 *   mpu h/w
 *
 ****************************************************************************/
bool up_mpu_check_active(uint32_t *mpu_regs)
{
	/* Set MPU_RNR register before getting corresponding
	 * MPU_RBAR and MPU_RLAR values
	 */
	putreg32(mpu_regs[MPU_REG_RNR], MPU_RNR);

	return (getreg32(MPU_RBAR) == mpu_regs[MPU_REG_RBAR] && getreg32(MPU_RLAR) == mpu_regs[MPU_REG_RASR]);
}

/****************************************************************************
 * Name: up_mpu_disable_region
 *
 * Description:
 *   Update tcb's mpu register values to disable a mpu region
 *   and set these new register values to mpu h/w as well.
 *
 ****************************************************************************/
void up_mpu_disable_region(uint32_t *mpu_regs)
{
	/* Disable region's Enable bit in mpu register settings */
	mpu_regs[MPU_REG_RASR] &= ~MPU_RLAR_ENABLE;

	/* Set updated mpu register values to mpu h/w */
	up_mpu_set_register(mpu_regs);
}
