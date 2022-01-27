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
 * arch/arm/src/armv7-m/up_assert.c
 *
 *   Copyright (C) 2009-2010, 2012-2014 Gregory Nutt. All rights reserved.
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

/* Output debug info if stack dump is selected -- even if debug is not
 * selected.
 */

#ifdef CONFIG_ARCH_STACKDUMP
#undef  CONFIG_DEBUG
#undef  CONFIG_DEBUG_ERROR
#undef  CONFIG_DEBUG_WARN
#undef  CONFIG_DEBUG_VERBOSE
#undef  CONFIG_LOGM
#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_ERROR 1
#define CONFIG_DEBUG_WARN 1
#define CONFIG_DEBUG_VERBOSE 1
#endif

#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/syslog/syslog.h>

#include <arch/board/board.h>
#include <tinyara/sched.h>

#ifdef CONFIG_APP_BINARY_SEPARATION
#include <tinyara/binfmt/elf.h>
#endif

#include "sched/sched.h"
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
#include <sys/boardctl.h>
#endif

#include <tinyara/mm/mm.h>

#ifdef CONFIG_BINMGR_RECOVERY
#include <stdbool.h>
#include "binary_manager/binary_manager.h"
#endif

#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
#include <tinyara/wqueue.h>
#endif
#endif

#include "irq/irq.h"
#include "task/task.h"

#include "up_arch.h"
#include "up_internal.h"
#include "mpu.h"

bool abort_mode = false;
extern uint32_t system_exception_location;
extern uint32_t user_assert_location;

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* USB trace dumping */

#ifndef CONFIG_USBDEV_TRACE
#undef CONFIG_ARCH_USBDUMP
#endif

#ifndef CONFIG_BOARD_RESET_ON_ASSERT
#define CONFIG_BOARD_RESET_ON_ASSERT 0
#endif

#define IS_FAULT_IN_USER_THREAD(fault_tcb)  ((void *)fault_tcb->uheap != NULL)
#define IS_FAULT_IN_USER_SPACE(asserted_location)   (is_kernel_space((void *)asserted_location) == false)

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_stackdump
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static void up_stackdump(uint32_t sp, uint32_t stack_base)
{
	uint32_t stack = sp & ~0x1f;
	uint32_t *ptr = (uint32_t *)stack;
	uint8_t i;

	lldbg("%08x:", stack);
	for (i = 0; i < 8; i++) {  // Print first 8 values for sp located
		if (stack < sp) {
			/* If stack pointer(sp) is aligned(sp & 0x1f) to an address outside allocated stack */
			/* Then, for stack addresses outside allocated stack, print 'xxxxxxxx' */
			lldbg_noarg(" xxxxxxxx");
		} else {
			/* For remaining stack addresses inside allocated stack, print proper stack address values */
			lldbg_noarg(" %08x", ptr[i]);
		}
		stack += 4;
	}
	lldbg_noarg("\n");

	for (; stack < stack_base; stack += 32) { // Print remaining stack values from 9th value to end
		ptr = (uint32_t *)stack;
		lldbg("%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   stack, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	}
}
#else
#define up_stackdump(sp, stack_base)
#endif

/****************************************************************************
 * Name: up_registerdump
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static inline void up_registerdump(void)
{
	/* Are user registers available from interrupt processing? */

	if (current_regs) {
		/* Yes.. dump the interrupt registers */

		lldbg("R0: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			  current_regs[REG_R0], current_regs[REG_R1],
			  current_regs[REG_R2], current_regs[REG_R3],
			  current_regs[REG_R4], current_regs[REG_R5],
			  current_regs[REG_R6], current_regs[REG_R7]);
		lldbg("R8: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			  current_regs[REG_R8], current_regs[REG_R9],
			  current_regs[REG_R10], current_regs[REG_R11],
			  current_regs[REG_R12], current_regs[REG_R13],
			  current_regs[REG_R14], current_regs[REG_R15]);

#ifdef CONFIG_ARMV7M_USEBASEPRI
		lldbg("xPSR: %08x BASEPRI: %08x CONTROL: %08x\n", current_regs[REG_XPSR], current_regs[REG_BASEPRI], getcontrol());
#else
		lldbg("xPSR: %08x PRIMASK: %08x CONTROL: %08x\n", current_regs[REG_XPSR], current_regs[REG_PRIMASK], getcontrol());
#endif

#ifdef REG_EXC_RETURN
		lldbg("EXC_RETURN: %08x\n", current_regs[REG_EXC_RETURN]);
#endif
	}
}
#else
#define up_registerdump()
#endif

/****************************************************************************
 * Name: assert_tracecallback
 ****************************************************************************/

#ifdef CONFIG_ARCH_USBDUMP
static int usbtrace_syslog(FAR const char *fmt, ...)
{
	va_list ap;
	int ret;

	/* Let vsyslog do the real work */

	va_start(ap, fmt);
	ret = lowvsyslog(LOG_INFO, fmt, ap);
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
 * Name: up_dumpstate
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static void up_dumpstate(void)
{
	struct tcb_s *rtcb = this_task();
	uint32_t sp = up_getsp();
	uint32_t ustackbase;
	uint32_t ustacksize;
#if CONFIG_ARCH_INTERRUPTSTACK > 3
	uint32_t istackbase;
	uint32_t istacksize;
#endif

	/* Get the limits on the user stack memory */

	ustackbase = (uint32_t)rtcb->adj_stack_ptr;
	ustacksize = (uint32_t)rtcb->adj_stack_size;

#if CONFIG_ARCH_INTERRUPTSTACK > 3
	/* Get the limits on the interrupt stack memory */

	istackbase = (uint32_t)&g_intstackbase;
	istacksize = (CONFIG_ARCH_INTERRUPTSTACK & ~3);

	/* Show interrupt stack info */

	lldbg("sp:     %08x\n", sp);
	lldbg("IRQ stack:\n");
	lldbg("  base: %08x\n", istackbase);
	lldbg("  size: %08x\n", istacksize);
#ifdef CONFIG_STACK_COLORATION
	lldbg("  used: %08x\n", up_check_intstack());
#endif

	/* Does the current stack pointer lie within the interrupt
	 * stack?
	 */

	if (sp <= istackbase && sp > istackbase - istacksize) {
		/* Yes.. dump the interrupt stack */

		up_stackdump(sp, istackbase);
	}

	/* Extract the user stack pointer if we are in an interrupt handler.
	 * If we are not in an interrupt handler.  Then sp is the user stack
	 * pointer (and the above range check should have failed).
	 */

	if (current_regs) {
		sp = current_regs[REG_R13];
		lldbg("sp:     %08x\n", sp);
	}

	lldbg("User stack:\n");
	lldbg("  base: %08x\n", ustackbase);
	lldbg("  size: %08x\n", ustacksize);
#ifdef CONFIG_STACK_COLORATION
	lldbg("  used: %08x\n", up_check_tcbstack(rtcb));
#endif

	/* Dump the user stack if the stack pointer lies within the allocated user
	 * stack memory.
	 */

	if (sp <= ustackbase && sp > ustackbase - ustacksize) {
		up_stackdump(sp, ustackbase);
	}
#else

	/* Show user stack info */

	lldbg("sp:         %08x\n", sp);
	lldbg("stack base: %08x\n", ustackbase);
	lldbg("stack size: %08x\n", ustacksize);
#ifdef CONFIG_STACK_COLORATION
	lldbg("stack used: %08x\n", up_check_tcbstack(rtcb));
#endif

	/* Dump the user stack if the stack pointer lies within the allocated user
	 * stack memory.
	 */

	if (sp > ustackbase || sp <= ustackbase - ustacksize) {
		lldbg("ERROR: Stack pointer is not within the allocated stack\n");
		lldbg("Proper task stack dump:\n");
		up_stackdump(ustackbase - ustacksize + 1, ustackbase);
		lldbg("Wrong Stack pointer %08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		sp, *((uint32_t *)sp + 0), *((uint32_t *)sp + 1), *((uint32_t *)sp + 2), ((uint32_t *)sp + 3),
		*((uint32_t *)sp + 4), ((uint32_t *)sp + 5), ((uint32_t *)sp + 6), ((uint32_t *)sp + 7));
	} else {
		up_stackdump(sp, ustackbase);
	}

#endif

	/* Then dump the registers (if available) */

	up_registerdump();

	/* Dump the state of all tasks (if available) */

	task_show_alivetask_list();

	/* Dump MPU regions info */

#ifdef CONFIG_ARMV7M_MPU
	mpu_show_regioninfo();
#endif

#ifdef CONFIG_ARCH_USBDUMP
	/* Dump USB trace data */

	(void)usbtrace_enumerate(assert_tracecallback, NULL);
#endif
}
#else
#define up_dumpstate()
#endif

/****************************************************************************
 * Name: _up_assert
 ****************************************************************************/
static void _up_assert(int errorcode)
{
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
#else
#ifndef CONFIG_BOARD_ASSERT_SYSTEM_HALT
	/* Are we in an interrupt handler or the idle task? */

	if (current_regs || (this_task())->pid == 0) {
#endif
		(void)irqsave();
		for (;;) {
#ifdef CONFIG_ARCH_LEDS
			//board_led_on(LED_PANIC);
			up_mdelay(250);
			//board_led_off(LED_PANIC);
			up_mdelay(250);
#endif
		}
#ifndef CONFIG_BOARD_ASSERT_SYSTEM_HALT
	} else {
		exit(errorcode);
	}
#endif
#endif /* CONFIG_BOARD_ASSERT_AUTORESET */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dump_stack : dumps the stack of current thread
 ****************************************************************************/
void dump_stack(void)
{
	/* ToDo: implement as per the armv7-m architecture */
	return;
}

/****************************************************************************
 * Name: dump_all_stack : dumps the stack of current thread
 ****************************************************************************/
void dump_all_stack(void)
{
	/* ToDo: implement as per the armv7-m architecture */
	return;
}

/****************************************************************************
 * Name: up_assert
 ****************************************************************************/

void up_assert(const uint8_t *filename, int lineno)
{
	/* ARCH_GET_RET_ADDRESS should always be
	 * called at the start of the function */

	size_t kernel_assert_location = 0;
	ARCH_GET_RET_ADDRESS(kernel_assert_location)
	struct tcb_s *fault_tcb = this_task();

	board_led_on(LED_ASSERTION);

	abort_mode = true;

	uint32_t asserted_location;

	/* Extract the PC value of instruction which caused the abort/assert */

	if (system_exception_location) {
		asserted_location = (uint32_t)system_exception_location;
		system_exception_location = 0x0;	/* reset */
	} else if (user_assert_location) {
		asserted_location = (uint32_t)user_assert_location;
		user_assert_location = 0x0;
	} else {
		asserted_location = (uint32_t)kernel_assert_location;
	}

#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Assertion failed at file:%s line: %d task: %s\n", filename, lineno, fault_tcb->name);
#else
	lldbg("Assertion failed at file:%s line: %d\n", filename, lineno);
#endif

#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
	if (IS_HPWORK || IS_LPWORK) {
		lldbg("Running work function is %x.\n", work_get_current());
	}
#endif
#endif /* defined(CONFIG_DEBUG_WORKQUEUE) */

	up_dumpstate();

	lldbg("Checking kernel heap for corruption...\n");
	if (mm_check_heap_corruption(g_kmmheap) == OK) {
		lldbg("No kernel heap corruption detected\n");
	} else {
		/* treat kernel fault */

		_up_assert(EXIT_FAILURE);
	}
#ifdef CONFIG_APP_BINARY_SEPARATION
	if (IS_FAULT_IN_USER_THREAD(fault_tcb)) {
		lldbg("Checking current app heap for corruption...\n");
		if (mm_check_heap_corruption((struct mm_heap_s *)(fault_tcb->uheap)) == OK) {
			lldbg("No app heap corruption detected\n");
		}
		elf_show_all_bin_addr();
	}
#endif
	lldbg("Assert location (PC) : %08x\n", asserted_location);

	/* Dump the asserted TCB */
	lldbg("*******************************************\n");
	lldbg("Asserted TCB Info\n");
	lldbg("*******************************************\n");

	task_show_tcbinfo(fault_tcb);

#if defined(CONFIG_BOARD_CRASHDUMP)
	board_crashdump(up_getsp(), fault_tcb, (uint8_t *)filename, lineno);
#endif

#ifdef CONFIG_BINMGR_RECOVERY
	if (IS_FAULT_IN_USER_SPACE(asserted_location)) {
		/* Recover user fault through binary manager */
		binary_manager_recover_userfault();
	} else
#endif
	{
		/* treat kernel fault */

		_up_assert(EXIT_FAILURE);
	}
}
