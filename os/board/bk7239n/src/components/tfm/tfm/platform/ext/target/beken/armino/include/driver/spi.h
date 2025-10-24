// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <driver/spi_types.h>
#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the SPI driver
 *
 * This API init the resoure common:
 *   - Init SPI driver control memory
 *
 * @attention 1. This API should be called before any other SPI APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_driver_init(void);

/**
 * @brief     Deinit the SPI driver
 *
 * This API free all resource related to SPI and disable SPI.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_driver_deinit(void);

/**
 * @brief     Init the SPI id
 *
 * This API init the SPI id:
 *  - Power up the SPI id
 *  - Configure the SPI id clock
 *  - Map the SPI id to dedicated GPIO port
 *  - Set the SPI parameters
 *  - Start the SPI id
 *
 * @param id SPI id
 * @param config SPI parameter settings
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: SPI config paramter is NULL
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id is invalid
 *    - others: other errors.
 */
bk_err_t bk_spi_init(spi_id_t id, const spi_config_t *config);

/**
 * @brief     Deinit the SPI id
 *
 * This API deinit the SPI id:
 *   - Stop the SPI id
 *   - Disable the SPI id interrupt
 *   - Power down the SPI id
 *
 * @param id SPI id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_deinit(spi_id_t id);

/**
 * @brief     Set the SPI mode
 *
 * @param id SPI id
 * @param mode SPI mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_set_mode(spi_id_t id, spi_mode_t mode);

/**
 * @brief     Set the SPI bit width
 *
 * @param id SPI id
 * @param bit_width SPI bit width
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_set_bit_width(spi_id_t id, spi_bit_width_t bit_width);

/**
 * @brief     Set the SPI wire mode
 *
 * @param id SPI id
 * @param wire_mode SPI wire mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_set_wire_mode(spi_id_t id, spi_wire_mode_t wire_mode);

/**
 * @brief     Set the SPI baud rate
 *
 * @param id SPI id
 * @param baud_rate SPI baud rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_set_baud_rate(spi_id_t id, uint32_t baud_rate);

/**
 * @brief     Set the SPI bit order
 *
 * @param id SPI id
 * @param bit_order SPI bit order
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_set_bit_order(spi_id_t id, spi_bit_order_t bit_order);

/**
 * @brief     Register the RX interrupt service routine for SPI id
 *
 * @param id spi id
 * @param isr SPI RX callback
 * @param param SPI RX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_register_rx_isr(spi_id_t id, spi_isr_t isr, void *param);

/**
 * @brief     Register the TX finish interrupt service routine for SPI id
 *
 * @param id SPi id
 * @param isr SPI TX finish callback
 * @param param SPi TX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_register_tx_finish_isr(spi_id_t id, spi_isr_t isr, void *param);

/**
 * @brief     Send data to the SPI port from a given buffer and length
 *
 * @param id spi id
 * @param data data buffer address
 * @param size data length to send
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - others: other errors.
 */
bk_err_t bk_spi_write_bytes(spi_id_t id, const void *data, uint32_t size);

/**
 * @brief     SPI read bytes from SPI buffer
 *
 * @param id SPI id
 * @param data pointer to the buffer
 * @param size data length to read
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - others: other errors.
 */
bk_err_t bk_spi_read_bytes(spi_id_t id, void *data, uint32_t size);

/**
 * @brief     SPI write and read bytes
 *
 * @param id SPI id
 * @param tx_data write data buffer address
 * @param tx_size data length to read
 * @param rx_data pointer to the receive buffer
 * @param rx_size data length to read
 * @param timeout_ms timeout ms, if set BEKEN_WAIT_FOREVER, read will wait forever
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - others: other errors.
 */
bk_err_t bk_spi_transmit(spi_id_t id, const void *tx_data, uint32_t tx_size, void *rx_data, uint32_t rx_size);


/**
 * @brief     Register the RX finish interrupt service routine for SPI id
 *
 * @param id spi id
 * @param isr SPI RX callback
 * @param param SPI RX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_register_rx_finish_isr(spi_id_t id, spi_isr_t isr, void *param);

/**
 * @brief     Unregister the RX interrupt service routine for SPI id
 *
 * @param id spi id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_unregister_rx_isr(spi_id_t id);

/**
 * @brief     Unregister the RX finish interrupt service routine for SPI id
 *
 * @param id spi id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_unregister_rx_finish_isr(spi_id_t id);

/**
 * @brief     Unregister the TX finish interrupt service routine for SPI id
 *
 * @param id spi id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_unregister_tx_finish_isr(spi_id_t id);


/**
 * @brief     Send data to the SPI port from a given buffer and length in async mode
 *
 * @param id spi id
 * @param data data buffer address
 * @param size data length to send
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - others: other errors.
 */
bk_err_t bk_spi_write_bytes_async(spi_id_t id, const void *data, uint32_t size);

/**
 * @brief     SPI read bytes from SPI buffer in async mode
 *
 * @param id SPI id
 * @param data pointer to the buffer
 * @param size data length to read
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - others: other errors.
 */
bk_err_t bk_spi_read_bytes_async(spi_id_t id, void *data, uint32_t size);




#if CONFIG_SPI_DMA

/**
 * @brief     Send data to the SPI port by dma from a given buffer and length
 *
 * @param id spi id
 * @param data data buffer address
 * @param size data length to send
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_spi_dma_write_bytes(spi_id_t id, const void *data, uint32_t size);

/**
 * @brief     SPI read bytes by dma from SPI buffer
 *
 * @param id SPI id
 * @param data pointer to the buffer
 * @param size data length to read
 * @param timeout_ms timeout ms, if set BEKEN_WAIT_FOREVER, read will wait forever
 *
 * @return
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - others: other errors.
 */
bk_err_t bk_spi_dma_read_bytes(spi_id_t id, void *data, uint32_t size);

/**
 * @brief     SPI write and read bytes byte dma
 *
 * @param id SPI id
 * @param tx_data write data buffer address
 * @param tx_size data length to read
 * @param rx_data pointer to the receive buffer
 * @param rx_size data length to read
 * @param timeout_ms timeout ms, if set BEKEN_WAIT_FOREVER, read will wait forever
 *
 * @return
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - others: other errors.
 */
bk_err_t bk_spi_dma_transmit(spi_id_t id, const void *tx_data, uint32_t tx_size, void *rx_data, uint32_t rx_size);

/**
 * @brief     SPI init duplex transmit
 *
 * @param id SPI id
 *
 *
 * @return
 *    - BK_OK: success
 *    - others: other errors.
 */
bk_err_t bk_spi_dma_duplex_init(spi_id_t id);

/**
 * @brief     SPI deinit duplex transmit
 *
 * @param id SPI id
 *
 *
 * @return
 *    - BK_OK: success
 *    - others: other errors.
 */
bk_err_t bk_spi_dma_duplex_deinit(spi_id_t id);

/**
 * @brief     SPI write and read bytes byte dma
 *
 * @param id SPI id
 * @param tx_data write data buffer address
 * @param tx_size data length to read
 * @param rx_data pointer to the receive buffer
 * @param rx_size data length to read
 *
 * @return
 *    - BK_ERR_SPI_NOT_INIT: SPI driver not init
 *    - BK_ERR_SPI_INVALID_ID: SPI id number is invalid
 *    - BK_ERR_SPI_ID_NOT_INIT: SPI id not init
 *    - BK_ERR_SPI_DUPLEX_SIZE_NOT_EQUAL: tx rx data size not equal
 *    - others: other errors.
 */
bk_err_t bk_spi_dma_duplex_xfer(spi_id_t id, const void *tx_data, uint32_t tx_size, void *rx_data, uint32_t rx_size);

#endif
#ifdef __cplusplus
}
#endif

