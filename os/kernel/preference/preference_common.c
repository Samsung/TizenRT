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
#if CONFIG_TASK_NAME_SIZE > 0
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/preference.h>

#include "sched/sched.h"
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/*
 * The private preference is a task-group shared resource: every thread of
 * a task must map to the same PREF_PRIVATE_PATH directory.  The group
 * identity is the name of its main task (the name given to task_create()),
 * but the NuttX 12.13.0 task group carries no name; group_create() keeps
 * the main task's pid in group->tg_pid instead, so resolve that TCB and
 * use its name.  The caller's own name (get_task_name()) must not be used:
 * pthread_setname_np() only rewrites the calling thread's name, and such a
 * renamed thread would silently move the group's private preferences into
 * a per-thread directory.  If the main task has already exited while the
 * group lives on, fall back to the caller's name as a best effort.
 */
const char *preference_group_name(FAR struct tcb_s *tcb)
{
	FAR struct tcb_s *maintcb = tcb;

	if (tcb->pid != tcb->group->tg_pid) {
		maintcb = nxsched_get_tcb(tcb->group->tg_pid);
		if (maintcb == NULL) {
			return get_task_name(tcb);
		}
	}

	return get_task_name(maintcb);
}

int preference_get_private_keypath(const char *key, char **path)
{
#if CONFIG_TASK_NAME_SIZE > 0
	int ret;
	struct tcb_s *tcb;

	/* Get path for app preference */
	tcb = this_task();
	if (!tcb->group) {
		prefdbg("Failed to get group\n");
		return PREFERENCE_OPERATION_FAIL;
	}
	/* Assign full path for app preference directory */
	ret = PREFERENCE_ASPRINTF(path, "%s/%s/%s", PREF_PRIVATE_PATH, preference_group_name(tcb), key);
	if (ret < 0) {
		prefdbg("Failed to allocate path\n");
		return PREFERENCE_OUT_OF_MEMORY;
	}
	prefvdbg("Preference key path = %s\n", *path);

	return OK;
#else
	prefdbg("Not supported private preference\n");
	return PREFERENCE_NOT_SUPPORTED;
#endif

}
