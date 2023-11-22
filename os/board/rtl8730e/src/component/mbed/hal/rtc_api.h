/** mbed Microcontroller Library
  ******************************************************************************
  * @file    rtc_api.h
  * @author
  * @version V1.0.0
  * @brief   This file provides mbed RTC API
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
#ifndef MBED_RTC_API_H
#define MBED_RTC_API_H

#include "device.h"
#if (defined(CONFIG_PLATFORM_8710C) && CONFIG_PLATFORM_8710C)
#if (defined(CONFIG_SYSTEM_TIME64) && CONFIG_SYSTEM_TIME64)
#include "time64.h"
#else
#include <time.h>
#endif

#undef time_t
#define time_t long long
#else
#include <time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup rtc RTC
 *  @ingroup    hal
 *  @brief      rtc functions
 *  @{
 */

#if (defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)) || \
	(defined(CONFIG_PLATFORM_8195BLP) && (CONFIG_PLATFORM_8195BLP == 1)) || \
	(defined(CONFIG_PLATFORM_8721D) && (CONFIG_PLATFORM_8721D == 1)) || \
	(defined(CONFIG_PLATFORM_8710C) && (CONFIG_PLATFORM_8710C == 1)) || \
	(defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1)) || \
	(defined(CONFIG_PLATFORM_AMEBAD2) && (CONFIG_PLATFORM_AMEBAD2 == 1)) || \
	(defined(CONFIG_PLATFORM_AMEBALITE) && (CONFIG_PLATFORM_AMEBALITE == 1)) || \
	(defined(CONFIG_PLATFORM_AMEBADPLUS) && (CONFIG_PLATFORM_AMEBADPLUS == 1)) || \
	(defined(CONFIG_PLATFORM_RTL8720F) && (CONFIG_PLATFORM_RTL8720F == 1))

///@name AmebaZ and AmebaPro and AmebaD and AmebaPro2 and AmebaZ2
///@{
typedef void (*alarm_irq_handler)(void);

struct alarm_s {
	uint32_t yday;//which day of the year
	uint32_t hour;
	uint32_t min;
	uint32_t sec;
};

typedef struct alarm_s alarm_t;
///@}
#endif //CONFIG_PLATFORM_8711B || CONFIG_PLATFORM_8195BLP || CONFIG_PLATFORM_8721D || CONFIG_PLATFORM_8735B || CONFIG_PLATFORM_8710C

///@name Ameba Common
///@{

/**
  * @brief  Initializes the RTC device, include clock, RTC registers and function.
  * @param  none
  * @retval none
  */
void rtc_init(void);

/**
  * @brief  Deinitializes the RTC device.
  * @param  none
  * @retval none
  */
void rtc_free(void);

/**
  * @brief  This function tells whether RTC is enabled or not.
  * @param  none
  * @retval 1: RTC is enable.
  * @retval 0: RTC is disable.
  */
int rtc_isenabled(void);

/**
  * @brief  Get current timestamp in seconds from RTC.
  * @param  none
  * @retval : The current timestamp in seconds which is calculated from 1970.1.1 00:00:00.
  */
time_t rtc_read(void);

/**
  * @brief  Set the specified timestamp in seconds to RTC.
  * @param  t: Seconds from 1970.1.1 00:00:00 to specified data and time which is to be set.
  * @retval  none
  */
void rtc_write(time_t t);

///@}
#if (defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)) || \
	(defined(CONFIG_PLATFORM_8195BLP) && (CONFIG_PLATFORM_8195BLP == 1)) || \
	(defined(CONFIG_PLATFORM_8721D) && (CONFIG_PLATFORM_8721D == 1)) || \
	(defined(CONFIG_PLATFORM_8710C) && (CONFIG_PLATFORM_8710C == 1)) || \
	(defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1)) || \
	(defined(CONFIG_PLATFORM_AMEBAD2) && (CONFIG_PLATFORM_AMEBAD2 == 1)) || \
  (defined(CONFIG_PLATFORM_AMEBADPLUS) && (CONFIG_PLATFORM_AMEBADPLUS == 1)) || \
	(defined(CONFIG_PLATFORM_AMEBALITE) && (CONFIG_PLATFORM_AMEBALITE == 1))

///@name AmebaZ and AmebaPro and AmebaPro2
///@{
/**
  * @brief  Set the specified RTC Alarm and interrupt.
  * @param  alarm: alarm object define in application software.
  * @param  alarmHandler:  alarm interrupt callback function.
  * @retval   status:
  *            - 1: success
  *            - Others: failure
  */
u32 rtc_set_alarm(alarm_t *alrm, alarm_irq_handler alarmHandler);

/**
  * @brief  Disable RTC Alarm and function.
  * @param  none
  * @retval   none
  */
void rtc_disable_alarm(void);
///@}
#endif //CONFIG_PLATFORM_8711B || CONFIG_PLATFORM_8195BLP || CONFIG_PLATFORM_8721D || CONFIG_PLATFORM_8735B || CONFIG_PLATFORM_8710C
///@}

#if (defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))
///@name AmebaPro2
///@{
/**
  * @brief  Initializes the RTC device with calibration setting, include clock, RTC registers and function.
  * @param  none
  * @retval none
  */
void rtc_init_with_cali(void);

/**
  * @brief  Set the specified RTC Alarm and interrupt.
  * @param  alarm: alarm object define in application software.
  * @param  alarmHandler:  alarm interrupt callback function.
  * @retval   status:
  *            - 1: success
  *            - Others: failure
  */
void rtc_set_alarm_time(time_t t, alarm_irq_handler alarmHandler);
/**
  * @brief  To set the compensation value of the RTC CLK.
  * @param  func_en The RTC compensation function enable. 1: Enable; 0:Disable
  * @retval   status:
  *            - 1: success
  *            - Others: failure
  */
u32 rtc_set_comp(u8 func_en);
///@}
#endif // CONFIG_PLATFORM_8735B

/*\@}*/

#ifdef __cplusplus
}
#endif

#endif
