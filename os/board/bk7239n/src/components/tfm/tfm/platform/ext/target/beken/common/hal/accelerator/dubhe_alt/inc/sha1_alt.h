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
 *  The SHA-1 standard was published by NIST in 1993.
 *
 *  http://www.itl.nist.gov/fipspubs/fip180-1.htm
 */
#ifndef _SHA1_ALT_H_
#define _SHA1_ALT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_SHA1_ALT)
#include "dubhe_hash.h"

#define MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED                  -0x0035  /**< SHA-1 hardware accelerator failed */

#if defined(ARM_CE_DUBHE_HASH)
#define MBEDTLS_ERR_SHA1_CTX_NULL       -0x00EC
#define MBEDTLS_ERR_SHA1_INPUT_LEN      -0x00ED
#define MBEDTLS_ERR_SHA1_NOMEM          -0x00EE
#define MBEDTLS_ERR_SHA1_CE_INIT        -0x00EF
#endif

/**
 * \brief          SHA-1 context structure
 */
typedef arm_ce_hash_context_t mbedtls_sha1_context;

#define mbedtls_sha1_init dubhe_sha1_init
#define mbedtls_sha1_free dubhe_sha1_free
#define mbedtls_sha1_clone dubhe_sha1_clone
#define mbedtls_sha1_starts_ret dubhe_sha1_starts_ret
#define mbedtls_sha1_update_ret dubhe_sha1_update_ret
#define mbedtls_sha1_finish_ret dubhe_sha1_finish_ret
#define mbedtls_internal_sha1_process dubhe_internal_sha1_process
#define mbedtls_sha1_starts dubhe_sha1_starts
#define mbedtls_sha1_update dubhe_sha1_update
#define mbedtls_sha1_finish dubhe_sha1_finish
#define mbedtls_sha1_process dubhe_sha1_process

#endif /*MBEDTLS_SHA1_ALT */

#ifdef __cplusplus
}
#endif

#endif /*_SHA1_ALT_H_*/