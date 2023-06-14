/*
 * Copyright (c) 2021, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of the Realtek nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define __OPT_BIG_BLOCK_SIZE (4 * 16)
#define __OPT_MID_BLOCK_SIZE (16)
#define BEGIN_UNROLL_BIG_BLOCK \
  .irp offset, 16,16,16,16
#define BEGIN_UNROLL_MID_BLOCK \
  .irp offset, 16
#define END_UNROLL .endr

#ifdef __ARM_REAL_M300_MODEL
#define ALIGN_BYTE 8
#else
#define ALIGN_BYTE 16
#endif

	.syntax unified
	.text
	.align	2
	.global	memcpy_mve
	.thumb
	.thumb_func
	.type	memcpy_mve, %function
memcpy_mve:
	@ r0: dst
	@ r1: src
	@ r2: len
	push	{r0, lr}

	/* Go to big block if r2 <= 64. It is faster */
	cmp	r2, #64
	bls	.Lbig_block
	/* If src is aligned, just go to the big block loop.  */
	ands	r3, r1, ALIGN_BYTE - 1
	beq	.Lbig_block

	/* Align dst only, not trying to align src. */

	ands	r3, r0, ALIGN_BYTE -1
	beq	.Lbig_block

	/* align dst */
	rsb	r3, #16
	subs	r2, r3
	VCTP.8	r3
	VPSTT
	vldrbt.u8	q0, [r1, #0]
	vstrbt.8	q0, [r0, #0]
	adds	r0, r3
	adds	r1, r3

	/* 64 bytes */
.Lbig_block:
	/* preprocessing */
	/* r3 contain lower 6 bits of length, and r2 contain remainder */
	and	r3, r2, __OPT_BIG_BLOCK_SIZE - 1 // midblock offset
	cmp	r2, __OPT_BIG_BLOCK_SIZE
	blt	.Lmid_block
	lsr	r2, r2, 6 // 512 byte per iteration
	WLS	LR, r2, .Lmid_block
	/* Kernel loop for big block copy */
	.align 2
.Lbig_block_loop:
	BEGIN_UNROLL_BIG_BLOCK
	vldrb.u8	q0, [r1], #\offset
	vstrb.8	q0, [r0], #\offset
	END_UNROLL
	LE	LR, .Lbig_block_loop

	/* 16 bytes */
.Lmid_block:
	WLSTP.8	LR, r3, .Ldone
	/* Kernel loop for mid-block copy */
	.align 2
.Lmid_block_loop:
	BEGIN_UNROLL_MID_BLOCK
	vldrb.u8	q0, [r1], #\offset
	vstrb.8	q0, [r0], #\offset
	END_UNROLL
	LETP	LR, .Lmid_block_loop

.Ldone:
	pop	{r0, lr}
	bx	lr


	.size	memcpy_mve, .-memcpy_mve
