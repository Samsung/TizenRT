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
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <debug.h>
#include <preference/preference.h>

#include "preference_internal.h"

int preference_remove(char *key)
{
	int ret;
	char *path;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	ret = preference_get_keypath(key, &path);
	if (ret < 0) {
		prefdbg("Failed to get preference path\n");
		return PREFERENCE_PATH_NOT_FOUND;
	}

	/* Remove key file */
	unlink(path);
	prefvdbg("Remove key file %s\n", path);

	free(path);

	return OK;
}

int preference_remove_all(void)
{
	int ret;
	DIR *dir;
	char *path;
	char *key_path;
	struct dirent *entry;

	/* Get path for app preference */
	ret = preference_get_dirpath(&path);
	if (ret < 0) {
		prefdbg("Failed to get preference dir path\n");
		return ret;
	}

	dir = (DIR *)opendir(path);
	if (!dir) {
		prefdbg("Failed to open dir %s, %d\n", path, errno);
		free(path);
		return PREFERENCE_IO_ERROR;
	}

	while (1) {
		entry = (struct dirent *)readdir(dir);
		if (!entry) {
			break;
		}
		ret = asprintf(&key_path, "%s/%s", path, entry->d_name);
		if (ret < 0) {
			closedir(dir);
			free(path);
			return PREFERENCE_OUT_OF_MEMORY;
		}
		/* Remove key file */
		prefvdbg("Remove key file %s\n", key_path);
		unlink(key_path);
		free(key_path);
	}
	closedir(dir);

	/* Remove app directory */
	prefvdbg("Remove preference dir %s\n", path);
	ret = rmdir(path);
	if (ret < 0) {
		prefdbg("Failed to remove dir %s, %d\n", path, errno);
		return PREFERENCE_IO_ERROR;
	}
	free(path);

	return OK;
}
