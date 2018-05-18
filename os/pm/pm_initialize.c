/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * pm/pm_initialize.c
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdlib.h>
#include <semaphore.h>
#include <assert.h>
#include <debug.h>
#include <tinyara/pm/pm.h>
#include <time.h>
#include "pm.h"
#include "pm_metrics.h"

#ifdef CONFIG_PM

/* PM initialization data: */
enum {
	SWITCH_THRESHOLD = 1,
	ENTER_THRESHOLD,
	EXIT_THRESHOLD
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* All PM global data: */

struct pm_global_s g_pmglobals;

/* Sleep state count threshold array. These values are used to enter the next reduced power state */
int g_pm_switch_thresholds[CONFIG_PM_NSTATE];

/* Sleep state activity threshold array. These values are used to enter any sleep state */
int g_pm_enter_thresholds[CONFIG_PM_NSTATE];

/* Sleep state activity threshold array. These values are used to exit any sleep state */
int g_pm_exit_thresholds[CONFIG_PM_NSTATE];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_parse_thresholds
 *
 * Description:
 *   Parse the switch, enter & exit threshold values to the global arrays.
 *   If not provided by the SoC initilaize them with default values.
 *
 ****************************************************************************/

static void pm_parse_thresholds(int array_type, const char *thr_str, int threshold_array[])
{
	/* Initialize state threshold value to be read from thresh_str */
	int threshold = 0;
	/* Variable to track number of tick count threshold provided by the SOC */
	int index = 0;

	/* Parse the sleep state tick count thresholds from the Kconfig */

	while (thr_str && *thr_str) {

		/* Extract thresholds from string input in config as integer value */
		threshold = strtoul(thr_str, NULL, 0);
		DEBUGASSERT(threshold >= 0);

		while (*thr_str != ',' && *thr_str) {
			thr_str++;
		}

		if (*thr_str == ',') {
			thr_str++;
		}

		index++;

		/* Case when more state threshold values are provided by PM_STATE_COUNT_THRESH*/
		if (index > (CONFIG_PM_NSTATE - 1)) {
			DEBUGASSERT(!(index > (CONFIG_PM_NSTATE - 1)));
		}

		/* Update sleep state threshold array with extracted integer value */
		threshold_array[index] = threshold;
	}

	/* Check if all the state entering tick count thresholds are provided by the SOC */

	if ((index > 0 && index < (CONFIG_PM_NSTATE - 1))) {

		/* Case when less state threshold values are provided by PM_STATE_COUNT_THRESH*/
		DEBUGASSERT(!(index > 0 && index < CONFIG_PM_NSTATE));

	} else if (index == 0) {

		/* Case when no state threshold values are provided assign default threshold values */
		for (index = 0; index < CONFIG_PM_NSTATE; index++) {
			switch (array_type) {
			case SWITCH_THRESHOLD:
				g_pm_switch_thresholds[index] = PM_DEFAULT_STATE_SWITCH_THRESH * index;
				break;
			case ENTER_THRESHOLD:
				g_pm_enter_thresholds[index] = PM_DEFAULT_STATE_ENTER_THRESH;
				break;
			case EXIT_THRESHOLD:
				g_pm_exit_thresholds[index] = PM_DEFAULT_STATE_EXIT_THRESH;
				break;
			default:
				break;
			}
		}
	}
}


/****************************************************************************
 * Name: pm_thresholds_init
 *
 * Description:
 *   Map the CPU IDLE power states & their transition timings to the TizenRT
 *   power management framework. If threshold counts are not provided by SOC,
 *   initialize them with default PM values of thresholds.
 *
 ****************************************************************************/

void pm_thresholds_init(void)
{
	pm_parse_thresholds(SWITCH_THRESHOLD, CONFIG_PM_STATE_SWITCH_THRESH, g_pm_switch_thresholds);
	pm_parse_thresholds(ENTER_THRESHOLD, CONFIG_PM_STATE_ENTER_THRESH, g_pm_enter_thresholds);
	pm_parse_thresholds(EXIT_THRESHOLD, CONFIG_PM_STATE_EXIT_THRESH, g_pm_exit_thresholds);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_initialize
 *
 * Description:
 *   This function is called by MCU-specific one-time at power on reset in
 *   order to initialize the power management capabilities.  This function
 *   must be called *very* early in the initializeation sequence *before* any
 *   other device drivers are initialize (since they may attempt to register
 *   with the power management subsystem).
 *
 * Input parameters:
 *   None.
 *
 * Returned value:
 *    None.
 *
 ****************************************************************************/

void pm_initialize(void)
{
	int domain_indx = 0;

	/* Initialize the registry and the PM global data structures.  The PM
	 * global data structure resides in .bss which is zeroed at boot time.  So
	 * it is only required to initialize non-zero elements of the PM global
	 * data structure here.
	 */

	for (domain_indx = 0; domain_indx < CONFIG_PM_NDOMAINS; domain_indx++) {
		sq_init(&g_pmglobals.domain[domain_indx].registry);
		sem_init(&g_pmglobals.domain[domain_indx].regsem, 0, 1);

#ifdef CONFIG_PM_METRICS
		struct timespec cur_time;

		/* Get current time */
		clock_gettime(CLOCK_REALTIME, &cur_time);

		struct pm_statechange_s *initnode = NULL;

		/* Initialize the domain's state history queue */

		sq_init(&g_pmglobals.domain[domain_indx].history);

		/* Create an initial state change node with NORMAL state and bootup time */

		initnode = (struct pm_statechange_s *)pm_alloc(1, sizeof(struct pm_statechange_s));

		initnode->state = PM_NORMAL;
		initnode->timestamp = cur_time.tv_sec;

		/* Add the initial state change node to the head of the history queue */

		sq_addlast((&initnode->entry), &g_pmglobals.domain[domain_indx].history);
#endif
#if CONFIG_PM_NSTATE > 0
		/* Initialize the sleep states' tick count threshold values */
		pm_thresholds_init();
#endif
	}
}
#endif							/* CONFIG_PM */
