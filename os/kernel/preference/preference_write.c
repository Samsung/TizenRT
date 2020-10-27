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
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <crc32.h>
#include <sys/stat.h>
#include <tinyara/preference.h>
#if CONFIG_TASK_NAME_SIZE > 0
#include <tinyara/sched.h>

#include "sched/sched.h"
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#if CONFIG_TASK_NAME_SIZE > 0
static int preference_private_setup(void)
{
	int ret;
	char *path;
	struct tcb_s *tcb;

	tcb = this_task();
	if (!tcb->group) {
		prefdbg("Failed to get group\n");
		return PREFERENCE_OPERATION_FAIL;
	}

	/* Assign full path for app preference directory */
	ret = PREFERENCE_ASPRINTF(&path, "%s/%s", PREF_PRIVATE_PATH, tcb->group->tg_name);

	/* Make app preference directory */
	ret = mkdir(path, 0777);
	PREFERENCE_FREE(path);
	if (ret < 0 && errno != EEXIST) {
		prefdbg("mkdir fail, %d\n", errno);
		return PREFERENCE_IO_ERROR;
	}

	return OK;
}
#endif
static int preference_shared_setup(char *key_path)
{
	int ret;
	int prepath_len;
	int index;
	char *dir_path;
	char *ptr;

	/* Allocate meomory for full path of key */
	prepath_len = strlen(PREF_SHARED_PATH) + 1;
	dir_path = (char *)PREFERENCE_ALLOC(prepath_len + strlen(key_path));
	if (dir_path == NULL) {
		return PREFERENCE_OUT_OF_MEMORY;
	}
	memset(dir_path, 0, prepath_len +  strlen(key_path));

	index = snprintf(dir_path, prepath_len + 1, "%s/", PREF_SHARED_PATH);
	if (index != prepath_len) {
		PREFERENCE_FREE(dir_path);
		return PREFERENCE_IO_ERROR;
	}

	/* Search and make ancestor directories in sequence.
	 * [TODO] will make one file instead of multiple directories by convering the path internally. */
	ptr = key_path;
	while (ptr != NULL && *ptr != '\0') {
		if (*ptr == '/') {
			prefvdbg("dir path = %s\n", dir_path);
			ret = mkdir(dir_path, 0777);
			if (ret < 0 && errno != EEXIST) {
				prefdbg("mkdir fail, %d\n", errno);
				PREFERENCE_FREE(dir_path);
				return PREFERENCE_IO_ERROR;
			}
		}
		dir_path[index++] = *ptr++;
	}
	PREFERENCE_FREE(dir_path);

	return OK;
}

static int preference_write_fs_key(char *path, preference_data_t *data)
{
	int fd;
	int ret;
	uint32_t crc_value;

	fd = open(path, O_WRONLY | O_CREAT, 0666);
	if (fd < 0) {
		prefdbg("open fail %d\n", errno);
		PREFERENCE_FREE(path);
		return PREFERENCE_IO_ERROR;
	}

	/* Calculate checksum of attributes, type, len and value */
	crc_value = crc32((uint8_t *)&data->attr.type, sizeof(value_attr_t) - sizeof(uint32_t));
	data->attr.crc = crc32part((uint8_t *)data->value, data->attr.len, crc_value);

	/* Write attributes of data : crc, type, len */
	ret = write(fd, (void *)&data->attr, sizeof(value_attr_t));
	if (ret != sizeof(value_attr_t)) {
		prefdbg("Failed to write key value, errno %d\n", errno);
		goto errout_with_close;
	}

	/* Write value data */
	ret = write(fd, (void *)data->value, data->attr.len);
	if (ret != data->attr.len) {
		prefdbg("Failed to write key value, errno %d\n", errno);
		goto errout_with_close;
	}

	close(fd);
	prefvdbg("Write Key Success : %s, len = %d\n", path, data->attr.len);
	PREFERENCE_FREE(path);

	return OK;
errout_with_close:
	close(fd);
	unlink(path);
	PREFERENCE_FREE(path);

	return PREFERENCE_IO_ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int preference_write_key(preference_data_t *data)
{
	int ret;
	char *path;

	if (data == NULL || data->key == NULL || (data->type != PRIVATE_PREFERENCE && data->type != SHARED_PREFERENCE)) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	if (data->type == PRIVATE_PREFERENCE) {
#if CONFIG_TASK_NAME_SIZE > 0
		ret = preference_private_setup();
		if (ret < 0) {
			prefdbg("Failed to set up preference\n");
			return ret;
		}
		ret = preference_get_private_keypath(data->key, &path);
		if (ret < 0) {
			prefdbg("Failed to get preference path\n");
			return ret;
		}
#else
		prefdbg("Not supported private preference\n");
		return PREFERENCE_NOT_SUPPORTED;
#endif
	} else {
		ret = preference_shared_setup(data->key);
		if (ret < 0) {
			prefdbg("Failed to set up preference\n");
			return ret;
		}
		ret = PREFERENCE_ASPRINTF(&path, "%s/%s", PREF_SHARED_PATH, data->key);
		if (ret < 0) {
			prefdbg("Failed to allocate path\n");
			return PREFERENCE_OUT_OF_MEMORY;
		}
	}
	prefvdbg("Preference key path = %s\n", path);

	ret = preference_write_fs_key(path, data);
#if !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_SIGNAL)
	if (ret == OK) {
		/* Execute callback if registered cb is existing */
		preference_send_cb_msg(data->type, data->key);
	}
#endif

	return ret;
}
