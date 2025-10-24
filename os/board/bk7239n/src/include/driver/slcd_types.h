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

#include <driver/int_types.h>
#include <common/bk_include.h>
#include "gpio_map.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLCD_SEG_0 = 0,
    SLCD_SEG_1,
    SLCD_SEG_2,
    SLCD_SEG_3,
    SLCD_SEG_4,
    SLCD_SEG_5,
    SLCD_SEG_6,
    SLCD_SEG_7,
    SLCD_SEG_8,
    SLCD_SEG_9,
    SLCD_SEG_10,
    SLCD_SEG_11,
    SLCD_SEG_12,
    SLCD_SEG_13,
    SLCD_SEG_14,
    SLCD_SEG_15,
    SLCD_SEG_16,
    SLCD_SEG_17,
    SLCD_SEG_18,
    SLCD_SEG_19,
    SLCD_SEG_20,
    SLCD_SEG_21,
    SLCD_SEG_22,
    SLCD_SEG_23,
    SLCD_SEG_24,
    SLCD_SEG_25,
    SLCD_SEG_26,
    SLCD_SEG_27,
    SLCD_SEG_28,
    SLCD_SEG_29,
    SLCD_SEG_30,
    SLCD_SEG_31,
} slcd_seg_id_t;

typedef enum {
    SLCD_COM_NUM_4 = 0,         // 4 COM, seg_id is from 0 to 31.
    SLCD_COM_NUM_8,             // 8 COM, seg_id is from 0 to 27.
} slcd_com_num_t;

typedef enum {
    SLCD_BIAS_1_PER_OF_3 = 0,   // 1/3
    SLCD_BIAS_1_PER_OF_4,       // 1/4
} slcd_bias_t;

typedef enum {
    SLCD_RATE_LEVEL_0 = 0,
    SLCD_RATE_LEVEL_1,
    SLCD_RATE_LEVEL_2,
    SLCD_RATE_LEVEL_3,
} slcd_rate_t;

typedef struct {
    gpio_id_t gpio_id;
    gpio_dev_t dev;
} slcd_gpio_map_t;

typedef struct {
    slcd_com_num_t com_num;
    slcd_bias_t slcd_bias;
    slcd_rate_t slcd_rate;
} slcd_config_t;

#ifdef __cplusplus
}
#endif