/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/stm32h745/stm32h745_haltick.c
 *
 *   Copyright (C) 2009, 2011-2015 Gregory Nutt. All rights reserved.
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
#include <tinyara/mm/heap_regioninfo.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/init.h>
#include <tinyara/irq.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"
#include "nvic.h"

#include <stm32h7xx_hal.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
TIM_HandleTypeDef   htim17;

static int stm32h745_tim17_update(int irq, FAR void *context, FAR void *arg)
{
  __HAL_TIM_CLEAR_IT(&htim17, TIM_IT_UPDATE);
  HAL_IncTick();

  return OK;
}

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/
int stm32h745_haltick_init(void)
{
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock;

    uint32_t              uwPrescalerValue;
    uint32_t              pFLatency;

    uint32_t TickPriority = TICK_INT_PRIORITY;

    /*Configure the TIM17 IRQ priority */
    if (TickPriority < (1UL << __NVIC_PRIO_BITS))
    {
        /*
        * Removed to use default interrupt priority
        HAL_NVIC_SetPriority(TIM17_IRQn, TickPriority ,0U);
        */

        /* Enable the TIM17 global Interrupt */
        HAL_NVIC_EnableIRQ(TIM17_IRQn);
        uwTickPrio = TickPriority;

        irq_attach(STM32H745_IRQ_TIM17, stm32h745_tim17_update, NULL);
        up_enable_irq(STM32H745_IRQ_TIM17);
    }
    else
    {
        return ERROR;
    }

    /* Enable TIM17 clock */
    __HAL_RCC_TIM17_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Compute TIM17 clock */
    uwTimclock = 2*HAL_RCC_GetPCLK2Freq();

    /* Compute the prescaler value to have TIM17 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM17 */
    htim17.Instance = TIM17;

    /* Initialize TIMx peripheral as follow:
    + Period = [(TIM17CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    htim17.Init.Period = (1000000U / 1000U) - 1U;
    htim17.Init.Prescaler = uwPrescalerValue;
    htim17.Init.ClockDivision = 0;
    htim17.Init.CounterMode = TIM_COUNTERMODE_UP;

    if(HAL_TIM_Base_Init(&htim17) == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        if(HAL_TIM_Base_Start_IT(&htim17) == HAL_OK)
        {
            return OK;
        }
        else
        {
            return ERROR;
        }
    }

    /* Return function status */
    return OK;
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM17 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_SuspendTick(void)
{
    /* Disable TIM17 update Interrupt */
    __HAL_TIM_DISABLE_IT(&htim17, TIM_IT_UPDATE);
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM17 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
    /* Enable TIM17 Update interrupt */
    __HAL_TIM_ENABLE_IT(&htim17, TIM_IT_UPDATE);
}












 




