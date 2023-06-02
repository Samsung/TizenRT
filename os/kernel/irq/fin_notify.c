/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <stdbool.h>
#include <arch/irq.h>
#include <sys/types.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/sched.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fin_notify
 *
 * Description:
 *   This function sends an irq event to specific pid task.
 *   If that task was waiting FIN, it will be unblocked when receiving irq event.
 *   But if that task was waiting another object, such as semaphore, this will update
 *  the data only into the specific pid task's tcb, not make it be unblocked.
 *
 * Inputs:
 *   pid - The task ID of the task to receive the irq event.
 *   data - The data which passed to user.
 *
 * Return Value:
 *   OK on success; or ERROR on failure
 *
 ****************************************************************************/

int fin_notify(pid_t pid, int data)
{
	int saved_state;
	struct tcb_s *tcb;

	saved_state = enter_critical_section();

	tcb = sched_gettcb(pid);
	if (tcb == NULL) {
		return ERROR;
	}

	if (tcb->fin_data == NO_FIN_DATA) {
		/* If this is the first irq, save the data in the first. */
		tcb->fin_data = data;
	} else {
		/* If there is already pending irq, queueing new data. */
		tcb->pending_fin_data = data;
	}

	if (tcb->task_state == TSTATE_WAIT_FIN) {
		up_unblock_task(tcb);
	}

	leave_critical_section(saved_state);

	return OK;
}
