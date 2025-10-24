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
#include <common/bk_include.h>
#include <driver/gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief GPIO API
 * @defgroup bk_api_gpio GPIO API group
 * @{
 */


/**
 * @brief     Init the GPIO driver
 *
 * This API init the resoure common to all GPIO channels:
 *   - Init GPIO driver control memory
 *
 * This API should be called before any other GPIO APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_gpio_driver_init(void);


/**
 * @brief     Deinit the GPIO driver
 *
 * This API free all resource related to GPIO and power down all GPIO channels.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_gpio_driver_deinit(void);


/**
 * @brief     enable GPIO output mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_output(gpio_id_t gpio_id);


/**
 * @brief     disable GPIO output mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_output(gpio_id_t gpio_id);


/**
 * @brief     enable GPIO input mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_input(gpio_id_t gpio_id);


/**
 * @brief     disable GPIO input mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_input(gpio_id_t gpio_id);


/**
 * @brief     enable GPIO pull mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_pull(gpio_id_t gpio_id);


/**
 * @brief     disable gpio pull mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_pull(gpio_id_t gpio_id);


/**
 * @brief     set GPIO as pull up mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_pull_up(gpio_id_t gpio_id);


/**
 * @brief     set GPIO as pull down mode
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_pull_down(gpio_id_t gpio_id);


/**
 * @brief     Config the GPIO mode
 *
 * This API config GPIO's mode
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_INVALID_MODE: invalid GPIO's io_mode/pull_mode
 *    - BK_ERR_GPIO_INTERNAL_USED:GPIO was map to another device
 *    - others: other errors.
 */
bk_err_t bk_gpio_set_config(gpio_id_t gpio_id, const gpio_config_t *config);


/**
 * @brief     Set the GPIO output high,
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_OUTPUT_MODE:GPIO not in output mode

 *    - others: other errors.
 */
bk_err_t bk_gpio_set_output_high(gpio_id_t gpio_id);

/**
 * @brief     Set the GPIO output low,
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_OUTPUT_MODE:GPIO not in output mode
 *    - others: other errors.
 */

bk_err_t bk_gpio_set_output_low(gpio_id_t gpio_id);

/**
 * @brief     Get the GPIO input value,
 *
 * This API get GPIO's input level: 0 :low_level 1:high_level.
 *
 * @return
 *    - input value
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bool bk_gpio_get_input(gpio_id_t gpio_id);

/**
 * @brief     Set the GPIO driver capacity.
 *
 * This API Set GPIO's output driver capacity which range is 0~3.
 *
 * @return
 *    - input value
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bool bk_gpio_set_capacity(gpio_id_t gpio_id, uint32 capacity);

/**
 * @brief     Config the GPIO intterrupt type mode  when use gpio intterrupt mode,
 *
 * This API config all GPIO channels' intterrupt mode, the mode included in gpio_int_type_t.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_INVALID_INT_TYPE: invalid GPIO int type
 *    - others: other errors.
 */
bk_err_t bk_gpio_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type);

/**
 * @brief     Enable GPIO intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - BK_ERR_GPIO_NOT_INPUT_MODE : GPIO is not input mode
 *    - others: other errors.
 */
bk_err_t bk_gpio_enable_interrupt(gpio_id_t id);

/**
 * @brief     Disable GPIO intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid GPIO channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_disable_interrupt(gpio_id_t id);

/**
 * @brief     Clear GPIO intterrupt.
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_clear_interrupt(gpio_id_t gpio_id);

/**
 * @brief     Register the interrupt service routine for GPIO channel
 *
 * This API regist gpio isr callback function.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_register_isr(gpio_id_t id, gpio_isr_t isr);

/**
 * @brief     Unregister the interrupt service routine for GPIO channel
 *
 * This API deregist gpio isr callback function.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_unregister_isr(gpio_id_t id);

#if CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
/**
 * @brief     Register the GPIO channel to wakeup source with select int type.
 *
 * This API regist gpio to wakeup source.
 * If the GPIO registered to wakeup source, the system at low voltage status
 * can be wake up by the GPIO with selected int_type, and hold on the system
 * at wakeup status some time.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - BK_ERR_GPIO_INVALID_INT_TYPE: invalid gpio int type
 *    - BK_ERR_GPIO_WAKESOURCE_OVER_MAX_CNT: too many gpio is register to wakeup source
 *      default max value is: CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT
 *    - others: other errors.
 */
bk_err_t bk_gpio_register_wakeup_source(gpio_id_t gpio_id, gpio_int_type_t int_type);

/**
 * @brief     Unregister the GPIO channel from wakeup source.
 *
 * This API unregist gpio from wakeup source.
 * If the GPIO has registered to wakeup source, this API will unregister the gpio
 * from wakeup source.
 * If bk_gpio_register_wakeup_source failed with BK_ERR_GPIO_WAKESOURCE_OVER_MAX_CNT,
 * it can call this API unregister one of gpio.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_unregister_wakeup_source(gpio_id_t gpio_id);

/**
 * @brief     Get the awakened GPIO ID.
 *
 * This API is used to get the GPIO_ID of the current wakeup.
 * If the GPIO ID is GPIO_NUM, please check that the registered interrupt wake up
 * GPIO ID is correct.And after the GPIO initialization is complete, the wake up ID
 * is registered
 *
 * @return
 *    - GPIO_ID: succeed
 *    - GPIO_NUM: error invalid gpio id
 *
 */
gpio_id_t bk_gpio_get_wakeup_gpio_id();

/**
 * @brief     Register the GPIO channel to keep status.
 *
 * This API regist gpio to keep status when enter low power or deepsleep.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT: too many gpio is register to keep status
 *      default max value is: CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT
 *    - others: other errors.
 */
bk_err_t bk_gpio_register_lowpower_keep_status(gpio_id_t gpio_id, const gpio_config_t *config);

/**
 * @brief     Unregister the GPIO channel from keep status.
 *
 * This API unregist gpio from keep status.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel
 *    - others: other errors.
 */
bk_err_t bk_gpio_unregister_lowpower_keep_status(gpio_id_t gpio_id);

/**
 * @brief using the gpio to control the external ldo
 *
 * control the external ldo,multi modules power on use one gpio control
 *
 * @attention
 * - This API is used to use the specific gpio(define in  GPIO_CTRL_LDO_MAP in gpio_map.h) control the external ldo(multi modules power on use one gpio control)
 *
 * @param
 * -module:gpio ctrl ldo module name
 * -gpio_id:gpio id
 * -value:gpio output state 
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_gpio_ctrl_external_ldo(gpio_ctrl_ldo_module_e module,gpio_id_t gpio_id,gpio_output_state_e value);

#else
/**
 * @brief     Register save all gpio reg value
 *
 * This API save all gpio reg value function.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_gpio_reg_save(uint32_t*  gpio_cfg);
/**
 * @brief     Register restore all gpio reg value
 *
 * This API restore all gpio reg value function.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_gpio_reg_restore(uint32_t*  gpio_cfg);
/**
 * @brief     Register configue the gpio wakeup value
 *
 * This API configue the gpio wakeup value function.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_gpio_wakeup_enable(int64_t index, uint64_t type_l, uint64_t type_h);

/**
 * @brief Register clear wake up interrupt
 *
 * This API clear wake up interrupt function.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_gpio_wakeup_interrupt_clear();

#endif

#if CONFIG_PM
/**
 * @brief Set GPIO wakeup config
 *
 * This API set GPIO wakeup index and int_type.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_GPIO_CHAN_ID: invalid gpio channel number
 *    - BK_ERR_GPIO_INVALID_INT_TYPE: invalid gpio int type
 *    - others: other errors.
 */
bk_err_t bk_gpio_wakeup_config_set(gpio_id_t id, gpio_int_type_t type);
#endif

#ifdef __cplusplus
}
#endif
