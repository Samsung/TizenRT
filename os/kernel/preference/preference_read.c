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
#include <unistd.h>
#include <debug.h>
#include <fcntl.h>
#include <errno.h>
#include <tinyara/preference.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int preference_read_fs_key(char *path, preference_data_t *data)
{
	int fd;
	int ret;
	int read_size;
	value_data_t value_data;

	fd = open(path, O_RDONLY, 0666);
	PREFERENCE_FREE(path);

	if (fd < 0) {
		if (errno == ENOENT) {
			return PREFERENCE_KEY_NOT_EXIST;
		}
		return PREFERENCE_IO_ERROR;
	}

	/* Read and Verify the type and len of data value */
	read_size = 2 * sizeof(int);
	ret = read(fd, (void *)&value_data, read_size);
	if (ret != read_size) {
		ret = PREFERENCE_IO_ERROR;
		goto errout;
	} else if (value_data.type < 0 || value_data.len < 0) {
		prefdbg("Invalid data %d\n", value_data.len);
		ret = PREFERENCE_OPERATION_FAIL;
		goto errout;
	} else if (value_data.type != data->value.type) {
		prefdbg("Invalid type. request type:%d, read type:%d\n", data->value.type, value_data.type);
		ret = PREFERENCE_INVALID_PARAMETER;
		goto errout;
	}
	data->value.len = value_data.len;

	/* Read the data value */
	switch (data->value.type) {
	case PREFERENCE_TYPE_INT:
	case PREFERENCE_TYPE_BOOL:
		ret = read(fd, (void *)&(data->value.i), data->value.len);
		prefvdbg("[READ] int type %d\n", data->value.i);
		break;
	case PREFERENCE_TYPE_DOUBLE:
		ret = read(fd, (void *)&(data->value.d), data->value.len);
		prefvdbg("[READ] double type %f\n", data->value.d);
		break;
	case PREFERENCE_TYPE_STRING:
		data->value.s = (char *)PREFERENCE_ALLOC(data->value.len);
		if (data->value.s == NULL) {
			ret = PREFERENCE_OUT_OF_MEMORY;
			goto errout;
		}
		ret = read(fd, data->value.s, data->value.len);
		prefvdbg("[READ] string type %s\n", data->value.s);
		if (ret != data->value.len) {
			PREFERENCE_FREE(data->value.s);
			data->value.s = NULL;
		}
		break;
	default:
		prefdbg("Invalid type %d\n");
		ret = PREFERENCE_INVALID_PARAMETER;
	}

	if (ret == data->value.len) {
		prefvdbg("Read key Success!\n");
		ret = OK;
	} else {
		ret = PREFERENCE_IO_ERROR;
	}
errout:	
	close(fd);

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int preference_read_key(preference_data_t *data)
{
	int ret;
	char *path;

	if (data == NULL || data->key == NULL || (data->type != PRIVATE_PREFERENCE && data->type != SHARED_PREFERENCE)) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	if (data->type == PRIVATE_PREFERENCE) {
		ret = preference_get_private_keypath(data->key, &path);
		if (ret < 0) {
			prefdbg("Failed to get preference path\n");
			return ret;
		}
	} else {
		ret = PREFERENCE_ASPRINTF(&path, "%s/%s", PREF_SHARED_PATH, data->key);
		if (ret < 0) {
			prefdbg("Failed to allocate path\n");
			return PREFERENCE_OUT_OF_MEMORY;
		}
	}

	return preference_read_fs_key(path, data);
}
