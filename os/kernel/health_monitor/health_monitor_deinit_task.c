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
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_deinit_task
 *
 * Description:
 *   Handle monitoring before a supplied TCB is torn down or restarted.
 *   Report unexpected exit if CONFIG_HEALTH_MONITOR_CHECK_EXIT is enabled.
 *   Remove the entry and clear its state if cleanup continues.
 *   Stopped and partially created targets need no HM cleanup.
 *
 *   The caller must keep the TCB alive and prevent re-registration. Check
 *   registration under the HM lock, then release it before diagnostics.
 *   Earlier cleanup may have changed task state or removed stack mappings.
 *
 * Inputs:
 *   tcb - Target TCB. NULL is ignored.
 *
 * Return Value:
 *   None. A registered target enters PANIC when exit checking is enabled.
 *
 ****************************************************************************/

void health_monitor_deinit_task(struct tcb_s *tcb)
{
	struct health_monitor_s *state;
	uint32_t index;
	irqstate_t flags;
#ifdef CONFIG_HEALTH_MONITOR_CHECK_EXIT
	irqstate_t critical_flags;
#endif

	if (tcb == NULL) {
		return;
	}

	flags = spin_lock_irqsave(&g_health_lock);
	state = health_monitor_get_state(tcb);
	if (!HEALTH_MONITOR_IS_REGISTERED(state)) {
		spin_unlock_irqrestore(&g_health_lock, flags);
		return;
	}

#ifdef CONFIG_HEALTH_MONITOR_CHECK_EXIT
	spin_unlock_irqrestore(&g_health_lock, flags);
	critical_flags = enter_critical_section();
	health_monitor_fail_unexpected_exit(tcb);
	leave_critical_section(critical_flags);
	flags = spin_lock_irqsave(&g_health_lock);
#endif

	for (index = 0; index < g_health_count && g_health_heap[index] != tcb; index++) ;
	if (index < g_health_count) {
		health_monitor_remove(index, clock_systimer());
	}
	state->timeout = 0;
	state->deadline = 0;
	health_monitor_publish();
	spin_unlock_irqrestore(&g_health_lock, flags);
}
