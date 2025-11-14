/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_asymmetric_signature.c
 *
 * This file contains the implementation of the entry points associated to the
 * asymmetric signature capability as described by the PSA Cryptoprocessor
 * Driver interface specification
 *
 */

#include "cc3xx_psa_asymmetric_signature.h"
#include "cc3xx_internal_drbg_util.h"
#include "cc3xx_internal_ecc_util.h"
#include "cc3xx_internal_rsa_util.h"
#include "cc3xx_psa_hash.h"
#include "cc3xx_internal_hash_util.h"
#include "mbedtls/hmac_drbg.h"
#include "psa/crypto.h"

#include "cc_common.h"
#include "cc_ecc_internal.h"
#include "cc_ecpki_build.h"
#include "cc_ecpki_domain.h"
#include "cc_ecpki_ecdsa.h"
#include "cc_ecpki_local.h"
#include "cc_pal_abort.h"
#include "cc_pal_mem.h"
#include "cc_pal_types.h"
#include "cc_rsa_schemes.h"
#include "cc_rsa_types.h"
#include "ecp_common.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>

#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

/* FixMe: Currently, some parts of the low-level driver are
 *        are not built at all based on the mbed TLS configuration,
 *        hence they can't be called from the interface code.
 *        Eventually, the low level driver should be made
 *        independent of the mbed TLS configuration and the
 *        interface layer should be the only part that should
 *        be configured through defines
 */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

static psa_status_t cc3xx_internal_ecdsa_verify(
    const psa_key_attributes_t *attributes,
    const uint8_t *key, size_t key_length,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_len,
    const uint8_t *signature,
    size_t signature_length,
    bool do_hashing)
{
    CCEcpkiHashOpMode_t hash_mode = CC_ECPKI_HASH_OpModeLast;
    CCEcdsaVerifyUserContext_t pVerifyUserContext;
    CCEcpkiUserPublKey_t pUserPublKey;
    CCEcpkiUserPrivKey_t pUserPrivKey;
    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_type_t key_bits = psa_get_key_bits(attributes);
    psa_ecc_family_t curve = PSA_KEY_TYPE_ECC_GET_FAMILY(key_type);
    CCEcpkiDomainID_t domainId;
    const CCEcpkiDomain_t *pDomain;
    CCEcpkiBuildTempData_t temp_data;
    psa_status_t err = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t cc_err;

    err = cc3xx_psa_hash_mode_to_cc_hash_mode(alg, do_hashing, &hash_mode);
    if (err != PSA_SUCCESS) {
        return err;
    }

    err = cc3xx_ecc_psa_domain_to_cc_domain(curve, key_bits, &domainId);
    if (err != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("Error - curve is not supported\n");
        return err;
    }

    pDomain = CC_EcpkiGetEcDomain(domainId);
    if (NULL == pDomain) {
        CC_PAL_LOG_ERR("Error - domain id %d is not supported\n", domainId);
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Check if the signature buffer is not of the expected size */
    if (signature_length != 2*CALC_FULL_BYTES(pDomain->ordSizeInBits)) {
        return PSA_ERROR_INVALID_SIGNATURE;
    }

    /* FIXME: From here the code is only applicable to weistrass and koblitz
     * curves, but needs to be extended to support Ed25519 when available
     */
    if (PSA_KEY_TYPE_IS_KEY_PAIR(key_type)) {
        err = cc3xx_ecc_psa_priv_to_cc_priv(domainId, key, key_length,
                                            &pUserPrivKey);
        if (err == PSA_SUCCESS) {
            err = cc3xx_ecc_cc_priv_to_cc_publ(&pUserPrivKey, &pUserPublKey);
        }
    } else {
        cc_err = CC_EcpkiPublKeyBuildAndCheck(pDomain, (uint8_t *)key,
                                              key_length, CheckPointersAndSizesOnly,
                                              &pUserPublKey, &temp_data);
        err = cc3xx_ecc_cc_error_to_psa_error(cc_err);
    }

    if (err != PSA_SUCCESS) {
        CC_PAL_LOG_ERR(
            "Error - building public key failed with return code 0x%08x\n",
            cc_err);
        return err;
    }

    cc_err = CC_EcdsaVerify(&pVerifyUserContext, &pUserPublKey, hash_mode,
                            (uint8_t *)signature, signature_length,
                            (uint8_t *)input, input_len);
    err = cc3xx_ecc_cc_error_to_psa_error(cc_err);

    if (err != PSA_SUCCESS) {
        CC_PAL_LOG_ERR(
            "Error - ecdsa signacture verification failed with return "
            "code 0x%08x\n",
            cc_err);
        return err;
    }

    return err;
}

static psa_status_t cc3xx_internal_ecdsa_sign(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_length, psa_algorithm_t alg, const uint8_t *input,
    size_t input_len, uint8_t *signature, size_t signature_size,
    size_t *signature_length, bool do_hashing)

{
    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_type_t key_bits = psa_get_key_bits(attributes);
    psa_ecc_family_t curve = PSA_KEY_TYPE_ECC_GET_FAMILY(key_type);
    psa_status_t err = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t cc_err;
    CCEcpkiDomainID_t domainId;
    const CCEcpkiDomain_t *pDomain;
    CCEcdsaSignUserContext_t pSignUserContext;
    EcdsaSignContext_t *pWorkingContext;
    CCEcpkiUserPrivKey_t pUserPrivKey;
    CCRndContext_t rnd_ctx;
    CCEcpkiHashOpMode_t hash_mode = CC_ECPKI_HASH_OpModeLast;
    mbedtls_hmac_drbg_context hmac_drbg_ctx;

    err = cc3xx_psa_hash_mode_to_cc_hash_mode(alg, do_hashing, &hash_mode);
    if (err != PSA_SUCCESS) {
        return err;
    }

    pWorkingContext = (EcdsaSignContext_t *)&pSignUserContext.context_buff;
    CC_PalMemSetZero(pWorkingContext, sizeof(EcdsaSignContext_t));

    /* FixMe: 521 bits keys are not producing the correct result with CC3XX */
    if (key_bits == 521) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    err = cc3xx_ecc_psa_domain_to_cc_domain(curve, key_bits, &domainId);
    if (err != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("Error - curve is not supported\n");
        return err;
    }

    pDomain = CC_EcpkiGetEcDomain(domainId);
    if (NULL == pDomain) {
        CC_PAL_LOG_ERR("Error - domain id %d is not supported\n", domainId);
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* The signature consists of two numbers with each having the length of the
     * private key */
    if (signature_size < (2*PSA_BITS_TO_BYTES(pDomain->ordSizeInBits))) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    cc_err =
        CC_EcpkiPrivKeyBuild(pDomain, key_buffer, key_length, &pUserPrivKey);

    err = cc3xx_ecc_cc_error_to_psa_error(cc_err);

    if (err != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("Error building private key with error code %d\n",
                       cc_err);
        goto cleanup;
    }

    if (PSA_ALG_ECDSA_IS_DETERMINISTIC(alg)) {
        /* When deterministic ECDSA is used we don't support
         * doing the hashing at the same time. We need alwas have
         * the hash as the input.
         */
        if (do_hashing) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        err = cc3xx_hmac_drbg_init_with_params(&hmac_drbg_ctx, input, input_len,
                                               key_buffer, key_length, pDomain);
        if (err != PSA_SUCCESS) {
            goto cleanup;
        }

        err = cc3xx_hmac_drbg_get_ctx(&rnd_ctx, &hmac_drbg_ctx);
    } else {
        err = cc3xx_ctr_drbg_get_ctx(&rnd_ctx);
    }

    if (err != PSA_SUCCESS) {
        goto cleanup;
    }

    *signature_length = signature_size;
    /*****  EcdsaSignFinish  ********/
    cc_err =
        CC_EcdsaSign(&rnd_ctx, &pSignUserContext, &pUserPrivKey, hash_mode,
                     (uint8_t *)input, input_len, signature, signature_length);

    err = cc3xx_ecc_cc_error_to_psa_error(cc_err);
    if (err != PSA_SUCCESS) {
        *signature_length = 0;
        CC_PAL_LOG_ERR("Error ecdsa signing with error code %d\n", cc_err);
        goto cleanup;
    }

cleanup:
    return err;
}

static psa_status_t cc3xx_internal_rsa_verify(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_length, psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    const uint8_t *signature,
    size_t signature_length, bool do_hashing)

{
#ifndef MBEDTLS_RSA_C
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t hash_bytes = PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg));
    psa_status_t err = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t cc_err = CC_FAIL;
    CCRsaPubUserContext_t *pPubUserContext;
    CCRsaUserPubKey_t *pUserPubKey;
    CCRsaHashOpMode_t hash_mode = CC_RSA_HASH_OpModeLast;

    err = cc3xx_psa_hash_mode_to_cc_hash_mode(alg, do_hashing, &hash_mode);
    if (err != PSA_SUCCESS) {
        return err;
    }

    pPubUserContext = (CCRsaPubUserContext_t *)mbedtls_calloc(
        1, sizeof(CCRsaPubUserContext_t));
    if (pPubUserContext == NULL) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    pUserPubKey =
        (CCRsaUserPubKey_t *)mbedtls_calloc(1, sizeof(CCRsaUserPubKey_t));
    if (pUserPubKey == NULL) {
        mbedtls_free(pPubUserContext);
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    if (PSA_KEY_TYPE_IS_KEY_PAIR(key_type)) {
        err = cc3xx_rsa_psa_priv_to_cc_pub(key_buffer, key_length, pUserPubKey);
    } else {
        err = cc3xx_rsa_psa_pub_to_cc_pub(key_buffer, key_length, pUserPubKey);
    }
    if (err != PSA_SUCCESS) {
        goto cleanup;
    }

    CCRsaPubKey_t *pPubKey = (CCRsaPubKey_t *)pUserPubKey->PublicKeyDbBuff;
    if (signature_length != CALC_FULL_BYTES(pPubKey->nSizeInBits)) {
        err = PSA_ERROR_INVALID_SIGNATURE;
        goto cleanup;
    }

    if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg)) {
        cc_err = CC_RsaPkcs1V15Verify(pPubUserContext, pUserPubKey, hash_mode,
                                      (uint8_t *)input, input_length,
                                      (uint8_t *)signature);
    } else if (PSA_ALG_IS_RSA_PSS(alg)) {
        cc_err = CC_RsaPssVerify(pPubUserContext, pUserPubKey, hash_mode,
                                 CC_PKCS1_MGF1, hash_bytes, (uint8_t *)input,
                                 input_length, (uint8_t *)signature);
    }

    err = cc3xx_rsa_cc_error_to_psa_error(cc_err);

cleanup:
    CC_PalMemSetZero(pPubUserContext, sizeof(CCRsaPubUserContext_t));
    CC_PalMemSetZero(pUserPubKey, sizeof(CCRsaUserPubKey_t));
    mbedtls_free(pPubUserContext);
    mbedtls_free(pUserPubKey);
    return err;
#endif /* MBEDTLS_RSA_C */
}

static psa_status_t cc3xx_internal_rsa_sign(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_length, psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    uint8_t *signature, size_t signature_size,
    size_t *signature_length, bool do_hashing)
{
#ifndef MBEDTLS_RSA_C
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_key_type_t key_bits = psa_get_key_bits(attributes);
    psa_status_t err = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t cc_err = CC_FATAL_ERROR;
    CCRndContext_t rnd_ctx;
    CCRsaPrivUserContext_t *user_context_ptr;
    CCRsaUserPrivKey_t *user_priv_key_ptr;
    CCRsaHashOpMode_t hash_mode = CC_RSA_HASH_OpModeLast;

    if (PSA_ALG_IS_RSA_PSS(alg)) {
        if (PSA_BITS_TO_BYTES(key_bits) <
            2 * PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg))) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    }

    err = cc3xx_psa_hash_mode_to_cc_hash_mode(alg, do_hashing, &hash_mode);
    if (err != PSA_SUCCESS) {
        return err;
    }

    user_priv_key_ptr =
        (CCRsaUserPrivKey_t *)mbedtls_calloc(1, sizeof(CCRsaUserPrivKey_t));
    if (user_priv_key_ptr == NULL) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    user_context_ptr = (CCRsaPrivUserContext_t *)mbedtls_calloc(
        1, sizeof(CCRsaPrivUserContext_t));
    if (user_context_ptr == NULL) {
        mbedtls_free(user_priv_key_ptr);
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    err = cc3xx_rsa_psa_priv_to_cc_priv(key_buffer, key_length,
                                           user_priv_key_ptr);
    if (err != PSA_SUCCESS) {
        cc_err = CC_FAIL;
        goto cleanup;
    }

    err = cc3xx_ctr_drbg_get_ctx(&rnd_ctx);
    if (err != PSA_SUCCESS) {
        cc_err = CC_FAIL;
        goto cleanup;
    }

    *signature_length = signature_size;

    if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg)) {
        cc_err =
            CC_RsaPkcs1V15Sign(&rnd_ctx, user_context_ptr, user_priv_key_ptr,
                               hash_mode, (uint8_t *)input, input_length,
                               (uint8_t *)signature, signature_length);
    } else if (PSA_ALG_IS_RSA_PSS(alg)) {
        /* Calculate the largest possible salt length. Normally this is the hash
         * length, which is the maximum length the salt can have. If there is
         * not enough room, use the maximum salt length that fits. The
         * constraint is that the hash length plus the salt length plus 2 bytes
         * must be at most the key length. This complies with FIPS 186-4 §5.5
         * (e) and RFC 8017 (PKCS#1 v2.2) §9.1.1 step 3.
         */
        size_t hash_len = PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg));
        size_t output_len = PSA_BITS_TO_BYTES(key_bits);
        size_t salt_len;

        if (output_len >= 2 * hash_len + 2) {
            salt_len = hash_len;
        } else {
            salt_len = output_len - hash_len - 2;
        }

        cc_err =
            CC_RsaPssSign(&rnd_ctx, user_context_ptr, user_priv_key_ptr,
                          hash_mode, CC_PKCS1_MGF1, salt_len, (uint8_t *)input,
                          input_length, (uint8_t *)signature, signature_length);
    }

cleanup:
    CC_PalMemSetZero(user_priv_key_ptr, sizeof(CCRsaUserPrivKey_t));
    CC_PalMemSetZero(user_context_ptr, sizeof(CCRsaPrivUserContext_t));
    mbedtls_free(user_priv_key_ptr);
    mbedtls_free(user_context_ptr);

    return cc3xx_rsa_cc_error_to_psa_error(cc_err);
#endif /* MBEDTLS_RSA_C */
}

/** \defgroup psa_asym_sign PSA driver entry points for asymmetric sign/verify
 *
 *  Entry points for asymmetric message signing and signature verification as
 *  described by the PSA Cryptoprocessor Driver interface specification
 *
 *  @{
 */
psa_status_t cc3xx_sign_hash(const psa_key_attributes_t *attributes,
                             const uint8_t *key, size_t key_length,
                             psa_algorithm_t alg, const uint8_t *input,
                             size_t input_length, uint8_t *signature,
                             size_t signature_size, size_t *signature_length)
{
    if (alg != PSA_ALG_RSA_PKCS1V15_SIGN_RAW &&
        input_length != PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg))) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (!PSA_KEY_TYPE_IS_ASYMMETRIC(psa_get_key_type(attributes))) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (PSA_ALG_IS_ECDSA(alg)) {
        return cc3xx_internal_ecdsa_sign(
            attributes, key, key_length, alg, input, input_length, signature,
            signature_size, signature_length, false);
    } else if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg) || PSA_ALG_IS_RSA_PSS(alg)) {
        return cc3xx_internal_rsa_sign(attributes, key, key_length, alg, input,
                                       input_length, signature, signature_size,
                                       signature_length, false);
    }

    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t cc3xx_verify_hash(const psa_key_attributes_t *attributes,
                               const uint8_t *key, size_t key_length,
                               psa_algorithm_t alg, const uint8_t *hash,
                               size_t hash_length, const uint8_t *signature,
                               size_t signature_length)
{
    if (alg != PSA_ALG_RSA_PKCS1V15_SIGN_RAW &&
        hash_length != PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg))) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (!PSA_KEY_TYPE_IS_ASYMMETRIC(psa_get_key_type(attributes))) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (PSA_ALG_IS_ECDSA(alg)) {
        return cc3xx_internal_ecdsa_verify(attributes, key, key_length, alg,
                                           hash, hash_length, signature,
                                           signature_length, false);
    } else if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg) || PSA_ALG_IS_RSA_PSS(alg)) {
        return cc3xx_internal_rsa_verify(attributes, key, key_length, alg, hash,
                                         hash_length, signature,
                                         signature_length, false);
    }

    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t cc3xx_sign_message(const psa_key_attributes_t *attributes,
                                const uint8_t *key, size_t key_length,
                                psa_algorithm_t alg, const uint8_t *input,
                                size_t input_length, uint8_t *signature,
                                size_t signature_size, size_t *signature_length)
{
    psa_status_t ret;
    uint8_t hash[HASH_SHA512_BLOCK_SIZE_IN_BYTES];
    size_t hash_len;

    if (!PSA_KEY_TYPE_IS_ASYMMETRIC(psa_get_key_type(attributes))) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (PSA_ALG_ECDSA_IS_DETERMINISTIC(alg)) {
        ret = cc3xx_hash_compute(PSA_ALG_SIGN_GET_HASH(alg), input,
                                 input_length, hash, sizeof(hash), &hash_len);
        if (ret != PSA_SUCCESS) {
            return ret;
        }

        return cc3xx_internal_ecdsa_sign(attributes, key, key_length, alg, hash,
                                         hash_len, signature, signature_size,
                                         signature_length, false);
    } else if (PSA_ALG_IS_ECDSA(alg)) {
        return cc3xx_internal_ecdsa_sign(
            attributes, key, key_length, alg, input, input_length, signature,
            signature_size, signature_length, true);
    } else if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg) || PSA_ALG_IS_RSA_PSS(alg)) {
        return cc3xx_internal_rsa_sign(attributes, key, key_length, alg, input,
                                       input_length, signature, signature_size,
                                       signature_length, true);
    }

    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t cc3xx_verify_message(const psa_key_attributes_t *attributes,
                                  const uint8_t *key, size_t key_length,
                                  psa_algorithm_t alg, const uint8_t *input,
                                  size_t input_length, const uint8_t *signature,
                                  size_t signature_length)
{
    if (!PSA_KEY_TYPE_IS_ASYMMETRIC(psa_get_key_type(attributes))) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (PSA_ALG_IS_ECDSA(alg)) {
        return cc3xx_internal_ecdsa_verify(attributes, key, key_length, alg,
                                           input, input_length, signature,
                                           signature_length, true);
    } else if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg) || PSA_ALG_IS_RSA_PSS(alg)) {
        return cc3xx_internal_rsa_verify(attributes, key, key_length, alg,
                                         input, input_length, signature,
                                         signature_length, true);
    }

    return PSA_ERROR_NOT_SUPPORTED;
}
/** @} */ // end of psa_asym_sign
