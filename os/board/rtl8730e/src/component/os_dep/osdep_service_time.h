/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 ******************************************************************************/

#ifndef __OSDEP_SERVICE_TIME_H_
#define __OSDEP_SERVICE_TIME_H_

/*
 *      These inlines deal with timer wrapping correctly. You are
 *      strongly encouraged to use them
 *      1. Because people otherwise forget
 *      2. Because if the timer wrap changes in future you wont have to
 *         alter your driver code.
 *
 * time_after(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
#ifndef time_after
#define time_after(a,b)	((long)(b) - (long)(a) < 0)
#endif
#ifndef time_before
#define time_before(a,b)	time_after(b,a)
#endif
#ifndef time_after_eq
#define time_after_eq(a,b)	((long)(a) - (long)(b) >= 0)
#endif
#ifndef time_before_eq
#define time_before_eq(a,b)	time_after_eq(b,a)
#endif

#define TIMER_MAX_DELAY    				0xFFFFFFFF

#ifndef mdelay
#define mdelay(t)					((t/portTICK_RATE_MS)>0)?(vTaskDelay(t/portTICK_RATE_MS)):(vTaskDelay(1))
#endif

#ifndef udelay
#define udelay(t)					((t/(portTICK_RATE_MS*1000))>0)?vTaskDelay(t/(portTICK_RATE_MS*1000)):(vTaskDelay(1))
#endif

#define rtw_warn_on(condition) do {} while (0)

#ifdef __cplusplus
extern "C" {
#endif
/*************************** Time Management *******************************/

/**
 * @brief  Get the count of ticks since the vTaskStartScheduler was called.
 * @return	The count of ticks since the vTaskStartScheduler was called.
 */
uint32_t	rtw_get_current_time(void);

/**
 * @brief  Convert system time to milliseconds.
 * @param[in] systime: The system time to be converted.
 * @return : The milliseconds that converted by the system time.
 */
uint32_t	rtw_systime_to_ms(uint32_t systime);

/**
 * @brief  Convert system time to seconds.
 * @param[in] systime: The system time to be converted.
 * @return : The seconds that converted by the system time.
 */
uint32_t rtw_systime_to_sec(uint32_t systime);

/**
 * @brief  Convert milliseconds to system time.
 * @param[in] systime: The milliseconds to be converted.
 * @return : The system time that converted by the milliseconds.
 */
uint32_t	rtw_ms_to_systime(uint32_t ms);

/**
 * @brief  Convert seconds to system time.
 * @param[in] systime: The seconds to be converted.
 * @return : The system time that converted by the seconds.
 */
uint32_t	rtw_sec_to_systime(uint32_t sec);

/**
 * @brief  Get the passing time from the "start" in milliseconds.
 * @param[in] start: The start time which is in system time format.
 * @return : The passing time from "start" in milliseconds.
 */
uint32_t	rtw_get_passing_time_ms(uint32_t start);

/**
 * @brief  Get the interval time from the "start" to "end" in milliseconds.
 * @param[in] start: The start time which is in system time format.
 * @param[in] end: The end time which is in system time format.
 * @return : The interval time from "start" to "end" in milliseconds.
 */
int32_t	rtw_get_time_interval_ms(uint32_t start, uint32_t end);
/*************************** End Time Management *******************************/

/**
 * @brief  This function suspends execution of the calling thread for "ms" milliseconds.
 * @param[in] ms: The time that the function sleep in milliseconds
 * @return	  None
*/
void	rtw_msleep_os(int ms);

/**
 * @brief  This function suspends execution of the calling thread for "us" microseconds.
 * @param[in] ms: The time that the function sleep in microseconds
 * @return	  None
*/
void	rtw_usleep_os(int us);


/**
 * @brief  This function delays a task for the giving time in milliseconds.
 * @param[in] ms: The amount of time, in milliseconds, that the calling task should block.
 * @return	  None
*/
void	rtw_mdelay_os(int ms);

/**
 * @brief  This function delays a task for the giving time in microseconds.
 * @param[in] ms: The amount of time, in microseconds, that the calling task should block.
 * @return	  None
*/
void	rtw_udelay_os(int us);

/**
 * @brief  This function for forcing a context switch.
 * @return	  None
*/
void	rtw_yield_os(void);
#ifdef __cplusplus
}
#endif

#endif
