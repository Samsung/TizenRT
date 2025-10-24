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
 * @brief AUD defines
 * @defgroup bk_api_aud_defs macos
 * @ingroup bk_api_aud
 * @{
 */

#define BK_ERR_AUD_DMIC_BASE                (BK_ERR_AUD_BASE - 30)
#define BK_ERR_AUD_DMIC_NOT_INIT            (BK_ERR_AUD_DMIC_BASE - 1) /**< audio dmic not init */

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
	AUD_DMIC_INT_DISABLE = 0,    /**< AUD DTMF interrupt disable */
	AUD_DMIC_INT_ENABLE,         /**< AUD DTMF interrupt enable */
	AUD_DMIC_INT_MAX,
} aud_dmic_int_t;

typedef enum {
	AUD_LOOP_DMIC2DAC_DISABLE = 0,    /**< ADC to DAC loop test disable */
	AUD_LOOP_DMIC2DAC_ENABLE,         /**< ADC to DAC loop test enable */
	AUD_LOOP_DMIC2DAC_MAX,
} aud_loop_dmic2dac_t;

typedef enum {
	AUD_DMIC_CHL_L = 0,    /**< enable dac left channel */
	AUD_DMIC_CHL_R,        /**< enable dac right channel */
	AUD_DMIC_CHL_LR,       /**< enable dac left and right channel */
	AUD_DMIC_CHL_MAX,
} aud_dmic_chl_t;

typedef enum {
	AUD_DMIC_CIC_SEL_16 = 0,
	AUD_DMIC_CIC_SEL_32,
	AUD_DMIC_CIC_SEL_64,
	AUD_DMIC_CIC_SEL_RESERVED,
	AUD_DMIC_CIC_SEL_MAX,
} aud_dmic_cic_sel_t;

typedef enum {
	AUD_DMIC_APLL_SEL_26M = 0,
	AUD_DMIC_APLL_SEL_APLL,
	AUD_DMIC_APLL_SEL_MAX,
} aud_dmic_apll_sel_t;

typedef enum {
	/* fifo status */
	AUD_DMIC_NEAR_FULL_MASK = 1 << 20,     /**< AUD DMIC FIFO near full */
	AUD_DMIC_NEAR_EMPTY_MASK = 1 << 21,    /**< AUD DMIC FIFO near empty */
	AUD_DMIC_FIFO_FULL_MASK = 1 << 22,     /**< AUD DMIC FIFO full */
	AUD_DMIC_FIFO_EMPTY_MASK = 1 << 23,    /**< AUD DMIC FIFO empty */
} aud_dmic_status_mask_t;

/**
 * @}
 */

/**
 * @brief audio dmic struct defines
 * @defgroup bk_api_aud_structs structs in AUD
 * @ingroup bk_api_aud
 * @{
 */

typedef struct {
	aud_dmic_chl_t dmic_chl;    /**< AUD DMIC channel */
	uint32_t samp_rate;         /**< AUD DMIC sample rate */
} aud_dmic_config_t;

#define DEFAULT_AUD_DMIC_CONFIG() {    \
        .dmic_chl = AUD_DMIC_CHL_L,    \
        .samp_rate = 32000,            \
    }

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
