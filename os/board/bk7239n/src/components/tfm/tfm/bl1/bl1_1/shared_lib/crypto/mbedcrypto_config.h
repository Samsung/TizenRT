/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#ifndef BL1_1_MBEDCRYPO_CONFIG_H
#define BL1_1_MBEDCRYPO_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_MEMORY_BUFFER_ALLOC_C

#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA224_C

#define MBEDTLS_AES_C
#define MBEDTLS_CIPHER_MODE_CTR

#ifdef __cplusplus
}
#endif

#endif /* BL1_1_MBEDCRYPO_CONFIG_H */
