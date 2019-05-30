/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-m/up_doirq.c
 *
 *   Copyright (C) 2009, 2011, 2013-2014 Gregory Nutt. All rights reserved.
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
#include <assert.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
#if CONFIG_ARCH_INTERRUPTSTACK > 7
extern uint32_t g_nestlevel; /* Initial top of interrupt stack */
#endif
/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

uint32_t *up_doirq(int irq, uint32_t *regs)
{
#if CONFIG_ARCH_INTERRUPTSTACK > 7
	irqstate_t flags;
	uint32_t stack_remain;
#endif
	board_led_on(LED_INIRQ);
#ifdef CONFIG_SUPPRESS_INTERRUPTS
	PANIC();
#else

#if CONFIG_ARCH_INTERRUPTSTACK > 7

#ifdef CONFIG_ARCH_NESTED_INTERRUPT_STACKCHECK

	/* check remaining stack available, assert if stack overflows */

	stack_remain = (CONFIG_ARCH_INTERRUPTSTACK & ~3) - up_check_intstack();
	if (stack_remain < 8) {
		lldbg("STACK OVERFLOW!!\n");
		PANIC();
	}
#endif
	/* Current regs non-zero indicates that we are processing an interrupt;
	 * regs holds the state of the interrupted logic; current_regs holds the
	 * state of the interrupted user task.  current_regs should, therefor,
	 * only be modified for outermost interrupt handler (when g_nestlevel == 0)
	 */

	flags = irqsave();

	if (g_nestlevel == 0) {
		current_regs = regs;
	}

	g_nestlevel++;

	irqrestore(flags);
#else
	uint32_t *savestate;

	/* Nested interrupts are not supported in this implementation.  If you want
	 * to implement nested interrupts, you would have to (1) change the way that
	 * current_regs is handled and (2) the design associated with
	 * CONFIG_ARCH_INTERRUPTSTACK.  The savestate variable will not work for
	 * that purpose as implemented here because only the outermost nested
	 * interrupt can result in a context switch (it can probably be deleted).
	 */
	savestate = (uint32_t *)current_regs;
	current_regs = regs;
	/* Acknowledge the interrupt */

	up_ack_irq(irq);
#endif

	/* Deliver the IRQ */

	irq_dispatch(irq, regs);

#if CONFIG_ARCH_INTERRUPTSTACK > 7
	/* Context switches are indicated by the returned value of this function.
	 * If a context switch occurred while processing the interrupt then
	 * current_regs may have change value.  If we return any value different
	 * from the input regs, then the lower level will know that a context
	 * switch occurred during interrupt processing.  Context switching should
	 * only be performed when the outermost interrupt handler returns.
	 */

	flags = irqsave();

	g_nestlevel--;

	if (g_nestlevel == 0) {
		regs = (uint32_t*)current_regs;
		current_regs = NULL;
	}

	/* Note that interrupts are left disabled.  This needed if context switch
	 * will be performed.  But, any case, the correct interrupt state should
	 * be restored when returning from the interrupt.
	 */

#else
	/* If a context switch occurred while processing the interrupt then
	 * current_regs may have change value.  If we return any value different
	 * from the input regs, then the lower level will know that a context
	 * switch occurred during interrupt processing.
	 */

	regs = (uint32_t *)current_regs;

	/* Restore the previous value of current_regs.  NULL would indicate that
	 * we are no longer in an interrupt handler.  It will be non-NULL if we
	 * are returning from a nested interrupt.
	 */

	current_regs = savestate;
#endif
#endif
	board_led_off(LED_INIRQ);
	return regs;
}
