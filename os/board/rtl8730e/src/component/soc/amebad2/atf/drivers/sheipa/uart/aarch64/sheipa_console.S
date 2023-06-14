/*
 * Copyright (c) 2016-2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <arch.h>
#include <asm_macros.S>
#include <assert_macros.S>
#include <console_macros.S>
#include <sheipa_console.h>


	.globl	console_core_init
	.globl	console_core_putc
	.globl	console_core_getc
	.globl	console_core_flush

	/* -----------------------------------------------
	 * int console_core_init(unsigned long base_addr,
	 * unsigned int uart_clk, unsigned int baud_rate)
	 * Function to initialize the console without a
	 * C Runtime to print debug information. This
	 * function will be accessed by console_init and
	 * crash reporting.
	 * We assume that the bootloader already set up
	 * the HW (baud, ...) and only enable the trans-
	 * mitter and receiver here.
	 * In: x0 - console base address
	 *     w1 - Uart clock in Hz
	 *     w2 - Baud rate
	 * Out: return 1 on success else 0 on error
	 * Clobber list : x1, x2, x3
	 * -----------------------------------------------
	 */
func console_core_init
	/* Check the input base address */
	cbz	x0, core_register_fail
	/* Check baud rate and uart clock for sanity */
	cbz	w1, core_register_fail
	cbz	w2, core_register_fail

	/* uart init setting */
	ldr	w3, =0x80
	str	w3, [x0,#0xc]
	ldr	w3, =0x0
	str	w3, [x0, #0x4]
	ldr	w3, =0x1b	/* Baudrate 57600 */
	str	w3, [x0, #0x0]
	ldr	w3, =0x3
	str	w3, [x0, #0xc]

	/* end uart init setting */
	mov	w0, #1
	ret
core_register_fail:
	mov	w0, wzr
	ret
endfunc console_core_init

	.globl console_core_register

	/* --------------------------------------------------
	 * int console_core_register(uintptr_t baseaddr,
	 *     uint32_t clock, uint32_t baud,
	 *     console_core_t *console);
	 * Function to initialize and register a new console.
	 * Storage passed in for the console struct *must*
	 * be persistent (i.e. not from the stack).
	 * In: x0 - UART register base address
	 *     w1 - UART clock in Hz
	 *     w2 - Baud rate
	 *     x3 - pointer to empty console_core_t struct
	 * Out: return 1 on success, 0 on error
	 * Clobber list : x0, x1, x2, x6, x7, x14
	 * --------------------------------------------------
	 */
func console_core_register
	mov	x7, x30
	mov	x6, x3
	cbz	x6, register_fail
	str	x0, [x6, #CONSOLE_T_UART_BASE]

	bl	console_core_init
	cbz	x0, register_fail

	mov	x0, x6
	mov	x30, x7
	finish_console_register core putc=1, getc=1, flush=1

register_fail:
	ret	x7
endfunc console_core_register

	/* --------------------------------------------------------
	 * int console_core_putc(int c, console_core_t *console)
	 * Function to output a character over the console. It
	 * returns the character printed on success or -1 on error.
	 * In : w0 - character to be printed
	 *      x1 - pointer to console_t structure
	 * Out : return -1 on error else return character.
	 * Clobber list : x2
	 * --------------------------------------------------------
	 */
func console_core_putc
#if ENABLE_ASSERTIONS
	cmp	x1, #0
	ASM_ASSERT(ne)
#endif /* ENABLE_ASSERTIONS */
	ldr	x1, [x1, #CONSOLE_T_UART_BASE]
	/* Check the input parameter */
	cbz	x1, putc_error
	/* Prepend '\r' to '\n' */
	cmp	w0, #0xA
	b.ne	2f
1:
	ldr	w2, [x1, #0x14]
	mov	w4, #0x20
	ANDS	w2, w2, w4
	beq	1b
	mov	w2, #0xD
	str	w2, [x1]
2:
	ldr	w2, [x1, #0x14]
	mov	w4, #0x20
	ANDS	w2, w2, w4
	beq	2b
	str	w0, [x1]
	ret
putc_error:
	mov	w0, #-1
	ret
endfunc console_core_putc

	/* ---------------------------------------------
	 * int console_core_getc(console_core_t *console)
	 * Function to get a character from the console.
	 * It returns the character grabbed on success
	 * or -1 if no character is available.
	 * In : x0 - pointer to console_t structure
	 * Out: w0 - character if available, else -1
	 * Clobber list : x0, x1
	 * ---------------------------------------------
	 */

func console_core_getc
#if ENABLE_ASSERTIONS
	cmp	x0, #0
	ASM_ASSERT(ne)
#endif /* ENABLE_ASSERTIONS */
	ldr	x0, [x0, #CONSOLE_T_UART_BASE]
	cbz	x0, getc_error
	ret
getc_error:
	mov	w0, #-1
	ret
endfunc console_core_getc
	/* ---------------------------------------------
	 * int console_core_flush(console_core_t *console)
	 * Function to force a write of all buffered
	 * data that hasn't been output.
	 * In : x0 - pointer to console_t structure
	 * Out : return -1 on error else return 0.
	 * Clobber list : x0, x1
	 * ---------------------------------------------
	 */
func console_core_flush
	/* Placeholder */
	mov	w0, #0
	ret
endfunc console_core_flush
