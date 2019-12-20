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
#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/init.h>
#include <tinyara/board.h>

#include "task/task.h"
#include "sched/sched.h"
#include "binary_manager.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define BM_EXCLUDE_SCHEDULING(tcb) \
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
extern sq_queue_t g_sem_list;

struct tcb_s *g_faultmsg_sender;
sq_queue_t g_faultmsg_list;
sq_queue_t g_freemsg_list;
static faultmsg_t g_prealloc_faultmsg[FAULTMSG_COUNT];

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_board_reset
 *
 * Description:
 *	 This function resets the board.
 *
 ****************************************************************************/
static void binary_manager_board_reset(void)
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
 * Name: recovery_release_binary_sem
 *
 * Description:
 *	 This function releases all kernel semaphores held by the threads in binary.
 *
 ****************************************************************************/
void recovery_release_binary_sem(int binid)
{
	sem_t *sem;
	irqstate_t flags;
	FAR struct semholder_s *holder;

	flags = irqsave();

	sem = (sem_t *)sq_peek(&g_sem_list);
	while (sem) {
#if CONFIG_SEM_PREALLOCHOLDERS > 0
		for (holder = sem->hhead; holder; holder = holder->flink)
#else
		holder = &sem->holder;
#endif
		{
			if (holder && holder->htcb && holder->htcb->group && holder->htcb->group->tg_binid == binid) {
				/* Increase semcount and release itself from holder */
				sem->semcount++;
				sem_releaseholder(sem, holder->htcb);
			}
		}
		sem = sq_next(sem);
	}
	irqrestore(flags);
}

/****************************************************************************
 * Name: recovery_recover_tcb
 *
 * Description:
 *	 This function recovers resources, semaphores, message queue, and watchdog.
 *
 ****************************************************************************/
static void recovery_recover_tcb(struct tcb_s *tcb)
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
 * Name: recovery_exclude_binary
 *
 * Description:
 *	 This function excludes all active threads in a same binary from scheduling.
 *
 ****************************************************************************/
static int recovery_exclude_binary(pid_t binid)
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
			recovery_recover_tcb(tcb);

			/* Remove the TCB from the task list associated with the state */
			BM_EXCLUDE_SCHEDULING(tcb);
			irqrestore(flags);
		}
		tcb = tcb->bin_flink;
	}
	/* Release all kernel semaphores held by the threads in binary */
	recovery_release_binary_sem(binid);

	return OK;
}

/****************************************************************************
 * Name: binary_manager_exclude_rtthreads
 *
 * Description:
 *	 This function excludes real-time threads in a same binary from scheduling.
 *
 ****************************************************************************/
void binary_manager_exclude_rtthreads(struct tcb_s *tcb)
{
	struct tcb_s *prev;
	struct tcb_s *next;

	prev = tcb->bin_blink;
	next = tcb->bin_flink;

	while (prev) {
		if (prev->sched_priority > BM_PRIORITY_MAX) {
			/* Recover semaphores, message queue, and watchdog timer resources.*/
			recovery_recover_tcb(prev);
			/* Remove the TCB from the task list associated with the state */
			BM_EXCLUDE_SCHEDULING(prev);
		}
		prev = prev->bin_blink;
	}

	while (next) {
		if (next->sched_priority > BM_PRIORITY_MAX) {
			/* Recover semaphores, message queue, and watchdog timer resources.*/
			recovery_recover_tcb(next);
			/* Remove the TCB from the task list associated with the state */
			BM_EXCLUDE_SCHEDULING(next);
		}
		next = next->bin_flink;
	}
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
		up_block_task(this_task(), TSTATE_WAIT_UNBLOCK);
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

	if (binid > 0) {
		/* Get binary id of fault task and check it is registered in binary manager */
		bin_idx = binary_manager_get_index_with_binid(binid);
		if (bin_idx < 0) {
			bmlldbg("binary pid %d is not registered to binary manager\n", binid);
			goto reboot_board;
		}

		/* Exclude its all children from scheduling if the binary is registered with the binary manager */
		ret = recovery_exclude_binary(binid);
		if (ret == OK) {
			/* load binary and update binid */
			BIN_STATE(bin_idx) = BINARY_FAULT;
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
		}
	}

reboot_board:
	/* Reboot the board  */
	bmlldbg("RECOVERY FAIL, BOARD RESET!!\n");
	binary_manager_board_reset();
}
