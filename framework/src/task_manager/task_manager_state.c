/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <sys/types.h>
#include <stdio.h>
#include <tinyara/fs/ioctl.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * taskmgr_update_task_state
 ****************************************************************************/
static void taskmgr_update_task_state(int handle)
{
	int ret;
	int fd;

	fd = taskmgr_get_drvfd();
	if (fd < 0) {
		return;
	}

	ret = ioctl(fd, TMIOC_CHECK_ALIVE, TASK_PID(handle));
	if (ret != OK && TASK_LIST_ADDR(handle) != NULL) {
		TASK_STATUS(handle) = TM_TASK_STATE_STOP;
	}
}

/****************************************************************************
 * taskmgr_update_task_state
 ****************************************************************************/
int taskmgr_get_task_state(int handle)
{
	if (IS_INVALID_HANDLE(handle) || TASK_LIST_ADDR(handle) == NULL) {
		return TM_TASK_STATE_UNREGISTERED;
	}

	taskmgr_update_task_state(handle);

	return (TASK_STATUS(handle));
}
