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
#include <unistd.h>
#include <debug.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <debug.h>
#include <preference/preference.h>

#include "preference_internal.h"

static int preference_setup(void)
{
	int ret;
	char *path;

	/* Get path for app preference */
	ret = preference_get_dirpath(&path);
	if (ret < 0) {
		prefdbg("Failed to get preference dir path\n");
		return ret;
	}

	/* Make app preference directory */
	ret = mkdir(path, 0777);
	free(path);

	if (ret < 0) {
		if (errno != EEXIST) {
			prefdbg("mkdir fail, %d\n", errno);
			return PREFERENCE_IO_ERROR;
		}
	}

	return OK;
}

static int preference_fs_write(char *key, preference_data_t *data)
{
	int fd;
	int ret;
	char *path;

	if (key == NULL || data == NULL) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	ret = preference_setup();
	if (ret < 0) {
		prefdbg("Failed to make preference directory\n");
		return ret;
	}

	ret = preference_get_keypath(key, &path);
	if (ret < 0) {
		prefdbg("Failed to get preference path\n");
		return ret;
	}

	fd = open(path, O_WRONLY | O_CREAT, 0666);
	free(path);

	if (fd < 0) {
		prefdbg("open fail %d\n", errno);
		return PREFERENCE_IO_ERROR;
	}

	/* Write the type of data value */
	ret = write(fd, (void *)&(data->type), sizeof(int));
	if (ret < 0) {
		ret = PREFERENCE_IO_ERROR;
		goto errout;
	}

	/* Write the length of data value */
	ret = write(fd, (void *)&(data->len), sizeof(int));
	if (ret < 0) {
		ret = PREFERENCE_IO_ERROR;
		goto errout;
	}

	/* Write the data value */
	switch (data->type) {
	case PREFERENCE_TYPE_INT:
	case PREFERENCE_TYPE_BOOL:
		prefvdbg("[WRITE] int type %d\n", data->value.i);
		ret = write(fd, (void *)&(data->value.i), data->len);
		break;
	case PREFERENCE_TYPE_DOUBLE:
		prefvdbg("[WRITE] double type %f\n", data->value.d);
		ret = write(fd, (void *)&(data->value.d), data->len);
		break;
	case PREFERENCE_TYPE_STRING:
		prefvdbg("[WRITE] string type %s\n", data->value.s);
		ret = write(fd, (void *)data->value.s, data->len);
		free(data->value.s);
		break;
	default:
		prefdbg("invalid type %d\n", data->type);
		ret = PREFERENCE_INVALID_PARAMETER;
		goto errout;
	}

	if (ret < 0) {
		ret = PREFERENCE_IO_ERROR;
		goto errout;
	}
	close(fd);
	prefvdbg("Write Key %s Preference Success.\n", key);

	return OK;
errout:
	close(fd);
	unlink(fd);

	return ret;
}

int preference_set_int(char *key, int value)
{
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.type = PREFERENCE_TYPE_INT;
	data.len = sizeof(int);
	data.value.i = value;

	return preference_fs_write(key, &data);
}

int preference_set_double(char *key, double value)
{
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.type = PREFERENCE_TYPE_DOUBLE;
	data.len = sizeof(double);
	data.value.d = value;

	return preference_fs_write(key, &data);
}

int preference_set_bool(char *key, bool value)
{
	preference_data_t data;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.type = PREFERENCE_TYPE_BOOL;
	data.len = sizeof(int);
	if (value) {
		data.value.i = 1;
	} else {
		data.value.i = 0;
	}

	return preference_fs_write(key, &data);
}

int preference_set_string(char *key, char *value)
{
	preference_data_t data;

	if (key == NULL || value == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	data.type = PREFERENCE_TYPE_STRING;
	data.len = strlen(value) + 1;
	if (data.len < 2) {
		return PREFERENCE_INVALID_PARAMETER;
	}
	data.value.s = (char *)malloc(data.len);
	strncpy(data.value.s, value, data.len);

	return preference_fs_write(key, &data);
}



int preference_system_set_string
