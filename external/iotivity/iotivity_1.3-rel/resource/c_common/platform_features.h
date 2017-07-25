//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//******************************************************************

/**
 * @file
 *
 * This file contains compiler and platform feature definitions.  These
 * can be used to enable functionality on only platforms that support
 * said functionality.
 */

#ifndef PLATFORM_FEATURES_H_
#define PLATFORM_FEATURES_H_


#ifndef __TIZENRT__
#if (__STDC_VERSION__ >= 201112L)
#  include <assert.h>
#  define OC_STATIC_ASSERT(condition, msg) static_assert(condition, msg)
#endif
#elif defined(_WIN32)
#  if defined(__msys_nt__) && !defined(__cplusplus)
#    define static_assert _Static_assert
#  endif
#  define OC_STATIC_ASSERT(condition, msg) static_assert(condition, msg)
#else
#  define OC_CAT_(a, b) a ## b
#  define OC_CAT(a, b) OC_CAT_(a, b)
/*
 * __COUNTER__ Expands to an integer that starts at 0 an is incremented by 1 every
 * time it is used in a source file.
 * It is used here to create a unique identifier.
 * It is supported in MSVC since at least VS2015 and gcc version 4.3.0
 */
#  ifdef __COUNTER__
#    define OC_STATIC_ASSERT(condition, msg) \
       { enum { OC_CAT(StaticAssert_, __COUNTER__) = 1/(int)(!!(condition)) }; }
#  else
     /*
      * Note this can not be used twice on the same line. Make sure header guards
      * (i.e #ifdef HEADER ... #endif) are used to avoid including twice.
      */
#    define OC_STATIC_ASSERT(condition, msg) \
       { enum { OC_CAT(StaticAssert_line_, __LINE__) = 1/(int)(!!(condition)) }; }
#  endif
#endif

#if !(defined _GLIBCXX_USE_NANOSLEEP) \
  && defined(__GNUC__) && (4 == __GNUC__) && (7 >= __GNUC_MINOR__)
#  define  _GLIBCXX_USE_NANOSLEEP 1
#endif

#ifndef INLINE_API
#  if defined(__cplusplus)
#    define INLINE_API inline
#  else
#    ifdef _MSC_VER
#      define INLINE_API static __inline
#    else
#      define INLINE_API static inline
#    endif
#  endif
#endif

#ifdef _MSC_VER
#  define OC_ANNOTATE_UNUSED
#else
#  define OC_ANNOTATE_UNUSED  __attribute__((unused))
#endif

#ifdef _WIN32
#  define __func__ __FUNCTION__
#  define strncasecmp _strnicmp
#  define strtok_r strtok_s
#  if _MSC_VER && (_MSC_VER < 1900)
#    include "windows/include/vs12_snprintf.h"
#  endif
#  define ssize_t SSIZE_T
#  define SHUT_RDWR           SD_BOTH
#  define sleep(SECS)         Sleep(1000*(SECS))
#  include "windows/include/memmem.h"
#  include "windows/include/win_sleep.h"
#  include "windows/include/pthread_create.h"
#endif

#ifdef HAVE_WINSOCK2_H
#  define OPTVAL_T(t)    (const char*)(t)
#  define OC_CLOSE_SOCKET(s) closesocket(s)
#else
#  define OPTVAL_T(t)    (t)
#  define OC_CLOSE_SOCKET(s) close(s)
#endif

#ifndef SIZE_MAX
/* Some systems fail to define SIZE_MAX in <stdint.h>, even though C99 requires it...
 * Conversion from signed to unsigned is defined in 6.3.1.3 (Signed and unsigned integers) p2,
 * which says: "the value is converted by repeatedly adding or subtracting one more than the
 * maximum value that can be represented in the new type until the value is in the range of the
 * new type."
 * So -1 gets converted to size_t by adding SIZE_MAX + 1, which results in SIZE_MAX.
 */
#  define SIZE_MAX ((size_t)-1)
#endif

#ifdef WITH_ARDUINO
/**
 * UINT16_MAX does not appear to be defined on Arduino so we define it here.
 */
#  define UINT16_MAX 65535

/**
 * Handle case that PRId64 is not defined in Arduino's inttypes.h
 */
#  if !defined(PRId64)
#    define PRId64 "lld"
#  endif
#endif

/**
 * Mark a parameter as unused. Used to prevent unused variable compiler warnings.
 */
#  define OC_UNUSED(x) (void)(x)

/**
 * Calling convention.
 */
#ifdef _WIN32
/*
 * Set to __stdcall for Windows, consistent with WIN32 APIs.
 */
#  define OC_CALL   __stdcall
#else
#  define OC_CALL
#endif

#endif
