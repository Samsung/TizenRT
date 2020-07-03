/****************************************************************************
 *  arch/arm/src/stm32l4/stm32l4_idle.c
 *
 *   Copyright (C) 2011-2012, 2015-2016 Gregory Nutt. All rights reserved.
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
#include <errno.h>

#include "chip.h"
#include "stm32l4_pm.h"
#include "up_internal.h"
#include "stm32l4_rtc.h"
#include <tinyara/rtc.h>
#include "stm32l4.h"
#include "stm32l4_gpio.h"
#include "stm32l4_start.h"
#include "stm32l4xx_hal_interface.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Does the board support an IDLE LED to indicate that the board is in the
 * IDLE state?
 */

#if defined(CONFIG_ARCH_LEDS) && defined(LED_IDLE)
#  define BEGIN_IDLE() board_led_on(LED_IDLE)
#  define END_IDLE()   board_led_off(LED_IDLE)
#else
#  define BEGIN_IDLE()
#  define END_IDLE()
#endif

#define PM_IDLE_DOMAIN 0 /* Revisit */
#define STM32L4_EXT_JOYKEY_ACTIVITY			4

#ifdef CONFIG_SCHED_TICKSUPPRESS
#ifndef CONFIG_STM32L4_RTC
#error "Cannot support ticksuppress without RTC enabled (Enable CONFIG_STM32L4_RTC)"
#endif
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_ARCH_SUPPORT_ENTER_SLEEP
int up_pmsleep(void)
{
  int index;
  int ret = OK;
  irqstate_t flags;

  flags = irqsave();
  sched_lock();
  for (index = PM_NORMAL; index < PM_SLEEP; index++)
    {
      if (pm_staycount(PM_IDLE_DOMAIN, index))
        {
          ret = -EAGAIN;
          goto errout_lock;
        }
    }
  ret = pm_changestate(0, PM_SLEEP);
  if (ret < 0)
    {
      goto errout;
    }
  set_exti_button();
  (void)stm32l4_pmstop2();

  stm32l4_clockenable();
errout:
  pm_changestate(0, PM_NORMAL);
errout_lock:
  sched_unlock();
  irqrestore(flags);

  return ret;
}
#endif
/****************************************************************************
 * Private Functions
 ****************************************************************************/

void up_extiisr(int irq, uint32_t *regs)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  up_lowputc('i');
  pm_activity(PM_IDLE_DOMAIN, STM32L4_EXT_JOYKEY_ACTIVITY);
}

void set_exti_button(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};         /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_SLEEP_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();   /*Configure GPIO pin : PC13 */
  __HAL_RCC_GPIOC_CLK_SLEEP_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  (void)irq_attach(STM32L4_IRQ_EXTI1510, (xcpt_t)up_extiisr, NULL);
}

/****************************************************************************
 * Name: up_idlepm
 *
 * Description:
 *   Perform IDLE state power management.
 *
 ****************************************************************************/

#ifdef CONFIG_PM
static void up_idlepm(void)
{
  static enum pm_state_e oldstate = PM_NORMAL;
  enum pm_state_e newstate;
  irqstate_t flags;
  int ret;
#ifdef CONFIG_STM32L4_RTC
  long ns1, ns2;
  clock_t time_intval;
  struct tm tp1, tp2;
#endif

  /* Decide, which power saving level can be obtained */

  newstate = pm_checkstate(PM_IDLE_DOMAIN);

  /* Check for state changes */

  if (newstate != oldstate)
    {
      flags = irqsave();

      /* Perform board-specific, state-dependent logic here */

      printf("newstate= %d oldstate=%d\n", newstate, oldstate);

      /* Then force the global state change */

      ret = pm_changestate(PM_IDLE_DOMAIN, newstate);
      if (ret < 0)
        {
          /* The new state change failed, revert to the preceding state */

          (void)pm_changestate(PM_IDLE_DOMAIN, oldstate);
        }
      else
        {
          /* Save the new state */

          oldstate = newstate;
        }

      /* MCU-specific power management logic */

      switch (newstate)
        {
        case PM_NORMAL:
          break;

        case PM_IDLE:
          break;

        case PM_STANDBY:
#ifdef CONFIG_SCHED_TICKSUPPRESS
#ifdef CONFIG_STM32L4_RTC
          /* Note RTC time before sleep */
          stm32l4_rtc_getdatetime_with_subseconds(&tp1, &ns1);
#endif
          /* Disable tick interrupts */
          supress_ticks();
          /* Set waketime interrupt for tickless idle  */
          set_waketime_interrupt();
          /* Enter sleep mode */
          stm32l4_pmsleep(false);
#ifdef CONFIG_STM32L4_RTC
          /* Read RTC time after wakeup */
          stm32l4_rtc_getdatetime_with_subseconds(&tp2, &ns2);
          time_intval = SEC2TICK(mktime(&tp2) - mktime(&tp1)) + NSEC2TICK(ns2 - ns1);
          /* Update ticks */
          g_system_timer +=  time_intval;
          uwTick += TICK2MSEC(time_intval);
          /* Execute waketime interrupt */
          execute_waketime_interrupt(time_intval);
#endif
          /* Enable tick interrupts */
          enable_ticks();

          /* We dont want state change directly
           * it is the resposibility of the scheduled
           * event to inform the PM Core about the
           * pm activity based on its requirement */
          //oldstate = PM_IDLE;	/* Re visit */
#else
          stm32l4_pmsleep(false);
#endif		/* CONFIG_SCHED_TICKSUPPRESS */

          break;

        case PM_SLEEP:
          /* Set EXTI interrupt */
          set_exti_button();

          (void)stm32l4_pmstop2();

          /* Re configure clocks */
          stm32l4_clockenable();

          ret = pm_changestate(PM_IDLE_DOMAIN, PM_NORMAL);
          if (ret < 0)
            {
              oldstate = PM_NORMAL;
            }

          printf("Wakeup from STOP2!!\n");
          break;

        default:
          break;
        }

      irqrestore(flags);
    }
}
#else
#  define up_idlepm()
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

  nxsched_process_timer();
#else

  /* Perform IDLE mode power management */

  up_idlepm();

  /* Sleep until an interrupt occurs to save power. */

#if !(defined(CONFIG_DEBUG_SYMBOLS) && defined(CONFIG_STM32L4_DISABLE_IDLE_SLEEP_DURING_DEBUG))
  BEGIN_IDLE();
  asm("WFI");
  END_IDLE();
#endif

#endif
}
