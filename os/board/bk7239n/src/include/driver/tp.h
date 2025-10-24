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

#include "driver/tp_types.h"


#ifdef __cplusplus
extern "C" {
#endif

// TP support maximum number.
#ifndef TP_SUPPORT_MAX_NUM
    #define TP_SUPPORT_MAX_NUM (1)
#endif

/**
 * @brief     Set tp sensor device list
 *
 * This API will set device list and device list size.
 *
 * param list: tp device list
 * param size: the size of tp device list
 *
 * @return
 *    - None
 */
void bk_tp_set_sensor_devices_list(const tp_sensor_config_t **list, uint16_t size);

/**
 * @brief     Init the tp
 *
 * This API will auto detect tp sensor, and init gpio, i2c module, sensor, ect.
 *
 * param config: process for sensor data
 *
 * @attation: you need make sure upper module exist.
 *
 * @return
 *    - 0: succeed
 *    - others: other errors.
 */
bk_err_t bk_tp_driver_init(tp_config_t *config);

/**
 * @brief     Deinit the tp
 *
 * This API will deinit sensor, i2c module, gpio, ect.
 *
 * @return
 *    - 0: succeed
 *    - others: other errors.
 */
bk_err_t bk_tp_driver_deinit(void);

/**
 * @brief     Read tp info callback
 *
 * This API will called after read tp info
 *
 * param tp_data: tp read info datas
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
void bk_tp_read_info_callback(tp_data_t *tp_data);

/**
 * @brief     Get current use tp
 *
 * This API will called after bk_tp_driver_init
 *
 * @return
 *    - tp_device_t *: succeed
 *    - NULL: current no sensor work.
 */
tp_device_t *bk_tp_get_device(void);

/**
 * @brief     Read sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * param addr: sensor read address
 *
 * param reg: sensor register address
 *
 * param value: sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int tp_i2c_read_uint8(uint8_t addr, uint8_t reg, uint8_t *buff, uint16_t len);

/**
 * @brief     Write sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * param addr: sensor write address
 *
 * param reg: sensor register address
 *
 * param value: sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int tp_i2c_write_uint8(uint8_t addr, uint8_t reg, uint8_t *buff, uint16_t len);

/**
 * @brief     Read sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * param addr: sensor read address
 *
 * param reg: sensor register address
 *
 * param value: sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int tp_i2c_read_uint16(uint8_t addr, uint16_t reg, uint8_t *buff, uint16_t len);

/**
 * @brief     Write sensor register value
 *
 * This API will called after bk_i2c_init
 *
 * param addr: sensor write address
 *
 * param reg: sensor register address
 *
 * param value: sensor register value
 *
 * @return
 *    - 0 : succeed
 *    - other: other errors.
 */
int tp_i2c_write_uint16(uint8_t addr, uint16_t reg, uint8_t *buff, uint16_t len);

#ifdef __cplusplus
}
#endif
