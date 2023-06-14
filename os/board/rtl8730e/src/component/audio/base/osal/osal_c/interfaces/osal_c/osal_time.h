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
 * @file osal_time.h
 *
 * @brief Declares the time, sleep interfaces.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TIME_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TIME_H

#include <stdint.h>

#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @name Unit conversion constants.
 */
#define OSAL_MILLISECONDS_PER_SECOND       (1000LL)           /**< Milliseconds per second. */
#define OSAL_MICROSECONDS_PER_SECOND       (1000000LL)        /**< Microseconds per second. */
#define OSAL_NANOSECONDS_PER_SECOND        (1000000000LL)     /**< Nanoseconds per second. */

#define OSAL_NANOSECONDS_PER_MICROSECOND   (1000LL)           /**< Nanoseconds per microsecond. */
#define OSAL_NANOSECONDS_PER_MILLISECOND   (1000000LL)        /**< Nanoseconds per millisecond. */


typedef int64_t nsecs_t;

/**
 * @brief Enumerates clock type.
 */
typedef enum {
	OSAL_TIME_REALTIME = 0,  /**< system-wide realtime clock */
	OSAL_TIME_MONOTONIC = 1, /**< monotonic time since unspecified starting point */
	OSAL_TIME_PROCESS = 2,   /**< high-resolution per-process clock */
	OSAL_TIME_THREAD = 3,    /**< high-resolution per-thread clock */
	OSAL_TIME_BOOTTIME = 4   /**< same as SYSTEM_TIME_MONOTONIC, but including CPU suspend time */
} OsalClock;

/**
 * @brief Defines time.
 */
typedef struct {
	int64_t sec;   // seconds
	int64_t nsec;  // nano seconds
} OsalTimespec;

/**
 * @brief Thread sleep, in seconds.
 *
 * @param sec Indicates the sleep time, in seconds.
 * @since 1.0
 * @version 1.0
 */
void OsalSleep(uint32_t sec);

/**
 * @brief Thread sleep, in milliseconds.
 *
 * @param ms Indicates the sleep time, in milliseconds.
 * @since 1.0
 * @version 1.0
 */
void OsalMSleep(uint32_t ms);

/**
 * @brief Get current time according to the clock type
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalGetTime(OsalClock clock, OsalTimespec *time);

/**
 * @brief Get current time in milliseconds.
 *
 * @since 1.0
 * @version 1.0
 */
int64_t OsalGetSysTimeMs(OsalClock clock);

/**
 * @brief Get current time in nanseconds.
 *
 * @since 1.0
 * @version 1.0
 */
int64_t OsalGetSysTimeNs(OsalClock clock);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TIME_H
/** @} */
/** @} */