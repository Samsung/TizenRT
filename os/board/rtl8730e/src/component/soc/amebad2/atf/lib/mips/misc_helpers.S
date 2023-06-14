/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Cortina Access. All rights reserved.
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
#include <asm_macros.S>
#include <assert_macros.S>

#define PTR_SIZE 4

	.weak zeromem16
	.weak memcpy16
	.global zeromem
	.global zero_normalmem
	.equ zeromem, zeromem16
	.equ zero_normalmem, zeromem16

	.set  noreorder

/* -----------------------------------------------------------------------
 * void zeromem16(void *mem, unsigned int length);
 *
 * Initialise a memory region to 0.
 * The memory address must be 16-byte aligned.
 * -----------------------------------------------------------------------
 */
func zeromem16
	/* t0 = start +  len */
	add  $t0, $a0,$a1
	/* if mem > start + len, done */
	bgt  $a0, $t0, 2f
	nop
1:
	sw   $zero, 0($a0)
	addi $a0, $a0, PTR_SIZE
	blt  $a0, $t0, 1b
	nop
2:
	jr   $ra
	nop
endfunc zeromem16

/* --------------------------------------------------------------------------
 * void memcpy16(void *dest, const void *src, unsigned int length)
 *
 * Copy length bytes from memory area src to memory area dest.
 * The memory areas should not overlap.
 * Destination and source addresses must be 16-byte aligned.
 * --------------------------------------------------------------------------
 */
func memcpy16
	add  $t0, $a0, $a2

	/* length < 1,done */
	li   $t1, 1
	blt  $a2, $t1, 2f
	nop
1:
	lw   $t1, 0($a1)
	nop
	sw   $t1, 0($a0)
	addi $a0, PTR_SIZE
	addi $a1, PTR_SIZE
	blt  $a0, $t0,1b
	nop
2:
	jr  $ra
	nop
endfunc memcpy16
