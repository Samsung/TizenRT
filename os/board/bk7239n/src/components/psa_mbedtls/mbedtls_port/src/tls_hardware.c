/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "mbedtls/build_info.h"
#include "mbedtls/entropy.h"

#include <sys/types.h>
#include <stdint.h>
#include <driver/trng.h>

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)

static int os_get_random(unsigned char *buf, size_t len)
{
    size_t i = 0;

    if (buf == NULL && len != 0)
        return -1;

    /* Initialize the hardware TRNG driver. */
    if (bk_trng_driver_init() != 0)
        return -1;

    while (i < len) {
        uint32_t rnd = (uint32_t)bk_rand();
        /* bk_rand() masks the TRNG value with RAND_MAX, so only use 24 full-entropy bits. */
        size_t copy = (len - i) < 3 ? (len - i) : 3;
        for (size_t j = 0; j < copy; j++)
            buf[i + j] = (unsigned char)(rnd >> (j * 8));
        i += copy;
    }

    return 0;
}

int mbedtls_hardware_poll( void *data,
                           unsigned char *output, size_t len, size_t *olen )
{
    (void) data;

    if (olen == NULL || output == NULL)
        return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;

    *olen = 0;

    if (os_get_random(output, len) != 0) {
        return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    }

    *olen = len;

    return 0;
}

#endif

