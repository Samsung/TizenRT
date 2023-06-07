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
 * arch/arm/src/armv8-m/up_assert.c
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

#include <tinyara/mm/mm.h>

#ifdef CONFIG_APP_BINARY_SEPARATION
#include <tinyara/binfmt/elf.h>
#endif
#include <tinyara/security_level.h>
#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <arch/reboot_reason.h>
#endif
#include "sched/sched.h"
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
#include <sys/boardctl.h>
#endif
#ifdef CONFIG_BINMGR_RECOVERY
#include <stdbool.h>
#include <unistd.h>
#include <queue.h>
#include <tinyara/wdog.h>
#include "semaphore/semaphore.h"
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
#include "nvic.h"

bool abort_mode = false;

#ifdef CONFIG_BINMGR_RECOVERY
extern struct tcb_s *g_faultmsg_sender;
extern sq_queue_t g_faultmsg_list;
extern sq_queue_t g_freemsg_list;
#endif

extern uint32_t system_exception_location;
extern uint32_t user_assert_location;
extern int g_irq_nums[3];
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
 * Public Variables
 ****************************************************************************/
char assert_info_str[CONFIG_STDIO_BUFFER_SIZE] = {'\0', };

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

#ifdef CONFIG_ARMV8M_USEBASEPRI
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
	uint32_t stackbase = 0;
	uint32_t stacksize = 0;
#if CONFIG_ARCH_INTERRUPTSTACK > 3
	uint32_t istackbase = 0;
	uint32_t istacksize = 0;
#endif
	uint32_t nestirqstkbase = 0;
	uint32_t nestirqstksize = 0;
	uint8_t irq_num;

	/* Get the limits for each type of stack */

	stackbase = (uint32_t)rtcb->adj_stack_ptr;
	stacksize = (uint32_t)rtcb->adj_stack_size;
#if CONFIG_ARCH_INTERRUPTSTACK > 3
	istackbase = (uint32_t)&g_intstackbase;
	istacksize = (CONFIG_ARCH_INTERRUPTSTACK & ~3);
#endif
#ifdef CONFIG_ARCH_NESTED_IRQ_STACK_SIZE
	nestirqstkbase = (uint32_t)&g_nestedirqstkbase;
	nestirqstksize = (CONFIG_ARCH_NESTED_IRQ_STACK_SIZE & ~3);
#endif
	bool is_irq_assert = false;
	bool is_sp_corrupt = false;

	/* Check if the assert location is in user thread or IRQ handler.
	 * If the irq_num is lesser than NVIC_IRQ_USAGEFAULT, then it is
	 * a fault and not an irq.
	 */
	if (g_irq_nums[2] && (g_irq_nums[0] <= NVIC_IRQ_USAGEFAULT)) {
		/* Assert in nested irq */
		irq_num = 1;
		is_irq_assert = true;
		lldbg("Code asserted in nested IRQ state!\n");
	} else if (g_irq_nums[1] && (g_irq_nums[0] > NVIC_IRQ_USAGEFAULT)) {
		/* Assert in nested irq */
		irq_num = 0;
		is_irq_assert = true;
		lldbg("Code asserted in nested IRQ state!\n");
	} else if (g_irq_nums[1] && (g_irq_nums[0] <= NVIC_IRQ_USAGEFAULT)) {
		/* Assert in irq */
		irq_num = 1;
		is_irq_assert = true;
		lldbg("Code asserted in IRQ state!\n");
	} else if (g_irq_nums[0] > NVIC_IRQ_USAGEFAULT) {
		/* Assert in irq */
		irq_num = 0;
		is_irq_assert = true;
		lldbg("Code asserted in IRQ state!\n");
	} else {
		/* Assert in user thread */
		lldbg("Code asserted in normal thread!\n");
		if (current_regs) {
			/* If assert is in user thread, but current_regs is not NULL,
			 * it means that assert happened due to a fault. So, we want to
			 * reset the sp to the value just before the fault happened
			 */
			sp = current_regs[REG_R13];
		}
	}

	/* Print IRQ handler details if required */

	if (is_irq_assert) {
		lldbg("IRQ num: %d\n", g_irq_nums[irq_num]);
		lldbg("IRQ handler: %08x \n", g_irqvector[g_irq_nums[irq_num]].handler);
#ifdef CONFIG_DEBUG_IRQ_INFO
		lldbg("IRQ name: %s \n", g_irqvector[g_irq_nums[irq_num]].irq_name);
#endif
		if ((sp <= nestirqstkbase) && (sp > (nestirqstkbase - nestirqstksize))) {
			stackbase = nestirqstkbase;
			stacksize = nestirqstksize;
			lldbg("Current SP is Nested IRQ SP: %08x\n", sp);
			lldbg("Nested IRQ stack:\n");
		} else
#if CONFIG_ARCH_INTERRUPTSTACK > 3
		if ((sp <= istackbase) && (sp > (istackbase - istacksize))) {
			stackbase = istackbase;
			stacksize = istacksize;
			lldbg("Current SP is IRQ SP: %08x\n", sp);
			lldbg("IRQ stack:\n");
		} else {
			is_sp_corrupt = true;
		}
#else
		if ((sp <= stackbase) && (sp > (stackbase - stacksize))) {
			lldbg("Current SP is User Thread SP: %08x\n", sp);
			lldbg("User stack:\n");
		} else {
			is_sp_corrupt = true;
		}
#endif
	} else if ((sp <= stackbase) && (sp > (stackbase - stacksize))) {
		lldbg("Current SP is User Thread SP: %08x\n", sp);
		lldbg("User stack:\n");
	} else {
		is_sp_corrupt = true;
	}


	if (is_sp_corrupt) {
		lldbg("ERROR: Stack pointer is not within any of the allocated stack\n");
		lldbg("Wrong Stack pointer %08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		sp, *((uint32_t *)sp + 0), *((uint32_t *)sp + 1), *((uint32_t *)sp + 2), ((uint32_t *)sp + 3),
		*((uint32_t *)sp + 4), ((uint32_t *)sp + 5), ((uint32_t *)sp + 6), ((uint32_t *)sp + 7));

		/* Since SP is corrupted, we dont know which stack was being used.
		 * So, dump all the available stacks.
		 */
#ifdef CONFIG_ARCH_NESTED_IRQ_STACK_SIZE
		lldbg("Nested IRQ stack dump:\n");
		up_stackdump(nestirqstkbase - nestirqstksize + 1, nestirqstkbase);
#endif
#if CONFIG_ARCH_INTERRUPTSTACK > 3
		lldbg("IRQ stack dump:\n");
		up_stackdump(istackbase - istacksize + 1, istackbase);
#endif
		lldbg("User thread stack dump:\n");
		up_stackdump(stackbase - stacksize + 1, stackbase);
	} else {
		/* Dump the stack region which contains the current stack pointer */
		lldbg("  base: %08x\n", stackbase);
		lldbg("  size: %08x\n", stacksize);
#ifdef CONFIG_STACK_COLORATION
		lldbg("  used: %08x\n", up_check_assertstack((uintptr_t)(stackbase - stacksize), stacksize));
#endif
		up_stackdump(sp, stackbase);
	}

	/* Then dump the registers (if available) */

	up_registerdump();

	/* Dump the state of all tasks (if available) */

	task_show_alivetask_list();

	/* Dump MPU regions info */

#ifdef CONFIG_ARMV8M_MPU
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
 * Name: print_assert_detail
 ****************************************************************************/

static inline void print_assert_detail(const uint8_t *filename, int lineno, struct tcb_s *fault_tcb, uint32_t asserted_location)
{
	irqstate_t flags = irqsave();
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Assertion failed at file:%s line: %d task: %s\n", filename, lineno, fault_tcb->name);
#else
	lldbg("Assertion failed at file:%s line: %d\n", filename, lineno);
#endif

	/* Print the extra arguments (if any) from ASSERT_INFO macro */
	if (assert_info_str[0]) {
		lldbg("%s\n", assert_info_str);
	}

#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
	if (IS_HPWORK || IS_LPWORK) {
		lldbg("Code asserted in workqueue!\n");
		lldbg("Running work function is %x.\n", work_get_current());
	}
#endif
#endif /* defined(CONFIG_DEBUG_WORKQUEUE) */

	up_dumpstate();

#ifdef CONFIG_APP_BINARY_SEPARATION
	elf_show_all_bin_section_addr();
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
	if (IS_FAULT_IN_USER_THREAD(fault_tcb)) {
		lldbg("Checking current app heap for corruption...\n");
		if (mm_check_heap_corruption((struct mm_heap_s *)(fault_tcb->uheap)) == OK) {
			lldbg("No app heap corruption detected\n");
		}
	}
#endif
	lldbg("Assert location (PC) : 0x%08x\n", asserted_location);

	/* Dump the asserted TCB */
	lldbg("*******************************************\n");
	lldbg("Asserted TCB Info\n");
	lldbg("*******************************************\n");

	task_show_tcbinfo(fault_tcb);

#if defined(CONFIG_BOARD_CRASHDUMP)
	board_crashdump(up_getsp(), fault_tcb, (uint8_t *)filename, lineno);
#endif
	irqrestore(flags);
}

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
 * Name: recovery_assert
 ****************************************************************************/

static inline void recovery_assert(uint32_t asserted_location)
{
	irqstate_t flags = irqsave();
	assertdbg("Checking kernel heap for corruption...\n");
	if (mm_check_heap_corruption(g_kmmheap) == OK) {
		assertdbg("No kernel heap corruption detected\n");
	} else {
		/* treat kernel fault */

		_up_assert(EXIT_FAILURE);
	}
	irqrestore(flags);

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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dump_stack : dumps the stack of current thread
 ****************************************************************************/
void dump_stack(void)
{
	/* ToDo: implement as per the armv8-m architecture */
	return;
}

/****************************************************************************
 * Name: dump_all_stack : dumps the stack of current thread
 ****************************************************************************/
void dump_all_stack(void)
{
	/* ToDo: implement as per the armv8-m architecture */
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

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	reboot_reason_write_user_intended();
#endif

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

	lldbg("==============================================\n");
	lldbg("Assertion failed\n");
	lldbg("==============================================\n");
#ifdef CONFIG_SECURITY_LEVEL
	lldbg("security level: %d\n", get_security_level());
#endif
	/* Print assert detail information and dump state,
	 * but if os security level is high, It is not printed.
	 */
	if (CHECK_SECURE_PERMISSION()) {
		print_assert_detail(filename, lineno, fault_tcb, asserted_location);
	}

	recovery_assert(asserted_location);
}


