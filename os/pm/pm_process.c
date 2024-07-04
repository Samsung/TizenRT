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
/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/pm/pm.h>
#include <tinyara/wdog.h>
#include <tinyara/sched.h>
#include <errno.h>
#include "pm.h"

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/
#ifndef CONFIG_PM_STANDBY_TO_SLEEP_THRESH
#define CONFIG_PM_STANDBY_TO_SLEEP_THRESH 3000
#endif
/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/************************************************************************
 * Private Functions
 ************************************************************************/

static uint32_t pm_gettimeinterval(void)
{
	uint32_t ret = CONFIG_PM_STANDBY_TO_SLEEP_THRESH;
#if defined(CONFIG_PM_LCD_NORMAL_TO_IDLE_THRESH) && defined(CONFIG_PM_LCD_IDLE_TO_STANDBY_THRESH)
	if (g_pmglobals.state == PM_NORMAL) {
		ret = CONFIG_PM_LCD_NORMAL_TO_IDLE_THRESH;
	} else if (g_pmglobals.state == PM_IDLE) {
		ret = CONFIG_PM_LCD_IDLE_TO_STANDBY_THRESH;
	}
#endif
	DEBUGASSERT(ret != 0);
	return ret;
}

static void pm_process(int argc)
{
	enum pm_state_e newstate;
	newstate = pm_checkstate();
	if (newstate > g_pmglobals.state) {
		/* After timer expiration change PM State to next feasible state */
		(void)pm_changestate(newstate);
		/* Resume Timer for power saving */
		if ((g_pmglobals.state != PM_SLEEP)) {
			(void)pm_process_resume();
		}
	}
}

/************************************************************************
 * Public Functions
 ************************************************************************/

/****************************************************************************
 * Name: pm_process_init
 *
 * Description:
 *   It initializes the pm state transition timer.
 *
 * Input Parameters:
 *
 * Returned Value:
 *   0 (OK), if able to start timer
 *  -1 (ERROR), on error.
 *
 ****************************************************************************/

int pm_process_init(void)
{
	/* Create WDOG timer if it is not already there */
	if (!g_pmglobals.wdog) {
		g_pmglobals.wdog = wd_create();
		if (g_pmglobals.wdog == NULL) {
			pmvdbg("Unable to create WDog timer\n");
			return ERROR;
		}
	}
	return OK;
}

/****************************************************************************
 * Name: pm_process_suspend
 *
 * Description:
 *   It stops the pm state transition timer.
 *
 * Input Parameters:
 *
 * Returned Value:
 *   0 (OK), if able to start timer
 *  -1 (ERROR), on error.
 *
 ****************************************************************************/

int pm_process_suspend(void)
{
	enum pm_state_e newstate;
	newstate = pm_checkstate();
	if (newstate < g_pmglobals.state) {
		/* Change to high power state immediately */
		(void)pm_changestate(newstate);
	}
	return OK;
}

/****************************************************************************
 * Name: pm_process_resume
 *
 * Description:
 *   It starts the pm state transition timer.
 *
 * Input Parameters:
 *
 * Returned Value:
 *    0 (OK), if able to start timer
 *   -1 (ERROR), on error.
 *
 ****************************************************************************/

int pm_process_resume(void)
{
	int ret;
	/* Start the WDOG timer with pm_process call */
	ret = wd_start(g_pmglobals.wdog, pm_gettimeinterval(), (wdentry_t)pm_process, 0);
	if (ret != OK) {
		pmlldbg("Unable to start timer to change state from %d\n", g_pmglobals.state);
	}
	return ret;
}
