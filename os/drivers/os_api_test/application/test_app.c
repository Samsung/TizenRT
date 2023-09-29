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

#include <debug.h>
#include <errno.h>
#include <queue.h>

#include <tinyara/mpu.h>
#include <tinyara/sched.h>
#include <tinyara/os_api_test_drv.h>

#include "sched/sched.h"
#include "../../../binfmt/libelf/libelf.h"
#include <binary_manager/binary_manager.h>

#ifdef CONFIG_SYSTIME_TIME64
extern uint64_t app_start_time_sec[3];
#else
extern uint32_t app_start_time_sec[3];
#endif
extern unsigned int app_start_time_csec[3];

char const *print_task_state[] = {"TSTATE_TASK_INVALID", "TSTATE_TASK_PENDING", "TSTATE_TASK_READYTORUN", "TSTATE_TASK_RUNNING", "TSTATE_TASK_INACTIVE", "TSTATE_WAIT_SEM", "TSTATE_WAIT_FIN"
#ifndef CONFIG_DISABLE_SIGNALS
		, "TSTATE_WAIT_SIG"
#endif
#ifndef CONFIG_DISABLE_MQUEUE
		, "TSTATE_WAIT_MQNOTEMPTY", "TSTATE_WAIT_MQNOTFULL"
#endif
#ifdef CONFIG_PAGING
		, "TSTATE_WAIT_PAGEFILL"
#endif
};
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tasklist_stackdump
 *
 * Description:  This function prints the stack dump of all the tasks in the
 * alive thread list.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_STACKDUMP
static void tasklist_stackdump(FAR struct tcb_s *tcb, FAR void *arg)
{
	uint32_t sp = tcb->xcp.regs[REG_R13];
	uint32_t stack_base = (uint32_t)tcb->adj_stack_ptr - tcb->adj_stack_size;
	uint32_t stack = sp & ~0x1f;
	uint32_t *ptr = (uint32_t *)stack;
	uint8_t i;

	dbg("%*s: %08x:", CONFIG_TASK_NAME_SIZE, tcb->name, stack);
	for (i = 0; i < 8; i++) {  // Print first 8 values for sp located
		if (stack < sp) {
			/* If stack pointer(sp) is aligned(sp & 0x1f) to an address outside allocated stack */
			/* Then, for stack addresses outside allocated stack, print 'xxxxxxxx' */
			dbg_noarg(" xxxxxxxx");
		} else {
			/* For remaining stack addresses inside allocated stack, print proper stack address values */
			dbg_noarg(" %08x", ptr[i]);
		}
		stack += 4;
	}
	dbg_noarg("\n");

	for (; stack < stack_base; stack += 32) { // Print remaining stack values from 9th value to end of stack
		ptr = (uint32_t *)stack;
		dbg("%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   stack, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	}
}
#endif

/****************************************************************************
 * Name: task_info_dump
 *
 * Description:  This function prints the TCB info of all the tasks in the
 * alive thread list in the orde task name, pid, priority,type, task state,
 * location of binary, used stack, total stack, stack alloc addr, stack
 * range and current stack pointer.
 *
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
static void task_info_dump(FAR struct tcb_s *tcb, FAR void *arg)
{
	size_t used_stack_size = up_check_tcbstack(tcb);
#if defined(CONFIG_BINARY_MANAGER)
	char *bin_loc = "kernel";
	if ((strncmp(BIN_NAME(tcb->group->tg_binidx), "app1", 4) == 0) || (strncmp(BIN_NAME(tcb->group->tg_binidx), "app2", 4) == 0) || (strncmp(BIN_NAME(tcb->group->tg_binidx), "common", 6) == 0)) {
		bin_loc = BIN_NAME(tcb->group->tg_binidx);
	}
#endif
#if CONFIG_TASK_NAME_SIZE > 0
        dbg("%*s | %5d | %4d | %5c | %22s | %9s | %7lu / %7lu     | %16p | 0x%08x - 0x%08x | 0x%08x\n", CONFIG_TASK_NAME_SIZE,
                        tcb->name, tcb->pid, tcb->sched_priority, ((tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL) ? 'P':'U', print_task_state[tcb->task_state],
#if defined(CONFIG_BINARY_MANAGER)
			bin_loc,
#endif
                        (unsigned long)used_stack_size, (unsigned long)tcb->adj_stack_size, tcb->stack_alloc_ptr, tcb->adj_stack_ptr, (uint32_t)tcb->adj_stack_ptr - tcb->adj_stack_size,
                        tcb->xcp.regs[REG_R13]);
#else
	dbg("%5d | %4d | %7lu / %7lu     | %16p | %22s | %9s\n",
			tcb->pid, tcb->sched_priority, (unsigned long)used_stack_size,
			(unsigned long)tcb->adj_stack_size, tcb->stack_alloc_ptr, print_task_state[tcb->task_state], BIN_NAME(tcb->group->tg_binidx);
#endif
}
#endif

/****************************************************************************
 * Name: test_gtasklist_info
 *
 * Description:  This function prints the TCB info of all the lists in
 * g_tasklisttable[]
 *
 ****************************************************************************/

uint8_t test_gtasklist_info(int cmd, unsigned long arg)
{
	uint8_t task_state_idx;
	FAR struct tcb_s *glisttcb;
	FAR struct tcb_s *glistnext;

	switch (cmd) {
	case TESTIOC_GET_TCBLIST_INFO:
		dbg("< ----------------- Gtask list table ----------------- >\n");
		dbg(" %21s | %*s\n", "g_tasklisttable", CONFIG_TASK_NAME_SIZE, "NAME");
		dbg("--------------------------------------------------------\n");
		/* Traverse through g_tasklisttable for NUM_TASK_STATES to print individual lists */
		for (task_state_idx = 1; task_state_idx < NUM_TASK_STATES; task_state_idx++) {
			glisttcb = (FAR struct tcb_s *)((FAR dq_queue_t *)g_tasklisttable[task_state_idx].list)->head;
			if (glisttcb) {
				for (glisttcb = (FAR struct tcb_s *)((FAR dq_queue_t *)g_tasklisttable[task_state_idx].list)->head; glisttcb; glisttcb = glistnext) {
					switch (task_state_idx) {
					case TSTATE_TASK_PENDING:
						dbg("g_pendingtasks         : ");
						break;
					case TSTATE_TASK_READYTORUN:
						dbg("g_readytorun           : ");
						break;
					case TSTATE_TASK_RUNNING:
						dbg("g_readytorun           : ");
						break;
					case TSTATE_TASK_INACTIVE:
						dbg("g_inactivetasks        : ");
						break;
					case TSTATE_WAIT_SEM:
						dbg("g_waitingforsemaphore  : ");
						break;
					case TSTATE_WAIT_FIN:
						dbg("g_waitingforfin        : ");
						break;
#ifndef CONFIG_DISABLE_SIGNALS
					case TSTATE_WAIT_SIG:
						dbg("g_waitingforsignal     : ");
						break;
#endif
#ifndef CONFIG_DISABLE_MQUEUE
					case TSTATE_WAIT_MQNOTEMPTY:
						dbg("g_waitingformqnotempty : ");
						break;
					case TSTATE_WAIT_MQNOTFULL:
						dbg("g_waitingformqnotfull  : ");
						break;
#endif
#ifdef CONFIG_PAGING
					case TSTATE_WAIT_PAGEFILL:
						dbg("g_waitingforfill       : ");
						break;
#endif
					default:
						dbg("INVALID!!");
					}
				dbg_noarg("%*s\n", CONFIG_TASK_NAME_SIZE, glisttcb->name);
				glistnext = glisttcb->flink;
				}
			}
		}
	dbg("< ---------------------------------------------------- >\n");
	}
	return OK;
}

/************************************************************************************
 * Name: test_tcb_info
 *
 * Description:  This function retrieves the TCB info of all the tasks
 *
 ************************************************************************************/
uint8_t test_tcb_info(int cmd, unsigned long arg)
{
	switch (cmd) {
	case TESTIOC_GET_TCB_INFO:
#if CONFIG_TASK_NAME_SIZE > 0
		dbg(" %*s | %5s | %4s | %5s | %22s | %9s | %7s / %7s | %16p | %17s       | %8s\n", CONFIG_TASK_NAME_SIZE, "NAME", "PID", "PRI", "TYPE", "TASK STATE", "LOCATION", "USED", "TOTAL STACK", "STACK ALLOC ADDR", "STACK RANGE", "CURRENT SP");
		dbg("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
#else
		dbg("%5s | %4s | %7s / %7s | %22s | %9s | %16s\n", "PID", "PRI", "USED", "TOTAL STACK", "STACK ALLOC ADDR", "TASK STATE", "LOCATION");
		dbg("-----------------------------------------------------------------------------------\n");
#endif
		sched_foreach(task_info_dump, NULL);
	}
	return OK;
}

/************************************************************************************
 * Name: test_tcb_callstack_info
 *
 * Description:  This function retrieves the TCB info of all the tasks
 *
 ************************************************************************************/
uint8_t test_tcb_callstack_info(int cmd, unsigned long arg)
{
	switch (cmd) {
	case TESTIOC_GET_TCBSTACK_INFO:
		/* Traverse through alive thread list to print stack for individual threads */
		sched_foreach(tasklist_stackdump, NULL);
	}

	return OK;
}

/************************************************************************************
 * Name: test_get_app_memory_addr
 *
 * Description:  This function retrieves all the applications' memory address
 * information such as text, ro, data and bss.
 *
 ************************************************************************************/
#ifdef CONFIG_APP_BINARY_SEPARATION
uint8_t test_get_app_memory_addr(int cmd, unsigned long arg)
{
	bin_addr_info_t *info;
	info = (bin_addr_info_t *)sq_peek(&g_bin_addr_list);

	switch (cmd) {
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
	case TESTIOC_GET_APP_MEMORY_ADDR:
		while (info) {
			dbg("Memory information for [%s] binary is:\n", BIN_NAME(info->bin_idx));
			dbg("   text_addr   : 0x%08x, text_size  : %8u\n", info->text_addr, info->text_size);
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
			dbg("   rodata_addr : 0x%08x\n", info->rodata_addr);
			dbg("   data_addr   : 0x%08x\n", info->data_addr);
			dbg("   bss_addr    : 0x%08x\n", info->bss_addr);
#endif
			info = (FAR void *)sq_next((FAR sq_entry_t *)info);
		}
#endif
	}
	return OK;
}

/************************************************************************************
 * Name: test_get_app_start_time
 *
 * Description:  This function retrieves the start time of applications since system
 * boot-up.
 *
 ************************************************************************************/
#ifdef CONFIG_DEBUG_SYSTEM
#ifdef CONFIG_SYSTIME_TIME64
uint64_t test_get_app_start_time_sec(int cmd, unsigned long arg)
#else
uint32_t test_get_app_start_time_sec(int cmd, unsigned long arg)
#endif
{
#ifdef CONFIG_SYSTIME_TIME64
	uint64_t sec = -EINVAL;
#else
	uint32_t sec = -EINVAL;
#endif

	switch (cmd) {
	case TESTIOC_GET_APP_START_TIME_SEC:
		sec = app_start_time_sec[arg];
		break;
	}
	return sec;
}

unsigned int test_get_app_start_time_csec(int cmd, unsigned long arg)
{
	unsigned int csec = 0;

	switch (cmd) {
	case TESTIOC_GET_APP_START_TIME_CSEC:
		csec = app_start_time_csec[arg];
		break;
	}
	return csec;
}
#endif
#endif

/************************************************************************************
 * Name: test_mpu_get_active_regions
 *
 * Description:  This function retrieves the number of active MPU regions.
 *
 ************************************************************************************/
#ifdef CONFIG_ARM_MPU
uint8_t test_mpu_get_active_regions(int cmd, unsigned long arg)
{
	uint8_t ret = -EINVAL;

	switch (cmd) {
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
	case TESTIOC_MPU_GET_ACTIVE_REGIONS:
		ret = mpu_get_nregion_info(MPU_REGION_MAX);
		break;
#endif
	}
	return ret;
}
#endif

