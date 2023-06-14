/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_timerisr.c
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

#include <stdint.h>
#include <time.h>
#include <assert.h>

#include <tinyara/arch.h>
#include <arch/irq.h>

#include "arm_internal.h"
#include "gic.h"
#include "arch_timer.h"

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
#define GENERICTIMERFREQ 50000000
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
//Relate to pdTICKS_TO_CNT in system_sheipa.c
#define SYSTICK_RELOAD ((GENERICTIMERFREQ / CLK_TCK) - 1)

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

    /* Clear interrupt */
    uint32_t delta_ticks;
    uint64_t last_cycle;

    last_cycle = arm_arch_timer_compare();

    delta_ticks = 1;

    arm_arch_timer_set_compare(last_cycle + delta_ticks * SYSTICK_RELOAD);
    return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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
  // uint32_t regval;
  // uint32_t cr;

  /* Disable GPT interrupts at the GIC */
  up_disable_irq(ARM_ARCH_TIMER_IRQ);
  /* Configure as a (rising) edge-triggered interrupt */
  // arm_gic_irq_trigger(ARM_ARCH_TIMER_IRQ, true);

  /* Attach the timer interrupt vector */
  irq_attach(ARM_ARCH_TIMER_IRQ, (xcpt_t)up_timerisr, NULL);

  //arm_arch_timer_count
	arm_arch_timer_set_compare(arm_arch_timer_count() + SYSTICK_RELOAD);
	arm_arch_timer_enable(1);

  /* And enable the timer interrupt at the GIC */
  up_prioritize_irq(ARM_ARCH_TIMER_IRQ, 224);
  up_enable_irq(ARM_ARCH_TIMER_IRQ);
}
