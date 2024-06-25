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

#include "pm.h"

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
	flags = enter_critical_section();
	now = clock_systimer();
	/* We need to check and change PM state transition only if one tick time has been passed,
	 * because state transition only happens when CPU receive TICK INTERRUPT. So checking pm state
	 * multiple times within one tick is waste of CPU clocks and we should avoid it.
	 */
	if ((now - stime) >= 1) {
		stime = now;
		/* Decide, which power saving level can be obtained */
		newstate = pm_checkstate();
		/* Perform state-dependent logic here */
		pmvdbg("newstate= %d\n", newstate);
#ifdef CONFIG_PM_METRICS
		pm_metrics_update_idle();
#endif
		/* Then force the global state change */
		pm_changestate(newstate);

		/* If current state is not good to go sleep then do core power saving*/
		if (g_pmglobals.state != PM_SLEEP) {
			goto EXIT;
		}
		
#ifdef CONFIG_PM_TIMEDWAKEUP
		/* set wakeup timer */
		delay = wd_getwakeupdelay();
		if (delay > 0) {
			if (delay < CONFIG_PM_MIN_SLEEP_TIME) {
				pmvdbg("Minimum sleep time should be %d\n", CONFIG_PM_MIN_SLEEP_TIME);
				goto EXIT;
			} else {
				pmvdbg("Setting timer and board will wake up after %d millisecond\n", delay);
				up_set_pm_timer(TICK2USEC(delay));
			}
		}
#endif
		up_pm_board_sleep(pm_wakehandler);
	}
EXIT:
	leave_critical_section(flags);
}

#endif /* CONFIG_PM */
