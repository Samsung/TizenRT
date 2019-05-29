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
/************************************************************************************
 * os/arch/arm/src/armv7-m/mpu.h
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

#ifdef CONFIG_ARMV7M_MPU
#ifndef __ASSEMBLY__
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <debug.h>
#ifdef CONFIG_BUILD_PROTECTED
#include <chip/mpu-reg.h>
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
#define MPU_RASR                0xe000eda0	/* MPU Region Attribute and Size Register */

#define MPU_RBAR_A1             0xe000eda4	/* MPU alias registers */
#define MPU_RASR_A1             0xe000eda8
#define MPU_RBAR_A2             0xe000edac
#define MPU_RASR_A2             0xe000edb0
#define MPU_RBAR_A3             0xe000edb4
#define MPU_RASR_A3             0xe000edb8

/* MPU Type Register Bit Definitions */

#define MPU_TYPE_SEPARATE       (1 << 0)	/* Bit 0: 0:unified or 1:separate memory maps */
#define MPU_TYPE_DREGION_SHIFT  (8)	/* Bits 8-15: Number MPU data regions */
#define MPU_TYPE_DREGION_MASK   (0xff << MPU_TYPE_DREGION_SHIFT)
#define MPU_TYPE_IREGION_SHIFT  (16)	/* Bits 16-23: Number MPU instruction regions */
#define MPU_TYPE_IREGION_MASK   (0xff << MPU_TYPE_IREGION_SHIFT)

/* MPU Control Register Bit Definitions */

#define MPU_CTRL_ENABLE         (1 << 0)	/* Bit 0: Enable the MPU */
#define MPU_CTRL_HFNMIENA       (1 << 1)	/* Bit 1: Enable MPU during hard fault, NMI, and FAULTMAS */
#define MPU_CTRL_PRIVDEFENA     (1 << 2)	/* Bit 2: Enable privileged access to default memory map */

/* MPU Region Number Register Bit Definitions */

#if defined(CONFIG_ARMV7M_MPU_NREGIONS)
#if CONFIG_ARMV7M_MPU_NREGIONS <= 8
#define MPU_RNR_MASK            (0x00000007)
#elif CONFIG_ARMV7M_MPU_NREGIONS <= 16
#define MPU_RNR_MASK            (0x0000000f)
#elif CONFIG_ARMV7M_MPU_NREGIONS <= 32
#define MPU_RNR_MASK            (0x0000001f)
#else
#error "FIXME: Unsupported number of MPU regions"
#endif
#endif

/* MPU Region Base Address Register Bit Definitions */

#define MPU_RBAR_REGION_SHIFT   (0)	/* Bits 0-3: MPU region */
#define MPU_RBAR_REGION_MASK    (15 << MPU_RBAR_REGION_SHIFT)
#define MPU_RBAR_VALID          (1 << 4)	/* Bit 4: MPU Region Number valid */
#define MPU_RBAR_ADDR_MASK      0xffffffe0	/* Bits N-31:  Region base addrese */

/* MPU Region Attributes and Size Register Bit Definitions */

#define MPU_RASR_ENABLE         (1 << 0)	/* Bit 0: Region enable */
#define MPU_RASR_SIZE_SHIFT     (1)	/* Bits 1-5: Size of the MPU protection region */

#define MPU_RASR_SIZE_MASK      (31 << MPU_RASR_SIZE_SHIFT)
#define MPU_RASR_SIZE_LOG2(n) ((n-1) << MPU_RASR_SIZE_SHIFT)
#define MPU_RASR_SRD_SHIFT      (8)	/* Bits 8-15: Subregion disable */
#define MPU_RASR_SRD_MASK       (0xff << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_0        (0x01 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_1        (0x02 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_2        (0x04 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_3        (0x08 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_4        (0x10 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_5        (0x20 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_6        (0x40 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_SRD_7        (0x80 << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_ATTR_SHIFT     (16)	/* Bits 16-31: MPU Region Attribute field */
#define MPU_RASR_ATTR_MASK      (0xffff << MPU_RASR_ATTR_SHIFT)
#define MPU_RASR_B              (1 << 16)	/* Bit 16: Bufferable */
#define MPU_RASR_C              (1 << 17)	/* Bit 17: Cacheable */
#define MPU_RASR_S              (1 << 18)	/* Bit 18: Shareable */
#define MPU_RASR_TEX_SHIFT    (19)	/* Bits 19-21: TEX Address Permisson */
#define MPU_RASR_TEX_MASK     (7 << MPU_RASR_TEX_SHIFT)
#define MPU_RASR_TEX(n)		((uint32_t)(n) << MPU_RASR_TEX_SHIFT)
#define MPU_RASR_AP_SHIFT       (24)	/* Bits 24-26: Access permission */
#define MPU_RASR_AP_MASK        (7 << MPU_RASR_AP_SHIFT)
#define MPU_RASR_AP_NONO      (0 << MPU_RASR_AP_SHIFT)	/* P:None U:None */
#define MPU_RASR_AP_RWNO      (1 << MPU_RASR_AP_SHIFT)	/* P:RW   U:None */
#define MPU_RASR_AP_RWRO      (2 << MPU_RASR_AP_SHIFT)	/* P:RW   U:RO   */
#define MPU_RASR_AP_RWRW      (3 << MPU_RASR_AP_SHIFT)	/* P:RW   U:RW   */
#define MPU_RASR_AP_RONO      (5 << MPU_RASR_AP_SHIFT)	/* P:RO   U:None */
#define MPU_RASR_AP_RORO      (6 << MPU_RASR_AP_SHIFT)	/* P:RO   U:RO   */
#define MPU_RASR_XN             (1 << 28)	/* Bit 28: Instruction access disable */

#ifndef CONFIG_ARMV7M_MPU_NREGIONS
#define CONFIG_ARMV7M_MPU_NREGIONS 8
#endif

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
 * Private Data
 ****************************************************************************/

/* These sets represent the set of disabled memory sub-regions.  A bit set
 * corresponds to a disabled sub-region; the LS bit corresponds to the first
 * region.
 *
 * The g_ms_regionmask array is indexed by the number of subregions at the
 * end of the region:  0 means no sub-regions are available(0xff) and 8 means
 * all subregions are available (0x00).
 */

static const uint8_t g_ms_regionmask[9] = {
	0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00
};

/* The g_ls_regionmask array is indexed by the number of subregions at the
 * beginning of the region:  0 means no sub-regions need be disabled (0x00)
 * and 8 means all subregions must be disabled (0xff).
 */

static const uint8_t g_ls_regionmask[9] = {
	0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

 /****************************************************************************
 * Name: mpu_subregion_ms
 *
 * Description:
 *   Given (1) the size of the memory to be mapped and (2) the log2 size
 *   of the mapping to use, determine the minimal sub-region set at the
 *   to be disabled at the higher end of the region.
 *
 * Assumption:
 *   l2size has the same properties as the return value from
 *   mpu_log2regionceil()
 *
 ****************************************************************************/

static inline uint32_t mpu_subregion_ms(size_t size, uint8_t l2size)
{
	unsigned int nsrs;
	uint32_t asize;
	uint32_t mask;

	/* Examples with l2size = 12:
	 *
	 *         Shifted Adjusted        Number      Sub-Region
	 * Size    Mask    Size      Shift Sub-Regions Bitset
	 * 0x1000  0x01ff  0x1000    9     8           0x00
	 * 0x0c00  0x01ff  0x0c00    9     6           0xc0
	 * 0x0c40  0x01ff  0x0e00    9     7           0x80
	 */

	if (l2size < 32) {
		mask = ((1 << l2size) - 1) >> 3;	/* Shifted mask */
	}

	/* The 4Gb region size is a special case */

	else {
		/* NOTE: There is no way to represent a 4Gb region size in the 32-bit
		 * input.
		 */

		mask = 0x1fffffff;		/* Shifted mask */
	}

	asize = (size + mask) & ~mask;	/* Adjusted size */
	nsrs = asize >> (l2size - 3);	/* Number of subregions */
	return g_ms_regionmask[nsrs];
}

/****************************************************************************
 * Name: mpu_subregion_ls
 *
 * Description:
 *   Given (1) the offset to the beginning of data in the region and (2) the
 *   log2 size of the mapping to use, determine the minimal sub-region set
 *   to span that memory region sub-region set at the to be disabled at the
 *   higher end of the region
 *
 * Assumption:
 *   l2size has the same properties as the return value from
 *   mpu_log2regionceil()
 *
 ****************************************************************************/

static inline uint32_t mpu_subregion_ls(size_t offset, uint8_t l2size)
{
	unsigned int nsrs;
	uint32_t aoffset;
	uint32_t mask;

	/* Examples with l2size = 12:
	 *
	 *         Shifted Adjusted        Number      Sub-Region
	 * Offset  Mask    Offset    Shift Sub-Regions Bitset
	 * 0x0000  0x01ff  0x0000    9     8           0x00
	 * 0x0400  0x01ff  0x0400    9     6           0x03
	 * 0x02c0  0x01ff  0x0200    9     7           0x01
	 */

	if (l2size < 32) {
		mask = ((1 << l2size) - 1) >> 3;	/* Shifted mask */
	}

	/* The 4Gb region size is a special case */

	else {
		/* NOTE: There is no way to represent a 4Gb region size in the 32-bit
		 * input.
		 */

		mask = 0x1fffffff;		/* Shifted mask */
	}

	aoffset = offset & ~mask;	/* Adjusted offset */
	nsrs = aoffset >> (l2size - 3);	/* Number of subregions */
	return g_ls_regionmask[nsrs];
}
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

static inline uint8_t mpu_log2regionceil(uintptr_t base, size_t size)
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

static inline uint8_t mpu_log2regionfloor(uintptr_t base, size_t size)
{
	uint8_t l2size = mpu_log2regionceil(base, size);

	if (l2size > 4 && (base & ((1 << l2size) - 1)) + size < (1 << l2size)) {
		l2size--;
	}

	return l2size;
}

/****************************************************************************
 * Name: mpu_subregion
 *
 * Description:
 *   Given the size of the (1) memory to be mapped and (2) the log2 size
 *   of the mapping to use, determine the minimal sub-region set to span
 *   that memory region.
 *
 * Assumption:
 *   l2size has the same properties as the return value from
 *   mpu_log2regionceil()
 *
 ****************************************************************************/

static inline uint32_t mpu_subregion(uintptr_t base, size_t size, uint8_t l2size)
{
	uint32_t mask;
	size_t offset;
	uint32_t ret;

	/* Eight subregions are supported.  The representation is as an 8-bit
	 * value with the LS bit corresponding to subregion 0.  A bit is set
	 * to disable the sub-region.
	 *
	 * l2size: Log2 of the actual region size is <= (1 << l2size);
	 */

	DEBUGASSERT(l2size > 4 && size <= (1 << l2size));

	/* For region sizes of 32, 64, and 128 bytes, the effect of setting
	 * one or more bits of the SRD field to 1 is UNPREDICTABLE.
	 */

	if (l2size < 8) {
		return 0;
	}

	/* Calculate the offset of the base address into the aligned region. */

	mask = (1 << l2size) - 1;
	offset = base & mask;

	/* Calculate the mask need to handle disabled subregions at the end of the
	 * region
	 */

	ret = mpu_subregion_ms(size + offset, l2size);

	/* Then OR in the mask need to handle disabled subretinos at the beginning
	 * of the region.
	 */

	ret |= mpu_subregion_ls(offset, l2size);
	return ret;
}

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
	uint32_t base, size, attr;

	/* save the current region before printing the information */
	temp = getreg32(MPU_RNR);

	lldbg("*****************************************************************************\n");
	lldbg("*REGION_NO.\tBASE_ADDRESS\t    SIZE\t REG\t   \tSTATUS\tATTRIBUTES*\n");
	lldbg("*****************************************************************************\n");
	for (idx = 0; idx < 8; idx++) {
		putreg32(idx, MPU_RNR);
		base = getreg32(MPU_RBAR);
		size = getreg32(MPU_RASR);
		attr = getreg32(MPU_RASR);
		lldbg("%8d\t\t%8X\t%8X\t%8X\t%8X\t%8X\n",
			(base & MPU_RBAR_REGION_MASK),
			(base & MPU_RBAR_ADDR_MASK),
			(size ? (1 << (((size & MPU_RASR_SIZE_MASK) >> MPU_RASR_SIZE_SHIFT) + 1)) : 0),
			((size & MPU_RASR_SRD_MASK) >> MPU_RASR_SRD_SHIFT),
			(size & MPU_RASR_ENABLE) ? 1 : 0,
			((attr & MPU_RASR_ATTR_MASK) >> MPU_RASR_ATTR_SHIFT));
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
}

/****************************************************************************
 * Name: mpu_priv_stronglyordered
 *
 * Description:
 *   Configure a region for privileged, strongly ordered memory
 *
 ****************************************************************************/

#if defined(CONFIG_ARMV7M_HAVE_ICACHE) || defined(CONFIG_ARMV7M_DCACHE)
static inline void mpu_priv_stronglyordered(uint32_t region, uintptr_t base, size_t size)
{
	uint32_t regval;
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region | MPU_RBAR_VALID, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region  */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size    */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions    */
			 /* Not Cacheable  */
			 /* Not Bufferable */
			 MPU_RASR_S |			/* Shareable      */
			 MPU_RASR_AP_RWNO;		/* P:RW   U:None  */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_C |			/* Cacheable     */
			 MPU_RASR_AP_RORO;		/* P:RO   U:RO   */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_C |			/* Cacheable     */
			 MPU_RASR_AP_RONO;		/* P:RO   U:None */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_S |			/* Shareable     */
			 MPU_RASR_C |			/* Cacheable     */
			 MPU_RASR_AP_RWRW;		/* P:RW   U:RW   */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_B |			/* Shareable     */
			 MPU_RASR_TEX(5) |			/* Cacheable     */
			 MPU_RASR_AP_RWRW;		/* P:RW   U:RW   */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_S |			/* Shareable     */
			 MPU_RASR_C |			/* Cacheable     */
			 MPU_RASR_AP_RWNO;		/* P:RW   U:None */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_S |			/* Shareable     */
			 MPU_RASR_C |			/* Cacheable     */
			 MPU_RASR_B |			/* Bufferable    */
			 MPU_RASR_AP_RWRW;		/* P:RW   U:RW   */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_S |			/* Shareable     */
			 MPU_RASR_C |			/* Cacheable     */
			 MPU_RASR_B |			/* Bufferable    */
			 MPU_RASR_AP_RWNO;		/* P:RW   U:None */
	putreg32(regval, MPU_RASR);
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
	uint8_t l2size;
	uint8_t subregions;

	DEBUGASSERT(region < CONFIG_ARMV7M_MPU_NREGIONS);
	/* Select the region */

	putreg32(region, MPU_RNR);

	/* Select the region base address */

	putreg32((base & MPU_RBAR_ADDR_MASK) | region, MPU_RBAR);

	/* Select the region size and the sub-region map */

	l2size = mpu_log2regionceil(base, size);
	subregions = mpu_subregion(base, size, l2size);

	/* The configure the region */

	regval = MPU_RASR_ENABLE |	/* Enable region */
			 MPU_RASR_SIZE_LOG2((uint32_t)l2size) |	/* Region size   */
			 ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |	/* Sub-regions   */
			 MPU_RASR_S |			/* Shareable     */
			 MPU_RASR_B |			/* Bufferable    */
			 MPU_RASR_AP_RWNO |		/* P:RW   U:None */
			 MPU_RASR_XN;			/* Instruction access disable */

	putreg32(regval, MPU_RASR);
}

#if defined(CONFIG_APP_BINARY_SEPARATION)
static inline void up_set_mpu_app_configuration(struct tcb_s *rtcb)
{
	if ((rtcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
		putreg32(rtcb->mpu_regs[REG_RNR], MPU_RNR);
		putreg32(rtcb->mpu_regs[REG_RBAR], MPU_RBAR);
		putreg32(rtcb->mpu_regs[REG_RASR], MPU_RASR);
	}
}
#else
#define up_set_mpu_app_configuration(x)
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __ARCH_ARM_SRC_COMMON_CORTEXM_MPU_H */
#endif
