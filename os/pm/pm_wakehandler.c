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
#include <debug.h>

#include <arch/irq.h>
#include <arch/limits.h>

#include "pm.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_wakehandler
 *
 * Description:
 *   This function is called when the core wakes up. The operations are that
 *   should be reflected in the kernel immediately after the core wakes up.
 *	 This behavior is only for the IDLE domain.
 *
 * Input parameters:
 *   missing_tick - Missed ticks while the core was sleeping.
 *
 * Returned value:
 *   None
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

void pm_wakehandler(clock_t missing_tick)
{
	irqstate_t flags  = enter_critical_section();

	pmllvdbg("missing_tick: %llu\n", missing_tick);

	if (missing_tick > 0) {
		clock_timer_nohz((clock_t)missing_tick);
		wd_timer_nohz(missing_tick > INT_MAX ? INT_MAX : (int)missing_tick);
		/* Check for the boundary case , in pm_timer_update(int ticks)
	 	* but we pass unsigned int to it */
		pm_timer_update(missing_tick);
	}

	/* After wakeup change PM State to STANDBY */
	pm_changestate(PM_STANDBY);

	leave_critical_section(flags);
}
