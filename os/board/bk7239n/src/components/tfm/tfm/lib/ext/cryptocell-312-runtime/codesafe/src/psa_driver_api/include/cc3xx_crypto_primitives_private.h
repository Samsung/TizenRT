/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_crypto_primitives_private.h
 *
 * This file contains the definition of public types associated
 * to the CC3XX driver. It's meant to be included only by those
 * modules which require access to the private implementation
 * of the CC3XX types
 *
 */

#ifndef CC3XX_CRYPTO_PRIMITIVES_PRIVATE_H
#define CC3XX_CRYPTO_PRIMITIVES_PRIVATE_H

/* Include the public header first as it contains the typedefs */
#include "cc3xx_crypto_primitives.h"

#include "psa/crypto.h"

#include "hash_driver.h"
#include "aes_driver.h"
#include "aesgcm_driver.h"
#include "aesccm_driver.h"
#include "chacha_driver.h"

/* Include the internal layer defines for Chacha20-Poly1305 because it is there
 * that the Chacha20-Poly1305 context is defined. This is due to the fact that
 * the low-level driver contexts don't support Chacha20-Poly1305 as a combined
 * operation with a requirement for state support (i.e. to support multipart)
 */
#include "cc3xx_internal_chacha20_poly1305.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct cc3xx_hash_operation_s
 *
 * \brief A structure holding state information for an Hash operation
 */
struct cc3xx_hash_operation_s {
    HashContext_t ctx; /*!< Low-level hash context */
};

/*!
 * \struct cc3xx_cipher_operation_s
 *
 * \brief A structure holding state information for an Cipher operation
 */
struct cc3xx_cipher_operation_s {
    psa_algorithm_t alg;          /*!< Cipher algorithm */
    psa_key_type_t key_type;      /*!< Key type */
    psa_encrypt_or_decrypt_t dir; /*!< Encrypt/decrypt direction */
    size_t block_size;            /*!< Block size of the cipher */

    /*! Function that adds padding for padding-enabled cipher modes */
    psa_status_t(*add_padding)(uint8_t *, size_t, size_t);
    /*! Function that processes the padding when padding modes are used */
    psa_status_t(*get_padding)(const uint8_t *, size_t, size_t *);

    uint8_t unprocessed_data[AES_BLOCK_SIZE]; /*!< Cached data */
    size_t  unprocessed_size;                 /*!< Size of the cached data */
    uint8_t iv[AES_IV_SIZE];                  /*!< Initialisation Vector */
    size_t  iv_size;                          /*!< Size of the IV */

    union {
        AesContext_t    aes;     /*!< Low-level AES context */
        ChachaContext_t chacha;  /*!< Low-level Chacha context */
    } ctx;
};

/*!
 * \struct cc3xx_mac_operation_s
 *
 * \brief A structure holding state information for a MAC operation
 */
struct cc3xx_mac_operation_s {
    psa_algorithm_t alg; /*!< MAC algorithm used in this context */
    union {
        cc3xx_cipher_operation_t cmac; /*!< Underlying cipher op for CMAC */
        cc3xx_hash_operation_t hmac;   /*!< Underlying hash op for HMAC */
    };
    /* Only for HMAC */
    uint8_t opad[PSA_HMAC_MAX_HASH_BLOCK_SIZE]; /*!< Opad as RFC-2104 */
};

/*!
 * \struct cc3xx_aead_operation_s
 *
 * \brief A structure holding state information for an AEAD operation
 */
struct cc3xx_aead_operation_s {
    psa_algorithm_t alg;          /*!< AEAD algorithm */
    psa_key_type_t key_type;      /*!< Key type */
    psa_encrypt_or_decrypt_t dir; /*!< Encrypt/decrypt direction */

    size_t tag_length;            /*!< Size of the authentication tag */

    union {
        AesGcmContext_t gcm;            /*!< Low-level GCM context */
        AesCcmContext_t ccm;            /*!< Low-level CCM context */
        ChachaPolyContext_t chachapoly; /*!< Low-level Chacha20-Poly1305 ctx */
    } ctx;
};

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_CRYPTO_PRIMITIVES_PRIVATE_H */
