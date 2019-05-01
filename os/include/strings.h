/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/**
 * @defgroup STRING_LIBC STRING
 * @brief Provides APIs for String
 * @ingroup KERNEL
 *
 * @{
 */

///@file strings.h
///@brief string operations

#ifndef __INCLUDE_STRINGS_H
#define __INCLUDE_STRINGS_H

#include <sys/types.h>

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
 * @brief case-insensitive string comparisons
 * @details @b #include <strings.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int strcasecmp(const char *cs, const char *ct);

/**
 * @brief case-insensitive string comparisons
 * @details @b #include <strings.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int strncasecmp(const char *cs, const char *ct, size_t nb);

#undef EXTERN
#if defined(__cplusplus)
}
#endif
#endif  /* __INCLUDE_STRINGS_H */
/**
 * @}
 */
