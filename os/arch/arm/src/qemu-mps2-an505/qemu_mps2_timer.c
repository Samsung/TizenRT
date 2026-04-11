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
#include <time.h>

#include <tinyara/arch.h>
#include <arch/irq.h>

#include "nvic.h"
#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"

#define SYSTICK_RELOAD ((QEMU_MPS2_AN505_SYSTICK_CLK / CLK_TCK) - 1)

#if SYSTICK_RELOAD > 0x00ffffff
#error SYSTICK_RELOAD exceeds the range of the RELOAD register
#endif

int up_timerisr(int irq, uint32_t *regs)
{
	UNUSED(irq);
	UNUSED(regs);

	sched_process_timer();
	return 0;
}

void up_timer_initialize(void)
{
	putreg32(0, NVIC_SYSTICK_CTRL);
	putreg32(0, NVIC_SYSTICK_CURRENT);
	putreg32(SYSTICK_RELOAD, NVIC_SYSTICK_RELOAD);

	(void)irq_attach(QEMU_MPS2_AN505_IRQ_SYSTICK, (xcpt_t)up_timerisr, NULL);

#ifdef CONFIG_ARCH_IRQPRIO
	up_prioritize_irq(QEMU_MPS2_AN505_IRQ_SYSTICK, NVIC_SYSH_PRIORITY_MIN);
#endif

	putreg32(NVIC_SYSTICK_CTRL_CLKSOURCE |
		 NVIC_SYSTICK_CTRL_TICKINT |
		 NVIC_SYSTICK_CTRL_ENABLE,
		 NVIC_SYSTICK_CTRL);
}

int up_timer_disable(void)
{
	uint32_t regval;

	regval = getreg32(NVIC_SYSTICK_CTRL);
	regval &= ~(NVIC_SYSTICK_CTRL_ENABLE | NVIC_SYSTICK_CTRL_TICKINT);
	putreg32(regval, NVIC_SYSTICK_CTRL);

	return OK;
}

int up_timer_enable(void)
{
	uint32_t regval;

	putreg32(0, NVIC_SYSTICK_CURRENT);

	regval = getreg32(NVIC_SYSTICK_CTRL);
	regval |= NVIC_SYSTICK_CTRL_CLKSOURCE |
		  NVIC_SYSTICK_CTRL_TICKINT |
		  NVIC_SYSTICK_CTRL_ENABLE;
	putreg32(regval, NVIC_SYSTICK_CTRL);

	return OK;
}
