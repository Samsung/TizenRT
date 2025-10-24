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

#include <driver/int_types.h>
#include <common/bk_include.h>
#include <driver/hal/hal_aud_types.h>

#include "aud_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_AUD_ADC_BASE             (BK_ERR_AUD_BASE -10)
#define BK_ERR_AUD_ADC_NOT_INIT         (BK_ERR_AUD_ADC_BASE - 1) /**< audio adc not init */


/**
 * @}
 */

/**
 * @brief AUD enum defines
 * @defgroup bk_api_aud_enum AUD enums
 * @ingroup bk_api_aud
 * @{
 */

typedef enum {
	AUD_ADC_MODE_DIFFEN = 0,
	AUD_ADC_MODE_SIGNAL_END,
	AUD_ADC_MODE_MAX,
} aud_adc_mode_t;

typedef enum {
	AUD_ADC_HPF_BYPASS_DISABLE = 0,    /**< AUD ADC hpf bypass disable */
	AUD_ADC_HPF_BYPASS_ENABLE,         /**< AUD ADC hpf bypass enable */
	AUD_ADC_HPF_BYPASS_MAX,
} aud_adc_hpf_bypass_t;

typedef enum {
	AUD_ADC_SAMP_EDGE_RISING = 0,    /**< AUD ADC sampling clock edge : rising edge */
	AUD_ADC_SAMP_EDGE_FALLING,       /**< AUD ADC sampling clock edge : falling edge */
	AUD_ADC_SAMP_EDGE_MAX,
} aud_adc_samp_edge_t;

typedef enum {
	AUD_ADCL_INT_DISABLE = 0,    /**< AUD ADCL interrupt disable */
	AUD_ADCL_INT_ENABLE,         /**< AUD ADCL interrupt enable */
	AUD_ADCL_INT_MAX,
} aud_adcl_int_t;

typedef enum {
	AUD_LOOP_ADC2DAC_DISABLE = 0,    /**< ADC to DAC loop test disable */
	AUD_LOOP_ADC2DAC_ENABLE,         /**< ADC to DAC loop test enable */
	AUD_LOOP_ADC2DAC_MAX,
} aud_loop_adc2dac_t;

typedef enum {
	AUD_AGC_NG_DISABLE = 0,    /**< disable noise gating */
	AUD_AGC_NG_ENABLE,         /**< enable noise gating */
	AUD_AGC_NG_MAX,
} aud_agc_ng_enable_t;

typedef enum {
	AUD_AGC_NG_METHOD_MUTE = 0,    /**< mute or demute : default */
	AUD_AGC_NG_METHOD_CREASE,      /**< gradually increase or decrease gac */
	AUD_AGC_NG_METHOD_MAX,
} aud_agc_method_t;

typedef enum {
	AUD_AGC_DECAY_TIME_128 = 0,    /**< AGC decay time :128  2.7ms */
	AUD_AGC_DECAY_TIME_256,        /**< AGC decay time :256 */
	AUD_AGC_DECAY_TIME_512,        /**< AGC decay time :512 */
	AUD_AGC_DECAY_TIME_1024,       /**< AGC decay time :1024 */
	AUD_AGC_DECAY_TIME_2048,       /**< AGC decay time :2048 */
	AUD_AGC_DECAY_TIME_4096,       /**< AGC decay time :4096 */
	AUD_AGC_DECAY_TIME_8192,       /**< AGC decay time :8192 */
	AUD_AGC_DECAY_TIME_16384,      /**< AGC decay time :16384  340ms*/
	AUD_AGC_DECAY_TIME_MAX,
} aud_agc_decay_time_t;

typedef enum {
	AUD_AGC_ATTACK_TIME_8 = 0,    /**< AGC attack time :8  0.1667ms */
	AUD_AGC_ATTACK_TIME_16,       /**< AGC attack time :16 */
	AUD_AGC_ATTACK_TIME_32,       /**< AGC attack time :32 */
	AUD_AGC_ATTACK_TIME_64,       /**< AGC attack time :64 */
	AUD_AGC_ATTACK_TIME_128,      /**< AGC attack time :128 */
	AUD_AGC_ATTACK_TIME_256,      /**< AGC attack time :256 */
	AUD_AGC_ATTACK_TIME_512,      /**< AGC attack time :512 */
	AUD_AGC_ATTACK_TIME_1024,     /**< AGC attack time :1024  21 ms*/
	AUD_AGC_ATTACK_TIME_MAX,
} aud_agc_attack_time_t;

typedef enum {
	AUD_AGC_IIR_COEF_1_32 = 0,    /**< AGC IRR coefficient : 1/32 */
	AUD_AGC_IIR_COEF_1_64,        /**< AGC IRR coefficient : 1/64 */
	AUD_AGC_IIR_COEF_1_128,       /**< AGC IRR coefficient : 1/128 */
	AUD_AGC_IIR_COEF_1_256,       /**< AGC IRR coefficient : 1/256 */
	AUD_AGC_IIR_COEF_1_512,       /**< AGC IRR coefficient : 1/512 */
	AUD_AGC_IIR_COEF_1_1024,      /**< AGC IRR coefficient : 1/1024 */
	AUD_AGC_IIR_COEF_1_2048,      /**< AGC IRR coefficient : 1/2048 */
	AUD_AGC_IIR_COEF_1_4096,      /**< AGC IRR coefficient : 1/4096 */
	AUD_AGC_IIR_COEF_MAX,
} aud_agc_iir_coef_t;

typedef enum {
	AUD_AGC_DISABLE = 0,    /**< disable AGC */
	AUD_AGC_ENABLE,         /**< enable AGC */
	AUD_AGC_MAX,
} aud_agc_enable_t;

typedef enum {
	AUD_AGC_MANUAL_PGA_DISABLE = 0,    /**< disable AGC manual set pga */
	AUD_AGC_MANUAL_PGA_ENABLE,         /**< enable AGC manual set pga */
	AUD_AGC_MANUAL_PGA_MAX,
} aud_agc_manual_pga_en_t;

typedef enum {
	AUD_ADC_DISABLE = 0,    /**< disable adc */
	AUD_ADC_ENABLE,         /**< enable adc */
	AUD_ADC_ENABLE_MAX,
} aud_adc_enable_t;

typedef enum {
	AUD_ADC_LINE_DISABLE = 0,    /**< disable line in */
	AUD_ADC_LINE_ENABLE,         /**< enable line in */
	AUD_ADC_LINE_ENABLE_MAX,
} aud_adc_line_enable_t;

typedef enum {
	AUD_ADC_CHL_L = 0,    /**< ADC L */
	AUD_ADC_CHL_R,        /**< ADC R */
	AUD_ADC_CHL_LR,       /**< ADC L&R */
	AUD_ADC_CHL_MAX,
} aud_adc_chl_t;

typedef enum {
	AUD_MIC_MIC1 = 0,    /**< enable mic1 */
	AUD_MIC_MIC2,        /**< enable mic2 */
	AUD_MIC_BOTH,        /**< enable mic1 and mic2 */
} aud_mic_id_t;

typedef enum {
	AUD_AGC_NOISE_TOUT_0 = 0,    /**< AGC noise tout: 0  0ms */
	AUD_AGC_NOISE_TOUT_4,        /**< AGC noise tout: 4  0.5ms */
	AUD_AGC_NOISE_TOUT_8,        /**< AGC noise tout: 8 */
	AUD_AGC_NOISE_TOUT_16,       /**< AGC noise tout: 16 */
	AUD_AGC_NOISE_TOUT_32,       /**< AGC noise tout: 32 */
	AUD_AGC_NOISE_TOUT_64,       /**< AGC noise tout: 64 */
	AUD_AGC_NOISE_TOUT_128,      /**< AGC noise tout: 128 */
	AUD_AGC_NOISE_TOUT_256,      /**< AGC noise tout: 256  32ms */
	AUD_AGC_NOISE_TOUT_MAX,
} aud_gac_noise_tout_t;

typedef enum {
	AUD_AGC_HIGH_DUR_0 = 0,    /**< AGC 0 ms，as soon as MIC_RSSI value higher than NOISE_HIGH，Noise gating work，leave off noise status */
	AUD_AGC_HIGH_DUR_4,        /**< AGC high duration: 4  0.5ms*/
	AUD_AGC_HIGH_DUR_8,        /**< AGC high duration: 8 */
	AUD_AGC_HIGH_DUR_16,       /**< AGC high duration: 16 */
	AUD_AGC_HIGH_DUR_32,       /**< AGC high duration: 32 */
	AUD_AGC_HIGH_DUR_64,       /**< AGC high duration: 64 */
	AUD_AGC_HIGH_DUR_128,      /**< AGC high duration: 128 */
	AUD_AGC_HIGH_DUR_256,      /**< AGC high duration: 256  32ms */
	AUD_AGC_HIGH_DUR_MAX,
} aud_gac_high_dur_t;

typedef enum {
	AUD_AGC_LOW_DUR_0 = 0,    /**< AGC 0 ms，as soon as MIC_RSSI value lower than NOISE_HIGH，Noise gating work，leave off noise status */
	AUD_AGC_LOW_DUR_4,        /**< AGC low duration: 4  0.5ms*/
	AUD_AGC_LOW_DUR_8,        /**< AGC low duration: 8 */
	AUD_AGC_LOW_DUR_16,       /**< AGC low duration: 16 */
	AUD_AGC_LOW_DUR_32,       /**< AGC low duration: 32 */
	AUD_AGC_LOW_DUR_64,       /**< AGC low duration: 64 */
	AUD_AGC_LOW_DUR_128,      /**< AGC low duration: 128 */
	AUD_AGC_LOW_DUR_256,      /**< AGC low duration: 256  32ms */
	AUD_AGC_LOW_DUR_MAX,
} aud_gac_low_dur_t;


typedef enum {
	/* fifo status */
	AUD_ADCL_NEAR_FULL_MASK = 1 << 2,      /**< AUD ADC left channel FIFO near full */
	AUD_ADCL_NEAR_EMPTY_MASK = 1 << 6,     /**< AUD ADC left channel FIFO near empty */
	AUD_ADCL_FIFO_FULL_MASK = 1 << 10,     /**< AUD ADC left channel FIFO full */
	AUD_ADCL_FIFO_EMPTY_MASK = 1 << 14,    /**< AUD ADC left channel FIFO empty */
} aud_adc_status_mask_t;


/**
 * @}
 */

/**
 * @brief audio adc struct defines
 * @defgroup bk_api_aud_structs structs in AUD
 * @ingroup bk_api_aud
 * @{
 */

typedef struct {
	aud_adc_chl_t adc_chl;                /**< AUD adc sample rate */
	uint32_t samp_rate;                   /**< AUD adc sample rate */
	uint32_t adc_gain;                    /**< AUD adc gain set */
	aud_adc_samp_edge_t adc_samp_edge;    /**< AUD ADC data sampling clock edge select  rising/falling */
	aud_adc_mode_t adc_mode;              /**< AUD ADC mode select  single_end/differen */
	aud_clk_t clk_src;                    /**< AUD ADC clock select  xtal/apll */
} aud_adc_config_t;

#define DEFAULT_AUD_ADC_CONFIG() {                                \
        .adc_chl = AUD_ADC_CHL_L,                                 \
        .samp_rate = 8000,                                        \
        .adc_gain = 0x2d,                                         \
        .adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING,                \
        .adc_mode = AUD_ADC_MODE_DIFFEN,                          \
        .clk_src = AUD_CLK_APLL,                                  \
    }


typedef struct {
	aud_adc_hpf_bypass_t adc_hpf2_bypass_enable;    /**< AUD adc hpf2 disable */
	aud_adc_hpf_bypass_t adc_hpf1_bypass_enable;    /**< AUD adc hpf1 disable */

	/* adc_config1 */
	uint32_t adc_hpf2_coef_B0;                      /**< AUD adc HPF2 coefficient B0 */
	uint32_t adc_hpf2_coef_B1;                      /**< AUD adc HPF2 coefficient B1 */
	uint32_t adc_hpf2_coef_B2;                      /**< AUD adc HPF2 coefficient B2 */

	/* adc_config2 */
	uint32_t adc_hpf2_coef_A0;                      /**< AUD adc HPF2 coefficient A0 */
	uint32_t adc_hpf2_coef_A1;                      /**< AUD adc HPF2 coefficient A1 */
} aud_adc_hpf_config_t;

typedef struct {
	/* agc_config0 */
	uint32_t agc_noise_thrd;                        /**< AUD AGC noise gating of excute，noise gating work when audio value is little */
	uint32_t agc_noise_high;                        /**< AUD AGC corresponding MIC_RSSI low 10 bit */
	uint32_t agc_noise_low;                         /**< AUD AGC corresponding MIC_RSSI low 10 bit */

	/* agc_config1 */
	uint32_t agc_noise_min;                         /**< AUD AGC {GAIN2[2:0] GAIN1[3:0]} when signal level below NOISE_LOW, when noise gating is enabled */
	aud_gac_noise_tout_t agc_noise_tout;            /**< AUD AGC noise tout */
	aud_gac_high_dur_t agc_high_dur;                /**< AUD AGC high noise gating work duration */
	aud_gac_low_dur_t agc_low_dur;                  /**< AUD AGC low noise gating work duration */
	uint32_t agc_min;                               /**< AUD AGC Minimum value of {GAIN2[2:0] GAIN1[3:0]} */
	uint32_t agc_max;                               /**< AUD AGC Maximum value of {GAIN2[2:0] GAIN1[3:0]} Also the  default gain setting when AGC is disabled */
	aud_agc_method_t agc_ng_method;                 /**< AUD AGC  noise gating method */
	aud_agc_ng_enable_t agc_ng_enable;              /**< AUD AGC  enable noise gating */

	/* agc_config2 */
	aud_agc_decay_time_t agc_decay_time;            /**< AUD AGC  agc decay time */
	aud_agc_attack_time_t agc_attack_time;          /**< AUD AGC  agc attack time */
	uint32_t agc_high_thrd;                         /**< AUD AGC coefficient high five bit of MIC_RSSI */
	uint32_t agc_low_thrd;                          /**< AUD AGC coefficient low five bit of MIC_RSSI */
	aud_agc_iir_coef_t agc_iir_coef;                /**< AUD AGC iir coefficient select */
	aud_agc_enable_t agc_enable;                    /**< AUD AGC enable */
	uint32_t manual_pga_value;                      /**< AUD AGC manual set PGA value */
	aud_agc_manual_pga_en_t manual_pga_enable;      /**< AUD AGC manual set PGA enable */
} aud_adc_agc_config_t;

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
