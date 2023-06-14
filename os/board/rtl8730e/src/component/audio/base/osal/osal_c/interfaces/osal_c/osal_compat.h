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
 * @file osal_compat.h
 *
 * @brief Declares commonly used compat macros.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_COMPAT_H
#define AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_COMPAT_H


#if defined(__RTOS__) || defined(__ICCARM__)

#ifndef __ICCARM__
#include <machine/endian.h>
#endif
#include "libs/strings.h"
#include "libs/string.h"
//#include "libs/uio.h"

#ifdef __ICCARM__
#ifndef  ssize_t
#define ssize_t int
#endif
#ifndef  off_t
#define off_t int
#endif
#endif

typedef off_t off64_t;



#ifndef ntohl
#define ntohl(x)  __ntohl(x)
#endif
#ifndef htonl
#define htonl(x)  __htonl(x)
#endif
#ifndef ntohs
#define ntohs(x)  __ntohs(x)
#endif
#ifndef htons
#define htons(x)  __htons(x)
#endif

// FIXME:
// On RTOS, we assume 32bit system
#ifndef SSIZE_MAX
#define SSIZE_MAX  INT_MAX
#endif

#define bswap_16(x) __builtin_bswap16(x)

#define typeof(x) __typeof(x)

#endif


/*
 * Needed for cases where something should be constexpr if possible, but not
 * being constexpr is fine if in pre-C++11 code (such as a const static float
 * member variable).
 */
#if defined(__cplusplus) && __cplusplus >= 201103L
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

/*
 * TEMP_FAILURE_RETRY is defined by some, but not all, versions of
 * <unistd.h>. If it's not already defined, then define it here.
 */
#ifndef TEMP_FAILURE_RETRY
/* Used to retry syscalls that can return EINTR. */
#define TEMP_FAILURE_RETRY(exp) ({         \
    typeof (exp) _rc;                      \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })
#endif

#define OS_PATH_SEPARATOR '/'

#endif // AMEBA_BASE_OSAL_OSAL_C_INTERFACES_OSAL_COMPAT_H
/** @} */
