/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-m/up_usagefault.c
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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DIVBYZERO	0x02000000
#define UNALIGNED	0x01000000
#define NOCP		0x00080000
#define INVPC		0x00040000
#define INVSTATE	0x00020000
#define UNDEFINSTR	0x00010000

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
 * Name: up_usagefault
 *
 * Description:
 *   This is Usage Fault exception handler.
 *
 ****************************************************************************/

int up_usagefault(int irq, FAR void *context, FAR void *arg)
{
	(void)irqsave();
	uint32_t *regs = (uint32_t *)context;
	uint32_t cfsr = getreg32(NVIC_CFAULTS);
	lldbg("PANIC!!! Usagefault occurred while executing instruction at address : 0x%08x\n", regs[REG_R15]);
	lldbg("CFAULTS: 0x%08x\n", cfsr);

	if (cfsr & DIVBYZERO) {
		lldbg("Divide by zero error has occurred.\n");
	} else if (cfsr & UNALIGNED) {
		lldbg("Unaligned access error has occurred.\n");
	} else if (cfsr & NOCP) {
		lldbg("A coprocessor error has occurred.\n");
	} else if (cfsr & INVPC) {
		lldbg("An integrity check error has occurred on EXC_RETURN. PC value might be invalid.\n");
	} else if (cfsr & INVSTATE) {
		lldbg("Invalid state. Instruction executed with invalid EPSR.T or EPSR.IT field.\n");
	} else if (cfsr & UNDEFINSTR) {
		lldbg("Undefined instruction executed.\n");
	}

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif

	PANIC();
	return 0;
}
