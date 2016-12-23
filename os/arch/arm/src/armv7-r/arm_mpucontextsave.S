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

	.file	"arm_mpucontextsave.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/
	.globl	up_mpucontextsave

	.syntax	unified
	.file	"arm_mpucontextsave.S"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

	.text

/****************************************************************************
 * Name: up_mpucontextsave
 *
 * Description:
 *   Save the specified task mpu context context. Full prototype is:
 *
 *     void up_mpucontextsave(uint32_t *restoreregs) noreturn_function;
 *
 * Return:
 *   None
 *
 ****************************************************************************/

	.globl	up_mpucontextsave
	.type	up_mpucontextsave, function

up_mpucontextsave:

	/* On entry, a1 (r0) holds address of the register save area.  All other
	 * registers are available for use.
	 *
	 * save the MPU context information 
	 */
	mov		r6, #0
	mov 	r5, #MPU_REG_USER_CONFIG1
1:
	mcr     p15, 0, r5, c6, c2, 0
	add     r1,  r0, #(4*REG_RNUM)
	str     r5, [r1, r6]

	mrc 	p15, 0, r7, c6, c1, 0
	add     r1, r0, #(4*REG_RBASE)
	str     r7, [r1, r6]

	mrc 	p15, 0, r7, c6, c1, 2
	add     r1, r0, #(4*REG_RSIZE)
	str     r7, [r1, r6]

	mrc 	p15, 0, r7, c6, c1, 4
	add     r1, r0, #(4*REG_RATTR)
	str     r7, [r1, r6]

	add		r6, #16
	add 	r5, #1
	cmp		r6, #MPU_CONTEXT_SIZE
	bne		1b
	mov		pc, lr

	.size up_mpucontextsave, . - up_mpucontextsave
	.end
