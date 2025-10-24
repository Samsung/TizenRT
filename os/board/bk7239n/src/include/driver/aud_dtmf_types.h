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
#include <driver/aud_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief audio dtmf defines
 * @defgroup bk_api_aud_defs macos
 * @ingroup bk_api_aud
 * @{
 */

#define BK_ERR_AUD_DTMF_BASE                (BK_ERR_AUD_BASE - 20)
#define BK_ERR_AUD_DTMF_NOT_INIT            (BK_ERR_AUD_DTMF_BASE - 1) /**< audio dtmf not init */

/**
 * @}
 */

/**
 * @brief audio dtmf enum defines
 * @defgroup bk_api_aud_enum AUD enums
 * @ingroup bk_api_aud
 * @{
 */

typedef enum {
	AUD_DTMF_INT_DISABLE = 0,    /**< AUD DTMF interrupt disable */
	AUD_DTMF_INT_ENABLE,         /**< AUD DTMF interrupt enable */
	AUD_DTMF_INT_MAX,
} aud_dtmf_int_t;

typedef enum {
	AUD_DTMF_DISABLE = 0,    /**< disable dtmf */
	AUD_DTMF_ENABLE,         /**< enable dtmf */
	AUD_DTMF_ENABLE_MAX,
} aud_dtmf_enable_t;

typedef enum {
	AUD_DTMF_TONE_PATTERN_DTMF = 0,    /**< 0: DTMF(Active_Time) + Zeros(Pause_Time) */
	AUD_DTMF_TONE_PATTERN_TONE,        /**< 1：Tone1(Active_Time) + Tone2(Pause_Time) */
	AUD_DTMF_TONE_PATTERN_MAX,
} aud_dtmf_tone_pattern_t;

typedef enum {
	AUD_DTMF_TONE_MODE_SIGNAL = 0,    /**< 0：signal mode */
	AUD_DTMF_TONE_MODE_CONTIUS,       /**< 1：continuous mode */
	AUD_DTMF_TONE_MODE_MAX,
} aud_dtmf_tone_mode_t;

typedef enum {
	AUD_LOOP_DTMF2DAC_DISABLE = 0,    /**< DTMF to DAC loop test disable */
	AUD_LOOP_DTMF2DAC_ENABLE,         /**< DTMF to DAC loop test enable */
	AUD_LOOP_DTMF2DAC_MAX,
} aud_loop_dtmf2dac_t;

typedef enum {
	AUD_DTMF_TONE_ATTU_MINUS_1 = 0,    /**< Tone1 damp set: -1 db */
	AUD_DTMF_TONE_ATTU_MINUS_2,        /**< Tone1 damp set: -2 db */
	AUD_DTMF_TONE_ATTU_MINUS_3,        /**< Tone1 damp set: -3 db */
	AUD_DTMF_TONE_ATTU_MINUS_4,        /**< Tone1 damp set: -4 db */
	AUD_DTMF_TONE_ATTU_MINUS_5,        /**< Tone1 damp set: -5 db */
	AUD_DTMF_TONE_ATTU_MINUS_6,        /**< Tone1 damp set: -6 db */
	AUD_DTMF_TONE_ATTU_MINUS_7,        /**< Tone1 damp set: -7 db */
	AUD_DTMF_TONE_ATTU_MINUS_8,        /**< Tone1 damp set: -8 db */
	AUD_DTMF_TONE_ATTU_MINUS_9,        /**< Tone1 damp set: -9 db */
	AUD_DTMF_TONE_ATTU_MINUS_10,       /**< Tone1 damp set: -10 db */
	AUD_DTMF_TONE_ATTU_MINUS_11,       /**< Tone1 damp set: -11 db */
	AUD_DTMF_TONE_ATTU_MINUS_12,       /**< Tone1 damp set: -12 db */
	AUD_DTMF_TONE_ATTU_MINUS_13,       /**< Tone1 damp set: -13 db */
	AUD_DTMF_TONE_ATTU_MINUS_14,       /**< Tone1 damp set: -14 db */
	AUD_DTMF_TONE_ATTU_MINUS_15,       /**< Tone1 damp set: -15 db */
	AUD_DTMF_TONE_ATTU_MINUS_MAX,
} aud_dtmf_tone_attu_t;

typedef enum {
	AUD_DTMF_TONE_DISABLE = 0,    /**< 0：disable tone */
	AUD_DTMF_TONE_ENABLE,         /**< 1：enable tone */
	AUD_DTMF_TONE_ENABLE_MAX,
} aud_dtmf_tone_enable_t;


typedef enum {
	/* fifo status */
	AUD_DTMF_NEAR_FULL_MASK = 1 << 3,      /**< AUD DTMF FIFO near full */
	AUD_DTMF_NEAR_EMPTY_MASK = 1 << 7,     /**< AUD DTMF FIFO near empty */
	AUD_DTMF_FIFO_FULL_MASK = 1 << 11,     /**< AUD DTMF FIFO full */
	AUD_DTMF_FIFO_EMPTY_MASK = 1 << 15,    /**< AUD DTMF FIFO empty */
} aud_dtmf_status_mask_t;


/**
 * @}
 */

/**
 * @brief audio dtmf struct defines
 * @defgroup bk_api_aud_structs structs in AUD
 * @ingroup bk_api_aud
 * @{
 */

typedef struct {
	/* dtmf_config0 */
	aud_dtmf_tone_pattern_t tone_pattern;    /**< AUD DTMF tone pattern */
	aud_dtmf_tone_mode_t tone_mode;          /**< AUD DTMF tone mode */
	uint32_t tone_pause_time;                /**< AUD DTMF tone pause time */
	uint32_t tone_active_time;               /**< AUD DTMF tone active time */

	/* dtmf_config1 */
	uint32_t tone1_step;                     /**< AUD DTMF Tone1 frequency set  Step = F(KHz) * 8191 */
	aud_dtmf_tone_attu_t tone1_attu;         /**< AUD DTMF Tone1 damp set  0 ：-1 ：-15 dB  */
//	aud_dtmf_tone_enable_t tone1_enable;     /**< AUD DTMF tone1 enable */

	/* dtmf_config2 */
	uint32_t tone2_step;                     /**< AUD DTMF Tone2 frequency set  Step = F(KHz) * 8191 */
	aud_dtmf_tone_attu_t tone2_attu;         /**< AUD DTMF Tone2 damp set  0 ：-1 ：-15 dB  */
//	aud_dtmf_tone_enable_t tone2_enable;     /**< AUD DTMF tone2 enable */
	aud_clk_t clk_src;                       /**< AUD DTMF clock select */
} aud_dtmf_config_t;


#define DEFAULT_AUD_DTMF_CONFIG() {                    \
        .tone_pattern = AUD_DTMF_TONE_PATTERN_DTMF,    \
        .tone_mode = AUD_DTMF_TONE_MODE_CONTIUS,       \
        .tone_pause_time = 20,                         \
        .tone_active_time = 40,                        \
        .tone1_step = 50,                              \
        .tone1_attu = 0,                               \
        .tone2_step = 100,                             \
        .tone2_attu = 0,                               \
        .clk_src = AUD_CLK_XTAL,                       \
    }

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
