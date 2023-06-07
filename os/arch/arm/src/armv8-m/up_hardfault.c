/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv8-m/up_hardfault.c
 *
 *   Copyright (C) 2009, 2013 Gregory Nutt. All rights reserved.
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
#include <tinyara/arch.h>

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <tinyara/security_level.h>

#include <tinyara/userspace.h>
#include <arch/irq.h>

#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <tinyara/reboot_reason.h>
#include <arch/reboot_reason.h>
#endif

#include "up_arch.h"
#include "nvic.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* If CONFIG_ARMV8M_USEBASEPRI=n, then debug output from this file may
 * interfere with context switching!
 */


#define INSN_SVC0        0xdf00	/* insn: svc 0 */

uint32_t system_exception_location;
/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: print_hardfault_detail
 ****************************************************************************/

static inline void print_hardfault_detail(int irq, uint32_t *regs)
{
	/* Dump some hard fault info */

	lldbg("PANIC!!! Hard fault: %08x\n", getreg32(NVIC_HFAULTS));
	lldbg("  IRQ: %d regs: %p\n", irq, regs);
	lldbg("  BASEPRI: %08x PRIMASK: %08x IPSR: %08x CONTROL: %08x\n",
		  getbasepri(), getprimask(), getipsr(), getcontrol());
	lldbg("  CFAULTS: %08x HFAULTS: %08x DFAULTS: %08x BFAULTADDR: %08x AFAULTS: %08x\n",
		  getreg32(NVIC_CFAULTS), getreg32(NVIC_HFAULTS),
		  getreg32(NVIC_DFAULTS), getreg32(NVIC_BFAULT_ADDR),
		  getreg32(NVIC_AFAULTS));
	lldbg("  R0: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		  regs[REG_R0], regs[REG_R1], regs[REG_R2], regs[REG_R3],
		  regs[REG_R4], regs[REG_R5], regs[REG_R6], regs[REG_R7]);
	lldbg("  R8: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		  regs[REG_R8], regs[REG_R9], regs[REG_R10], regs[REG_R11],
		  regs[REG_R12], regs[REG_R13], regs[REG_R14], regs[REG_R15]);

#ifdef CONFIG_ARMV8M_USEBASEPRI
#ifdef REG_EXC_RETURN
	lldbg("  xPSR: %08x BASEPRI: %08x EXC_RETURN: %08x (saved)\n",
		  current_regs[REG_XPSR], current_regs[REG_BASEPRI],
		  current_regs[REG_EXC_RETURN]);
#else
	lldbg("  xPSR: %08x BASEPRI: %08x (saved)\n",
		  current_regs[REG_XPSR], current_regs[REG_BASEPRI]);
#endif
#else
#ifdef REG_EXC_RETURN
	lldbg("  xPSR: %08x PRIMASK: %08x EXC_RETURN: %08x (saved)\n",
		  current_regs[REG_XPSR], current_regs[REG_PRIMASK], current_regs[REG_EXC_RETURN]);
#else
	lldbg("  xPSR: %08x PRIMASK: %08x (saved)\n",
		  current_regs[REG_XPSR], current_regs[REG_PRIMASK]);
#endif
#endif

}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_hardfault
 *
 * Description:
 *   This is Hard Fault exception handler.  It also catches SVC call
 *   exceptions that are performed in bad contexts.
 *
 ****************************************************************************/

int up_hardfault(int irq, FAR void *context, FAR void *arg)
{
	uint32_t *regs = (uint32_t *)context;
	system_exception_location = regs[REG_R15];
	/* Get the value of the program counter where the fault occurred */

#ifndef CONFIG_ARMV8M_USEBASEPRI
	uint16_t *pc = (uint16_t *)regs[REG_PC] - 1;

	/* Check if the pc lies in known FLASH memory.
	 * REVISIT:  What if the PC lies in "unknown" external memory?  Best
	 * use the BASEPRI register if you have external memory.
	 */

#ifdef CONFIG_BUILD_PROTECTED
	/* In the kernel build, SVCalls are expected in either the base, kernel
	 * FLASH region, kernel RAM region or in the user FLASH region.
	 */

	if ((is_kernel_text_space((void *)pc)) || (sched_self()->uspace &&
			 (uintptr_t)pc >= (uintptr_t)sched_self()->uspace->->us_textstart &&
			 (uintptr_t)pc < (uintptr_t)sched_self()->uspace->us_textend))
#else
	/* SVCalls are expected only from the base, kernel FLASH & RAM region */

	if (is_kernel_text_space((void *)pc))
#endif
	{
		/* Fetch the instruction that caused the Hard fault */

		uint16_t insn = *pc;
		sllvdbg("  PC: %p INSN: %04x\n", pc, insn);

		/* If this was the instruction 'svc 0', then forward processing
		 * to the SVCall handler
		 */

		if (insn == INSN_SVC0) {
			sllvdbg("Forward SVCall\n");
			return up_svcall(irq, context, arg);
		}
	}
#endif

	if (CHECK_SECURE_PERMISSION()) {
		print_hardfault_detail(irq, regs);
	}

	(void)irqsave();

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif
	PANIC();
	return OK;
}
