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

#include <driver/lcd_types.h>
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Send the lcd qspi cmd and cmd value
 *
 * This API is used to write qspi lcd register
 *
 * @param qspi_id the qspi device id number
 * @param write_cmd the writing cmd of lcd qspi device
 * @param cmd the register to be set
 * @param data the register parameter to be send
 * @param data_len the length of data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lcd_qspi_send_cmd(qspi_id_t qspi_id, uint8_t write_cmd, uint8_t cmd, const uint8_t *data, uint8_t data_len);

/**
 * @brief     Start sending the qspi data with 4 wire
 *
 * This API is used to start sending the qspi data
 *
 * @param qspi_id the qspi device id number
 * @param reg_config the struct of qspi configuration
 * @param addr_is_4wire determine whether the address is four wires or one wire
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lcd_qspi_quad_write_start(qspi_id_t qspi_id, lcd_qspi_write_config_t reg_config, bool addr_is_4wire);

/**
 * @brief     Stop sending the qspi data with 4 wire
 *
 * This API is used to stop sending the qspi data
 *
 * @param qspi_id the qspi device id number
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lcd_qspi_quad_write_stop(qspi_id_t qspi_id);

/**
 * @brief     Init the lcd qspi
 *
 * This API init the lcd qspi module:
 *  - reset the qspi hardware
 *  - init the qspi driver
 *  - set the qspi clock
 *  - init the dma2d driver
 *  - enable the qspi quad write
 *  - init the qspi lcd device
 * 
 * @param qspi_id the qspi device id number
 * @param device the struct of lcd device
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lcd_qspi_init(qspi_id_t qspi_id, const lcd_device_t *device);

/**
 * @brief     Deinit the lcd qspi
 *
 * This API deinit the lcd qspi module:
 *  - deinit the qspi driver
 *  - deinit the dma2d driver
 *  - close the qspi lcd device
 * 
 * @param qspi_id the qspi device id number
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lcd_qspi_deinit(qspi_id_t qspi_id);

/**
 * @brief     Read the lcd qspi register value 
 *
 * This API read the lcd qspi register value
 * 
 * @param qspi_id the qspi device id number
 * @param data the value of the read register
 * @param device the struct of lcd device
 * @param regist_addr the address of lcd device register
 * @param data_len the length of the lcd device register paramter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_lcd_qspi_read_data(qspi_id_t qspi_id, uint8_t *data, const lcd_device_t *device, uint8_t regist_addr, uint8_t data_len);

/**
 * @brief     Send a frame data to device display
 * 
 * @param qspi_id the qspi device id number
 * @param device the struct of lcd device
 * @param data the data to send
 * @param data_len the length of data to send
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_lcd_qspi_send_data(qspi_id_t qspi_id, const lcd_device_t *device, uint32_t *data, uint32_t data_len);

/**
 * @brief     Open the qspi lcd device
 *
 * @param qspi_id the qspi device id number
 * @param device the struct of lcd device
 *
 * @return
 *    - None
 */
void bk_lcd_qspi_disp_open(qspi_id_t qspi_id, const lcd_device_t *device);

/**
 * @brief     Close the qspi lcd device
 *
 * @param qspi_id the qspi device id number
 *
 * @return
 *    - None
 */
void bk_lcd_qspi_disp_close(qspi_id_t qspi_id);


#ifdef __cplusplus
}
#endif

