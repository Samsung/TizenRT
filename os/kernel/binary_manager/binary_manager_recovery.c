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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <queue.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/init.h>
#include <tinyara/board.h>
#include <tinyara/wdog.h>
#include <tinyara/reboot_reason.h>

#include "task/task.h"
#include "sched/sched.h"
#include "semaphore/semaphore.h"
#include "binary_manager.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Move tcb from current state list to inactive list */
#define BM_DEACTIVATE_TASK(tcb) \
	do { \
		dq_rem((FAR dq_entry_t *)tcb, (dq_queue_t *)g_tasklisttable[tcb->task_state].list); \
		dq_addlast((FAR dq_entry_t *)tcb, (FAR dq_queue_t *)g_tasklisttable[TSTATE_TASK_INACTIVE].list); \
		tcb->task_state = TSTATE_TASK_INACTIVE; \
	} while (0)

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
/* The maximum count of fault messages */
#define FAULTMSG_COUNT         USER_BIN_COUNT

extern bool abort_mode;

struct tcb_s *g_faultmsg_sender;
sq_queue_t g_faultmsg_list;
sq_queue_t g_freemsg_list;
static faultmsg_t g_prealloc_faultmsg[FAULTMSG_COUNT];

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_recover_tcb
 *
 * Description:
 *	 This function recovers resources, semaphores, message queue, and watchdog.
 *
 ****************************************************************************/
static void binary_manager_recover_tcb(struct tcb_s *tcb)
{
	sem_t *sem;
	int state;

	state = tcb->task_state;

	if (tcb->waitdog) {
		(void)wd_delete(tcb->waitdog);
		tcb->waitdog = NULL;
	}

	if (state == TSTATE_WAIT_SEM) {
		sem = tcb->waitsem;
		ASSERT(sem != NULL && sem->semcount < 0);
		sem_canceled(tcb, sem);
		sem->semcount++;
		tcb->waitsem = NULL;
	} else if (state == TSTATE_WAIT_MQNOTEMPTY) {
		ASSERT(tcb->msgwaitq && tcb->msgwaitq->nwaitnotempty > 0);
		tcb->msgwaitq->nwaitnotempty--;
	} else if (state == TSTATE_WAIT_MQNOTFULL) {
		ASSERT(tcb->msgwaitq && tcb->msgwaitq->nwaitnotfull > 0);
		tcb->msgwaitq->nwaitnotfull--;
	}
}

/****************************************************************************
 * Name: binary_manager_deactivate_binary
 *
 * Description:
 *   This function excludes all active threads in a same binary from scheduling
 *   and all user binaries' state are changed to BINARY_FAULT.
 *
 ****************************************************************************/
static int binary_manager_deactivate_binary(int bin_idx)
{
	irqstate_t flags;
	struct tcb_s *ptr;

	if (bin_idx < 0 || bin_idx > USER_BIN_COUNT) {
		bmdbg("Invalid binary index, %d\n", bin_idx);
		return BINMGR_INVALID_PARAM;
	}

	/* Update binary state */
	BIN_STATE(bin_idx) = BINARY_FAULT;

	if (bin_idx == BM_CMNLIB_IDX) {
		/*
		 * In case of common binary, it doesn't need to deactivate binary
		 * because all threads of common binary are linked to a list of user binary and they are deactivated by user binary's deactivation.
		 * So it is enough to update state of common binary to BINARY_FAULT.
		 */
		return BINMGR_OK;
	}

	/* Get a tcb of main task */
	ptr = BIN_NRTLIST(bin_idx);
	while (ptr) {
		flags = enter_critical_section();
		/* Recover semaphores, message queue, and watchdog timer resources.*/
		binary_manager_recover_tcb(ptr);
		/* Remove the TCB from the task list associated with the state */
		BM_DEACTIVATE_TASK(ptr);
		ptr = ptr->bin_flink;
		leave_critical_section(flags);
	}
	/* Release all kernel semaphores held by the threads in binary */
	binary_manager_release_binary_sem(bin_idx);

	return BINMGR_OK;
}

/****************************************************************************
 * Name: binary_manager_unblock_fault_message_sender
 *
 * Description:
 *	 This function adds a fault message to the fault message list
 *   and unblocks fault message sender.
 *
 ****************************************************************************/
static void binary_manager_unblock_fault_message_sender(int bin_idx)
{
	struct faultmsg_s *msg;

	/* Check there are a fault message sender and available fault message */
	if (g_faultmsg_sender && (msg = (faultmsg_t *)sq_remfirst(&g_freemsg_list))) {
		msg->binidx = bin_idx;
		sq_addlast((sq_entry_t *)msg, (sq_queue_t *)&g_faultmsg_list);

		/* Unblock fault message sender */
		if (g_faultmsg_sender->task_state == TSTATE_WAIT_FIN) {
			up_unblock_task_without_savereg(g_faultmsg_sender);
		}
		return;
	}

	/* Board reset on failure of recovery */
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_deactivate_rtthreads
 *
 * Description:
 *	 This function excludes real-time threads in a same binary from scheduling.
 *
 ****************************************************************************/
void binary_manager_deactivate_rtthreads(int bin_idx)
{
	struct tcb_s *ptr;

	if (bin_idx > 0) {
		ptr = BIN_RTLIST(bin_idx);
		while (ptr) {
			/* Recover semaphores, message queue, and watchdog timer resources.*/
			binary_manager_recover_tcb(ptr);
			/* Remove the TCB from the task list associated with the state */
			BM_DEACTIVATE_TASK(ptr);
			ptr = ptr->bin_flink;
		}
	}
}


/****************************************************************************
 * Name: binary_manager_recover_userfault
 *
 * Description:
 *	 This function deactivates RT threads and unblocks fault message sender.
 *
 ****************************************************************************/
void binary_manager_recover_userfault(void)
{
	int bin_idx;
	/* Get a tcb of fault thread for fault handling */
	struct tcb_s *tcb = this_task();
#ifndef CONFIG_BINMGR_RELOAD_REBOOT // Board Reset for binary reloading
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	/* If a fault happens in common binary or user binaries, it needs to reload all user binaries */
	int bin_count = binary_manager_get_ucount();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		/* Exclude its all children from scheduling if the binary is registered with the binary manager */
		binary_manager_deactivate_rtthreads(bin_idx);
	}
	/* Send fault message and Unblock fault message sender */
	bin_idx = tcb->group->tg_binidx;
	return binary_manager_unblock_fault_message_sender(bin_idx);
#else
	if (tcb != NULL && tcb->group != NULL) {
		/* Exclude realtime task/pthreads from scheduling */
		bin_idx = tcb->group->tg_binidx;
		binary_manager_deactivate_rtthreads(bin_idx);

		/* Send fault message and Unblock fault message sender */
		return binary_manager_unblock_fault_message_sender(bin_idx);
	}
#endif
#endif
	/* Board reset on failure of recovery */
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
}


/****************************************************************************
 * Name: binary_manager_set_faultmsg_sender
 *
 * Description:
 *	 This function sets tcb of Fault message Sender.
 *
 ****************************************************************************/
void binary_manager_set_faultmsg_sender(pid_t pid)
{
	g_faultmsg_sender = sched_gettcb(pid);
}

/****************************************************************************
 * Name: binary_manager_faultmsg_sender
 *
 * Description:
 *	 This thread waits for fault message which is sent by fault handler.
 *   It is unblocked by fault handler with fault message when fault happens.
 *   And then, it sends message with binary id to binary manager for recovery.
 *
 ****************************************************************************/
int binary_manager_faultmsg_sender(int argc, char *argv[])
{
	int idx;
	int ret;
	faultmsg_t *msg;
	binmgr_request_t request_msg;

	/* Initialize pre-allocated fault messages */

	for (idx = 0; idx < FAULTMSG_COUNT; idx++) {
		sq_addlast((FAR sq_entry_t *)&g_prealloc_faultmsg[idx], (FAR sq_queue_t *)&g_freemsg_list);
	}

	while (1) {
		/* Wait for fault messages and handle it */
		up_block_task(this_task(), TSTATE_WAIT_FIN);
		while (!sq_empty(&g_faultmsg_list)) {
			msg = (struct faultmsg_s *)sq_remfirst(&g_faultmsg_list);
			request_msg.cmd = BINMGR_FAULT;
			request_msg.requester_pid = msg->binidx;
			bmllvdbg("Send fault message, bin id %d\n", request_msg.requester_pid);
			ret = mq_send(binary_manager_get_mqfd(), (const char *)&request_msg, sizeof(binmgr_request_t), BINMGR_FAULT_PRIO);
			ASSERT(ret == OK);
			sq_addlast((FAR sq_entry_t *)msg, (FAR sq_queue_t *)&g_freemsg_list);
		}
	}

	return 0;
}

/****************************************************************************
 * Name: binary_manager_recovery
 *
 * Description:
 *   This function will receive the binary id which is one of the registered binary with binary manager.
 *   If the binary is registered, it excludes its children from scheduling
 *   and creates loading thread which will terminate them and load binary again.
 *   Otherwise, board will be rebooted.
 *
 ****************************************************************************/
void binary_manager_recovery(int bin_idx)
{
	int ret;

	bmllvdbg("Try to recover fault with binid %d\n", bin_idx);

	if (bin_idx < 0) {
		goto reboot_board;
	}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	/* If a fault happens in common or user binaries, we need to reload the library and all user binaries */
	int bidx;
	int bin_count = binary_manager_get_ucount();

	for (bidx = 0; bidx <= bin_count; bidx++) {
		/* Exclude its all children from scheduling if the binary is registered with the binary manager */
		ret = binary_manager_deactivate_binary(bidx);
		if (ret != BINMGR_OK) {
			bmlldbg("Failed to deactivate binary, bin idx %d\n", bidx);
			goto reboot_board;
		}
	}
#else
	/* Exclude its all children from scheduling if the binary is registered with the binary manager */
	ret = binary_manager_deactivate_binary(bin_idx);
	if (ret != BINMGR_OK) {
		bmlldbg("Failed to deactivate binary, bin idx %d\n", bin_idx);
		goto reboot_board;
	}
#endif
	/* Create loader to reload binary */
	ret = binary_manager_execute_loader(LOADCMD_RELOAD, bin_idx);
	if (ret == OK) {
		abort_mode = false;
		bmllvdbg("Loading thread with pid %d will reload binaries!\n", ret);
		return;
	}

reboot_board:
	/* Reboot the board  */
	bmlldbg("RECOVERY FAIL, BOARD RESET!!\n");
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
}
