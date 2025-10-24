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

#include <driver/qspi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the QSPI driver
 *
 * This API init the resoure common:
 *   - Init QSPI driver control memory
 *
 * @attention 1. This API should be called before any other QSPI APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_driver_init(void);

/**
 * @brief     Deinit the QSPI driver
 *
 * This API free all resource related to QSPI and disable QSPI.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_driver_deinit(void);

/**
 * @brief     Init the QSPI
 *
 * This API init the QSPI:
 *  - Power up the QSPI
 *  - Configure the QSPI clock
 *  - Map the QSPI to dedicated GPIO port
 *  - Enabel QSPI interrupt
 *
 * @param config QSPI parameter settings
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_QSPI_NOT_INIT: QSPI driver not init
 *    - BK_ERR_NULL_PARAM: QSPI config parameter is NULL
 *    - others: other errors.
 */
bk_err_t bk_qspi_init(qspi_id_t id, const qspi_config_t *config);

/**
 * @brief     Deinit the QSPI
 *
 * This API deinit the QSPI:
 *   - Disable the QSPI interrupt
 *   - Power down the QSPI
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_deinit(qspi_id_t id);

/**
 * @brief     Init the QSPI command
 *
 * @param cmd QSPI device command
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_command(qspi_id_t id, const qspi_cmd_t *cmd);

/**
 * @brief     QSPI indirect mode write
 *
 * @param base_addr QSPI write address
 * @param data QSPI write data
 * @param size the size of QSPI write data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_write(qspi_id_t id, const void *data, uint32_t size);

/**
 * @brief     QSPI indirect mode read
 *
 * @param base_addr QSPI read address
 * @param data QSPI read data
 * @param size the size of QSPI read data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_read(qspi_id_t id, void *data, uint32_t size);

/**
 * @brief     Register the TX interrupt service routine for QSPI
 *
 * @param isr QSPI TX callback
 * @param param QSPI TX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_register_tx_isr(qspi_isr_t isr, void *param);

/**
 * @brief     Register the RX interrupt service routine for QSPI
 *
 * @param isr QSPI RX callback
 * @param param QSPI RX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_register_rx_isr(qspi_isr_t isr, void *param);

#ifdef __cplusplus
}
#endif
