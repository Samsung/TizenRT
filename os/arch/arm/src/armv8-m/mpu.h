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
/************************************************************************************
 * os/arch/arm/src/armv8-m/mpu.h
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_COMMON_CORTEXM_MPU_H
#define __ARCH_ARM_SRC_COMMON_CORTEXM_MPU_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_ARMV8M_MPU
#ifndef __ASSEMBLY__
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <debug.h>
#ifdef CONFIG_BUILD_PROTECTED
#include "mpu-reg.h"
#endif
#include "up_arch.h"
#endif
#include <sched.h>
/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* MPU Register Addresses */

#define MPU_TYPE                0xe000ed90	/* MPU Type Register */
#define MPU_CTRL                0xe000ed94	/* MPU Control Register */
#define MPU_RNR                 0xe000ed98	/* MPU Region Number Register */
#define MPU_RBAR                0xe000ed9c	/* MPU Region Base Address Register */
#define MPU_RLAR                0xe000eda0	/* MPU Region Attribute and Size Register */

#define MPU_RBAR_A1             0xe000eda4	/* MPU alias registers */
#define MPU_RLAR_A1             0xe000eda8
#define MPU_RBAR_A2             0xe000edac
#define MPU_RLAR_A2             0xe000edb0
#define MPU_RBAR_A3             0xe000edb4
#define MPU_RLAR_A3             0xe000edb8

#define MPU_MAIR0		0xe000edc0	/* MPU attribute indirection registers */
#define MPU_MAIR1		0xe000edc4


/* MPU Type Register Bit Definitions */

#define MPU_TYPE_SEPARATE       (1 << 0)	/* Bit 0: 0:unified or 1:separate memory maps */
#define MPU_TYPE_DREGION_SHIFT  (8)		/* Bits 8-15: Number MPU data regions */
#define MPU_TYPE_DREGION_MASK   (0xff << MPU_TYPE_DREGION_SHIFT)
#define MPU_TYPE_IREGION_SHIFT  (16)		/* Bits 16-23: Number MPU instruction regions */
#define MPU_TYPE_IREGION_MASK   (0xff << MPU_TYPE_IREGION_SHIFT)

/* MPU Control Register Bit Definitions */

#define MPU_CTRL_ENABLE         (1 << 0)	/* Bit 0: Enable the MPU */
#define MPU_CTRL_HFNMIENA       (1 << 1)	/* Bit 1: Enable MPU during hard fault, NMI, and FAULTMAS */
#define MPU_CTRL_PRIVDEFENA     (1 << 2)	/* Bit 2: Enable privileged access to default memory map */

/* MPU Region Number Register Bit Definitions */

#if defined(CONFIG_ARMV8M_MPU_NREGIONS)
#if CONFIG_ARMV8M_MPU_NREGIONS <= 8
#define MPU_RNR_MASK            (0x00000007)
#elif CONFIG_ARMV8M_MPU_NREGIONS <= 16
#define MPU_RNR_MASK            (0x0000000f)
#elif CONFIG_ARMV8M_MPU_NREGIONS <= 32
#define MPU_RNR_MASK            (0x0000001f)
#else
#error "FIXME: Unsupported number of MPU regions"
#endif
#endif

/* MPU Region Base Address Register Bit Definitions */

#define MPU_RBAR_XN		(1 << 0)			/* Bits 0: MPU region */
#define MPU_RBAR_AP_SHIFT	(1)				/* Bits 1-2 */
#define MPU_RBAR_AP_MASK	(3 << MPU_RBAR_AP_SHIFT)
#define MPU_RBAR_SH_SHIFT	(3)
#define MPU_RBAR_SH_MASK	(3 << MPU_RBAR_SH_SHIFT)
#define MPU_RBAR_ADDR_MASK      0xffffffe0			/* Bits N-31:  Region base addrese */

#define MPU_RBAR_SH_NON		(0 << MPU_RBAR_SH_SHIFT)
#define MPU_RBAR_SH_OUT		(1 << MPU_RBAR_SH_SHIFT)
#define MPU_RBAR_SH_IN		(2 << MPU_RBAR_SH_SHIFT)

#define MPU_RBAR_AP_RWNO	(0 << MPU_RBAR_AP_SHIFT)	/* P:RW   U:None */
#define MPU_RBAR_AP_RWRW	(1 << MPU_RBAR_AP_SHIFT)	/* P:RW   U:RW   */
#define MPU_RBAR_AP_RONO	(2 << MPU_RBAR_AP_SHIFT)	/* P:RO   U:None */
#define MPU_RBAR_AP_RORO	(3 << MPU_RBAR_AP_SHIFT)	/* P:RO   U:RO   */

/* MPU Region Limit Address Register Bit Definitions */

#define MPU_RLAR_ENABLE         (1 << 0)			/* Bit 0: Region enable */
#define MPU_RLAR_ATTR_SHIFT	(1)
#define MPU_RLAR_ATTR_MASK	(7 << MPU_RLAR_ATTR_SHIFT)
#define MPU_RLAR_LIMIT_MASK	0xffffffe0

/* MPU Memory Attribute Index Register Definitions */

#define MPU_MAIR_SHIFT			(1)
#define MPU_MAIR_IDX(idx)		((idx & 7) << MPU_MAIR_SHIFT)

/* define memory attribute of Normal memory with  write-through transient,  write allocation */
#define NORMAL_WT_T_WA			(0x01)

/* define memory attribute of Normal memory with  write-through transient,  read allocation */
#define NORMAL_WT_T_RA			(0x02)

/* define memory attribute of Normal memory with  write-through transient,  read & write allocation */
#define NORMAL_WT_T_RWA			(0x03)

/* define memory attribute of Normal memory with  non-cacheable */
#define NORMAL_NC			(0x04)

/* define memory attribute of Normal memory with  write-back transient,  write allocation */
#define NORMAL_WB_T_WA			(0x05)

/* define memory attribute of Normal memory with  write-back transient,  read allocation */
#define NORMAL_WB_T_RA			(0x06)

/* define memory attribute of Normal memory with  write-back transient,  read and write allocation */
#define NORMAL_WB_T_RWA			(0x07)

/* define memory attribute of Normal memory with  write-through non-transient, no  allocation */
#define NORMAL_WT_NT			(0x08)

/* define memory attribute of Normal memory with  write-through non-transient,  write allocation */
#define NORMAL_WT_NT_WA			(0x09)

/* define memory attribute of Normal memory with  write-through non-transient,  read allocation */
#define NORMAL_WT_NT_RA			(0x0A)

/* define memory attribute of Normal memory with  write-through non-transient,  read and write allocation */
#define NORMAL_WT_NT_RWA		(0x0B)

/* define memory attribute of Normal memory with  write-back non-transient, no  allocation */
#define NORMAL_WB_NT			(0x0C)

/* define memory attribute of Normal memory with  write-back non-transient,  write allocation */
#define NORMAL_WB_NT_WA			(0x0D)

/* define memory attribute of Normal memory with  write-back non-transient,  read allocation */
#define NORMAL_WB_NT_RA			(0x0E)

/* define memory attribute of Normal memory with  write-back non-transient,  read and write allocation */
#define NORMAL_WB_NT_RWA		(0x0F)

/* define memory attribute of Device memory with non-gathering, non-reording, non-Early Write Acknowledge */
#define DEVICE_NG_NR_NE			((0<<4)|(0<<2))

/* define memory attribute of Device memory with non-gathering, non-reording, Early Write Acknowledge */
#define DEVICE_NG_NR_E			((0<<4)|(1<<2))

/* define memory attribute of Device memory with non-gathering, reording, Early Write Acknowledge */
#define DEVICE_NG_R_E			((0<<4)|(2<<2))

/* define memory attribute of Device memory with gathering, reording, Early Write Acknowledge */
#define DEVICE_G_R_E			((0<<4)|(3<<2))

#define MPU_MEM_ATTR0                   ((NORMAL_NC << 4) | NORMAL_NC)
#define MPU_MEM_ATTR1                   ((NORMAL_WT_T_RA << 4)  | NORMAL_WT_T_RA)
#define MPU_MEM_ATTR2                   ((NORMAL_WB_T_RWA << 4)  | NORMAL_WB_T_RWA)
#define MPU_MEM_ATTR3                   (DEVICE_NG_NR_NE)
#define MPU_MEM_ATTR4                   (DEVICE_NG_NR_NE)
#define MPU_MEM_ATTR5                   (DEVICE_NG_NR_NE)
#define MPU_MEM_ATTR6                   (DEVICE_NG_NR_NE)
#define MPU_MEM_ATTR7                   (DEVICE_NG_NR_NE)

#define MPU_MEM_ATTR_IDX_NC		0
#define MPU_MEM_ATTR_IDX_WT_T_RA	1
#define MPU_MEM_ATTR_IDX_WB_T_RWA	2
#define MPU_MEM_ATTR_IDX_DEVICE		3


/************************************************************************************
 * Global Function Prototypes
 ************************************************************************************/

#ifndef __ASSEMBLY__
#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: mpu_log2regionceil
 *
 * Description:
 *   Determine the smallest value of l2size (log base 2 size) such that the
 *   following is true:
 *
 *   (base & ((1 << l2size) - 1)) + size <= (1 << l2size)
 *
 ****************************************************************************/

uint8_t mpu_log2regionceil(uintptr_t base, size_t size);

/****************************************************************************
 * Name: mpu_log2regionfloor
 *
 * Description:
 *   Determine the largest value of l2size (log base 2 size) such that the
 *   following is true:
 *
 *   (base & ((1 << l2size) - 1)) + size >= (1 << l2size)
 *
 ****************************************************************************/

uint8_t mpu_log2regionfloor(uintptr_t base, size_t size);

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

/****************************************************************************
 * Name: mpu_showtype
 *
 * Description:
 *   Show the characteristics of the MPU
 *
 ****************************************************************************/

static inline void mpu_showtype(void)
{
#if defined(CONFIG_DEBUG) && defined(CONFIG_DEBUG_ERROR)
	uint32_t regval = getreg32(MPU_TYPE);
	lldbg("%s MPU Regions: data=%d instr=%d\n",
		(regval & MPU_TYPE_SEPARATE) != 0 ? "Separate" : "Unified",
		(regval & MPU_TYPE_DREGION_MASK) >> MPU_TYPE_DREGION_SHIFT,
		(regval & MPU_TYPE_IREGION_MASK) >> MPU_TYPE_IREGION_SHIFT);
#endif
}

/****************************************************************************
 * Name: mpu_show_regioninfo
 *
 * Description:
 *   Show the current mpu region setting
 *
 ****************************************************************************/

static inline void mpu_show_regioninfo(void)
{
#ifdef CONFIG_DEBUG
	int idx, temp;
	uint32_t rbar, rlar;

	/* save the current region before printing the information */
	temp = getreg32(MPU_RNR);

	lldbg("*****************************************************************************\n");
	lldbg("*REGION_NO.\tBASE_ADDRESS\t    SIZE\t   STATUS\t ACCESS\t   \tNO_EXECUTE*\n");
	lldbg("*****************************************************************************\n");
	for (idx = 0; idx < 8; idx++) {
		putreg32(idx, MPU_RNR);
		rbar = getreg32(MPU_RBAR);
		rlar = getreg32(MPU_RLAR);
		lldbg("%8d\t\t%8X\t%8X\t%8X\t%8X\t%8X\n",
			idx,
			(rbar & MPU_RBAR_ADDR_MASK),
			(((rlar & MPU_RLAR_LIMIT_MASK) - (rbar & MPU_RBAR_ADDR_MASK)) | 0x1f) + 1,
			(rlar & MPU_RLAR_ENABLE) ? 1 : 0,
			((rbar & MPU_RBAR_AP_MASK) >> MPU_RBAR_AP_SHIFT),
			(rbar & MPU_RBAR_XN));
	}
	lldbg("*****************************************************************************\n");
	/* restore the previous region */
	putreg32(temp, MPU_RNR);
#endif
}

/****************************************************************************
 * Name: mpu_control
 *
 * Description:
 *   Configure and enable (or disable) the MPU
 *
 ****************************************************************************/

static inline void mpu_control(bool enable, bool hfnmiena, bool privdefena)
{
	uint32_t regval = 0;

	if (enable) {
		regval |= MPU_CTRL_ENABLE;	/* Enable the MPU */
		/* save the current region before printing the information */
		if (hfnmiena) {
			regval |= MPU_CTRL_HFNMIENA;	/* Enable MPU during hard fault, NMI, and FAULTMAS */
		}

		if (privdefena) {
			regval |= MPU_CTRL_PRIVDEFENA;	/* Enable privileged access to default memory map */
		}
	}
	/* restore the previous region */
	putreg32(regval, MPU_CTRL);

	/* Set attribute registers */
	regval = MPU_MEM_ATTR0;
	regval |= MPU_MEM_ATTR1 << 8;
	regval |= MPU_MEM_ATTR2 << 16;
	regval |= MPU_MEM_ATTR3 << 24;
	putreg32(regval, MPU_MAIR0);
	regval = MPU_MEM_ATTR4;
	regval |= MPU_MEM_ATTR5 << 8;
	regval |= MPU_MEM_ATTR6 << 16;
	regval |= MPU_MEM_ATTR7 << 24;
	putreg32(regval, MPU_MAIR1);
}

/****************************************************************************
 * Name: mpu_priv_stronglyordered
 *
 * Description:
 *   Configure a region for privileged, strongly ordered memory
 *
 ****************************************************************************/

#if defined(CONFIG_ARMV8M_HAVE_ICACHE) || defined(CONFIG_ARMV8M_DCACHE)
static inline void mpu_priv_stronglyordered(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_OUT;
	regval |= MPU_RBAR_AP_RWNO;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WT_T_RA);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}
#endif

/****************************************************************************
 * Name: mpu_user_flash
 *
 * Description:
 *   Configure a region for user program flash
 *
 ****************************************************************************/

static inline void mpu_userflash(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_NON;
	regval |= MPU_RBAR_AP_RORO;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WT_T_RA);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

/****************************************************************************
 * Name: mpu_privflash
 *
 * Description:
 *   Configure a region for privileged program flash
 *
 ****************************************************************************/

static inline void mpu_privflash(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_NON;
	regval |= MPU_RBAR_AP_RONO;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WT_T_RA);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

/****************************************************************************
 * Name: mpu_userintsram
 *
 * Description:
 *   Configure a region as user internal SRAM
 *
 ****************************************************************************/

static inline void mpu_userintsram(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_OUT;
	regval |= MPU_RBAR_AP_RWRW;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WT_T_RA);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

/****************************************************************************
 * Name: mpu_userintsram_wb
 *
 * Description:
 *   Configure a region as user internal SRAM
 *
 ****************************************************************************/

static inline void mpu_userintsram_wb(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_NON;
	regval |= MPU_RBAR_AP_RWRW;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_NC);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

/****************************************************************************
 * Name: mpu_privintsram
 *
 * Description:
 *   Configure a region as privileged internal SRAM
 *
 ****************************************************************************/

static inline void mpu_privintsram(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_OUT;
	regval |= MPU_RBAR_AP_RWNO;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WT_T_RA);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

/****************************************************************************
 * Name: mpu_userextsram
 *
 * Description:
 *   Configure a region as user external SRAM
 *
 ****************************************************************************/

static inline void mpu_userextsram(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_OUT;
	regval |= MPU_RBAR_AP_RWRW;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WT_T_RA);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

/****************************************************************************
 * Name: mpu_privextsram
 *
 * Description:
 *   Configure a region as privileged external SRAM
 *
 ****************************************************************************/

static inline void mpu_privextsram(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */
	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_OUT;
	regval |= MPU_RBAR_AP_RWNO;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_WT_T_RA);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

/****************************************************************************
 * Name: mpu_peripheral
 *
 * Description:
 *   Configure a region as privileged periperal address space
 *
 ****************************************************************************/

static inline void mpu_peripheral(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;

	DEBUGASSERT(region < CONFIG_ARMV8M_MPU_NREGIONS);
	DEBUGASSERT(!(base & ~MPU_RBAR_ADDR_MASK));
	DEBUGASSERT(!(size & ~MPU_RLAR_LIMIT_MASK));

	/* Select the region */
	putreg32(region, MPU_RNR);

	/* Select the region base address */
	regval = (base & MPU_RBAR_ADDR_MASK);
	regval |= MPU_RBAR_SH_OUT;
	regval |= MPU_RBAR_AP_RWNO;
	regval |= MPU_RBAR_XN;
	putreg32(regval, MPU_RBAR);

	/* The configure the region */
	regval = (base + size) & MPU_RLAR_LIMIT_MASK;
	regval |= MPU_MAIR_IDX(MPU_MEM_ATTR_IDX_DEVICE);
	regval |= MPU_RLAR_ENABLE;
	putreg32(regval, MPU_RLAR);
}

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __ARCH_ARM_SRC_COMMON_CORTEXM_MPU_H */
#endif
