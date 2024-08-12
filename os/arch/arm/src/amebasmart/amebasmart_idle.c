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
 *   wakeuphandler - The wakeuphandler function that must be called after each board wakeup.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void up_pm_board_sleep(void (*wakeuphandler)(clock_t, pm_wakeup_reason_code_t))
{
	uint32_t xModifiableIdleTime = 0;
	irqstate_t flags;
	int ret;
	/* TODO: When enabling SMP, PM state coherency should be verified for 
		primary and secondary cores. Each of the cores has it's own idle task,
		need to make sure both cores handle their responsibilities correctly,
		EG: Secondary core should be in hotplug mode, primary core should check
		the secondary core state before going to sleep
	*/
	if (up_cpu_index() == 0) {
#ifdef CONFIG_SMP
/* For CPU_RUNNING case 
CPU_RUNNING:
Send SGI for core 1 to do task migration and shut down, we will expect that to be handled
nicely before the next cycle of core 0 reaches
*/
RESLEEP_1:
#endif
		/* mask sys tick interrupt*/
		arm_arch_timer_int_mask(1);
		up_timer_disable();
		flags = irqsave();
		/* TODO: If there is an interrupt happening here, what do we expect to happen?
			If it is one of the wakeup sources, it will recognized but will not be serviced
			If it is not one of the wakeup sources, it will not be recognized at all
		*/
#ifdef CONFIG_SMP
/* For CPU_WAKE_FROM_SLEEP case 
CPU_WAKE_FROM_SLEEP:
Theoretically, we will not expect PM state to drop to sleep again in such short time span
(ie. The time interval between core0 trigger core 1 to wakeup)
But in case that happens, we have to introduce some strategy to handle that
*/
RESLEEP_2:
		/*PG flow */
		if (pmu_get_sleep_type() == SLEEP_PG) {
			/* CPU1 is in task schedular, tell CPU1 to enter hotplug */
			if (up_get_secondary_cpu_state(1) == CPU_RUNNING) {
				/* CPU1 may in WFI idle state. Wake it up to enter hotplug itself */
				up_irq_enable();
				arm_cpu_sgi(GIC_IRQ_SGI4, (1 << 1));
				arm_arch_timer_int_mask(0);
				up_timer_enable();
				/* TODO: Remove DelayUs(100) to enable quick sleep in PM for SMP enable case */
				DelayUs(100);
				goto RESLEEP_1;
			}
			/* CPU1 just come back from pg, so can't sleep here */
			if (up_get_secondary_cpu_state(1) == CPU_WAKE_FROM_SLEEP) {
				pmdbg("Secondary core just woke from PG sleep!\n");
				goto RESLEEP_2;
			}
			/* CG flow */
		} else {
			if (!check_wfi_state(1)) {
				goto EXIT;
			}
		}
#endif
		/* Interrupt source will wake cpu up, just leave expected idle time as 0
		Enter sleep mode for AP */
		configPRE_SLEEP_PROCESSING(xModifiableIdleTime, wakeuphandler);
		/* When wake from pg, arm timer has been reset, so a new compare value is necessary to
		trigger an timer interrupt */
		if (pmu_get_sleep_type() == SLEEP_PG) {
			up_timer_enable();
			arm_arch_timer_set_compare(arm_arch_timer_count() + 50000);
		}
		arm_arch_timer_int_mask(0);
		configPOST_SLEEP_PROCESSING(xModifiableIdleTime);
#ifdef CONFIG_SMP
EXIT:
#endif				
		/* Re-enable interrupts and sys tick*/
		up_irq_enable();
	}
}
#else
#define up_pm_board_sleep(wakeuphandler)
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
#ifdef CONFIG_SMP
	/* set core to WFE */
	__asm("WFE");
#else
	/* power saving when idle*/
	__asm(" DSB");
	__asm(" WFI");
	__asm(" ISB");
#endif
#endif
}

#ifdef CONFIG_PM
void arm_pminitialize(void)
{
	/* Then initialize the TinyAra power management subsystem properly */
	pm_initialize();
}
#endif
