/*
 * Copyright (c) 2015-2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <assert.h>
#include <string.h>

#include <libfdt.h>

#include <platform_def.h>

#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <common/desc_image_load.h>
#include <common/fdt_fixup.h>
#include <lib/optee_utils.h>
#include <lib/utils.h>
#include <plat/common/platform.h>

#include "sheipa_private.h"

typedef struct {
	unsigned int IDAU_BARx;		/*!< ,	Address offset: 0x00 */
	unsigned int IDAU_LARx;		/*!< ,	Address offset: 0x04 */
} MPC_EntryTypeDef;

typedef struct {
	MPC_EntryTypeDef ENTRY[8]; /*!< ,	Address offset: 0x00 ~ 0x3C*/
	unsigned int IDAU_CTRL;		/*!< ,	Address offset: 0x40 */
	unsigned int IDAU_LOCK;		/*!< ,	Address offset: 0x44 */
} MPC_TypeDef;

#define MPC1			((MPC_TypeDef			*) MPC1_BASE_S)

extern void sys_timer_enable(unsigned char en);

/* Data structure which holds the extents of the trusted SRAM for BL2 */
static meminfo_t bl2_tzram_layout __aligned(CACHE_WRITEBACK_GRANULE);

void bl2_early_platform_setup2(u_register_t arg0, u_register_t arg1,
			       u_register_t arg2, u_register_t arg3)
{
	meminfo_t *mem_layout = (void *)arg1;

	/* Initialize the console to provide early debug support */
	sheipa_console_init();

	/* Setup the BL2 memory layout */
	bl2_tzram_layout = *mem_layout;

	/* Allow BL2 to see the whole Trusted RAM */
	bl2_tzram_layout.total_base = BL2_RW_BASE;
	bl2_tzram_layout.total_size = BL2_RW_SIZE;

	plat_sheipa_io_setup();
}

static void security_setup(void)
{
	/*
	 * This is where a TrustZone address space controller and other
	 * security related peripherals, would be configured.
	 */
}

void bl2_platform_setup(void)
{
	security_setup();

	/* AmebaD2 Initialize timer */
	sys_timer_enable(1);
}

#ifdef __aarch64__
#define SHEIPA_CONFIGURE_BL2_MMU(...)	sheipa_configure_mmu_el1(__VA_ARGS__)
#else
#define SHEIPA_CONFIGURE_BL2_MMU(...)	sheipa_configure_mmu_svc_mon(__VA_ARGS__)
#endif

void bl2_plat_arch_setup(void)
{
	SHEIPA_CONFIGURE_BL2_MMU(bl2_tzram_layout.total_base,
			      bl2_tzram_layout.total_size,
			      BL_CODE_BASE, BL_CODE_END,
			      BL_RO_DATA_BASE, BL_RO_DATA_END
#if USE_COHERENT_MEM
			      , BL_COHERENT_RAM_BASE, BL_COHERENT_RAM_END
#endif
				  );
}

/*******************************************************************************
 * Gets SPSR for BL32 entry
 ******************************************************************************/
static uint32_t sheipa_get_spsr_for_bl32_entry(void)
{
#ifdef __aarch64__
	/*
	 * The Secure Payload Dispatcher service is responsible for
	 * setting the SPSR prior to entry into the BL3-2 image.
	 */
	return 0;
#else
	return SPSR_MODE32(MODE32_svc, SPSR_T_ARM, SPSR_E_LITTLE,
			   DISABLE_ALL_EXCEPTIONS);
#endif
}

/*******************************************************************************
 * Gets SPSR for BL33 entry
 ******************************************************************************/
static uint32_t sheipa_get_spsr_for_bl33_entry(void)
{
	uint32_t spsr;
#ifdef __aarch64__
	unsigned int mode;

	/* Figure out what mode we enter the non-secure world in */
	mode = (el_implemented(2) != EL_IMPL_NONE) ? MODE_EL2 : MODE_EL1;
	/* Currently we don't need EL2(hyp mode), so we jump to EL1 from EL3 */
	mode = MODE_EL1;

	/*
	 * TODO: Consider the possibility of specifying the SPSR in
	 * the FIP ToC and allowing the platform to have a say as
	 * well.
	 */
	spsr = SPSR_64(mode, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
#else
	spsr = SPSR_MODE32(MODE32_svc,
		    plat_get_ns_image_entrypoint() & 0x1,
		    SPSR_E_LITTLE, DISABLE_ALL_EXCEPTIONS);
#endif
	return spsr;
}

static int sheipa_bl2_handle_post_image_load(unsigned int image_id)
{
	int err = 0;
	bl_mem_params_node_t *bl_mem_params = get_bl_mem_params_node(image_id);
#if defined(SPD_opteed) || defined(AARCH32_SP_OPTEE)
	bl_mem_params_node_t *pager_mem_params = NULL;
	bl_mem_params_node_t *paged_mem_params = NULL;
#endif

	assert(bl_mem_params);

	switch (image_id) {
	case BL32_IMAGE_ID:
#if defined(SPD_opteed) || defined(AARCH32_SP_OPTEE)
		pager_mem_params = get_bl_mem_params_node(BL32_EXTRA1_IMAGE_ID);
		assert(pager_mem_params);

		paged_mem_params = get_bl_mem_params_node(BL32_EXTRA2_IMAGE_ID);
		assert(paged_mem_params);

		err = parse_optee_header(&bl_mem_params->ep_info,
					 &pager_mem_params->image_info,
					 &paged_mem_params->image_info);
		if (err != 0) {
			WARN("OPTEE header parse error.\n");
		}

#if defined(SPD_opteed)
		/*
		 * OP-TEE expect to receive DTB address in x2.
		 * This will be copied into x2 by dispatcher.
		 */
		bl_mem_params->ep_info.args.arg3 = PLAT_SHEIPA_DT_BASE;
#else /* case AARCH32_SP_OPTEE */
		bl_mem_params->ep_info.args.arg0 =
					bl_mem_params->ep_info.args.arg1;
		bl_mem_params->ep_info.args.arg1 = 0;
		bl_mem_params->ep_info.args.arg2 = PLAT_SHEIPA_DT_BASE;
		bl_mem_params->ep_info.args.arg3 = 0;
#endif
#endif
		bl_mem_params->ep_info.spsr = sheipa_get_spsr_for_bl32_entry();
		break;

	case BL33_IMAGE_ID:
#ifdef AARCH32_SP_OPTEE
		/* AArch32 only core: OP-TEE expects NSec EP in register LR */
		pager_mem_params = get_bl_mem_params_node(BL32_IMAGE_ID);
		assert(pager_mem_params);
		pager_mem_params->ep_info.lr_svc = bl_mem_params->ep_info.pc;

		MPC1->ENTRY[7].IDAU_BARx = SHEIPA_OPTEE_SHMEM_START - DRAM_START_ADDR;
#else
		MPC1->ENTRY[7].IDAU_BARx = NS_DRAM0_BASE - DRAM_START_ADDR;
#endif
		/* configure MPC to set FIP location to Non-secure */
		MPC1->ENTRY[7].IDAU_LARx = 0x00600000 - 1;

		MPC1->IDAU_CTRL |= BIT(7);
		MPC1->IDAU_LOCK = 1;

		/* BL33 expects to receive the primary CPU MPID (through r0) */
		bl_mem_params->ep_info.args.arg0 = 0xffff & read_mpidr();
		bl_mem_params->ep_info.spsr = sheipa_get_spsr_for_bl33_entry();

		break;
	default:
		/* Do nothing in default case */
		break;
	}

	return err;
}

/*******************************************************************************
 * This function can be used by the platforms to update/use image
 * information for given `image_id`.
 ******************************************************************************/
int bl2_plat_handle_post_image_load(unsigned int image_id)
{
	return sheipa_bl2_handle_post_image_load(image_id);
}

uintptr_t plat_get_ns_image_entrypoint(void)
{
	return NS_IMAGE_OFFSET;
}
