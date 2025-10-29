/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_key_agreement.c
 *
 * This file contains the implementation of the entry points associated to the
 * raw key agreement (i.e. ECDH) as described by the PSA Cryptoprocessor Driver
 * interface specification
 *
 */

#include "cc3xx_psa_key_agreement.h"
#include "cc3xx_internal_ecdh.h"
#include "cc3xx_internal_ecc_util.h"

/** \defgroup psa_key_agreement PSA driver entry points for raw key agreement
 *
 *  Entry points for raw key agreement as described by the PSA Cryptoprocessor
 *  Driver interface specification
 *
 *  @{
 */
psa_status_t cc3xx_key_agreement(
        const psa_key_attributes_t *attributes,
        const uint8_t *priv_key, size_t priv_key_size,
        const uint8_t *publ_key, size_t publ_key_size,
        uint8_t *output, size_t output_size, size_t *output_length,
        psa_algorithm_t alg)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_bits_t key_bits = psa_get_key_bits(attributes);
    psa_ecc_family_t curve = PSA_KEY_TYPE_ECC_GET_FAMILY(key_type);

    cc3xx_ecdh_context_t ctx;

    switch (alg) {
    case PSA_ALG_ECDH:
        /* CC-312 only supports the X25519 function w/bit size m = 256 */
        if (curve == PSA_ECC_FAMILY_MONTGOMERY && key_bits == 256) {
            if (( ret = cc3xx_ecdh_calc_secret_mont(
                    priv_key,
                    priv_key_size,
                    publ_key,
                    publ_key_size,
                    output,
                    &output_size) )
                    != PSA_SUCCESS) {
                *output_length = 0;
                return ret;
            }
        } else {
            if (( ret = cc3xx_ecc_psa_domain_to_cc_domain(
                    curve,
                    key_bits,
                    &ctx.domain_id) )
                    != PSA_SUCCESS) {
                *output_length = 0;
                return ret;
            }

            if (( ret = cc3xx_ecc_psa_priv_to_cc_priv(
                    ctx.domain_id,
                    priv_key,
                    priv_key_size,
                    &ctx.priv_key) )
                    != PSA_SUCCESS) {
                *output_length = 0;
                return ret;
            }

            if (( ret = cc3xx_ecc_psa_publ_to_cc_publ(
                    ctx.domain_id,
                    publ_key,
                    publ_key_size,
                    &ctx.publ_key) )
                    != PSA_SUCCESS) {
                *output_length = 0;
                return ret;
            }

            if (( ret = cc3xx_ecdh_calc_secret_wrst(
                    &ctx,
                    output,
                    &output_size) )
                    != PSA_SUCCESS) {
                *output_length = 0;
                return ret;
            }
        }

        *output_length = output_size;

        break;
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return ret;
}
/** @} */ // end of psa_key_agreement
