/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <arch/irq.h>

#include "nvic.h"
#include "chip.h"
#include "ram_vectors.h"
#include "up_arch.h"
#include "up_internal.h"

#define DEFPRIORITY32 \
	(NVIC_SYSH_PRIORITY_DEFAULT << 24 | \
	 NVIC_SYSH_PRIORITY_DEFAULT << 16 | \
	 NVIC_SYSH_PRIORITY_DEFAULT << 8  | \
	 NVIC_SYSH_PRIORITY_DEFAULT)

volatile uint32_t *current_regs;

static inline uintptr_t qemu_mps2_external_priority_addr(int irq)
{
	return ARMV8M_NVIC_BASE + 0x0400 + (uintptr_t)(irq - QEMU_MPS2_AN505_IRQ_FIRST);
}

static inline uintptr_t qemu_mps2_system_priority_addr(int irq)
{
	return ARMV8M_NVIC_BASE + 0x0d14 + (uintptr_t)irq;
}

static inline void qemu_mps2_prioritize_syscall(int priority)
{
	putreg8((uint8_t)priority,
		qemu_mps2_system_priority_addr(QEMU_MPS2_AN505_IRQ_SVCALL));
}

static int qemu_mps2_irqinfo(int irq, uintptr_t *regaddr, uint32_t *bit,
			     uintptr_t offset)
{
	int n;

	DEBUGASSERT(irq >= QEMU_MPS2_AN505_IRQ_NMI && irq < NR_IRQS);

	if (irq >= QEMU_MPS2_AN505_IRQ_FIRST) {
		n = irq - QEMU_MPS2_AN505_IRQ_FIRST;
		*regaddr = NVIC_IRQ_ENABLE(n) + offset;
		*bit = (uint32_t)1 << (n & 0x1f);
		return OK;
	}

	*regaddr = NVIC_SYSHCON;
	if (irq == QEMU_MPS2_AN505_IRQ_MEMFAULT) {
		*bit = NVIC_SYSHCON_MEMFAULTENA;
	} else if (irq == QEMU_MPS2_AN505_IRQ_BUSFAULT) {
		*bit = NVIC_SYSHCON_BUSFAULTENA;
	} else if (irq == QEMU_MPS2_AN505_IRQ_USAGEFAULT) {
		*bit = NVIC_SYSHCON_USGFAULTENA;
	} else if (irq == QEMU_MPS2_AN505_IRQ_SYSTICK) {
		*regaddr = NVIC_SYSTICK_CTRL;
		*bit = NVIC_SYSTICK_CTRL_TICKINT;
	} else {
		return ERROR;
	}

	return OK;
}

void up_irqinitialize(void)
{
	uintptr_t regaddr;
	int num_priority_registers;

#ifdef CONFIG_ARCH_RAMVECTORS
	up_ramvec_initialize();
#endif

	num_priority_registers =
		(((getreg32(NVIC_ICTR) & NVIC_ICTR_INTLINESNUM_MASK) >>
		  NVIC_ICTR_INTLINESNUM_SHIFT) + 1) * 8;

	regaddr = NVIC_IRQ0_3_PRIORITY;
	while (num_priority_registers-- > 0) {
		putreg32(DEFPRIORITY32, regaddr);
		regaddr += sizeof(uint32_t);
	}

	current_regs = NULL;

	irq_attach(QEMU_MPS2_AN505_IRQ_SVCALL, up_svcall, NULL);
	irq_attach(QEMU_MPS2_AN505_IRQ_HARDFAULT, up_hardfault, NULL);
	irq_attach(QEMU_MPS2_AN505_IRQ_MEMFAULT, up_memfault, NULL);
	irq_attach(QEMU_MPS2_AN505_IRQ_BUSFAULT, up_busfault, NULL);
	irq_attach(QEMU_MPS2_AN505_IRQ_USAGEFAULT, up_usagefault, NULL);
	up_enable_irq(QEMU_MPS2_AN505_IRQ_MEMFAULT);
	up_enable_irq(QEMU_MPS2_AN505_IRQ_BUSFAULT);
	up_enable_irq(QEMU_MPS2_AN505_IRQ_USAGEFAULT);

#ifdef CONFIG_ARCH_IRQPRIO
	up_prioritize_irq(QEMU_MPS2_AN505_IRQ_PENDSV, NVIC_SYSH_PRIORITY_MIN);
	up_prioritize_irq(QEMU_MPS2_AN505_IRQ_SVCALL, NVIC_SYSH_SVCALL_PRIORITY);
#else
	putreg8((uint8_t)NVIC_SYSH_PRIORITY_MIN,
		qemu_mps2_system_priority_addr(QEMU_MPS2_AN505_IRQ_PENDSV));
	qemu_mps2_prioritize_syscall(NVIC_SYSH_SVCALL_PRIORITY);
#endif

#ifndef CONFIG_SUPPRESS_INTERRUPTS
	irqenable();
#endif
}

void up_disable_irq(int irq)
{
	uintptr_t regaddr;
	uint32_t bit;
	uint32_t regval;

	if (qemu_mps2_irqinfo(irq, &regaddr, &bit,
			      NVIC_IRQ0_31_CLEAR - NVIC_IRQ0_31_ENABLE) != OK) {
		return;
	}

	if (irq < QEMU_MPS2_AN505_IRQ_FIRST) {
		regval = getreg32(regaddr);
		regval &= ~bit;
		putreg32(regval, regaddr);
		return;
	}

	putreg32(bit, regaddr);
}

void up_enable_irq(int irq)
{
	uintptr_t regaddr;
	uint32_t bit;
	uint32_t regval;

	if (qemu_mps2_irqinfo(irq, &regaddr, &bit, 0) != OK) {
		return;
	}

	if (irq < QEMU_MPS2_AN505_IRQ_FIRST) {
		regval = getreg32(regaddr);
		regval |= bit;
		putreg32(regval, regaddr);
		return;
	}

	putreg32(bit, regaddr);
}

void up_ack_irq(int irq)
{
	UNUSED(irq);
}

#ifdef CONFIG_ARCH_IRQPRIO
int up_prioritize_irq(int irq, int priority)
{
	DEBUGASSERT(irq >= QEMU_MPS2_AN505_IRQ_MEMFAULT && irq < NR_IRQS &&
		    (unsigned)priority <= NVIC_SYSH_PRIORITY_MIN);

	if (irq >= QEMU_MPS2_AN505_IRQ_FIRST) {
		putreg8((uint8_t)priority, qemu_mps2_external_priority_addr(irq));
		return OK;
	}

	if (irq >= QEMU_MPS2_AN505_IRQ_MEMFAULT) {
		putreg8((uint8_t)priority, qemu_mps2_system_priority_addr(irq));
		return OK;
	}

	return ERROR;
}
#endif
