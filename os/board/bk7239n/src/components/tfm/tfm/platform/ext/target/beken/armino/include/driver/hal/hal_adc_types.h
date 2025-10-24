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

#include <common/bk_err.h>
#include <driver/hal/hal_gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_ADC_STATIS 1


/**
 * @brief ADC defines
 * @addtogroup bk_api_adc_defs ADC API group
 * @{
 */
//TODO fix it
#define ADC_ACURACY_10_BIT	0
#define ADC_ACURACY_12_BIT	1

#define ADC_READ_SEMAPHORE_WAIT_TIME	1000 //TODO fix it

#define ADC_SRC_DCO_CLK		120000000
#define ADC_SRC_DPLL_CLK	240000000
#define ADC_SRC_26M_CLK		26000000
#define ADC_SRC_32M_CLK		32000000

#define ADC_FIFO_LEVEL_MAX         32


/**
 * @}
 */


/**
 * @brief ADC enum defines
 * @defgroup bk_api_adc_enum ADC enums
 * @ingroup bk_api_adc
 * @{
 */
typedef enum {
	ADC_SCLK_DCO = 0,  /**< ADC source clock dco */
	ADC_SCLK_XTAL_26M, /**< ADC source clock xtal 26M */
	ADC_SCLK_DPLL, /**< ADC source clock dpll */
	ADC_SCLK_32M, /**< ADC source clock 32M */
	ADC_SCLK_NONE, /**< ADC has not such source clock*/
} adc_src_clk_t;

typedef enum {
	ADC_SLEEP_MODE = 0, /**< ADC sleep mode */
	ADC_SINGLE_STEP_MODE, /**< ADC single step mode */
	ADC_SOFTWARE_CONTRL_MODE,     /**< ADC software control mode */
	ADC_CONTINUOUS_MODE,/**< ADC continuous mode */
	ADC_NONE_MODE,
} adc_mode_t;

typedef enum {
	ADC_1 = 1,
	ADC_2,
	ADC_3,
	ADC_4,
	ADC_5,
	ADC_6,
	ADC_7,
	ADC_8,
	ADC_9,
	ADC_10,
	ADC_11,
	ADC_12,
	ADC_13,
	ADC_MAX,
} adc_chan_t;

#if ADC_ACURACY_10_BIT
typedef enum {
	ADC_SATURATE_MODE_NONE = 0,
	ADC_SATURATE_MODE_0,
	ADC_SATURATE_MODE_1,
} adc_saturate_mode_t;
#else
typedef enum {
	ADC_SATURATE_MODE_NONE = 0,
	ADC_SATURATE_MODE_0,
	ADC_SATURATE_MODE_1,
	ADC_SATURATE_MODE_2,
	ADC_SATURATE_MODE_3,
} adc_saturate_mode_t;
#endif

/**
 * @}
 */

/**
 * @brief ADC struct defines
 * @defgroup bk_api_adc_structs in ADC
 * @ingroup bk_api_adc
 * @{
 */

typedef struct {
	uint32_t clk;           /**< ADC sample clock division: adc_clk=clk/[2*(pre_div +1)] */
	uint32_t sample_rate;   /**< ADC sample rate, every period ouput 16 adc_clks in continuous mode period = (16+samp_rate)*adc_clk*/
	uint32_t adc_filter;    /**< ADC filter: the output data rate = period/(adc_filter +1) */
	uint32_t steady_ctrl;   /**< ADC steady_ctrl : steady time = (steady_ctrl +1)*8 adc_clk  */
	adc_mode_t adc_mode;    /**< ADC mode set */
	adc_src_clk_t src_clk;  /**< ADC source clcok choose*/
	adc_chan_t chan;        /**< ADC work channel*/
	adc_saturate_mode_t saturate_mode; /**< ADC saturate_mode*/
    uint32_t is_open;       /**< ADC channel is open or not*/
    uint16_t *output_buf;   /**< ADC channel output buffer*/
    int32_t output_buf_len; /**< ADC channel output buffer length*/
} adc_config_t;


typedef struct {
	adc_chan_t adc_chan;
	gpio_id_t gpio_id;
	gpio_dev_t gpio_dev;
} adc_gpio_map_t;

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

