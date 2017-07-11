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
 * include/string.h
 *
 *   Copyright (C) 2007-2012, 2014 Gregory Nutt. All rights reserved.
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
 * @defgroup STRING_LIBC STRING
 * @brief Provides APIs for String
 * @ingroup KERNEL
 *
 * @{
 */

///@file string.h
///@brief String APIs

#ifndef __INCLUDE_STRING_H
#define __INCLUDE_STRING_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Compatibility definitions */

#ifndef CONFIG_ARCH_BZERO
#define bzero(s, n) (void)memset(s, 0, n)
#endif

#define bcopy(b1, b2, len) (void)memmove(b2, b1, len)

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strdup(FAR const char *s);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strndup(FAR const char *s, size_t size);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR const char *strerror(int);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
size_t strlen(FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
size_t strnlen(FAR const char *, size_t);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strcat(FAR char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strncat(FAR char *, FAR const char *, size_t);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int strcmp(FAR const char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int strncmp(FAR const char *, FAR const char *, size_t);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int strcasecmp(FAR const char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int strncasecmp(FAR const char *, FAR const char *, size_t);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strcpy(char *dest, FAR const char *src);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *stpcpy(char *dest, FAR const char *src);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strncpy(char *, FAR const char *, size_t);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strpbrk(FAR const char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strchr(FAR const char *s, int c);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strrchr(FAR const char *s, int c);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
size_t strspn(FAR const char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
size_t strcspn(FAR const char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strstr(FAR const char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strcasestr(FAR const char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strtok(FAR char *, FAR const char *);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *strtok_r(FAR char *, FAR const char *, FAR char **);

/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR void *memchr(FAR const void *s, int c, size_t n);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR void *memccpy(FAR void *s1, FAR const void *s2, int c, size_t n);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int memcmp(FAR const void *s1, FAR const void *s2, size_t n);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR void *memcpy(FAR void *dest, FAR const void *src, size_t n);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR void *memmove(FAR void *dest, FAR const void *src, size_t count);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR void *memset(FAR void *s, int c, size_t n);

/**
 * @brief Copies up to size - 1 characters from the NUL-terminated string src to dst, NUL-terminating the result.
 * @param[in] A pointer to the destination string.
 * @param[in] A pointer to the source string.
 * @param[in] The size of the destination buffer.
 * @return Total length of the string they tried to create.
 * @since Tizen RT v1.0
 */
size_t strlcpy(char *__restrict dest, const char *__restrict src, size_t size);
#undef EXTERN
#if defined(__cplusplus)
}
#endif
#endif							/* __INCLUDE_STRING_H */
/**
 * @}
 */
