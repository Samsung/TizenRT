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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <string.h>
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Global Variables
 ****************************************************************************/

struct tcb_s *g_health_heap[HEALTH_MONITOR_HEAP_CAPACITY];
uint32_t g_health_count;
spinlock_t g_health_lock;
volatile uint32_t g_health_sequence;
volatile uint32_t g_health_next_count;
volatile clock_t g_health_next_tick;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_snapshot
 *
 * Description:
 *   Capture target metadata while the registry lock protects its HM state.
 ****************************************************************************/

void health_monitor_snapshot(struct tcb_s *tcb, struct health_monitor_snapshot_s *info)
{
	info->tcb_address = (uintptr_t)tcb;
	info->pid = tcb->pid;
	info->deadline = health_monitor_get_state(tcb)->deadline;
	info->timeout = health_monitor_get_state(tcb)->timeout;
	info->task_state = tcb->task_state;
	info->priority = tcb->sched_priority;
	info->kernel_task = (tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL;
#ifdef CONFIG_SMP
	info->cpu = tcb->cpu;
#else
	info->cpu = 0;
#endif
#if CONFIG_TASK_NAME_SIZE > 0
	memcpy(info->name, tcb->name, sizeof(info->name));
	info->name[CONFIG_TASK_NAME_SIZE] = '\0';
#endif
}

/****************************************************************************
 * Name: health_monitor_publish
 *
 * Description:
 *   Publish the heap root as a lockless sequence/count/tick snapshot.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock.
 *
 ****************************************************************************/

void health_monitor_publish(void)
{
	uint32_t sequence = g_health_sequence;
	clock_t next = g_health_count ? health_monitor_get_state(g_health_heap[0])->deadline : 0;

	/* Mark sequence odd to indicate an in-progress update. */
	g_health_sequence = sequence + 1;
#ifdef CONFIG_SMP
	SP_DMB();
#endif

	g_health_next_count = g_health_count;
	g_health_next_tick = next;
#ifdef CONFIG_SMP
	SP_DMB();
#endif

	/* Mark sequence even after writing the payload. */
	g_health_sequence = sequence + 2;
}

/****************************************************************************
 * Name: health_monitor_sift_up
 *
 * Description:
 *   Restore min-heap order by moving an entry toward the root.
 *
 * Inputs:
 *   index - Heap index to move toward the root.
 *   now   - Reference tick for comparison.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock.
 *
 ****************************************************************************/

void health_monitor_sift_up(uint32_t index, clock_t now)
{
	struct tcb_s *entry = g_health_heap[index];
	clock_t deadline = health_monitor_get_state(entry)->deadline;
	uint32_t parent;

	while (index > 0) {
		parent = (index - 1) / 2;
		if (!health_monitor_earlier(deadline, health_monitor_get_state(g_health_heap[parent])->deadline, now)) {
			break;
		}

		g_health_heap[index] = g_health_heap[parent];
		index = parent;
	}

	g_health_heap[index] = entry;
}

/****************************************************************************
 * Name: health_monitor_sift_down
 *
 * Description:
 *   Restore min-heap order by moving an entry toward the leaves.
 *
 * Inputs:
 *   index - Heap index to move toward the leaves.
 *   now   - Reference tick for comparison.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock.
 *
 ****************************************************************************/

void health_monitor_sift_down(uint32_t index, clock_t now)
{
	struct tcb_s *entry = g_health_heap[index];
	clock_t deadline = health_monitor_get_state(entry)->deadline;
	uint32_t child;

	while ((child = index * 2 + 1) < g_health_count) {
		if (child + 1 < g_health_count && health_monitor_earlier(health_monitor_get_state(g_health_heap[child + 1])->deadline,
			health_monitor_get_state(g_health_heap[child])->deadline, now)) {
			child++;
		}

		if (!health_monitor_earlier(health_monitor_get_state(g_health_heap[child])->deadline, deadline, now)) {
			break;
		}

		g_health_heap[index] = g_health_heap[child];
		index = child;
	}

	g_health_heap[index] = entry;
}
