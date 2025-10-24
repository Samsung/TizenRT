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

#include <driver/aon_rtc_types.h>
#include "sys/time.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	  Init the AON_RTC driver
 *
 * This API init the resoure common:
 *	 - Init AON_RTC driver control memory
 *	 - Register ISR to NVIC/INTC/PLIC...
 *
 * @attention 1. This API should be called before any other AON_RTC APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_driver_init(void);

/**
 * @brief	  Denit the AON_RTC driver
 *
 * This API deinit the resoure common:
 *	 - Deinit AON_RTC driver control memory
 *	 - Deinit AON RTC HW:Disable AON RTC, unregister isr handler
 *
 * @attention 1. This API should be called after any other AON_RTC APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_driver_deinit(void);

//remove it, only one HW can't be used for many APPs.
#if (CONFIG_AON_RTC && (!CONFIG_AON_RTC_MANY_USERS))
/**
 * @brief	  Create a request to use the selected AON RTC timer.
 *			  For period timer, every tick will come an isr,
 *			  the caller should call bk_aon_rtc_destroy if it won't use it.
 *			  For once timer, only come one isr, after the isr finish,
 *			  the timer will be destoried by driver layer, the caller no needs to destroy it.
 *
 * This API init the resoure common:
 *	 - Init AON_RTC driver control memory
 *	 - Enable the AON RTC HW, and set the AON RTC counter to run.
 *
 * @param id 	which AON RTC will be selected.
 * @param tick  The tick value for the AON RTC timer, it will be set to AON RTC.
 * @param isr	The counter runs to setted value(tick), will call this isr
 * @param p_isr_param	Interaction with APP and Driver layer
 * @param period	Once timer or period timer
 *
 * @attention 1. This API should be called after bk_aon_rtc_driver_init.
*				 Driver layer will call bk_aon_rtc_driver_init,
 *				 but can't guarantee APP call this function after driver inited.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_create(aon_rtc_id_t id, uint32_t tick, bool period);

/**
 * @brief	  Dstory the requested AON RTC period timer which is created by the caller.
 *
 * This API destory the resoure common:
 *	 - Clear AON_RTC driver control memory.
 *	 - Disable the AON RTC HW, and set the AON RTC HW to default status.
 *
 * @attention 1. This API should be called after bk_aon_rtc_driver_init and bk_aon_rtc_create.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_destroy(aon_rtc_id_t id);

/**
 * @brief     Register the tick interrupt service routine for AON_RTC id
 *
 * @param id aon_rtc id
 * @param isr AON_RTC tick callback
 * @param param AON_RTC tick callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_register_tick_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param);

/**
 * @brief     Register the upper interrupt service routine for AON_RTC id
 *
 * @param id aon_rtc id
 * @param isr AON_RTC upper callback
 * @param param AON_RTC upper callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_register_upper_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param);
#endif

/**
 * @brief     Register an alarm to AON_RTC id timer
 *       !!! NOTES: the callback function is in ISR, so can't block too much time. F.E:can't call sleep/suspend; !!!
 *       !!! NOTES: freertos forbid call free/malloc in ISR which include suspend and resume. !!!
 * @id: register to which aon_rtc id
 * @param alarm_info_p includes below info:
 *   name_p: the name of the registered alarm 
 *   period_time: what's the alarm time, milliseconds * AON_RTC_MS_TICK_CNT
 *   period_cnt: 0xFFFFFFF means always period until user unregister it, or means period how many times
 *   callback: if the alarm comes, will call this callback
 *   param_p: the param will be used for callback 
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_alarm_register(aon_rtc_id_t id, alarm_info_t *alarm_info_p);

/**
 * @brief     Unregister an alarm from AON_RTC id timer
 *
 * @attention 1. Please don't unregister self in the alarm's callback.
 *
 * @param id: unregister the aon_rtc id alarm
 * @param name_p: the name of the registered alarm which will be unregistered.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_alarm_unregister(aon_rtc_id_t id, uint8_t *name_p);

#if (CONFIG_AON_RTC && (!CONFIG_AON_RTC_MANY_USERS))
/**
 * @brief     Register the rtc tick init
 *
 * @param param none
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_tick_init();

/**
 * @brief     Register rtc wakeup
 * @param    period: wake up timer
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_open_rtc_wakeup(uint32_t period);
#endif

/**
 * @brief  Get AON RTC current tick with 64 bits
 *	       AON RTC uses 32 Bits counter with 32K clock, the max time is about 36.4 hours.
 *         Set upper interrupt as the 0xFFFFFFFF ticks as one round.
 * @id: register to which aon_rtc id
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
uint64_t bk_aon_rtc_get_current_tick(aon_rtc_id_t id);

#if CONFIG_AON_RTC_DEBUG
/**
 * @brief     test rtc get/set tick(round *cycles count) consume time.
 *
 * @id: register to which aon_rtc id
 * @round:
 * @cycles:
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
void bk_aon_rtc_timing_test(aon_rtc_id_t id, uint32_t round, uint32_t cycles, uint32_t set_tick);
#endif

/**
 * @brief     dump aon rtc debug info
 *
 * @id: which aon_rtc id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
void bk_aon_rtc_dump(aon_rtc_id_t id);

/**
 * @brief     Get time of day
 *
 * @tv: Define parameters according to "structure timeval"
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_rtc_gettimeofday(struct timeval *tv, void *ptz);

/**
 * @brief     Set time of day
 *
 * @tv: Define parameters according to "structure timeval"
 * @tz: Define parameters according to "structure timezone"
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_rtc_settimeofday(const struct timeval *tv,const struct timezone *tz);

/**
 * @brief     Get Deepsleep duration seconds
 *
 * @deepsleep_seconds: The incoming formal parameter is assigned the deepsleep time
 * Unit : second
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_rtc_get_deepsleep_duration_seconds(uint32_t *deepsleep_seconds);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif



