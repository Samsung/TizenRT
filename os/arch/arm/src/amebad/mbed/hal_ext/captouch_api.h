/** mbed Microcontroller Library
  ******************************************************************************
  * @file    captouch_api.h
  * @author 
  * @version V1.0.0
  * @brief   This file provides following mbed CAPTOUCH API
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2006-2013 ARM Limited
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ****************************************************************************** 
  */
#ifndef MBED_CAPTOUCH_API_H
#define MBED_CAPTOUCH_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup captouch CAPTOUCH
 *  @ingroup    hal
 *  @brief      captouch functions
 *  @{
 */


#if ((defined (CONFIG_PLATFORM_8721D) && (CONFIG_PLATFORM_8721D == 1)))
///@name AmebaD Only
///@{


typedef struct captouch_s captouch_t;

/**
  * @brief  initializes the captouch device.
  * @param  obj: captouch object define in application software.
  * @retval none  
  */
void captouch_init (captouch_t *obj);

/**
  * @brief  deinitializes the captouch device.
  * @param  obj: captouch object define in application software.
  * @retval none  
  */
void captouch_deinit (captouch_t *obj);

/**
  * @brief  enable specified channel.
  * @param  obj: captouch object define in application software.
  * @param  Channel: specified channel index, which can be 0~3
  * @retval none
  */
void captouch_ch_enable(captouch_t *obj, u8 Channel);


/**
  * @brief  disable specified channel.
  * @param  obj: captouch object define in application software.
  * @param  Channel: specified channel index, which can be 0~3
  * @retval none
  */
void captouch_ch_disable(captouch_t *obj, u8 Channel);

/**
  * @brief  set captouch scan interval.
  * @param  obj: captouch object define in application software.
  * @param Interval: scan interval in units of ms
  * @retval none
  */
void captouch_set_scan_interval(captouch_t *obj, u32 Interval);

/**
  * @brief  read baseline data from specified channel.
  * @param  obj: captouch object define in application software.
  * @param  Channel: specified channel index, which can be 0~3
  * @retval baseline data
  */
u32 captouch_get_ch_baseline(captouch_t *obj, u8 Channel);

/**
  * @brief  read average data from specified channel.
   * @param  obj: captouch object define in application software.
  * @param  Channel: specified channel index, which can be 0~3
  * @retval average data
  */
u32 captouch_get_ch_data(captouch_t *obj, u8 Channel);

///@}

/*\@}*/

///@}
#endif //CONFIG_PLATFORM_8721D

#ifdef __cplusplus
}
#endif

#endif
