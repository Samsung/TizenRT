/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/**
 * @file iotbus_i2c.c
 * @brief Iotbus APIs for I2C
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <tinyara/i2c.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_i2c.h>

struct _iotbus_i2c_s {
	int fd;
};

struct _iotbus_i2c_wrapper_s {
	struct _iotbus_i2c_s *handle;
};

#ifdef __cplusplus
extern "C" {
#endif

iotbus_i2c_context_h iotbus_i2c_init(int bus)
{
	int fd;
	char dev_path[16] = { 0, };
	struct _iotbus_i2c_s *handle;
	iotbus_i2c_context_h dev;

	snprintf(dev_path, sizeof(dev_path), "/dev/i2c-%d", bus);
	fd = open(dev_path, O_RDWR);
	if (fd < 0) {
		return NULL;
	}

	handle = (struct _iotbus_i2c_s *)malloc(sizeof(struct _iotbus_i2c_s));
	if (!handle) {
		goto errout_with_close;
	}

	dev = (struct _iotbus_i2c_wrapper_s *)malloc(sizeof(struct _iotbus_i2c_wrapper_s));
	if (!dev) {
		free(handle);
		goto errout_with_close;
	}

	handle->fd = fd;
	dev->handle = handle;

	return dev;

errout_with_close:
	close(fd);
	return NULL;
}

int iotbus_i2c_stop(iotbus_i2c_context_h hnd)
{
	struct _iotbus_i2c_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_i2c_s *)hnd->handle;

	close(handle->fd);
	free(handle);
	hnd->handle = NULL;
	free(hnd);

	return IOTBUS_ERROR_NONE;
}

int iotbus_i2c_set_frequency(iotbus_i2c_context_h hnd, iotbus_i2c_mode_e mode)
{
	int ret = -1;
	uint32_t frequency = 0;
	struct _iotbus_i2c_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_i2c_s *)hnd->handle;

	switch (mode) {
	case IOTBUS_I2C_STD:
		frequency = 100000;
		break;
	case IOTBUS_I2C_FAST:
		frequency = 400000;
		break;
	case IOTBUS_I2C_HIGH:
		frequency = 3400000;
		break;
	default:
		return IOTBUS_ERROR_NOT_SUPPORTED;
	}

	ret = ioctl(handle->fd, I2C_FREQUENCY, (unsigned long)((uintptr_t)&frequency));
	if (ret < 0) {
		return IOTBUS_ERROR_NOT_SUPPORTED;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_i2c_set_address(iotbus_i2c_context_h hnd, uint8_t address)
{
	int ret = -1;
	struct _iotbus_i2c_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	/* check if address length is 7-bit */
	if ((address < 0x01) || (address > 0x7F)) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_i2c_s *)hnd->handle;
	ret = ioctl(handle->fd, I2C_SLAVE, (unsigned long)((uintptr_t)&address));
	if (ret < 0) {
		return IOTBUS_ERROR_NOT_SUPPORTED;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_i2c_read(iotbus_i2c_context_h hnd, uint8_t *data, size_t length)
{
	int ret = -1;
	struct _iotbus_i2c_s *handle;

	if (!hnd || !hnd->handle || !data) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_i2c_s *)hnd->handle;
	ret = read(handle->fd, data, length);

	return ret;
}

int iotbus_i2c_write(iotbus_i2c_context_h hnd, const uint8_t *data, size_t length)
{
	int ret = -1;
	struct _iotbus_i2c_s *handle;

	if (!hnd || !hnd->handle || !data) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_i2c_s *)hnd->handle;

	ret = write(handle->fd, data, length);

	return ret;
}

#ifdef __cplusplus
}
#endif
