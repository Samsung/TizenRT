/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#if defined(TFM_PSA_API) || !defined(PLATFORM_DEFAULT_NV_SEED)

#include "tfm_plat_crypto_nv_seed.h"

#ifndef MBEDTLS_ENTROPY_NV_SEED
#define MBEDTLS_ENTROPY_NV_SEED
#endif /* !MBEDTLS_ENTROPY_NV_SEED */

#ifdef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#undef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#endif /* MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES */

#ifndef MBEDTLS_PLATFORM_NV_SEED_READ_MACRO
#define MBEDTLS_PLATFORM_NV_SEED_READ_MACRO   tfm_plat_crypto_nv_seed_read /**< Default nv_seed_read function to use, can be undefined */
#endif /* !MBEDTLS_PLATFORM_NV_SEED_READ_MACRO */

#ifndef MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO
#define MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO  tfm_plat_crypto_nv_seed_write /**< Default nv_seed_write function to use, can be undefined */
#endif /* !MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO */

#endif /* defined(TFM_PSA_API) || !defined(PLATFORM_DEFAULT_NV_SEED) */
