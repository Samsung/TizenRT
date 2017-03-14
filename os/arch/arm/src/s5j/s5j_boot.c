/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_boot.c
 *
 *   Copyright (C) 2009-2010, 2014-2015 Gregory Nutt. All rights reserved.
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
#include <debug.h>

#include <tinyara/init.h>

#include "up_arch.h"
#include "up_internal.h"

#include <chip.h>
#include <arch/chip/chip_types.h>
#include "arm.h"
#ifdef CONFIG_ARMV7M_MPU
#include "mpu.h"
#endif
#include "cache.h"
#include "fpu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
extern uint32_t _vector_start;
extern uint32_t _vector_end;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void up_copyvectorblock(void)
{
	uint32_t *src = (uint32_t *)&_vector_start;
	uint32_t *end = (uint32_t *)&_vector_end;
	uint32_t *dest = (uint32_t *)VECTOR_BASE;

	while (src < end) {
		*dest++ = *src++;
	}
}

#ifdef CONFIG_ARMV7M_MPU
int s5j_mpu_initialize(void)
{
	uint32_t base;
	uint32_t size;

	/* flash region
	 * region 0 : 0x0400_0000 ++ 4MB  BL1 + TinyAra OS + SSS F/W
	 */
	mpu_priv_flash(0x0, 0x04000000);

	/* S5JT200 memory address in below
	 * 0x0202_0000 ~ 0x0215_FFFF  ~1280KB
	 * 0x0203_0000 ~ 0x0231_FFFF  ~128KB
	 * 0x0400_0000 ~ 0x04FF_FFFF  ~16MB(sidk s5jt200 has 8MB)
	 */

	/* access permission for available area */
	/* Region 1, for ISRAM(0x0200_0000++2048KB, RW-WBWA */
	mpu_user_intsram_wb(0x02000000, 0x200000);

	/* Region 2,3, for wifi driver non-$(0x020E_0000++512KB(+128KB), RW-NCNB */
	mpu_priv_noncache(0x020E0000, 0x20000);
	mpu_priv_noncache(0x02100000, 0x80000);

	/*
	 * Region 4 for RO in Flash : 0x0400_0000 ++ 4MB, RO-WT
	 * BL1 + TinyAra OS + SSS F/W, wifi F/W, CM0 F/W
	 */
	base = CONFIG_S5J_FLASH_BASE;
	size = CONFIG_S5J_BOOTLOADER_REGION_SIZE;
	mpu_priv_flash(base, size);

	/* Region 5, for wifi dedicated area, RW-WBWA, 0x043C_0000 -- 8KB */
	mpu_user_intsram_wb(CONFIG_NVRAM_WIFI_START, 8 * 1024);

#ifdef CONFIG_FS_SMARTFS
	/*
	 * Region 6, FILE SYSTEM : 0x0440_0000 ++ 4MB - 256KB
	 * filesystem - smartfs
	 */
	base = CONFIG_S5J_FLASH_BASE + CONFIG_S5J_BOOTLOADER_REGION_SIZE;
	size = CONFIG_S5J_FLASH_SIZE - CONFIG_S5J_BOOTLOADER_REGION_SIZE;
	mpu_user_intsram_wb(base, size);
#endif

	/*
	 * Region 7, SSS F/W region2 : 0x047C_0000 ++ 256KB
	 * SSS key area RW, priority higher than 0x0400_0000, 4MB attribute(WBWA, RO)
	 */
	base = CONFIG_S5J_FLASH_BASE + CONFIG_S5J_FLASH_SIZE;
	size = 256 * 1024;
	mpu_user_intsram_wb(base, size);

	/* region 8, for Sflash Mirror(0x6000_0000, RO) */
	mpu_priv_flash(0x60000000, CONFIG_S5J_FLASH_SIZE);

	/* Region 9, for vecotr table(0x8000_0000, RW-STRONG-ORDER */
	mpu_priv_stronglyordered(0x80000000, 0x10000000);

	/* Region 10, for vecotr table(0xFFFF_0000++4KB, RO-WT */
	mpu_priv_flash(0xFFFF0000, 0x1000);

	mpu_control(true);

	return 0;
}
#endif

void arm_boot(void)
{
#ifdef CONFIG_S5J_DEBUG_BREAK
	__asm__ __volatile__("b  .");
#endif

	up_copyvectorblock();

#ifdef CONFIG_ARMV7R_MEMINIT
	/* Initialize the .bss and .data sections as well as RAM functions
	 * now after RAM has been initialized.
	 *
	 * NOTE that if SDRAM were supported, this call might have to be
	 * performed after returning from tms570_board_initialize()
	 */
	arm_data_initialize();
#endif

#ifdef CONFIG_ARMV7M_MPU
	s5j_mpu_initialize();
	arch_enable_icache();
	arch_enable_dcache();
#endif

	cal_init();

#ifdef USE_EARLYSERIALINIT
	up_earlyserialinit();
#endif
	os_start();

}
