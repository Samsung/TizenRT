/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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

#include <arch/board/board.h>
#include <tinyara/config.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/board.h>
#include <tinyara/pm/pm.h>
#include <modules/pm.h>
#include "../../../../pm/pm.h"

#include "chip.h"
#include "up_internal.h"
#include <driver/uart.h>
#include <stdbool.h>


/*=====================DEFINE SECTION START====================*/
typedef int                                bk_err_t;      /**< Return error code */
#define MAX_REASONABLE_TICKS               86400000ULL    /* 24 hours in ticks (1ms per tick) */
#define DOMAIN_NAME_SUSPEND_TEST           "SUSPEND_TEST"
#define PM_DEFAULT_SLEEP_TIME_US           (100000)  //100ms
/*=====================DEFINE SECTION END======================*/


/*=====================VARIABLE SECTION START==================*/
clock_t s_missed_ticks = 0;
extern sq_queue_t g_wdactivelist;
/*=====================VARIABLE SECTION END====================*/

/*================FUNCTION DECLARATION SECTION START===========*/
extern void up_set_pm_timer(unsigned int interval_us);
extern void up_set_dvfs(int div_lvl);
extern uint32_t pm_get_sleep_time(void);
extern struct pm_domain_s *pm_domain_find(FAR const char *domain_name);
int armino_sleep_processing(void);
#ifdef CONFIG_SCHED_TICKSUPPRESS
extern void clock_timer_nohz(clock_t ticks);
extern void wd_timer_nohz(clock_t ticks);
#endif
#if defined(CONFIG_RTC_DRIVER)
extern void armino_update_rtc_tick_after_sleep(void);
#endif

/*================FUNCTION DECLARATION SECTION END=============*/

#ifdef CONFIG_PM
void up_enable_and_compensate_systick(void)
{
#ifdef CONFIG_PM_TICKSUPPRESS
	struct pm_sleep_ops *sleep_ops = g_pmglobals.sleep_ops;
	clock_t missing_tick;

	if (!sleep_ops->get_missingtick) {
		return;
	}

	missing_tick = sleep_ops->get_missingtick();

	if (missing_tick > 0) {
		/* Correcting for missed system ticks in sleep. */
		clock_timer_nohz(missing_tick);

		/* Compensate wd timer for missing ticks by pm sleep.
		 * But to guarantee fast execution of interrupt service routine after wakeup,
		 * expiration of wd_timer is not done here
		 *
		 *     WAKE UP -> HW IRQ ISR -> THREAD -> TICK ISR
		 *           |              |          |           |
		 *           +--------------+----------+-----------+
		 *     (corrects tick)                       (expire timer)
		 */
		wd_timer_nohz(missing_tick);
	}
#endif
	(void)up_timer_enable();
}
static pm_wakeup_reason_code_t up_get_wakeup_reason(void)
{
	return bk_pm_sleep_wakeup_reason_get();
}

static clock_t up_get_missed_ticks(void)
{
	clock_t ticks = s_missed_ticks;
	s_missed_ticks = 0;  // Clear after reading to prevent double compensation
	return ticks;
}
static bool up_check_valid_wakeup(void)
{
	/* Check watchdog active list to see if there is any wakeup timer with lag > 0 */
	bool has_valid_wakeup         = false;
	struct wdog_s *curr           = NULL;

	for (curr = (FAR struct wdog_s *)g_wdactivelist.head; curr; curr = curr->next) {
		if (WDOG_ISWAKEUP(curr)) {
			has_valid_wakeup = true;
			break;
		}
	}

	/* Check whether it set sleep time for registered domain*/
	struct pm_domain_s * domain = pm_domain_find(DOMAIN_NAME_SUSPEND_TEST);
	if((has_valid_wakeup == false)&& (domain != NULL)) {
		return 0;
	}
	return 1;
}
/****************************************************************************
 * Name: up_pm_board_sleep
 *
 * Description:
 *   Perform IDLE state power management.
 *
 * Input Parameters:
 *   handler - The handler function that must be called after each board wakeup.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
static int up_pm_board_sleep(void)
{
	if (!g_pmglobals.is_running) {
		return 0;
	}
	if(up_check_valid_wakeup() == 0) {
		return 0;
	}
	if(pm_checkstate() == PM_SLEEP)
	{

	}
	else
	{

	}

	return 0;
}
#else
#define up_pm_board_sleep() 0
#endif

#ifdef CONFIG_PM

struct pm_sleep_ops armino_sleep_ops = {
	.sleep = up_pm_board_sleep,
	.set_timer = up_set_pm_timer,
	.get_wakeupreason = up_get_wakeup_reason,
	.get_missingtick = up_get_missed_ticks,
};

#ifdef CONFIG_PM_DVFS

struct pm_clock_ops armino_clock_ops = {
	.adjust_dvfs = up_set_dvfs,
};
#endif
#endif
/****************************************************************************
 * Name: up_idle
 *
 * Description:
 *   up_idle() is the logic that will be executed when their is no other
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

#endif
	armino_sleep_processing();
}
#ifdef CONFIG_PM
void arm_pminitialize(void)
{
	/* Then initialize the TinyAra power management subsystem properly */
	pm_initialize(&armino_sleep_ops);

#ifdef CONFIG_PM_DVFS
	pm_clock_initialize(&armino_clock_ops);
#endif
}
#endif

/****************************************************************************
 * Name: armino_sleep_processing
 *
 * Description:
 *   Process sleep and handle time compensation when waking up.
 *   This function calculates the actual sleep duration and compensates
 *   system ticks to maintain accurate time keeping.
 *
 * Time Compensation Table:
 * +--------------+---------------+-------------------+-----------------------+-----------------+
 * | Sleep Time   | ticks_passed  | SysTick Compensate| WakeupHandler Comp.  | Total Compensate|
 * +--------------+---------------+-------------------+-----------------------+-----------------+
 * | 0.5 tick     | 0 or 1        | 1 tick            | 0 tick                | 1 tick       |
 * | 1.5 tick     | 2             | 1 tick            | 1 tick                | 2 tick       |
 * | 2.5 tick     | 3             | 1 tick            | 2 tick                | 3 tick       |
 * | 10 tick      | 10            | 1 tick            | 9 tick                | 10 tick      |
 * +--------------+---------------+-------------------+-----------------------+-----------------+
 *
 * Input Parameters:
 *   wakeuphandler - Callback function to handle wakeup and time compensation
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
int armino_sleep_processing()
{
	uint64_t missed_ticks          =  0;
	uint64_t usec_passed           =  0;
	clock_t ticks_passed           =  0;
	volatile uint32_t int_level    = 0;

	int_level = pm_disable_int();
	sched_lock();
	/*Check whether the system is going to enter different sleep mode*/
	up_pm_board_sleep();
	/* Enter sleep mode */
	missed_ticks = bk_pm_suppress_ticks_and_sleep(0);

	#ifdef CONFIG_EXTERN_32K
	/* For external 32.768 KHz crystal */
	usec_passed = (missed_ticks * 15625ULL) >> 9;
	#else
	/* For internal 32 KHz oscillator */
	usec_passed = (missed_ticks * 125ULL) >> 2;
	#endif
	/* Convert to system ticks with proper rounding (USEC2TICK macro does this) */
	ticks_passed = USEC2TICK(usec_passed);

	#if defined(CONFIG_PM_TICKSUPPRESS) && defined(CONFIG_PM)
	/* Update kernel tick only if we slept for more than 1 tick
	 * Note: SysTick interrupt already compensates 1 tick when waking up,
	 * so we only need to compensate for the remaining (ticks_passed - 1) ticks
	 * to avoid double-counting the first tick.
	 */
	if(ticks_passed > 1)
	{
		/* Protect against abnormally large tick values
		 * RTC is 64-bit microsecond counter, theoretical max ~584,942 years
		 * But abnormally large values may indicate:
		 * 1. Hardware/software bug
		 * 2. RTC corruption
		 * 3. Unintended very long sleep
		 *
		 * Set limit to 24 hours = 86400000 ticks (assuming 1ms per tick)
		 * This allows for reasonable deep sleep scenarios while protecting against errors
		 */

		if (ticks_passed > MAX_REASONABLE_TICKS) {
			bk_printf("Warning: Abnormal ticks_passed=0x%llx (>24h), capping to max value,missed_ticks=0x%llx,usec_passed=0x%llx\n",
				(unsigned long long)ticks_passed,
				(unsigned long long)missed_ticks,
				(unsigned long long)usec_passed);
			ticks_passed = 0;  // Reset to 0 to prevent system tick overflow
		}
		s_missed_ticks = ticks_passed;
		up_enable_and_compensate_systick();
#if defined(CONFIG_RTC_DRIVER)
		/* Update RTC tick tracking to prevent double compensation.
		 * After sleep wakeup, we've already compensated for the sleep
		 * duration via s_missed_ticks. We need to update g_last_rtc_tick
		 * in timerisr.c to the current RTC tick value, otherwise the next
		 * timer interrupt will calculate the delta from the pre-sleep
		 * RTC tick value and cause duplicate compensation.
		 */
		armino_update_rtc_tick_after_sleep();
#endif
	}
	else
	{
		s_missed_ticks = 0;
#if defined(CONFIG_RTC_DRIVER)
		/* Even if ticks_passed <= 1, we should still update g_last_rtc_tick
		 * to prevent the timer ISR from calculating a large delta that includes
		 * the sleep duration.
		 */
		armino_update_rtc_tick_after_sleep();
#endif
	}

	#endif
	pm_enable_int(int_level);
	sched_unlock();

	#if defined(CONFIG_PM_DEBUG)
	if(ticks_passed > 10)
	{
		bk_printf("Missed ticks=%llu,ticks_passed=%llu(usec=%llu),wakeup_reason=%d,wakeup_src:%s\n", 
					(unsigned long long)missed_ticks,
					(unsigned long long)ticks_passed,
					(unsigned long long)usec_passed,
					bk_pm_sleep_wakeup_reason_get(),
					bk_rtc_get_first_alarm_name());

	}
	#endif
	return 0;
}