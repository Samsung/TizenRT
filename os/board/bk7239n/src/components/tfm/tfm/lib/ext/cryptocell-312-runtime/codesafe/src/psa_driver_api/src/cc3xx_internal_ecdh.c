/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_ecdh.c
 *
 * This file contains the implementation of the internal functions required to
 * perform Elliptic Curve Diffie-Hellman, i.e. ECDH raw key agreement
 *
 */

#include "cc3xx_internal_ecdh.h"
#include <cc_ecpki_dh.h>
#include <cc_ec_mont_api.h>
#include <cc_pal_log.h>
#include <cc_pal_mem.h>

/** \defgroup internal_ecdh_util Internal ECDH functions
 *
 *  Internal functions required to implement the Diffie-Hellman protocol based
 *  on Elliptic Curves, i.e. ECDH
 *
 *  @{
 */
psa_status_t cc3xx_ecdh_calc_secret_wrst(
        cc3xx_ecdh_context_t *ctx,
        uint8_t *secret, size_t *secret_size)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t status;
    CCEcdhTempData_t EcdhTempData;

    if (NULL == ctx || NULL == secret) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        ret = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    status = CC_EcdhSvdpDh(&ctx->publ_key,
                           &ctx->priv_key,
                           secret,
                           secret_size,
                           &EcdhTempData);
    if (status != CC_OK) {
        CC_PAL_LOG_ERR("Error calculating shared secret value\n");
        ret = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    ret = PSA_SUCCESS;

exit:
    CC_PalMemSetZero(&EcdhTempData, sizeof(CCEcdhTempData_t));
    return ret;
}

psa_status_t cc3xx_ecdh_calc_secret_mont(
        const uint8_t *priv_key, size_t priv_key_size,
        const uint8_t *publ_key, size_t publ_key_size,
        uint8_t *secret, size_t *secret_size)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t status;
    CCEcMontTempBuff_t EcMontTempBuff;

    if (NULL == priv_key || NULL == publ_key || NULL == secret) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        ret = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    status = CC_EcMontScalarmult(secret,
                                 secret_size,
                                 priv_key,
                                 priv_key_size,
                                 publ_key,
                                 publ_key_size,
                                 &EcMontTempBuff);
    if (status != CC_OK) {
        CC_PAL_LOG_ERR("Error calculating shared secret value\n");
        ret = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    ret = PSA_SUCCESS;

exit:
    CC_PalMemSetZero(&EcMontTempBuff, sizeof(CCEcMontTempBuff_t));
    return ret;
}
/** @} */ // end of internal_ecdh_util
