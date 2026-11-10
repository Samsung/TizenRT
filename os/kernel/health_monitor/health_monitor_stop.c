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
#include <assert.h>
#include <errno.h>
#include <tinyara/clock.h>
#include "sched/sched.h"
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_remove
 *
 * Description:
 *   Remove an entry from the heap and restore min-heap order.
 *
 * Inputs:
 *   index - Heap slot to remove.
 *   now   - Reference tick for comparison.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock, index < g_health_count.
 *
 ****************************************************************************/

static void health_monitor_remove(uint32_t index, clock_t now)
{
	g_health_count--;
	if (index < g_health_count) {
		g_health_heap[index] = g_health_heap[g_health_count];
		if (index > 0 && health_monitor_earlier(health_monitor_get_state(g_health_heap[index])->deadline,
			health_monitor_get_state(g_health_heap[(index - 1) / 2])->deadline, now)) {
			health_monitor_sift_up(index, now);
		} else {
			health_monitor_sift_down(index, now);
		}
	}

	g_health_heap[g_health_count] = NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_stop
 *
 * Description:
 *   Unregister the calling thread from health monitoring.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   OK if registered and removed; -ENOENT if not registered.
 *
 * Assumptions:
 *   Called from thread context.
 *
 ****************************************************************************/

int health_monitor_stop(void)
{
	struct health_monitor_s *state;
	struct tcb_s *tcb;
	irqstate_t flags;
	uint32_t index;
	int ret = OK;

	flags = spin_lock_irqsave(&g_health_lock);

	tcb = this_task();
	state = health_monitor_get_state(tcb);
	if (state->timeout == 0) {
		ret = -ENOENT;
	} else {
		for (index = 0; index < g_health_count; index++) {
			if (g_health_heap[index] == tcb) {
				break;
			}
		}

		DEBUGASSERT(index < g_health_count);
		if (index < g_health_count) {
			health_monitor_remove(index, clock_systimer());
		}

		state->timeout = 0;
		state->deadline = 0;
		health_monitor_publish();
	}

	spin_unlock_irqrestore(&g_health_lock, flags);
	return ret;
}
