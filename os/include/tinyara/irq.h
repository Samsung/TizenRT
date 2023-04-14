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
 *
 *   Copyright (C) 2007-2011, 2013 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_IRQ_H
#define __INCLUDE_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifndef __ASSEMBLY__
#include <assert.h>
#endif

#include <sys/types.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* IRQ detach is a convenience definition.  Detaching an interrupt handler
 * is equivalent to setting a NULL interrupt handler.
 */

#ifndef __ASSEMBLY__
#ifdef CONFIG_DEBUG_IRQ_INFO
#define irq_attach(irq, handler, arg) irq_attach_withname(irq, handler, arg, #handler)
#define irq_detach(irq) irq_attach_withname(irq, NULL, NULL, NULL)
#else
#define irq_detach(irq) irq_attach(irq, NULL, NULL)
#endif
#endif

#define NO_FIN_DATA -999
/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This struct defines the way the registers are stored */

#ifndef __ASSEMBLY__
typedef int (*xcpt_t)(int irq, FAR void *context, FAR void *arg);
#endif

/* Now include architecture-specific types */

#include <arch/irq.h>

/****************************************************************************
 * Public Variables
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: irq_attach
 *
 * Description:
 *   Configure the IRQ subsystem so that IRQ number 'irq' is dispatched to
 *   'isr' with argument 'arg'
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_IRQ_INFO
int irq_attach_withname(int irq, xcpt_t isr, FAR void *arg, const char *name);
#else
int irq_attach(int irq, xcpt_t isr, FAR void *arg);
#endif

#ifdef CONFIG_DEBUG_IRQ_INFO

/****************************************************************************
 * Name: irq_info
 *
 * Description:
 *   Display the registered IRQ numbers and it's isr details to user
 *
 ****************************************************************************/

void irq_info(void);

#endif

#ifdef CONFIG_IRQCOUNT
irqstate_t enter_critical_section(void);
#else
#  define enter_critical_section() irqsave()
#endif

#ifdef CONFIG_IRQCOUNT
void leave_critical_section(irqstate_t flags);
#else
#  define leave_critical_section(f) irqrestore(f)
#endif

/**
 * @cond
 * @internal
 */
int fin_notify(pid_t pid, int data);
/**
 * @endcond
 */
/**
 * @brief wait for kenrel irq event
 * @details @b #include <tinyara/irq.h> \n
 * @return On success, OK is returned. On failure, ERROR will be returned.
 * SYSTEM CALL API \n
 * @since TizenRT v3.1 PRE
 */
int fin_wait(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif							/* __INCLUDE_IRQ_H */
