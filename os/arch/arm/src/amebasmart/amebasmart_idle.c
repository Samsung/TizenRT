/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_idle.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#include "amebasmart_config.h"
#include "arch_timer.h"
#include "ameba_soc.h"
#include "osdep_service.h"
#endif

#ifdef CONFIG_SMP
#include "gic.h"
#include "smp.h"
#endif

#ifdef CONFIG_PM

static int up_pm_board_sleep(void);
static pm_wakeup_reason_code_t up_get_wakeupreason(void);
static clock_t up_get_missingtick(void);
extern int up_set_pm_timer(unsigned int interval_us);

struct pm_sleep_ops rtl8730e_sleep_ops = {
	.sleep = up_pm_board_sleep,
	.set_timer = up_set_pm_timer,
	.get_wakeupreason = up_get_wakeupreason,
	.get_missingtick = up_get_missingtick,
};

#ifdef CONFIG_PM_DVFS

extern void up_set_dvfs(int div_lvl);
struct pm_clock_ops rtl8730e_clock_ops = {
	.adjust_dvfs = up_set_dvfs,
};
#endif

static pm_wakeup_reason_code_t up_get_wakeupreason(void)
{
	return tizenrt_get_wakeupreason();
}

static clock_t up_get_missingtick(void)
{
	return tizenrt_get_missingtick();
}
/****************************************************************************
 * Name: up_pm_board_sleep
 *
 * Description:
 *   Perform IDLE state power management.
 * 
 * Input Parameters:
 *   wakeuphandler - The wakeuphandler function that must be called after each board wakeup.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static int up_pm_board_sleep(void)
{
	/* Interrupt source will wake cpu up, just leave expected idle time as 0
	Enter sleep mode for AP */
	config_SLEEP_PROCESSING();

	return 0;
}
#else
#define up_pm_board_sleep() 0
#endif

/****************************************************************************
 * Name: up_idle
 *
 * Description:
 *   up_idle() is the logic that will be executed when there is no other
 *   ready-to-run task.  This is processor idle time and will continue until
 *   some interrupt occurs to cause a context switch from the idle task.
 *
 *   Processing in this state may be processor-specific. e.g., this is where
 *   power management operations might be performed.
 *
 ****************************************************************************/

void up_idle(void)
{
#if defined(CONFIG_SUPPRESS_INTERRUPTS) || defined(CONFIG_SUPPRESS_TIMER_INTS)
	/* If the system is idle and there are no timer interrupts, then process
	 * "fake" timer interrupts. Hopefully, something will wake up.
	 */

	sched_process_timer();
#else

#ifdef CONFIG_PM
	/* set core to WFE */
	__asm("WFE");
#endif

#endif
}

#ifdef CONFIG_PM
void arm_pminitialize(void)
{
	/* Then initialize the TinyAra power management subsystem properly */
	pm_initialize(&rtl8730e_sleep_ops);

#ifdef CONFIG_PM_DVFS
	pm_clock_initialize(&rtl8730e_clock_ops);
#endif
}
#endif
