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
#endif

/****************************************************************************
 * Name: up_idlepm
 *
 * Description:
 *   Perform IDLE state power management.
 *
 ****************************************************************************/

#ifdef CONFIG_PM
static enum pm_state_e oldstate = PM_NORMAL;
static void up_idlepm(void)
{
	uint32_t xModifiableIdleTime = 0;
	enum pm_state_e newstate;
	irqstate_t flags;
	int ret;

	/* Decide, which power saving level can be obtained */
	newstate = pm_checkstate();

	/* Check if new attainable state is different from current state */
	if (newstate != oldstate)
	{
		/* TODO: SMP yet to be verified*/
		/* Perform board-specific, state-dependent logic here */
	  	pmvdbg("newstate= %d oldstate=%d\n", newstate, oldstate);

		/* Then force the global state change */
		ret = pm_changestate(newstate);
		if (ret < 0) {
			/* The new state change failed, revert to the preceding state */
			pmdbg("State change failed! Current state = %d, newstate = %d\n", oldstate, newstate);
			/* Old state need not to be updated, since the state change is rejected */
			goto REJECTED;
		} else {
			/* Save the new state */
			oldstate = newstate;
		}
		/* MCU-specific power management logic */
		/* TODO: When LCD is integrated, some of the state operation might require revision
		   PM_IDLE: We will expect LCD to go through background light dimming
		   PM_STANDBY: LCD shut down, single core might be able to handle remaining tasks
		   			   Then, we can do vPortSecondaryOff() for SMP case (ie. shutdown secondary core)
		*/
		switch (newstate) {
			case PM_NORMAL:
				/* In PM_NORMAL, we have nothing to do, set core to WFE */
				__asm(" WFE");
				break;
			case PM_IDLE:
				/* In PM_IDLE, we have nothing to do, set core to WFE */
				__asm(" WFE");
				break;
			case PM_STANDBY:
				/* In PM_STANDBY, we have nothing to do, set core to WFE */
				__asm(" WFE");
				break;
			case PM_SLEEP:
				/* TODO: When enabling SMP, PM state coherency should be verified for 
				   primary and secondary cores. Each of the cores has it's own idle task,
				   need to make sure both cores handle their responsibilities correctly,
				   EG: Secondary core should be in hotplug mode, primary core should check
				   the secondary core state before going to sleep
				*/
#ifndef CONFIG_SMP
				up_set_pm_timer();
#endif
				if (up_cpu_index() == 0) {
					/* mask sys tick interrupt*/
					arm_arch_timer_int_mask(1);
					up_timer_disable();
					flags = irqsave();
					/* TODO: If there is an interrupt happening here, what do we expect to happen?
					   If it is one of the wakeup sources, it will recognized but will not be serviced
					   If it is not one of the wakeup sources, it will not be recognized at all
					*/
					if (tizenrt_ready_to_sleep()) {
#ifdef CONFIG_SMP
						/*PG flow */
						if (pmu_get_sleep_type() == SLEEP_PG) {
							/* CPU1 just come back from pg, so can't sleep here */
							if (pmu_get_secondary_cpu_state(1) == CPU1_WAKE_FROM_PG) {
								goto EXIT;
							}

							/* CPU1 is in task schedular, tell CPU1 to enter hotplug */
							if (pmu_get_secondary_cpu_state(1) == CPU1_RUNNING) {
								/* CPU1 may in WFI idle state. Wake it up to enter hotplug itself */
								up_irq_enable();
								//arm_gic_raise_softirq(1, 0);
								arm_arch_timer_int_mask(0);
								DelayUs(100);
								goto EXIT;
							}

							/* For SMP case, timer should be set after confirming secondary core enter hotplug mode */
							if (pmu_get_secondary_cpu_state(1) == CPU1_HOTPLUG) {
								up_set_pm_timer();
							}
							/* CG flow */
						} else {
							if (!check_wfi_state(1)) {
								goto EXIT;
							}
						}
#endif
						/* Interrupt source from BT/UART will wake cpu up, just leave expected idle time as 0
						Enter sleep mode for AP */
						configPRE_SLEEP_PROCESSING(xModifiableIdleTime);
						/* When wake from pg, arm timer has been reset, so a new compare value is necessary to
						trigger an timer interrupt */
						if (pmu_get_sleep_type() == SLEEP_PG) {
							up_timer_enable();
							arm_arch_timer_set_compare(arm_arch_timer_count() + 50000);
						}
						arm_arch_timer_int_mask(0);
						configPOST_SLEEP_PROCESSING(xModifiableIdleTime);
					}
					else {
						/* power saving when idle*/
						arm_arch_timer_int_mask(0);
						__asm(" DSB");
						__asm(" WFI");
						__asm(" ISB");
					}
#ifdef CONFIG_SMP
EXIT:
#endif				
					/* Re-enable interrupts and sys tick*/
					up_irq_enable();
				}
				/* If secondary core idle loop enters here, it should guarantee it has entered hotplug state*/
				else if (up_cpu_index() == 1) {
					if (pmu_get_sleep_type() == SLEEP_PG) {
						if (tizenrt_ready_to_sleep()) {
							/* CPU1 will enter hotplug state. Raise a task yield to migrate its task */
							pmu_set_secondary_cpu_state(1, CPU1_HOTPLUG);
						}
					}

					flags = irqsave();
					__asm("	DSB");
					__asm("	WFI");
					__asm("	ISB");
					up_irq_enable();
#ifdef CONFIG_SMP
					goto EXIT2;
#endif
				}
				/* Note: Wakeup from sleep, change the state back to PM_NORMAL 
				   At this point, we do not need to do anything, as the
				   wakeup callback handler will invoke pm_activity()
				*/

/* TODO: This exit is for secondary core
   After 2nd core entered hotplug mode, TizenRT should remove the idle task for 2nd core
   Revisit here to see if anything else need to be done */
#ifdef CONFIG_SMP
EXIT2:
#endif
/* If state transition is rejected, exit directly*/
REJECTED:
				break;
			default:
				break;
		}
	} else {
		/* If a state is locked, we will not be able to do state transition
		But idle thread might still have chance to be doing judgement under some
		conditions (ie. context switched to idle loop), thus if there is no
		state transition happening, we invoke WFE for the core to rest (ie. a kind of HW sleep),
		to prevent from unecessary power consumption
		*/
		__asm(" WFE");
	}
}
#else
#define up_idlepm()
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

	/* Sleep until an interrupt occurs to save power */

	up_idlepm();
#endif
}

#ifdef CONFIG_PM
void arm_pminitialize(void)
{
	/* Then initialize the TinyAra power management subsystem properly */
	pm_initialize();
}
#endif
