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
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <debug.h>
#include <sys/prctl.h>
#include <preference/preference.h>

#include "preference_internal.h"

static int preference_get_dir_name(char* name)
{
	int ret;
	int ppid = -1;

	/* Get main Task ID in group */
	ret = prctl((int)PR_GET_TGTASK, &ppid);
	if (ret < 0 || ppid < 0) {
		prefdbg("Failed to get parent task %d\n", errno);
		return PREFERENCE_PATH_NOT_FOUND;
	}

	/* Get Task Name */
	ret = prctl((int)PR_GET_NAME, name, ppid, 0, 0);
	if (ret < 0) {
		prefdbg("Failed to get task name %d\n", errno);
		return PREFERENCE_PATH_NOT_FOUND;
	}

	return OK;
}

int preference_get_dirpath(char **path)

{
	int ret;
	char name[CONFIG_TASK_NAME_SIZE + 1];

	ret = preference_get_dir_name(name);
	if (ret < 0) {
		return ret;
	}

	/* Assign full path for app preference */
	ret = asprintf(path, "%s/%s", PREF_PATH, name);
	if (ret < 0) {
		prefdbg("Failed to allocate path\n");
		return PREFERENCE_OUT_OF_MEMORY;
	}
	prefvdbg("Preference path = %s\n", *path);

	return OK;

}

int preference_get_keypath(char *key, char **path)
{
	int ret;
	char name[CONFIG_TASK_NAME_SIZE + 1];

	ret = preference_get_dir_name(name);
	if (ret < 0) {
		return ret;
	}

	/* Assign full path for app preference */
	ret = asprintf(path, "%s/%s/%s", PREF_PATH, name, key);
	if (ret < 0) {
		prefdbg("Failed to allocate path\n");
		return PREFERENCE_OUT_OF_MEMORY;
	}
	prefvdbg("Preference key path = %s\n", *path);

	return OK;
}
