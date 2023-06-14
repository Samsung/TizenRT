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

/*
 * The CC_LIKELY and CC_UNLIKELY macros let the programmer give hints to
 * the compiler about the expected result of an expression. Some compilers
 * can use this information for optimizations.
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
 * @file osal_macros.h
 *
 * @brief Declares the common macros
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_MACROS_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_MACROS_H

#ifdef __cplusplus
#define CC_LIKELY(exp)    (__builtin_expect( !!(exp), true ))
#define CC_UNLIKELY(exp)  (__builtin_expect( !!(exp), false ))
#else
#define CC_LIKELY(exp)    (__builtin_expect( !!(exp), 1 ))
#define CC_UNLIKELY(exp)  (__builtin_expect( !!(exp), 0 ))
#endif

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_MACROS_H
/** @} */