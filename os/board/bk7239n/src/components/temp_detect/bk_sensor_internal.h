// Copyright 2020-2025 Beken
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

#include <common/sys_config.h>
#include <components/sensor.h>

#define TEMPD_TAG "tempd"
#define TEMPD_LOGI(...) BK_LOGI(TEMPD_TAG, ##__VA_ARGS__)
#define TEMPD_LOGW(...) BK_LOGW(TEMPD_TAG, ##__VA_ARGS__)
#define TEMPD_LOGE(...) BK_LOGE(TEMPD_TAG, ##__VA_ARGS__)
#define TEMPD_LOGD(...) BK_LOGD(TEMPD_TAG, ##__VA_ARGS__)

#define ADC_TMEP_DETECT_INTVAL_CHANGE               (120) // 2 mins

#define BK_ERR_TEMPD_SAMPLE_NO_DATA                 BK_ERR_TEMPD_BASE

void temp_detect_change_configuration(uint32_t intval, uint32_t thre, uint32_t dist);
int temp_detect_init(uint32_t init_val);
int temp_detect_deinit(void);
int temp_detect_stop(void);
int temp_detect_start(void);
bool temp_detect_is_init(void);
int temp_detect_get_temperature(uint32_t *temperature);
bk_err_t bk_sensor_traversal_temperature_callback(uint16_t adc_code, float temp_code, float temp_last);
bk_err_t bk_sensor_set_current_temperature(float temperature);

/* volt detector API, for upper layer and other modules */
int volt_detect_start(void);
int volt_detect_stop(void);
int volt_single_get_current_voltage(UINT32 *volt_value);
bk_err_t bk_sensor_traversal_voltage_callback(uint16_t adc_code, float volt_code, float volt_last);
bk_err_t bk_sensor_set_current_voltage(float voltage);
