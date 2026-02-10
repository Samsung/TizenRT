/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 * os/kernel/power/power.h
 *
 ****************************************************************************/

#ifndef __KERNEL_POWER_H
#define __KERNEL_POWER_H

#include <tinyara/pm/pm.h>

/*Sleep ops structure*/
struct pm_sleep_ops *g_sleep_ops = NULL;

/*Indicates board is ready to change state*/
bool is_running = false;

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#ifdef CONFIG_PM_METRICS
/****************************************************************************
 * Name: pm_metrics_update_idle
 *
 * Description:
 *   This function is called inside pm_idle. It counts the frequency of domain, which
 *   make board unable to go into sleep during idle cpu time.
 *
 * Input parameters:
 *   None
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_idle(void);


/****************************************************************************
 * Name: pm_metrics_update_wakeup_reason
 *
 * Description:
 *   This function is called inside pm_wakehandler. It counts the frequency of wakeup
 *   sources, which are waking up the board.
 *
 * Input parameters:
 *   wakeup_src   - the wakeup reason code.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_wakeup_reason(pm_wakeup_reason_code_t wakeup_src);

/****************************************************************************
 * Name: pm_metrics_update_missing_tick
 *
 * Description:
 *   This function is called inside pm_wakehandler. It checks the amount of time board
 *   was in sleep.
 *
 * Input parameters:
 *   missing_tick - the amount of time the board was in sleep.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_missing_tick(clock_t missing_tick);

#else

#define pm_metrics_update_idle()
#define pm_metrics_update_wakeup_reason(wakeup_src)
#define pm_metrics_update_missing_tick(missing_tick)

#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif // #define __KERNEL_POWER_H

