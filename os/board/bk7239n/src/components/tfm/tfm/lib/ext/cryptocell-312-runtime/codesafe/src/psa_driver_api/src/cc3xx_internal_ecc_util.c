/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_internal_ecc_util.h"
#include "psa/crypto.h"

#include "cc_ecpki_error.h"
#include "cc_pal_abort.h"
#include "cc_pal_mem.h"
#include "cc_pal_types.h"

#include "cc_ecpki_build.h"
#include "cc_ecpki_domain.h"
#include "cc_ecpki_local.h"
#include "pka_ec_wrst.h"

#include "mbedtls_cc_ec_mont_edw_error.h"

/** \file cc3xx_internal_ecc_util.c
 *
 * This file contains the implementation of the internal functions and utilities
 * required to translate between PSA and CC low-level Elliptic Curve types and
 * format conversions
 *
 */

/** \defgroup internal_ecc_util Internal ECC utility functions
 *
 *  Internal functions required to provide utilities for handling ECC type
 *  conversions
 *
 *  @{
 */
psa_status_t cc3xx_ecc_cc_error_to_psa_error(CCError_t cc_error)
{
#ifndef CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION
    /* Limited coverage error code conversion routine */
    switch (cc_error) {
    case CC_SUCCESS:
        return PSA_SUCCESS;
    case CC_ECDSA_VERIFY_INCONSISTENT_VERIFY_ERROR:
    case CC_EC_EDW_SIGN_VERIFY_FAILED_ERROR:
        return PSA_ERROR_INVALID_SIGNATURE;
    default:
        return PSA_ERROR_HARDWARE_FAILURE;
    }
#else /* CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION */
    psa_status_t err;

    switch (cc_error) {
    case CC_SUCCESS:
        return PSA_SUCCESS;
        break;

    case CC_ECPKI_ILLEGAL_DOMAIN_ID_ERROR:
    case CC_ECPKI_BUILD_KEY_ILLEGAL_DOMAIN_ID_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_ILLEGAL_DOMAIN_ID_ERROR:
    case CC_ECPKI_BUILD_DOMAIN_ID_IS_NOT_VALID_ERROR:
    case CC_ECDH_SVDP_DH_ILLEGAL_DOMAIN_ID_ERROR:
    case CC_ECDSA_SIGN_INVALID_DOMAIN_ID_ERROR:
    case CC_ECDSA_VERIFY_INVALID_DOMAIN_ID_ERROR:
    case CC_ECPKI_INVALID_DOMAIN_ID_ERROR:
    case CC_EC_MONT_IS_NOT_SUPPORTED:
    case CC_EC_EDW_IS_NOT_SUPPORTED:

        err = PSA_ERROR_NOT_SUPPORTED;
        break;

    case CC_ECPKI_INTERNAL_ERROR:
    case CC_ECDH_SVDP_DH_PARTNER_PUBL_KEY_VALID_TAG_ERROR:
    case CC_ECDH_SVDP_DH_USER_PRIV_KEY_VALID_TAG_ERROR:
    case CC_ECDH_SVDP_DH_NOT_CONCENT_PUBL_AND_PRIV_DOMAIN_ID_ERROR:
    case CC_ECDSA_SIGN_INVALID_IS_EPHEMER_KEY_INTERNAL_ERROR:
    case CC_ECDSA_SIGN_USER_PRIV_KEY_VALIDATION_TAG_ERROR:
    case CC_ECDSA_VERIFY_SIGNER_PUBL_KEY_VALIDATION_TAG_ERROR:
    case CC_ECPKI_INVALID_PRIV_KEY_TAG_ERROR:
    case CC_ECPKI_INVALID_PUBL_KEY_TAG_ERROR:
    case CC_ECIES_INVALID_PUBL_KEY_TAG_ERROR:
    case CC_ECIES_INVALID_PRIV_KEY_TAG_ERROR:
    case CC_ECIES_INVALID_PRIV_KEY_VALUE_ERROR:

        err = PSA_ERROR_INVALID_ARGUMENT;
        break;

    case CC_ECDSA_VERIFY_INCONSISTENT_VERIFY_ERROR:
    case CC_EC_EDW_SIGN_VERIFY_FAILED_ERROR:
        err = PSA_ERROR_INVALID_SIGNATURE;
        break;

    case CC_ECPKI_DOMAIN_PTR_ERROR:
    case CC_ECPKI_GEN_KEY_INVALID_PRIVATE_KEY_PTR_ERROR:
    case CC_ECPKI_GEN_KEY_INVALID_PUBLIC_KEY_PTR_ERROR:
    case CC_ECPKI_GEN_KEY_INVALID_TEMP_DATA_PTR_ERROR:
    case CC_ECPKI_RND_CONTEXT_PTR_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_COMPRESSION_MODE_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_IN_PTR_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_USER_PRIV_KEY_PTR_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_SIZE_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_DATA_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_PUBL_KEY_IN_PTR_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_USER_PUBL_KEY_PTR_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_PUBL_KEY_SIZE_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_PUBL_KEY_DATA_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_CHECK_MODE_ERROR:
    case CC_ECPKI_BUILD_KEY_INVALID_TEMP_BUFF_PTR_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_INVALID_USER_PUBL_KEY_PTR_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_ILLEGAL_COMPRESSION_MODE_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_INVALID_EXTERN_PUBL_KEY_PTR_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_INVALID_PUBL_KEY_SIZE_PTR_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_INVALID_PUBL_KEY_SIZE_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_ILLEGAL_VALIDATION_TAG_ERROR:
    case CC_ECPKI_EXPORT_PUBL_KEY_INVALID_PUBL_KEY_DATA_ERROR:
    case CC_ECPKI_BUILD_DOMAIN_DOMAIN_PTR_ERROR:
    case CC_ECPKI_BUILD_DOMAIN_EC_PARAMETR_PTR_ERROR:
    case CC_ECPKI_BUILD_DOMAIN_EC_PARAMETR_SIZE_ERROR:
    case CC_ECPKI_BUILD_DOMAIN_COFACTOR_PARAMS_ERROR:
    case CC_ECPKI_BUILD_DOMAIN_SECURITY_STRENGTH_ERROR:
    case CC_ECPKI_BUILD_SCA_RESIST_ILLEGAL_MODE_ERROR:
    case CC_ECDH_SVDP_DH_INVALID_PARTNER_PUBL_KEY_PTR_ERROR:
    case CC_ECDH_SVDP_DH_INVALID_USER_PRIV_KEY_PTR_ERROR:
    case CC_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_PTR_ERROR:
    case CC_ECDH_SVDP_DH_INVALID_TEMP_DATA_PTR_ERROR:
    case CC_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_PTR_ERROR:
    case CC_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_ERROR:
    case CC_ECDSA_SIGN_INVALID_USER_CONTEXT_PTR_ERROR:
    case CC_ECDSA_SIGN_INVALID_USER_PRIV_KEY_PTR_ERROR:
    case CC_ECDSA_SIGN_ILLEGAL_HASH_OP_MODE_ERROR:
    case CC_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_PTR_ERROR:
    case CC_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_SIZE_ERROR:
    case CC_ECDSA_SIGN_USER_CONTEXT_VALIDATION_TAG_ERROR:
    case CC_ECDSA_SIGN_INVALID_SIGNATURE_OUT_PTR_ERROR:
    case CC_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_PTR_ERROR:
    case CC_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_ERROR:
    case CC_ECDSA_SIGN_INVALID_EPHEMERAL_KEY_PTR_ERROR:
    case CC_ECDSA_SIGN_INVALID_RND_CONTEXT_PTR_ERROR:
    case CC_ECDSA_SIGN_INVALID_RND_FUNCTION_PTR_ERROR:
    case CC_ECDSA_SIGN_SIGNING_ERROR:
    case CC_ECDSA_VERIFY_INVALID_USER_CONTEXT_PTR_ERROR:
    case CC_ECDSA_VERIFY_INVALID_SIGNER_PUBL_KEY_PTR_ERROR:
    case CC_ECDSA_VERIFY_ILLEGAL_HASH_OP_MODE_ERROR:
    case CC_ECDSA_VERIFY_INVALID_SIGNATURE_IN_PTR_ERROR:
    case CC_ECDSA_VERIFY_INVALID_SIGNATURE_SIZE_ERROR:
    case CC_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_PTR_ERROR:
    case CC_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_SIZE_ERROR:
    case CC_ECDSA_VERIFY_USER_CONTEXT_VALIDATION_TAG_ERROR:
    case CC_ECC_ILLEGAL_HASH_MODE_ERROR:
    case CC_ECPKI_INVALID_RND_FUNC_PTR_ERROR:
    case CC_ECPKI_INVALID_RND_CTX_PTR_ERROR:
    case CC_ECPKI_INVALID_DATA_IN_PASSED_STRUCT_ERROR:
    case CC_ECPKI_INVALID_BASE_POINT_PTR_ERROR:
    case CC_ECIES_INVALID_PUBL_KEY_PTR_ERROR:
    case CC_ECIES_INVALID_PRIV_KEY_PTR_ERROR:
    case CC_ECIES_INVALID_KDF_DERIV_MODE_ERROR:
    case CC_ECIES_INVALID_KDF_HASH_MODE_ERROR:
    case CC_ECIES_INVALID_SECRET_KEY_PTR_ERROR:
    case CC_ECIES_INVALID_SECRET_KEY_SIZE_ERROR:
    case CC_ECIES_INVALID_CIPHER_DATA_PTR_ERROR:
    case CC_ECIES_INVALID_CIPHER_DATA_SIZE_PTR_ERROR:
    case CC_ECIES_INVALID_CIPHER_DATA_SIZE_ERROR:
    case CC_ECIES_INVALID_TEMP_DATA_PTR_ERROR:
    case CC_ECIES_INVALID_EPHEM_KEY_PAIR_PTR_ERROR:
    case CC_EC_EDW_INVALID_INPUT_POINTER_ERROR:
    case CC_EC_EDW_INVALID_INPUT_SIZE_ERROR:
    case CC_EC_EDW_INVALID_SCALAR_SIZE_ERROR:
    case CC_EC_EDW_INVALID_SCALAR_DATA_ERROR:
    case CC_EC_EDW_RND_CONTEXT_PTR_INVALID_ERROR:
    case CC_EC_EDW_RND_GEN_VECTOR_FUNC_ERROR:
    case CC_EC_MONT_INVALID_INPUT_POINTER_ERROR:
    case CC_EC_MONT_INVALID_INPUT_SIZE_ERROR:
    case CC_EC_MONT_INVALID_DOMAIN_ID_ERROR:
    case CC_ECEDW_INTERNAL_ERROR:
    case CC_ECMONT_INTERNAL_ERROR:

        err = PSA_ERROR_INVALID_ARGUMENT;
        break;

    default:
        err = PSA_ERROR_GENERIC_ERROR;
        CC_PAL_LOG_ERR("Unknown CC_ERROR %d\r\n", cc_error);
        break;
    }

    CC_PAL_LOG_DEBUG("Converted CC_ERROR %d (0x%08x) to PSA_ERROR %d\r\n",
                     cc_error, cc_error, err);
    return err;
#endif /* CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION */
}

psa_status_t cc3xx_ecc_cc_priv_to_cc_publ(CCEcpkiUserPrivKey_t *pUserPrivKey,
                                          CCEcpkiUserPublKey_t *pUserPublKey)
{
    psa_status_t err = PSA_ERROR_GENERIC_ERROR;
    CCError_t cc_err;
    CCEcpkiPublKey_t *pPublKey;
    CCEcpkiPrivKey_t *pPrivKey;
    uint32_t orderSizeInWords;
    /* In the ecp_wrst_mul the temp_buffer seems zero initialized */
    CCEcpkiKgTempData_t temp_buffer;
    CC_PalMemSetZero(&temp_buffer, sizeof(temp_buffer));

    pPublKey = (CCEcpkiPublKey_t *)&pUserPublKey->PublKeyDbBuff;
    pPrivKey = (CCEcpkiPrivKey_t *)&pUserPrivKey->PrivKeyDbBuff;

    orderSizeInWords =
        (pPrivKey->domain.ordSizeInBits + CC_BITS_IN_32BIT_WORD - 1) /
        CC_BITS_IN_32BIT_WORD;

    cc_err = PkaEcWrstScalarMult(
        &pPrivKey->domain,     /*!< [in] Pointer to EC domain. */
        pPrivKey->PrivKey,     /*!< [in] Pointer to the scalsr buffer. */
        orderSizeInWords,      /*!< [in] Size of the scalsr in 32-bit words. */
        pPrivKey->domain.ecGx, /*!< [in] Pointer to input point X coordinate. */
        pPrivKey->domain.ecGy, /*!< [in] Pointer to input point Y coordinate. */
        pPublKey->x, /*!< [out] Pointer to output point X coordinate. */
        pPublKey->y, /*!< [out] Pointer to output point Y coordinate. */
        (uint32_t
             *)&temp_buffer); /*!< [in] The pointer to the temp buffer of
                               size not less, than
                               CC_PKA_ECPKI_SCALAR_MUL_BUFF_MAX_LENGTH_IN_WORDS.
                               */
    err = cc3xx_ecc_cc_error_to_psa_error(cc_err);

    if (err != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("Error building private key with error code %d\n",
                       cc_err);
        return err;
    }

    pPublKey->domain = pPrivKey->domain;
    pUserPublKey->valid_tag = CC_ECPKI_PUBL_KEY_VALIDATION_TAG;

    return err;
}

psa_status_t cc3xx_ecc_cc_publ_to_psa_publ(CCEcpkiUserPublKey_t *pUserPublKey,
                                           uint8_t *publ_key,
                                           size_t  *publ_key_length)
{
    CCError_t cc_err;

    if (publ_key == NULL || publ_key_length == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }


    cc_err = CC_EcpkiPubKeyExport(pUserPublKey, CC_EC_PointUncompressed,
                                  publ_key, publ_key_length);

    return cc3xx_ecc_cc_error_to_psa_error(cc_err);
}

psa_status_t cc3xx_ecc_psa_domain_to_cc_domain(psa_ecc_family_t curve,
                                               psa_key_bits_t key_bits,
                                               CCEcpkiDomainID_t *pDomainId)
{
    switch (curve) {
    case PSA_ECC_FAMILY_SECP_K1:
        switch (key_bits) {
        case 192:
            *pDomainId = CC_ECPKI_DomainID_secp192k1;
            break;
        case 224:
            *pDomainId = CC_ECPKI_DomainID_secp224k1;
            break;
        case 256:
            *pDomainId = CC_ECPKI_DomainID_secp256k1;
            break;
        default:
            return PSA_ERROR_NOT_SUPPORTED;
        }
        break;
    case PSA_ECC_FAMILY_SECP_R1:
        switch (key_bits) {
        case 192:
            *pDomainId = CC_ECPKI_DomainID_secp192r1;
            break;
        case 224:
            *pDomainId = CC_ECPKI_DomainID_secp224r1;
            break;
        case 256:
            *pDomainId = CC_ECPKI_DomainID_secp256r1;
            break;
        case 384:
            *pDomainId = CC_ECPKI_DomainID_secp384r1;
            break;
        case 521:
            *pDomainId = CC_ECPKI_DomainID_secp521r1;
            break;
        default:
            return PSA_ERROR_NOT_SUPPORTED;
        }
        break;
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_ecc_psa_priv_to_cc_priv(CCEcpkiDomainID_t DomainId,
                                           const uint8_t *priv_key,
                                           size_t priv_key_size,
                                           CCEcpkiUserPrivKey_t *pUserPrivKey)
{
    CCError_t status;
    const CCEcpkiDomain_t *pDomain;

    if (NULL == priv_key || NULL == pUserPrivKey) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    pDomain = CC_EcpkiGetEcDomain(DomainId);
    if (NULL == pDomain) {
        CC_PAL_LOG_ERR("Domain ID %d is not supported\n", DomainId);
        return PSA_ERROR_NOT_SUPPORTED;
    }

    status =
        CC_EcpkiPrivKeyBuild(pDomain, priv_key, priv_key_size, pUserPrivKey);
    if (status != CC_OK) {
        CC_PAL_LOG_ERR("Error building private key\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_ecc_psa_publ_to_cc_publ(CCEcpkiDomainID_t DomainId,
                                           const uint8_t *publ_key,
                                           size_t publ_key_size,
                                           CCEcpkiUserPublKey_t *pUserPublKey)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t status;
    const CCEcpkiDomain_t *pDomain;
    CCEcpkiBuildTempData_t BuildTempData;

    if (NULL == publ_key || NULL == pUserPublKey) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        ret = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    pDomain = CC_EcpkiGetEcDomain(DomainId);
    if (NULL == pDomain) {
        CC_PAL_LOG_ERR("Domain ID %d is not supported\n", DomainId);
        ret = PSA_ERROR_NOT_SUPPORTED;
        goto exit;
    }

    status = CC_EcpkiPublKeyBuildAndCheck(pDomain, (uint8_t *)publ_key,
                                          publ_key_size, ECpublKeyFullCheck,
                                          pUserPublKey, &BuildTempData);
    if (status != CC_OK) {
        CC_PAL_LOG_ERR("Error building public key\n");
        ret = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    ret = PSA_SUCCESS;

exit:
    CC_PalMemSetZero(&BuildTempData, sizeof(CCEcpkiBuildTempData_t));
    return ret;
}
/** @} */ // end of internal_ecc_util
