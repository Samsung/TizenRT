/*
 * Copyright (c) 2020 Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <assert_macros.S>
#include <platform_def.h>

	.globl	plat_my_core_pos
	.globl	plat_get_my_entrypoint
	.globl	platform_mem_init
	.globl	plat_sheipa_calc_core_pos
	.globl	plat_crash_console_init
	.globl	plat_crash_console_putc
	.globl	plat_crash_console_flush
	.globl	plat_secondary_cold_boot_setup
	.globl	plat_get_my_entrypoint
	.globl	plat_is_my_cpu_primary


func plat_my_core_pos
	ldcopr	r0, MPIDR
	b	plat_sheipa_calc_core_pos
endfunc plat_my_core_pos
#if CPU_ARMv8_2
/*
 *  unsigned int plat_sheipa_calc_core_pos(u_register_t mpidr);
 *  With this function: CorePos = (ClusterId * 4) + CoreId
 */
func plat_sheipa_calc_core_pos
	lsr	r0, r0, #8
	and	r1, r0, #MPIDR_CPU_MASK
	and	r0, r0, #MPIDR_CLUSTER_MASK
	add	r0, r1, r0, LSR #5
	bx	lr
endfunc plat_sheipa_calc_core_pos
#else
/*
 *  unsigned int plat_sheipa_calc_core_pos(u_register_t mpidr);
 *  With this function: CorePos = (ClusterId * 4) + CoreId
 */
func plat_sheipa_calc_core_pos
	and	r1, r0, #MPIDR_CPU_MASK
	and	r0, r0, #MPIDR_CLUSTER_MASK
	add	r0, r1, r0, LSR #6
	bx	lr
endfunc plat_sheipa_calc_core_pos
#endif
	/* -----------------------------------------------------
	 * unsigned int plat_is_my_cpu_primary (void);
	 *
	 * Find out whether the current cpu is the primary
	 * cpu.
	 * -----------------------------------------------------
	 */
func plat_is_my_cpu_primary
	ldcopr	r0, MPIDR
	ldr	r1, =(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK)
	and	r0, r0, r1
	cmp	r0, #SHEIPA_PRIMARY_CPU
	moveq	r0, #1
	movne	r0, #0
	bx	lr
endfunc plat_is_my_cpu_primary

	/* -----------------------------------------------------
	 * void plat_secondary_cold_boot_setup (void);
	 *
	 * This function performs any platform specific actions
	 * needed for a secondary cpu after a cold reset e.g
	 * mark the cpu's presence, mechanism to place it in a
	 * holding pen etc.
	 * -----------------------------------------------------
	 */
func plat_secondary_cold_boot_setup
	/* Calculate address of our hold entry */
	bl	plat_my_core_pos
	mov_imm	r2, PLAT_SHEIPA_HOLD_BASE
	mov	r1, #0x0
	str	r1, [r2, #0x8]
	/* Wait until we have a go */
poll_mailbox:
	ldr	r1, [r2, #0x8]
	cmp	r1, #0
	beq	1f
	mov_imm	r0, PLAT_SHEIPA_TRUSTED_MAILBOX_BASE
	ldr	r1, [r0]
	bx	r1
1:
	wfe
	b	poll_mailbox
endfunc plat_secondary_cold_boot_setup

func plat_get_my_entrypoint
	/* TODO support warm boot */
	mov	r2, lr
	bl	plat_is_my_cpu_primary
	cmp	r0, #0
	beq	1f
	mov_imm	r1, 0x42008264
	mov_imm	r3, 0x10000
	ldr	r4, [r1]
	tst	r4, r3
	beq	1f
	mov_imm	r1, PLAT_SHEIPA_WARM_BOOT_BASE
	ldr	r0, [r1]
	mov	lr, r2
	bx	lr
1:
	mov	r0, #0
	mov	lr, r2
	bx	lr
endfunc plat_get_my_entrypoint

func platform_mem_init
	bx	lr
endfunc platform_mem_init

	/* ---------------------------------------------
	 * int plat_crash_console_init(void)
	 * Function to initialize the crash console
	 * without a C Runtime to print crash report.
	 * Clobber list : r0, r1, r2
	 * ---------------------------------------------
	 */
func plat_crash_console_init
	mov_imm	r0, PLAT_SHEIPA_CRASH_UART_BASE
	mov_imm	r1, PLAT_SHEIPA_CRASH_UART_CLK_IN_HZ
	mov_imm	r2, PLAT_SHEIPA_CONSOLE_BAUDRATE
	b	console_core_init
endfunc plat_crash_console_init

	/* ---------------------------------------------
	 * int plat_crash_console_putc(int c)
	 * Function to print a character on the crash
	 * console without a C Runtime.
	 * Clobber list : r1, r2
	 * ---------------------------------------------
	 */
func plat_crash_console_putc
	mov_imm	r1, PLAT_SHEIPA_CRASH_UART_BASE
	b	console_core_putc
endfunc plat_crash_console_putc

	/* ---------------------------------------------
	 * int plat_crash_console_flush(int c)
	 * Function to force a write of all buffered
	 * data that hasn't been output.
	 * Out : return -1 on error else return 0.
	 * Clobber list : r0, r1
	 * ---------------------------------------------
	 */
func plat_crash_console_flush
	mov_imm	r0, PLAT_SHEIPA_CRASH_UART_BASE
	b	console_core_flush
endfunc plat_crash_console_flush
