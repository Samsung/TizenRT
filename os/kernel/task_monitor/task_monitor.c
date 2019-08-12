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
#include <sched.h>
#include <sys/boardctl.h>
#include <tinyara/sched.h>
#include "task_monitor_internal.h"

static int monitored_tasks_list[CONFIG_MAX_TASKS];

void task_monitor_update_list(int pid, int status)
{
	monitored_tasks_list[PIDHASH(pid)] = status;
}

/****************************************************************************
 * Main Function
 ****************************************************************************/
int task_monitor(int argc, char *argv[])
{
	int pid_idx;
	struct tcb_s *tcb;

	/* Initialize the monitored tasks list */
	for (pid_idx = 0; pid_idx < CONFIG_MAX_TASKS; pid_idx++) {
		monitored_tasks_list[pid_idx] = NOT_MONITORED;
	}

	while (1) {
		if (sleep(CONFIG_TASK_MONITOR_INTERVAL) > 0) {
			/* Wake up because of signal.
			 * In this case, reset the interval again.
			 */
			continue;
		}

		/* Check if registered tasks/pthread is alive or not.
		 * If one of them is not alive, task monitor resets the system.
		 */
		for (pid_idx = 0; pid_idx < CONFIG_MAX_TASKS; pid_idx++) {
			if (monitored_tasks_list[pid_idx] == MONITORED) {
				tcb = sched_gettcb(pid_idx);
				if (tcb == NULL) {
					/* Invalid operation */
					boardctl(BOARDIOC_RESET, 0);
				}

				if (tcb->is_active == false) {
					/* There is not alive task/pthread.
					 * System will be reset.
					 */
					boardctl(BOARDIOC_RESET, 0);
				} else {
					/* Reset the registered task's/pthread's active flag. */
					tcb->is_active = false;
				}
			}
		}
	}

	return 0;
}
