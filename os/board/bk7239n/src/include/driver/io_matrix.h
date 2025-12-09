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
#include <driver/io_matrix_types.h>
#ifdef CONFIG_IO_MATRIX_VER2_0
#include <driver/gpio_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief IO_MATRIX API
 * @defgroup bk_api_iomx IO_MATRIX API group
 * @{
 */

/**
 * @brief     Init the IO_MATRIX driver
 *
 * This API init the resoure common
 *
 * @attention 1. This API should be called before any other IO_MATRIX APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_iomx_driver_init(void);

/**
 * @brief     Deinit the IO_MATRIX driver
 *
 * This API free all resource related to IO_MATRIX and power down IO_MATRIX.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_iomx_driver_deinit(void);

/**
 * @brief     Get gpio function code
 *
 * This API gets gpio code
 *
 * @param gpio_id: gpio index
 *
 * @return
 *    - IOMX_CODE_T func_code
 */
uint32_t bk_iomx_get_gpio_func_code(uint32_t gpio_id);

/**
 * @brief     Set gpio function code
 *
 * This API sets gpio code
 *
 * @param gpio_id: gpio index
 * @param func_code: function code
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_iomx_set_gpio_func(uint32_t gpio_id, IOMX_CODE_T func_code);

#ifdef CONFIG_IO_MATRIX_VER2_0
/**
 * @brief     Direct set GPIO(IO_MATRIX) config value.
 *
 * @NOTES     Please be care to use this API,unless you know the detail mean,
 *            because it will set the GPIO config value.
 *            Sometimes the special GPIO needs to re-used by one more owner,
 *            after the second owner re-used it, it should restore the config value.
 *            Before re-use a GPIO, the owner can call bk_iomx_get_value
 *            and bakup it, after re-used finish, it call bk_iomx_set_value
 *            to restore the prevous value.
 */
void bk_iomx_set_value(gpio_id_t id, uint32_t v);


/**
 * @brief     get GPIO(IO_MATRIX) config value.
 */
uint32_t bk_iomx_get_value(gpio_id_t id);

/**
 * @brief     enable GPIO(IO_MATRIX) pull mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_enable_pull(gpio_id_t gpio_id);

/**
 * @brief     disable gpio(IO_MATRIX) pull mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_disable_pull(gpio_id_t gpio_id);

/**
 * @brief     set GPIO(IO_MATRIX) as pull up mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_pull_up(gpio_id_t gpio_id);

/**
 * @brief     set GPIO(IO_MATRIX) as pull down mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_pull_down(gpio_id_t gpio_id);

/**
 * @brief     Set the GPIO(IO_MATRIX) output high,
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_OUTPUT_MODE:GPIO not in output mode

 *    - others: other errors.
 */
bk_err_t bk_iomx_output_high(gpio_id_t gpio_id);

/**
 * @brief     Set the GPIO(IO_MATRIX) output low,
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_OUTPUT_MODE:GPIO not in output mode
 *    - others: other errors.
 */
bk_err_t bk_iomx_output_low(gpio_id_t gpio_id);

/**
 * @brief     Get the GPIO(IO_MATRIX) input value,
 *
 * This API get GPIO's input level: 0 :low_level 1:high_level.
 *
 * @return
 *    - input value
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bool bk_iomx_get_input(gpio_id_t gpio_id);

/**
 * @brief     Get the GPIO(IO_MATRIX) output value,
 *
 * This API get GPIO's input level: 0 :low_level 1:high_level.
 *
 * @return
 *    - input value
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bool bk_iomx_get_output(gpio_id_t gpio_id);

/**
 * @brief     Set the GPIO(IO_MATRIX) driver capacity.
 *
 * This API Set GPIO's output driver capacity which range is 0~3.
 *
 * @return
 *    - input value
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bk_err_t bk_iomx_set_capacity(gpio_id_t gpio_id, uint32 capacity);

/**
 * @brief     Register the interrupt service routine for GPIO(IO_MATRIX) channel
 *
 * This API regist gpio isr callback function.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_register_isr(gpio_id_t gpio_id, gpio_isr_t isr);

/**
 * @brief     Register the interrupt service routine for GPIO(IO_MATRIX) channel with private data
 *
 * This API regist gpio isr callback function with private data.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_register_isr_ex(gpio_id_t gpio_id, gpio_isr_priv_t isr, void *priv);

/**
 * @brief     Enable GPIO(IO_MATRIX) intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bk_err_t bk_iomx_enable_interrupt(gpio_id_t gpio_id);

/**
 * @brief     Disable GPIO(IO_MATRIX) intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_disable_interrupt(gpio_id_t gpio_id);

/**
 * @brief     Clear GPIO(IO_MATRIX) intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_iomx_clear_interrupt(gpio_id_t gpio_id);

/**
 * @brief     Config the GPIO(IO_MATRIX) intterrupt type mode  when use gpio intterrupt mode,
 *
 * This API config all GPIO channels' intterrupt mode, the mode included in gpio_int_type_t.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_INVALID_INT_TYPE: invalid GPIO int type
 *    - others: other errors.
 */
bk_err_t bk_iomx_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type);
#endif
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

