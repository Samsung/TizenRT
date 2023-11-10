/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-a/arm_assert.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership. The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
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
#undef CONFIG_DEBUG
#undef CONFIG_DEBUG_ERROR
#undef CONFIG_DEBUG_WARN
#undef CONFIG_DEBUG_VERBOSE
#undef CONFIG_LOGM
#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_ERROR 1
#define CONFIG_DEBUG_WARN 1
#define CONFIG_DEBUG_VERBOSE 1
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/syslog/syslog.h>
#include <tinyara/usb/usbdev_trace.h>

#include <arch/board/board.h>
#include <tinyara/sched.h>

#include <tinyara/mm/mm.h>

#ifdef CONFIG_ARCH_USE_MMU
#include <tinyara/mmu.h>
#endif

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
#include "up_internal.h"

bool abort_mode = false;

#ifdef CONFIG_BINMGR_RECOVERY
extern struct tcb_s *g_faultmsg_sender;
extern sq_queue_t g_faultmsg_list;
extern sq_queue_t g_freemsg_list;
#endif

extern uint32_t system_exception_location;
extern uint32_t user_assert_location;
extern int g_irq_num;
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

#define IS_FAULT_IN_USER_THREAD(fault_tcb)		((void *)fault_tcb->uheap != NULL)
#define IS_FAULT_IN_USER_SPACE(asserted_location)	(is_kernel_space((void *)asserted_location) == false)


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

	lldbg_noarg("%08x:", stack);
	for (i = 0; i < 8; i++) {	// Print first 8 values for sp located
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
		lldbg_noarg("%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			 stack, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	}
}

/****************************************************************************
 * Name: arm_registerdump
 ****************************************************************************/

static void arm_registerdump(volatile uint32_t *regs)
{
	/* Dump the interrupt registers */
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("Register dump\n");
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("R0: %08x R1: %08x R2: %08x R3: %08x\n",
	regs[REG_R0], regs[REG_R1], regs[REG_R2], regs[REG_R3]);
#ifdef CONFIG_ARM_THUMB
	lldbg_noarg("R4: %08x R5: %08x R6: %08x FP: %08x\n",
	regs[REG_R4], regs[REG_R5], regs[REG_R6], regs[REG_R7]);
	lldbg_noarg("R8: %08x SB: %08x SL: %08x R11: %08x\n",
	regs[REG_R8], regs[REG_R9], regs[REG_R10], regs[REG_R11]);
#else
	lldbg_noarg("R4: %08x R5: %08x R6: %08x R7: %08x\n",
	regs[REG_R4], regs[REG_R5], regs[REG_R6], regs[REG_R7]);
	lldbg_noarg("R8: %08x SB: %08x SL: %08x	FP: %08x\n",
	regs[REG_R8], regs[REG_R9], regs[REG_R10], regs[REG_R11]);
#endif
	lldbg_noarg("IP: %08x SP: %08x LR: %08x PC: %08x\n",
	regs[REG_R12], regs[REG_R13], regs[REG_R14], regs[REG_R15]);

#if defined(REG_BASEPRI)
	lldbg_noarg("xPSR: %08x BASEPRI: %08x CONTROL: %08x\n",
	regs[REG_XPSR], regs[REG_BASEPRI], getcontrol());
#elif defined(REG_PRIMASK)
	lldbg_noarg("xPSR: %08x PRIMASK: %08x CONTROL: %08x\n",
	regs[REG_XPSR], regs[REG_PRIMASK], getcontrol());
#elif defined(REG_CPSR)
	lldbg_noarg("CPSR: %08x\n", regs[REG_CPSR]);
#endif

#ifdef REG_EXC_RETURN
	lldbg_noarg("EXC_RETURN: %08x\n", regs[REG_EXC_RETURN]);
#endif
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("\n");
}

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

static void up_dumpstate(struct tcb_s *fault_tcb, uint32_t asserted_location)
{
	uint32_t sp = up_getsp();
	uint32_t stackbase = 0;
	uint32_t stacksize = 0;
#if CONFIG_ARCH_INTERRUPTSTACK > 3
	uint32_t istackbase = 0;
	uint32_t istacksize = 0;
#endif

	/* Update the xcp context */
#ifdef CONFIG_APP_BINARY_SEPARATION
	if (!IS_FAULT_IN_USER_THREAD(fault_tcb)) {
#endif
		if (CURRENT_REGS)
		{
			fault_tcb->xcp.regs = (uint32_t *)CURRENT_REGS;
		}
		else
		{
			up_saveusercontext(fault_tcb->xcp.regs);
		}
#ifdef CONFIG_APP_BINARY_SEPARATION
	}
#endif
	/* Get the limits for each type of stack */
	stackbase = (uint32_t)fault_tcb->adj_stack_ptr;
	stacksize = (uint32_t)fault_tcb->adj_stack_size;
#if CONFIG_ARCH_INTERRUPTSTACK > 7
#ifdef CONFIG_SMP
	istackbase = (uint32_t)arm_intstack_alloc(),
#else
	istackbase = (uint32_t)&g_intstackbase,
#endif
	istacksize = (CONFIG_ARCH_INTERRUPTSTACK & ~7);
#endif
	bool is_irq_assert = false;
	bool is_sp_corrupt = false;

	/* Check if the assert location is in user thread or IRQ handler.
	 * If the irq_num is lesser than NVIC_IRQ_USAGEFAULT, then it is
	 * a fault and not an irq.
	 */
	if (g_irq_num >= 0) {
		/* Assert in irq */
		is_irq_assert = true;
		lldbg_noarg("Code asserted in IRQ state!\n");
		lldbg_noarg("IRQ num: %d\n", g_irq_num);
		lldbg_noarg("IRQ handler: %08x \n", g_irqvector[g_irq_num].handler);
#ifdef CONFIG_DEBUG_IRQ_INFO
		lldbg_noarg("IRQ name: %s \n", g_irqvector[g_irq_num].irq_name);
#endif
	} else {
		/* Assert in user thread */
		lldbg_noarg("Code asserted in normal thread!\n");
		if (CURRENT_REGS) {
			/* If assert is in user thread, but current_regs is not NULL,
			 * it means that assert happened due to a fault. So, we want to
			 * reset the sp to the value just before the fault happened
			 */
			sp = CURRENT_REGS[REG_R13];
		}
	}
	lldbg_noarg("Assert location (PC) : 0x%08x\n", asserted_location);
	lldbg_noarg("\n");

	/* Dump the registers */
	if (CURRENT_REGS) {
		arm_registerdump(CURRENT_REGS);
	}

	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("Stack Dump\n");
	lldbg_noarg("=====================================================================\n");
	/* Print stack dump */
	if (is_irq_assert) {
		/* Assert in irq */
#if CONFIG_ARCH_INTERRUPTSTACK > 7
		if ((sp <= istackbase) && (sp > (istackbase - istacksize))) {
			stackbase = istackbase;
			stacksize = istacksize;
			lldbg_noarg("Current SP is IRQ SP: %08x\n", sp);
			lldbg_noarg("IRQ stack:\n");
		} else {
			is_sp_corrupt = true;
		}
#else
		if ((sp <= stackbase) && (sp > (stackbase - stacksize))) {
			lldbg_noarg("Current SP is User Thread SP: %08x\n", sp);
			lldbg_noarg("User stack:\n");
		} else {
			is_sp_corrupt = true;
		}
#endif
	} else if ((sp <= stackbase) && (sp > (stackbase - stacksize))) {
		lldbg_noarg("Current SP is User Thread SP: %08x\n", sp);
		lldbg_noarg("User stack:\n");
	} else {
		is_sp_corrupt = true;
	}

	if (is_sp_corrupt) {
		lldbg_noarg("ERROR: Stack pointer is not within the allocated stack\n");
		lldbg_noarg("ERROR: SP = 0x%08x\n", sp);
		if (is_irq_assert) {
#if CONFIG_ARCH_INTERRUPTSTACK > 7
			lldbg_noarg("ERROR: IRQ stack = 0x%08x - 0x%08x\n", istackbase - istacksize + 1, istackbase);
#else
			lldbg_noarg("ERROR: USR stack = 0x%08x - 0x%08x\n", stackbase - stacksize + 1, stackbase);
#endif
		} else {
			lldbg_noarg("ERROR: USR stack = 0x%08x - 0x%08x\n", stackbase - stacksize + 1, stackbase);
		}
		
		lldbg_noarg("Wrong Stack pointer %08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		sp, *((uint32_t *)sp + 0), *((uint32_t *)sp + 1), *((uint32_t *)sp + 2), ((uint32_t *)sp + 3),
		*((uint32_t *)sp + 4), ((uint32_t *)sp + 5), ((uint32_t *)sp + 6), ((uint32_t *)sp + 7));

		if (is_irq_assert) {
#if CONFIG_ARCH_INTERRUPTSTACK > 7
			lldbg_noarg("IRQ stack dump:\n");
			up_stackdump(istackbase - istacksize + 1, istackbase);
#else
			lldbg_noarg("User thread stack dump:\n");
			up_stackdump(stackbase - stacksize + 1, stackbase);
#endif
		} else {
			lldbg_noarg("User thread stack dump:\n");
			up_stackdump(stackbase - stacksize + 1, stackbase);
		}
	} else {
		/* Dump the stack region which contains the current stack pointer */
		lldbg_noarg("  base: %08x\n", stackbase);
		lldbg_noarg("  size: %08x\n", stacksize);
#ifdef CONFIG_STACK_COLORATION
		lldbg_noarg("  used: %08x\n", up_check_assertstack((uintptr_t)(stackbase - stacksize), stackbase));
#endif
		up_stackdump(sp, stackbase);
	}
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("\n");

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_ARCH_MMUDUMP)
	if (IS_FAULT_IN_USER_THREAD(fault_tcb)) {
		mmu_dump_app_pgtbl();
		lldbg_noarg("\n");
	}
#endif

#ifdef CONFIG_ARCH_USBDUMP
	/* Dump USB trace data */
	(void)usbtrace_enumerate(assert_tracecallback, NULL);
#endif

	lldbg_noarg("\n");
}
#endif /* CONFIG_ARCH_STACKDUMP */

/****************************************************************************
 * Name: arm_assert
 ****************************************************************************/

static void arm_assert(void)
{
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
#else
#ifndef CONFIG_BOARD_ASSERT_SYSTEM_HALT
	/* Are we in an interrupt handler or the idle task? */

	if (CURRENT_REGS || (this_task())->flink == NULL) {
#endif
		/* Disable interrupts on this CPU */
		irqsave();

#ifdef CONFIG_SMP
		/* Try (again) to stop activity on other CPUs */
		spin_trylock(&g_cpu_irqlock);
#endif

		for (; ; )
		{
#ifdef CONFIG_ARCH_LEDS
		/* FLASH LEDs a 2Hz */
		board_autoled_on(LED_PANIC);
		up_mdelay(250);
		board_autoled_off(LED_PANIC);
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
 * Name: print_assert_detail
 ****************************************************************************/

static inline void print_assert_detail(const uint8_t *filename, int lineno, struct tcb_s *fault_tcb, uint32_t asserted_location)
{
  board_autoled_on(LED_ASSERTION);

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	reboot_reason_write_user_intended();
#endif

#if CONFIG_TASK_NAME_SIZE > 0
	lldbg_noarg("Assertion failed "
#ifdef CONFIG_SMP
	"CPU%d "
#endif
	"at file:%s line: %d task: %s pid: %d\n",
#ifdef CONFIG_SMP
	up_cpu_index(),
#endif
	filename, lineno, fault_tcb->name, fault_tcb->pid);
#else
	lldbg_noarg("Assertion failed "
#ifdef CONFIG_SMP
	"CPU%d "
#endif
	"at file:%s line: %d pid: %d\n", 
#ifdef CONFIG_SMP
	up_cpu_index(),
#endif
	filename, lineno, fault_tcb->pid);
#endif

	/* Print the extra arguments (if any) from ASSERT_INFO macro */
	if (assert_info_str[0]) {
		lldbg_noarg("%s\n", assert_info_str);
	}

#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
	if (IS_HPWORK || IS_LPWORK) {
		lldbg_noarg("Code asserted in workqueue!\n");
		lldbg_noarg("Running work function is %x.\n", work_get_current());
	}
#endif
#endif /* defined(CONFIG_DEBUG_WORKQUEUE) */

	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("Checking kernel heap for corruption...\n");
	lldbg_noarg("=====================================================================\n");
	if (mm_check_heap_corruption(g_kmmheap) == OK) {
		lldbg_noarg("No kernel heap corruption detected\n");
	} else {
		lldbg_noarg("##########################################################################################################################################\n");
		lldbg_noarg("\n\n");
		/* treat kernel fault */
		arm_assert();
	}
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("\n");
	
	/* Dump the state of all tasks (if available) */
	task_show_alivetask_list();
	lldbg_noarg("\n");

	/* Dump the asserted TCB */
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("Asserted TCB Info\n");
	lldbg_noarg("=====================================================================\n");
	task_show_tcbinfo(fault_tcb);
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("\n");

#ifdef CONFIG_APP_BINARY_SEPARATION
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("Application Info\n");
	lldbg_noarg("=====================================================================\n");
	elf_show_all_bin_section_addr();
	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("\n");
#endif

	up_dumpstate(fault_tcb, asserted_location);

#ifdef CONFIG_APP_BINARY_SEPARATION
	if (IS_FAULT_IN_USER_THREAD(fault_tcb)) {
		lldbg_noarg("=====================================================================\n");
		lldbg_noarg("Checking current app heap for corruption...\n");
		lldbg_noarg("=====================================================================\n");
		if (mm_check_heap_corruption((struct mm_heap_s *)(fault_tcb->uheap)) == OK) {
			lldbg_noarg("No app heap corruption detected\n");
		}
		lldbg_noarg("=====================================================================\n");
	}
#endif

#if defined(CONFIG_BOARD_CRASHDUMP)
	lldbg_noarg("Perform Crashdump\n");
	board_crashdump(up_getsp(), fault_tcb, (uint8_t *)filename, lineno);
	lldbg_noarg("\n");
#endif

}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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


	board_autoled_on(LED_ASSERTION);
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

	irqstate_t flags = irqsave();
	lldbg_noarg("\n\n");
	lldbg_noarg("##########################################################################################################################################\n");
	lldbg_noarg("Assertion failed\n");
	lldbg_noarg("##########################################################################################################################################\n");
#ifdef CONFIG_SECURITY_LEVEL
	lldbg_noarg("security level: %d\n", get_security_level());
#endif
	/* Print assert detail information and dump state,
	 * but if os security level is high, It is not printed.
	 */
	if (!IS_SECURE_STATE()) {
		print_assert_detail(filename, lineno, fault_tcb, asserted_location);
	}

	lldbg_noarg("##########################################################################################################################################\n");
	lldbg_noarg("\n\n");
	irqrestore(flags);

#ifdef CONFIG_BINMGR_RECOVERY
	if (IS_FAULT_IN_USER_SPACE(asserted_location)) {
		/* Recover user fault through binary manager */
		binary_manager_recover_userfault();
	} else
#endif
	{
		/* treat kernel fault */
		arm_assert();
	}
}
