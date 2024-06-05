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
static volatile uint32_t g_cpugating_flag[CONFIG_SMP_NCPUS];

void up_set_gating_flag_status(uint32_t CoreID, uint32_t val)
{
	g_cpugating_flag[CoreID] = val;
	ARM_DSB();
	/* Flag already reach 0 */
	if (!g_cpugating_flag[CoreID]) {
		SP_SEV();
	}
}

uint32_t up_get_gating_flag_status(uint32_t CoreID)
{
	return g_cpugating_flag[CoreID];
}
/****************************************************************************
 * Name: arm_gating_handler
=======
 * Private Functions
 ****************************************************************************/
#define portCPU_IRQ_DISABLE()										\
	__asm volatile ( "CPSID i" ::: "memory" );						\
	__asm volatile ( "DSB" );										\
	__asm volatile ( "ISB" );
/****************************************************************************
 * Public Functions
 ****************************************************************************/
uint32_t ulPortInterruptLock(void)
{
	uint32_t key;

	__asm volatile("mrs	%0, cpsr	\n":"=r"(key)::"memory");
	portCPU_IRQ_DISABLE();

	return key;
}

/*-----------------------------------------------------------*/

void ulPortInterruptUnLock(uint32_t key)
{
	__asm volatile("msr	cpsr_c, %0	\n"::"r"(key):"memory");
}

/****************************************************************************
 * Name: arm_flash_handler
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
	int cpu = this_cpu();
	uint32_t PrevIrqStatus = irqsave();
	g_cpugating_flag[cpu]++;
	ARM_DSB();
	ARM_ISB();
	while (g_cpugating_flag[cpu]) {
		SP_WFE();
	}
	irqrestore(PrevIrqStatus);

	return OK;
}

/****************************************************************************
 * Name: up_cpu_gating
 *
 * Description:
 *   Send signal for target CPU to enter gating.
 *
 * Input Parameters:
 *   cpu - The index of the CPU being gated.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void up_cpu_gating(int cpu)
{
	DEBUGASSERT(cpu >= 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

	/* Fire SGI for cpu to enter gating */
	arm_cpu_sgi(GIC_IRQ_SGI3, (1 << cpu));
}

#endif /* CONFIG_CPU_GATING */
