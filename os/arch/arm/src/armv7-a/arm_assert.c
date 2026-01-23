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
#include "binary_manager/binary_manager_internal.h"
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
#include "binary_manager/binary_manager_internal.h"
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
extern int g_irq_num[CONFIG_SMP_NCPUS];
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

#ifdef CONFIG_APP_BINARY_SEPARATION
#define IS_FAULT_IN_USER_THREAD(fault_tcb)		((void *)fault_tcb->uheap != NULL)
#define IS_FAULT_IN_USER_SPACE(asserted_location)	(is_kernel_space((void *)asserted_location) == false)
#else
#define IS_FAULT_IN_USER_THREAD(fault_tcb)		(false)
#define IS_FAULT_IN_USER_SPACE(asserted_location)	(false)
#endif

#if CONFIG_TASK_NAME_SIZE > 0
#define LOG_TASK_NAME  fault_tcb->name
#else
#define LOG_TASK_NAME   "N/A"
#endif

#define NORMAL_STATE 0
#define ABORT_STATE 1

/****************************************************************************
 * Public Variables
 ****************************************************************************/
char assert_info_str[CONFIG_STDIO_BUFFER_SIZE] = { '\0', };

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Variable to check the recursive abort */
static int state = NORMAL_STATE;

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

	for (; stack < stack_base; stack += 32) {	// Print remaining stack values from 9th value to end
		ptr = (uint32_t *) stack;
		lldbg("%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n", stack, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	}
}

/****************************************************************************
 * Name: arm_registerdump
 ****************************************************************************/

static void arm_registerdump(volatile uint32_t *regs)
{
	if (regs) {
		/* Dump the interrupt registers */
		lldbg_noarg("===========================================================\n");
		lldbg_noarg("Asserted task's register dump\n");
		lldbg_noarg("===========================================================\n");
		lldbg("R0: %08x %08x %08x %08x", regs[REG_R0], regs[REG_R1], regs[REG_R2], regs[REG_R3]);
#ifdef CONFIG_ARM_THUMB
		lldbg_noarg(" %08x %08x %08x %08x\n", regs[REG_R4], regs[REG_R5], regs[REG_R6], regs[REG_R7]);
		lldbg("R8: %08x %08x %08x %08x", regs[REG_R8], regs[REG_R9], regs[REG_R10], regs[REG_R11]);
#else
		lldbg_noarg(" %08x %08x %08x %08x\n",
		regs[REG_R4], regs[REG_R5], regs[REG_R6], regs[REG_R7]);
		lldbg("R8: %08x %08x %08x %08x",
		regs[REG_R8], regs[REG_R9], regs[REG_R10], regs[REG_R11]);
#endif
		lldbg_noarg(" %08x %08x %08x %08x\n", regs[REG_R12], regs[REG_R13], regs[REG_R14], regs[REG_R15]);

#if defined(REG_BASEPRI)
		lldbg("xPSR: %08x BASEPRI: %08x CONTROL: %08x\n", regs[REG_XPSR], regs[REG_BASEPRI], getcontrol());
#elif defined(REG_PRIMASK)
		lldbg("xPSR: %08x PRIMASK: %08x CONTROL: %08x\n", regs[REG_XPSR], regs[REG_PRIMASK], getcontrol());
#elif defined(REG_CPSR)
		lldbg("CPSR: %08x\n", regs[REG_CPSR]);
#endif

#ifdef REG_EXC_RETURN
		lldbg("EXC_RETURN: %08x\n", regs[REG_EXC_RETURN]);
#endif
	}
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
 * Name: check_assert_location
 ****************************************************************************/

static void check_assert_location(uint32_t *sp, bool *is_irq_assert)
{
	int cpu = this_cpu();
	if (g_irq_num[cpu] >= 0) {
		/* Assert in irq */
		*is_irq_assert = true;
		lldbg("Code asserted in IRQ state!\n");
		lldbg("IRQ num: %d\n", g_irq_num[cpu]);
		lldbg("IRQ handler: %08x\n", g_irqvector[g_irq_num[cpu]].handler);
#ifdef CONFIG_DEBUG_IRQ_INFO
		lldbg("IRQ name: %s\n", g_irqvector[g_irq_num[cpu]].irq_name);
#endif
	} else {
		/* Assert in user thread */
		lldbg("Code asserted in normal thread!\n");
		if (CURRENT_REGS) {
			/* If assert is in user thread, but current_regs is not NULL,
			 * it means that assert happened due to a fault. So, we want to
			 * reset the sp to the value just before the fault happened
			 */
			*sp = CURRENT_REGS[REG_R13];
		}
	}
}

/****************************************************************************
 * Name: check_sp_corruption
 ****************************************************************************/

static void check_sp_corruption(uint32_t sp, uint32_t *stackbase, uint32_t *stacksize, uint32_t istackbase, uint32_t istacksize, bool is_irq_assert, bool *is_sp_corrupt)
{
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("Asserted task's stack details\n");
	lldbg_noarg("===========================================================\n");
	if (is_irq_assert) {
		/* Assert in irq */
#if CONFIG_ARCH_INTERRUPTSTACK > 7
		if ((sp <= istackbase) && (sp > (istackbase - istacksize))) {
			*stackbase = istackbase;
			*stacksize = istacksize;
			lldbg("Current SP is IRQ SP: %08x\n", sp);
			lldbg("IRQ stack:\n");
		} else {
			*is_sp_corrupt = true;
		}
#else
		if ((sp <= *stackbase) && (sp > (*stackbase - *stacksize))) {
			lldbg("Current SP is User Thread SP: %08x\n", sp);
			lldbg("User stack:\n");
		} else {
			*is_sp_corrupt = true;
		}
#endif
	} else if ((sp <= *stackbase) && (sp > (*stackbase - *stacksize))) {
		lldbg("Current SP is User Thread SP: %08x\n", sp);
		lldbg("User stack:\n");
	} else {
		*is_sp_corrupt = true;
	}
}

/****************************************************************************
 * Name: print_stack_dump
 ****************************************************************************/

static void print_stack_dump(uint32_t sp, uint32_t stackbase, uint32_t stacksize, uint32_t istackbase, uint32_t istacksize, bool is_irq_assert, bool is_sp_corrupt)
{
	if (is_sp_corrupt) {
		lldbg("ERROR: Stack pointer is not within the allocated stack\n");
		lldbg("ERROR: SP = 0x%08x\n", sp);
		if (is_irq_assert) {
#if CONFIG_ARCH_INTERRUPTSTACK > 7
			lldbg("ERROR: IRQ stack = 0x%08x - 0x%08x\n", istackbase - istacksize + 1, istackbase);
#else
			lldbg("ERROR: USR stack = 0x%08x - 0x%08x\n", stackbase - stacksize + 1, stackbase);
#endif
		} else {
			lldbg("ERROR: USR stack = 0x%08x - 0x%08x\n", stackbase - stacksize + 1, stackbase);
		}

		lldbg("Wrong Stack pointer %08x: %08x %08x %08x %08x %08x %08x %08x %08x\n", sp, *((uint32_t *) sp + 0), *((uint32_t *) sp + 1), *((uint32_t *) sp + 2), ((uint32_t *) sp + 3), *((uint32_t *) sp + 4), ((uint32_t *) sp + 5), ((uint32_t *) sp + 6), ((uint32_t *) sp + 7));
		if (is_irq_assert) {
#if CONFIG_ARCH_INTERRUPTSTACK > 7
			lldbg("IRQ stack dump:\n");
			up_stackdump(istackbase - istacksize + 1, istackbase);
#else
			lldbg("User thread stack dump:\n");
			up_stackdump(stackbase - stacksize + 1, stackbase);
#endif
		} else {
			lldbg("User thread stack dump:\n");
			up_stackdump(stackbase - stacksize + 1, stackbase);
		}
	} else {
		/* Dump the stack region which contains the current stack pointer */
		lldbg("  base: %08x\n", stackbase);
		lldbg("  size: %08x\n", stacksize);
#ifdef CONFIG_STACK_COLORATION
		lldbg("  used: %08x\n", up_check_assertstack((uintptr_t)(stackbase - stacksize), stackbase));
#endif
		up_stackdump(sp, stackbase);
	}
}

/****************************************************************************
 * Name: up_dumpstate
 ****************************************************************************/

static void up_dumpstate(struct tcb_s *fault_tcb, uint32_t asserted_location)
{
	uint32_t sp = up_getsp();
	uint32_t stackbase = 0;
	uint32_t stacksize = 0;
	uint32_t istackbase;
	uint32_t istacksize;
#if CONFIG_ARCH_INTERRUPTSTACK > 3
	istackbase = 0;
	istacksize = 0;
#else
	istackbase = 0xFFFFFFFF;
	istacksize = 0xFFFFFFFF;
#endif

	/* Update the xcp context */
#ifdef CONFIG_APP_BINARY_SEPARATION
	if (!IS_FAULT_IN_USER_THREAD(fault_tcb)) {
#endif
		if (CURRENT_REGS) {
			fault_tcb->xcp.regs = (uint32_t *)CURRENT_REGS;
		} else {
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
	/* Initialize istackbase based on the interrupt stack size and proper alignment value (~7) */
	istackbase = ((uint32_t)arm_intstack_alloc() + (CONFIG_ARCH_INTERRUPTSTACK & ~7));
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
	check_assert_location(&sp, &is_irq_assert);

	/*Print IRQ handler details if required */
	check_sp_corruption(sp, &stackbase, &stacksize, istackbase, istacksize, is_irq_assert, &is_sp_corrupt);

	/* Print stack dump */
	print_stack_dump(sp, stackbase, stacksize, istackbase, istacksize, is_irq_assert, is_sp_corrupt);

	/* Dump the asserted TCB */
	task_show_tcbinfo(fault_tcb);

	/* Dump the registers */
	arm_registerdump(CURRENT_REGS);

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_ARCH_MMUDUMP)
	if (IS_FAULT_IN_USER_THREAD(fault_tcb)) {
		mmu_dump_app_pgtbl();
	}
#endif

#ifdef CONFIG_ARCH_USBDUMP
	/* Dump USB trace data */
	(void)usbtrace_enumerate(assert_tracecallback, NULL);
#endif

}
#endif							/* CONFIG_ARCH_STACKDUMP */

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

		for (;;) {
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
#endif							/* CONFIG_BOARD_ASSERT_AUTORESET */
}

/****************************************************************************
 * Name: dump_stack : dumps the stack of current thread
 ****************************************************************************/
void dump_stack(void)
{
	/* ToDo: implement as per the armv7-a architecture */
}

/****************************************************************************
 * Name: dump_all_stack : dumps the stack of current thread
 ****************************************************************************/
void dump_all_stack(void)
{
	/* ToDo: implement as per the armv7-a architecture */
}

/****************************************************************************
 * Name: check_heap_corrupt
 ****************************************************************************/

void check_heap_corrupt(struct tcb_s *fault_tcb)
{
	if (!IS_SECURE_STATE()) {
#ifdef CONFIG_APP_BINARY_SEPARATION
		for (int index = 1; index <= CONFIG_NUM_APPS; index++) {
			lldbg_noarg("===========================================================\n");
			lldbg_noarg("Checking app %d heap for corruption \n",index);
			lldbg_noarg("===========================================================\n");
			struct mm_heap_s *app_heap = BIN_BINARY_HEAP_PTR(index);
			mm_check_heap_corruption(app_heap);
		}
#endif

		lldbg_noarg("===========================================================\n");
		lldbg_noarg("Checking kernel heap for corruption\n");
		lldbg_noarg("===========================================================\n");
	}
	if (mm_check_heap_corruption(g_kmmheap) != OK) {
		/* treat kernel fault */
		arm_assert();
	}
}

/****************************************************************************
 * Name: print_assert_detail
 ****************************************************************************/

static inline void print_assert_detail(const uint8_t *filename, int lineno, struct tcb_s *fault_tcb, uint32_t asserted_location)
{
	int cpu = up_cpu_index();
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("Assertion details\n");
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("Assertion failed CPU%d at file: %s line %d", cpu, filename, lineno);

	/* Check if the crash is in irq or task context and accordingly
	 * print either the irq number or task name, pid.
	 * If in irq context, then g_irq_num[cpu] holds the irq numner. 
	 * Else, g_irq_num[cpu] is -1. 
	 */
	if (g_irq_num[cpu] >= 0) {
		lldbg_noarg(" irq: %d\n", g_irq_num[cpu]);
	} else {
		lldbg_noarg(" task: %s pid: %d\n", LOG_TASK_NAME, fault_tcb->pid);
	}
	
	/* Print the extra arguments (if any) from ASSERT_INFO macro */
	if (assert_info_str[0]) {
		lldbg("%s\n", assert_info_str);
	}
	lldbg("Assert location (PC) : 0x%08x\n", asserted_location);

#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
	if (IS_HPWORK || IS_LPWORK) {
		lldbg_noarg("Code asserted in workqueue!\n");
		lldbg_noarg("Running work function is %x.\n", work_get_current());
	}
#endif
#endif							/* defined(CONFIG_DEBUG_WORKQUEUE) */
	up_dumpstate(fault_tcb, asserted_location);

	/* Dump the state of all tasks (if available) */
	task_show_alivetask_list();

#ifdef CONFIG_APP_BINARY_SEPARATION
	elf_show_all_bin_section_addr();
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

	irqstate_t flags = enter_critical_section();

	uint32_t asserted_location = 0;

	abort_mode = true;

	/* Check if we are in recursive abort */
	if (state == ABORT_STATE) {
		/* treat kernel fault */
		arm_assert();
	} else {
		state = ABORT_STATE;
	}
	/* Extract the PC value of instruction which caused the abort/assert */

	if (system_exception_location) {
		asserted_location = (uint32_t)system_exception_location;
		system_exception_location = 0x0;	/* reset */
	} else if (user_assert_location) {
		asserted_location = (uint32_t)user_assert_location;
		user_assert_location = 0x0;
	} else {
		asserted_location = (uint32_t)GET_RETURN_ADDRESS();
	}

#ifdef CONFIG_SMP
	/* Pause all other CPUs to avoid mix up of logs while printing assert logs */
	up_cpu_pause_all();

#endif

	struct tcb_s *fault_tcb = this_task();
	/* Add new line to distinguish between normal log and assert log.*/
	lldbg_noarg("\n");

	board_autoled_on(LED_ASSERTION);

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	reboot_reason_try_write_assert();
#endif


#ifdef CONFIG_SECURITY_LEVEL
	lldbg_noarg("security level: %d\n", get_security_level());
#endif
	/* Print assert detail information and dump state,
	 * but if os security level is high, It is not printed.
	 */
	if (!IS_SECURE_STATE()) {
		print_assert_detail(filename, lineno, fault_tcb, asserted_location);
	}

	/* Heap corruption check */
	check_heap_corrupt(fault_tcb);

	/* Closing log line */
	lldbg_noarg("##########################################################################################################################################\n");

#if defined(CONFIG_BOARD_CRASHDUMP)
	lldbg_noarg("Perform Crashdump\n");
	board_crashdump(up_getsp(), fault_tcb, (uint8_t *)filename, lineno);
	lldbg_noarg("\n");
#endif

#ifdef CONFIG_BINMGR_RECOVERY
	if (IS_FAULT_IN_USER_SPACE(asserted_location)) {
		/* Recover user fault through binary manager */
		binary_manager_recover_userfault();
	} else
#endif
	{
		if (!IS_SECURE_STATE()) {
			up_flush_console();
		}

		/* treat kernel fault */
		arm_assert();
	}
	leave_critical_section(flags);
}
