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
//

#pragma once

#ifndef _ES8311_TYPES_H_
#define _ES8311_TYPES_H_

#include <driver/i2s_types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    ES8311_MIC_GAIN_MIN = -1,
    ES8311_MIC_GAIN_0DB,
    ES8311_MIC_GAIN_6DB,
    ES8311_MIC_GAIN_12DB,
    ES8311_MIC_GAIN_18DB,
    ES8311_MIC_GAIN_24DB,
    ES8311_MIC_GAIN_30DB,
    ES8311_MIC_GAIN_36DB,
    ES8311_MIC_GAIN_42DB,
    ES8311_MIC_GAIN_MAX
} es8311_mic_gain_t;

/**
 * @brief Select es8311 work mode
 */
typedef enum {
    ES8311_MODE_ENCODE = 1,  /*!< select adc */
    ES8311_MODE_DECODE,      /*!< select dac */
    ES8311_MODE_BOTH,        /*!< select both adc and dac */
    ES8311_MODE_LINE_IN,     /*!< set adc channel */
} es8311_mode_t;

/**
 * @brief Select adc channel for input mic signal
 */
typedef enum {
    ES8311_ADC_INPUT_LINE1 = 0x00,  /*!< mic input to adc channel 1 */
    ES8311_ADC_INPUT_LINE2,         /*!< mic input to adc channel 2 */
    ES8311_ADC_INPUT_ALL,           /*!< mic input to both channels of adc */
    ES8311_ADC_INPUT_DIFFERENCE,    /*!< mic input to adc difference channel */
    ES8311_ADC_INPUT_DMIC,          /*!< dmic input to adc difference channel */
} es8311_adc_input_t;

/**
 * @brief Select channel for dac output
 */
typedef enum {
	ES8311_DAC_OUTPUT_LINE1 = 0x00,  /*!< dac output signal to channel 1 */
	ES8311_DAC_OUTPUT_LINE2,         /*!< dac output signal to channel 2 */
	ES8311_DAC_OUTPUT_ALL,           /*!< dac output signal to both channels */
} es8311_dac_output_t;

/*
 * Clock coefficient structer
 */
struct mclk_div_coeff {
    uint32_t mclk;          /* mclk frequency */
    uint32_t samp_rate;     /* sample rate */
    uint32_t lrck_div;      /* LRCK (master)=MCLK/(LRCK_DIV+1) */
    uint8_t bclk_div;       /* BCCK (master)=MCLK/(BCLK_DIV+1) */
    uint8_t pre_div;        /* mclk_prediv=mclkin/(DIV_PRE+1) */
    uint8_t adc_div;        /* adc_mclk=dig_mclk/(DIV_CLKADC+1) */
    uint8_t dac_div;        /* dac_mclk=dig_mclk/(DIV_CLKDAC+1) */
    uint8_t scale;          /* adc scale */
    uint8_t adc_osr;        /* adc osr */
    uint8_t dac_osr;        /* dac osr */
};

/**
 * @brief Select I2S interface samples per second
 */
typedef enum {
    ES8311_SAMP_RATE_8K,      /*!< set to  8k samples per second */
    ES8311_SAMP_RATE_12K,     /*!< set to 12k samples per second */
    ES8311_SAMP_RATE_16K,     /*!< set to 16k samples per second */
    ES8311_SAMP_RATE_24K,     /*!< set to 24k samples per second */
    ES8311_SAMP_RATE_32K,     /*!< set to 32k samples per second */
    ES8311_SAMP_RATE_48K,     /*!< set to 48k samples per second */
    ES8311_SAMP_RATE_11_025K, /*!< set to 11.025k samples per second */
    ES8311_SAMP_RATE_22_05K,  /*!< set to 22.05k samples per second */
    ES8311_SAMP_RATE_44_1K,   /*!< set to 44.1k samples per second */
} es8311_samp_rate_t;

/**
 * @brief I2s interface configuration for audio codec chip
 */
typedef struct {
    i2s_role_t role;               /*!< I2S role */
    i2s_work_mode_t mode;          /*!< I2S work mode */
    es8311_samp_rate_t samp_rate;  /*!< I2S sample rate per second */
    i2s_data_width_t bits;         /*!< i2s number of bits per sample */
} es8311_i2s_config_t;

/**
 * @brief Configure es8311 audio codec chip
 */
typedef struct {
    es8311_adc_input_t adc_input;      /*!< set adc channel */
//    es8311_dac_output_t dac_output;  /*!< set dac channel */
    es8311_mode_t codec_mode;          /*!< select codec mode: adc, dac or both */
    es8311_i2s_config_t i2s_cfg;       /*!< set I2S configuration */
} es8311_codec_config_t;

#define DEFAULT_ES8311_SETUP_CONFIG() {                                         \
        .adc_input = ES8311_ADC_INPUT_DMIC,                                     \
        .codec_mode = ES8311_MODE_ENCODE,                                       \
        .i2s_cfg = {                                                            \
                       .role = I2S_ROLE_MASTER,                                 \
                       .mode = I2S_WORK_MODE_I2S,                               \
                       .samp_rate = ES8311_SAMP_RATE_16K,                       \
                       .bits = I2S_DATA_WIDTH_16,                               \
                    },                                                          \
    }

#ifdef __cplusplus
}
#endif

#endif
