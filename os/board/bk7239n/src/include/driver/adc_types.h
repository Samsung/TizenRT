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

#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ADC defines
 * @defgroup bk_api_adc_defs macos
 * @ingroup bk_api_adc
 * @{
 */
#define BK_ERR_ADC_NOT_INIT              (BK_ERR_ADC_BASE - 1) /**< ADC driver not init */
#define BK_ERR_ADC_INVALID_CHAN          (BK_ERR_ADC_BASE - 2) /**< ADC chan number is invalid */
#define BK_ERR_ADC_BUSY			 (BK_ERR_ADC_BASE - 3) /**< ADC channel is busy */
#define BK_ERR_ADC_INVALID_MODE	         (BK_ERR_ADC_BASE - 4) /**< ADC invalid mode */
#define BK_ERR_ADC_CHAN_NOT_INIT         (BK_ERR_ADC_BASE - 5) /**< ADC chan not init */
#define BK_ERR_ADC_INVALID_SCLK_MODE     (BK_ERR_ADC_BASE - 6) /**< ADC no such source clock */
#define BK_ERR_ADC_INIT_MUTEX            (BK_ERR_ADC_BASE - 7) /**< ADC mutex lock */
#define BK_ERR_ADC_GET_READ_SEMA         (BK_ERR_ADC_BASE - 8) /**< ADC get semaphore error */
#define BK_ERR_ADC_INIT_READ_SEMA        (BK_ERR_ADC_BASE - 9) /**< ADC read data error */
#define BK_ERR_ADC_SIZE_TOO_BIG          (BK_ERR_ADC_BASE - 10) /**< ADC read size is bigger than adc driver buffer size */
#define BK_ERR_ADC_INSUFFICIENT_MEM      (BK_ERR_ADC_BASE - 11) /**< ADC out of memory */
#define BK_ERR_ADC_DEINIT_MUTEX          (BK_ERR_ADC_BASE - 12) /**< ADC mutex deinit failed */
#define BK_ERR_ADC_DEINIT_READ_SEMA      (BK_ERR_ADC_BASE - 13) /**< ADC read sync deinit failed */
#define BK_ERR_ADC_DPLL_NOT_SUPPORTED    (BK_ERR_ADC_BASE - 14) /**< ADC DPLL clock source not supported */

#ifndef CONFIG_ADC_BUF_SIZE
#define CONFIG_ADC_BUF_SIZE              32
#endif

#ifndef CONFIG_ADC_SAMPLE_CAPACITY_CNT
#define CONFIG_ADC_SAMPLE_CAPACITY_CNT   32
#endif

/**
 * @brief ADC interrupt service routine
 */
typedef void (*adc_isr_t)(uint32_t param);
typedef void (*adc_isr_cb)(uint32_t param);


/**
 * @}
 */


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

