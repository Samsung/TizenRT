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

/* This memset is almost the same as __aeabi_memset in __aeabi_memset-mve.S,
   except that memset should return r0 and have different argument order.   */
#if (defined (__ARM_FEATURE_MVE))
#define PTR r0
#define LEN r2
#define VAL r1

#ifdef __ARM_REAL_M300_MODEL
#define ALIGN_BYTE 8
#else
#define ALIGN_BYTE 16
#endif

	.thumb
	.syntax unified
	.global memset_mve
	.type	memset_mve, %function
memset_mve:
	push	{lr}
	vdup.8	q0, VAL
	mov	VAL, r0 // used for temp

	/* if len is less than 16, just go and do one iteration */
	cmp	LEN, #16
	bls	.Lloop_start
	/* if src is aligned, just go to the mid block loop.  */
	ands	r3, PTR, ALIGN_BYTE-1
	beq	.Lloop_start

	/* Count remaining byte and do the needed number of lanes*/
	rsb	r3, ALIGN_BYTE
	vctp.8	r3
	vpst
	vstrbt.8	q0, [PTR, #0]
	adds	PTR, r3
	subs	LEN, r3

// 16 bytes
.Lloop_start:
	WLSTP.8	LR, LEN, .Lloop_end
	/* Kernel loop for mid-block copy */
	.align 2
.Lloop_body:
	vstrb.8	q0, [PTR], #16
	LETP	LR, .Lloop_body
.Lloop_end:
	mov	r0, VAL
	pop	{pc}
	.size memset_mve, . - memset_mve
#endif
