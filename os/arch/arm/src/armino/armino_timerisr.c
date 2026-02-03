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

 #include <tinyara/config.h>

 #include <stdint.h>
 #include <time.h>
 #include <debug.h>
 #include <tinyara/arch.h>
 #include <arch/board/board.h>
 
 #include "nvic.h"
 #include "clock/clock.h"
 #include "up_internal.h"
 #include "up_arch.h"
 #include "modules/pm.h"
 #include <driver/hal/hal_aon_rtc_types.h>
 #include <driver/aon_rtc_types.h>
 #include <driver/aon_rtc.h>
 
 #include "chip.h"
 
 #include "sys_ll.h"
 
 /****************************************************************************
  * Pre-processor Definitions
  ****************************************************************************/
 
 /* The desired timer interrupt frequency is provided by the definition
  * CLK_TCK (see include/time.h).  CLK_TCK defines the desired number of
  * system clock ticks per second.  That value is a user configurable setting
  * that defaults to 100 (100 ticks per second = 10 MS interval).
  *
  */
 
 
 /* The size of the reload field is 24 bits.  Verify that the reload value
  * will fit in the reload register. for beken chip,we always use internal 32k clock
  */
 uint32_t SystemCoreClock = 32000; 
 
 #define SYSTICK_RELOAD ((SystemCoreClock / CLK_TCK) - 1)
 
 #if SYSTICK_RELOAD > 0x00ffffff
 #  error SYSTICK_RELOAD exceeds the range of the RELOAD register
 #endif
 
 #if defined(CONFIG_RTC_DRIVER)
 /****************************************************************************
  * Private Data
  ****************************************************************************/
 
 /* Last RTC tick value, used for tick compensation */
 static uint64_t g_last_rtc_tick = 0;
 
 /****************************************************************************
  * Private Functions
  ****************************************************************************/
 
 /****************************************************************************
  * Name: armino_calculate_delta_ticks
  *
  * Description:
  *   Calculate the number of ticks that should have elapsed based on RTC
  *   time difference. This compensates for missing ticks when interrupts
  *   are disabled for long periods.
  *
  * Input Parameters:
  *   None
  *
  * Returned Value:
  *   Number of ticks to process (at least 1)
  *
  ****************************************************************************/
 
 static uint32_t armino_calculate_delta_ticks(void)
 {
     uint64_t current_rtc_tick;
     uint64_t rtc_tick_diff;
     uint32_t delta_ticks;
     uint32_t rtc_clock_freq;
     uint32_t rtc_tick_per_system_tick;
 
     /* Get current RTC tick value  */
     current_rtc_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
 
     /* First call or RTC wrap/reset: initialize and return 1 tick */
     if (g_last_rtc_tick == 0 || current_rtc_tick < g_last_rtc_tick) {
         g_last_rtc_tick = current_rtc_tick;
         return 1;
     } else {
         rtc_tick_diff = current_rtc_tick - g_last_rtc_tick;
 
         /* Convert RTC tick difference to system ticks
          * 
          * Formula (same as PM module):
          * delta_ticks = rtc_tick_diff / (rtc_clock_freq * USEC_PER_TICK / 1000000)
          * 
          * Simplified:
          * delta_ticks = (rtc_tick_diff * 1000000) / (rtc_clock_freq * USEC_PER_TICK)
          * 
          * Where:
          * - rtc_clock_freq: RTC clock frequency in Hz (32000 or 32768)
          * - USEC_PER_TICK: System tick period in microseconds (usually 1000, i.e., 1ms per tick)
          * 
          * This matches the conversion logic used in PM module for consistency.
          * Use the same logic as ana_rtc_driver.c to determine RTC clock frequency.
          */
         #ifdef CONFIG_EXTERN_32K
         /* External 32.768 KHz crystal */
         rtc_clock_freq = 32768;
         #else
         /* Internal 32 KHz oscillator */
         rtc_clock_freq = 32000;
         #endif
         rtc_tick_per_system_tick = (rtc_clock_freq * USEC_PER_TICK) / 1000000;
 
         /* Calculate delta_ticks with rounding
          * Formula: delta_ticks = (rtc_tick_diff * 1000000 + half_rtc_tick_per_system_tick) / (rtc_clock_freq * USEC_PER_TICK)
          */
         uint64_t half_rtc_tick_per_system_tick = rtc_tick_per_system_tick / 2;
         uint64_t numerator = (rtc_tick_diff * 1000000ULL) + half_rtc_tick_per_system_tick;
         uint64_t denominator = (uint64_t)rtc_clock_freq * USEC_PER_TICK;
         
         uint64_t delta_ticks_64 = numerator / denominator;
         
         /* Guard against uint32_t overflow in delta_ticks */
         if (delta_ticks_64 > UINT32_MAX) {
             /* Reset to avoid time jump if RTC is abnormal */
             g_last_rtc_tick = current_rtc_tick;
             return 1;
         }
         
         delta_ticks = (uint32_t)delta_ticks_64;
         
         /* Ensure at least 1 tick is processed (the current tick interrupt) */
         if (delta_ticks == 0) {
             delta_ticks = 1;
         }
     }
 
     /* Update last RTC tick for next calculation.
      * We update based on the number of system ticks processed to align with tick boundaries.
      * 
      * Convert system ticks back to RTC ticks:
      * rtc_tick_increment = delta_ticks * rtc_tick_per_system_tick
      * 
      * Note: rtc_clock_freq and rtc_tick_per_system_tick are already set above, reuse them.
      */
     g_last_rtc_tick += (uint64_t)delta_ticks * rtc_tick_per_system_tick;
 
     return delta_ticks;
 }
 
 /****************************************************************************
  * Name: armino_update_rtc_tick_after_sleep
  *
  * Description:
  *   Update the last RTC tick value after sleep wakeup to prevent double
  *   compensation. This function should be called after sleep processing
  *   has compensated for missed ticks to synchronize the RTC tick tracking.
  *
  * Input Parameters:
  *   None
  *
  * Returned Value:
  *   None
  *
  ****************************************************************************/
 
 void armino_update_rtc_tick_after_sleep(void)
 {
     /* Update g_last_rtc_tick to current RTC tick value to prevent
      * double compensation in the next timer interrupt.
      * 
      * After sleep wakeup, armino_sleep_processing() has already
      * compensated for the sleep duration. If we don't update
      * g_last_rtc_tick here, the next timer interrupt will calculate
      * the delta from the pre-sleep RTC tick value, causing duplicate
      * compensation.
      */
     g_last_rtc_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
 }
 #endif /* CONFIG_RTC_DRIVER */
 
 /****************************************************************************
  * Private Functions
  ****************************************************************************/
 
 /****************************************************************************
  * Function:  armino_timerisr
  *
  * Description:
  *   The timer ISR will perform a variety of services for various portions
  *   of the systems.
  *
  ****************************************************************************/
 
 int up_timerisr(int irq, uint32_t *regs)
 {
     uint32_t delta_ticks;
     uint32_t ticks_to_process;
     
 #if defined(CONFIG_RTC_DRIVER)
     /* Calculate how many ticks should have elapsed based on RTC time.
      * 
      * Tick compensation mechanism:
      * This compensation is based on hardware RTC (Real-Time Clock), which runs
      * at 32kHz (32000 Hz) and continues counting even when interrupts are disabled.
      * Unlike SysTick which can be affected by interrupt latency, the RTC provides
      * an independent time reference that accurately reflects the actual elapsed time.
      * 
      * Key advantages:
      * 1. RTC runs independently and is not affected by interrupt disabling
      * 2. Provides accurate time reference even during long critical sections
      * 3. Can detect and compensate for multiple missed ticks
      * 
      * Note: To enable tick compensation, CONFIG_RTC must be enabled to ensure
      * the RTC hardware is properly initialized and available.
      */
     delta_ticks = armino_calculate_delta_ticks();
 #else
     delta_ticks = 1;
 #endif
 
     /* Process all missing ticks */
     ticks_to_process = delta_ticks;
     while (ticks_to_process > 0) {
         /* process missing ticks */
         sched_process_timer();
         ticks_to_process--;
     }
 
 
     return 0;
 }
 
 /****************************************************************************
  * Function:  up_timer_initialize
  *
  * Description:
  *   This function is called during start-up to initialize
  *   the timer interrupt.
  *
  ****************************************************************************/
 void up_timer_initialize(void)
 {
 #if defined(CONFIG_RTC_DRIVER)
     /* Initialize RTC-based tick compensation before enabling timer.
      * Note: RTC initialization (up_rtc_initialize) is called before
      * up_timer_initialize() during system startup, so bk_aon_rtc_get_current_tick()
      * can safely return correct RTC tick values at this point.
      * This ensures we can compensate for ticks lost during initialization.
      */
     g_last_rtc_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
 #endif
 
     sys_ll_set_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(1);
 
   /* Set the SysTick interrupt to the min priority */ 
 #ifdef CONFIG_ARCH_IRQPRIO
   up_prioritize_irq(ARMINO_IRQ_SYSTICK, NVIC_SYSH_PRIORITY_DEFAULT);
 #else
   uint32_t regval;
   regval = getreg32(NVIC_SYSH12_15_PRIORITY);
   regval &= ~NVIC_SYSH_PRIORITY_PR15_MASK;
   regval |= (NVIC_SYSH_PRIORITY_MIN << NVIC_SYSH_PRIORITY_PR15_SHIFT);
   putreg32(regval, NVIC_SYSH12_15_PRIORITY);
 #endif
 
   putreg32(0, NVIC_SYSTICK_CTRL);
   putreg32(0, NVIC_SYSTICK_CURRENT);
 
   /* Configure SysTick to interrupt at the requested rate */
 
   putreg32(SYSTICK_RELOAD, NVIC_SYSTICK_RELOAD);
 
   /* Attach the timer interrupt vector */
 
   (void)irq_attach(ARMINO_IRQ_SYSTICK, (xcpt_t)up_timerisr, NULL);
 
   /* Enable SysTick interrupts */
 
   putreg32((NVIC_SYSTICK_CTRL_TICKINT |
             NVIC_SYSTICK_CTRL_ENABLE), NVIC_SYSTICK_CTRL);
 
   /* And enable the timer interrupt */
   up_enable_irq(ARMINO_IRQ_SYSTICK);
 
 }
 
 int up_timer_disable(void)
 {
     //TODO: disable timer interrupt,we will adapt this function 
     return OK;
 }
 
 int up_timer_enable(void)
 {
     bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_TICK_COMP,0x1,0x0);
     //TODO: enable timer interrupt,we will adapt this function 
     return OK;
 }