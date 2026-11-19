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
#include <stdbool.h>
#include <tinyara/clock.h>
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_deinit_task
 *
 * Description:
 *   Require STOP before a supplied TCB is torn down or restarted. A registered
 *   target is fatal; stopped and partially created targets need no HM cleanup.
 *   Lifecycle callbacks may STOP before reaching this common boundary.
 *
 *   The caller must keep the TCB alive and prevent re-registration. Capture
 *   the active verdict under the HM lock, then release it before diagnostics.
 *   Earlier cleanup may have changed task state or removed stack mappings.
 *
 * Inputs:
 *   tcb - Target TCB. NULL is ignored.
 *
 * Return Value:
 *   None. An active target is diagnosed and enters PANIC.
 *
 ****************************************************************************/

void health_monitor_deinit_task(struct tcb_s *tcb)
{
	struct health_monitor_failure_s failure;
	irqstate_t flags;
	bool active;

	if (tcb == NULL) {
		return;
	}

	flags = spin_lock_irqsave(&g_health_lock);
	active = health_monitor_get_state(tcb)->timeout != 0;
	if (active) {
		failure.type = HEALTH_MONITOR_FAILURE_UNEXPECTED_EXIT;
		failure.detected_at = clock_systimer();
		health_monitor_snapshot(tcb, &failure.task);
	}
	spin_unlock_irqrestore(&g_health_lock, flags);
	if (!active) {
		return;
	}

	health_monitor_fail(&failure);
}
