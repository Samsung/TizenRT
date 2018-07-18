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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <preference/preference.h>

#include "preference_internal.h"

static int preference_fs_read(char *key, preference_data_t *data)
{
	int ret;
	int type;
	char *path;

	if (key == NULL || data == NULL) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	ret = preference_get_keypath(key, &path);
	if (ret < 0) {
		prefdbg("Failed to get preference path\n");
		return ret;
	}

	int fd = open(path, O_RDONLY, 0666);
	free(path);

	if (fd < 0) {
		if (errno == ENOENT) {
			return PREFERENCE_INVALID_PARAMETER;
		}
		return PREFERENCE_IO_ERROR;
	}

	/* Read the type of data value and Compare to the type of request */
	ret = read(fd, (void *)&type, sizeof(int));
	if (ret < 0) {
		ret = PREFERENCE_IO_ERROR;
		goto errout;
	} else if (type < 0 || data->type < 0 || data->type != type) {
		prefdbg("Invalid type. request type:%d, read type:%d\n", type, data->type);
		ret = PREFERENCE_INVALID_PARAMETER;
		goto errout;
	}

	/* Read the len of data value */
	ret = read(fd, (void *)&(data->len), sizeof(int));
	if (ret < 0 || data->len < 0) {
		ret = PREFERENCE_IO_ERROR;
		goto errout;
	}

	/* Read the data value */
	switch (data->type) {
	case PREFERENCE_TYPE_INT:
	case PREFERENCE_TYPE_BOOL:
		ret = read(fd, (void *)&(data->value.i), data->len);
		prefvdbg("[READ] int type %d\n", data->value.i);
		break;
	case PREFERENCE_TYPE_DOUBLE:
		ret = read(fd, (void *)&(data->value.d), data->len);
		prefvdbg("[READ] double type %f\n", data->value.d);
		break;
	case PREFERENCE_TYPE_STRING:
		data->value.s = (char *)malloc(data->len);
		ret = read(fd, data->value.s, data->len);
		prefvdbg("[READ] string type %s\n", data->value.s);
		if (ret < 0) {
			free(data->value.s);
		}
		break;
	default:
		prefdbg("Invalid type %d\n");
		ret = PREFERENCE_INVALID_PARAMETER;
	}

	if (ret > 0) {
		prefvdbg("Write key %s Success\n", key);
		ret = OK;
	}
errout:	
	close(fd);

	return ret;
}

int preference_get_int(char *key, int *value)
{
	int ret;
	preference_data_t result;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	result.type = PREFERENCE_TYPE_INT;
	ret = preference_fs_read(key, &result);

	if (ret == OK) {
		*value = result.value.i;
	}

	return ret;
}

int preference_get_bool(char *key, bool *value)
{
	int ret;
	preference_data_t result;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	result.type = PREFERENCE_TYPE_BOOL;
	ret = preference_fs_read(key, &result);

	if (ret == OK) {
		if (result.value.i == 0) {
			*value = false;
		} else if (result.value.i == 1) {
			*value = true;
		}
	}

	return ret;
}

int preference_get_double(char *key, double *value)
{
	int ret;
	preference_data_t result;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	result.type = PREFERENCE_TYPE_DOUBLE;
	ret = preference_fs_read(key, &result);

	if (ret == OK) {
		*value = result.value.d;
	}

	return ret;
}

int preference_get_string(char *key, char **value)
{
	int ret;
	preference_data_t result;

	if (key == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	result.type = PREFERENCE_TYPE_STRING;
	ret = preference_fs_read(key, &result);

	if (ret == OK) {
		*value = result.value.s;
	}

	return ret;
}
