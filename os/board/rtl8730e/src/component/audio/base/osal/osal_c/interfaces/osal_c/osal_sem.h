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
 * @file osal_sem.h
 *
 * @brief Declares semaphore structures and interfaces.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_SEM_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_SEM_H

#include <stdint.h>

#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif


#define OSAL_WAIT_FOREVER 0xFFFFFFFF

#ifdef __linux__
#define OSALC_SEM_HANLDE_SIZE 96
#else
#define OSALC_SEM_HANLDE_SIZE 12
#endif

/**
 * @brief Defines a mutex handle.
 *
 * @since 1.0
 * @version 1.0
 */
typedef struct OsalSem {
	uint8_t semaphore[OSALC_SEM_HANLDE_SIZE]; /**< Pointer to a semaphore object to operate */
} __attribute__((aligned(4))) OsalSem;

/**
 * @brief Convenient macro to define a semaphore handle
 *
 * @since 1.0
 * @version 1.0
 */
#define OSAL_DECLARE_SEMAPHORE(sem) OsalSem sem

/**
 * @brief Initializes a semaphore.
 *
 * @param sem Indicates the pointer to the semaphore {@link OsalSem}.
 * @param value Indicates the initial value of the semaphore.
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
rt_status_t OsalSemInit(OsalSem *sem, uint32_t value);

/**
 * @brief Waits for a semaphore.
 *
 * @param sem Indicates the pointer to the semaphore {@link OsalSem}.
 * @param ms Indicates the timeout interval.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_FAILURE | Failed to invoke the system function to wait for the semaphore.
 * OSAL_ERR_INVALID_PARAM | Invalid function parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalSemWait(OsalSem *sem, uint32_t ms);

/**
 * @brief Releases a semaphore.
 *
 * @param sem Indicates the pointer to the semaphore {@link OsalSem}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalSemPost(OsalSem *sem);

/**
 * @brief Destroys a semaphore.
 *
 * @param sem Indicates the pointer to the semaphore {@link OsalSem}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalSemDestroy(OsalSem *sem);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_SEM_H
/** @} */
