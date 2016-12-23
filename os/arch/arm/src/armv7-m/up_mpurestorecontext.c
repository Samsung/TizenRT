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

#include <sys/types.h>
#include <arch/irq.h>
#include "mpu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_mpurestorecontext
 *
 * Description:
 *   Restore the specified task mpu context. MPU's region start address is the
 *   input address that is passed to this function.
 *
 * Full prototype is:
 *     void up_mpurestorecontext(const uint32_t *restoreregs) noreturn_function;
 *
 * Return:
 *   None
 *
 ****************************************************************************/

void up_mpurestorecontext(const uint32_t *regs)
{
	int regns;
	uint32_t *from = (uint32_t *)(&regs[REG_RNUM]);

	for (regns = MPU_REG_USER_CONFIG1; regns < MPU_REG_KERN_PERI; regns++) {
		putreg32(regns, MPU_RNR);
		putreg32(*from++, MPU_RNR);
		putreg32(*from++, MPU_RBAR);
		putreg32(*from++, MPU_RASR);
	}

}
