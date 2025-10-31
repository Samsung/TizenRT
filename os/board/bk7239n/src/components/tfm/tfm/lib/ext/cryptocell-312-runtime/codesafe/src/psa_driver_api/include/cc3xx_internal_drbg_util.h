/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_drbg_util.h
 *
 * This file contains the declarations of the internal functions to provide
 * a DRBG, i.e. Deterministic Random Binary Generator based on algorithms as
 * CTR and HMAC, plus to handle entropy contexts. This file has a direct
 * dependency on mbedTLS related objects
 *
 */

#ifndef CC3XX_INTERNAL_DRBG_UTIL_H
#define CC3XX_INTERNAL_DRBG_UTIL_H

#include "psa/crypto.h"

#include "cc_ecpki_types.h"
#include "cc_rnd_common.h"

#include "mbedtls/hmac_drbg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   Initialises the low-evel random context with a global CTR-DRBG
 *          context that can be used by the caller.
 *
 * \param[out] rnd_ctx A pointer to the low-level random context of type
 *                     \ref CCRndContext_t
 *
 * \retval PSA_SUCCESS or a value of type \ref psa_status_t
 */
psa_status_t cc3xx_ctr_drbg_get_ctx(CCRndContext_t *rnd_ctx);

/**
 * \brief  Initialises the low-level random context with the HMAC-DRBG context
 *         that is passed as input, and make it usable by the caller
 *
 * \param[out] rnd_ctx       A pointer to the low-level random context of type
 *                           \ref CCRndContext_t
 * \param[in]  hmac_drbg_ctx A pointer to the HMAC-DRBG context to be used as
 *                           input for the initialisation
 *
 * \retval PSA_SUCCESS or a value of type \ref psa_status_t
 */
psa_status_t cc3xx_hmac_drbg_get_ctx(CCRndContext_t *rnd_ctx,
                                     mbedtls_hmac_drbg_context *hmac_drbg_ctx);
/**
 * \brief Initialises the HMAC-DRBG context with the specified parameters
 *
 * \param[out] hmac_drbg_ctx Pointer to the HMAC-DRBG context to be initialised
 * \param[in]  hash          Buffer holding the hash to be used as input for
 *                           the seed
 * \param[in]  hash_len      Size in bytes of the hash
 * \param[in]  key_buffer    Buffer holding the key material to be used as
 *                           input for the seed
 * \param[in]  key_len       Size in bytes of the key_buffer
 * \param[in]  pDomain       Pointer to \ref CCEcpkiDomain_t that holds the
 *                           parameter of the elliptic curve
 *
 * \retval PSA_SUCCESS or a value of type \ref psa_status_t
 */
psa_status_t cc3xx_hmac_drbg_init_with_params(mbedtls_hmac_drbg_context *hmac_drbg_ctx,
                                              const uint8_t *hash,
                                              size_t hash_len,
                                              const uint8_t *key_buffer,
                                              size_t key_len,
                                              const CCEcpkiDomain_t *pDomain);
#ifdef __cplusplus
}
#endif
#endif /* CC3XX_INTERNAL_DRBG_UTIL_H */
