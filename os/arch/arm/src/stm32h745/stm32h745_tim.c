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
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/stm32h745/stm32h745_tim.c
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

#include <tinyara/timer.h>
#include <tinyara/irq.h>
#include <arch/board/board.h>

#include "nvic.h"
#include "clock/clock.h"
#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"
#include <stm32h7xx_hal.h>

#if defined(CONFIG_TIMER)
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define TIMER_SYSTEMFREQ 200000000

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct stm32h745_tim_lowerhalf_s 
{
  const struct timer_ops_s *ops;  /* Lowerhalf operations */
  TIM_HandleTypeDef htim;
  bool started;       /* True: Timer is started */
  tccb_t callback;
  void *arg;          /* Argument passed to upper half callback */
  uint32_t timeout;
  int irq;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int  stm32h745_tim_start(struct timer_lowerhalf_s *lower);
static int  stm32h745_tim_stop(struct timer_lowerhalf_s *lower);
static int  stm32h745_tim_getstatus(struct timer_lowerhalf_s *lower, struct timer_status_s *status);
static int  stm32h745_tim_settimeout(struct timer_lowerhalf_s *lower, uint32_t timeout);
static void stm32h745_tim_setcallback(struct timer_lowerhalf_s *lower, CODE tccb_t callback, void *arg);
static int  stm32h745_tim_ioctl(struct timer_lowerhalf_s *lower, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct timer_ops_s g_timer_ops = 
{
  .start = stm32h745_tim_start,
  .stop = stm32h745_tim_stop,
  .getstatus = stm32h745_tim_getstatus,
  .settimeout = stm32h745_tim_settimeout,
  .setcallback = stm32h745_tim_setcallback,
  .ioctl = stm32h745_tim_ioctl,
};

static struct stm32h745_tim_lowerhalf_s g_tim_lowerhalf = 
{
  .ops = &g_timer_ops,
  .started = false,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: stm32h745_tim_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Values:
 *
 ****************************************************************************/
static void stm32h745_tim_handler(int irq, FAR void *context, FAR void *arg)
{
  struct stm32h745_tim_lowerhalf_s *priv = (struct stm32h745_tim_lowerhalf_s *)arg;

  uint32_t next_interval_us = 0;
  DEBUGASSERT(priv);

  if(priv->callback != NULL)
  {
    priv->callback(&next_interval_us, priv->arg);
  }

  __HAL_TIM_CLEAR_IT(&priv->htim, TIM_IT_UPDATE);
}


/****************************************************************************
 * Name: stm32h745_tim_start
 *
 * Description:
 *   Start the timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int  stm32h745_tim_start(struct timer_lowerhalf_s *lower)
{
  struct stm32h745_tim_lowerhalf_s *priv = (struct stm32h745_tim_lowerhalf_s *)lower;

  DEBUGASSERT(priv);
  if (!priv)
  {
    return ERROR;
  }

  __HAL_TIM_SET_COUNTER(&priv->htim, 0);
  if(HAL_TIM_Base_Start_IT(&priv->htim) == HAL_OK)
  {
    return ERROR;
  }

  priv->started = true;
  return OK;
}

/****************************************************************************
 * Name: stm32h745_tim_stop
 *
 * Description:
 *   Stop the timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int  stm32h745_tim_stop(struct timer_lowerhalf_s *lower)
{
  struct stm32h745_tim_lowerhalf_s *priv = (struct stm32h745_tim_lowerhalf_s *)lower;

  DEBUGASSERT(priv);
  if (!priv)
  {
    return ERROR;
  }

  __HAL_TIM_SET_COUNTER(&priv->htim, 0);
  if(HAL_TIM_Base_Stop_IT(&priv->htim) == HAL_OK)
  {
    return ERROR;
  }

  priv->started = false;
  return OK;
}

/****************************************************************************
 * Name: stm32h745_tim_getstatus
 *
 * Description:
 *   get GPT status
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   status  - A pointer saved current GPT status
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int  stm32h745_tim_getstatus(struct timer_lowerhalf_s *lower, struct timer_status_s *status)
{
  struct stm32h745_tim_lowerhalf_s *priv = (struct stm32h745_tim_lowerhalf_s *)lower;

  if (priv->started)
  {
    status->flags |= TCFLAGS_ACTIVE;
  }

  if (priv->callback)
  {
    status->flags |= TCFLAGS_HANDLER;
  }

  status->timeleft = 0;

  return OK;
}

/****************************************************************************
 * Name: stm32h745_tim_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   timeout - The new timeout value in microseconds.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int  stm32h745_tim_settimeout(struct timer_lowerhalf_s *lower, uint32_t timeout)
{
  struct stm32h745_tim_lowerhalf_s *priv = (struct stm32h745_tim_lowerhalf_s *)lower;

  DEBUGASSERT(priv);
  if (!priv)
  {
    return ERROR;
  }

  priv->htim.Init.Prescaler = 19999;
  priv->htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  priv->htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  priv->htim.Init.RepetitionCounter = 0;
  priv->htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  priv->htim.Init.Period = (uint16_t)(((TIMER_SYSTEMFREQ / (priv->htim.Init.Prescaler + 1)) / 1000)*timeout - 1);

  if (HAL_TIM_Base_Init(&priv->htim) != HAL_OK)
  {
    return ERROR;
  }

  priv->timeout = timeout;
  return OK;
}

/****************************************************************************
 * Name: stm32h745_tim_setcallback
 *
 * Description:
 *   Set the user provided timeout callback.
 *
 * Input Parameters:
 *   lower    - A pointer the publicly visible representation of the
 *              "lower-half" driver state structure.
 *   callback - The new timer expiration function pointer. If this
 *              function pointer is NULL, then the reset-on-expiration
 *              behavior is restored,
 *  arg       - Argument that will be provided in the callback
 *
 * Returned Values:
 *   The previous timer expiration function pointer or NULL is there was
 *   no previous function pointer.
 *
 ****************************************************************************/
static void stm32h745_tim_setcallback(struct timer_lowerhalf_s *lower, CODE tccb_t callback, void *arg)
{
  struct stm32h745_tim_lowerhalf_s *priv = (struct stm32h745_tim_lowerhalf_s *)lower;
  
  DEBUGASSERT(priv);

  if(priv)
  {
    irqstate_t flags = irqsave();

    /* Save the new callback */
    priv->callback = callback;
    priv->arg = arg;

    irqrestore(flags);
  }
}

/****************************************************************************
 * Name: stm32h745_tim_ioctl
 *
 * Description:
 *   Any ioctl commands that are not recognized by the "upper-half" driver
 *   are forwarded to the lower half driver through this method.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *   cmd   - The ioctl command value
 *   arg   - The optional argument that accompanies the 'cmd'.  The
 *           interpretation of this argument depends on the particular
 *           command.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int  stm32h745_tim_ioctl(struct timer_lowerhalf_s *lower, int cmd, unsigned long arg)
{
  struct stm32h745_tim_lowerhalf_s *priv = (struct stm32h745_tim_lowerhalf_s *)lower;

  DEBUGASSERT(priv);
  if (!priv)
  {
    return ERROR;
  }

  int ret = ERROR;

  switch (cmd)
  {
  case TCIOC_SETDIV:
    break;
  case TCIOC_GETDIV:
    break;
  case TCIOC_SETMODE:
    break;
  case TCIOC_SETRESOLUTION:
    break;
  case TCIOC_SETIRQPRIO:
    break;
  case TCIOC_SETCLKSRC:
    break;
  default:
    break;
  }

  return ret;
}

/****************************************************************************
 * Public Function prototypes
 * This timer is testing system clock, expected 5msec toggle PB7
 ****************************************************************************/
int stm32h745_tim_init(const char *devpath, int timer)
{
  struct stm32h745_tim_lowerhalf_s *priv = NULL;
  priv = &g_tim_lowerhalf;

  switch (timer) 
  {
  case 15:  /* TIMER 15  */
    __HAL_RCC_TIM15_CLK_ENABLE();
    priv->htim.Instance = TIM15;
    priv->irq = STM32H745_IRQ_TIM15;
    break;

  case 16:  /* TIMER 16 */
    __HAL_RCC_TIM16_CLK_ENABLE();
    priv->htim.Instance = TIM16;
    priv->irq = STM32H745_IRQ_TIM16;
    break;

  default:
    priv = NULL;
    return ERROR;
  }

  irq_attach(priv->irq, stm32h745_tim_handler, priv);
  up_enable_irq(priv->irq);

  priv->started = false;
  priv->callback = NULL;

  if (!timer_register(devpath, (struct timer_lowerhalf_s *)priv))
  {
    return ERROR;
  }

  return OK;
}

#endif




