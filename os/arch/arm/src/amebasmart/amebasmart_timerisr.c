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

#include "up_internal.h"
#include "gic.h"
#include "arch_timer.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  up_timerisr
 *
 * Description:
 *   The timer ISR will perform a variety of services for various portions
 *   of the systems.
 *
 ****************************************************************************/

int up_timerisr(int irq, uint32_t *regs)
{
    /* Clear interrupt */
    uint32_t delta_ticks;
    uint64_t last_cycle;

    last_cycle = arm_arch_timer_compare();

    if (arm_arch_timer_count() < last_cycle) {
      return -1;
    } else {
      delta_ticks = (uint32_t)((arm_arch_timer_count() - last_cycle) / pdTICKS_TO_CNT) + 1;
    }

    u32 ticks_to_process = delta_ticks;
    while (ticks_to_process > 0) {
      /* process missing ticks */
      sched_process_timer();
      ticks_to_process--;
    }

    arm_arch_timer_set_compare(last_cycle + delta_ticks * pdTICKS_TO_CNT);
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
  /* Disable GPT interrupts at the GIC */
  up_disable_irq(ARM_ARCH_TIMER_IRQ);

  /* Attach the timer interrupt vector */
  irq_attach(ARM_ARCH_TIMER_IRQ, (xcpt_t)up_timerisr, NULL);

  /* Only enable the timer for CPU0 */
  if (up_cpu_index() == 0) {
    arm_arch_timer_set_compare(arm_arch_timer_count() + pdTICKS_TO_CNT);
    arm_arch_timer_enable(1);
  }

  /* And enable the timer interrupt at the GIC */
  up_prioritize_irq(ARM_ARCH_TIMER_IRQ, 224);
  up_enable_irq(ARM_ARCH_TIMER_IRQ);
}

int up_timer_disable(void)
{
  arm_arch_timer_enable(0);
  return 0;
}

int up_timer_enable(void)
{
	/* When wake from pg, arm timer has been reset, so a new compare value is necessary to
	trigger an timer interrupt */
  arm_arch_timer_set_compare(arm_arch_timer_count() + pdTICKS_TO_CNT);
  arm_arch_timer_enable(1);
  return 0;
}
