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
 * kernel/irq/irq_dispatch.c
 *
 *   Copyright (C) 2007, 2008 Gregory Nutt. All rights reserved.
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

#include <debug.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>

#include "irq/irq.h"

#ifdef CONFIG_IRQ_SCHED_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: irq_dispatch
 *
 * Description:
 *   This function must be called from the achitecture-specific logic in
 *   order to dispatch an interrupt to the appropriate, registered handling
 *   logic.
 *
 ***************************************************************************/
void irq_dispatch(int irq, FAR void *context)
{
	xcpt_t vector;
	FAR void *arg;

	/* Perform some sanity checks */

#if NR_IRQS > 0
	if ((unsigned)irq >= NR_IRQS || g_irqvector[irq].handler == NULL) {
		vector = irq_unexpected_isr;
		arg    = NULL;
	} else {
		vector = g_irqvector[irq].handler;
		arg    = g_irqvector[irq].arg;
#ifdef CONFIG_DEBUG_IRQ_INFO
		g_irqvector[irq].count++;
#endif
	}
#else
	vector = irq_unexpected_isr;
	arg    = NULL;
#endif

#ifdef CONFIG_IRQ_SCHED_HISTORY
	save_irq_scheduling_status(irq, (void *)vector);
#endif

	/* Then dispatch to the interrupt handler */

	vector(irq, context, arg);
}
