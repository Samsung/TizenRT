/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/preference.h>

#include "sched/sched.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int preference_get_private_keypath(const char *key, char **path)
{
	int ret;
#ifdef CONFIG_APP_BINARY_SEPARATION
	pid_t pid;
	struct tcb_s *tcb;
#endif

	/* Get path for app preference */
#ifdef CONFIG_APP_BINARY_SEPARATION
	tcb = this_task();
	pid = tcb->group->tg_loadtask;
	if (pid > 0) {
		tcb = sched_gettcb(pid);
		if (tcb == NULL) {
			prefdbg("Failed to get main task %d\n", pid);
			return PREFERENCE_OPERATION_FAIL;
		}
	}
	/* Assign full path for app preference*/
	ret = PREFERENCE_ASPRINTF(path, "%s/%s/%s", PREF_PRIVATE_PATH, tcb->name, key);
#else
	ret = PREFERENCE_ASPRINTF(path, "%s/%s", PREF_PRIVATE_PATH, key);
#endif
	if (ret < 0) {
		prefdbg("Failed to allocate path\n");
		return PREFERENCE_OUT_OF_MEMORY;
	}
	prefvdbg("Preference key path = %s\n", *path);

	return OK;
}
