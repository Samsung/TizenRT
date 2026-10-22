/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#ifndef __KERNEL_HEALTH_MONITOR_H
#define __KERNEL_HEALTH_MONITOR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <tinyara/compiler.h>
#include <tinyara/health_monitor.h>

#ifdef CONFIG_HEALTH_MONITOR

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define HEALTH_MONITOR_HEAP_CAPACITY     CONFIG_MAX_TASKS

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* KICK changes only the monitored deadline. The heap retains the earlier check_at
 * until the timer inspects the entry. Storage is allocated statically by
 * the implementation, with one entry per possible registered thread.
 */

struct tcb_s;

struct health_monitor_entry_s {
	struct tcb_s *tcb;
	clock_t check_at;
};

struct health_monitor_snapshot_s {
	uintptr_t tcb_address;
	pid_t pid;
	clock_t deadline;
	uint32_t timeout;
	uint8_t task_state;
	uint8_t priority;
	uint8_t cpu;
	bool kernel_task;
#if CONFIG_TASK_NAME_SIZE > 0
	char name[CONFIG_TASK_NAME_SIZE + 1];
#endif
};

enum health_monitor_check_result_e {
	HEALTH_MONITOR_CHECK_OK,
	HEALTH_MONITOR_CHECK_DEFERRED,
	HEALTH_MONITOR_CHECK_TIMEOUT
};

enum health_monitor_failure_type_e {
	HEALTH_MONITOR_FAILURE_TIMEOUT,
	HEALTH_MONITOR_FAILURE_UNEXPECTED_EXIT
};

struct health_monitor_failure_s {
	struct health_monitor_snapshot_s task;
	clock_t detected_at;
	enum health_monitor_failure_type_e type;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

bool health_monitor_is_tick_before(clock_t lhs, clock_t rhs);

void health_monitor_timer(void);
int health_monitor_get_next_check_time(clock_t *check_at);
void health_monitor_init_task(struct tcb_s *tcb);
void health_monitor_deinit_task(struct tcb_s *tcb);

#endif /* CONFIG_HEALTH_MONITOR */
#endif /* __KERNEL_HEALTH_MONITOR_H */
