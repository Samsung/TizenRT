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

#include <driver/slcd_types.h>

#ifdef __cplusplus
extern "C" {
#endif


void slcd_hal_soft_reset(void);

void slcd_hal_lcd_on(void);

void slcd_hal_lcd_off(void);

void slcd_hal_set_com_number(slcd_com_num_t com_num);

void slcd_hal_set_lcd_bias(slcd_bias_t lcd_bias);

void slcd_hal_set_lcd_rate(slcd_rate_t rate_value);

void slcd_hal_set_seg_value(slcd_seg_id_t seg_id, uint8_t value);

void slcd_hal_set_seg00_03_value(uint32_t value);

void slcd_hal_set_seg04_07_value(uint32_t value);

void slcd_hal_set_seg08_11_value(uint32_t value);

void slcd_hal_set_seg12_15_value(uint32_t value);

void slcd_hal_set_seg16_19_value(uint32_t value);

void slcd_hal_set_seg20_23_value(uint32_t value);

void slcd_hal_set_seg24_27_value(uint32_t value);

void slcd_hal_set_seg28_31_value(uint32_t value);


#ifdef __cplusplus
}
#endif