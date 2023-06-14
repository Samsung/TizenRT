/* mbed Microcontroller Library
 ******************************************************************************
 *
 * Copyright(c) 2006 - 2022 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#ifndef MBED_GPIO_EXT_API_H
#define MBED_GPIO_EXT_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup gpio_ex GPIO_EX
 *  @ingroup    hal
 *  @brief      gpio extended functions
 *  @{
 */

/**
  * @brief  Sets value to the selected output port pin.
  * @param  obj: gpio object define in application software.
  * @param  value: specifies the value to be written to the selected pin
  * 	This parameter can be one of the following values:
  *		@arg 0: Pin state set to low
  *		@arg 1: Pin state set to high
  * @retval none
  */
void gpio_direct_write(gpio_t *obj, BOOL value) ;

/**
  * @brief  Sets pull type to the selected pin.
  * @param  obj: gpio object define in application software.
  * @param  pull_type: this parameter can be one of the following values:
  *		@arg PullNone: HighZ, user can input high or low use this pin
  *		@arg OpenDrain(is OpenDrain output): no pull + OUT + GPIO[gpio_bit] = 0
  *		@arg PullDown: pull down
  *		@arg PullUp: pull up
  * @retval none
  */
void gpio_pull_ctrl(gpio_t *obj, PinMode pull_type);

/**
  * @brief  Deinitializes the GPIO device, include mode/direction/pull control registers.
  * @param  obj: gpio object define in application software.
  * @retval none
  */
void gpio_deinit(gpio_t *obj);

/**
  * @brief  Set GPIO direction.
  * @param  obj: gpio object define in application software.
  * @param  direction: this parameter can be one of the following values:
  *		@arg PIN_INPUT: this pin is input
  *		@arg PIN_OUTPUT: this pin is output
  * @retval none
  */
void gpio_change_dir(gpio_t *obj, PinDirection direction);

#if (defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1))  ||  (defined(CONFIG_PLATFORM_8195BLP) && (CONFIG_PLATFORM_8195BLP == 1))
///@name AmebaPro Only
///@{
/**
  * @brief  Sets schmitt trigger on/off control on the given GPIO pin .
  * @param  pin: PinName according to pinmux spec.
  * @param  ctrl  The on/off control:
  *                      - 0: disable the schmitt trigger.
  *                      - 1: enable the schmitt trigger.
  * @param  v_h3l1  The GPIO Group Voltage Select:
  *                      - 0: 1.8V.
  *                      - 1: 3V.
  * @retval none
  */
void gpio_schmitt_ctrl(PinName pin, BOOLEAN ctrl, uint8_t v_h3l1);
///@}
#endif //CONFIG_PLATFORM_8195BHP || CONFIG_PLATFORM_8195BLP

#ifdef __cplusplus
}
#endif

#endif
