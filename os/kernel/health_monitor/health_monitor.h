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
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <tinyara/compiler.h>
#include <tinyara/health_monitor.h>
#include <tinyara/sched.h>
#include <tinyara/spinlock.h>

#ifdef CONFIG_HEALTH_MONITOR

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define HEALTH_MONITOR_HEAP_CAPACITY     CONFIG_MAX_TASKS

/****************************************************************************
 * Public Types
 ****************************************************************************/

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
 * Public Variables
 ****************************************************************************/

extern struct tcb_s *g_health_heap[HEALTH_MONITOR_HEAP_CAPACITY];
extern uint32_t g_health_count;
extern volatile uint32_t g_health_sequence;
extern volatile uint32_t g_health_next_count;
extern volatile clock_t g_health_next_tick;
extern spinlock_t g_health_lock;
extern bool g_health_watchdog_started;

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_get_state
 *
 * Description:
 *   Return the health monitor state embedded in a TCB.
 *
 * Inputs:
 *   tcb - Target TCB.
 *
 * Return Value:
 *   Pointer to the TCB's health monitor state.
 *
 * Assumptions:
 *   Caller must protect TCB lifetime and serialize access to its state.
 *
 ****************************************************************************/

static inline struct health_monitor_s *health_monitor_get_state(struct tcb_s *tcb)
{
	DEBUGASSERT(tcb != NULL);
	return &tcb->health_monitor;
}

/****************************************************************************
 * Name: health_monitor_earlier
 *
 * Description:
 *   Return whether lhs is an earlier deadline than rhs relative to now.
 *
 * Inputs:
 *   lhs  - First deadline tick.
 *   rhs  - Second deadline tick.
 *   now  - Reference tick for comparison.
 *
 * Return Value:
 *   True if lhs is earlier than rhs; false otherwise.
 *
 * Assumptions:
 *   Ticks must be within the supported half-range of now.
 *
 ****************************************************************************/

static inline bool health_monitor_earlier(clock_t lhs, clock_t rhs, clock_t now)
{
#ifdef CONFIG_SYSTEM_TIME64
	return (int64_t)(lhs - now) < (int64_t)(rhs - now);
#else
	return (int32_t)(lhs - now) < (int32_t)(rhs - now);
#endif
}

/****************************************************************************
 * Name: health_monitor_is_tick_before
 *
 * Description:
 *   Return whether lhs tick is before rhs tick.
 *
 * Inputs:
 *   lhs - First tick.
 *   rhs - Second tick.
 *
 * Return Value:
 *   True if lhs is before rhs; false otherwise.
 *
 * Assumptions:
 *   Separation must be less than half the clock range.
 *
 ****************************************************************************/

static inline bool health_monitor_is_tick_before(clock_t lhs, clock_t rhs)
{
#ifdef CONFIG_SYSTEM_TIME64
	return (int64_t)(lhs - rhs) < 0;
#else
	return (int32_t)(lhs - rhs) < 0;
#endif
}

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

void health_monitor_timer(void);
int health_monitor_get_next_check_time(clock_t *check_at);
void health_monitor_init_task(struct tcb_s *tcb);
void health_monitor_deinit_task(struct tcb_s *tcb);

void health_monitor_snapshot(struct tcb_s *tcb, struct health_monitor_snapshot_s *info);
void health_monitor_fail(const struct health_monitor_failure_s *failure);
void health_monitor_publish(void);
void health_monitor_sift_up(uint32_t index, clock_t now);
void health_monitor_sift_down(uint32_t index, clock_t now);

#endif /* CONFIG_HEALTH_MONITOR */
#endif /* __KERNEL_HEALTH_MONITOR_H */
