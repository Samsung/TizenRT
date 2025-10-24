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

#include <driver/lcd_qspi_types.h>
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Get lcd qspi device struct by device name
 * 
 * @param name the name of lcd qspi device
 *
 * @return
 *     - lcd qspi device struct: succeed
 *     - NULL: not find device.
 */
lcd_qspi_device_t *bk_lcd_qspi_get_device_by_name(char *name);

/**
 * @brief     Get lcd qspi device struct by device name
 * 
 * @param ppi the ppi of lcd qspi device
 *
 * @return
 *     - lcd qspi device struct: succeed
 *     - NULL: not find device.
 */
lcd_qspi_device_t *bk_lcd_qspi_get_device_by_ppi(media_ppi_t ppi);

/**
 * @brief     Init the lcd qspi
 *
 * This API init the lcd qspi module:
 *  - reset the qspi hardware
 *  - init the qspi driver
 *  - init the psram
 *  - init the dma2d driver
 *  - enable quad write
 *  - open the backlight of lcd qspi 
 * 
 * @param device the struct of lcd qspi device
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lcd_qspi_init(const lcd_qspi_device_t *device);

/**
 * @brief     Deinit the lcd qspi
 *
 * This API deinit the lcd qspi module:
 *  - deinit the psram
 *  - deinit the dma2d driver
 *  - close the backlight of lcd qspi
 * 
 * @param device the struct of lcd qspi device
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lcd_qspi_deinit(lcd_qspi_device_t *device);

/**
 * @brief     Send a frame data to device display
 * 
 * @param device the struct of lcd qspi device
 * @param data the data to send
 * @param data_len the length of data to send
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_lcd_qspi_send_data(const lcd_qspi_device_t *device, uint32_t *data, uint8_t data_len);

#ifdef __cplusplus
}
#endif

