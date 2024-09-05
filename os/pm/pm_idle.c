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
#include <tinyara/common_logs/common_logs.h>

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
	clock_t delay;
#endif
#ifdef CONFIG_SMP
	int cpu;
	int gated_cpu_count = 0;
	FAR struct tcb_s *tcb;
#endif
	flags = enter_critical_section();
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
				}
				/* Gate the cpu first, before checking which task it is handling */
				up_cpu_gating(cpu);
				/* Polling the status of the target CPU, 
				* The flag should be > 0 if it is already gated
				*/
				while (!up_get_gating_flag_status(cpu));
				tcb = current_task(cpu);
				/* Check if current cpu is in idle thread, and whether there is pending
				 * pause request on primary core 
				 */
				if (tcb->pid != cpu) {
					pmdbg("Sleep abort! CPU%d task: %s!\n", cpu, tcb->name);
					gated_cpu_count = cpu;
					goto EXIT;
				}
			}
		}
#endif
		/* Then force the global state change */
		if (pm_changestate(newstate) < 0) {
			/* The new state change failed */
			pmdbg("%s: newstate = %d\n", clog_message_str[CMN_LOG_FAILED_OP], newstate);
			goto EXIT;
		}
		/* If current state is not good to go sleep then do core power saving*/
		if (g_pmglobals.state != PM_SLEEP) {
			goto EXIT;
		}
#ifdef CONFIG_SMP
		/* Send signal to shutdown other cores here */
		for (cpu = 1; cpu < CONFIG_SMP_NCPUS; cpu++) {
			up_cpu_hotplug(cpu);
			/* Reset core gating status flag */
			up_set_gating_flag_status(cpu, 0);
			/* Check whether each of the cpu has entered hotplug */
			while(up_get_cpu_state(cpu) != CPU_HOTPLUG);
		}
#endif
#ifdef CONFIG_PM_TIMEDWAKEUP
		/* set wakeup timer */
		delay = wd_getwakeupdelay();
		if (delay > 0) {
			if (delay < MSEC2TICK(CONFIG_PM_SLEEP_ENTRY_WAIT_MS)) {
				pmvdbg("Wdog Timer Delay: %dms is less than SLEEP_ENTRY_WAIT: %dms\n", TICK2MSEC(delay), CONFIG_PM_SLEEP_ENTRY_WAIT_MS);
				goto EXIT;
			} else {
				pmvdbg("Setting timer and board will wake up after %d millisecond\n", delay);
				up_set_pm_timer(TICK2USEC(delay));
			}
		}
#endif
		up_pm_board_sleep(pm_wakehandler);
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
	leave_critical_section(flags);
}

#endif /* CONFIG_PM */
