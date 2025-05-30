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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <assert.h>
#include <tinyara/pm/pm.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include "../kernel/sched/sched.h"

#include "pm.h"
#ifdef CONFIG_SMP
#include "../arch/arm/src/armv7-a/smp.h"
#endif

#ifdef CONFIG_PM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static clock_t stime;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_idle
 *
 * Description:
 *   This function is called by IDLE thread to make board sleep. This function
 *   also allow to set wake up timer & handler and do all the PM pre processing
 *   required before going to sleep.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void pm_idle(void)
{
	irqstate_t flags;
	enum pm_state_e newstate;
	clock_t now;
#ifdef CONFIG_PM_TIMEDWAKEUP
	clock_t delay = 0;
#endif
#ifdef CONFIG_SMP
	int cpu;
	int gated_cpu_count = 0;
	FAR struct tcb_s *tcb;
#endif
	/* State change only if PM is ready to state change */
	if (!g_pmglobals.is_running) {
		return;
	}
	flags = enter_critical_section();
	sched_lock();
	now = clock_systimer();
	/* We need to check and change PM state transition only if one tick time has been passed,
	 * because state transition only happens when CPU receive TICK INTERRUPT. So checking pm state
	 * multiple times within one tick is waste of CPU clocks and we should avoid it.
	 */
	if (now > stime) {
		stime = now;
		/* Decide, which power saving level can be obtained */
		newstate = pm_checkstate();
#ifdef CONFIG_PM_METRICS
		pm_metrics_update_idle();
#endif
#ifdef CONFIG_PM_TIMEDWAKEUP
		/* get wakeup timer */
		if (newstate == PM_SLEEP) {
			delay = wd_getwakeupdelay();
			if ((delay > 0) && (delay < MSEC2TICK(CONFIG_PM_SLEEP_ENTRY_WAIT_MS))) {
				pmvdbg("Wdog Timer Delay: %ldms is less than SLEEP_ENTRY_WAIT: %ldms\n", TICK2MSEC(delay), CONFIG_PM_SLEEP_ENTRY_WAIT_MS);
				goto EXIT;
			}
		}
#endif
		/* Perform state-dependent logic here */
		/* For SMP case, we need to check secondary core status
	     * If secondary core status is not in idle thread, abort
	     * the sleep and check again on next cycle
		 */
#ifdef CONFIG_SMP
		if (newstate == PM_SLEEP) {
			for (cpu = 1; cpu < CONFIG_SMP_NCPUS; cpu++) {
				/* If the CPU is just back from sleep, abort the sleep */
				if (up_get_cpu_state(cpu) == CPU_WAKE_FROM_SLEEP) {
					goto EXIT;
				} else if (up_get_cpu_state(cpu) == CPU_HOTPLUG) {
					continue;
				}

				/* Gate the cpu first, before checking which task it is handling */
				if (!up_get_gating_flag_status(cpu)) {
					up_set_gating_flag_status(cpu, 1);
					up_cpu_gating(cpu);
					gated_cpu_count = cpu;
				}
				while (up_get_gating_flag_status(cpu) == 1) {
					/* If there is a pause request, we should handle it first */
					if (up_cpu_pausereq(up_cpu_index())) {
						pmdbg("Sleep abort! CPU%d\n", cpu);
						goto EXIT;
					}
				}

				tcb = current_task(cpu);
				/* Check if current cpu is in idle thread, and whether there is pending
				 * pause request on primary core 
				 */
				if (tcb->pid != cpu) {
					pmvdbg("Sleep abort! CPU%d task: %s!\n", cpu, tcb->name);
					goto EXIT;
				}
			}
		}
#endif
		/* Then force the global state change */
		if (pm_changestate(newstate) < 0) {
			/* The new state change failed */
			pmdbg("State change failed! newstate = %d\n", newstate);
			goto EXIT;
		}
		/* If current state is not good to go sleep then do core power saving*/
		if (g_pmglobals.state != PM_SLEEP) {
			goto EXIT;
		}
#ifdef CONFIG_SMP
		/* Send signal to shutdown other cores here */
		for (cpu = 1; cpu < CONFIG_SMP_NCPUS; cpu++) {
			if (up_get_cpu_state(cpu) == CPU_RUNNING) {
				up_cpu_hotplug(cpu);
			}
			/* Reset core gating status flag */
			up_set_gating_flag_status(cpu, 0);
			/* Check whether each of the cpu has entered hotplug */
			while (up_get_cpu_state(cpu) != CPU_HOTPLUG);
		}
		gated_cpu_count = 0;
#endif
#ifdef CONFIG_PM_TIMEDWAKEUP
		/* set wakeup timer */
		if (delay > 0) {
			pmvdbg("Setting timer and board will wake up after %ld millisecond\n", delay);
			g_pmglobals.sleep_ops->set_timer(TICK2USEC(delay));
		}
#endif
		g_pmglobals.sleep_ops->sleep(pm_wakehandler);
		stime = clock_systimer();
	}
EXIT:
#ifdef CONFIG_SMP
	/* Check if any core is gated, resume it */
	while (gated_cpu_count) {
		up_set_gating_flag_status(gated_cpu_count, 0);
		gated_cpu_count--;
	}
#endif
	sched_unlock();
	leave_critical_section(flags);
}

#endif /* CONFIG_PM */
