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
//
#pragma once
#include <common/bk_include.h>
#include <driver/hal/hal_gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief GPIO APIs
 * @addtogroup bk_api_gpio GPIO API group
 * @{
 */

/**
 * @brief GPIO defines
 * @defgroup bk_api_gpio_defs macos
 * @ingroup bk_api_gpio
 * @{
 */
#define BK_ERR_GPIO_CHAN_ID			((int)(BK_ERR_GPIO_BASE - 1)) /**< gpio channel number is invalid */
#define BK_ERR_GPIO_INVALID_MODE		((int)(BK_ERR_GPIO_BASE - 2)) /**< gpio mode is invalid  */
#define BK_ERR_GPIO_NOT_INPUT_MODE		((int)(BK_ERR_GPIO_BASE - 3)) /**< gpio is not in input mode */
#define BK_ERR_GPIO_SET_INVALID_FUNC_MODE	((int)(BK_ERR_GPIO_BASE - 4)) /**< gpio perial mode is invalid or was be set as 2nd func*/
#define BK_ERR_GPIO_INVALID_INT_TYPE		((int)(BK_ERR_GPIO_BASE - 5)) /**< gpio int type is invalid*/
#define BK_ERR_GPIOS_MAP_NONE			((int)(BK_ERR_GPIO_BASE - 6)) /**< gpio map device is none*/
#define BK_ERR_GPIO_NOT_OUTPUT_MODE		((int)(BK_ERR_GPIO_BASE - 7)) /**< gpio is not in output mode */
#define BK_ERR_GPIO_BITS_NUM			((int)(BK_ERR_GPIO_BASE - 8)) /**< gpio map bit num is error */
#define BK_ERR_GPIO_INTERNAL_USED		((int)(BK_ERR_GPIO_BASE - 9)) /**< gpio map was be map to a device */
#define BK_ERR_GPIO_MAP_PWMS_CHAN		((int)(BK_ERR_GPIO_BASE - 10)) /**< gpio map to pwm pwms' channel is invalid*/
#define BK_ERR_GPIO_INVALID_ID  		((int)(BK_ERR_GPIO_BASE - 11)) /**< gpio id is invalid */
#define BK_ERR_GPIO_WAKESOURCE_OVER_MAX_CNT 	((int)(BK_ERR_GPIO_BASE - 12)) /**< too much GPIO is register to wakeup source */
#define BK_ERR_ANA_GPIO_TYPE_NOT_SUPPORT 	((int)(BK_ERR_GPIO_BASE - 13)) /**< analog gpio wake source pin config is invalid>*/

/**
 * @brief GPIO interrupt service routine
 */
typedef void (*gpio_isr_t)(gpio_id_t gpio_id);


/**
 * @brief GPIO interrupt service routine,adaption TizenRT
 */
typedef void (*gpio_isr_priv_t)(gpio_id_t gpio_id, void *priv);

typedef struct {
	uint32_t id;
	gpio_int_type_t int_type;
	gpio_isr_t isr;
	uint32_t valid;
} gpio_wakeup_config_t;

#ifdef CONFIG_GPIO_CLOCK_PIN_SUPPORT
typedef enum {
	GPIO_CLK_PIN_SRC_ROSC = 0,
	GPIO_CLK_PIN_SRC_XTALL,    // external low freq xtal
	GPIO_CLK_PIN_SRC_XTALH,    // origin high freq xtal
	GPIO_CLK_PIN_SRC_XTALHD,   // divided high freq xtal
	GPIO_CLK_PIN_SRC_XTALH32K, // divided to 32k
	GPIO_CLK_PIN_SRC_DCO,
	GPIO_CLK_PIN_SRC_DPLL,
	GPIO_CLK_PIN_SRC_APLL,
	GPIO_CLK_PIN_SRC_INVALID,
} gpio_clk_pin_src_t;

typedef enum {
	GPIO_CLK_PIN_PORT_ANA = 0,
	GPIO_CLK_PIN_PORT_DIG,
	GPIO_CLK_PIN_PORT_INVALID,
} gpio_clk_pin_port_t;

typedef enum {
	GPIO_CLK_PIN_OPEN = 0,
	GPIO_CLK_PIN_CLOSE,
	GPIO_CLK_PIN_OP_INVALID,
} gpio_clk_pin_op_e;

typedef struct {
	gpio_clk_pin_op_e op;
	void *args;
} gpio_clk_pin_op_t;
#endif

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
