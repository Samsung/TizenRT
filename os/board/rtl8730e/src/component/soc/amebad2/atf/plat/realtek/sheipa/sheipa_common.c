/*
 * Copyright (c) 2015-2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <platform_def.h>

#include <arch_helpers.h>
#include <common/bl_common.h>
#include <lib/xlat_tables/xlat_tables_v2.h>

#include "sheipa_private.h"

#define MAP_DEVICE0	MAP_REGION_FLAT(DEVICE0_BASE,			\
					DEVICE0_SIZE,			\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_DEVICE1	MAP_REGION_FLAT(DEVICE1_BASE,			\
					DEVICE1_SIZE,			\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_SHARED_RAM	MAP_REGION_FLAT(SHARED_RAM_BASE,		\
					SHARED_RAM_SIZE,		\
					MT_DEVICE  | MT_RW | MT_SECURE)

#define MAP_BL2_MEM	MAP_REGION_FLAT(BL2_BASE, BL2_LIMIT-BL2_BASE,	\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_BL32_MEM	MAP_REGION_FLAT(BL32_MEM_BASE, BL32_MEM_SIZE,	\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_S_DRAM0	MAP_REGION_FLAT(NS_DRAM0_BASE, NS_DRAM0_SIZE,	\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_NS_DRAM0	MAP_REGION_FLAT(NS_DRAM0_BASE, NS_DRAM0_SIZE,	\
					MT_MEMORY | MT_RW | MT_NS)

#define MAP_FLASH0	MAP_REGION_FLAT(SHEIPA_FLASH0_BASE, \
					SHEIPA_FLASH0_SIZE, \
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_KM0_RAM	MAP_REGION_FLAT(KM0_RAM, KM0_RAM_SIZE, \
					MT_MEMORY | MT_RW | MT_NS)

/*
 * Table of regions for various BL stages to map using the MMU.
 * This doesn't include TZRAM as the 'mem_layout' argument passed to
 * arm_configure_mmu_elx() will give the available subset of that,
 */
#if IMAGE_BL1
static const mmap_region_t plat_sheipa_mmap[] = {
	MAP_FLASH0,
	MAP_SHARED_RAM,
	MAP_DEVICE0,
	MAP_DEVICE1,
	MAP_BL2_MEM,
	{0}
};
#endif
#if IMAGE_BL2
static const mmap_region_t plat_sheipa_mmap[] = {
	MAP_FLASH0,
	MAP_SHARED_RAM,
	MAP_DEVICE0,
	MAP_S_DRAM0,
	MAP_DEVICE1,
	MAP_BL32_MEM,
	{0}
};
#endif
#if IMAGE_BL31
static const mmap_region_t plat_sheipa_mmap[] = {
	MAP_SHARED_RAM,
	MAP_DEVICE0,
	MAP_DEVICE1,
	MAP_BL32_MEM,
	{0}
};
#endif

#if IMAGE_BL32
/*
 * Table of regions to map using the MMU.
 * This doesn't include TZRAM as the 'mem_layout' argument passed to
 * configure_mmu_elx() will give the available subset of that,
 */
const mmap_region_t plat_sheipa_mmap[] = {
	MAP_SHARED_RAM,
	MAP_DEVICE0,
	MAP_NS_DRAM0,
	MAP_DEVICE1,
	MAP_KM0_RAM,
	{0}
};
#endif
/*******************************************************************************
 * Macro generating the code for the function setting up the pagetables as per
 * the platform memory map & initialize the mmu, for the given exception level
 ******************************************************************************/
#if USE_COHERENT_MEM
#define DEFINE_CONFIGURE_MMU_EL(_el)					\
	void sheipa_configure_mmu_##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long code_start,		\
				   unsigned long code_limit,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit,		\
				   unsigned long coh_start,		\
				   unsigned long coh_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(code_start, code_start,			\
				code_limit - code_start,		\
				MT_CODE | MT_SECURE);			\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_RO_DATA | MT_SECURE);		\
		mmap_add_region(coh_start, coh_start,			\
				coh_limit - coh_start,			\
				MT_DEVICE | MT_RW | MT_SECURE);		\
		mmap_add(plat_sheipa_mmap);				\
		init_xlat_tables();					\
									\
		enable_mmu_##_el(0);					\
	}
#else
#define DEFINE_CONFIGURE_MMU_EL(_el)					\
	void sheipa_configure_mmu_##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long code_start,		\
				   unsigned long code_limit,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(code_start, code_start,			\
				code_limit - code_start,		\
				MT_CODE | MT_SECURE);			\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_RO_DATA | MT_SECURE);		\
		mmap_add(plat_sheipa_mmap);				\
		init_xlat_tables();					\
									\
		enable_mmu_##_el(0);					\
	}
#endif

/* Define EL1 and EL3 variants of the function initialising the MMU */
#ifdef __aarch64__
DEFINE_CONFIGURE_MMU_EL(el1)
DEFINE_CONFIGURE_MMU_EL(el3)
#else
DEFINE_CONFIGURE_MMU_EL(svc_mon)
#endif

void sheipa_config_setup(void)
{
	return;
}

