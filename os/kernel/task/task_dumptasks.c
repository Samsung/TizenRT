/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#undef  CONFIG_DEBUG
#undef  CONFIG_DEBUG_ERROR
#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_ERROR 1

#include <assert.h>
#include <stdint.h>
#include <debug.h>
#include <sys/types.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/sched.h>
#include <tinyara/security_level.h>

#include "task/task.h"

/* NOTE : This file is for assert usage only. */

#ifdef CONFIG_STACK_COLORATION
#if CONFIG_TASK_NAME_SIZE > 0
#define TASKDUMP_ARGS_FORMAT "%*s | %5s | %4s | %7s / %7s | %16s | %8s | %10s \n"
#define TASKDUMP_FORMAT "%*s | %5d | %4d | %7lu / %7lu | %16p | %8p | %10u \n"
#define TASKDUMP_VALUE  CONFIG_TASK_NAME_SIZE, tcb->name, tcb->pid, tcb->sched_priority, (unsigned long)used_stack_size, \
	(unsigned long)tcb->adj_stack_size, tcb->stack_alloc_ptr, tcb, tcb->task_state
#define TASKDUMP_ARGS  CONFIG_TASK_NAME_SIZE, "NAME", "PID", "PRI", "USED", "TOTAL STACK",  "STACK ALLOC ADDR", "TCB ADDR", "TASK STATE"
#else
#define TASKDUMP_ARGS_FORMAT "%5s | %4s | %7s / %7s | %16s | %8s | %10s \n"
#define TASKDUMP_FORMAT "%5d | %4d | %7lu / %7lu | %16p | %8p | %10u \n"
#define TASKDUMP_VALUE  tcb->pid, tcb->sched_priority, tcb, (unsigned long)used_stack_size, (unsigned long)tcb->adj_stack_size\
	, tcb->stack_alloc_ptr, tcb, tcb->task_state
#define TASKDUMP_ARGS  "PID", "PRI", "USED", "TOTAL STACK", "STACK ALLOC ADDR", "TCB ADDR", "TASK STATE"
#endif
#else
#if CONFIG_TASK_NAME_SIZE > 0
#define TASKDUMP_ARGS_FORMAT "%*s | %5s | %4s | %7s | %16s | %8s | %10s \n"
#define TASKDUMP_FORMAT "%*s | %5d | %4d | %7lu | %16p | %8p | %10u \n"
#define TASKDUMP_VALUE  CONFIG_TASK_NAME_SIZE, tcb->name, tcb->pid, tcb->sched_priority, (unsigned long)tcb->adj_stack_size\
	, tcb->stack_alloc_ptr, tcb, tcb->task_state
#define TASKDUMP_ARGS  CONFIG_TASK_NAME_SIZE, "NAME", "PID", "PRI", "TOTAL STACK",  "STACK ALLOC ADDR", "TCB ADDR", "TASK STATE"
#else
#define TASKDUMP_ARGS_FORMAT "%5s | %4s | %7s | %16s | %8s | %10s \n"
#define TASKDUMP_FORMAT "%5d | %4d | %7lu | %16p | %8p | %10u \n"
#define TASKDUMP_VALUE  tcb->pid, tcb->sched_priority, tcb, (unsigned long)tcb->adj_stack_size, tcb->stack_alloc_ptr, tcb, tcb->task_state
#define TASKDUMP_ARGS  "PID", "PRI", "TOTAL STACK", "STACK ALLOC ADDR", "TCB ADDR", "TASK STATE"
#endif
#endif

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(CONFIG_ARCH_STACKDUMP) && (defined(CONFIG_ARCH_ARMV7A_FAMILY) || defined(CONFIG_ARCH_ARMV8M_FAMILY))
/****************************************************************************
 * Name: task_dump_registers
 *
 * Description:
 *   Dump a stopped task's registers without reading its stack or changing context.
 *   The caller protects the TCB lifetime and stops other CPUs before calling.
 *
 ****************************************************************************/

void task_dump_registers(FAR struct tcb_s *tcb)
{
	const volatile void *regs;

	DEBUGASSERT(tcb != NULL);
	if (IS_SECURE_STATE()) {
		return;
	}

	regs = up_get_task_registers(tcb);
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg_noarg("\nTask pid=%d name=%s\n", (int)tcb->pid, tcb->name);
#else
	lldbg_noarg("\nTask pid=%d\n", (int)tcb->pid);
#endif
	if (regs) {
		up_registerdump(regs);
	} else {
		lldbg_noarg("register context unavailable\n");
	}
}

/****************************************************************************
 * Name: task_dump
 *
 * Description:
 *   Dump a stopped task's registers and usable stack without changing context.
 *   The caller protects the TCB lifetime and stops other CPUs before calling.
 *
 ****************************************************************************/

void task_dump(FAR struct tcb_s *tcb)
{
	const volatile void *regs;
	uintptr_t stack_end;
	uintptr_t stack_start;
	size_t stack_size;
	uintptr_t sp;

	DEBUGASSERT(tcb != NULL);
	if (IS_SECURE_STATE()) {
		return;
	}

	regs = up_get_task_registers(tcb);
	task_dump_registers(tcb);
	if (regs == NULL) {
		lldbg_noarg("Stack pointer unavailable; stack dump skipped\n");
		return;
	}
	sp = up_getusrsp(regs);

	stack_end = up_get_task_stack_end(tcb);
	stack_size = tcb->adj_stack_size;
	if (!tcb->stack_alloc_ptr || stack_size == 0 || stack_end - stack_size < (uintptr_t)tcb->stack_alloc_ptr) {
		lldbg("Task stack bounds unavailable\n");
		return;
	}
	stack_start = stack_end - stack_size;
	if ((sp % sizeof(uintptr_t)) != 0 || sp < stack_start || sp >= stack_end) {
		lldbg_noarg("SP outside usable stack; stack dump skipped\n");
		return;
	}
	up_stackdump(sp, stack_end);
}
#endif

/****************************************************************************
 * Name: task_show_tcbinfo
 * 
 * NOTE : This function is for assert usage only.
 ****************************************************************************/

void task_show_tcbinfo(struct tcb_s *tcb)
{
#ifdef CONFIG_LIB_SYSCALL
	int nsyscall = tcb->xcp.nsyscalls;
#endif
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("Task TCB info\n");
	lldbg_noarg("===========================================================\n");
	lldbg("PID         : %d\n", (int)tcb->pid);
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Name        : %s\n", tcb->name);
#endif
#ifdef CONFIG_SMP
	lldbg("CPU         : %d\n", (int)tcb->cpu);
#else
	lldbg("CPU         : 0\n");
#endif
	lldbg("Priority    : %d\n", (int)tcb->sched_priority);
	lldbg("TCB address : %p\n", tcb);
	lldbg("State       : %u\n", tcb->task_state);
	lldbg("Flags       : %u\n", tcb->flags);
	lldbg("Lock count  : %u\n", tcb->lockcount);
#if CONFIG_RR_INTERVAL > 0
	lldbg("Timeslice   : %d\n", tcb->timeslice);
#endif
	lldbg("Waitdog     : %p\n", tcb->waitdog);
	lldbg("WaitSem     : %p\n", tcb->waitsem);
#ifndef CONFIG_DISABLE_MQUEUE
	lldbg("MsgwaitQ    : %p\n", tcb->msgwaitq);
#endif
#ifndef CONFIG_DISABLE_SIGNALS
	lldbg("Sigdeliver  : %p\n", tcb->xcp.sigdeliver);
#endif
#ifdef CONFIG_LIB_SYSCALL
	lldbg("Nsyscalls   : %u\n", nsyscall);
	for (int i = 0; i < nsyscall; i++) {
		lldbg("Syscall %d   : %p\n", i, tcb->xcp.syscall[i].sysreturn);
	}
#endif
}

/****************************************************************************
 * Name: task_taskdump
 * 
 * NOTE : This function is for assert usage only.
 ****************************************************************************/

static void task_taskdump(FAR struct tcb_s *tcb, FAR void *arg)
{
#ifdef CONFIG_STACK_COLORATION
	size_t used_stack_size = up_check_tcbstack(tcb);
#endif

	/* Dump interesting properties of this task */

	lldbg_noarg(TASKDUMP_FORMAT, TASKDUMP_VALUE);

#ifdef CONFIG_STACK_COLORATION
	if (used_stack_size >= tcb->adj_stack_size) {
		lldbg_noarg("  !!! PID (%d) STACK OVERFLOW !!! \n", tcb->pid);
	}
#endif
}

static void task_stackdump(FAR struct tcb_s *tcb, FAR void *arg)
{
	(void)arg;
	task_dump(tcb);
}

/****************************************************************************
 * Name: task_show_alivetask_list
 * 
 * NOTE : This function is for assert usage only.
 ****************************************************************************/

void task_show_alivetask_list(void)
{
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("List of all tasks in the system\n");
	lldbg_noarg("===========================================================\n");

	lldbg_noarg(TASKDUMP_ARGS_FORMAT, TASKDUMP_ARGS);
	lldbg_noarg("-------------------------------------------------------------------------------------------------------------------\n");

	/* Dump interesting properties of each task in the crash environment */

	sched_foreach(task_taskdump, NULL);
	lldbg_noarg("-------------------------------------------------------------------------------------------------------------------\n");

	lldbg_noarg("===========================================================\n");
	lldbg_noarg("All tasks: register and stack dump\n");
	lldbg_noarg("===========================================================\n");
	sched_foreach(task_stackdump, NULL);
	lldbg_noarg("===========================================================\n");

}
