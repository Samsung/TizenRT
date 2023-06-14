/** mbed Microcontroller Library
 ******************************************************************************
 * @file    analogin_api.h
 * @author
 * @version V1.0.0
 * @brief   This file provides following mbed Analog_in API
 ******************************************************************************
 * @attention
 ******************************************************************************
 * @attention
 *
 * Copyright(c) 2006 - 2022 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
#ifndef MBED_ANALOGIN_API_H
#define MBED_ANALOGIN_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup analog_in ANALOG_IN
 *  @ingroup    hal
 *  @brief      analog_in functions
 *  @{
 */

///@name Ameba Common
///@{

typedef struct analogin_s analogin_t;


/**
  * @brief  Initializes the ADC device, include clock/function/ADC registers.
  * @param  obj: adc object define in application software.
  * @param  pin: adc PinName according to pinmux spec.
  * @retval none
  */
void analogin_init(analogin_t *obj, PinName pin);

/**
  * @brief  Deinitializes the ADC device, include clock/function/ADC registers.
  * @param  obj: adc object define in application software.
  * @retval none
  */
void analogin_deinit(analogin_t *obj);

/**
  * @brief  Reads data from the specified adc channel fifo.
  * @param  obj: adc object define in application software.
  * @retval : adc channel data(float)
  */
float analogin_read(analogin_t *obj);

/**
  * @brief  Reads data from the specified adc channel fifo.
  * @param  obj: adc object define in application software.
  * @retval : 16bit adc channel data(int)
  */
uint16_t analogin_read_u16(analogin_t *obj);

/**
  * @brief Get channel voltage in mV according to conversion data from normal channel(0-5 & 7-9).
  * @param adc_data: ADC conversion data from normal channel.
  * @return Normal channel voltage in mV.
  */
uint32_t analogin_voltage_norm(uint16_t adc_data);

/**
  * @brief Get channel voltage in mV according to conversion data from VBAT channel(6).
  * @param adc_data: ADC conversion data from VBAT channel.
  * @return VBAT channel voltage in mV.
  */
uint32_t analogin_voltage_vbat(uint16_t adc_data);

///@}
/*\@}*/

#ifdef __cplusplus
}
#endif

#endif//MBED_ANALOGIN_API_H
