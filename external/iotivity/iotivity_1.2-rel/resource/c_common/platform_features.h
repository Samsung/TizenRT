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
#  if (__cplusplus >=201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define SUPPORTS_DEFAULT_CTOR
#  endif
#endif

#ifndef __TIZENRT__
#  if (__STDC_VERSION__ >= 201112L)
#    include <assert.h>
#    define OC_STATIC_ASSERT(condition, msg) static_assert(condition, msg)
#endif
#  elif defined(_WIN32)
#    if defined(__msys_nt__) && !defined(__cplusplus)
#      define static_assert _Static_assert
#    endif
#    define OC_STATIC_ASSERT(condition, msg) static_assert(condition, msg)
#  else
#    define OC_STATIC_ASSERT(condition, msg) ((void)sizeof(char[2*!!(condition) - 1]))
#  endif

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
#  define F_OK                0
#  define sleep(SECS)         Sleep(1000*(SECS))
#  ifdef __cplusplus
#    define SUPPORTS_DEFAULT_CTOR
#  endif
#  include "windows/include/win_sleep.h"
#  include "windows/include/pthread_create.h"
#endif

#ifdef HAVE_WINSOCK2_H
#  define OPTVAL_T(t)    (const char*)(t)
#else
#  define OPTVAL_T(t)    (t)
#endif

#endif
