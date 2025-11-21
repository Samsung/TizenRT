/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_ecc_util.h
 *
 * This file contains the declarations of the internal functions and utilities
 * required to translate between PSA and CC low-level Elliptic Curve types and
 * format conversions
 *
 */

#ifndef CC3XX_INTERNAL_ECC_UTIL_H
#define CC3XX_INTERNAL_ECC_UTIL_H

#include "psa/crypto.h"

#include "cc_ecpki_domain.h"
#include "cc_ecpki_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Convert CC error code to PSA error
 *
 * @param[in]   cc_error  The CryptoCell error
 *
 * @return A PSA error code
 */
psa_status_t cc3xx_ecc_cc_error_to_psa_error(CCError_t cc_error);

/**
 * @brief   Convert ECPKI private key to ECKPKI public key
 *
 * @param[in]   pUserPrivKey  ECPKI public key
 * @param[out]  pUserPublKey  ECPKI public key
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecc_cc_priv_to_cc_publ(CCEcpkiUserPrivKey_t *pUserPrivKey,
                                          CCEcpkiUserPublKey_t *pUserPublKey);

/**
 * @brief   Convert ECPKI to PSA public key buffer
 *
 * @param[in]     publ_key        Public key buffer
 * @param[in/out] publ_key_length Public key buffer size
 * @param[out]    pUserPublKey    ECPKI public key
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecc_cc_publ_to_psa_publ(CCEcpkiUserPublKey_t *pUserPublKey,
                                           uint8_t *publ_key,
                                           size_t  *publ_key_length);

/**
 * @brief   Extract domain ID from PSA
 *
 * @param[in]  curve         Curve extracted from EC key type
 * @param[in]  key_bits      Size of key in bits
 * @param[out] pDomainId     EC domain identifier
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 */
psa_status_t cc3xx_ecc_psa_domain_to_cc_domain(psa_ecc_family_t curve,
                                               psa_key_bits_t key_bits,
                                               CCEcpkiDomainID_t *pDomainId);

/**
 * @brief   Import private key using a domain ID and key buffer
 *
 * @param[in]  DomainId      EC domain identifier
 * @param[in]  priv_key      Private key buffer
 * @param[in]  priv_key_size Private key buffer size
 * @param[out] pUserPrivKey  ECPKI private key
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecc_psa_priv_to_cc_priv(CCEcpkiDomainID_t DomainId,
                                           const uint8_t *priv_key,
                                           size_t priv_key_size,
                                           CCEcpkiUserPrivKey_t *pUserPrivKey);

/**
 * @brief   Import public key using a domain ID and key buffer
 *
 * @param[in]  DomainId      EC domain identifier
 * @param[in]  publ_key      Public key buffer
 * @param[in]  publ_key_size Public key buffer size
 * @param[out] pUserPublKey  ECPKI public key
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecc_psa_publ_to_cc_publ(CCEcpkiDomainID_t DomainId,
                                           const uint8_t *publ_key,
                                           size_t publ_key_size,
                                           CCEcpkiUserPublKey_t *pUserPublKey);
#ifdef __cplusplus
}
#endif
#endif /* CC3XX_INTERNAL_ECC_UTIL_H */

