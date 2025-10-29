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

#include "chip.h"
#include "up_internal.h"
#include "pm_sleep_callback.h"

// /* CRITICAL: Workaround for compiler bug using inline assembly
//  *
//  * Problem: Compiler generates "asrs r3, r0, #31" which sign-extends r0
//  * This causes 0x2ee000497c to become 0xffffffffe000497c when r0's bit 31 = 1
//  *
//  * Evidence from assembly (line 310157, 310164):
//  *   bl   bk_aon_rtc_get_us
//  *   asrs r3, r0, #31        ← Should be: mov r3, r1
//  *
//  * Solution: Inline assembly to manually extract r0 and r1 after function call
//  */
// static __attribute__((always_inline)) inline uint64_t rtc_get_us_safe(void)
// {
// 	uint32_t low_val, high_val;

// 	__asm__ __volatile__ (
// 		"bl bk_aon_rtc_get_us \n\t"   // Call function, returns in r0:r1
// 		"mov %0, r0           \n\t"   // Save r0 to low_val
// 		"mov %1, r1           \n\t"   // Save r1 to high_val (NOT asr #31!)
// 		: "=r" (low_val), "=r" (high_val)   // Outputs
// 		:                                     // No inputs
// 		: "r0", "r1", "r2", "r3", "lr", "memory"  // Clobbers
// 	);

// 	// Combine using bitwise OR (not addition) to avoid any signed operations
// 	return ((uint64_t)high_val << 32) | (uint64_t)low_val;
// }

typedef int             bk_err_t;            /**< Return error code */

extern void up_set_pm_timer(unsigned int interval_us); 
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_PM
void armino_sleep_processing(void (*wakeuphandler)(clock_t, pm_wakeup_reason_code_t));
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
#define config_SLEEP_PROCESSING( x )         		(armino_sleep_processing( x ))
void up_pm_board_sleep(void (*wakeuphandler)(clock_t, pm_wakeup_reason_code_t))
{
	//config_SLEEP_PROCESSING(wakeuphandler);
	bk_pm_sleep_mode_set(PM_MODE_NORMAL_SLEEP);
	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x1,0x0);
}
#else
#define up_pm_board_sleep(wakeuphandler)
#endif

#ifdef CONFIG_PM

struct pm_sleep_ops armino_sleep_ops = {
	.sleep = up_pm_board_sleep,
	.set_timer = up_set_pm_timer,
};
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
extern uint64_t bk_pm_suppress_ticks_and_sleep(uint32_t sleep_ticks);
extern void pm_wakehandler(clock_t missing_tick, pm_wakeup_reason_code_t wakeup_src);
extern uint64_t bk_aon_rtc_get_ms(void);
void up_idle(void)
{
#if defined(CONFIG_SUPPRESS_INTERRUPTS) || defined(CONFIG_SUPPRESS_TIMER_INTS)
  /* If the system is idle and there are no timer interrupts, then process
   * "fake" timer interrupts. Hopefully, something will wake up.
   */
	sched_process_timer();
#else

#endif
    //bk_pm_suppress_ticks_and_sleep(0);
	// armino_sleep_processing(pm_wakehandler);
}
#ifdef CONFIG_PM
void arm_pminitialize(void)
{
	/* Then initialize the TinyAra power management subsystem properly */
	pm_initialize(&armino_sleep_ops);

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
void armino_sleep_processing(void (*wakeuphandler)(clock_t, pm_wakeup_reason_code_t))
{
	// uint64_t tick_before_sleep     =  0;
	// uint64_t tick_after_sleep      =  0;
	uint64_t missed_ticks          =  0;
	uint64_t usec_passed           =  0;
	clock_t ticks_passed           =  0;
	uint16_t wakeup_reason         =  0;
	//volatile uint32_t int_level    =  0;

	// int_level = pm_disable_int();

	// /* Use safe wrapper function to avoid compiler's "asrs r3, r0, #31" bug */
	// tick_before_sleep = rtc_get_us_safe();
	sched_lock();
	/* Enter sleep mode */
	missed_ticks = bk_pm_suppress_ticks_and_sleep(0);

	// /* Use safe wrapper for after-sleep timestamp */
	// tick_after_sleep = rtc_get_us_safe();
	// /* Calculate actual sleep duration in microseconds */
	// /* Protect against RTC counter overflow or abnormal values */
	// if (tick_after_sleep >= tick_before_sleep) {
	// 	usec_passed = ((uint64_t)tick_after_sleep - (uint64_t)tick_before_sleep);
	// } else {
	// 	/* RTC counter overflow detected, use maximum safe value */
	// 	usec_passed = 0;
	// 	bk_printf("Warning: RTC overflow detected (before=0x%llx, after=0x%llx)\n",tick_before_sleep,tick_after_sleep);
	// }

	#ifdef CONFIG_EXTERN_32K
	/* For external 32.768 KHz crystal */
	usec_passed = (missed_ticks * 15625ULL) >> 9;
	#else
	/* For internal 32 KHz oscillator */
	usec_passed = (missed_ticks * 125ULL) >> 2;
	#endif
	/* Convert to system ticks with proper rounding (USEC2TICK macro does this) */
	ticks_passed = USEC2TICK(usec_passed);

	/* Get wakeup reason */
	wakeup_reason = bk_pm_sleep_wakeup_reason_get();

	#if defined(CONFIG_PM_DEBUG)
	if(ticks_passed > 10)
	{
		bk_printf("Missed ticks = %llu,ticks_passed = %llu (usec=%llu), wakeup_reason = %d\n", 
		          (unsigned long long)missed_ticks,
		          (unsigned long long)ticks_passed,
		          (unsigned long long)usec_passed,
		          wakeup_reason);
	}
	#endif

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
		#define MAX_REASONABLE_TICKS  86400000ULL  /* 24 hours in ticks (1ms per tick) */

		if (ticks_passed > MAX_REASONABLE_TICKS) {
		// bk_printf("Warning: Abnormal ticks_passed=%llu (>24h), capping to max value,(before=0x%llx, after=0x%llx)\n",
		// 											(unsigned long long)ticks_passed,tick_before_sleep,tick_after_sleep);
		bk_printf("Warning: Abnormal ticks_passed=0x%llx (>24h), capping to max value,missed_ticks=0x%llx,usec_passed=0x%llx\n",
													(unsigned long long)ticks_passed,
													(unsigned long long)missed_ticks,
													(unsigned long long)usec_passed);
		ticks_passed = 0;  // Reset to 0 to prevent system tick overflow
		}
		if (wakeuphandler) {
			/* Pass (ticks_passed - 1) to compensate for ticks missed during sleep
			 * excluding the one already handled by SysTick interrupt
			 */
			wakeuphandler(ticks_passed - 1, wakeup_reason);
		}
	}
	#endif
	sched_unlock();
}