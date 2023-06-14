/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017, Realtek. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <platform_def.h>

.globl general_exception
.globl tlb_refill_exception
/*
 * General exception vector
 * Call plat_report_exception with arg set to 0
 *
 */
func general_exception
    mfc0	$k0, CP0_EPC
    mfc0	$k1, CP0_CAUSE
    li		$a0, 0
    j		plat_report_exception
    nop
endfunc general_exception

/*
 * TLB miss
 * Call plat_report_exception with arg set to 1
 *
 */
func tlb_refill_exception
    mfc0	$k0, CP0_EPC
    mfc0	$k1, CP0_CAUSE
    li		$a0, 1
    j		plat_report_exception
    nop
endfunc tlb_refill_exception

