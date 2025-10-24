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

#include <driver/hal/hal_gpio_types.h>

typedef struct{
    uint8_t samp_mode ;  // 1: Single Mode 0: Continue Mode
    uint8_t samp_numb ;  // Sample Numbers @SingleMode : 0/1/2/3 -> 1/2/4/8
    uint8_t samp_chan ;  // Sample Channel : 0 ~ 15
    uint8_t cic2_bpss ;  // Cic2 Filter Bypass
    uint8_t comp_bpss ;  // Comp Filter Bypass
    uint8_t cic2_gain ;  // Cic2 Gains Setting (0dBm @ 0x2d)
    uint8_t int_enable ; // ADC Interrupt Enable -- {near_empt, near_full, fifo_empt, fifo_full, samp_finish}
    uint16_t cali_gains ; // ADC Calibration Gain -- 0 : 1/4096 : 2
    uint16_t cali_offset ; // ADC Calibration Offset
} sdmadc_config_t;

typedef enum {
	SDMADC_CONTINUOUS_MODE = 0 ,/**< SDMADC continuous mode */
	SDMADC_SINGLE_STEP_MODE,/**< SDMADC single step mode */
	SDMADC_NONE_MODE,
} sdmadc_mode_t;

typedef enum {
	ONEPOINT_PER_STEP = 0,
	TWOPOINT_PER_STEP,
	FOURPOINT_PER_STEP,
	EIGHTPOINT_PER_STEP,
}sdmadc_sample_numb_t;

typedef struct _sdmadc_calibrate_val_
{
    int16_t low;
    int16_t high;
} sdmadc_calibrate_val;

typedef enum {
	SDMADC_1 = 1,
	SDMADC_2,
	SDMADC_3,
	SDMADC_4,
	SDMADC_5,
	SDMADC_6,
	SDMADC_7,
	SDMADC_8,
	SDMADC_9,
	SDMADC_10,
	SDMADC_11,
	SDMADC_12,
	SDMADC_13,
	SDNADC_MAX,
} sdmadc_chan_t;

typedef struct {
	sdmadc_chan_t sdmadc_chan;
	gpio_id_t gpio_id;
	gpio_dev_t gpio_dev;
} sdmadc_gpio_map_t;

#ifndef CONFIG_SDMADC_BUF_SIZE
#define CONFIG_SDMADC_BUF_SIZE              1024
#endif

#define CFG_SDMADC_OFFSET                          32768

#ifdef __cplusplus
}
#endif