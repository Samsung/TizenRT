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

#include <errno.h>
#include <sched.h>
#include <queue.h>
#include <semaphore.h>

#include <sys/boardctl.h>
#include <tinyara/sched.h>

#include "task_monitor_internal.h"

static task_monitor_node_t g_monitored_tasks_list[CONFIG_MAX_TASKS];
static task_monitor_node_queue_t g_que_list[TASK_MONITOR_CHECK_TIME];
static int g_monitor_cnt;
static sem_t g_stop_sem;

int task_monitor_register_list(int pid, int interval)
{
	if (interval < CONFIG_TASK_MONITOR_INTERVAL || CONFIG_TASK_MONITOR_MAX_INTERVAL < interval) {
		dbg("Must be greater than %d and less than or equal to %d.\n", CONFIG_TASK_MONITOR_INTERVAL, CONFIG_TASK_MONITOR_MAX_INTERVAL);
		return EINVAL;
	}

	int hash_pid = PIDHASH(pid);
	if (g_monitored_tasks_list[hash_pid].interval != 0) {
		dbg("Already registered.\n");
		return EALREADY;
	}

	g_monitored_tasks_list[hash_pid].pid = pid;
	if (interval % CONFIG_TASK_MONITOR_INTERVAL == 0) {
		g_monitored_tasks_list[hash_pid].interval = interval / CONFIG_TASK_MONITOR_INTERVAL;
	} else {
		/* If a value that is not divided, it is rounded up. */
		g_monitored_tasks_list[hash_pid].interval = (interval / CONFIG_TASK_MONITOR_INTERVAL) + 1;
		if (g_monitored_tasks_list[hash_pid].interval > TASK_MONITOR_CHECK_TIME) {
			g_monitored_tasks_list[hash_pid].interval--;
		}
	}

	dq_addlast((FAR dq_entry_t *)&g_monitored_tasks_list[hash_pid], &g_que_list[g_monitored_tasks_list[hash_pid].interval - 1].q);

	g_monitor_cnt++;
	if (g_monitor_cnt == 1) {
		sem_post(&g_stop_sem);
	}
	return OK;
}

void task_monitor_unregester_list(int pid)
{
	int hash_pid;
	int interval;
	hash_pid = PIDHASH(pid);

	if (g_monitored_tasks_list[hash_pid].interval == 0) {
		/* Not registered. */
		return;
	}

	g_monitored_tasks_list[hash_pid].pid = 0;
	interval = g_monitored_tasks_list[hash_pid].interval;
	g_monitored_tasks_list[hash_pid].interval = 0;

	dq_rem((FAR dq_entry_t *)&g_monitored_tasks_list[hash_pid], &g_que_list[interval - 1].q);

	g_monitor_cnt--;
}

static void task_monitor_init(void)
{
	int pid_idx;
	int time_idx;

	/* Initialize the dequeue list */
	for (time_idx = 0; time_idx < TASK_MONITOR_CHECK_TIME; ++time_idx) {
		g_que_list[time_idx].remaining_count = time_idx;
		dq_init(&(g_que_list[time_idx].q));
	}

	/* Initialize the monitored tasks list */
	for (pid_idx = 0; pid_idx < CONFIG_MAX_TASKS; pid_idx++) {
		g_monitored_tasks_list[pid_idx].flink = NULL;
		g_monitored_tasks_list[pid_idx].blink = NULL;
		g_monitored_tasks_list[pid_idx].pid = 0;
		g_monitored_tasks_list[pid_idx].interval = 0;
	}

	g_monitor_cnt = 0;
	sem_init(&g_stop_sem, 0, 0);
}
/****************************************************************************
 * Main Function
 ****************************************************************************/
int task_monitor(int argc, char *argv[])
{
	int time_idx;
	struct tcb_s *tcb;
	task_monitor_node_t *next_mon_node;

	task_monitor_init();

	while (1) {

		if (g_monitor_cnt == 0) {
			/* if g_monitor_cnt is 0,
			 * task_monitor is not working.
			 */
			while (sem_wait(&g_stop_sem) < 0);
		}

		if (sleep(CONFIG_TASK_MONITOR_INTERVAL) > 0) {
			/* Wake up because of signal.
			 * In this case, reset the interval again.
			 */
			continue;
		}

		/* Check if registered tasks/pthread is alive or not.
		 * If one of them is not alive, task monitor resets the system.
		 */
		for (time_idx = 0; time_idx < TASK_MONITOR_CHECK_TIME; ++time_idx) {

			if (g_que_list[time_idx].remaining_count == 0) {

				next_mon_node = (task_monitor_node_t *)dq_peek(&g_que_list[time_idx].q);

				while (next_mon_node != NULL) {

					tcb = sched_gettcb(next_mon_node->pid);

					if (tcb == NULL || tcb->is_active == false) {
						/* Invalid operation and
						*  There is not alive task/pthread.
						*  System will be reset.
						*/
						boardctl(BOARDIOC_RESET, 0);
					} else {
						/* Reset the registered task's/pthread's active flag. */
						tcb->is_active = false;
					}
					next_mon_node = (task_monitor_node_t *)dq_next(next_mon_node);
				}
				/* Restore the check time. */
				g_que_list[time_idx].remaining_count = time_idx;
			} else {
				g_que_list[time_idx].remaining_count--;
			}
		}
	}

	(void)sem_destroy(&g_stop_sem);
	return 0;
}
