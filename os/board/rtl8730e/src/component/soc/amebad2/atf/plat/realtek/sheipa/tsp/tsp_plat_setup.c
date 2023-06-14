/*
 * Copyright (c) 2014-2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/bl_common.h>
#include <plat/arm/common/plat_arm.h>
#include <assert.h>

#include "../sheipa_private.h"

#define MAP_BL_TSP_TOTAL	MAP_REGION_FLAT(			\
					BL32_BASE,			\
					BL32_END - BL32_BASE,		\
					MT_MEMORY | MT_RW | MT_SECURE)

#if SEPARATE_CODE_AND_RODATA
#define MAP_BL_RO		MAP_REGION_FLAT(			\
					BL_CODE_BASE,			\
					BL_CODE_END - BL_CODE_BASE,	\
					MT_CODE | MT_SECURE),		\
				MAP_REGION_FLAT(			\
					BL_RO_DATA_BASE,		\
					BL_RO_DATA_END			\
						- BL_RO_DATA_BASE,	\
					MT_RO_DATA | MT_SECURE)
#else
#define MAP_BL_RO		MAP_REGION_FLAT(			\
					BL_CODE_BASE,			\
					BL_CODE_END - BL_CODE_BASE,	\
					MT_CODE | MT_SECURE)
#endif

extern const mmap_region_t plat_sheipa_mmap[];

/*******************************************************************************
 * Initialize the UART
 ******************************************************************************/
void tsp_early_platform_setup(void)
{
	/*
	 * Initialize a different console than already in use to display
	 * messages from TSP
	 */
	sheipa_console_init();

	/* Initialize the platform config for future decision making */
	sheipa_config_setup();
}

/*******************************************************************************
 * Perform platform specific setup placeholder
 ******************************************************************************/
void tsp_platform_setup(void)
{
	return;
}

/*******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this is only intializes the MMU
 ******************************************************************************/
void tsp_plat_arch_setup(void)
{
#if USE_COHERENT_MEM
	/* Ensure ARM platforms don't use coherent memory in TSP */
	assert((BL_COHERENT_RAM_END - BL_COHERENT_RAM_BASE) == 0U);
#endif

	const mmap_region_t bl_regions[] = {
		MAP_BL_TSP_TOTAL,
		MAP_BL_RO,
		{0}
	};

	setup_page_tables(bl_regions, plat_sheipa_mmap);
	enable_mmu_el1(0);
}
