/*
 * Copyright (c) 2013-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017, Realtek. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <auth_mod.h>
#include <bl1.h>
#include <bl_common.h>
#include <debug.h>
#include <errno.h>
#include <platform.h>
#include <platform_def.h>
#include <stdint.h>
#include <delay_timer.h>
#include "bl2_private.h"

/*
 * Check for platforms that use obsolete image terminology
 */
#ifdef BL30_BASE
# error "BL30_BASE platform define no longer used - please use SCP_BL2_BASE"
#endif

/*******************************************************************************
 * Load the SCP_BL2 image if there's one.
 * If a platform does not want to attempt to load SCP_BL2 image it must leave
 * SCP_BL2_BASE undefined.
 * Return 0 on success or if there's no SCP_BL2 image to load, a negative error
 * code otherwise.
 ******************************************************************************/
static int load_scp_bl2(void)
{
	int e = 0;
#ifdef SCP_BL2_BASE
	meminfo_t scp_bl2_mem_info;
	image_info_t scp_bl2_image_info;

	/*
	 * It is up to the platform to specify where SCP_BL2 should be loaded if
	 * it exists. It could create space in the secure sram or point to a
	 * completely different memory.
	 *
	 * The entry point information is not relevant in this case as the AP
	 * won't execute the SCP_BL2 image.
	 */
	INFO("BL2: Loading SCP_BL2\n");
	bl2_plat_get_scp_bl2_meminfo(&scp_bl2_mem_info);
	scp_bl2_image_info.h.version = VERSION_1;
	e = load_auth_image(&scp_bl2_mem_info,
			    SCP_BL2_IMAGE_ID,
			    SCP_BL2_BASE,
			    &scp_bl2_image_info,
			    NULL);

	if (e == 0) {
		/* The subsequent handling of SCP_BL2 is platform specific */
		e = bl2_plat_handle_scp_bl2(&scp_bl2_image_info);
		if (e) {
			ERROR("Failure in platform-specific handling of SCP_BL2 image.\n");
		}
	}
#endif /* SCP_BL2_BASE */

	return e;
}

#ifndef PRELOADED_BL33_BASE
/*******************************************************************************
 * Load the BL33 image.
 * The bl2_to_bl31_params param will be updated with the relevant BL33
 * information.
 * Return 0 on success, a negative error code otherwise.
 ******************************************************************************/
static int load_bl33(bl31_params_t *bl2_to_bl31_params)
{
	meminfo_t bl33_mem_info;
	int e;

	INFO("BL2: Loading BL33\n");
	assert(bl2_to_bl31_params != NULL);

	bl2_plat_get_bl33_meminfo(&bl33_mem_info);

	/* Load the BL33 image in non-secure memory provided by the platform */
	e = load_auth_image(&bl33_mem_info,
			    BL33_IMAGE_ID,
			    plat_get_ns_image_entrypoint(),
			    bl2_to_bl31_params->bl33_image_info,
			    bl2_to_bl31_params->bl33_ep_info);

	if (e == 0) {
		bl2_plat_set_bl33_ep_info(bl2_to_bl31_params->bl33_image_info,
					  bl2_to_bl31_params->bl33_ep_info);
	}

	return e;
}
#endif /* PRELOADED_BL33_BASE */


/*******************************************************************************
 * The only thing to do in BL2 is to load further images and pass control to
 * BL31. The memory occupied by BL2 will be reclaimed by BL3x stages. BL2 runs
 * entirely in S-EL1.
 ******************************************************************************/
void bl2_main(void)
{
	bl31_params_t *bl2_to_bl31_params;
#ifdef PRELOADED_BL33_BASE
	entry_point_info_t *bl31_ep_info;
#endif /* PRELOADED_BL33_BASE */
	int e;
	typedef void   bl33_ep_func();
	bl33_ep_func * bl33_ep;

	NOTICE("BL2: %s\n", version_string);
	NOTICE("BL2: %s\n", build_message);

	/* Perform remaining generic architectural setup in S-EL1 */
	bl2_arch_setup();

#if TRUSTED_BOARD_BOOT
	/* Initialize authentication module */
	auth_mod_init();
#endif /* TRUSTED_BOARD_BOOT */

	/*
	 * Load the subsequent bootloader images
	 */
	e = load_scp_bl2();
	if (e) {
		ERROR("Failed to load SCP_BL2 (%i)\n", e);
		plat_error_handler(e);
	}

	/* Perform platform setup in BL2 after loading SCP_BL2 */
	bl2_platform_setup();

        /*
         * Get a pointer to the memory the platform has set aside to pass
         * information to BL31.
         */
        bl2_to_bl31_params = bl2_plat_get_bl31_params();


#ifdef PRELOADED_BL33_BASE
	/*
	 * In this case, don't load the BL33 image as it's already loaded in
	 * memory. Update BL33 entrypoint information.
	 */
	INFO("BL2: Populating the entrypoint info for the preloaded BL33\n");
	bl2_to_bl31_params->bl33_ep_info->pc = PRELOADED_BL33_BASE;
	bl2_plat_set_bl33_ep_info(NULL, bl2_to_bl31_params->bl33_ep_info);
#else
	e = load_bl33(bl2_to_bl31_params);
	if (e) {
		ERROR("Failed to load BL33 (%i)\n", e);
		plat_error_handler(e);
	}
#endif /* PRELOADED_BL33_BASE */


	/* Flush the params to be passed to memory */
	bl2_plat_flush_bl31_params();

	/*
	 * Start BL33 
	 */

	bl33_ep = (bl33_ep_func*) bl2_to_bl31_params->bl33_ep_info->pc;
 
        /* clean and invalidate data cache to commit instructions
           from cache to SRAM */
        flush_dcache_all();

        /* invalidate instruction cache to fetch directly from SRAM */
        inv_icache_all();

	/* jump to BL33 entrypoint passing no arg */
	bl33_ep();
}
