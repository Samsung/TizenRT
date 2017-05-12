/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <chip/mpu-reg.h>
#include "up_internal.h"
#include "svcall.h"
#include "mpu.h"

	.file	"arm_mpuguardsave.S"

	.globl  up_mpuguardsave
	.type	up_mpuguardsave, function
up_mpuguardsave:
	push	{lr}
	mov     r2, #MPU_REG_STK_GUARD
	mcr     p15, 0, r2, c6, c2, 0
	add     r1,  r0, #(4*REG_RNUM_STKGUARD)
	str     r2, [r1]

	mrc     p15, 0, r2, c6, c1, 0
	add     r1, r0, #(4*REG_RBASE_STKGUARD)
	str     r2, [r1]

	mrc     p15, 0, r2, c6, c1, 2
	add     r1, r0, #(4*REG_RSIZE_STKGUARD)
	str     r2, [r1]

	mrc     p15, 0, r2, c6, c1, 4
	add     r1, r0, #(4*REG_RATTR_STKGUARD)
	str     r2, [r1]
	pop		{lr}
	mov     pc, lr
	.size 	up_mpuguardsave, . - up_mpuguardsave
	.end
