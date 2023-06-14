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
 * @file osal_condition.h
 *
 * @brief Declares condition types and interfaces.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_CONDITION_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_CONDITION_H

#include <stdint.h>

#include "osal_c/osal_types.h"
#include "osal_c/osal_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
#define OSALC_COND_HANLDE_SIZE 52
#else
#define OSALC_COND_HANLDE_SIZE 12
#endif

/**
 * @brief Defines a condition handle.
 *
 * @since 1.0
 * @version 1.0
 */
typedef struct {
	uint8_t cond[OSALC_COND_HANLDE_SIZE];  /**< Pointer to a condition object to operate */
} __attribute__((aligned(4))) OsalCondition;

/**
 * @brief Convenient macro to define a condition handle
 *
 * @since 1.0
 * @version 1.0
 */
#define OSAL_DECLARE_CONDITION(cond) OsalCondition condition

/**
 * @brief Initialize a condition handle.
 *
 * @param cond Indicates the pointer to the condition handle.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid function parameter.
 * OSAL_ERR_NO_MEMORY | Memory allocation fails.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalConditionInit(OsalCondition *cond);

/**
 * @brief Wait on the condition handle.
 *
 * @param cond Indicates the pointer to the condition handle.
 * @param mutex Indicates the pointer to the mutex handle {@link OsalMutex}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid function parameter.
 * Other non-zero number | The operation is failed.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalConditionWait(OsalCondition *cond, OsalMutex *mutex);

/**
 * @brief Wait on the condition with relative timeout.
 *
 * @param cond Indicates the pointer to the condition handle.
 * @param mutex Indicates the pointer to the mutex handle {@link OsalMutex}.
 * @param nanosec Indicates nanosecond time out to wait the condition.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid function parameter.
 * Other non-zero number | The operation is failed.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalConditionWaitRelative(OsalCondition *cond, OsalMutex *mutex, int64_t nanosec);

/**
 * @brief Signal the condition, allowing all threads to continue.
 *
 * @param cond Indicates the pointer to the condition handle.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid function parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalConditionBroadcast(OsalCondition *cond);

/**
 * @brief Signal the condition, allow one thread to continue.
 *
 * @param cond Indicates the pointer to the condition handle.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid function parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalConditionSignal(OsalCondition *cond);

/**
 * @brief Release condition handle's resource.
 *
 * @param cond Indicates the pointer to the condition handle.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid function parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalConditionDestroy(OsalCondition *cond);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_CONDITION_H
/** @} */
