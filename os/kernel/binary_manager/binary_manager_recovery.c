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
#include <queue.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#ifdef CONFIG_BOARDCTL_RESET
#include <sys/boardctl.h>
#endif
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/init.h>
#include <tinyara/board.h>
#ifdef CONFIG_SUPPORT_COMMON_BINARY
#include <tinyara/binfmt/binfmt.h>
#endif

#include "task/task.h"
#include "sched/sched.h"
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

#ifdef CONFIG_SUPPORT_COMMON_BINARY
extern struct binary_s *g_lib_binp;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_reset_board
 *
 * Description:
 *	 This function resets the board.
 *
 ****************************************************************************/
static void binary_manager_reset_board(void)
{
#ifdef CONFIG_BOARDCTL_RESET
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
#else
	(void)irqsave();
	sched_lock();
	for (;;) {
		lldbg("\nASSERT!! Push the reset button!\n");
		up_mdelay(10000);
	}
#endif
}

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
 *	 This function excludes all active threads in a same binary from scheduling.
 *
 ****************************************************************************/
static int binary_manager_deactivate_binary(pid_t binid)
{
	irqstate_t flags;
	struct tcb_s *tcb;

	tcb = sched_gettcb(binid);
	if (tcb == NULL) {
		return ERROR;
	}

	/* A main task which has 'binid' is a head of binary list */
	while (tcb) {
		if (tcb->task_state != TSTATE_TASK_INACTIVE) {
			flags = irqsave();
			/* Recover semaphores, message queue, and watchdog timer resources.*/
			binary_manager_recover_tcb(tcb);

			/* Remove the TCB from the task list associated with the state */
			BM_DEACTIVATE_TASK(tcb);
			irqrestore(flags);
		}
		tcb = tcb->bin_flink;
	}
	/* Release all kernel semaphores held by the threads in binary */
	binary_manager_release_binary_sem(binid);

	return OK;
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
void binary_manager_deactivate_rtthreads(struct tcb_s *tcb)
{
	struct tcb_s *prev;
	struct tcb_s *next;

	prev = tcb->bin_blink;
	next = tcb->bin_flink;

	if (tcb->sched_priority > BM_PRIORITY_MAX) {
		if (tcb->waitdog) {
			(void)wd_delete(tcb->waitdog);
			tcb->waitdog = NULL;
		}
		/* Remove the TCB from the task list associated with the state */
		BM_DEACTIVATE_TASK(tcb);
	}

	while (prev) {
		if (prev->sched_priority > BM_PRIORITY_MAX) {
			/* Recover semaphores, message queue, and watchdog timer resources.*/
			binary_manager_recover_tcb(prev);
			/* Remove the TCB from the task list associated with the state */
			BM_DEACTIVATE_TASK(prev);
		}
		prev = prev->bin_blink;
	}

	while (next) {
		if (next->sched_priority > BM_PRIORITY_MAX) {
			/* Recover semaphores, message queue, and watchdog timer resources.*/
			binary_manager_recover_tcb(next);
			/* Remove the TCB from the task list associated with the state */
			BM_DEACTIVATE_TASK(next);
		}
		next = next->bin_flink;
	}
}

/****************************************************************************
 * Name: binary_manager_recover_userfault
 *
 * Description:
 *	 This function deactivates RT threads and unblocks fault message sender.
 *
 ****************************************************************************/
void binary_manager_recover_userfault(uint32_t assert_pc)
{
	int binid;
	int bin_idx;
	struct tcb_s *tcb;
	struct faultmsg_s *msg;

	tcb = this_task();
	if (tcb != NULL && tcb->group != NULL) {
		tcb->lockcount = 0;
		binid = tcb->group->tg_binid;
		bin_idx = binary_manager_get_index_with_binid(binid);
		if (BIN_RTTYPE(bin_idx) == BINARY_TYPE_REALTIME) {
			/* Exclude realtime task/pthreads from scheduling */
			binary_manager_deactivate_rtthreads(tcb);
		}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
		if (g_lib_binp) {
			uint32_t start = (uint32_t)g_lib_binp->alloc[ALLOC_TEXT];
			uint32_t end = start + g_lib_binp->textsize;
			if (assert_pc >= start && assert_pc <= end) {
				binid = BM_BINID_LIBRARY;
			}
		}
#endif
		/* Add fault message and Unblock Fault message Sender */
		if (g_faultmsg_sender && (msg = (faultmsg_t *)sq_remfirst(&g_freemsg_list))) {
			msg->binid = binid;
			sq_addlast((sq_entry_t *)msg, (sq_queue_t *)&g_faultmsg_list);

			/* Unblock fault message sender */
			if (g_faultmsg_sender->task_state == TSTATE_WAIT_FIN) {
				up_recovery_unblock_task(g_faultmsg_sender);
			}
			return;
		}
	}

	/* Board reset on failure of recovery */
	binary_manager_reset_board();
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
			request_msg.requester_pid = msg->binid;
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
void binary_manager_recovery(int binid)
{
	int ret;
	int bin_idx;
	char type_str[1];
	char data_str[1];
	char *loading_data[LOADTHD_ARGC + 1];

	bmllvdbg("Try to recover fault with binid %d\n", binid);

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	/* binid is zero means a crash happened in common library
	 * We need to reload the library and all the apps
	 */
	if (binid == BM_BINID_LIBRARY) {
		int bin_count = binary_manager_get_ucount();
		for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
			binid = BIN_ID(bin_idx);
			/* Exclude its all children from scheduling if the binary is registered with the binary manager */
			ret = binary_manager_deactivate_binary(binid);

			if (ret != OK) {
				bmlldbg("Failure during recovery excluding binary pid = %d\n", binid);
				goto reboot_board;
			}

			BIN_STATE(bin_idx) = BINARY_FAULT;
		}

		binid = BM_BINID_LIBRARY;
	}
#endif

	if (binid > 0) {
		/* Get binary id of fault task and check it is registered in binary manager */
		bin_idx = binary_manager_get_index_with_binid(binid);
		if (bin_idx < 0) {
			bmlldbg("binary pid %d is not registered to binary manager\n", binid);
			goto reboot_board;
		}

		/* Exclude its all children from scheduling if the binary is registered with the binary manager */
		ret = binary_manager_deactivate_binary(binid);
		if (ret != OK) {
			bmlldbg("Failure during recovery excluding binary pid = %d\n", binid);
			goto reboot_board;
		}
		BIN_STATE(bin_idx) = BINARY_FAULT;
	}

	memset(loading_data, 0, sizeof(char *) * (LOADTHD_ARGC + 1));
	loading_data[0] = itoa(LOADCMD_RELOAD, type_str, 10);
	loading_data[1] = itoa(binid, data_str, 10);
	loading_data[2] = NULL;
	ret = binary_manager_loading(loading_data);
	if (ret > 0) {
		abort_mode = false;
		bmllvdbg("Loading thread with pid %d will reload binaries!\n", ret);
		return;
	}

reboot_board:
	/* Reboot the board  */
	bmlldbg("RECOVERY FAIL, BOARD RESET!!\n");
	binary_manager_reset_board();
}
