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
	 * In: r0 - console base address
	 *     r1 - Uart clock in Hz
	 *     r2 - Baud rate
	 * Out: return 1 on success else 0 on error
	 * Clobber list : r1, r2, r3
	 * -----------------------------------------------
	 */
func console_core_init
	/* Check the input base address */
	cmp	r0, #0
	beq	core_register_fail
	/* Check baud rate and uart clock for sanity */
	cmp	r1, #0
	beq	 core_register_fail
	cmp	r2, #0
	beq	core_register_fail

	/* uart init setting, open AP log path */
	ldr	r3, [r0, #0x58]
	orr	r3,r3, #(0x1 << 30)
	str	r3, [r0, #0x58]

	/* end uart init setting */
	mov	r0, #1
	bx	lr
core_register_fail:
	mov	r0, #0
	bx	lr
endfunc console_core_init

	.globl console_core_register

	/* --------------------------------------------------
	 * int console_core_register(uintptr_t baseaddr,
	 *     uint32_t clock, uint32_t baud,
	 *     console_core_t *console);
	 * Function to initialize and register a new console.
	 * Storage passed in for the console struct *must*
	 * be persistent (i.e. not from the stack).
	 * In: r0 - UART register base address
	 *     r1 - UART clock in Hz
	 *     r2 - Baud rate
	 *     r3 - pointer to empty console_core_t struct
	 * Out: return 1 on success, 0 on error
	 * Clobber list : r0, r1, r2
	 * --------------------------------------------------
	 */
func console_core_register
	push	{r4, lr}
	mov	r4, r3
	cmp	r4, #0
	beq register_fail
	str	r0, [r4, #CONSOLE_T_UART_BASE]

	bl	console_core_init
	cmp	r0, #0
	beq register_fail

	mov	r0, r4
	pop	{r4, lr}
	finish_console_register core putc=1, getc=1, flush=1

register_fail:
	pop	{r4, pc}
endfunc console_core_register

	/* --------------------------------------------------------
	 * int console_core_putc(int c, console_core_t *console)
	 * Function to output a character over the console. It
	 * returns the character printed on success or -1 on error.
	 * In : r0 - character to be printed
	 *      r1 - pointer to console_t structure
	 * Out : return -1 on error else return character.
	 * Clobber list : r2
	 * --------------------------------------------------------
	 */
func console_core_putc
#if ENABLE_ASSERTIONS
	cmp	r1, #0
	ASM_ASSERT(ne)
#endif /* ENABLE_ASSERTIONS */
	ldr	r1, [r1, #CONSOLE_T_UART_BASE]
	/* Check the input parameter */
	cmp	r1, #0
	beq	putc_error
	/* Prepend '\r' to '\n' */
	cmp	r0, #0xA
	bne	2f
1:
	ldr	r2, [r1, #0x14]	/* LSR[23]: TP4F_NOT_FULL */
	tst	r2, #0x800000
	beq	1b
	mov	r2, #0xD
	str	r2, [r1, #0x68]	/* UART transmitter holding register 4: FIFO entry for Cortex-A*/
2:
	ldr	r2, [r1, #0x14]
	tst	r2, #0x800000
	beq	2b
	str	r0, [r1, #0x68]
	bx	lr
putc_error:
	mov	r0, #-1
	bx	lr
endfunc console_core_putc

	/* ---------------------------------------------
	 * int console_core_getc(console_core_t *console)
	 * Function to get a character from the console.
	 * It returns the character grabbed on success
	 * or -1 if no character is available.
	 * In : r0 - pointer to console_t structure
	 * Out: r0 - character if available, else -1
	 * Clobber list : r0, r1
	 * ---------------------------------------------
	 */

func console_core_getc
#if ENABLE_ASSERTIONS
	cmp	r0, #0
	ASM_ASSERT(ne)
#endif /* ENABLE_ASSERTIONS */
	ldr	r0, [r0, #CONSOLE_T_UART_BASE]
	cmp	r0, #0
	beq	getc_error
	bx	lr
getc_error:
	mov	r0, #-1
	bx	lr
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
	mov	r0, #0
	bx	lr
endfunc console_core_flush
