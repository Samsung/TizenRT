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

#include <common/bk_include.h>
#include <driver/sdio_host_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the sdio host driver
 *
 * This API init the resoure common:
 *   - Init sdio host driver control memory
 *
 * @attention 1. This API should be called before any other sdio host APIs
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_driver_init(void);

/**
 * @brief     Deinit the sdio host driver
 *
 * This API free all resource related to sdio host and disable sdio host
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_driver_deinit(void);

/**
 * @brief     Init the sdio host
 *
 * This API init the sdio host:
 *  - Power up the sdio
 *  - Configure the sdio clock soure to 26M
 *  - Enable sdio host interrupt
 *  - Map the sdio to dedicated GPIO port
 *  - Set the clock frequency and bus width
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_init(const sdio_host_config_t *config);

/**
 * @brief     Deinit the sdio host
 *
 * This API deinit the sdio host:
 *   - Reset all configuration of sdio host to default value
 *   - Clear sdio host interrupt status
 *   - Power down the sdio host
 *   - Disable sdio host interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_deinit(void);

/**
 * @brief     Set the sdio host clock frequency
 *
 * @param clock_freq clock frequency
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_set_clock_freq(sdio_host_clock_freq_t clock_freq);

/**
 * @brief     Send the command from host to slave
 *
 * @param command the configuration information for the command
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - BK_ERR_NULL_PARAM: command is NULL
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_send_command(const sdio_host_cmd_cfg_t *command);

/**
 * @brief     Host wait for salve command response
 *
 * @param cmd_index the command index
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - BK_ERR_SDIO_HOST_CMD_RSP_TIMEOUT: sdio host wait slave command over time
 *    - BK_ERR_SDIO_HOST_CMD_RSP_CRC_FAIL: sdio host wait slave command crc fail
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_wait_cmd_response(uint32_t cmd_index);

/**
 * @brief     Get the command response
 *
 * @param argument_index the command argument index
 *
 * @return the response register value
 */
uint32_t bk_sdio_host_get_cmd_rsp_argument(sdio_host_response_t argument_index);

/**
 * @brief     Config the data
 *
 * @param data_config the configuration information for the data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - BK_ERR_NULL_PARAM: data_config is NULL
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_config_data(const sdio_host_data_config_t *data_config);

/**
 * @brief     Set the data timeout
 *
 * @param timeout the data timeout
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_set_data_timeout(uint32_t timeout);

/**
 * @brief     Write data to tx fifo
 *
 * @param write_data pointer to data to write
 * @param data_size size of data to be sent, the data_size value should be aligned with 512 bytes.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - BK_ERR_NULL_PARAM: write_data is NULL
 *    - BK_ERR_SDIO_HOST_DATA_TIMEOUT: sdio host wait slave data timeout
 *    - BK_ERR_SDIO_HOST_DATA_CRC_FAIL: sdio host receive slave data crc fail
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_write_fifo(const uint8_t *write_data, uint32_t data_size);

/**
 * @brief     Host wait slave data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - BK_ERR_SDIO_HOST_DATA_TIMEOUT: sdio host wait slave data timeout
 *    - BK_ERR_SDIO_HOST_DATA_CRC_FAIL: sdio host receive slave data crc fail
 *    - others: other errors.
 */
bk_err_t bk_sdio_host_wait_receive_data(void);

/**
 * @brief     Read data from rx fifo with timeout mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_READ_DATA_FAIL: sdio host read data from FIFO timeout
 *    - *save_v_p:if success, will save read value to this pointer.
 */
bk_err_t bk_sdio_host_read_fifo(uint32_t *save_v_p);

/**
 * @brief     Read many blocks data from rx fifo with timeout mode.
 *
 * @param blk_cnt:read how many blocks data out from SDIO FIFO.
 *                  the block_size is 512 bytes.
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_READ_DATA_FAIL: sdio host read data from FIFO timeout
 *    - *read_data:if success, will save read value to this pointer.
 */
bk_err_t bk_sdio_host_read_blks_fifo(uint8_t *read_data, uint32_t blk_cnt);

#ifdef __cplusplus
}
#endif

