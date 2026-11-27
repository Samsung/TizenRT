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
#include <errno.h>
#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include "sched/sched.h"
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_kick
 *
 * Description:
 *   Renew the calling thread's deadline and restore heap order immediately.
 *   Report an error if the calling thread is not registered in the heap.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   OK if the deadline was renewed
 *   -EPERM if called from interrupt context
 *   -ENOENT if not registered in the heap.
 *
 * Assumptions:
 *   Called from thread context.
 *
 ****************************************************************************/

int health_monitor_kick(void)
{
	struct health_monitor_s *state;
	struct tcb_s *tcb;
	irqstate_t flags;
	uint32_t index;
	clock_t now;
	int ret = -ENOENT;

	if (up_interrupt_context()) {
		return -EPERM;
	}

	flags = spin_lock_irqsave(&g_health_lock);

	tcb = this_task();
	state = health_monitor_get_state(tcb);
	if (HEALTH_MONITOR_IS_REGISTERED(state)) {
		for (index = 0; index < g_health_count; index++) {
			if (g_health_heap[index] == tcb) {
				now = clock_systimer();
				state->deadline = now + state->timeout;
				health_monitor_sift_down(index, now);
				health_monitor_publish();
				ret = OK;
				break;
			}
		}
	}

	spin_unlock_irqrestore(&g_health_lock, flags);
	return ret;
}
