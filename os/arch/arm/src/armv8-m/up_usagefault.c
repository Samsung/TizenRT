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
 * arch/arm/src/armv8-m/up_usagefault.c
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
#include <tinyara/security_level.h>

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

#define DIVBYZERO	0x02000000
#define UNALIGNED	0x01000000
#define STKOF		0x00100000
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
 * Name: print_usagefault_detail
 ****************************************************************************/

static inline void print_usagefault_detail(uint32_t *regs, uint32_t cfsr)
{
	lldbg("#########################################################################\n");
	lldbg("PANIC!!! Usagefault at instruction: 0x%08x\n", regs[REG_R15]);

	if (cfsr & DIVBYZERO) {
		lldbg("FAULT TYPE: DIVBYZERO (Divide by zero error has occurred).\n");
	} else if (cfsr & UNALIGNED) {
		lldbg("FAULT TYPE: UNALIGNED (Unaligned access error has occurred).\n");
	} else if (cfsr & STKOF) {
		lldbg("FAULT TYPE: STKOF (Stack overflow error has occurred).\n");
	} else if (cfsr & NOCP) {
		lldbg("FAULT TYPE: NOCP (A coprocessor error has occurred).\n");
	} else if (cfsr & INVPC) {
		lldbg("FAULT TYPE: INVPC (Integrity check error during EXC_RETURN).\n");
	} else if (cfsr & INVSTATE) {
		lldbg("FAULT TYPE: INVSTATE (Invalid state. Check EPSR T bit. XPSR = 0x%08x).\n", regs[REG_XPSR]);
	} else if (cfsr & UNDEFINSTR) {
		lldbg("FAULT TYPE: UNDEFINSTR (Undefined instruction executed).\n");
	}

	lldbg("FAULT REGS: CFAULTS: 0x%08x\n", cfsr);
	lldbg("#########################################################################\n");
}

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
	system_exception_location = regs[REG_R15];
	if (cfsr & INVPC) {
		/* As PC value might be invalid use LR value to determine if
		 * the crash occurred in the kernel space or in the user space */
		system_exception_location = regs[REG_R14];
	}

	if (CHECK_SECURE_PERMISSION()) {
		print_usagefault_detail(regs, cfsr);
	}

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	 up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif

	PANIC();
	return 0;
}
