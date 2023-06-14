/** mbed Microcontroller Library
  ******************************************************************************
  * @file    trng_api.h
  * @author
  * @version V1.0.0
  * @brief   This file provides following mbed TRNG API
  ******************************************************************************
  * @attention
  *
 * Copyright(c) 2007 - 2022 Realtek Corporation. All rights reserved.
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
#ifndef MBED_TRNG_API_H
#define MBED_TRNG_API_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup trng TRNG
 *  @ingroup    hal
 *  @brief      trng functions
 *  @{
 */

#if defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1)
///@name AmebaPro2 Only
///@{

u32 trng_get_rand(void);
void trng_init(void);
void trng_deinit(void);
///@}

#endif

#ifdef __cplusplus
}
#endif

#endif
