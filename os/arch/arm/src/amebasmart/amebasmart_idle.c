/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_idle.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#include "amebasmart_config.h"
#include "arch_timer.h"
#include "ameba_soc.h"
#include "osdep_service.h"
#endif

#ifdef CONFIG_SMP
#include "gic.h"
#include "smp.h"
#endif

#ifdef CONFIG_PM
/****************************************************************************
 * Name: up_pm_board_sleep
 *
 * Description:
 *   Perform IDLE state power management.
 * 
 * Input Parameters:
 *   handler - The handler function that must be called after each board wakeup.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void up_pm_board_sleep(void (*handler)(clock_t, pm_wakeup_reason_code_t))
{
	uint32_t xModifiableIdleTime = 0;
	irqstate_t flags;
	/* TODO: When enabling SMP, PM state coherency should be verified for 
		primary and secondary cores. Each of the cores has it's own idle task,
		need to make sure both cores handle their responsibilities correctly,
		EG: Secondary core should be in hotplug mode, primary core should check
		the secondary core state before going to sleep
	*/
	if (up_cpu_index() == 0) {
		/* mask sys tick interrupt*/
		arm_arch_timer_int_mask(1);
		up_timer_disable();
		flags = irqsave();
		if (tizenrt_ready_to_sleep()) {
			/* Interrupt source will wake cpu up, just leave expected idle time as 0
			Enter sleep mode for AP */
			configPRE_SLEEP_PROCESSING(xModifiableIdleTime, handler);
			/* When wake from pg, arm timer has been reset, so a new compare value is necessary to
			trigger an timer interrupt */
			if (pmu_get_sleep_type() == SLEEP_PG) {
				up_timer_enable();
				arm_arch_timer_set_compare(arm_arch_timer_count() + 50000);
			}
			arm_arch_timer_int_mask(0);
			configPOST_SLEEP_PROCESSING(xModifiableIdleTime);
		} 	
		/* Re-enable interrupts and sys tick*/
		up_irq_enable();
	}
}
#else
#define up_pm_board_sleep(handler)
#endif

/****************************************************************************
 * Name: up_idle
 *
 * Description:
 *   up_idle() is the logic that will be executed when there is no other
 *   ready-to-run task.  This is processor idle time and will continue until
 *   some interrupt occurs to cause a context switch from the idle task.
 *
 *   Processing in this state may be processor-specific. e.g., this is where
 *   power management operations might be performed.
 *
 ****************************************************************************/

void up_idle(void)
{
#if defined(CONFIG_SUPPRESS_INTERRUPTS) || defined(CONFIG_SUPPRESS_TIMER_INTS)
	/* If the system is idle and there are no timer interrupts, then process
	 * "fake" timer interrupts. Hopefully, something will wake up.
	 */

	nxsched_process_timer();
#else
	/* set core to WFE */
	__asm("WFE");

#endif
}

#ifdef CONFIG_PM
void arm_pminitialize(void)
{
	/* Then initialize the TinyAra power management subsystem properly */
	pm_initialize();
}
#endif
