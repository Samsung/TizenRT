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

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>

#include <tinyara/wdog.h>

#include "wdog/wdog.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

/********************************************************************************
 * Private Types
 ********************************************************************************/

/********************************************************************************
 * Global Variables
 ********************************************************************************/

/********************************************************************************
 * Private Variables
 ********************************************************************************/

/********************************************************************************
 * Private Functions
 ********************************************************************************/

/********************************************************************************
 * Public Functions
 ********************************************************************************/

/********************************************************************************
 * Name: wd_getwakeupdelay
 *
 * Description:
 *  The function returns the delay of the wdog that is registered as a CPU wakeup
 *  source and will expire soonest.
 *
 * Parameters:
 *	None
 *
 * Return Value:
 *  delay of the wdog that is registered as a CPU wakeup source and will expire soonest.
 *
 ********************************************************************************/

clock_t wd_getwakeupdelay(void)
{
	clock_t delay = 0;
	struct wdog_s *curr;
	irqstate_t flags;

	flags = enter_critical_section();
	for (curr = (FAR struct wdog_s *)g_wdactivelist.head; curr; curr = curr->next) {
		delay += curr->lag;
		if (WDOG_ISWAKEUP(curr)) {
			leave_critical_section(flags);
			return delay;
		}
	}

	leave_critical_section(flags);
	return 0;
}
