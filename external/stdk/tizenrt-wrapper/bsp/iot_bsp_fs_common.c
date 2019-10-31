/* ***************************************************************************
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "iot_bsp_fs.h"

/* Using romfs and smartfs as underlying storage
 * they are mounted during booting up*/
iot_error_t iot_bsp_fs_init()
{
	return IOT_ERROR_NONE;
}

iot_error_t iot_bsp_fs_deinit()
{
	return IOT_ERROR_NONE;
}

iot_error_t iot_bsp_fs_open(const char *filename, iot_bsp_fs_open_mode_t mode, iot_bsp_fs_handle_t *handle)
{
	mode_t fmode;

	if (mode == FS_READONLY) {
		fmode = O_RDONLY;
	} else {
		fmode = O_RDWR | O_CREAT;
	}
	
	int fd = open(filename, fmode);
	if (fd < 0) {
		return IOT_ERROR_FS_OPEN_FAIL;
	}
	
	handle->fd = fd;
	snprintf(handle->filename, sizeof(handle->filename), "%s", filename);
	return IOT_ERROR_NONE;
}

iot_error_t iot_bsp_fs_open_from_stnv(const char *filename, iot_bsp_fs_handle_t *handle)
{
	return iot_bsp_fs_open(filename, FS_READONLY, handle);
}

iot_error_t iot_bsp_fs_read(iot_bsp_fs_handle_t handle, char *buffer, unsigned int length)
{
	int ret;
	struct stat st;
	int filelen;

	int fd = handle.fd;
	if (fd < 0) {
		return IOT_ERROR_FS_NO_FILE;
	}

	fstat(fd, &st);
	filelen = st.st_size;
	ret = read(fd, buffer, filelen);
	buffer[filelen] = '\0';
	if (ret <= 0) {
		return IOT_ERROR_FS_READ_FAIL;
	}

	return IOT_ERROR_NONE;
}

iot_error_t iot_bsp_fs_write(iot_bsp_fs_handle_t handle, const char *data, unsigned int length)
{
	int ret;

	if (handle.fd < 0) {
		return IOT_ERROR_FS_NO_FILE;
	}

	/*write 'NULL' to file end*/
	ret = write(handle.fd, data, length+1);
	if (ret <=0) {
		return IOT_ERROR_FS_WRITE_FAIL;
	}

	return IOT_ERROR_NONE;
}

iot_error_t iot_bsp_fs_close(iot_bsp_fs_handle_t handle)
{
	int ret;
	if (handle.fd < 0) {
		return IOT_ERROR_FS_NO_FILE;
	}

	ret = close(handle.fd);
	if (ret == -1) {
		return IOT_ERROR_FS_CLOSE_FAIL;
	}

	return IOT_ERROR_NONE;
}

iot_error_t iot_bsp_fs_remove(const char *filename)
{
	int ret;
	if (filename == NULL) {
		return IOT_ERROR_INVALID_ARGS;
	}

	ret = unlink(filename);
	if (ret < 0) {
		return IOT_ERROR_FS_REMOVE_FAIL;
	}
	return IOT_ERROR_NONE;
}
