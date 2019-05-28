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
 * @file iotbus_spi.c
 * @brief Iotbus APIs for SPI
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <tinyara/spi/spi.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_spi.h>
#include "iotbus_internal.h"

#define _IOTBUS_SPI_MAX_FREQUENCY 12000000 //12Mhz

struct _iotbus_spi_s {
	int fd;
	struct spi_io_config config;
};

struct _iotbus_spi_wrapper_s {
	struct _iotbus_spi_s *handle;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Public Functions
 */
iotbus_spi_context_h iotbus_spi_open(unsigned int bus, const struct iotbus_spi_config_s *user_config)
{
	struct _iotbus_spi_s *handle;
	iotbus_spi_context_h dev;

	/*  validate input parameters */
	if (!user_config) {
		return NULL;
	}
	if (user_config->bits_per_word != 8) {
		return NULL;
	}
	if (user_config->frequency == 0 || user_config->frequency > _IOTBUS_SPI_MAX_FREQUENCY) {
		return NULL;
	}

	switch (user_config->mode) {
	case IOTBUS_SPI_MODE0:
	case IOTBUS_SPI_MODE1:
	case IOTBUS_SPI_MODE2:
	case IOTBUS_SPI_MODE3:
		break;
	default:
		return NULL;
	}

	/*	Open driver */
	char spi_dev[16] = { 0, };
	snprintf(spi_dev, 16, "/dev/spi-%d", bus);
	int fd = open(spi_dev, O_RDWR);
	if (fd < 0) {
		ibdbg("open %s failed: %d\n", spi_dev, errno);
		return NULL;
	}

	struct spi_io_config config = {user_config->bits_per_word,
								   user_config->frequency,
								   user_config->chip_select,
								   user_config->mode};
	int res = ioctl(fd, SPIIOC_SET_CONFIG, (unsigned long)((uintptr_t)&config));
	if (res < 0) {
		ibdbg("set config fail\n");
		close(fd);
		return NULL;
	}

	/*  create handle */
	handle = (struct _iotbus_spi_s *)malloc(sizeof(struct _iotbus_spi_s));
	if (!handle) {
		close(fd);
		return NULL;
	}

	dev = (struct _iotbus_spi_wrapper_s *)malloc(sizeof(struct _iotbus_spi_wrapper_s));
	if (!dev) {
		close(fd);
		free(handle);
		return NULL;
	}

	handle->fd = fd;
	handle->config = config;

	dev->handle = handle;

	return dev;
}

int iotbus_spi_set_config(iotbus_spi_context_h hnd, const struct iotbus_spi_config_s *config)
{
	struct _iotbus_spi_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_spi_s *)hnd->handle;

	struct spi_io_config conf = { config->bits_per_word, config->frequency, config->chip_select, config->mode };
	int res = ioctl(handle->fd, SPIIOC_SET_CONFIG, (unsigned long)((uintptr_t)&conf));
	if (res < 0) {
		ibdbg("set config fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_spi_write(iotbus_spi_context_h hnd, uint8_t *txbuf, size_t length)
{
	struct _iotbus_spi_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!txbuf || length < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_spi_s *)hnd->handle;

	int res = write(handle->fd, txbuf, length);
	if (res < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_spi_recv(iotbus_spi_context_h hnd, uint8_t *rxbuf, size_t length)
{
	struct _iotbus_spi_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!rxbuf || length < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_spi_s *)hnd->handle;

	int res = read(handle->fd, rxbuf, length);
	if (res < 0) {
		return IOTBUS_ERROR_NONE;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_spi_transfer_buf(iotbus_spi_context_h hnd, uint8_t *txbuf, uint8_t *rxbuf, size_t length)
{
	struct _iotbus_spi_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!txbuf || !rxbuf || length < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_spi_s *)hnd->handle;

	struct spi_io_msg msg = {rxbuf, txbuf, length};
	int res = ioctl(handle->fd, SPIIOC_EXCHANGE, (unsigned long)((uintptr_t)&msg));
	if (res < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return 0;
}

int iotbus_spi_close(iotbus_spi_context_h hnd)
{
	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}
	struct _iotbus_spi_s *handle = (struct _iotbus_spi_s *)hnd->handle;
	close(handle->fd);
	free(hnd->handle);
	hnd->handle = NULL;
	free(hnd);

	return IOTBUS_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif
