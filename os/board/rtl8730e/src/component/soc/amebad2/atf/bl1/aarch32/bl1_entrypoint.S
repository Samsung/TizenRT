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

	.globl	bl1_vector_table
	.globl	bl1_entrypoint

	/* -----------------------------------------------------
	 * Setup the vector table to support SVC & MON mode.
	 * -----------------------------------------------------
	 */
vector_base bl1_vector_table
	b	bl1_entrypoint
	b	report_exception	/* Undef */
	b	bl1_aarch32_smc_handler	/* SMC call */
	b	report_exception	/* Prefetch abort */
	b	report_exception	/* Data abort */
	b	report_exception	/* Reserved */
	b	report_exception	/* IRQ */
	b	report_exception	/* FIQ */

	/* -----------------------------------------------------
	 * bl1_entrypoint() is the entry point into the trusted
	 * firmware code when a cpu is released from warm or
	 * cold reset.
	 * -----------------------------------------------------
	 */

func bl1_entrypoint

	mov sp, #0
	mov lr, #0

	cps #MODE32_sys
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	mov r4, #0
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	mov r9, #0
	mov r10, #0
	mov r11, #0
	mov r12, #0
	mov sp, #0
	mov lr, #0

	cps #MODE32_irq
	mov sp, #0
	mov lr, #0

	cps #MODE32_fiq
	mov r8, #0
	mov r9, #0
	mov r10, #0
	mov r11, #0
	mov r12, #0
	mov sp, #0
	mov lr, #0

	cps #MODE32_abt
	mov sp, #0
	mov lr, #0

	cps #MODE32_und
	mov sp, #0
	mov lr, #0

	cps #MODE32_mon
	mov sp, #0
	mov lr, #0

	cps #MODE32_svc

	mov r0, #(0xFF <<20)
	mcr p15, 0, r0, c1, c0, 2
	mrc p15, 0, r0, c1, c0, 2
	ubfx r0, r0, #20, #1
	cmp r0, #1
	bne ttb_setup

	mrc p15, 0, r1, c1, c0, 2
	orr r1, r1, #(0xF <<20)
	mcr p15, 0, r1, c1, c0, 2
	mov r1, #(0x1<<30)
	isb
	vmsr fpexc, r1

	mov r1, #0
	mov r2, #0
	vmov.f64	d0, r1, r2
	vmov.f64	d1, d0
	vmov.f64	d2, d0
	vmov.f64	d3, d0
	vmov.f64	d4, d0
	vmov.f64	d5, d0
	vmov.f64	d6, d0
	vmov.f64	d7, d0
	vmov.f64	d8, d0
	vmov.f64	d9, d0
	vmov.f64	d10, d0
	vmov.f64	d11, d0
	vmov.f64	d12, d0
	vmov.f64	d13, d0
	vmov.f64	d14, d0
	vmov.f64	d15, d0
	vmov.f64	d16, d0
	vmov.f64	d17, d0
	vmov.f64	d18, d0
	vmov.f64	d19, d0
	vmov.f64	d20, d0
	vmov.f64	d21, d0
	vmov.f64	d22, d0
	vmov.f64	d23, d0
	vmov.f64	d24, d0
	vmov.f64	d25, d0
	vmov.f64	d26, d0
	vmov.f64	d27, d0
	vmov.f64	d28, d0
	vmov.f64	d29, d0
	vmov.f64	d30, d0
	vmov.f64	d31, d0

ttb_setup:

/* ---------------------------------------------------------------------
* If the reset address is programmable then bl1_entrypoint() is
* executed only on the cold boot path. Therefore, we can skip the warm
* boot mailbox mechanism.
* ---------------------------------------------------------------------
*/
	el3_entrypoint_common					\
		_init_sctlr=1					\
		_warm_boot_mailbox=!PROGRAMMABLE_RESET_ADDRESS	\
		_secondary_cold_boot=!COLD_BOOT_SINGLE_CPU	\
		_init_memory=1					\
		_init_c_runtime=1				\
		_exception_vectors=bl1_vector_table

	/* -----------------------------------------------------
	 * Perform BL1 setup
	 * -----------------------------------------------------
	 */
	bl	bl1_setup

	/* -----------------------------------------------------
	 * Jump to main function.
	 * -----------------------------------------------------
	 */
	bl	bl1_main

	/* -----------------------------------------------------
	 * Jump to next image.
	 * -----------------------------------------------------
	 */

	/*
	 * Get the smc_context for next BL image,
	 * program the gp/system registers and save it in `r4`.
	 */
	bl	smc_get_next_ctx
	mov	r4, r0

	/* Only turn-off MMU if going to secure world */
	ldr	r5, [r4, #SMC_CTX_SCR]
	tst	r5, #SCR_NS_BIT
	bne	skip_mmu_off

	/*
	 * MMU needs to be disabled because both BL1 and BL2/BL2U execute
	 * in PL1, and therefore share the same address space.
	 * BL2/BL2U will initialize the address space according to its
	 * own requirement.
	 */
	bl	disable_mmu_icache_secure
	stcopr	r0, TLBIALL
	dsb	sy
	isb

skip_mmu_off:
	/* Restore smc_context from `r4` and exit secure monitor mode. */
	mov	r0, r4
	monitor_exit
endfunc bl1_entrypoint
