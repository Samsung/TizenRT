/*
 * Copyright (c) 2015-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017, Realtek. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>

.globl    bl1_entrypoint

/* -----------------------------------------------------
 * bl1_entrypoint() is the entry point into the trusted
 * firmware code when a cpu is released from warm or
 * cold reset.
 * -----------------------------------------------------
 */

.func bl1_entrypoint
    /* ---------------------------------------------------------------------
     * If the reset address is programmable then bl1_entrypoint() is
     * executed only on the cold boot path. Therefore, we can skip the warm
     * boot mailbox mechanism.
     * ---------------------------------------------------------------------
     */
bl1_entrypoint:
    .set noreorder

    la        $t9, reset
    jalr      $t9
    nop

    .org 0x100
    /* TLB refill */
    j tlb_refill_exception
    nop

    .org 0x180
    /* General exception */
    j general_exception
    nop

    .align 4
reset:

    /* Init Timer */
    mtc0    $zero, CP0_COUNT
    mtc0    $zero, CP0_COMPARE

    la       $a0, __BSS_START__
    la       $a1, __BSS_SIZE__
    jal      zeromem16
    nop

    la       $a0, __DATA_RAM_START__
    la       $a1, __DATA_ROM_START__
    la       $a2, __DATA_SIZE__
    jal      memcpy16
    nop

    jal   plat_set_my_stack
    nop
    /* ---------------------------------------------
     * Architectural init. can be generic e.g.
     * enabling stack alignment and platform spec-
     * ific.Perform the latter here
     * and the former in bl1_main.
     * ---------------------------------------------
     */
    jal    bl1_early_platform_setup
    nop
    jal    bl1_plat_arch_setup
    nop

    /* --------------------------------------------------
     * Initialize platform and jump to our c-entry point
     * for this type of reset.
     * --------------------------------------------------
     */
    jal    bl1_main
    nop

1:  b    1b
    nop
.endfunc
