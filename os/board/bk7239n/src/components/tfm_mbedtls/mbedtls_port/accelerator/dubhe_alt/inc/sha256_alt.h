/**
* @if copyright_display
 *      Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
* @endif
 */
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

#ifndef _SHA256_ALT_H_
#define _SHA256_ALT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_SHA256_ALT)
#include "dubhe_hash.h"
/*
 * \brief          SHA-1 context structure
 */
typedef arm_ce_hash_context_t mbedtls_sha256_context;

#define MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED                -0x0037  /**< SHA-256 hardware accelerator failed */

#if defined(ARM_CE_DUBHE_HASH)
#define MBEDTLS_ERR_SHA256_INPUT_LEN    -0x00E1
#define MBEDTLS_ERR_SHA256_NOMEM        -0x00E2
#define MBEDTLS_ERR_SHA256_CE_INIT      -0x00E3
#define MBEDTLS_ERR_SHA256_CTX_NULL     -0x00E4
#endif

#define mbedtls_sha256_init dubhe_sha256_init
#define mbedtls_sha256_free dubhe_sha256_free
#define mbedtls_sha256_clone dubhe_sha256_clone
#define mbedtls_sha256_starts_ret dubhe_sha256_starts_ret
#define mbedtls_sha256_update_ret dubhe_sha256_update_ret
#define mbedtls_sha256_finish_ret dubhe_sha256_finish_ret
#define mbedtls_internal_sha256_process dubhe_internal_sha256_process
#define mbedtls_sha256_starts dubhe_sha256_starts
#define mbedtls_sha256_update dubhe_sha256_update
#define mbedtls_sha256_finish dubhe_sha256_finish
#define mbedtls_sha256_process dubhe_sha256_process

int dubhe_internal_sha256_process( mbedtls_sha256_context *ctx,
                                const unsigned char data[64] );

int dubhe_internal_sha224_process( mbedtls_sha256_context *ctx,
                                const unsigned char data[64] );

#endif /*MBEDTLS_SHA256_ALT*/

#ifdef __cplusplus
}
#endif

#endif /*_SHA256_ALT_H_*/