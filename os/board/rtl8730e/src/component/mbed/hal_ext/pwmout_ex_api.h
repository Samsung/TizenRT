/** mbed Microcontroller Library
  ******************************************************************************
  * @file    pwmout_ex_api.h
  * @author
  * @version V1.0.0
  * @brief   This file provides mbed pwm API
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
#ifndef MBED_PWMOUT_EX_API_H
#define MBED_PWMOUT_EX_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup pwm PWM
 *  @ingroup    hal
 *  @brief      pwm functions
 *  @{
 */

///@name Ameba Common
///@{
#if !defined(CONFIG_PLATFORM_8710C)
typedef void (*pwm_lim_callback_t)(void *, u8 dir);
typedef void (*pwm_period_callback_t)(void *);
#endif

#if defined(CONFIG_PLATFORM_8735B)
typedef void (*pwm_dma_callback_t)(void *);
#endif

/**
  * @brief  Set the start timing offset of the specified channel in seconds.
  * @param  obj: PWM object define in application software.
  * @param  seconds: The period value to be set in seconds.
  * @retval none
  */
void pwmout_startoffset(pwmout_t *obj, float seconds);

/**
  * @brief  Set the start timing offset of the specified channel in millseconds.
  * @param  obj: PWM object define in application software.
  * @param  ms: The period value to be set in millseconds.
  * @retval none
  */
void pwmout_startoffset_ms(pwmout_t *obj, int ms);

/**
  * @brief  Set the start timing offset of the specified channel in microseconds.
  * @param  obj: PWM object define in application software.
  * @param  us: The period value to be set in microseconds.
  * @retval none
  */
void pwmout_startoffset_us(pwmout_t *obj, int us);

//typedef struct pwmout_s pwmout_t;

/**
 *  @brief To enable the PWM period end interrupt.
 *
 *  @param obj: PWM object define in application software.
 *  @param callback The callback function. It will be called when the interrupt is accurred.
 *  @param arg The argument of the callback function.
 *  @param enable To enable(1) or disable(0) the interrupt. For interrupt disable, the arguments
 *                'callback' & 'arg' are ignored.
 *  @returns void
 */
void pwmout_period_int(pwmout_t *obj, pwm_period_callback_t callback, u8 enable);

/**
 *  @brief To setup the PWM duty auto adjustment interrupt.
 *
 *  @param obj: PWM object define in application software.
 *  @param callback The callback function. It will be called when the interrupt is accurred.
 *  @param direction The bit map to enable/disable the interrupt. Bit 1 control the interrupt of duty duration
 *                reachs the up limit. Bit 0 control the interrupt of duty duration reachs the down limit.
 *  @param enable To enable(1) or disable(0) the interrupt. For interrupt disable, the arguments
 *                'callback' & 'arg' are ignored.
 *
 *  @returns void
 */
void pwmout_autoadj_int(pwmout_t *obj, pwm_lim_callback_t callback, u8 direction, u8 enable);

/**
 *  @brief To start the PWM duty auto-adjustment for duty duration increasing.
 *
 *  @param obj: PWM object define in application software.
 *  @param max_duty_us The up limit of the duty duration, in us.
 *  @param step_sz_us The step size of each duty duration increasing, in us.
 *  @param step_period_cnt The stay time of each duty duration increasing step, uint is PWM period.
 *
 *  @returns The result.
 */
void pwmout_autoadj_inc(pwmout_t *obj, u32 max_duty_us, u32 step_sz_us, u32 step_period_cnt);

/**
 *  @brief To start the PWM duty auto-adjustment for duty duration decreasing.
 *
 *  @param obj: PWM object define in application software.
 *  @param min_duty_us The up limit of the duty duration, in us.
 *  @param step_sz_us The step size of each duty duration increasing, in us.
 *  @param step_period_cnt The stay time of each duty duration increasing step, uint is PWM period.
 *
 *  @returns The result.
 */
void pwmout_autoadj_dec(pwmout_t *obj, u32 min_duty_us, u32 step_sz_us, u32 step_period_cnt);

/**
  *  @brief  To Eable the PWM function.
  *  @param obj: PWM object define in application software.
  *
  *  @returns void
  */
void pwmout_start(pwmout_t *obj);

/**
  *  @brief  To Disable the PWM function.
  *  @param obj: PWM object define in application software.
  *
  *  @returns void
  */
void pwmout_stop(pwmout_t *obj);

/**
  * @brief  Eable the Multible PWM function/registers of the specified pin with default parameters in same time.
  * @param  pin_ctrl: the pinname of specified channel to be set. Bit 0 control the PWM0. Bit 1 control the PWM1.
  *                          Bit 2 control the PWM2...etc
  *
  *  @returns void
  */
void pwmout_multi_start(u8 pin_ctrl);


#if defined(CONFIG_PLATFORM_8735B)

/**
  * @brief  Use DMA to set the pulse width of the specified channel in microseconds.
  * @param  obj: PWM object define in application software.
  * @param  ptxbuf: The array buffer describing period and duty that ready to send.
  * @param  len: The amount of data ready to send, count in bytes.
  * @param  mode: Different dma mode, 5: only change duty, 6: only change period, 7: change duty and period simultaneously.
  * @retval none
  */

void pwmout_dma(pwmout_t *obj, uint8_t *ptx_buf, uint32_t len, int mode, pwm_dma_callback_t callback);

/**
  * @brief  Enable pwm complementary mode, while setting DandBand value.
  * @param  obj: PWM object define in application software.
  * @param  group: chose pwm pair to be complementary.
  * @param  DB_value: The value to set rising and falling edge delay time, the delay time will be (1/sclk)*DB_value.
  * @param  en: To enable or disable pwm dma mode in chosen pwm pair.
  * @retval none
  */

void pwmout_complementary(pwmout_t *obj, uint8_t group, uint8_t DB_value, bool en);

#endif


///@}

/*\@}*/

#ifdef __cplusplus
}
#endif

#endif


