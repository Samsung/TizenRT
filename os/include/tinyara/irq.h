/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#ifndef __ASSEMBLY__
# include <stdint.h>
#include <assert.h>
# include <arch/irq.h>
#endif

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
/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__
/* This type is an unsigned integer type large enough to hold the largest
 * IRQ number.
 */

#if NR_IRQS <= 256
typedef uint8_t irq_t;
#elif NR_IRQS <= 65536
typedef uint16_t irq_t;
#else
typedef uint32_t irq_t;
#endif

/* This struct defines the way the registers are stored */

/* This type is an unsigned integer type large enough to hold the largest
 * mapped vector table index.
 */

#ifdef CONFIG_ARCH_MINIMAL_VECTORTABLE
#if CONFIG_ARCH_NUSER_INTERRUPTS <= 256
typedef uint8_t irq_mapped_t;
#elif CONFIG_ARCH_NUSER_INTERRUPTS <= 65536
typedef uint16_t irq_mapped_t;
#else
typedef uint32_t irq_mapped_t;
#endif
#endif /* CONFIG_ARCH_MINIMAL_VECTORTABLE */

/* This struct defines the form of an interrupt service routine */

typedef CODE int (*xcpt_t)(int irq, FAR void *context, FAR void *arg);
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

#ifdef CONFIG_ARCH_MINIMAL_VECTORTABLE
/* This is the interrupt vector mapping table.  This must be provided by
 * architecture specific logic if CONFIG_ARCH_MINIMAL_VECTORTABLE is define
 * in the configuration.
 *
 * REVISIT: Currently declared in sched/irq/irq.h.  This declaration here
 * introduces a circular dependency since it depends on NR_IRQS which is
 * defined in arch/irq.h but arch/irq.h includes nuttx/irq.h and we get
 * here with NR_IRQS undefined.
 */

/* EXTERN const irq_mapped_t g_irqmap[NR_IRQS]; */
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

#ifdef CONFIG_IRQCHAIN
int irqchain_detach(int irq, xcpt_t isr, FAR void *arg);
#else
#  define irqchain_detach(irq, isr, arg) irq_detach(irq)
#endif

/****************************************************************************
 * Name: enter_critical_section
 *
 * Description:
 *   If SMP is enabled:
 *     Take the CPU IRQ lock and disable interrupts on all CPUs.  A thread-
 *     specific counter is increment to indicate that the thread has IRQs
 *     disabled and to support nested calls to enter_critical_section().
 *
 *     NOTE: Most architectures do not support disabling all CPUs from one
 *     CPU.  ARM is an example.  In such cases, logic in
 *     enter_critical_section() will still manage entrance into the
 *     protected logic using spinlocks.
 *
 *   If SMP is not enabled:
 *     This function is equivalent to up_irq_save().
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   An opaque, architecture-specific value that represents the state of
 *   the interrupts prior to the call to enter_critical_section();
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_INSTRUMENTATION_CSECTION)
irqstate_t enter_critical_section(void);
#else
#  define enter_critical_section(f) irqsave(f)
#endif

/****************************************************************************
 * Name: leave_critical_section
 *
 * Description:
 *     This function is equivalent to up_irq_restore().
 *
 * Input Parameters:
 *   flags - The architecture-specific value that represents the state of
 *           the interrupts prior to the call to enter_critical_section();
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_INSTRUMENTATION_CSECTION)
void leave_critical_section(irqstate_t flags);
#else
#  define leave_critical_section(f) irqrestore(f)
#endif

/****************************************************************************
 * Name: arm_spin_lock_irqsave
 *
 *     This function is equivalent to up_irq_restore().
 *
 * Input Parameters:
 *   flags - The architecture-specific value that represents the state of
 *           the interrupts prior to the call to enter_critical_section();
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_INSTRUMENTATION_CSECTION)
void leave_critical_section(irqstate_t flags);
#else
#define leave_critical_section(f) irqrestore(f)
#endif

/****************************************************************************
 * Name: arm_spin_lock_irqsave
 *
 * Description:
 *     This function is equivalent to enter_critical_section().
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   An opaque, architecture-specific value that represents the state of
 *   the interrupts prior to the call to arm_spin_lock_irqsave();
 *
 ****************************************************************************/

#if defined(CONFIG_SPINLOCK_IRQ) && \
    defined(CONFIG_ARCH_GLOBAL_IRQDISABLE)
irqstate_t arm_spin_lock_irqsave(void);
#else
#  define arm_spin_lock_irqsave(f) enter_critical_section(f)
#endif

/****************************************************************************
 * Name: arm_spin_unlock_irqrestore
 *
 * Description:
 *     This function is equivalent to leave_critical_section().
 *
 * Input Parameters:
 *   flags - The architecture-specific value that represents the state of
 *           the interrupts prior to the call to arm_spin_lock_irqsave();
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#if defined(CONFIG_SPINLOCK_IRQ) && \
    defined(CONFIG_ARCH_GLOBAL_IRQDISABLE)
void arm_spin_unlock_irqrestore(irqstate_t flags);
#else
#  define arm_spin_unlock_irqrestore(f) leave_critical_section(f)
#endif

/****************************************************************************
 * Name: irq_info
 *
 * Description:
 *   Display the registered IRQ numbers and it's isr details to user
 *
 ****************************************************************************/

void irq_info(void);

#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_IRQ_H */
