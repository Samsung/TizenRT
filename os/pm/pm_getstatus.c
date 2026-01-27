/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
#include <tinyara/pm/pm.h>
#include "pm.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_getstatus
 *
 * Description:
 *   This function is called to get the current PM running status.
 *
 * Input parameters:
 *   None
 *
 * Returned value:
 *   PM_STATUS_STOPPED (0) - PM is stopped
 *   PM_STATUS_RUNNING (1) - PM is running
 *
 ****************************************************************************/

int pm_getstatus(void)
{
	int status;

	pm_lock();
	status = g_pmglobals.is_running ? PM_STATUS_RUNNING : PM_STATUS_STOPPED;
	pm_unlock();

	return status;
}

#endif /* CONFIG_PM */
