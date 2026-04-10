/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * mbedtls_ms_time() for TizenRT.
 *
 * platform_util.c selects its implementation via _POSIX_VERSION, which
 * TizenRT does not define even though clock_gettime() is available.
 * MBEDTLS_PLATFORM_MS_TIME_ALT suppresses the built-in selection and
 * lets us provide this implementation directly.
 */

#include <tinyara/config.h>
#include <time.h>
#include "mbedtls/build_info.h"
#include "mbedtls/platform_time.h"

#if defined(MBEDTLS_HAVE_TIME) && defined(MBEDTLS_PLATFORM_MS_TIME_ALT)

mbedtls_ms_time_t mbedtls_ms_time(void)
{
    struct timespec tv;

    if (clock_gettime(CLOCK_MONOTONIC, &tv) != 0) {
        /* Fallback: second-granularity only */
        return (mbedtls_ms_time_t)time(NULL) * 1000;
    }

    return (mbedtls_ms_time_t)tv.tv_sec * 1000
           + (mbedtls_ms_time_t)tv.tv_nsec / 1000000;
}

#endif /* MBEDTLS_HAVE_TIME && MBEDTLS_PLATFORM_MS_TIME_ALT */
