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
#ifdef CONFIG_PM_DVFS

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_clock_initialize
 *
 * Description:
 *   This function is called by MCU-specific one-time at power on reset in
 *   order to initialize the pm clock capabilites.  This function
 *   must be called *very* early in the initialization sequence *before* any
 *   other device drivers are initialize (since they may attempt to register
 *   with the power management subsystem). It also fills the PM ops with the
 *   required BSP APIs.
 *
 * Input parameters:
 *   dvfs_ops: pm power gating operations to use.
 *
 * Returned value:
 *    None.
 *
 ****************************************************************************/

void pm_clock_initialize(struct pm_clock_ops *dvfs_ops)
{
	g_pmglobals.dvfs_ops = dvfs_ops;
}

#endif /* CONFIG_PM_DVFS */
#endif /* CONFIG_PM */
