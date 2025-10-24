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

#include <components/log.h>
#include <driver/touch_types.h>


#define TOUCH_TAG "touch"
#define TOUCH_LOGI(...) BK_LOGI(TOUCH_TAG, ##__VA_ARGS__)
#define TOUCH_LOGW(...) BK_LOGW(TOUCH_TAG, ##__VA_ARGS__)
#define TOUCH_LOGE(...) BK_LOGE(TOUCH_TAG, ##__VA_ARGS__)
#define TOUCH_LOGD(...) BK_LOGD(TOUCH_TAG, ##__VA_ARGS__)

#define SOC_TOUCH_ID_NUM		16

void touch_isr(void);

/**
 * @brief     enable/disable calibretion of touch channel
 *
 * This API enable or disable the touch channel calibretion.
 *
 *
 * @param
 *    - enable: enable -- 1; disable -- 0;
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_calib_enable(uint32_t enable);

/**
 * @brief     get the status of touch channel interrupt
 *
 * This API get the status of touch channel interrupt. One bit corresponding to one channel.
 *
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_get_int_status(void);

/**
 * @brief     clear the status of touch interrupt
 *
 * This API clear the status of touch interrupt. One bit corresponding to one channel.
 *
 *
 * @param
 *    - touch_id: touch channel, channel 0 ~ channel 15;
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_clear_int(touch_channel_t touch_id);

/**
 * @brief     set test mode of touch
 *
 * This API set test period and number of touch.
 *
 *
 * @param
 *    - period: set touch period;
 *    - number: the number of touch will trigger an effective touch;
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_set_test_mode(uint8_t period, uint8_t number);

/**
 * @brief     set calibration mode of touch
 *
 * This API set calibration period and number of touch.
 *
 *
 * @param
 *    - period: set touch calibration period;
 *    - number: the number of calibration will trigger an effective calibration;
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_set_calib_mode(uint8_t period, uint8_t number);

