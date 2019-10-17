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

extern bool abort_mode;
extern sq_queue_t g_sem_list;

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/* Reboot the board */
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

static void recovery_release_binary_sem(int binid)
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
			if (holder && holder->htcb && holder->htcb->group && holder->htcb->group->tg_loadtask == binid) {
				/* Increase semcount and release itself from holder */
				sem->semcount++;
				sem_releaseholder(sem, holder->htcb);
			}
		}
		sem = sq_next(sem);
	}
	irqrestore(flags);
}

static void recovery_exclude_scheduling_each(FAR struct tcb_s *tcb, FAR void *arg)
{
	int binid;
	irqstate_t flags;

	binid = (int)arg;
	if (binid < 0) {
		return;
	}

	if (tcb->group->tg_loadtask == binid) {

		/* Recover semaphores, message queue, and watchdog timer resources.*/
		task_recover(tcb);

		flags = irqsave();
		/* Remove the TCB from the task list associated with the state */
		dq_rem((FAR dq_entry_t *)tcb, (dq_queue_t *)g_tasklisttable[tcb->task_state].list);
		sched_addblocked(tcb, TSTATE_TASK_INACTIVE);
		irqrestore(flags);
		bmllvdbg("Remove pid %d from task list\n", tcb->pid);
	}
}

/****************************************************************************
 * Name: recovery_exclude_scheduling
 *
 * Description:
 *   This function will move all the tasks and threads created by the binary
 *   i.e input pid to inactive task list to exclude them from scheduling.
 *
 * Input parameters:
 *   pid   -   The pid of the binary, whoes all children to be killed
 *
 * Returned Value:
 *   Zero (OK) on success; otherwise -1 (ERROR) value is returned.
 *
 ****************************************************************************/
static int recovery_exclude_scheduling(int binid)
{
	if (binid < 0) {
		return ERROR;
	}

	/* Exclude all tasks and pthreads created in a binary which has 'binid' from scheduling */
	sched_foreach(recovery_exclude_scheduling_each, (FAR void *)binid);

	/* Release all semaphores held by the threads in binary */
	recovery_release_binary_sem(binid);

	return OK;
}

/****************************************************************************
 * Name: binary_manager_recovery
 *
 * Description:
 *   This function will receive the faulty pid and check if its binary id is one
 *   of the registered binary with binary manager.
 *   If the binary is registered, it excludes its children from scheduling
 *   and creates loading thread which will terminate them and load binary again.
 *   Otherwise, board will be rebooted.
 *
 * Input parameters:
 *   pid   -   The pid of recovery message
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void binary_manager_recovery(int pid)
{
	int ret;
	int bin_id;
	int bin_idx;
	char type_str[1];
	struct tcb_s *tcb;
	char *loading_data[LOADTHD_ARGC + 1];

	bmllvdbg("Try to recover fault with pid %d\n", pid);

	if (pid > 0) {
		/* Get binary id of fault task and check it is registered in binary manager */
		tcb = sched_gettcb(pid);
		if (tcb == NULL || tcb->group == NULL || tcb->group->tg_loadtask < 0) {
			bmlldbg("Failed to get pid %d binary info\n", pid);
			goto reboot_board;
		}
		bin_id = tcb->group->tg_loadtask;
		bmllvdbg("pid %d, binary id %d\n", pid, bin_id);

		bin_idx = binary_manager_get_index_with_binid(bin_id);
		if (bin_idx < 0) {
			bmlldbg("binary pid %d is not registered to binary manager\n", bin_id);
			goto reboot_board;
		}

		/* Exclude its all children from scheduling if the binary is registered with the binary manager */
		ret = recovery_exclude_scheduling(bin_id);
		if (ret == OK) {
			/* load binary and update binid */
			BIN_STATE(bin_idx) = BINARY_FAULT;
			memset(loading_data, 0, sizeof(char *) * (LOADTHD_ARGC + 1));
			loading_data[0] = itoa(LOADCMD_RELOAD, type_str, 10);
			loading_data[1] = BIN_NAME(bin_idx);
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
