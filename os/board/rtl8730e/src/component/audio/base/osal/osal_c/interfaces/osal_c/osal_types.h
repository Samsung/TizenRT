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
 * @file osal_types.h
 *
 * @brief Declares common types.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TYPES_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#include "osal_c/osal_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

// `rt_status_t` is a simple error type used to communicate whether a particular
// action was successful or not. It is defined as being a signed 32-bit integer.
// The value `OSAL_OK` (0) indicates an operation was successful. All other values
// indicate an error of some form.
typedef int32_t rt_status_t;


// used to represent pointers to arbitrary untyped data buffers.
typedef void *rt_handle_t;

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_TYPES_H
/** @} */
