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
#include <crc32.h>
#include <tinyara/preference.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int preference_read_fs_key(char *path, preference_data_t *data)
{
	int fd;
	int ret;
	uint32_t check_crc;
	value_attr_t attr;

	fd = open(path, O_RDONLY, 0666);
	PREFERENCE_FREE(path);

	if (fd < 0) {
		if (errno == ENOENT) {
			return PREFERENCE_KEY_NOT_EXIST;
		}
		return PREFERENCE_IO_ERROR;
	}

	/* Read and Verify attributes of data : crc, type, len */
	ret = read(fd, (FAR uint8_t *)&attr, sizeof(value_attr_t));
	if (ret != sizeof(value_attr_t)) {
		prefdbg("Failed to read attribute, errno %d\n", errno);
		ret = PREFERENCE_IO_ERROR;
		goto errout;
	} else if (attr.type < 0 || attr.len < 0) {
		prefdbg("Invalid data : type %d, len %d\n", attr.type, attr.len);
		ret = PREFERENCE_OPERATION_FAIL;
		goto errout;
	} else if (attr.type != data->attr.type) {
		prefdbg("Invalid type. request type:%d, read type:%d\n", data->attr.type, attr.type);
		ret = PREFERENCE_INVALID_PARAMETER;
		goto errout;
	}
	data->attr.len = attr.len;
	data->value = PREFERENCE_ALLOC(data->attr.len);
	if (data->value == NULL) {
		ret = PREFERENCE_OUT_OF_MEMORY;
		goto errout;
	}

	/* Read value data */
	ret = read(fd, (void *)data->value, data->attr.len);
	if (ret != data->attr.len) {
		prefdbg("Failed to read key value, errno %d\n", errno);
		ret = PREFERENCE_IO_ERROR;
		goto errout_with_free;
	}

	/* Calculate and Verify the checksum */
	check_crc = crc32((uint8_t *)&data->attr.type, sizeof(value_attr_t) - sizeof(uint32_t));
	check_crc = crc32part((uint8_t *)data->value, data->attr.len, check_crc);
	if (check_crc != attr.crc) {
		prefdbg("Invalid checksum, read crc : %u, calculated crc : %u\n", attr.crc, check_crc);
		ret = PREFERENCE_INVALID_DATA;
		goto errout_with_free;
	}

	prefvdbg("Read key Success!\n");
	close(fd);

	return OK;
errout_with_free:
	PREFERENCE_FREE(data->value);
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
