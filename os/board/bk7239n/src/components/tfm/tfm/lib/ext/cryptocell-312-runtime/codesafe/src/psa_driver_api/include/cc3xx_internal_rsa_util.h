/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_rsa_util.h
 *
 * This file contains the declarations of the internal utility functions used
 * to manipulate RSA types and key formats
 *
 */

#ifndef CC3XX_INTERNAL_RSA_UTIL_H
#define CC3XX_INTERNAL_RSA_UTIL_H

#include "psa/crypto.h"

#include "cc_common.h"
#include "cc_ecpki_error.h"
#include "cc_pal_abort.h"
#include "cc_pal_mem.h"
#include "cc_pal_types.h"

#include "cc_ecpki_build.h"
#include "cc_ecpki_domain.h"
#include "cc_ecpki_ecdsa.h"
#include "cc_ecpki_kg.h"
#include "cc_ecpki_local.h"
#include "pka_ec_wrst.h"

#include "cc_rsa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal and maximal size of RSA modulus in bits
 * According to FIPS 186-4 size in bits should be in range [1024...3072]
 */
#if defined(ARCH_IS_CC310)
#define CC3XX_RSA_MIN_VALID_KEY_SIZE_VALUE_IN_BITS 1024
#define CC3XX_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS 2048
#define CC3XX_RSA_MIN_VALID_KEYGEN_SIZE_VALUE_IN_BITS 1024
#define CC3XX_RSA_MAX_VALID_KEYGEN_SIZE_VALUE_IN_BITS 2048
#else
#define CC3XX_RSA_MIN_VALID_KEY_SIZE_VALUE_IN_BITS 1024
#define CC3XX_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS 4096
#define CC3XX_RSA_MIN_VALID_KEYGEN_SIZE_VALUE_IN_BITS 1024
#define CC3XX_RSA_MAX_VALID_KEYGEN_SIZE_VALUE_IN_BITS 3072
#endif

/**
 * \brief Converts a \ref CCError_t to the corresponding \ref psa_status_t
 *
 * \param[in] cc_error An error of type \ref CCError_t
 *
 * \retval The corresponding value of type \ref psa_status_t
 */
psa_status_t cc3xx_rsa_cc_error_to_psa_error(CCError_t cc_error);

/**
 * \brief Translate an RSA private key in DER format
 */
psa_status_t cc3xx_rsa_save_der_priv_key(uint8_t *key_buffer,
                                         size_t key_buffer_size, uint32_t *n,
                                         uint32_t *e, uint32_t *d, uint32_t *p,
                                         uint32_t *q, uint32_t *dP, uint32_t *dQ,
                                         uint32_t *qInv, size_t d_size_bytes,
                                         size_t *key_buffer_length);
/**
 * \brief Extract the public key from the private key in PSA format
 */
psa_status_t cc3xx_rsa_psa_priv_to_psa_publ(uint8_t *priv_key_buffer,
                                            size_t priv_key_buffer_size,
                                            uint8_t *publ_key_buffer,
                                            size_t publ_key_buffer_size,
                                            size_t *publ_key_buffer_length);
/**
 * \brief Convert a private key in PSA format to the same key in CC format, i.e.
 *        the type specified by the low-level driver code, i.e.
 *        \ref CCRsaUserPrivKey_t
 */
psa_status_t cc3xx_rsa_psa_priv_to_cc_priv(const uint8_t *psa_priv_key_buffer,
                                           size_t psa_priv_key_buffer_size,
                                           CCRsaUserPrivKey_t *UserPrivKey_ptr);
/**
 * \brief Extract the public key from a private key in PSA format, and in the
 *        process translates it to CC format, i.e. the type specified by the
 *        low-level driver, i.e. \ref CCRsaUserPubKey_t
 */
psa_status_t cc3xx_rsa_psa_priv_to_cc_pub(const uint8_t *psa_priv_key_buffer,
                                          size_t psa_priv_key_buffer_size,
                                          CCRsaUserPubKey_t *UserPubKey_ptr);
/**
 * \brief Convert a public key in PSA format to the same key in CC format, i.e.
 *        the type specified by the low-level driver code, i.e.
 *        \ref CCRsaUserPubKey_t
 */
psa_status_t cc3xx_rsa_psa_pub_to_cc_pub(const uint8_t *psa_pub_key_buffer,
                                         size_t psa_pub_key_buffer_size,
                                         CCRsaUserPubKey_t *UserPubKey_ptr);
#ifdef __cplusplus
}
#endif
#endif /* CC3XX_INTERNAL_RSA_UTIL_H */
