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
#include <stdbool.h>
#include <apps/builtin.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * taskmgr_is_permitted
 ****************************************************************************/
bool taskmgr_is_permitted(int handle, pid_t pid)
{
	int chk_idx;
	int builtin_cnt;
	int permission = TM_PERMISSION(handle);

	builtin_cnt = get_builtin_list_cnt();

	if (permission == TM_APP_PERMISSION_ALL \
		|| (permission == TM_APP_PERMISSION_DEDICATE && TM_GID(handle) == pid)) {
		return true;
	}

	if (permission == TM_APP_PERMISSION_GROUP) {
		if (pid == TM_GID(handle)) {
			return true;
		}
		/* check pid's group id */
		for (chk_idx = 0; chk_idx < builtin_cnt; chk_idx++) {
			if (TM_PID(chk_idx) == pid && TM_GID(chk_idx) == TM_GID(handle)) {
				return true;
			}
		}
	}

	return false;
}
