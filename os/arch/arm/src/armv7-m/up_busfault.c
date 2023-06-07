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
 * arch/arm/src/armv7-m/up_busfault.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#undef CONFIG_DEBUG
#undef CONFIG_DEBUG_ERROR
#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_ERROR 1		/* Enable them to print crash logs always regardless of CONFIG_DEBUG and CONFIG_DEBUG_ERROR */

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

#define BFARVALID	0x00008000
#define LSPERR	0x00002000
#define STKERR	0x00001000
#define UNSTKERR	0x00000800
#define IMPRECISERR	0x00000400
#define PRECISERR	0x00000200
#define IBUSERR	0x00000100

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
 * Name: print_busfault_detail
 ****************************************************************************/

static inline void print_busfault_detail(uint32_t *regs, uint32_t cfsr, uint32_t bfar)
{

	lldbg("#########################################################################\n");
	lldbg("PANIC!!! Bus fault at instruction: 0x%08x\n", regs[REG_R15]);

	if (cfsr & PRECISERR) {
		lldbg("FAULT TYPE: PRECISERR (Precise data access error occurred).\n");
	} else if (cfsr & IMPRECISERR) {
		lldbg("FAULT TYPE: IMPRECISERR (Imprecise data access error occurred).\n");
	} else if (cfsr & STKERR) {
		lldbg("FAULT TYPE: STKERR (Error while stacking registers during exception entry).\n");
	} else if (cfsr & UNSTKERR) {
		lldbg("FAULT TYPE: UNSTKERR (Error while unstacking registers during exception return).\n");
	} else if (cfsr & LSPERR) {
		lldbg("FAULT TYPE: LSPERR (Error occurred during lazy state preservation of Floating Point unit registers).\n");
	} else if (cfsr & IBUSERR) {
		lldbg("FAULT TYPE: IBUSERR (Error on an instruction prefetch).\n");
	}

	if (cfsr & BFARVALID) {
		lldbg("FAULT ADDRESS: 0x%08x\n", bfar);
	} else {
		lldbg("FAULT ADDRESS: Unable to determine fault address.\n");
	}

	lldbg("FAULT REGS: CFAULTS: 0x%08x BFAR: 0x%08x\n", cfsr, bfar);
	lldbg("#########################################################################\n");

}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_busfault
 *
 * Description:
 *   This is Bus Fault exception handler.
 *
 ****************************************************************************/

int up_busfault(int irq, FAR void *context, FAR void *arg)
{
	(void)irqsave();
	uint32_t *regs = (uint32_t *)context;
	uint32_t cfsr = getreg32(NVIC_CFAULTS);
	uint32_t bfar = getreg32(NVIC_BFAULT_ADDR);
	system_exception_location = regs[REG_R15];

	if (CHECK_SECURE_PERMISSION()) {
		print_busfault_detail(regs, cfsr, bfar);
	}

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	if (cfsr & IBUSERR) {
		up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
	} else {
		up_reboot_reason_write(REBOOT_SYSTEM_DATAABORT);
	}
#endif

	PANIC();
	return 0;
}
