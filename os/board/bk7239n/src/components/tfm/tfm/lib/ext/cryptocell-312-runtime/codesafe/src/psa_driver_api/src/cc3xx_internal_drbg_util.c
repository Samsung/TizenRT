/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_drbg_util.c
 *
 * This file contains the implementation of the internal functions to provide
 * a DRBG, i.e. Deterministic Random Binary Generator based on algorithms as
 * CTR and HMAC, plus to handle entropy contexts. This file has a direct
 * dependency on mbedTLS related objects
 *
 */

#include "cc3xx_internal_drbg_util.h"
#include "cc_common.h"
#include "cc_common_math.h"
#include "cc_ecpki_types.h"
#include "cc_pal_mem.h"
#include "cc_rnd_common.h"
#include "psa/crypto.h"

#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/hmac_drbg.h"

#define CC3XX_CTR_DRBG_INITIALIZED 0x5A44A5A8

/** \defgroup internal_drbg_util Internal DRBG objects
 *
 *  Internal functions and global objects required to provide DRBG
 *  functionalities. It has a direct dependency on mbedTLS CTR/DRBG/entropy
 *  objects
 *
 *  @{
 */
mbedtls_ctr_drbg_context global_ctr_drbg_ctx;
mbedtls_hmac_drbg_context global_hmac_drbg_ctx;
mbedtls_entropy_context global_entropy_ctx;

size_t cc3xx_ctr_drbg_initialized;
size_t cc3xx_hmac_drbg_initialized;

psa_status_t cc3xx_ctr_drbg_get_ctx(CCRndContext_t *rnd_ctx)
{

    int error;
    CCError_t cc_error;

    if (cc3xx_ctr_drbg_initialized != CC3XX_CTR_DRBG_INITIALIZED) {
        CC_PalMemSetZero(&global_ctr_drbg_ctx,
                         sizeof(mbedtls_ctr_drbg_context));
        CC_PalMemSetZero(&global_entropy_ctx, sizeof(mbedtls_entropy_context));

        mbedtls_ctr_drbg_init(&global_ctr_drbg_ctx);
        mbedtls_entropy_init(&global_entropy_ctx);

        error =
            mbedtls_ctr_drbg_seed(&global_ctr_drbg_ctx, mbedtls_entropy_func,
                                  &global_entropy_ctx, NULL, 0);
        if (error != 0) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        cc3xx_ctr_drbg_initialized = CC3XX_CTR_DRBG_INITIALIZED;
    }

    rnd_ctx->rndState = &global_ctr_drbg_ctx;
    rnd_ctx->entropyCtx = &global_entropy_ctx;

    cc_error = CC_RndSetGenerateVectorFunc(rnd_ctx, mbedtls_ctr_drbg_random);
    if (cc_error != 0) {
        cc3xx_ctr_drbg_initialized = 0;
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

psa_status_t
cc3xx_hmac_drbg_init_with_params(mbedtls_hmac_drbg_context *hmac_drbg_ctx,
                                 const uint8_t *hash, size_t hash_len,
                                 const uint8_t *key_buffer, size_t key_len,
                                 const CCEcpkiDomain_t *pDomain)
{

    int error;
    /* This seed should be double the size of the maximum order size to store
     * the hash +  */
    uint8_t seed_buff[CC_ECPKI_MODUL_MAX_LENGTH_IN_WORDS * 4 * 2] = {0};
    size_t hash_bits = PSA_BYTES_TO_BITS(hash_len);
    size_t hash_bytes_used = 0;
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    if (hmac_drbg_ctx == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    mbedtls_hmac_drbg_init(hmac_drbg_ctx);

    if (hash_bits > pDomain->ordSizeInBits) {
        hash_bytes_used = PSA_BITS_TO_BYTES(pDomain->ordSizeInBits);
    } else {
        hash_bytes_used = hash_len;
    }

    if (2 * key_len > sizeof(seed_buff)) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    /* FIXME: A test is needed to verify that the seed_buff < pDomain->ecR */
    CC_PalMemCopy(seed_buff + key_len + (key_len - hash_bytes_used), hash,
                  hash_bytes_used);

    CC_PalMemCopy(seed_buff, key_buffer, key_len);

    error = mbedtls_hmac_drbg_seed_buf(hmac_drbg_ctx, md_info, seed_buff, 2 * key_len);
    if (error != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_hmac_drbg_get_ctx(CCRndContext_t *rnd_ctx,
                                     mbedtls_hmac_drbg_context *hmac_drbg_ctx)
{

    int error;

    if (hmac_drbg_ctx == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    } else {
        rnd_ctx->rndState = hmac_drbg_ctx;
    }

    rnd_ctx->entropyCtx = &global_entropy_ctx;

    error = CC_RndSetGenerateVectorFunc(rnd_ctx, mbedtls_hmac_drbg_random);
    if (error != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}
/** @} */ // end of internal_drbg_util
