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
#include <unistd.h>
#include <debug.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <tinyara/preference.h>
#if CONFIG_TASK_NAME_SIZE > 0
#include <sys/types.h>
#include <tinyara/sched.h>

#include "sched/sched.h"
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int preference_remove_fs_key(char *path)
{
	int ret;

	if (path == NULL) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	/* Remove key file */
	ret = unlink(path);
	if (ret < 0) {
		if (errno == ENOENT) {
			ret = PREFERENCE_KEY_NOT_EXIST;
			prefdbg("key file is not exist : %s\n", path);
		} else {
			ret = PREFERENCE_IO_ERROR;
			prefdbg("Failed to remove key file %s\n", path);
		}
	} else {
		ret = OK;
		prefvdbg("Removed key file %s\n", path);
	}
	PREFERENCE_FREE(path);

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int preference_remove_key(int type, const char *key)
{
	int ret;
	char *path;

	if (key == NULL || (type != PRIVATE_PREFERENCE && type != SHARED_PREFERENCE)) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	if (type == PRIVATE_PREFERENCE) {
		ret = preference_get_private_keypath(key, &path);
		if (ret < 0) {
			prefdbg("Failed to get preference path\n");
			return ret;
		}
	} else {
		ret = PREFERENCE_ASPRINTF(&path, "%s/%s", PREF_SHARED_PATH, key);
		if (ret < 0) {
			prefdbg("Failed to allocate path\n");
			return PREFERENCE_OUT_OF_MEMORY;
		}
	}

	return preference_remove_fs_key(path);
}

int preference_remove_all_key(int type, const char *path)
{
	int ret;
	DIR *dir;
	char *dir_path;
	char *key_path;
	struct dirent *entry;
#if CONFIG_TASK_NAME_SIZE > 0
	struct tcb_s *tcb;
#endif

	if ((type != PRIVATE_PREFERENCE && type != SHARED_PREFERENCE) || (type == SHARED_PREFERENCE && path == NULL)) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	if (type == PRIVATE_PREFERENCE) {
#if CONFIG_TASK_NAME_SIZE > 0
		tcb = this_task();
		if (!tcb->group) {
			prefdbg("Failed to get group\n");
			return PREFERENCE_OPERATION_FAIL;
		}

		/* Assign full path for app preference directory */
		ret = PREFERENCE_ASPRINTF(&dir_path, "%s/%s", PREF_PRIVATE_PATH, tcb->group->tg_name);
#else
		prefdbg("Not supported private preference\n");
		return PREFERENCE_NOT_SUPPORTED;
#endif
	} else {
		/* Get path for shared preference */
		ret = PREFERENCE_ASPRINTF(&dir_path, "%s/%s", PREF_SHARED_PATH, path);
	}
	if (ret < 0) {
		prefdbg("Failed to allocate path\n");
		return PREFERENCE_OUT_OF_MEMORY;
	}

	prefvdbg("preference dir path = %s\n", dir_path);

	dir = (DIR *)opendir(dir_path);
	if (!dir) {
		prefdbg("Failed to open dir %s, %d\n", dir_path, errno);
		if (errno == ENOENT) {
			ret = PREFERENCE_PATH_NOT_FOUND;
		} else {
			ret = PREFERENCE_IO_ERROR;
		}
		goto errout_with_free;
	}

	while (1) {
		entry = (struct dirent *)readdir(dir);
		if (!entry) {
			break;
		}
		ret = PREFERENCE_ASPRINTF(&key_path, "%s/%s", dir_path, entry->d_name);
		if (ret < 0) {
			ret = PREFERENCE_OUT_OF_MEMORY;
			break;
		}
		/* Remove key file */
		prefvdbg("Remove key file : %s\n", key_path);
		ret = preference_remove_fs_key(key_path);
		if (ret < 0) {
			prefdbg("Failed to remove key file %d\n", ret);
			break;
		}
	}
	closedir(dir);

	if (ret < 0) {
		goto errout_with_free;
	}

	ret = OK;

errout_with_free:
	PREFERENCE_FREE(dir_path);

	return ret;
}
