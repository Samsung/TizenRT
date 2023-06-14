/*
 * Copyright (c) 2015-2016, ARM Limited and Contributors. All rights reserved.
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

#include <arch_helpers.h>
#include <assert.h>
#include <debug.h>
#include <platform.h>
#include <bl_common.h>

/*******************************************************************************
 * This function prepares the context for Secure/Normal world images.
 * Normal world images are transitioned to EL2(if supported) else EL1.
 ******************************************************************************/
void bl1_prepare_next_image(unsigned int image_id)
{
	image_desc_t *image_desc;
	entry_point_info_t *next_bl_ep;
	typedef void  bl2_ep_func(uintptr_t);
        bl2_ep_func * bl2_ep;


	/* Get the image descriptor. */
	image_desc = bl1_plat_get_image_desc(image_id);
	assert(image_desc);

	/* Get the entry point info. */
	next_bl_ep = &image_desc->ep_info;

	/* Allow platform to make change */
	bl1_plat_set_ep_info(image_id, next_bl_ep);

	/* Indicate that image is in execution state. */
	image_desc->state = IMAGE_STATE_EXECUTED;

	print_entry_point_info(next_bl_ep);

	bl2_ep = (bl2_ep_func*)next_bl_ep->pc;

	/* clean and invalidate data cache to commit instructions
	   from cache to SRAM */
	flush_dcache_all();

	/* invalidate instruction cache to fetch directly from SRAM */
	inv_icache_all();

	/* jump to BL2 entrypoint passing bl2_tzram_layout as arg */
	bl2_ep((uintptr_t) next_bl_ep->args.arg1);
}
