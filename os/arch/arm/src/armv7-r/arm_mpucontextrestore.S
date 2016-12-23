/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/irq.h>
#include "up_internal.h"
#include "svcall.h"
#include "mpu.h"

	.file	"arm_mpucontextrestore.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/
	.globl	up_mpucontextrestore

	.syntax	unified
	.file	"arm_mpucontextrestore.S"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: up_mpucontextrestore
 *
 * Description:
 *   Restore the specified task mpu context context. Full prototype is:
 *
 *     void up_mpucontextrestore(uint32_t *restoreregs) noreturn_function;
 *
 * Return:
 *   None
 *
 ****************************************************************************/

	.globl	up_mpucontextrestore
	.type	up_mpucontextrestore, function

up_mpucontextrestore:

	/* On entry, a1 (r0) holds address of the register save area.  All other
	 * registers are available for use.
	 *
	 * restore the MPU context information 
	 */
	mov		r3, #0
	mov 	r5, #MPU_REG_USER_CONFIG1
1:
	mcr     p15, 0, r5, c6, c2, 0
	add     r6,  r0, #(4*REG_RNUM)
	str     r5, [r6, r3]
	mcr 	p15, 0, r6, c6, c1, 2

	add     r6, r0, #(4*REG_RBASE)
	ldr     r2, [r6, r3]
	mcr 	p15, 0, r2, c6, c1, 0

	add     r6, r0, #(4*REG_RSIZE)
	ldr     r2, [r6, r3]
	mcr 	p15, 0, r2, c6, c1, 2

	add     r6, r0, #(4*REG_RATTR)
	ldr     r2, [r6, r3]
	mcr 	p15, 0, r2, c6, c1, 4

	add		r3, #16 /* offset to next mpu region context */
	add 	r5, #1
	cmp		r3, #MPU_CONTEXT_SIZE
	bne		1b
	mov		pc, lr
	.size up_mpucontextrestore, . - up_mpucontextrestore
	.end
