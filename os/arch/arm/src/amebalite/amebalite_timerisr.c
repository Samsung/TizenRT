/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#include "chip.h"

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
extern uint32_t SystemCoreClock;

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
 * will fit in the reload register.
 */

#define SYSTICK_RELOAD ((SystemCoreClock / CLK_TCK) - 1)

#if SYSTICK_RELOAD > 0x00ffffff
#  error SYSTICK_RELOAD exceeds the range of the RELOAD register
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  amebalite_timerisr
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
 * Function:  up_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/

void up_timer_initialize(void)
{
  /* Set the SysTick interrupt to the min priority */ 
#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(AMEBALITE_IRQ_SYSTICK, NVIC_SYSH_PRIORITY_DEFAULT);
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

  (void)irq_attach(AMEBALITE_IRQ_SYSTICK, (xcpt_t)up_timerisr, NULL);

  /* Enable SysTick interrupts */

  putreg32((NVIC_SYSTICK_CTRL_CLKSOURCE | NVIC_SYSTICK_CTRL_TICKINT |
            NVIC_SYSTICK_CTRL_ENABLE), NVIC_SYSTICK_CTRL);

  /* And enable the timer interrupt */

  //up_enable_irq(AMEBALITE_IRQ_SYSTICK);
}
