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

	.thumb
	.syntax unified
def_fn strlen_mve
	.cfi_sections .debug_frame
	.cfi_startproc
	/* @ r0: src */

	vmov.i8	q1, #0
	/* r1 is start address. r0 is end address. */
	mov	r1, r0
	/* check src0 is 64 bits aligned. */
	ands	r2, r0, ALIGN_BYTE -1
	bne	.Lmisaligned

	/* Kernel loop */
	.align 2
.Lbody:
	/* check src != 0. */

	vldrb.u8	q0, [r0], #16
	/* find src != 0 */
	vcmp.i8	eq, q0, q1
	vmrs	r3, p0
	subs	r3, #0
	bne	.Laddress
	b.w .Lbody
.Laddress:
	sub	r0, #16
.Lhaszero:
	/* calculate first element which is zero */
	rbit r3, r3
	clz r3, r3
	add r0, r3
	sub r0, r0, r1

.Ldone:

	bx	lr

.Lmisaligned:
	rsb r2,#16
	vldrb.u8	q0, [r0, #0]
	vcmp.i8	eq, q0, q1
	vmrs	r3, p0
	subs	r3, #0
	bne	.Lhaszero
	add	r0, r2
	b .Lbody
	.cfi_endproc
	.size strlen_mve, . - strlen_mve



