/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @addtogroup OSAL
 * @{
 *
 * @brief Declares the structures and interfaces for the Operating System Abstraction Layer (OSAL) module.
 *
 * The OSAL module provide a unified interfaces that adapter to different OS. The interfaces include the
 * memory management, thread, mutex, semaphore, time.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file osal_timer.h
 *
 * @brief Declares timer types and interfaces.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TIMER_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TIMER_H

#include <stdint.h>

#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __linux__
#define OSALC_TIMER_HANLDE_SIZE 32
#endif

/**
 * @brief Defines the timer types
 *
 * hardware timer 0~7 is basic timer, precision is 30 microsecond, max is 131071999969 microsecond
 * hardware timer 8 is pwm timer, precision is 25 nanosecond, max is 1638 microsecond
 * hardware timer 9 is capture timer, precision is 25 nanosecond, max is 1638 microsecond
 * hardware timer 10~13 is basic timer, precision is 1 microsecond, max is 4294967295 microsecond
 */

typedef enum {
	OSAL_TIMER_HW_TIMER0 = 0,
	OSAL_TIMER_HW_TIMER1 = 1,
	OSAL_TIMER_HW_TIMER2 = 2,
	OSAL_TIMER_HW_TIMER3 = 3,
	OSAL_TIMER_HW_TIMER4 = 4,
	OSAL_TIMER_HW_TIMER5 = 5,
	OSAL_TIMER_HW_TIMER6 = 6,
	OSAL_TIMER_HW_TIMER7 = 7,
	OSAL_TIMER_HW_TIMER8 = 8,
	OSAL_TIMER_HW_TIMER9 = 9,
	OSAL_TIMER_HW_TIMER10 = 10,
	OSAL_TIMER_HW_TIMER11 = 11,
	OSAL_TIMER_HW_TIMER12 = 12,
	OSAL_TIMER_HW_TIMER13 = 13,
} OsalTimerType;

/**
 * @brief Defines a timer handle.
 *
 * @since 1.0
 * @version 1.0
 */
typedef struct OsalTimer {
	uint8_t timer[OSALC_TIMER_HANLDE_SIZE];  /**< Pointer to a timer object to operate */
} __attribute__((aligned(4))) OsalTimer;

/**
 * @brief Describes a timer execution function type.
 */
typedef void (*OsalTimerFunc)(void *arg);

/**
 * @brief Convenient macro to define a timer handle
 *
 * @since 1.0
 * @version 1.0
 */
#define OSAL_DECLARE_TIMER(timer) OsalTimer timer

/**
 * @brief Creates a timer.
 *
 * @param timer Indicates the pointer to the timer {@link OsalTimer}.
 * @param idx Indicates the timer type {@link OsalTimerType}
 * @param interval Indicates the timer interval, the unit is microsecond.
 * @param func Indicates the timer execution function {@link OsalTimerFunc}.
 * @param arg Indicates the argument passed to the timer execution function.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_FAILURE | Failed to invoke the system function to start the timer.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalTimerCreate(OsalTimer *timer, uint32_t timer_id, uint32_t interval,  OsalTimerFunc func, void *arg);

/**
 * @brief Starts a timer.
 *
 * @param timer Indicates the pointer to the timer {@link OsalTimer}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_FAILURE | Failed to invoke the system function to start the timer.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalTimerStartOnce(OsalTimer *timer);

/**
 * @brief Starts a periodic timer.
 *
 * @param timer Indicates the pointer to the timer {@link OsalTimer}.
 * @param interval Indicates the timer interval, in microseconds.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_FAILURE | Failed to invoke the system function to start the timer.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalTimerStartLoop(OsalTimer *timer);

/**
 * @brief Stops a timer.
 *
 * @param timer Indicates the pointer to the timer {@link OsalTimer}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_FAILURE | Failed to invoke the system function to delete the timer.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalTimerStop(OsalTimer *timer);

/**
 * @brief Destroys a timer
 *
 * @param timer Indicates the pointer to the timer {@link OsalTimer}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_FAILURE | Failed to invoke the system function to start the timer.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalTimerDestroy(OsalTimer *timer);

/**
 * @brief Sets the interval of a timer.
 * If change when timer start, the change will take effect immediately
 * Next callback will called after the new interval
 *
 * @param timer Indicates the pointer to the timer {@link OsalTimer}.
 * @param interval Indicates the timer interval, in microseconds.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_FAILURE | Failed to invoke the system function.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalTimerSetTimeout(OsalTimer *timer, uint32_t interval);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TIMER_H
/** @} */
/** @} */
