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
#include <stdbool.h>
#include <debug.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <tinyara/preference.h>
#include <tinyara/common_logs/common_logs.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int preference_check_fs_key(char *path, bool *existing)
{
	int ret;
	struct stat st;

	if (path == NULL) {
		prefdbg("%s \n", clog_message_str[CMN_LOG_INVALID_VAL]);
		return PREFERENCE_INVALID_PARAMETER;
	}

	*existing = false;

	ret = stat(path, &st);
	PREFERENCE_FREE(path);

	if (ret == OK && S_ISREG(st.st_mode)) {
		*existing = true;
	} else if (errno != ENOENT) {
		return PREFERENCE_IO_ERROR;
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int preference_check_key(int type, const char *key, bool *result)
{
	int ret;
	char *path;

	if (key == NULL || (type != PRIVATE_PREFERENCE && type != SHARED_PREFERENCE)) {
		prefdbg("%s \n",clog_message_str[CMN_LOG_INVALID_VAL]);
		return PREFERENCE_INVALID_PARAMETER;
	}

	if (type == PRIVATE_PREFERENCE) {
		ret = preference_get_private_keypath(key, &path);
		if (ret < 0) {
			prefdbg("%s get preference path \n", clog_message_str[CMN_LOG_FAILED_OP]);
			return ret;
		}
	} else {
		ret = PREFERENCE_ASPRINTF(&path, "%s/%s", PREF_SHARED_PATH, key);
		if (ret < 0) {
			prefdbg("%s allocate path \n", clog_message_str[CMN_LOG_FAILED_OP]);
			return PREFERENCE_OUT_OF_MEMORY;
		}
	}

	return preference_check_fs_key(path, result);
}
