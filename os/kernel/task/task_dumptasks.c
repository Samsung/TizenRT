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

#include <debug.h>
#include <sys/types.h>
#include <tinyara/arch.h>
#include <tinyara/sched.h>

/* NOTE : This file is for assert usage only. */

#ifdef CONFIG_STACK_COLORATION
#if CONFIG_TASK_NAME_SIZE > 0
#define TASKDUMP_ARGS_FORMAT "%*s | %5s | %4s | %7s / %7s | %16s | %8s | %10s\n"
#define TASKDUMP_FORMAT "%*s | %5d | %4d | %7lu / %7lu | %16p | %8p | %10u\n"
#define TASKDUMP_VALUE  CONFIG_TASK_NAME_SIZE, tcb->name, tcb->pid, tcb->sched_priority, (unsigned long)used_stack_size, (unsigned long)tcb->adj_stack_size, tcb->stack_alloc_ptr, tcb, tcb->task_state
#define TASKDUMP_ARGS  CONFIG_TASK_NAME_SIZE, "NAME", "PID", "PRI", "USED", "TOTAL STACK",  "STACK ALLOC ADDR", "TCB ADDR", "TASK STATE"
#else
#define TASKDUMP_ARGS_FORMAT "%5s | %4s | %7s / %7s | %16s | %8s | %10s\n"
#define TASKDUMP_FORMAT "%5d | %4d | %7lu / %7lu | %16p | %8p | %10u\n"
#define TASKDUMP_VALUE  tcb->pid, tcb->sched_priority, tcb, (unsigned long)used_stack_size, (unsigned long)tcb->adj_stack_size, tcb->stack_alloc_ptr, tcb, tcb->task_state
#define TASKDUMP_ARGS  "PID", "PRI", "USED", "TOTAL STACK", "STACK ALLOC ADDR", "TCB ADDR", "TASK STATE"
#endif
#else
#if CONFIG_TASK_NAME_SIZE > 0
#define TASKDUMP_ARGS_FORMAT "%*s | %5s | %4s | %7s | %16s | %8s | %10s\n"
#define TASKDUMP_FORMAT "%*s | %5d | %4d | %7lu | %16p | %8p | %10u\n"
#define TASKDUMP_VALUE  CONFIG_TASK_NAME_SIZE, tcb->name, tcb->pid, tcb->sched_priority, (unsigned long)tcb->adj_stack_size, tcb->stack_alloc_ptr, tcb, tcb->task_state
#define TASKDUMP_ARGS  CONFIG_TASK_NAME_SIZE, "NAME", "PID", "PRI", "TOTAL STACK",  "STACK ALLOC ADDR", "TCB ADDR", "TASK STATE"
#else
#define TASKDUMP_ARGS_FORMAT "%5s | %4s | %7s | %16s | %8s | %10s\n"
#define TASKDUMP_FORMAT "%5d | %4d | %7lu | %16p | %8p | %10u\n"
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

/****************************************************************************
 * Name: task_show_tcbinfo
 * 
 * NOTE : This function is for assert usage only.
 ****************************************************************************/

void task_show_tcbinfo(struct tcb_s *tcb)
{
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("Asserted task's TCB info \n");
	lldbg_noarg("===========================================================\n");
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
	lldbg("Nsyscalls   : %u\n", tcb->xcp.nsyscalls);
	lldbg("Syscall     : %p\n", tcb->xcp.syscall);
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
}
