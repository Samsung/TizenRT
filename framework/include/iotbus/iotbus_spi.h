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
 * @defgroup SPI SPI
 * @brief Provides APIs for SPI (Serial Peripheral Interface)
 * @ingroup IOTBUS
 * @{
 */

/**
 * @file iotbus_spi.h
 * @brief Iotbus APIs for SPI
 */

#include <stdint.h>

#ifndef IOTBUS_SPI_H_
#define IOTBUS_SPI_H_

/**
 * @brief Enumeration of SPI mode
 * @details
 * Enumeration Details:\n
 * IOTBUS_SPI_MODE0\n
 * IOTBUS_SPI_MODE1\n
 * IOTBUS_SPI_MODE2\n
 * IOTBUS_SPI_MODE3\n
 */
typedef enum {
	IOTBUS_SPI_MODE0 = 0, IOTBUS_SPI_MODE1, IOTBUS_SPI_MODE2, IOTBUS_SPI_MODE3,
} iotbus_spi_mode_e;

/**
 * @brief Structure of SPI configuration
 */
struct iotbus_spi_config_s {
	char bits_per_word;
	int lsb; // 0 is MSB, 1 is LSB
	unsigned int chip_select;
	unsigned int frequency; // frequency in Hz
	iotbus_spi_mode_e mode;
};

struct _iotbus_spi_s;

/**
 * @brief Pointer definition to the internal struct _iotbus_spi_s
 */
typedef struct _iotbus_spi_s *iotbus_spi_context_h;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief initializes spi_context.
 *
 * @param[in] bus spi bus number
 * @param[in] config spi config
 *                   - config->bits_per_word : bits per word
 *                   - config->lsb : 0 is MSB, 1 is LSB
 *                   - config->chip_select : chip select number
 *                   - config->frequency : frequency in Hz
 *                   - config->mode : spi mode
 * @return On success, handle of spi_context is returned. On failure, NULL is returned.
 * @since Tizen RT v1.0
 */
iotbus_spi_context_h iotbus_spi_open(unsigned int bus, const struct iotbus_spi_config_s *config);

/**
 * @brief writes data over spi bus.
 *
 * @param[in] hnd handle of spi_context
 * @param[in] txbuf the pointer of tx data buffer
 * @param[in] length size to write
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_spi_write(iotbus_spi_context_h hnd, uint8_t *txbuf, size_t length);

/**
 * @brief reads data over spi bus.
 *
 * @param[in] hnd handle of spi_context
 * @param[in] rxbuf the pointer of rx data buffer
 * @param[in] length size to read
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_spi_recv(iotbus_spi_context_h hnd, uint8_t *rxbuf, size_t length);

/**
 * @brief transfers rx and tx data over spi bus.
 *
 * @param[in] hnd handle of spi_context
 * @param[in] txbuf the pointer of tx data buffer
 * @param[in] rxbuf the pointer of rx data buffer
 * @param[in] length size to transfer
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_spi_transfer_buf(iotbus_spi_context_h hnd, uint8_t *txbuf, uint8_t *rxbuf, size_t length);

/**
 * @brief closes spi_context.
 *
 * @param[in] hnd handle of spi_context
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_spi_close(iotbus_spi_context_h hnd);

#ifdef __cplusplus
}
#endif

#endif /* IOTBUS_SPI_H_ */

/** @} */ // end of SPI group
