/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/
#ifndef SYNTIANT_NDP_PORTABILITY_H
#define SYNTIANT_NDP_PORTABILITY_H

#ifdef __cplusplus
extern "C" {
#endif

/* for TizenRT integration, we temporarily remove __KERNEL__ */
#ifdef __KERNEL__
#undef __KERNEL__
#endif

/* integers, strings, etc */
#ifdef __LINUX_KERNEL__
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#ifdef __linux__
#include <sys/time.h>
#endif
#ifdef WINDOWS_KERNEL
#include <wdm.h>
#endif
#endif

/* run-time assert */
#if defined(__KERNEL__) || defined(WINDOWS_KERNEL) || defined(_MSC_VER)
#ifdef assert
#undef assert
#endif
#define assert(x)
#endif

/* compile-time assert */
#define SYNTIANT_ASSERT_CONCAT_(a, b) a##b
#define SYNTIANT_ASSERT_CONCAT(a, b) SYNTIANT_ASSERT_CONCAT_(a, b)

#if defined(__MINGW32__) || defined(WIN32)
#define SYNTIANT_CASSERT(e, m)
#else
#ifdef __COUNTER__
#define SYNTIANT_CASSERT(e, m) \
    enum { SYNTIANT_ASSERT_CONCAT(static_assert_, __COUNTER__) \
           = 1/(int)(!!(e)) };
#else
#define SYNTIANT_CASSERT(e, m) \
    enum { SYNTIANT_ASSERT_CONCAT(assert_line_, __LINE__) \
           = 1/(int)(!!(e)) };
#endif
#endif

#ifndef PACK_DATA
#define SYNTIANT_PACK __attribute__((packed))
#else
#define SYNTIANT_PACK
#endif

#ifdef __KERNEL__
#define SYNTIANT_PRINTF printk
#else
#ifndef _MSC_VER
#define SYNTIANT_PRINTF printf
#else
#define SYNTIANT_PRINTF DbgPrint
#endif
#endif

int ERROR_PRINTF(const char * fmt, ...);

/* snprintf */
#if defined(__KERNEL__) || defined(WINDOWS_KERNEL) || defined(__APPLE__)
#else

#define SYNTIANT_SNPRINTF

/* #include <strings.h> */
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

#if !defined(HAVE_SNPRINTF)
int snprintf(char *str,size_t count,const char *fmt,...);
int asprintf(char **ptr, const char *format, ...);
#endif

#if !defined(HAVE_VSNPRINTF)
int vsnprintf (char *str, size_t count, const char *fmt, va_list args);
int vasprintf(char **ptr, const char *format, va_list ap);
#endif
#endif


#if defined(__KERNEL__)
typedef struct timespec64 syntiant_ms_time;
#elif defined(__linux__)
#define syntiant_ms_time struct timeval
#ifndef timersub
#define timersub(a, b, result)                              \
  do {                                                      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;           \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;        \
    if ((result)->tv_usec < 0) {                            \
      --(result)->tv_sec;                                   \
      (result)->tv_usec += 1000000;                         \
    }                                                       \
  } while (0)
#endif
#else
/* customer platform implementation here */
#define syntiant_ms_time unsigned long
#endif
int syntiant_get_ms_time(syntiant_ms_time *ms_time);
unsigned long syntiant_get_ms_elapsed(syntiant_ms_time *ms_time);

#ifdef __cplusplus
}
#endif

/*
 * Taken from Linux kernel commit 294f69e662d15
 *
 * Add the pseudo keyword 'fallthrough' so case statement blocks
 * must end with any of these keywords:
 *   break;
 *   fallthrough;
 *   goto <label>;
 *   return [expression];
 *
 *  gcc: https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes
 */
#if __has_attribute(__fallthrough__)
# define fallthrough                    __attribute__((__fallthrough__))
#else
# define fallthrough                    do {} while (0)  /* fallthrough */
#endif

#endif
