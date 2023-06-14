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

#ifdef __ARM_REAL_M300_MODEL
#define ALIGN_BYTE 8
#else
#define ALIGN_BYTE 16
#endif

	.macro def_fn f p2align=0
	.text
	.p2align \p2align
	.global \f
	.type \f, %function
\f:
	.endm

	.text
	.thumb
	.syntax unified
def_fn strcmp_mve
	.cfi_sections .debug_frame
	.cfi_startproc
	/* @ r0: src0 */
	/* @ r1: src1 */
#ifndef STRCMP_NO_PRECHECK
	ldrb	r2, [r0]
	ldrb	r3, [r1]
	cmp	r2, #1
	it	cs
	cmpcs	r2, r3
	bne	.Lfastpath_exit
#endif
	vmov.i8	q2, #0
	/* check src0 is aligned. */
	ands	ip, r0, ALIGN_BYTE -1
	bne.w	.Lmisaligned

	/* Kernel loop */
	.align 2
.Lbody:
	/* Check (src0 != 0 && src0 == src1) */

	vldrb.u8	q0, [r0], #16
	vldrb.u8	q1, [r1], #16
	/* If find element in src0 == 0. Mov the result from p0 to r2 */
	vcmp.i8	eq, q0, q2
	vmrs	r2, p0
	/* If find element in src0 != element in src1. Mov the result from p0 to r3 */
	vcmp.i8	ne, q0, q1
	vmrs	r3, p0
	orrs	r3, r2
	bne	.Laddress
	b.w .Lbody
.Laddress:
	sub	r0, #16
	sub	r1, #16
.Lhaszero:
	/* caluate first element which violate (src0 != 0 && src0 == src1) */
	rbit r3, r3
	clz r3, r3

	/* vmov.u8 r2, q0[r3] is not legal, load from memory instead. */
	ldrb r2, [r0, r3]
	ldrb r3, [r1, r3]
.Lfastpath_exit:
	subs	r0, r2, r3

.Ldone:

	bx	lr

.Lmisaligned:
	tst	r1, ALIGN_BYTE -1
	/* src1 is aligned, go to .Lbody */
	beq	.Lbody
	rsb ip,#16
	vldrb.u8	q0, [r0, #0]
	vldrb.u8	q1, [r1, #0]
	vcmp.i8	eq, q0, q2
	vmrs	r2, p0
	vcmp.i8	ne, q0, q1
	vmrs	r3, p0
	orrs	r3, r2
	bne	.Lhaszero
	add	r0, ip
	add	r1, ip
	b.w .Lbody
	.cfi_endproc
	.size strcmp_mve, . - strcmp_mve



