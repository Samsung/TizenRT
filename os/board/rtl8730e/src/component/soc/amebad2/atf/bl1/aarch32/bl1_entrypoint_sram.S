/*
 * Copyright (c) 2016-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <common/bl_common.h>
#include <context.h>
#include <el3_common_macros.S>
#include <smccc_helpers.h>
#include <smccc_macros.S>

	.globl	bl1_boot_sram

func bl1_boot_sram
	b	bl1_entrypoint
endfunc bl1_boot_sram

