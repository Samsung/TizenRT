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
 * arch/arm/src/armv7-m/up_memfault.c
 *
 *   Copyright (C) 2011, 2013 Gregory Nutt. All rights reserved.
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

#undef CONFIG_DEBUG
#undef CONFIG_DEBUG_ERROR
#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_ERROR 1

#include <assert.h>
#include <debug.h>

#include <arch/irq.h>

#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <tinyara/reboot_reason.h>
#include <arch/reboot_reason.h>
#endif

#include "up_arch.h"
#include "nvic.h"
#include "up_internal.h"

extern uint32_t system_exception_location;
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MMARVALID	0x00000080
#define MLSPERR	0x00000020
#define MSTKERR	0x00000010
#define MUNSTKERR	0x00000008
#define DACCVIOL	0x00000002
#define IACCVIOL	0x00000001

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
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_memfault
 *
 * Description:
 *   This is Memory Management Fault exception handler.  Normally we get here
 *   when the Cortex M3 MPU is enabled and an MPU fault is detected.  However,
 *   I understand that there are other error conditions that can also generate
 *   memory management faults.
 *
 ****************************************************************************/

int up_memfault(int irq, FAR void *context, FAR void *arg)
{
	/* Dump some memory management fault info */

	(void)irqsave();
	uint32_t *regs = (uint32_t *)context;
	uint32_t cfsr = getreg32(NVIC_CFAULTS);
	uint32_t mmfar = getreg32(NVIC_MEMMANAGE_ADDR);
	system_exception_location = regs[REG_R15];

	lldbg("PANIC!!! Memory Management Fault occurred while executing instruction at address : 0x%08x\n", regs[REG_R15]);
	lldbg("CFAULTS: 0x%08x\n", cfsr);

	if (cfsr & MMARVALID) {
		lldbg("Access violation occurred at address (MMFAR) : 0x%08x\n", mmfar);
	} else {
		lldbg("Unable to determine access violation address.\n");
	}

	if (cfsr & DACCVIOL) {
		lldbg("Data access violation occurred.\n");
	} else if (cfsr & IACCVIOL) {
		lldbg("Instruction access violation occurred while fetching instruction from an Execute Never (XN) region.\n");
		system_exception_location = regs[REG_R14];	/* The PC value might be invalid, so use LR */
	} else if (cfsr & MSTKERR) {
		lldbg("Error while stacking registers during exception entry.\n");
	} else if (cfsr & MUNSTKERR) {
		lldbg("Error while unstacking registers during exception return.\n");
	} else if (cfsr & MLSPERR) {
		lldbg("Error occurred during lazy state preservation of Floating Point unit registers.\n");
	}

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	if (cfsr & IACCVIOL) {
		up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
	} else {
		up_reboot_reason_write(REBOOT_SYSTEM_DATAABORT);
	}
#endif

	PANIC();
	return OK;					/* Won't get here */
}
