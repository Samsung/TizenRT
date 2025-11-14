/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "attest_public_key.h"
#include "psa/crypto.h"
#include <stdint.h>
#include "attest.h"

/*!
 * \def ECC_CURVE_SECP256R1_PUBLIC_KEY_LENGTH
 *
 * \brief Calculates the size of ECC public key in bytes based on the bit size
 *        of the curve
 */
#define ECC_CURVE_SECP256R1_PUBLIC_KEY_LENGTH (1 + 2 * PSA_BITS_TO_BYTES(256))

/*!
 * \var public_key_registered
 *
 * \brief Indicates whether the public part of the attestation key pair was
 *        registered to Crypto service or not.
 */
static uint32_t public_key_registered = 0;

static psa_status_t attest_import_public_key(psa_key_handle_t *public_key,
                                             const uint8_t *key_buf,
                                             size_t ken_len,
                                             psa_ecc_family_t curve_type)
{
    psa_key_attributes_t key_attributes = psa_key_attributes_init();

    /* Setup the key usage flags, algorithm and key type for public key */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(curve_type));

    /* Register public key to Crypto service */
    return psa_import_key(&key_attributes, key_buf, ken_len, public_key);
}

extern const psa_ecc_family_t initial_attest_curve_type;
extern const uint8_t initial_attest_pub_key[];
extern const uint32_t initial_attest_pub_key_size;

enum psa_attest_err_t
attest_register_initial_attestation_public_key(psa_key_handle_t *public_key)
{
    psa_status_t res;

    /* Public key should be unregistered at this point */
    if (public_key_registered != 0) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    if (initial_attest_curve_type != PSA_ECC_FAMILY_SECP_R1) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    if (initial_attest_pub_key_size != ECC_CURVE_SECP256R1_PUBLIC_KEY_LENGTH) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    res = attest_import_public_key(public_key, initial_attest_pub_key,
                                   initial_attest_pub_key_size,
                                   initial_attest_curve_type);
    if (res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    public_key_registered = 1;

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_unregister_initial_attestation_public_key(psa_key_handle_t public_key)
{
    psa_status_t crypto_res;

    if (public_key_registered != 1) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    crypto_res = psa_destroy_key(public_key);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }
    public_key_registered = 0;

    return PSA_ATTEST_ERR_SUCCESS;
}
