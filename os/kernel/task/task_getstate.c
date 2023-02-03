/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

/************************************************************************
 * Included Files
 ************************************************************************/

#include <errno.h>
#include <sched.h>

/************************************************************************
 * Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Global Variables
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/************************************************************************
 * Private Function Prototypes
 ************************************************************************/

/************************************************************************
 * Private Functionss
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name:  task_getstate
 *
 * Description:
 *   Get the task state associated with the pid.
 *
 * Inputs
 *   pid - the task ID of the task to query.
 *
 * Return Value:
 *   On success, task_getstate() returns task state. On error, less than
 *   zero is returned.
 *
 *   ESRCH - The task whose ID is pid could not be found.
 *
 ************************************************************************/

uint8_t task_getstate(pid_t pid)
{
	struct tcb_s *tcb;

	/* Get the TCB associated with the pid */
	tcb = sched_gettcb(pid);

	if (!tcb) {
		/* No task with this pid was found */
		return -ESRCH;
	}

	return tcb->task_state;
}
