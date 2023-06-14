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
 * @file osal_thread.h
 *
 * @brief Declares thread types and interfaces.
 *
 * Threads created by the OSAL module are in the detached state. When releasing threads, the service module must
 * first terminate its own threads and then {@link OsalThreadDestroy} can be called to release applied resources.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_THREAD_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_THREAD_H

#include <stdint.h>
#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumerates thread priorities.
 *
 * @since 1.0
 * @version 1.0
 */
typedef enum {
	OSAL_THREAD_PRI_LOWEST         =  19,
	OSAL_THREAD_PRI_BACKGROUND     =  10,
	OSAL_THREAD_PRI_NORMAL         =   0,
	OSAL_THREAD_PRI_FOREGROUND     =  -2,
	OSAL_THREAD_PRI_DISPLAY        =  -4,
	OSAL_THREAD_PRI_URGENT_DISPLAY =  -8,
	OSAL_THREAD_PRI_VIDEO          = -10,
	OSAL_THREAD_PRI_AUDIO          = -16,
	OSAL_THREAD_PRI_URGENT_AUDIO   = -19,
	OSAL_THREAD_PRI_HIGHEST        = -20,

	OSAL_THREAD_PRI_DEFAULT        = OSAL_THREAD_PRI_NORMAL,
	OSAL_THREAD_PRI_MORE_FAVORABLE = -1,
	OSAL_THREAD_PRI_LESS_FAVORABLE = +1,
} OSAL_THREAD_PRIORITY;

/**
 * @brief Describes thread parameters.
 *
 * @since 1.0
 * @version 1.0
 */
struct OsalThreadParam {
	char *name; /**< Thread name */
	size_t stack_size; /**< Thread stack size */
	bool joinable; /**< Thread joinable */
	OSAL_THREAD_PRIORITY priority; /**< Thread priority */
};
typedef struct OsalThreadParam OsalThreadParam;

/**
 * @brief Defines a thread id
 *
 * @since 1.0
 * @version 1.0
 */
typedef void *osal_thread_id_t;

/**
 * @brief Defines a thread routine function type.
 *
 * @since 1.0
 * @version 1.0
 */
typedef void *(*OsalThreadFunc)(void *);


/**
 * @brief Defines a thread handle.
 *
 * @since 1.0
 * @version 1.0
 */
typedef struct OsalThread OsalThread;


/**
 * @brief Creates a thread.
 *
 * @param func Indicates the thread callback function {@link OsalThreadFunc}.
 * @param data Indicates the pointer to the parameter passed to the thread callback function.
 * @param name Indicates the name of the thread.
 * @param prio Indicates the priority of the thread.
 * @param stack_size Indicates the stack size of the thread.
 * @param thread Indicates the pointer to the thread handle {@link OsalThread}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 * OSAL_ERR_NO_MEMORY | Memory allocation fails.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalThreadCreate(OsalThreadFunc func, void *data, const char *name, int32_t prio, size_t stack_size, OsalThread **thread);

/**
 * @brief Creates a thread.
 *
 * @param func Indicates the thread callback function {@link OsalThreadFunc}.
 * @param data Indicates the pointer to the parameter passed to the thread callback function.
 * @param param Indicates the pointer to the parameter used to start a thread. For details, see {@link OsalThreadParam}.
 * @param thread Indicates the pointer to the thread handle {@link OsalThread}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 * OSAL_ERR_NO_MEMORY | Memory allocation fails.
 * OSAL_ERR_FAILURE | Create thread fails.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalThreadCreateEx(OsalThreadFunc func, void *data, OsalThreadParam *param, OsalThread **thread);

/**
 * @brief Detach a thread.
 *
 * @param thread Indicates the pointer to the thread handle {@link OsalThread}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 * OSAL_ERR_FAILURE | Detach thread fails.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalThreadDetach(OsalThread *thread);

/**
 * @brief Join a thread.
 *
 * @param thread Indicates the pointer to the thread handle {@link OsalThread}.
 *
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * OSAL_ERR_INVALID_PARAM | Invalid parameter.
 * OSAL_ERR_FAILURE | Detach thread fails.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalThreadJoin(OsalThread *thread);

/**
 * @brief Exit a thread.
 *
 * @param retval Indicates the return value of a thread
 *
 * @since 1.0
 * @version 1.0
 */
void OsalThreadExit(void *retval);

/**
 * @brief Destroys a thread.
 *
 * @param thread Indicates the pointer to the thread {@link OsalThread}.
 * @return Returns a value listed below: \n
 * OSAL_STATUS | Description
 * ----------------------| -----------------------
 * OSAL_OK | The operation is successful.
 * HDF_ERR_INVALID_PARAM | Invalid parameter.
 *
 * @since 1.0
 * @version 1.0
 */
rt_status_t OsalThreadDestroy(OsalThread *thread);

/**
 * @brief Get thread id of a thread handle
 *
 * @param thread Indicates the pointer to the thread {@link OsalThread}.
 * @return Returns the thread id of thread handle
 *
 * @since 1.0
 * @version 1.0
 */
osal_thread_id_t OsalThreadGetID(OsalThread *thread);

/**
 * @brief Get thread id of the current running thread
 *
 * @return Returns the thread id of current running thread
 *
 * @since 1.0
 * @version 1.0
 */
osal_thread_id_t OsalGetCurrentThreadID(void);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_THREAD_H
/** @} */
