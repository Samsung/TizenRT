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
#include <driver/touch_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief TOUCH API
 * @defgroup touch.h TOUCH API group
 * @{
 */

/**
 * @brief     Init the touch gpio pin
 *
 * This API init the GPIO pin corresponding to touch channel:
 *   - Unmap gpio pin function
 *   - Change gpio pin function to touch
 *   - Configure gpio to be high impedance
 *
 * This API should be called before any other TOUCH APIs.
 *
 * @param
 *    - touch_id: touch channel, channel 0 ~ channel 15
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_gpio_init(touch_channel_t touch_id);

/**
 * @brief     enable touch function
 *
 * This API enable the touch function:
 *   - Select the touch channel
 *   - Power on the touch module
 *
 *
 * @param
 *    - touch_id: touch channel, channel 0 ~ channel 15
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_enable(touch_channel_t touch_id);

/**
 * @brief     disable touch function
 *
 * This API disable the touch function:
 *   - Power down the touch module
 *
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_disable(void);

/**
 * @brief     config touch function
 *
 * This API config the touch function:
 *   - Set touch sensitivity level, the smaller the value, the more sensitive. The range of level value from 0 to 3
 *   - Set touch detect threshold, the range of value from 0 to 7.
 *   - Set touch detect range, 8pF/12pF/19pF/27pF
 *
 *
 * @param
 *    - touch_config: the config of touch work mode
 *
 * @return
 *    - BK_OK: succeed
 *
 *
 * Usage example:
 *
 *		touch_config_t touch_config;
 *		touch_config.sensitivity_level = TOUCH_SENSITIVITY_LEVLE_3;
 *		touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
 *		touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
 *		bk_touch_config(&touch_config);
 */
bk_err_t bk_touch_config(const touch_config_t *touch_config);

/**
 * @brief     start calibrating touch channel
 *
 * This API start calibrating the touch channel that selected.
 *
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_calibration_start(void);

/**
 * @brief     enable/disable scan mode of touch channel
 *
 * This API enable or disable the scan mode of touch channel.
 *
 *
 * @param
 *    - enable: enable —— 1; disable —— 0;
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_scan_mode_enable(uint32_t enable);

/**
 * @brief     enable/disable adc mode of touch channel
 *
 * This API enable or disable the adc mode of touch channel.
 *
 *
 * @param
 *    - enable: enable —— 1; disable —— 0;
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_adc_mode_enable(uint32_t enable);

/**
 * @brief     enable manul calibration mode of touch channel
 *
 * This API enable the manul calibration mode of touch channel:
 *    - set the calibration value of manul mode;
 *    - enable the manul calibretion mode;
 *
 *
 * @param
 *    - calib_value: calibration value of manul mode, the max value is 0x1FF;
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_manul_mode_enable(uint32_t calib_value);

/**
 * @brief     disable manul calibration mode of touch channel
 *
 * This API disable the manul calibration mode of touch channel
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_manul_mode_disable(void);

/**
 * @brief     set multi channels that need to scan
 *
 * This API set multi channels that need to scan
 *
 *
 * @param
 *    - touch_id: the multi channels that need to scan. See the enum of touch_channel_t.
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_scan_mode_multi_channl_set(touch_channel_t touch_id);

/**
 * @brief     enable/disable touch channel interrupt
 *
 * This API enable or disable the touch channel interrupt:
 *    - register the interrupt service handle;
 *    - enable or disable the touch interrupt;
 *
 *
 * @param
 *    - touch_id: touch channel, channel 0 ~ channel 15;
 *    - enable: enable —— 1; disable —— 0;
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_int_enable(touch_channel_t touch_id, uint32_t enable);

/**
 * @brief     get the calibration value of touch channel
 *
 * This API get the calibration value of touch channel.
 *
 *
 * @param
 *    - None
 *
 * @return
 *    - calib_value: the max value is 0x1FF.
 */
uint32_t bk_touch_get_calib_value(void);

/**
 * @brief     get the status of touch channel
 *
 * This API get the status of touch channel:
 *
 *
 * @param
 *    - None
 *
 * @return
 *    - touch_status: the touch status of every channel. One bit corresponding to one channel. 1 —— the channel is touched; 0 —— the channel is idle.
 */
uint32_t bk_touch_get_touch_status(void);

/**
 * @brief     init the digital tube of touch
 *
 * This API init the digital tube:
 *
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_touch_digital_tube_init(void);

/**
 * @brief     display the value in the digital tube 
 *
 * This API display the value in the digital tube
 *
 *
 * @param
 *    - disp_value: the value need to display;
 *
 *
 * @return
 *    - None
 */
void bk_touch_digital_tube_display(uint8_t disp_value);

/**
 * @brief     get the touch channel for low voltage and deepsleep wake up 
 *
 * This API get the touch channel for low voltage and deepsleep wake up
 *
 *
 * @param
 *    - channel: touch channel, channel 0 ~ channel 15. See the enum of touch_channel_t.
 *
 *
 * @return
 *    - None
 */
void bk_touch_wakeup_channel_set(touch_channel_t channel);

/**
 * @brief     set the touch channel for low voltage and deepsleep wake up
 *
 * This API set the touch channel for low voltage and deepsleep wake up
 *
 *
 * @param
 *    - None
 *
 *
 * @return
 *    - s_touch_wakeup_channel: the channel number of touch
 */
uint32_t bk_touch_wakeup_channel_get(void);

/**
 * @brief     Register touch low voltage and deepsleep config
 *
 * This API register touch low voltage and deepsleep config
 *
 *
 * @param
 *    - None
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_pm_init(void);

/**
 * @brief     Register touch isr
 *
 * This API register touch isr
 *
 *
 * @param
 *    - touch_id: touch channel, channel 0 ~ channel 15;
 *    - isr: touch isr callback;
 *    - param: touch isr callback parameter;
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_touch_register_touch_isr(touch_channel_t touch_id, touch_isr_t isr, void *param);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


