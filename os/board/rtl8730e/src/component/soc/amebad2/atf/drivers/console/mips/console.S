/*
 * Copyright (c) 2016-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017, Realtek. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <asm_macros.S>

	.globl	console_init
	.globl	console_uninit
	.globl	console_putc
	.globl	console_getc
	.globl	console_flush

	/*
	 *  The console base is in the data section and not in .bss
	 *  even though it is zero-init. In particular, this allows
	 *  the console functions to start using this variable before
	 *  the runtime memory is initialized for images which do not
	 *  need to copy the .data section from ROM to RAM.
	 */
.section .data.console_base ; .align 2
	console_base: .word 0x0

	/* -----------------------------------------------
	 * int console_init(uintptr_t base_addr,
	 * unsigned int uart_clk, unsigned int baud_rate)
	 * Function to initialize the console without a
	 * C Runtime to print debug information. It saves
	 * the console base to the data section.
	 * In: a0 - console base address
	 *     a1 - Uart clock in Hz
	 *     a2 - Baud rate
	 * out: return 1 on success else 0 on error
	 * Clobber list : a1 - a3
	 * -----------------------------------------------
	 */
func console_init
	/* Check the input base address */
	beqz	$a0, init_fail
 	la	$a3, console_base
	sw	$a0, 0($a3)
	b	console_core_init
	 nop
init_fail:
	jr	$ra
	 nop
endfunc console_init

	/* -----------------------------------------------
	 * void console_uninit(void)
	 * Function to finish the use of console driver.
	 * It sets the console_base as NULL so that any
	 * further invocation of `console_putc` or
	 * `console_getc` APIs would return error.
	 * -----------------------------------------------
	 */
func console_uninit
	move	$a0, $zero
	la	$a3, console_base
	sw	$a0, 0($a3)
	jr	$ra
	 nop
endfunc console_uninit

	/* ---------------------------------------------
	 * int console_putc(int c)
	 * Function to output a character over the
	 * console. It returns the character printed on
	 * success or -1 on error.
	 * In : a0 - character to be printed
	 * Out : return -1 on error else return character.
	 * Clobber list : a1, a2
	 * ---------------------------------------------
	 */
func console_putc
	la	$a2, console_base
	lw	$a1, 0($a2)
         nop
	b	console_core_putc
         nop
endfunc console_putc

	/* ---------------------------------------------
	 * int console_getc(void)
	 * Function to get a character from the console.
	 * It returns the character grabbed on success
	 * or -1 on error.
	 * Clobber list : a0, a1
	 * ---------------------------------------------
	 */
func console_getc
	la	$a1, console_base
	lw	$a0, 0($a1)
         nop
	b	console_core_getc
         nop
endfunc console_getc

	/* ---------------------------------------------
	 * int console_flush(void)
	 * Function to force a write of all buffered
	 * data that hasn't been output. It returns 0
	 * upon successful completion, otherwise it
	 * returns -1.
	 * Clobber list : a0, a1
	 * ---------------------------------------------
	 */
func console_flush
	la	$a1, console_base
	lw	$a0, 0($a1)
         nop
	b	console_core_flush
         nop
endfunc console_flush
