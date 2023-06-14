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
 * @file osal_mem.h
 *
 * @brief Declares the memory request and release interfaces.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_MEM_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_MEM_H

#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates memory of a specified size.
 *
 * @param size Indicates the size of memory to allocate.
 *
 * @return Returns the pointer to the allocated memory if the operation is successful; returns <b>NULL</b> otherwise.
 *
 * @since 1.0
 * @version 1.0
 */
void *OsalMemAlloc(size_t size, const char *name, int32_t ext);

/**
 * @brief Allocates memory of a specified size, and clears the allocated memory.
 *
 * @param size Indicates the size of memory to allocate.
 *
 * @return Returns the pointer to the allocated memory if the operation is successful; returns <b>NULL</b> otherwise.
 *
 * @since 1.0
 * @version 1.0
 */
void *OsalMemCalloc(size_t size, const char *name, int32_t ext);

/**
 * @brief realloc memory of a specified size
 *
 * @param ptr pointer the allocated memory .
 * @param size Indicates the size of memory to allocate.
 *
 * @return Returns the pointer to the allocated memory if the operation is successful; returns <b>NULL</b> otherwise.
 *
 * @since 1.0
 * @version 1.0
 */
void *OsalMemRealloc(void *ptr, size_t size, const char *name, int32_t ext);

/**
 * @brief Releases memory.
 *
 * @param mem Indicates the pointer to the memory to release.
 *
 * @since 1.0
 * @version 1.0
 */
void  OsalMemFree(void *mem);


#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_MEM_H
/** @} */
