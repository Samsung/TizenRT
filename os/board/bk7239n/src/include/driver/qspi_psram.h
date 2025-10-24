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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the QSPI psram
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_QSPI_NOT_INIT: QSPI driver not init
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_init(qspi_id_t id);

/**
 * @brief     Deinit the QSPI psram
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_QSPI_NOT_INIT: QSPI driver not init
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_deinit(qspi_id_t id);

/**
 * @brief      QSPI psram enter quad mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_enter_quad_mode(qspi_id_t id);

/**
 * @brief      QSPI psram exit quad mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_exit_quad_mode(qspi_id_t id);

/**
 * @brief      QSPI psram init quad write
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_quad_write(qspi_id_t id);

/**
 * @brief      QSPI psram init single write
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_single_write(qspi_id_t id);

/**
 * @brief      QSPI psram init quad read
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_quad_read(qspi_id_t id);

/**
 * @brief      QSPI psram init single read
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_single_read(qspi_id_t id);

/**
 * @brief      QSPI psram write data
 *
 * @param base_addr QSPI write address
 * @param data QSPI write data
 * @param size the size of QSPI write data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_write(qspi_id_t id, uint32_t base_addr, const void *data, uint32_t size);


/**
 * @brief      QSPI psram read data
 *
 * @param base_addr QSPI read address
 * @param data QSPI read data
 * @param size the size of QSPI read data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_psram_read(qspi_id_t id, uint32_t base_addr, void *data, uint32_t size);

#ifdef __cplusplus
}
#endif

