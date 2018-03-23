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
#include <stdlib.h>
#include <sys/types.h>
#include <tinyara/spi/spi.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_spi.h>

#define _IOTBUS_SPI_MAX_FREQUENCY 12000000 //12Mhz

struct _iotbus_spi_s {
	unsigned int bpw;
	int freq; // clock speed
	int cs;
	iotbus_spi_mode_e mode;
	struct spi_dev_s *sdev;
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
iotbus_spi_context_h iotbus_spi_open(unsigned int bus, const struct iotbus_spi_config_s *config)
{
	struct spi_dev_s *spi_dev;
	struct _iotbus_spi_s *handle;
	iotbus_spi_context_h dev;

	if (!config) {
		return NULL;
	}
	if (config->bits_per_word != 8) {
		return NULL;
	}
	if (config->frequency == 0 || config->frequency > _IOTBUS_SPI_MAX_FREQUENCY) {
		return NULL;
	}

	switch (config->mode) {
	case IOTBUS_SPI_MODE0:
	case IOTBUS_SPI_MODE1:
	case IOTBUS_SPI_MODE2:
	case IOTBUS_SPI_MODE3:
		break;
	default:
		return NULL;
	}

	handle = (struct _iotbus_spi_s *)malloc(sizeof(struct _iotbus_spi_s));
	if (!handle) {
		return NULL;
	}

	dev = (struct _iotbus_spi_wrapper_s *)malloc(sizeof(struct _iotbus_spi_wrapper_s));
	if (!dev) {
		free(handle);
		return NULL;
	}

	spi_dev = up_spiinitialize(bus);
	if (!spi_dev) {
		free(handle);
		free(dev);
		return NULL;
	}

	handle->bpw = config->bits_per_word;
	handle->freq = config->frequency;
	handle->cs = config->chip_select;
	handle->mode = config->mode;

	SPI_LOCK(spi_dev, true);
	SPI_SETMODE(spi_dev, handle->mode);
	SPI_SETBITS(spi_dev, handle->bpw);
	SPI_SETFREQUENCY(spi_dev, handle->freq);
	SPI_LOCK(spi_dev, false);

	handle->sdev = spi_dev;
	dev->handle = handle;

	return dev;
}

int iotbus_spi_write(iotbus_spi_context_h hnd, uint8_t *txbuf, size_t length)
{
	struct spi_dev_s *dev;
	struct _iotbus_spi_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!txbuf || length < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_spi_s *)hnd->handle;
	dev = (struct spi_dev_s *)handle->sdev;
	SPI_LOCK(dev, true);
	SPI_SELECT(dev, handle->cs, true);
	SPI_SNDBLOCK(dev, txbuf, length); // return type is void
	SPI_SELECT(dev, handle->cs, false);
	SPI_LOCK(dev, false);

	return IOTBUS_ERROR_NONE;
}

int iotbus_spi_recv(iotbus_spi_context_h hnd, uint8_t *rxbuf, size_t length)
{
	struct spi_dev_s *dev;
	struct _iotbus_spi_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!rxbuf || length < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_spi_s *)hnd->handle;
	dev = (struct spi_dev_s *)handle->sdev;
	SPI_LOCK(dev, true);
	SPI_SELECT(dev, handle->cs, true);
	SPI_RECVBLOCK(dev, rxbuf, length); // return type is void
	SPI_SELECT(dev, handle->cs, false);
	SPI_LOCK(dev, false);

	return IOTBUS_ERROR_NONE;
}

#ifdef CONFIG_SPI_EXCHANGE
int iotbus_spi_transfer_buf(iotbus_spi_context_h hnd, uint8_t *txbuf, uint8_t *rxbuf, size_t length)
{
	struct spi_dev_s *dev;
	struct _iotbus_spi_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!txbuf || !rxbuf || length < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_spi_s *)hnd->handle;
	dev = (struct spi_dev_s *)handle->sdev;
	SPI_LOCK(dev, true);
	SPI_SELECT(dev, handle->cs, true);
	SPI_EXCHANGE(dev, txbuf, rxbuf, length);
	SPI_SELECT(dev, handle->cs, false);
	SPI_LOCK(dev, false);
	return 0;
}
#endif

int iotbus_spi_close(iotbus_spi_context_h hnd)
{
	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	free(hnd->handle);
	hnd->handle = NULL;
	free(hnd);

	return IOTBUS_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif
