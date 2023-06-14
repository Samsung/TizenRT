/*
 * Copyright (c) 2015-2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <platform_def.h>
#include <drivers/console.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/bl_common.h>

#include "sheipa_private.h"

/* Data structure which holds the extents of the trusted SRAM for BL1*/
static meminfo_t bl1_tzram_layout;


meminfo_t *bl1_plat_sec_mem_layout(void)
{
	return &bl1_tzram_layout;
}

/*******************************************************************************
 * Perform any BL1 specific platform actions.
 ******************************************************************************/
void bl1_early_platform_setup(void)
{
	/* Initialize the console to provide early debug support */
	sheipa_console_init();

	/* Allow BL1 to see the whole Trusted RAM */
	bl1_tzram_layout.total_base = BL1_RW_BASE;
	bl1_tzram_layout.total_size = BL1_RW_SIZE;

#if (BOOT_FLASH)
	sheipa_dram_init();
	/* Read secure boot image in flash to dram */
	rtk_flash_read();
#endif
}

/******************************************************************************
 * Perform the very early platform specific architecture setup.  This only
 * does basic initialization. Later architectural setup (bl1_arch_setup())
 * does not do anything platform specific.
 *****************************************************************************/
#ifdef __aarch64__
#define SHEIPA_CONFIGURE_BL1_MMU(...)	sheipa_configure_mmu_el3(__VA_ARGS__)
#else
#define SHEIPA_CONFIGURE_BL1_MMU(...)	sheipa_configure_mmu_svc_mon(__VA_ARGS__)
#endif

void bl1_plat_arch_setup(void)
{
	SHEIPA_CONFIGURE_BL1_MMU(bl1_tzram_layout.total_base,
				bl1_tzram_layout.total_size,
				BL_CODE_BASE - 0x20, BL1_CODE_END,
				BL1_RO_DATA_BASE, BL1_RO_DATA_END
#if USE_COHERENT_MEM
				, BL_COHERENT_RAM_BASE, BL_COHERENT_RAM_END
#endif
				);
}

void bl1_platform_setup(void)
{
	plat_sheipa_io_setup();
}
