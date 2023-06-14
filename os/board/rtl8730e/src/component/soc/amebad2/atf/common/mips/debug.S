/*
 * Copyright (c) 2014-2016, ARM Limited and Contributors. All rights reserved.
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

#include <arch.h>
#include <asm_macros.S>

	.globl	asm_print_str
	.globl	asm_print_hex
	.globl	asm_assert
	.globl	do_panic
	.globl	asm_print_hex_32

	.set    noreorder
/* Since the max decimal input number is 65536 */
#define MAX_DEC_DIVISOR		10000
/* The offset to add to get ascii for numerals '0 - 9' */
#define ASCII_OFFSET_NUM	0x30

#if ASM_ASSERTION
	.section .rodata.assert_str, "aS"
assert_msg1:
	.asciz "ASSERT: File "
assert_msg2:
	.asciz " Line "

	/*
	 * This macro is intended to be used to print the
	 * line number in decimal. Used by asm_assert macro.
	 * The max number expected is 65536.
	 * In: s2 = the decimal to print.
	 * Clobber:
	 */
.macro asm_print_line_dec
	li		$s4,10		/* Divide by 10 after every loop iteration */
	li		$s5,MAX_DEC_DIVISOR
dec_print_loop:
	divu	$s2,$s5
	mflo	$s0				/* Get the quotient */
	mfhi	$s2				/* Find the remainder */
	addi	$a0,$s0,ASCII_OFFSET_NUM	/* Convert to ascii */
	jal	plat_crash_console_putc
	nop
	divu	$s5,$s4
	mflo	$s5				/* Reduce divisor */
	bne	    $s5,$zero, dec_print_loop
	nop
.endm


/* ---------------------------------------------------------------------------
 * Assertion support in assembly.
 * The below function helps to support assertions in assembly where we do not
 * have a C runtime stack. Arguments to the function are :
 * a0 - File name
 * a1- Line no
 * Clobber list :
 * ---------------------------------------------------------------------------
 */
func asm_assert
	move	$s0,$a0
	move	$s1,$a1
	jal	plat_crash_console_init
	nop
	/* Ensure the console is initialized */
	/* Check if the console is initialized */
	beq	$v0,$zero, _assert_loop
	nop
	/* The console is initialized */
	la	$s2,assert_msg1
	jal	asm_print_str
	nop
	move	$s2,$s0
	jal	asm_print_str
	nop
	la	$s2,assert_msg2
	jal	asm_print_str
	nop
	/* Check if line number higher than max permitted */
	li	$t0,0xffff
	bgt	$s1,$t0,_assert_loop
	nop
	move	$s2,$s1
	asm_print_line_dec
_assert_loop:
	j	_assert_loop
	nop
endfunc asm_assert
#endif

/*
 * This function prints a string from address in s2.
 * In: s2 = pointer to string.
 * Clobber:
 */
func asm_print_str
	move	$s3, $ra
1:
	lb	$a0,0($s2)
	nop
	addi	$s2,$s2,1
	beq	$a0,$zero,2f
	nop
	jal	plat_crash_console_putc
	nop
	j	1b
	nop
2:
	jr	$s3
	nop
endfunc asm_print_str

/*
 * This function prints a hexadecimal number in s2.
 * In: s2 = the hexadecimal to print.
 * Clobber:
 */
func asm_print_hex
	j 	asm_print_hex_32
	nop
endfunc asm_print_hex

/*
 * This function prints a hexadecimal number in s2.
 * In: s2 = the hexadecimal to print.
 * Clobber:
 */
func asm_print_hex_32
	move	$s4,$ra
	li	$s5,32          /* No of bits to convert to ascii */
1:
	li	$t0,4
	subu	$s5,$s5,$t0
	srlv	$a0,$s2,$s5
	andi	$a0,$a0,0xf
	li	$t0,0xA
	blt	$a0,$t0,2f
	nop
	/* Add by 0x27 in addition to ASCII_OFFSET_NUM
	 * to get ascii for characters 'a - f'.
	 */
	addi	$a0,$a0,0x27
2:
	addi	$a0,$a0,ASCII_OFFSET_NUM
	jal	plat_crash_console_putc
	nop
	bne	$s5,$zero,1b
	nop
	jr	$s4
	nop
endfunc asm_print_hex_32
	/***********************************************************
	 * The common implementation of do_panic for all BL stages
	 ***********************************************************/

	.section .rodata.panic_str, "aS"
	panic_msg: .asciz "PANIC at PC : 0x"

/* ---------------------------------------------------------------------------
 * do_panic assumes that it is invoked from a C Runtime Environment ie a
 * valid stack exists. This call will not return.
 * Clobber list :
 * ---------------------------------------------------------------------------
 */

func do_panic
	move	$s0,$ra
	jal	plat_crash_console_init
	nop
	/* Check if the console is initialized */
	beq	$v0,$zero,_panic_handler
	nop
	/* The console is initialized */
	la	$s2,panic_msg
	jal	asm_print_str
	nop

	move	$s2,$s0
	/* The panic location is lr -4 */
	li	$t0,4
	sub	$s2,$s2,$t0
	jal	asm_print_hex
	nop

_panic_handler:
	/*Pass to plat_panic_handler the address from where do_panic was
	 * called. */
	move	$ra,$s0
	j	plat_panic_handler
	nop
endfunc do_panic
