/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * include/assert.h
 *
 *   Copyright (C) 2007-2009, 2011-2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @defgroup ASSERT_KERNEL ASSERT
 * @brief Provides APIs for Assertion
 * @ingroup KERNEL
 *
 * @{
 */

/// @file assert.h
/// @brief Assert APIs

#ifndef __INCLUDE_ASSERT_H
#define __INCLUDE_ASSERT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/compiler.h>
#include <tinyara/config.h>
#include <stdint.h>

extern char assert_info_str[CONFIG_STDIO_BUFFER_SIZE];
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Macro Name: ASSERT, VERIFY, et al. */

#undef ASSERT					/* Assert if the condition is not true */
#undef VERIFY					/* Assert if a function returns a negative value */
#undef DEBUGASSERT				/* Like ASSERT, but only if CONFIG_DEBUG is defined */
#undef DEBUGVERIFY				/* Like VERIFY, but only if CONFIG_DEBUG is defined */
#undef PANIC					/* Unconditional abort */

#ifdef CONFIG_HAVE_FILENAME
#define ASSERT_INFO(f, fmt, ...) \
	{ \
		if (!(f)) { \
			  snprintf(assert_info_str, CONFIG_STDIO_BUFFER_SIZE, fmt, ##__VA_ARGS__); \
			  up_assert((const uint8_t *)__FILE__, (int)__LINE__); \
		} \
	}

#define ASSERT(f) \
	{ if (!(f)) up_assert((const uint8_t *)__FILE__, (int)__LINE__); }

#define VERIFY(f) \
	{ if ((f) < 0) up_assert((const uint8_t *)__FILE__, (int)__LINE__); }

#define PANIC() \
	up_assert((const uint8_t *)__FILE__, (int)__LINE__)

#ifdef CONFIG_DEBUG

#define DEBUGASSERT_INFO(f, fmt, ...) \
	{ \
		if (!(f)) { \
			  snprintf(assert_info_str, CONFIG_STDIO_BUFFER_SIZE, fmt, ##__VA_ARGS__); \
			  up_assert((const uint8_t *)__FILE__, (int)__LINE__); \
		} \
	}

#define DEBUGASSERT(f) \
	{ if (!(f)) up_assert((const uint8_t *)__FILE__, (int)__LINE__); }

#define DEBUGVERIFY(f) \
	{ if ((f) < 0) up_assert((const uint8_t *)__FILE__, (int)__LINE__); }

#define DEBUGPANIC() \
	up_assert((const uint8_t *)__FILE__, (int)__LINE__)

#else

#define DEBUGASSERT_INFO(f, fmt, ...)
#define DEBUGASSERT(f)
#define DEBUGVERIFY(f) ((void)(f))
#define DEBUGPANIC()

#endif							/* CONFIG_DEBUG */

#else

#define ASSERT_INFO(f, fmt, ...) \
	{ \
		if (!(f)) { \
			  snprintf(assert_info_str, CONFIG_STDIO_BUFFER_SIZE, fmt, ##__VA_ARGS__); \
			  up_assert(); \
		} \
	}
/**
 * @brief Assert if the condition is not true
 *
 * @details @b #include <assert.h>
 * @param[in] f assertion condition which shall have a scalar type
 * @return none
 * @since TizenRT v1.0
 */
#define ASSERT(f)        { if (!(f)) up_assert(); }
/**
 * @brief Assert if a function returns a negative value
 *
 * @details @b #include <assert.h>
 * @param[in] f assertion condition which shall have a scalar type
 * @return none
 * @since TizenRT v1.0
 */
#define VERIFY(f)        { if ((f) < 0) up_assert(); }
/**
 * @brief Unconditional abort
 *
 * @details @b #include <assert.h>
 * @return none
 * @since TizenRT v1.0
 */
#define PANIC()          up_assert()

#ifdef CONFIG_DEBUG

#define DEBUGASSERT_INFO(f, fmt, ...) \
	{ \
		if (!(f)) { \
			  snprintf(assert_info_str, CONFIG_STDIO_BUFFER_SIZE, fmt, ##__VA_ARGS__): \
			  up_assert(); \
		} \
	}
/**
 * @brief Like ASSERT, but only if CONFIG_DEBUG is defined
 *
 * @details @b #include <assert.h>
 * @param[in] f assertion condition which shall have a scalar type
 * @return none
 * @since TizenRT v1.0
 */
#define DEBUGASSERT(f) { if (!(f)) up_assert(); }
/**
 * @brief Like VERIFY, but only if CONFIG_DEBUG is defined
 *
 * @details @b #include <assert.h>
 * @param[in] f assertion condition which shall have a scalar type
 * @return none
 * @since TizenRT v1.0
 */
#define DEBUGVERIFY(f) { if ((f) < 0) up_assert(); }
/**
 * @brief Like PANIC, but only if CONFIG_DEBUG is defined
 *
 * @details @b #include <assert.h>
 * @return none
 * @since TizenRT v1.0
 */
#define DEBUGPANIC()   up_assert()

#else

#define DEBUGASSERT_INFO(f, fmt, ...)
#define DEBUGASSERT(f)
#define DEBUGVERIFY(f) ((void)(f))
#define DEBUGPANIC()

#endif							/* CONFIG_DEBUG */
#endif

/**
 * @cond
 * @internal
 * @{
 */
#ifndef assert
#define assert ASSERT
#endif

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_HAVE_FILENAME
void up_assert(FAR const uint8_t *filename, int linenum);
#else
void up_assert(void);
#endif

#ifdef CONFIG_FRAME_POINTER
/*****************************************************************************
 * dump_stack : Dumps the call stack of the calling Task/Thread
 *****************************************************************************/
void dump_stack(void);

/*****************************************************************************
 * dump_all_stack : Dumps the call stack of all the tasks/threads in system
 *****************************************************************************/
void dump_all_stack(void);
#endif
/**
 * @}
 * @endcond
 */
#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_ASSERT_H */

/**
 * @}
 */
