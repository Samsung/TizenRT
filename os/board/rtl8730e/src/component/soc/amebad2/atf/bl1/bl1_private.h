/*
 * Copyright (c) 2013-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BL1_PRIVATE_H
#define BL1_PRIVATE_H

#include <stdint.h>

#include <common/bl_common.h>

/******************************************
 * Function prototypes
 *****************************************/
void bl1_arch_setup(void);
void bl1_arch_next_el_setup(void);

void bl1_prepare_next_image(unsigned int image_id);

u_register_t bl1_fwu_smc_handler(unsigned int smc_fid,
		u_register_t x1,
		u_register_t x2,
		u_register_t x3,
		u_register_t x4,
		void *cookie,
		void *handle,
		unsigned int flags);

#endif /* BL1_PRIVATE_H */
