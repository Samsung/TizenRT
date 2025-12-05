/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MBEDTLS_POLY1305_ALT_H
#define _MBEDTLS_POLY1305_ALT_H

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(MBEDTLS_POLY1305_ALT)

/************************ Typedefs  ****************************/
typedef struct mbedtls_poly1305_context
{
    uint32_t r[4];      /** The value for 'r' (low 128 bits of the key). */
    uint32_t s[4];      /** The value for 's' (high 128 bits of the key). */
    uint32_t acc[5];    /** The accumulator number. */
    uint8_t queue[16];  /** The current partial block of data. */
    size_t queue_len;   /** The number of bytes stored in 'queue'. */
}
mbedtls_poly1305_context;

#endif

#ifdef __cplusplus
}
#endif

#endif /* poly1305_alt.h */
