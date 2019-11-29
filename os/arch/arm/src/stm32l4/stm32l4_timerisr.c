/****************************************************************************
 * arch/arm/src/stm32l4/stm32l4_timerisr.c
 *
 *   Copyright (C) 2009, 2017 Gregory Nutt. All rights reserved.
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

#include "chip.h"
#include "stm32l4.h"
#include "stm32l4xx_hal_interface.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The desired timer interrupt frequency is provided by the definition
 * CLK_TCK (see include/time.h).  CLK_TCK defines the desired number of
 * system clock ticks per second.  That value is a user configurable setting
 * that defaults to 100 (100 ticks per second = 10 MS interval).
 *
 * The RCC feeds the Cortex System Timer (SysTick) with the AHB clock (HCLK)
 * divided by 8.  The SysTick can work either with this clock or with the
 * Cortex clock (HCLK), configurable in the SysTick Control and Status
 * register.
 */

#undef CONFIG_STM32L4_SYSTICK_HCLKd8 /* Power up default is HCLK, not HCLK/8 */
                                   /* And I don't know now to re-configure it yet */

#ifdef CONFIG_STM32L4_SYSTICK_HCLKd8
#  define SYSTICK_RELOAD ((STM32L4_HCLK_FREQUENCY / 8 / CLK_TCK) - 1)
#else
#  define SYSTICK_RELOAD ((STM32L4_HCLK_FREQUENCY / CLK_TCK) - 1)
#endif

/* The size of the reload field is 24 bits.  Verify that the reload value
 * will fit in the reload register.
 */

#if SYSTICK_RELOAD > 0x00ffffff
#  error SYSTICK_RELOAD exceeds the range of the RELOAD register
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  stm32l4_timerisr
 *
 * Description:
 *   The timer ISR will perform a variety of services for various portions
 *   of the systems.
 *
 ****************************************************************************/

int up_timerisr(int irq, uint32_t *regs)
{
    /* Process timer interrupt */
    sched_process_timer();

    return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  arm_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/

void up_timer_initialize(void)
{
  uint32_t regval;

  /* Set the SysTick interrupt to the default priority */

  regval = getreg32(NVIC_SYSH12_15_PRIORITY);
  regval &= ~NVIC_SYSH_PRIORITY_PR15_MASK;
  regval |= (NVIC_SYSH_PRIORITY_DEFAULT << NVIC_SYSH_PRIORITY_PR15_SHIFT);
  putreg32(regval, NVIC_SYSH12_15_PRIORITY);

  /* Make sure that the SYSTICK clock source is set correctly */

#if 0 /* Does not work.  Comes up with HCLK source and I can't change it */
  regval = getreg32(NVIC_SYSTICK_CTRL);
#ifdef CONFIG_STM32L4_SYSTICK_HCLKd8
  regval &= ~NVIC_SYSTICK_CTRL_CLKSOURCE;
#else
  regval |= NVIC_SYSTICK_CTRL_CLKSOURCE;
#endif
  putreg32(regval, NVIC_SYSTICK_CTRL);
#endif

  /* Configure SysTick to interrupt at the requested rate */

  putreg32(SYSTICK_RELOAD, NVIC_SYSTICK_RELOAD);

  /* Attach the timer interrupt vector */

  (void)irq_attach(STM32L4_IRQ_SYSTICK, (xcpt_t)up_timerisr, NULL);

  /* Enable SysTick interrupts */

  putreg32((NVIC_SYSTICK_CTRL_CLKSOURCE | NVIC_SYSTICK_CTRL_TICKINT |
            NVIC_SYSTICK_CTRL_ENABLE), NVIC_SYSTICK_CTRL);

  /* And enable the timer interrupt */

  up_enable_irq(STM32L4_IRQ_SYSTICK);
}


/****************************************************************************
 * Function:  arm_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/

int up_sys_timerisr(int irq, uint32_t *regs)
{
    HAL_IncTick();
    return 0;
}

void up_sys_timer_initialize(void)
{
    uint32_t regval;
#if 0
    /* Set the SysTick interrupt to the default priority */
    regval = getreg32(NVIC_SYSH12_15_PRIORITY);
    regval &= ~NVIC_SYSH_PRIORITY_PR15_MASK;
    //regval |= (NVIC_SYSH_PRIORITY_DEFAULT << NVIC_SYSH_PRIORITY_PR15_SHIFT);
    regval |= (NVIC_SYSH_PRIORITY_MIN << NVIC_SYSH_PRIORITY_PR15_SHIFT);
    putreg32(regval, NVIC_SYSH12_15_PRIORITY);

    /* Make sure that the SYSTICK clock source is set correctly */
#if 0 /* Does not work. Comes up with HCLK source and I can't change it */
    regval = getreg32(NVIC_SYSTICK_CTRL);
#ifdef CONFIG_STM32L4_SYSTICK_HCLKd8
    regval &= ~NVIC_SYSTICK_CTRL_CLKSOURCE;
#else
    regval |= NVIC_SYSTICK_CTRL_CLKSOURCE;
#endif
    putreg32(regval, NVIC_SYSTICK_CTRL);
#endif

    /* configure SysTick to interrupt at the requested rate */

    putreg32((SYSTICK_RELOAD)/10, NVIC_SYSTICK_RELOAD);
#endif
    /* Attach the timer interrupt vector */
    (void)irq_attach(STM32L4_IRQ_SYSTICK, (xcpt_t)up_sys_timerisr, NULL);

    /* Enable SysTick interrupt */
    putreg32((NVIC_SYSTICK_CTRL_CLKSOURCE | NVIC_SYSTICK_CTRL_TICKINT |
              NVIC_SYSTICK_CTRL_ENABLE), NVIC_SYSTICK_CTRL);

    /* And enable the timer interrupt */

    up_enable_irq(STM32L4_IRQ_SYSTICK);
}

void up_sys_timer_deinitialize(void)
{
    (void)irq_detach(STM32L4_IRQ_SYSTICK);
    up_disable_irq(STM32L4_IRQ_SYSTICK);
}

/****************************************************************************
 * Function:  arm_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/
static TIM_HandleTypeDef htim17;

int up_hal_timerisr(int irq, uint32_t *regs)
{
  __HAL_TIM_CLEAR_IT(&htim17, TIM_IT_UPDATE);
  HAL_IncTick();
  return 0;
}

void up_hal_timer_initialize(void)
{
  __HAL_RCC_TIM17_CLK_ENABLE();

  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 1200 - 1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 100 - 1;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if(HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    printf("HAL TIM Base Init Error 1\n");
  }

  HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);

  (void)irq_attach(STM32L4_IRQ_TIM17, (xcpt_t)up_hal_timerisr, NULL);
  up_enable_irq(STM32L4_IRQ_TIM17);

  if(HAL_TIM_Base_Start_IT(&htim17) != HAL_OK)
  {
    printf("HAL TIM Base Start IT Error 2\n");
  }
}


void up_hal_timer_deinitialize(void)
{
  __HAL_RCC_TIM17_CLK_DISABLE();
  HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM17_IRQn);
  (void)irq_detach(STM32L4_IRQ_TIM17);
  up_disable_irq(STM32L4_IRQ_TIM17);
}

void supress_hal_tick(void)
{
  HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM17_IRQn);
}

void enable_hal_tick(void)
{
  HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
}
