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
#include <tinyara/spi/spi.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_spi.h>

#define _IOTBUS_SPI_MAX_FREQUENCY 12000000 //12Mhz

struct _iotbus_spi_s {
	unsigned int bpw;
	int lsb;
	int freq; // clock speed
	int cs;
	iotbus_spi_mode_e mode;
#ifdef CONFIG_SPI
	struct spi_dev_s *sdev;
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_SPI

/**
 * Private Functions
 */

/**
 * Public Functions
 */

iotbus_spi_context_h iotbus_spi_open(unsigned int bus, const struct iotbus_spi_config_s *config)
{
	if (!config)
		return NULL;

	if (config->bits_per_word != 8)
		return NULL;
	if (config->frequency == 0 || config->frequency > _IOTBUS_SPI_MAX_FREQUENCY)
		return NULL;
	if (config->lsb != 0 && config->lsb != 1)
		return NULL;
	switch (config->mode) {
	case IOTBUS_SPI_MODE0:
	case IOTBUS_SPI_MODE1:
	case IOTBUS_SPI_MODE2:
	case IOTBUS_SPI_MODE3:
		break;
	default:
		return NULL;
	}

	struct spi_dev_s *dev = up_spiinitialize(bus);
	if (!dev)
		return NULL;

	struct _iotbus_spi_s *handle = (struct _iotbus_spi_s *)malloc(sizeof(struct _iotbus_spi_s));
	handle->bpw = config->bits_per_word;
	handle->lsb = config->lsb;
	handle->freq = config->frequency;
	handle->cs = config->chip_select;
	handle->mode = config->mode;

	handle->sdev = dev;

	SPI_LOCK(dev, true);
	SPI_SETMODE(dev, handle->mode);

	if (handle->lsb == 0) // MSB
		SPI_SETBITS(dev, handle->bpw);
	else
		// LSB
		SPI_SETBITS(dev, -handle->bpw);

	SPI_SETFREQUENCY(dev, handle->freq);
	SPI_LOCK(dev, false);
	return handle;
}

int iotbus_spi_write(iotbus_spi_context_h hnd, uint8_t *txbuf, size_t length)
{
	if (!hnd)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	if (!txbuf || length < 0)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	struct spi_dev_s *dev = (struct spi_dev_s *)hnd->sdev;
	SPI_LOCK(dev, true);
	SPI_SELECT(dev, hnd->cs, true);
	SPI_SNDBLOCK(dev, txbuf, length); // return type is void
	SPI_SELECT(dev, hnd->cs, false);
	SPI_LOCK(dev, false);
	return 0;
}

int iotbus_spi_recv(iotbus_spi_context_h hnd, uint8_t *rxbuf, size_t length)
{

	if (!hnd)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	if (!rxbuf || length < 0)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	struct spi_dev_s *dev = (struct spi_dev_s *)hnd->sdev;
	SPI_LOCK(dev, true);
	SPI_SELECT(dev, hnd->cs, true);
	SPI_RECVBLOCK(dev, rxbuf, length); // return type is void
	SPI_SELECT(dev, hnd->cs, false);
	SPI_LOCK(dev, false);
	return 0;
}

int iotbus_spi_transfer_buf(iotbus_spi_context_h hnd, uint8_t *txbuf, uint8_t *rxbuf, size_t length)
{

	if (!hnd)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	if (!txbuf || !rxbuf || length < 0)
		return IOTBUS_ERROR_INVALID_PARAMETER;

#ifdef CONFIG_SPI_EXCHANGE
	struct spi_dev_s *dev = (struct spi_dev_s *)hnd->sdev;
	SPI_LOCK(dev, true);
	SPI_SELECT(dev, hnd->cs, true);
	SPI_EXCHANGE(dev, txbuf, rxbuf, length);
	SPI_SELECT(dev, hnd->cs, false);
	SPI_LOCK(dev, false);
	return 0;
#else
	return IOTBUS_ERROR_NOT_SUPPORTED;
#endif
}

int iotbus_spi_close(iotbus_spi_context_h hnd)
{
	if (!hnd)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	free(hnd);

	return 0;
}
#else // CONFIG_SPI
iotbus_spi_context_h iotbus_spi_open(unsigned int bus,
				const struct iotbus_spi_config_s *config)
{
	return NULL;
}
int iotbus_spi_write(iotbus_spi_context_h hnd, uint8_t *txbuf, size_t length)
{
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int iotbus_spi_recv(iotbus_spi_context_h hnd, uint8_t *rxbuf, size_t length)
{
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int iotbus_spi_transfer_buf(iotbus_spi_context_h hnd, uint8_t *txbuf,
				uint8_t *rxbuf, size_t length)
{
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int iotbus_spi_close(iotbus_spi_context_h hnd)
{
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
#endif     // CONFIG_SPI

#ifdef __cplusplus
}
#endif
