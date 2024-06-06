/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-a/arm_cpugating.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/sched_note.h>

#include "up_internal.h"
#include "cp15_cacheops.h"
#include "gic.h"
#include "sched/sched.h"
#include "barriers.h"

#ifdef CONFIG_CPU_GATING
volatile uint32_t ulFlashPG_Flag = 0;
/****************************************************************************
 * Name: arm_gating_handler
 *
 * Description:
 *   This is the handler for SGI3.  This handler simply send the another core
 *	 to wfe mode, to prevent from hang when flash operation is invoked
 *
 * Input Parameters:
 *   Standard interrupt handling
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
int arm_gating_handler(int irq, void *context, void *arg)
{
	uint32_t PrevIrqStatus = irqsave();
	ulFlashPG_Flag++;
	ARM_DSB();
	ARM_ISB();
	while(ulFlashPG_Flag) {
		__asm__ __volatile__ ("wfe" : : : "memory");
	}
	irqrestore(PrevIrqStatus);

	return OK;
}

#endif /* CONFIG_CPU_GATING */
