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
 * kernel/irq/irq_attach.c
 *
 *   Copyright (C) 2007-2008, 2010, 2012 Gregory Nutt. All rights reserved.
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

#include <tinyara/irq.h>

#include "irq/irq.h"

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
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: irq_attach
 *
 * Description:
 *   Configure the IRQ subsystem so that IRQ number 'irq' is dispatched to
 *   'isr'
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_IRQ_INFO
int irq_attach_withname(int irq, xcpt_t isr, FAR void *arg, const char *name)
#else
int irq_attach(int irq, xcpt_t isr, FAR void *arg)
#endif
{
#if NR_IRQS > 0
	int ret = ERROR;

	if ((unsigned)irq < NR_IRQS) {
		irqstate_t state;

		/* If the new ISR is NULL, then the ISR is being detached.
		 * In this case, disable the ISR and direct any interrupts
		 * to the unexpected interrupt handler.
		 */

		state = enter_critical_section();
		if (isr == NULL) {
			/* Disable the interrupt if we can before detaching it.  We might
			 * not be able to do this if:  (1) the device does not have a
			 * centralized interrupt controller (so up_disable_irq() is not
			 * supported).  Or (2) if the device has different number for vector
			 * numbers and IRQ numbers (in that case, we don't know the correct
			 * IRQ number to use to disable the interrupt).  In those cases, the
			 * code will just need to be careful that it disables all interrupt
			 * sources before detaching from the interrupt vector.
			 */

#if !defined(CONFIG_ARCH_NOINTC) && !defined(CONFIG_ARCH_VECNOTIRQ)
			up_disable_irq(irq);
#endif
			/* Detaching the ISR really means re-attaching it to the
			 * unexpected exception handler.
			 */

			isr = irq_unexpected_isr;
			arg = NULL;
		}

		/* Save the new ISR in the table. */

		g_irqvector[irq].handler = isr;
		g_irqvector[irq].arg     = arg;
#ifdef CONFIG_DEBUG_IRQ_INFO
		/* Reset the irq counter to 0 and it's applicable for both irq_attach and irq_detach */
		g_irqvector[irq].count   = 0;
		if (name != NULL) {
			strncpy(g_irqvector[irq].irq_name, name, MAX_IRQNAME_SIZE);
			g_irqvector[irq].irq_name[MAX_IRQNAME_SIZE] = '\0';
		} else {
			/* irq_detach will set isr as NULL, thus name would be passed as NULL. Set the irq_name to Null */
			g_irqvector[irq].irq_name[0] = '\0';
		}
#endif
		leave_critical_section(state);
		ret = OK;
	}

	return ret;
#else
	return OK;
#endif
}
