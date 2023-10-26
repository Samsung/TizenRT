/******************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/****************************************************************************
 *
 *   Copyright (C) 2016, 2018 Gregory Nutt. All rights reserved.
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

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/syslog/syslog.h>
#include <tinyara/usb/usbdev_trace.h>
#include <tinyara/security_level.h>

#include <arch/board/board.h>

#include "sched/sched.h"
#include "xtensa.h"

bool abort_mode = false;

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* USB trace dumping */

#ifndef CONFIG_USBDEV_TRACE
#undef CONFIG_ARCH_USBDUMP
#endif

/****************************************************************************
 * Public Variables
 ****************************************************************************/
char assert_info_str[CONFIG_STDIO_BUFFER_SIZE] = {'\0', };

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: assert_tracecallback
 ****************************************************************************/

#ifdef CONFIG_ARCH_USBDUMP
static int usbtrace_syslog(FAR const char *fmt, ...)
{
	va_list ap;
	int ret;

	/* Let nx_vsyslog do the real work */

	va_start(ap, fmt);
	ret = nx_vsyslog(LOG_EMERG, fmt, &ap);
	va_end(ap);
	return ret;
}

static int assert_tracecallback(FAR struct usbtrace_s *trace, FAR void *arg)
{
	usbtrace_trprintf(usbtrace_syslog, trace->event, trace->value);
	return 0;
}
#endif

/****************************************************************************
 * Name: xtensa_assert
 ****************************************************************************/

static void xtensa_assert(int errorcode) noreturn_function;
static void xtensa_assert(int errorcode)
{
	if (!IS_SECURE_STATE()) {
#ifdef CONFIG_DEBUG_ERROR
		/* Dump the processor state */

		xtensa_dumpstate();
#endif

#ifdef CONFIG_ARCH_USBDUMP
		/* Dump USB trace data */

		(void)usbtrace_enumerate(assert_tracecallback, NULL);
#endif

#ifdef CONFIG_BOARD_CRASHDUMP
		/* Perform board-specific crash dump */

		board_crashdump(up_getsp(), this_task(), filename, lineno);
#endif
	}

	/* Are we in an interrupt handler or the idle task? */

	if (CURRENT_REGS || this_task()->pid == 0) {
			/* Blink the LEDs forever */

		(void)up_irq_save();
		for (;;) {
#ifdef CONFIG_ARCH_LEDS
			board_autoled_on(LED_PANIC);
			up_mdelay(250);
			board_autoled_off(LED_PANIC);
			up_mdelay(250);
#endif
		}
	} else {
		/* Assertions in other contexts only cause the thread to exit */

		exit(errorcode);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_assert
 ****************************************************************************/

void up_assert(const uint8_t *filename, int lineno)
{
	board_autoled_on(LED_ASSERTION);

	abort_mode = true;

	lldbg("==============================================\n");
	lldbg("Assertion failed\n");
	lldbg("==============================================\n");

	if (!IS_SECURE_STATE()) {
#if defined(CONFIG_DEBUG)
#if CONFIG_TASK_NAME_SIZE > 0 && defined(CONFIG_DEBUG_ERROR)
		struct tcb_s *rtcb = this_task();
		lldbg("Assertion failed at file:%s line: %d task: %s\n", filename, lineno, rtcb->name);
#else
		lldbg("Assertion failed at file:%s line: %d\n", filename, lineno);
#endif
#endif

		/* Print the extra arguments (if any) from ASSERT_INFO macro */
		if (assert_info_str[0]) {
			lldbg("%s\n", assert_info_str);
		}
	}

	xtensa_assert(EXIT_FAILURE);
}

/****************************************************************************
 * Name: xtensa_panic
 *
 * Description:
 *   PANIC if an unhandled exception is received:
 *
 *   - NMI exception
 *   - Debug exception
 *   - Double exception
 *   - Kernel exception
 *   - Co-processor exception
 *   - High priority level2-6 Exception.
 *
 * Input Parameters:
 *   xcptcode - Identifies the unhandled exception (see include/esp32/irq.h)
 *   regs - The register save are at the time of the interrupt.
 *
 * Returned Value:
 *   Does not return
 *
 ****************************************************************************/

void xtensa_panic(int xptcode, uint32_t *regs)
{
	/* We get here when a un-dispatch-able, irrecoverable exception occurs */

	board_autoled_on(LED_ASSERTION);

	if (!IS_SECURE_STATE()) {
#if defined(CONFIG_DEBUG)
#if CONFIG_TASK_NAME_SIZE > 0 && defined(CONFIG_DEBUG_ERROR)
		struct tcb_s *rtcb = this_task();
		lldbg("Unhandled Exception %d task: %s\n", xptcode, rtcb->name);
#else
		lldbg("Unhandled Exception %d\n", xptcode);
#endif
#endif
	}

	CURRENT_REGS = regs;
	xtensa_assert(EXIT_FAILURE);	/* Should not return */
	for (;;) ;
}

/****************************************************************************
 * Name: xtensa_user
 *
 * Description:
 *   PANIC if certain User Exceptions are received received.  All values for
 *   EXCCAUSE are listed below (not all generate PANICs):
 *
 *   0  IllegalInstructionCause
 *      Illegal instruction
 *   1  SyscallCause
 *      SYSCALL instruction
 *   2  InstructionFetchErrorCause
 *      Processor internal physical address or data error during instruction
 *      fetch.
 *   3  LoadStoreErrorCause
 *      Processor internal physical address or data error during load or
 *      store.
 *   4  Level1InterruptCause
 *      Level-1 interrupt as indicated by set level-1 bits in the INTERRUPT
 *      register.
 *   5  AllocaCause
 *      MOVSP instruction, if caller’s registers are not in the register file.
 *   6  IntegerDivideByZeroCause
 *      QUOS, QUOU, REMS, or REMU divisor operand is zero.
 *   7  PCValueErrorCause Next PC Value Illegal
 *   8  PrivilegedCause
 *      Attempt to execute a privileged operation when CRING != 0
 *   9  LoadStoreAlignmentCause
 *      Load or store to an unaligned address.
 *   10..11 Reserved for Cadence
 *   12 InstrPIFDataErrorCause
 *      PIF data error during instruction fetch.
 *   13 LoadStorePIFDataErrorCause
 *      Synchronous PIF data error during LoadStore access.
 *   14 InstrPIFAddrErrorCause
 *      PIF address error during instruction fetch.
 *   15 LoadStorePIFAddrErrorCause
 *      Synchronous PIF address error during LoadStore access.
 *   16 InstTLBMissCause
 *      Error during Instruction TLB refill
 *   17 InstTLBMultiHitCause
 *      Multiple instruction TLB entries matched
 *   18 InstFetchPrivilegeCause
 *      An instruction fetch referenced a virtual address at a ring leve
 *      less than CRING.
 *   19 Reserved for Cadence
 *   20 InstFetchProhibitedCause
 *       An instruction fetch referenced a page mapped with an attribute
 *      that does not permit instruction fetch.
 *   21..23 Reserved for Cadence
 *   24 LoadStoreTLBMissCause
 *      Error during TLB refill for a load or store.
 *   25 LoadStoreTLBMultiHitCause
 *      Multiple TLB entries matched for a load or store.
 *   26 LoadStorePrivilegeCause
 *      A load or store referenced a virtual address at a ring level less
 *      than CRING.
 *   27 Reserved for Cadence
 *   28 LoadProhibitedCause
 *      A load referenced a page mapped with an attribute that does not
 *      permit loads.
 *   29 StoreProhibitedCause
 *     A store referenced a page mapped with an attribute that does not
 *     permit stores.
 *   30..31 Reserved for Cadence
 *   32..39 CoprocessornDisabled
 *     Coprocessor n instruction when cpn disabled. n varies 0..7 as the
 *     cause varies 32..39.
 *   40..63 Reserved
 *
 * Input Parameters:
 *   exccause - Identifies the EXCCAUSE of the user exception
 *   regs - The register save are at the time of the interrupt.
 *
 * Returned Value:
 *   Does not return
 *
 ****************************************************************************/

void xtensa_user(int exccause, uint32_t *regs)
{
	/* We get here when a un-dispatch-able, irrecoverable exception occurs */

	board_autoled_on(LED_ASSERTION);

	if (!IS_SECURE_STATE()) {
#if defined(CONFIG_DEBUG)
#if CONFIG_TASK_NAME_SIZE > 0 && defined(CONFIG_DEBUG_ERROR)
		struct tcb_s *rtcb = this_task();
		lldbg("User Exception: EXCCAUSE=%04x task: %s\n", exccause, rtcb->name);
#else
		lldbg("User Exception: EXCCAUSE=%04x\n", exccause);
#endif
#endif
	}

	CURRENT_REGS = regs;
	xtensa_assert(EXIT_FAILURE);	/* Should not return */
	for (;;) ;
}
