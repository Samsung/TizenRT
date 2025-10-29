/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_ccm.h
 *
 * This file contains the declarations of the internal functions to
 * perform AEAD using the AES-CCM algorithm
 *
 */

#ifndef CC3XX_INTERNAL_CCM_H
#define CC3XX_INTERNAL_CCM_H

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! AES CCM mode: CCM. */
#define CC3XX_CCM_MODE_CCM 0
/*! AES CCM mode: CCM star. */
#define CC3XX_CCM_MODE_STAR 1

/**
 * \brief Encrypt and create auth tag with AES-CCM
 */
psa_status_t cc3xx_encrypt_ccm(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *plaintext,
    size_t plaintext_length, uint8_t *ciphertext, size_t ciphertext_size,
    size_t *ciphertext_length);

/**
 * \brief Decrypt and validate auth tag with AES-CCM
 */
psa_status_t cc3xx_decrypt_ccm(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *ciphertext,
    size_t ciphertext_length, uint8_t *plaintext, size_t plaintext_size,
    size_t *plaintext_length);

/**
 * \brief    Initialize the specified AES CCM context
 */
void cc3xx_ccm_init(AesCcmContext_t *ctx);

/**
 * \brief   Release and clear the specified AES CCM context
 */
void cc3xx_ccm_free(AesCcmContext_t *ctx);

/**
 * \brief   Set the encryption key
 */
psa_status_t cc3xx_ccm_setkey_enc(
        AesCcmContext_t *ctx,
        const uint8_t *key,
        size_t key_bits);

/**
 * \brief   Set the decryption key
 */
psa_status_t cc3xx_ccm_setkey_dec(
        AesCcmContext_t *ctx,
        const uint8_t *key,
        size_t key_bits);

/**
 * \brief   Set the message and additional data lengths
 */
psa_status_t cc3xx_ccm_set_lengths(
        AesCcmContext_t *ctx,
        size_t aadSize,
        size_t dataSize);

/**
 * \brief   Set the nonce
 */
psa_status_t cc3xx_ccm_set_nonce(
        AesCcmContext_t *ctx,
        const uint8_t *pNonce,
        size_t sizeOfN,
        size_t sizeOfT,
        uint32_t ccmMode);

/**
 * \brief   Pass in the additional data
 */
psa_status_t cc3xx_ccm_update_ad(
        AesCcmContext_t *ctx,
        const uint8_t *add,
        size_t aadSize);

/**
 * \brief   Encrypt or decrypt a message fragment.
 */
psa_status_t cc3xx_ccm_update(
        AesCcmContext_t *ctx,
        size_t dataSize,
        const uint8_t *input,
        uint8_t *output);

/**
 * \brief   Finish the encryption/decryption and compute the authentication tag.
 */
psa_status_t cc3xx_ccm_finish(
        AesCcmContext_t *ctx,
        uint8_t *macBuf,
        size_t sizeOfT,
        size_t *tag_len);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_INTERNAL_CCM_H */
