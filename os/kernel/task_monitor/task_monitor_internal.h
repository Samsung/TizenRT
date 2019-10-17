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

#ifndef __SCHED_TASK_MONITOR_TASK_MONITOR_INTERNAL_H
#define __SCHED_TASK_MONITOR_TASK_MONITOR_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <queue.h>

#ifdef CONFIG_TASK_MONITOR

#define TASK_MONITOR_CHECK_TIME    (CONFIG_TASK_MONITOR_MAX_INTERVAL / CONFIG_TASK_MONITOR_INTERVAL)

struct task_monitor_node_s {
	FAR struct task_monitor_node_s *flink;	/* Doubly linked list */
	FAR struct task_monitor_node_s *blink;
	int pid;				/* tcb's pid */
	int interval;
};

typedef struct task_monitor_node_s task_monitor_node_t;

struct task_monitor_node_queue_s {
	dq_queue_t q;
	int remaining_count;
};

typedef struct task_monitor_node_queue_s task_monitor_node_queue_t;

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
int task_monitor(int argc, char *argv[]);
int task_monitor_register_list(int pid, int interval);
void task_monitor_unregester_list(int pid);
#endif							/* CONFIG_TASK_MONITOR */
#endif							/* __SCHED_TASK_MONITOR_TASK_MONITOR_INTERNAL_H */
