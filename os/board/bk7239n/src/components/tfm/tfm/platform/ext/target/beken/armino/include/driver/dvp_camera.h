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
#include "driver/dvp_camera_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the camera
 *
 * This API will auto detect dvp sensor, and init sensor, jpeg module, i2c module, psram, dma, ect.
 *
 * @param config process for sensor data
 *
 * @attation 1. you need make sure upper module exist.
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */

bk_err_t bk_dvp_camera_driver_init(dvp_camera_config_t *config);

/**
 * @brief     Deinit the camera
 *
 * This API will deinit  sensor, jpeg module, i2c module, psram, dma, ect.
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_dvp_camera_driver_deinit(void);

/**
 * @brief     Get current use camera
 *
 * This API will called after bk_dvp_camera_driver_init
 *
 * @return
 *    - dvp_camera_device_t *: succeed
 *    - NULL: current no sensor work.
 */
dvp_camera_device_t *bk_dvp_camera_get_device(void);

/**
 * @brief     uvc power on
 *
 * This API called by user, before calling bk_dvp_driver_init, you should power on dvp
 *
 * @param enable power up/down:1/0
 *
 * @attation 1. This api config ctrl by marco VIDEO_GPIO_CTRL_LDO_ENABLE
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dvp_camera_power_enable(uint8_t enable);

const dvp_sensor_config_t *bk_dvp_get_sensor_auto_detect(void);

#ifdef __cplusplus
}
#endif
