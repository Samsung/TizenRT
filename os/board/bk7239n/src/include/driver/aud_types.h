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

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*aud_isr_t)(void *param);


/**
 * @brief AUD defines
 * @defgroup bk_api_aud_defs macos
 * @ingroup bk_api_aud
 * @{
 */

#define BK_ERR_AUD_NOT_INIT			(BK_ERR_AUD_BASE - 1) /**< AUD driver not init */
#define BK_ERR_AUD_ISR_ID			(BK_ERR_AUD_BASE - 2) /**< AUD isr id is invalid */
#define BK_ERR_AUD_ADC_MODE         (BK_ERR_AUD_BASE - 3) /**< AUD adc work mode is invalid */

#define AUD_HAL_REG_BASE			AUD_LL_REG_BASE
#define AUD_DAC_FIFO_ADDR			(AUD_HAL_REG_BASE + 0x33 * 4)

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
	AUD_CLK_XTAL = 0, /**< XTAL: 26MHz */
	AUD_CLK_APLL,     /**< APLL: 24.576MHz*/
	AUD_CLK_MAX
} aud_clk_t;

typedef enum {
	AUD_ISR_DTMF = 0, /**< dtmf_int_en */
	AUD_ISR_ADCL,     /**< adcl_int_en */
	AUD_ISR_DACR,     /**< dacr_int_en */
	AUD_ISR_DACL,     /**< dacl_int_en */
	AUD_ISR_DMIC,     /**< dmic_int_en */
	AUD_ISR_MAX       /**< AUD isr id max */
} aud_isr_id_t;

typedef enum {
	AUD_ADC_WORK_MODE_NULL = 0, /**< no mode */
	AUD_ADC_WORK_MODE_ADC,      /**< ADC mode */
	AUD_ADC_WORK_MODE_DTMF,     /**< DTMF mode */
	AUD_ADC_WORK_MODE_MAX,
} aud_adc_work_mode_t;

typedef enum {
	AUD_ADC_INTF_MODE_DIFFEN = 0,
	AUD_ADC_INTF_MODE_SIGNAL_END,
	AUD_ADC_INTF_MODE_MAX,
} aud_adc_intf_mode_t;

typedef enum {
	AUD_ADC_SAMP_RATE_8K = 0,  /**< ADC sample rate : 8k */
	AUD_ADC_SAMP_RATE_11_025K, /**< ADC sample rate : 11.025k */
	AUD_ADC_SAMP_RATE_12K,     /**< ADC sample rate : 12k */
	AUD_ADC_SAMP_RATE_16K,     /**< ADC sample rate : 16k */
	AUD_ADC_SAMP_RATE_22_05K,  /**< ADC sample rate : 22.05k */
	AUD_ADC_SAMP_RATE_24K,     /**< ADC sample rate : 24k */
	AUD_ADC_SAMP_RATE_32K,     /**< ADC sample rate : 32k */
	AUD_ADC_SAMP_RATE_44_1K,   /**< ADC sample rate : 44.1k */
	AUD_ADC_SAMP_RATE_48K,     /**< ADC sample rate : 48k */
	AUD_ADC_SAMP_RATE_MAX,
} aud_adc_samp_rate_t;

typedef enum {
	AUD_ADC_HPF_BYPASS_DISABLE = 0, /**< AUD ADC hpf bypass disable */
	AUD_ADC_HPF_BYPASS_ENABLE,      /**< AUD ADC hpf bypass enable */
	AUD_ADC_HPF_BYPASS_MAX,
} aud_adc_hpf_bypass_t;

typedef enum {
	AUD_ADC_SAMP_EDGE_RISING = 0, /**< AUD ADC sampling clock edge : rising edge */
	AUD_ADC_SAMP_EDGE_FALLING,    /**< AUD ADC sampling clock edge : falling edge */
	AUD_ADC_SAMP_EDGE_MAX,
} aud_adc_samp_edge_t;

typedef enum {
	AUD_ADCL_INT_DISABLE = 0, /**< AUD ADCL interrupt disable */
	AUD_ADCL_INT_ENABLE,     /**< AUD ADCL interrupt enable */
	AUD_ADCL_INT_MAX,
} aud_adcl_int_t;

typedef enum {
	AUD_DTMF_INT_DISABLE = 0, /**< AUD DTMF interrupt disable */
	AUD_DTMF_INT_ENABLE,     /**< AUD DTMF interrupt enable */
	AUD_DTMF_INT_MAX,
} aud_dtmf_int_t;

typedef enum {
	AUD_LOOP_ADC2DAC_DISABLE = 0, /**< ADC to DAC loop test disable */
	AUD_LOOP_ADC2DAC_ENABLE,     /**< ADC to DAC loop test enable */
	AUD_LOOP_ADC2DAC_MAX,
} aud_loop_adc2dac_t;

typedef enum {
	AUD_LOOP_DTMF2DAC_DISABLE = 0, /**< DTMF to DAC loop test disable */
	AUD_LOOP_DTMF2DAC_ENABLE,     /**< DTMF to DAC loop test enable */
	AUD_LOOP_DTMF2DAC_MAX,
} aud_loop_dtmf2dac_t;

typedef enum {
	AUD_AGC_NG_DISABLE = 0, /**< disable noise gating */
	AUD_AGC_NG_ENABLE,      /**< enable noise gating */
	AUD_AGC_NG_MAX,
} aud_agc_ng_enable_t;

typedef enum {
	AUD_AGC_NG_METHOD_MUTE = 0, /**< mute or demute : default */
	AUD_AGC_NG_METHOD_CREASE,   /**< gradually increase or decrease gac */
	AUD_AGC_NG_METHOD_MAX,
} aud_agc_method_t;

typedef enum {
	AUD_AGC_DECAY_TIME_128 = 0, /**< AGC decay time :128  2.7ms */
	AUD_AGC_DECAY_TIME_256,     /**< AGC decay time :256 */
	AUD_AGC_DECAY_TIME_512,     /**< AGC decay time :512 */
	AUD_AGC_DECAY_TIME_1024,     /**< AGC decay time :1024 */
	AUD_AGC_DECAY_TIME_2048,     /**< AGC decay time :2048 */
	AUD_AGC_DECAY_TIME_4096,     /**< AGC decay time :4096 */
	AUD_AGC_DECAY_TIME_8192,     /**< AGC decay time :8192 */
	AUD_AGC_DECAY_TIME_16384,     /**< AGC decay time :16384  340ms*/
	AUD_AGC_DECAY_TIME_MAX,
} aud_agc_decay_time_t;

typedef enum {
	AUD_AGC_ATTACK_TIME_8 = 0, /**< AGC attack time :8  0.1667ms */
	AUD_AGC_ATTACK_TIME_16,     /**< AGC attack time :16 */
	AUD_AGC_ATTACK_TIME_32,     /**< AGC attack time :32 */
	AUD_AGC_ATTACK_TIME_64,     /**< AGC attack time :64 */
	AUD_AGC_ATTACK_TIME_128,     /**< AGC attack time :128 */
	AUD_AGC_ATTACK_TIME_256,     /**< AGC attack time :256 */
	AUD_AGC_ATTACK_TIME_512,     /**< AGC attack time :512 */
	AUD_AGC_ATTACK_TIME_1024,     /**< AGC attack time :1024  21 ms*/
	AUD_AGC_ATTACK_TIME_MAX,
} aud_agc_attack_time_t;

typedef enum {
	AUD_AGC_IIR_COEF_1_32 = 0, /**< AGC IRR coefficient : 1/32 */
	AUD_AGC_IIR_COEF_1_64,     /**< AGC IRR coefficient : 1/64 */
	AUD_AGC_IIR_COEF_1_128,    /**< AGC IRR coefficient : 1/128 */
	AUD_AGC_IIR_COEF_1_256,    /**< AGC IRR coefficient : 1/256 */
	AUD_AGC_IIR_COEF_1_512,    /**< AGC IRR coefficient : 1/512 */
	AUD_AGC_IIR_COEF_1_1024,   /**< AGC IRR coefficient : 1/1024 */
	AUD_AGC_IIR_COEF_1_2048,   /**< AGC IRR coefficient : 1/2048 */
	AUD_AGC_IIR_COEF_1_4096,   /**< AGC IRR coefficient : 1/4096 */
	AUD_AGC_IIR_COEF_MAX,
} aud_agc_iir_coef_t;

typedef enum {
	AUD_AGC_DISABLE = 0, /**< disable AGC */
	AUD_AGC_ENABLE,      /**< enable AGC */
	AUD_AGC_MAX,
} aud_agc_enable_t;

typedef enum {
	AUD_GAC_MANUAL_PGA_DISABLE = 0, /**< disable AGC manual set pga */
	AUD_GAC_MANUAL_PGA_ENABLE,      /**< enable AGC manual set pga */
	AUD_GAC_MANUAL_PGA_MAX,
} aud_agc_manual_pga_en_t;

typedef enum {
	AUD_ADC_DISABLE = 0, /**< disable adc */
	AUD_ADC_ENABLE,      /**< enable adc */
	AUD_ADC_ENABLE_MAX,
} aud_adc_enable_t;

typedef enum {
	AUD_ADC_LINE_DISABLE = 0, /**< disable line in */
	AUD_ADC_LINE_ENABLE,      /**< enable line in */
	AUD_ADC_LINE_ENABLE_MAX,
} aud_adc_line_enable_t;

typedef enum {
	AUD_MIC_DISABLE = 0, /**< disable all mic */
	AUD_MIC_MIC1_ENABLE, /**< enable mic1 */
	AUD_MIC_MIC2_ENABLE, /**< enable mic2 */
	AUD_MIC_ALL_ENABLE,  /**< enable all mic */
	AUD_MIC_MAX,
} aud_mic_enable_t;

typedef enum {
	AUD_MIC_MIC1,        /**< enable mic1 */
	AUD_MIC_MIC2,        /**< enable mic2 */
	AUD_MIC_BOTH,        /**< enable mic1 and mic2 */
} aud_mic_id_t;

typedef enum {
	AUD_GAC_NOISE_TOUT_0 = 0,       /**< AGC noise tout: 0  0ms */
	AUD_GAC_NOISE_TOUT_4,           /**< AGC noise tout: 4  0.5ms */
	AUD_GAC_NOISE_TOUT_8,           /**< AGC noise tout: 8 */
	AUD_GAC_NOISE_TOUT_16,          /**< AGC noise tout: 16 */
	AUD_GAC_NOISE_TOUT_32,          /**< AGC noise tout: 32 */
	AUD_GAC_NOISE_TOUT_64,          /**< AGC noise tout: 64 */
	AUD_GAC_NOISE_TOUT_128,         /**< AGC noise tout: 128 */
	AUD_GAC_NOISE_TOUT_256,         /**< AGC noise tout: 256  32ms */
	AUD_GAC_NOISE_TOUT_MAX,
} aud_gac_noise_tout_t;

typedef enum {
	AUD_GAC_HIGH_DUR_0 = 0,       /**< AGC 0 ms，as soon as MIC_RSSI value higher than NOISE_HIGH，Noise gating work，leave off noise status */
	AUD_GAC_HIGH_DUR_4,           /**< AGC high duration: 4  0.5ms*/
	AUD_GAC_HIGH_DUR_8,           /**< AGC high duration: 8 */
	AUD_GAC_HIGH_DUR_16,          /**< AGC high duration: 16 */
	AUD_GAC_HIGH_DUR_32,          /**< AGC high duration: 32 */
	AUD_GAC_HIGH_DUR_64,          /**< AGC high duration: 64 */
	AUD_GAC_HIGH_DUR_128,         /**< AGC high duration: 128 */
	AUD_GAC_HIGH_DUR_256,         /**< AGC high duration: 256  32ms */
	AUD_GAC_HIGH_DUR_MAX,
} aud_gac_high_dur_t;

typedef enum {
	AUD_GAC_LOW_DUR_0 = 0,       /**< AGC 0 ms，as soon as MIC_RSSI value lower than NOISE_HIGH，Noise gating work，leave off noise status */
	AUD_GAC_LOW_DUR_4,           /**< AGC low duration: 4  0.5ms*/
	AUD_GAC_LOW_DUR_8,           /**< AGC low duration: 8 */
	AUD_GAC_LOW_DUR_16,          /**< AGC low duration: 16 */
	AUD_GAC_LOW_DUR_32,          /**< AGC low duration: 32 */
	AUD_GAC_LOW_DUR_64,          /**< AGC low duration: 64 */
	AUD_GAC_LOW_DUR_128,         /**< AGC low duration: 128 */
	AUD_GAC_LOW_DUR_256,         /**< AGC low duration: 256  32ms */
	AUD_GAC_LOW_DUR_MAX,
} aud_gac_low_dur_t;


/* DTMF */

typedef enum {
	AUD_DTMF_DISABLE = 0, /**< disable dtmf */
	AUD_DTMF_ENABLE,      /**< enable dtmf */
	AUD_DTMF_ENABLE_MAX,
} aud_dtmf_enable_t;

typedef enum {
	AUD_DTMF_TONE_PATTERN_DTMF = 0, /**< 0: DTMF(Active_Time) + Zeros(Pause_Time) */
	AUD_DTMF_TONE_PATTERN_TONE,     /**< 1：Tone1(Active_Time) + Tone2(Pause_Time) */
	AUD_DTMF_TONE_PATTERN_MAX,
} aud_dtmf_tone_pattern_t;

typedef enum {
	AUD_DTMF_TONE_MODE_SIGNAL = 0, /**< 0：signal mode */
	AUD_DTMF_TONE_MODE_CONTIUS,    /**< 1：continuous mode */
	AUD_DTMF_TONE_MODE_MAX,
} aud_dtmf_tone_mode_t;

typedef enum {
	AUD_DTMF_TONE_ATTU_MINUS_1 = 0, /**< Tone1 damp set: -1 db */
	AUD_DTMF_TONE_ATTU_MINUS_2,     /**< Tone1 damp set: -2 db */
	AUD_DTMF_TONE_ATTU_MINUS_3,     /**< Tone1 damp set: -3 db */
	AUD_DTMF_TONE_ATTU_MINUS_4,     /**< Tone1 damp set: -4 db */
	AUD_DTMF_TONE_ATTU_MINUS_5,     /**< Tone1 damp set: -5 db */
	AUD_DTMF_TONE_ATTU_MINUS_6,     /**< Tone1 damp set: -6 db */
	AUD_DTMF_TONE_ATTU_MINUS_7,     /**< Tone1 damp set: -7 db */
	AUD_DTMF_TONE_ATTU_MINUS_8,     /**< Tone1 damp set: -8 db */
	AUD_DTMF_TONE_ATTU_MINUS_9,     /**< Tone1 damp set: -9 db */
	AUD_DTMF_TONE_ATTU_MINUS_10,    /**< Tone1 damp set: -10 db */
	AUD_DTMF_TONE_ATTU_MINUS_11,    /**< Tone1 damp set: -11 db */
	AUD_DTMF_TONE_ATTU_MINUS_12,    /**< Tone1 damp set: -12 db */
	AUD_DTMF_TONE_ATTU_MINUS_13,    /**< Tone1 damp set: -13 db */
	AUD_DTMF_TONE_ATTU_MINUS_14,    /**< Tone1 damp set: -14 db */
	AUD_DTMF_TONE_ATTU_MINUS_15,    /**< Tone1 damp set: -15 db */
	AUD_DTMF_TONE_ATTU_MINUS_MAX,
} aud_dtmf_tone_attu_t;

typedef enum {
	AUD_DTMF_TONE_DISABLE = 0, /**< 0：disable tone */
	AUD_DTMF_TONE_ENABLE,      /**< 1：enable tone */
	AUD_DTMF_TONE_ENABLE_MAX,
} aud_dtmf_tone_enable_t;

typedef enum {
	AUD_ADC_TRACMOD_MANUAL_DISABLE = 0, /**< 0：disable ADC fractional frequency division of manual set */
	AUD_ADC_TRACMOD_MANUAL_ENABLE,      /**< 1：enable ADC fractional frequency division of manual set */
	AUD_ADC_TRACMOD_MANUAL_EN_MAX,
} aud_adc_fracmod_manual_en_t;

/**dac enum */
typedef enum {
	AUD_DAC_DISABLE = 0,	/**< 0：disable dac */
	AUD_DAC_ENABLE,			/**< 1：enable dac */
	AUD_DAC_OTHERS,
} aud_dac_enable_t;

typedef enum {
	AUD_DAC_CHL_DISABLE = 0, /**< disable all dac channel */
	AUD_DAC_CHL_L_ENABLE,    /**< enable dac left channel */
	AUD_DAC_CHL_R_ENABLE,    /**< enable dac right channel */
	AUD_DAC_CHL_LR_ENABLE,   /**< enable dac left and right channel */
	AUD_DAC_CHL_MAX,
} aud_dac_chl_enable_t;

typedef enum {
	AUD_DAC_WORK_MODE_SIGNAL_END = 0,
	AUD_DAC_WORK_MODE_DIFFEN,
	AUD_DAC_WORK_MODE_MAX,
} aud_dac_work_mode_t;

typedef enum {
	AUD_DAC_SAMP_RATE_8K = 0,	/**< DAC sample rate : 8k */
	AUD_DAC_SAMP_RATE_11_025K,	/**< DAC sample rate : 11.025k */
	AUD_DAC_SAMP_RATE_12K,		/**< DAC sample rate : 12k */
	AUD_DAC_SAMP_RATE_16K,		/**< DAC sample rate : 16k */
	AUD_DAC_SAMP_RATE_22_05K,	/**< DAC sample rate : 22.05k */
	AUD_DAC_SAMP_RATE_24K,		/**< DAC sample rate : 24k */
	AUD_DAC_SAMP_RATE_32K,		/**< DAC sample rate : 32k */
	AUD_DAC_SAMP_RATE_44_1K,	/**< DAC sample rate : 44.1k */
	AUD_DAC_SAMP_RATE_48K,		/**< DAC sample rate : 48k */
	AUD_DAC_SAMP_RATE_MAX,
} aud_dac_samp_rate_t;

typedef enum
{
	AUD_DAC_HPF_BYPASS_DISABLE = 0,		/**< AUD DAC hpf bypass disable */
	AUD_DAC_HPF_BYPASS_ENABLE,			/**< AUD DAC hpf bypass enable */
	AUD_DAC_HPF_BYPASS_OTHERS,
} aud_dac_hpf_bypass_t;

typedef enum {
	AUD_DAC_CLK_INVERT_RISING = 0,		/**< AUD dac output clock edge rising */
	AUD_DAC_CLK_INVERT_FALLING,			/**< AUD dac output clock edge falling */
	AUD_DAC_CLK_INVERT_OTHERS,
} aud_dac_clk_invert_t;

typedef enum {
	AUD_DACR_INT_DISABLE = 0,			/**< AUD dac right channel interrupt disable */
	AUD_DACR_INT_ENABLE,				/**< AUD dac right channel interrupt enable */
	AUD_DACL_INT_DISABLE,				/**< AUD dac left channel interrupt disable */
	AUD_DACL_INT_ENABLE,				/**< AUD dac left channel interrupt enable */
	AUD_DAC_INT_OTHERS,
} aud_dac_int_enable_t;

typedef enum {
	AUD_DAC_FILT_DISABLE = 0,			/**< AUD dac filter disable */
	AUD_DAC_FILT_ENABLE,				/**< AUD dac filter enable */
	AUD_DAC_FILT_OTHERS,
} aud_dac_filt_enable_t;

typedef enum {
	AUD_DAC_FRACMOD_MANUAL_DISABLE = 0,		/**< disable dac fractional frequency division of manual set */
	AUD_DAC_FRACMOD_MANUAL_ENABLE,			/**< enable ADC fractional frequency division of manual set */
	ADU_DAC_FRACMOD_MANUAL_OTHERS,
} aud_dac_fracmod_manual_t;

typedef enum {
	/* fifo status */
	AUD_ADCL_NEAR_FULL_MASK = 1 << 2,		/**< AUD ADC left channel FIFO near full */
	AUD_DTMF_NEAR_FULL_MASK = 1 << 3,		/**< AUD DTMF FIFO near full */
	AUD_ADCL_NEAR_EMPTY_MASK = 1 << 6,		/**< AUD ADC left channel FIFO near empty */
	AUD_DTMF_NEAR_EMPTY_MASK = 1 << 7,		/**< AUD DTMF FIFO near empty */
	AUD_ADCL_FIFO_FULL_MASK = 1 << 10,		/**< AUD ADC left channel FIFO full */
	AUD_DTMF_FIFO_FULL_MASK = 1 << 11,		/**< AUD DTMF FIFO full */
	AUD_ADCL_FIFO_EMPTY_MASK = 1 << 14,		/**< AUD ADC left channel FIFO empty */
	AUD_DTMF_FIFO_EMPTY_MASK = 1 << 15,		/**< AUD DTMF FIFO empty */
} aud_adc_status_mask_t;

typedef enum {
	AUD_DACR_NEAR_FULL_MASK = 1,			/**< AUD DAC right channel fifo near full */
	AUD_DACL_NEAR_FULL_MASK = 1 << 1,		/**< AUD DAC left channel fifo near full */
	AUD_DACR_NEAR_EMPTY_MASK = 1 << 4,		/**< AUD DAC right channel fifo near empty */
	AUD_DACL_NEAR_EMPTY_MASK = 1 << 5,		/**< AUD DAC left channel fifo near empty */
	AUD_DACR_FIFO_FULL_MASK = 1 << 8,		/**< AUD DAC right channel fifo full */
	AUD_DACL_FIFO_FULL_MASK = 1 << 9,		/**< AUD DAC left channel fifo full */
	AUD_DACR_FIFO_EMPTY_MASK = 1 << 12,		/**< AUD DAC right channel fifo empty */
	AUD_DACL_FIFO_EMPTY_MASK = 1 << 13,		/**< AUD DAC left channel fifo empty */
} aud_dac_status_mask_t;

/**
 * @}
 */

/**
 * @brief AUD struct defines
 * @defgroup bk_api_aud_structs structs in AUD
 * @ingroup bk_api_aud
 * @{
 */

typedef struct {
	//aud_isr_id_t id;
	icu_int_src_t int_src;
	int_group_isr_t isr;
	//dma_dev_t dma_dev;
} aud_int_config_t;

typedef struct {
	/* audio_config */
	aud_adc_enable_t adc_enable;                    /**< AUD adc enable */
	aud_adc_line_enable_t line_enable;              /**< AUD line in enable */
	aud_dtmf_enable_t dtmf_enable;                  /**< AUD dtmf enable */

	/* dtmf_config0 */
	aud_dtmf_tone_pattern_t tone_pattern;           /**< AUD dtmf tone pattern */
	aud_dtmf_tone_mode_t tone_mode;                 /**< AUD dtmf tone mode */
	uint32_t tone_pause_time;                       /**< AUD dtmf tone pause time */
	uint32_t tone_active_time;                      /**< AUD dtmf tone active time */

	/* dtmf_config1 */
	uint32_t tone1_step;                            /**< AUD dtmf Tone1 frequency set  Step = F(KHz) * 8191 */
	aud_dtmf_tone_attu_t tone1_attu;                /**< AUD dtmf Tone1 damp set  0 ：-1 ：-15 dB  */
	aud_dtmf_tone_enable_t tone1_enable;            /**< AUD dtmf tone1 enable */

	/* dtmf_config2 */
	uint32_t tone2_step;                            /**< AUD dtmf Tone2 frequency set  Step = F(KHz) * 8191 */
	aud_dtmf_tone_attu_t tone2_attu;                /**< AUD dtmf Tone2 damp set  0 ：-1 ：-15 dB  */
	aud_dtmf_tone_enable_t tone2_enable;            /**< AUD dtmf tone2 enable */

	/* fifo_config */
	uint32_t dtmf_wr_threshold;                     /**< AUD dtmf write threshold value */
	aud_dtmf_int_t dtmf_int_enable;                 /**< AUD dtmf interrupt request enable */
	aud_loop_dtmf2dac_t loop_ton2dac;               /**< AUD dtmf to DAC loop test enable */
} aud_dtmf_config_t;


typedef struct {
	/* audio mic config */
	aud_mic_enable_t mic_config;

	/* audio_config */
	aud_adc_samp_rate_t samp_rate;                  /**< AUD adc sample rate */
	aud_adc_enable_t adc_enable;                    /**< AUD adc enable */
	aud_adc_line_enable_t line_enable;              /**< AUD line in enable */
	aud_dtmf_enable_t dtmf_enable;                  /**< AUD dtmf enable */

	/* adc_config0 */
	uint32_t adc_hpf2_coef_B2;                      /**< AUD adc HPF2 coefficient B2 */
	aud_adc_hpf_bypass_t adc_hpf2_bypass_enable;    /**< AUD adc hpf2 disable */
	aud_adc_hpf_bypass_t adc_hpf1_bypass_enable;    /**< AUD adc hpf1 disable */
	uint32_t adc_set_gain;                          /**< AUD adc gain set */
	aud_adc_samp_edge_t adc_samp_edge;              /**< ADC data sampling clock edge select  rising/falling */

	/* adc_config1 */
	uint32_t adc_hpf2_coef_B0;                      /**< AUD adc HPF2 coefficient B0 */
	uint32_t adc_hpf2_coef_B1;                      /**< AUD adc HPF2 coefficient B1 */

	/* adc_config2 */
	uint32_t adc_hpf2_coef_A0;                      /**< AUD adc HPF2 coefficient A0 */
	uint32_t adc_hpf2_coef_A1;                      /**< AUD adc HPF2 coefficient A1 */

	/* fifo_config */
	uint32_t dtmf_wr_threshold;                     /**< AUD dtmf write threshold value */
	uint32_t adcl_wr_threshold;                     /**< AUD adcl write threshold value */
	aud_dtmf_int_t dtmf_int_enable;                 /**< AUD dtmf interrupt request enable */
	aud_adcl_int_t adcl_int_enable;                 /**< AUD adcl interrupt request enable */
	//aud_loop_dtmf2dac_t loop_ton2dac;             /**< AUD dtmf to DAC loop test enable */
	aud_loop_adc2dac_t loop_adc2dac;                /**< AUD adc to DAC loop test enable */

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
} aud_adc_config_t;

#define DEFAULT_AUD_ADC_CONFIG() {                                \
        .mic_config = AUD_MIC_DISABLE,                            \
        .samp_rate = AUD_ADC_SAMP_RATE_8K,                        \
        .adc_enable = AUD_ADC_DISABLE,                            \
        .line_enable = AUD_ADC_LINE_DISABLE,                      \
        .dtmf_enable = AUD_DTMF_DISABLE,                          \
        .adc_hpf2_coef_B2 = 0,                                    \
        .adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE,      \
        .adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE,      \
        .adc_set_gain = 0x2d,                                     \
        .adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING,                \
        .adc_hpf2_coef_B0 = 0,                                    \
        .adc_hpf2_coef_B1 = 0,                                    \
        .adc_hpf2_coef_A0 = 0,                                    \
        .adc_hpf2_coef_A1 = 0,                                    \
        .dtmf_wr_threshold = 8,                                   \
        .adcl_wr_threshold = 8,                                   \
        .dtmf_int_enable = AUD_DTMF_INT_DISABLE,                  \
        .adcl_int_enable = AUD_ADCL_INT_DISABLE,                  \
        .loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE,                 \
        .agc_noise_thrd = 101,                                    \
        .agc_noise_high = 101,                                    \
        .agc_noise_low = 160,                                     \
        .agc_noise_min = 1,                                       \
        .agc_noise_tout = 0,                                      \
        .agc_high_dur = 3,                                        \
        .agc_low_dur = 3,                                         \
        .agc_min = 1,                                             \
        .agc_max = 4,                                             \
        .agc_ng_method = AUD_AGC_NG_METHOD_MUTE,                  \
        .agc_ng_enable = AUD_AGC_NG_DISABLE,                      \
        .agc_decay_time = AUD_AGC_DECAY_TIME_128,                 \
        .agc_attack_time = AUD_AGC_ATTACK_TIME_128,               \
        .agc_high_thrd = 18,                                      \
        .agc_low_thrd = 0,                                        \
        .agc_iir_coef = AUD_AGC_IIR_COEF_1_1024,                  \
        .agc_enable = AUD_AGC_DISABLE,                            \
        .manual_pga_value = 0,                                    \
        .manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE,          \
    }


typedef struct {
	/* audio_config */
	aud_dac_enable_t dac_enable;						/**< AUD dac enable */
	aud_dac_samp_rate_t samp_rate;						/**< AUD dac sample rate */
	aud_dac_chl_enable_t dac_chl;						/**< AUD dac channel */
	aud_dac_work_mode_t work_mode;						/**< AUD dac work mode */

	/* dac_config0 */
	uint16_t dac_hpf2_coef_B2;							/**< AUD dac HPF2 coefficient B2 */
	aud_dac_hpf_bypass_t dac_hpf2_bypass_enable;		/**< AUD dac hpf2 disable */
	aud_dac_hpf_bypass_t dac_hpf1_bypass_enable;		/**< AUD dac hpf1 disable */
	uint16_t dac_set_gain;								/**< AUD dac gain set */
	aud_dac_clk_invert_t dac_clk_invert;				/**< AUD dac output clock edge select */

	/* dac_config1 */
	uint16_t dac_hpf2_coef_B0;							/**< AUD dac HPF2 coefficient B0 */
	uint16_t dac_hpf2_coef_B1;							/**< AUD dac HPF2 coefficient B1 */

	/* dac_config2 */
	uint16_t dac_hpf2_coef_A1;							/**< AUD dac HPF2 coefficient A1 */
	uint16_t dac_hpf2_coef_A2;							/**< AUD dac HPF2 coefficient A2 */

	/* fifo_config */
	uint8_t dacr_rd_threshold;							/**< AUD dacl read threshold value */
	uint8_t dacl_rd_threshold;							/**< AUD dacr read threshold value */
	uint8_t dacr_int_enable;							/**< AUD dacr interrupt request enable */
	uint8_t dacl_int_enable;							/**< AUD dacl interrupt request enable */

	/* extend_config */
	aud_dac_filt_enable_t dac_filt_enable;				/**< AUD dac filter enable */
} aud_dac_config_t;

#define DEFAULT_AUD_DAC_CONFIG() {                                \
        .dac_enable = AUD_DAC_DISABLE,                            \
        .samp_rate = AUD_DAC_SAMP_RATE_8K,                        \
        .dac_chl = AUD_DAC_CHL_L_ENABLE,                          \
        .work_mode = AUD_DAC_WORK_MODE_DIFFEN,                    \
        .dac_hpf2_coef_B2 = 0x3A22,                               \
        .dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE,      \
        .dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE,      \
        .dac_set_gain = 0x15,                                     \
        .dac_clk_invert = AUD_DAC_CLK_INVERT_RISING,              \
        .dac_hpf2_coef_B0 = 0x3A22,                               \
        .dac_hpf2_coef_B1 = 0x8BBF,                               \
        .dac_hpf2_coef_A1 = 0x751C,                               \
        .dac_hpf2_coef_A2 = 0xC9E6,                               \
        .dacr_rd_threshold = 0x4,                                 \
        .dacl_rd_threshold = 0x4,                                 \
        .dacr_int_enable = 0x0,                                   \
        .dacl_int_enable = 0x0,                                   \
        .dac_filt_enable = AUD_DAC_FILT_DISABLE,                  \
    }

typedef struct {
	int32_t flt0_A1;
	int32_t flt0_A2;
	int32_t flt0_B0;
	int32_t flt0_B1;
	int32_t flt0_B2;

	int32_t flt1_A1;
	int32_t flt1_A2;
	int32_t flt1_B0;
	int32_t flt1_B1;
	int32_t flt1_B2;

	int32_t flt2_A1;
	int32_t flt2_A2;
	int32_t flt2_B0;
	int32_t flt2_B1;
	int32_t flt2_B2;

	int32_t flt3_A1;
	int32_t flt3_A2;
	int32_t flt3_B0;
	int32_t flt3_B1;
	int32_t flt3_B2;
} aud_eq_config_t;


/**
 * @}
 */


#ifdef __cplusplus
}
#endif
