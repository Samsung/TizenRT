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

#include "sched/sched.h"
#ifdef CONFIG_BOARD_ASSERT_AUTORESET
#include <sys/boardctl.h>
#endif
#ifdef CONFIG_BINMGR_RECOVERY
#include <fcntl.h>
#include <mqueue.h>
#include "binary_manager/binary_manager.h"
#endif
#include "irq/irq.h"

#include "up_arch.h"
#include "up_internal.h"
#include "mpu.h"

#ifdef CONFIG_BINMGR_RECOVERY
bool abort_mode = false;
uint32_t assert_pc;
extern uint32_t g_assertpc;
extern mqd_t g_binmgr_mq_fd;
#endif
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

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_getsp
 ****************************************************************************/

/* I don't know if the builtin to get SP is enabled */

static inline uint32_t up_getsp(void)
{
	uint32_t sp;
	__asm__
	(
		"\tmov %0, sp\n\t"
		: "=r"(sp)
	);
	return sp;
}

/****************************************************************************
 * Name: up_stackdump
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static void up_stackdump(uint32_t sp, uint32_t stack_base)
{
	uint32_t stack;

	for (stack = sp & ~0x1f; stack < stack_base; stack += 32) {
		uint32_t *ptr = (uint32_t *)stack;
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
 * Name: up_taskdump
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
static void up_taskdump(FAR struct tcb_s *tcb, FAR void *arg)
{
	/* Dump interesting properties of this task */

#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("%10s | %5d | %4d | %7lu / %7lu\n",
			tcb->name, tcb->pid, tcb->sched_priority,
			(unsigned long)up_check_tcbstack(tcb), (unsigned long)tcb->adj_stack_size);
#else
	lldbg("%5d | %4d | %7lu / %7lu\n",
			tcb->pid, tcb->sched_priority, (unsigned long)up_check_tcbstack(tcb),
			(unsigned long)tcb->adj_stack_size);
#endif

	if (tcb->pid != 0 && up_check_tcbstack(tcb) == tcb->adj_stack_size) {
		lldbg("  !!! PID (%d) STACK OVERFLOW !!! \n", tcb->pid);
	}
}
#endif

/****************************************************************************
 * Name: up_showtasks
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
static inline void up_showtasks(void)
{
	lldbg("*******************************************\n");
	lldbg("List of all tasks in the system:\n");
	lldbg("*******************************************\n");

#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("   NAME   |  PID  |  PRI |    USED /  TOTAL STACK\n");
	lldbg("-------------------------------------------------\n");
#else
	lldbg("  PID | PRI |   USED / TOTAL STACK\n");
	lldbg("----------------------------------\n");
#endif

	/* Dump interesting properties of each task in the crash environment */

	sched_foreach(up_taskdump, NULL);
}
#else
#define up_showtasks()
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

	if (rtcb->pid == 0) {
		ustackbase = g_idle_topstack - 4;
		ustacksize = CONFIG_IDLETHREAD_STACKSIZE;
	} else {
		ustackbase = (uint32_t)rtcb->adj_stack_ptr;
		ustacksize = (uint32_t)rtcb->adj_stack_size;
	}

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
	} else {
		up_stackdump(sp, ustackbase);
	}

#endif

	/* Then dump the registers (if available) */

	up_registerdump();

	/* Dump the state of all tasks (if available) */

	up_showtasks();

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
#ifdef CONFIG_BINMGR_RECOVERY
	int ret;
	binmgr_request_t request_msg;
	struct tcb_s *tcb;
	uint32_t ksram_segment_end = (uint32_t)__ksram_segment_start__ + (uint32_t)__ksram_segment_size__;
	uint32_t kflash_segment_end = (uint32_t)__kflash_segment_start__ + (uint32_t)__kflash_segment_size__;

	/* Check if the PC lies within kernel context */
	if ((assert_pc >= (uint32_t)__ksram_segment_start__ && assert_pc <= ksram_segment_end) || (assert_pc >= (uint32_t)__kflash_segment_start__ && assert_pc < kflash_segment_end)) {

		/* Reboot the system if fault is in kernel context */

#ifdef CONFIG_BOARD_ASSERT_AUTORESET
		boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
#else
		(void)irqsave();
		for (;;) {
#ifdef CONFIG_ARCH_LEDS
			//board_autoled_on(LED_PANIC);
			up_mdelay(250);
			//board_autoled_off(LED_PANIC);
			up_mdelay(250);
#endif
		}
#endif
	} else {

		request_msg.cmd = BINMGR_FAULT;
		request_msg.requester_pid = getpid();
		if (current_regs) {
			tcb = sched_self();
			sched_removereadytorun(tcb);
#if CONFIG_RR_INTERVAL > 0
			tcb->timeslice = 0;
#endif
			tcb->sched_priority = SCHED_PRIORITY_MIN;
			sched_addreadytorun(tcb);
		}

		/* Send a message to fault manager with pid of the faulty task */
		ret = mq_send(g_binmgr_mq_fd, (const char *)&request_msg, sizeof(binmgr_request_t), BINMGR_FAULT_PRIO);
		DEBUGASSERT(ret == 0);
		if (current_regs) {
			tcb = sched_self();
			current_regs = tcb->xcp.regs;
		}
	}
#else

	/* Are we in an interrupt handler or the idle task? */
	if (current_regs || (this_task())->pid == 0) {
		(void)irqsave();
		for (;;) {
#ifdef CONFIG_ARCH_LEDS
			//board_led_on(LED_PANIC);
			up_mdelay(250);
			//board_led_off(LED_PANIC);
			up_mdelay(250);
#endif
		}
	} else {
		exit(errorcode);
	}
#endif
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
	board_led_on(LED_ASSERTION);

#if defined(CONFIG_DEBUG_DISPLAY_SYMBOL) || defined(CONFIG_BINMGR_RECOVERY)
	abort_mode = true;
#endif

#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Assertion failed at file:%s line: %d task: %s\n", filename, lineno, this_task()->name);
#else
	lldbg("Assertion failed at file:%s line: %d\n", filename, lineno);
#endif

#ifdef CONFIG_BINMGR_RECOVERY
	/* Extract the PC value of instruction which caused the abort/assert */

	if (current_regs) {
		assert_pc = current_regs[REG_R14];
	} else {
		assert_pc = (uint32_t)g_assertpc;
	}
#endif

#ifndef CONFIG_BINMGR_RECOVERY
	up_dumpstate();
#endif

#if defined(CONFIG_BOARD_ASSERT_AUTORESET) && !defined(CONFIG_BINMGR_RECOVERY)
	(void)boardctl(BOARDIOC_RESET, 0);
#endif
	_up_assert(EXIT_FAILURE);
}
