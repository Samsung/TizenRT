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
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_init_task
 *
 * Description:
 *   Initialize a newly created TCB to the unregistered state. Does not start
 *   monitoring.
 *
 * Inputs:
 *   tcb - TCB of the newly created task or pthread.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Called during task creation before activation. Only for new, unregistered
 *   TCBs.
 *
 ****************************************************************************/

void health_monitor_init_task(struct tcb_s *tcb)
{
	struct health_monitor_s *state;

	state = health_monitor_get_state(tcb);
	state->deadline = 0;
	state->timeout = 0;
}
