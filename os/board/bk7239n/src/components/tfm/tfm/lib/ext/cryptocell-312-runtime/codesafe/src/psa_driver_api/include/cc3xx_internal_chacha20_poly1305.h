/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_chacha20_poly1305.h
 *
 * This file contains the declarations of the internal functions to
 * perform AEAD using the Chacha20-Poly1305 algorithm
 *
 */

#ifndef CC3XX_INTERNAL_CHACHA20_POLY1305_H
#define CC3XX_INTERNAL_CHACHA20_POLY1305_H

#include "psa/crypto.h"
#include "chacha_driver.h"
#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief A context type to implement multipart APIs on the Chacha20-Poly1305
 *        algorithm. The low level driver does not implement it so we let the
 *        driver interface implement it by separately including Chacha20 context
 *        and Poly1305 state defined in the low level driver and combining them.
 *        As this is an internal detail of the driver implementation, follow
 *        the same guidelines of the other contexts in terms of naming.
 */
typedef struct ChachaPolyContext_t {
    ChachaContext_t chacha;        /*!< Context of the underlying Chacha20 */
    PolyState_t poly;              /*!< Context of the underlying Poly1305 */
    size_t ad_len;             /*!< Length of the data to be authenticated */
    size_t plaintext_len;          /*!< Length of the data to be encrypted */
    size_t curr_ad_len;         /*!< Size of the data authenticated so far */
    size_t curr_plaintext_len;      /*!< Size of the data encrypted so far */
    bool bAuthenticateInput; /*!< True when input is used for AEAD authent */
} ChachaPolyContext_t;

/**
 * \brief Generates an OTK as per RFC7539. This function must be called only
 *        after the key has been set on the Chacha20 context
 *
 * \note This function implements the block which generates the one time key
 *       to be used for Poly1305 as part of RFC7539 to be generated through
 *       Chacha20, i.e.
 *           poly1305_key_gen(key, nonce):
 *               counter = 0;
 *               block = chacha_block(key, counter, nonce)
 *               return block[0..31]
 *
 * \param[in,out] context  Pointer to the Chacha context to be used for the
 *                         encryption. Note that this is the Chacha and not
 *                         the ChachaPoly context because encryption only is
 *                         required
 * \param[out]    otk      Buffer to hold the one time key produced.
 * \param[in]     otk_size Size in bytes of the otk buffer. Must be at least 32
 */
psa_status_t cc3xx_chacha20_poly1305_gen_otk(
    ChachaContext_t *context,
    uint8_t *otk,
    size_t otk_size);

/**
 * \brief Sets the value of the one time key (OTK) to be used by the Poly1305
 *        algorithm in a multipart flow.
 *
 * \param[in,out] state      Pointer to the Poly1305 state to be used
 * \param[in]     otk        Buffer containing the OTK to be set in the state
 * \param[in]     otk_length Size in bytes of the data in the otk buffer. Must
 *                           be 32.
 *
 * \return psa_status_t
 */
psa_status_t cc3xx_chacha20_poly1305_set_otk(
    PolyState_t *state,
    const uint8_t *otk,
    size_t otk_length);

/**
 * \brief Sets the lengths for the data to be authenticate and for the
 *        data to be encrypted. These lengths are used by the algorithm
 *        to build the associated AEAD construction as per RFC7539 that
 *        is authenticated with Poly1305.
 *
 * \param[in,out] context          Context for the multipart Chacha20Poly1305
 * \param[in]     ad_length        Expected byte length of the additional data
 * \param[in]     plaintext_length Expected byte length of the data to encrypt
 *
 * \return psa_status_t
 */
psa_status_t cc3xx_chacha20_poly1305_set_lengths(
    ChachaPolyContext_t *context,
    size_t ad_length,
    size_t plaintext_length);

/**
 * \brief Updates the additional data to be authenticated with a new chunk of
 *        information
 *
 * \param[in,out] context      Context for the multipart Chacha20Poly1305
 * \param[in]     input        Buffer containing the chunk of additional data
 * \param[in]     input_length Size in bytes of the data in the input buffer
 *
 * \return psa_status_t
 */
psa_status_t cc3xx_chacha20_poly1305_update_ad(
    ChachaPolyContext_t *context,
    const uint8_t *input,
    size_t input_length);

/**
 * \brief Updates the data to be encrypted or decrypted, based on the type of
 *        multipart operation, with a new chunk of information
 *
 * \param[in,out] context       Context for the multipart Chacha20Poly1305
 * \param[in]     input         Buffer containing the chunk of new data
 * \param[in]     input_length  Size in bytes of the data pointed in input
 * \param[out]    output        Buffer to hold the processed data
 * \param[in]     output_size   Size in bytes of the output buffer
 * \param[out]    output_length Length in bytes of the processed data
 *
 * \return psa_status_t
 */
psa_status_t cc3xx_chacha20_poly1305_update(
    ChachaPolyContext_t *context,
    const uint8_t *input,
    size_t input_length,
    uint8_t *output,
    size_t output_size,
    size_t *output_length);

/**
 * \brief Finalize the multipart AEAD producing the authentication tag
 *
 * \param[in,out] context    Context for the multipart Chacha20Poly1305
 * \param[out]    tag        Buffer containing the produced tag from Poly1305
 * \param[in]     tag_size   Size in bytes of the tag buffer. Must be >= 16
 * \param[out]    tag_length Length in bytes of the tag. On success is 16
 *
 * \return psa_status_t
 */
psa_status_t cc3xx_chacha20_poly1305_finish(
    ChachaPolyContext_t *context,
    uint8_t *tag,
    size_t tag_size,
    size_t *tag_length);

/**
 * \brief Verifies that the tag given as parameter matches the one from the
 *        finalization of the multipart AEAD operation
 *
 * \param[in,out] context  Context for the multipart Chacha20Poly1305
 * \param[in]     tag      Buffer containing the tag to verify
 * \param[in]     tag_size Size in bytes of the tag buffer
 *
 * \return psa_status_t
 */
psa_status_t cc3xx_chacha20_poly1305_verify(
    ChachaPolyContext_t *context,
    const uint8_t *tag,
    size_t tag_size);
/**
 * \brief Encrypt and create auth tag with Chacha20-Poly1305
 */
psa_status_t cc3xx_encrypt_chacha20_poly1305(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *plaintext,
    size_t plaintext_length, uint8_t *ciphertext, size_t ciphertext_size,
    size_t *ciphertext_length);

/**
 * \brief Decrypt and validate auth tag with Chacha20-Poly1305
 */
psa_status_t cc3xx_decrypt_chacha20_poly1305(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *ciphertext,
    size_t ciphertext_length, uint8_t *plaintext, size_t plaintext_size,
    size_t *plaintext_length);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_INTERNAL_CHACHA20_POLY1305_H */
