/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_gcm.h
 *
 * This file contains the declarations of the internal functions required to
 * perform AEAD using AES-GCM
 *
 */

#ifndef CC3XX_INTERNAL_GCM_H
#define CC3XX_INTERNAL_GCM_H

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Encrypt and create auth tag with AES-GCM
 */
psa_status_t cc3xx_gcm_encrypt(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *plaintext,
    size_t plaintext_length, uint8_t *ciphertext, size_t ciphertext_size,
    size_t *ciphertext_length);

/**
 * \brief Decrypt and validate auth tag with AES-GCM
 */
psa_status_t cc3xx_gcm_decrypt(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *ciphertext,
    size_t ciphertext_length, uint8_t *plaintext, size_t plaintext_size,
    size_t *plaintext_length);
/**
 * \brief    Initialize the specified AES GCM context
 */
void cc3xx_gcm_init(AesGcmContext_t *ctx);

/**
 * \brief   Release and clear the specified AES GCM context
 */
void cc3xx_gcm_free(AesGcmContext_t *ctx);

/**
 * \brief   Set the encryption key
 */
psa_status_t cc3xx_gcm_setkey_enc(
    AesGcmContext_t *ctx,
    const uint8_t *key,
    size_t key_bits);
/**
 * \brief   Set the decryption key
 */
psa_status_t cc3xx_gcm_setkey_dec(
    AesGcmContext_t *ctx,
    const uint8_t *key,
    size_t key_bits);
/**
 * \brief   Set the nonce and update the tag size of the context
 */
psa_status_t cc3xx_gcm_set_nonce(
    AesGcmContext_t *ctx,
    const uint8_t *nonce,
    size_t nonce_size,
    size_t tag_size);
/**
 * \brief   Set the message and additional data lengths
 */
psa_status_t cc3xx_gcm_set_lengths(
    AesGcmContext_t *ctx,
    size_t aadSize,
    size_t dataSize);
/**
 * \brief   Pass in the additional data
 */
psa_status_t cc3xx_gcm_update_ad(
    AesGcmContext_t *ctx,
    const uint8_t *aad,
    size_t aad_size);
/**
 * \brief   Encrypt or decrypt a message fragment.
 */
psa_status_t cc3xx_gcm_update(
    AesGcmContext_t *ctx,
    size_t size,
    const uint8_t *input,
    uint8_t *output);
/**
 * \brief   Finish the encryption/decryption and compute the authentication tag.
 */
psa_status_t cc3xx_gcm_finish(
    AesGcmContext_t *ctx,
    uint8_t *tag,
    size_t tag_size,
    size_t *tag_len);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_INTERNAL_GCM_H */
