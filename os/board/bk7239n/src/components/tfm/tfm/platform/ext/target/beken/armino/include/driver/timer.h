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
#include <driver/timer_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief Timer API
 * @defgroup bk_api_timer timer API group
 * @{
 */

/**
 * @brief     Init the timer driver
 *
 * This API init the resoure common to all timer channels:
 *   - Init timer driver control memory
 *
 * This API should be called before any other timer APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_timer_driver_init(void);

/**
 * @brief     Deinit the timer driver
 *
 * This API free all resource related to timer and power down all timer channels.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_timer_driver_deinit(void);

/**
 * @brief     Start the timer
 *
 * @attention 1. timer2 has been used for time cali
 * @attention 2. timer3 has been used for clock, but bk7271 use timer1
 * @attention 3. bk7271 has used timer0(10ms)
 *
 * @param timer_id the timer ID to be started
 * @param time_ms time delay value of the timer
 * @param callback the timer call back function
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_TIMER_NOT_INIT: timer driver not init
 *    - BK_ERR_TIMER_ID: timer id is invalid
 *    - BK_ERR_TIMER_IS_RUNNING: timer id is running
 *    - others: other errors.
 */
bk_err_t bk_timer_start(timer_id_t timer_id, uint32_t time_ms, timer_isr_t callback);

/**
 * @brief     Start the timer without set callback
 *
 * @param timer_id the timer ID to be started
 * @param timer_ms time delay value of the timer
 *
 * @return
 *    - BK_OK: succeed
 *    - others: others errors
 */
bk_err_t bk_timer_start_without_callback(timer_id_t timer_id, uint32_t time_ms);

/**
 * @brief     Stop the timer
 *
 * @param timer_id the timer ID to be stoped
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_timer_stop(timer_id_t timer_id);

/**
 * @brief     Get the current counter value
 *
 * @attention 1. The caller need to make sure the parameter timer_id is correct!
 *
 * @param timer_id the timer id
 *
 * @return counter value
 */
uint32_t bk_timer_get_cnt(timer_id_t timer_id);

/**
 * @brief     Enable the timer
 *
 * @param timer_id the timer ID
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_timer_enable(timer_id_t timer_id);

/**
 * @brief     Disable the timer
 *
 * @param timer_id the timer ID
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_timer_disable(timer_id_t timer_id);

/**
 * @brief     Get the counter value set by bk_timer_start()
 *
 * @param timer_id the timer id
 *
 * @return counter value set by bk_timer_start()
 */
uint32_t bk_timer_get_period(timer_id_t timer_id);

/**
 * @brief     Get the timer enable status
 *
 * @param timer_id the timer id
 *
 * @return timer enable status
 */
uint32_t bk_timer_get_enable_status(void);

/**
 * @brief     Get whether the timer interrupt triggered
 *
 * @param timer_id the timer id
 *
 * @return the timer interrupt status
 */
bool bk_timer_is_interrupt_triggered(timer_id_t timer_id);

/**
 * @brief     Get the timer count value in ms or us
 * 
 * @param timer_id the timer id
 * @param div the divider of clock frequency
 * @param last_count last count of the timer
 * @param timer_value_unit_t the time unit, ms or us
 * @return the timer value in micro seconds
 */
uint64_t bk_timer_get_time(timer_id_t timer_id, uint32_t div, uint32_t last_count, timer_value_unit_t unit_type);


/**
 * @brief     This function set the delay based on the current running counter, which is different from bk_timer_start.
 * 
 * @param timer_id the timer id
 * @param time_us time delay value of the timer by us
 * @param callback the timer delay call back function
 * @return 
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_timer_delay_with_callback(timer_id_t timer_id, uint64_t time_us, timer_isr_t callback);

/**
 * @brief     Cancel the timer delay task, reset timer and set ISR to NULL, it will not disable the timer, which is different from bk_timer_stop. 
 * 
 * @param timer_id the timer id
 * @return 
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_timer_cancel(timer_id_t timer_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
