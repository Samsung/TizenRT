/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_chacha20_poly1305.c
 *
 * This file contains the implementation of the internal functions to
 * perform AEAD using the Chacha20-Poly1305 algorithm as per RFC7539
 *
 */

#include "cc_common.h"
#include "cc_pal_abort.h"
#include "cc_pal_mem.h"
#include "cc_pal_types.h"
#include "chacha20_alt.h"
#include "chacha_driver.h"
#include "poly.h"
#include <psa/crypto.h>

#include "cc3xx_internal_chacha20.h"
#include "cc3xx_internal_chacha20_poly1305.h"

static psa_status_t chacha20_poly1305_crypt_and_tag(
    cryptoDirection_t direction, const uint8_t *key_buffer,
    size_t key_buffer_size, size_t length, const uint8_t *nonce,
    size_t nonce_size, const uint8_t *aad, size_t aad_len, const uint8_t *input,
    uint8_t *output, size_t *output_length, uint8_t *tag, size_t tag_size)
{
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;
    CCError_t rc = CC_FAIL;
    mbedtls_poly_key polyKey = {0};
    mbedtls_poly_mac polyMac = {0};
    /* RFC7539 specifies that initial value of the counter must be 1 during
     * Chacha20 encryption/decryption
     */
    uint32_t counter = 1;

    if (tag_size < 16) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* RFC7539 supports only 12 bytes nonce: It is the concatenation of a
     * constant and a 64 bit IV - but we don't care here how it's been
     * derived
     */
    if (nonce_size != CHACHA_IV_96_SIZE_BYTES) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ChachaContext_t context = {0};

    cc3xx_chacha20_init(&context);

    status = cc3xx_chacha20_setkey(&context, key_buffer, key_buffer_size);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    status = cc3xx_chacha20_set_nonce(&context, nonce, nonce_size);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    status = cc3xx_chacha20_set_counter(&context, counter);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Assumes that upper layers have checked that output is at least the
     * size of the input
     */
    size_t output_size = length;
    size_t update_length = 0;
    status = cc3xx_chacha20_update(&context, input, length,
                                   output, output_size, &update_length);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Generate polyKey */
    status = cc3xx_chacha20_poly1305_gen_otk(&context,
                                            (uint8_t *)&polyKey,
                                            sizeof(polyKey));
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Authenticate using the generated key */
    rc = PolyMacCalc(polyKey, aad, aad_len,
                 (direction == CRYPTO_DIRECTION_ENCRYPT) ? output : input,
                 length, polyMac, true);
    if (rc != CC_OK) {
        status = PSA_ERROR_HARDWARE_FAILURE;
        goto cleanup;
    }

    CC_PalMemCopy(tag, polyMac, sizeof(polyMac));
cleanup:
    cc3xx_chacha20_free(&context);

    if (rc == CC_OK) {
        status = PSA_SUCCESS;
        if (direction == CRYPTO_DIRECTION_ENCRYPT) {
            /*
             * Since tag must be 16, we hardcode it.
             * The tag is appended to the ciphertext and the length is expected
             * to be the combined effort.
             */
            *output_length = length + 16;
        } else {
            /* The plaintext is only the length of the decrypted ciphertext, no
             * tag included. */
            *output_length = length;
        }
    } else if (output != NULL) {
        /*
         * If the output is not NULL and the rc was not 0, something bad
         * happened. We clear the output, and the status should have been set to
         * non-success.
         */
        CC_PalMemSetZero(output, length);
    }

    return status;
}

/** \defgroup internal_chacha20_poly1305 Internal Chacha20-Poly1305 functions
 *
 *  Internal functions used by the driver to perform AEAD using Chacha20-Poly1305 mode
 *
 *  @{
 */
psa_status_t cc3xx_chacha20_poly1305_gen_otk(
    ChachaContext_t *context,
    uint8_t *otk,
    size_t otk_size)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    uint8_t chachaInState[CHACHA_BLOCK_SIZE_BYTES] = {0};
    uint8_t chachaOutState[CHACHA_BLOCK_SIZE_BYTES] = {0};
    size_t out_size = 0;

    if (context == NULL || otk == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (otk_size < 32) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* Generate polyKey */
    status = cc3xx_chacha20_set_counter(context, 0);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Calling chacha20_update using an all-zero input is equivalent in getting
     * the output of the chacha_block() function only, i.e. the keystream. The
     * size will be a 64 byte block but we need to take only the first 32 as
     * output and they will represent the OTK (256 bit key (r,s))
     */
    status = cc3xx_chacha20_update(context,
                                   chachaInState, sizeof(chachaInState),
                                   chachaOutState, sizeof(chachaOutState),
                                   &out_size);
    if (status != PSA_SUCCESS) {
        return status;
    }

    if (out_size < 32) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Copy generated key as result of previous step */
    CC_PalMemCopy(otk, chachaOutState, 32);

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_poly1305_set_otk(
    PolyState_t *state,
    const uint8_t *otk,
    size_t otk_length)
{
    CCError_t rc = CC_FAIL;

    if (state == NULL || otk == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    rc = PolyInit(state, otk, otk_length);
    if (rc != CC_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_poly1305_set_lengths(
    ChachaPolyContext_t *context,
    size_t ad_length,
    size_t plaintext_length)
{
    if (context == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    context->ad_len = ad_length;
    context->plaintext_len = plaintext_length;
    context->curr_ad_len = 0;
    context->curr_plaintext_len = 0;

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_poly1305_update_ad(
    ChachaPolyContext_t *context,
    const uint8_t *input,
    size_t input_length)
{
    bool isPolyAeadMode = false;
    CCError_t rc = CC_FAIL;

    if (context == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (context->curr_ad_len + input_length > context->ad_len) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (context->curr_ad_len + input_length == context->ad_len) {
        /* Pad the last block of this update */
        isPolyAeadMode = true;
    }

    rc = PolyUpdate(&context->poly, input, input_length, isPolyAeadMode);
    if (rc != CC_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    /* Update the current AD length before returning */
    context->curr_ad_len += input_length;

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_poly1305_update(
    ChachaPolyContext_t *context,
    const uint8_t *input,
    size_t input_length,
    uint8_t *output,
    size_t output_size,
    size_t *output_length)
{
    bool isPolyAeadMode = false;
    CCError_t rc = CC_FAIL;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    if (context == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* We still need to get calls to update AD to expected values */
    if (context->curr_ad_len != context->ad_len) {
        return PSA_ERROR_BAD_STATE;
    }

    if (context->curr_plaintext_len + input_length > context->plaintext_len) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Process (encrypt or decrypt) the input using Chacha20 */
    status = cc3xx_chacha20_update(&context->chacha, input, input_length,
                                   output, output_size, output_length);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Processing is terminated, but we still need to update the Poly1305
     * ongoing operation with the data being processed. This will be either
     * the plaintext or the ciphertext based on the direction of encryption
     * decryption, i.e. in an encryption operation the output is added to
     * the AEAD construction, while in decryption is the input that is added
     */
    const uint8_t *data_for_aead_construction = output;
    if (context->bAuthenticateInput) {
        data_for_aead_construction = input;
    }

    if (context->curr_plaintext_len + input_length == context->plaintext_len) {
        /* Pad the last block of this update */
        isPolyAeadMode = true;
    }

    /* input length and output length are the same by construction */
    rc = PolyUpdate(&context->poly, data_for_aead_construction, input_length,
                    isPolyAeadMode);
    if (rc != CC_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    /* This is the last block so we need to add LE bytes for ad_len and
     * plaintext_len to be each encoded as 8 bytes numbers
     */
    if (isPolyAeadMode) {
        uint32_t lastBlock[CC_POLY_BLOCK_SIZE_IN_WORDS];
        /* Safely assume processor endianess is little endian */
        lastBlock[0] = context->ad_len;
        lastBlock[1] = 0;
        lastBlock[2] = context->plaintext_len;
        lastBlock[3] = 0;
        rc = PolyUpdate(&context->poly, (const uint8_t *)lastBlock,
                        CC_POLY_BLOCK_SIZE_IN_BYTES, isPolyAeadMode);
        if (rc != CC_OK) {
            return PSA_ERROR_HARDWARE_FAILURE;
        }
    }

    /* Update the current plaintext length before returning */
    context->curr_plaintext_len += input_length;

    return status;
}

psa_status_t cc3xx_chacha20_poly1305_finish(
        ChachaPolyContext_t *context,
        uint8_t *tag,
        size_t tag_size,
        size_t *tag_length)
{
    /* As the update don't store data, the finish will only return the tag */
    CCError_t rc = CC_FAIL;

    if (context == NULL || tag == NULL ||
        tag_length == NULL || tag_size < CC_POLY_BLOCK_SIZE_IN_BYTES) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    *tag_length = 0;

    rc = PolyFinish(&context->poly, tag, tag_size);
    if (rc != CC_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    *tag_length = CC_POLY_BLOCK_SIZE_IN_BYTES;

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_poly1305_verify(
        ChachaPolyContext_t *context,
        const uint8_t *tag,
        size_t tag_size)
{
    uint8_t local_tag_buffer[CC_POLY_BLOCK_SIZE_IN_BYTES];
    size_t tag_length = 0;
    int diff, i;

    if (tag == NULL || tag_size != CC_POLY_BLOCK_SIZE_IN_BYTES) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_status_t status = cc3xx_chacha20_poly1305_finish(context,
                                                local_tag_buffer,
                                                sizeof(local_tag_buffer),
                                                &tag_length);
    if (status != PSA_SUCCESS) {
        return status;
    }

    if (tag_length != CC_POLY_BLOCK_SIZE_IN_BYTES) {
        return PSA_ERROR_INVALID_SIGNATURE;
    }

    /* Check tag in "constant-time" */
    for (diff = 0, i = 0; i < tag_length; i++) {
        diff |= tag[i] ^ local_tag_buffer[i];
    }

    if (diff != 0) {
        return PSA_ERROR_INVALID_SIGNATURE;
    } else {
        return PSA_SUCCESS;
    }
}

psa_status_t cc3xx_encrypt_chacha20_poly1305(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *plaintext,
    size_t plaintext_length, uint8_t *ciphertext, size_t ciphertext_size,
    size_t *ciphertext_length)
{
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;

    /*
     * The tag should be directly behind the ciphertext,
     * and the ciphertext is as long as the plaintext input.
     */
    uint8_t *tag = ciphertext + plaintext_length;

    status = chacha20_poly1305_crypt_and_tag(
        CRYPTO_DIRECTION_ENCRYPT, key_buffer, key_buffer_size, plaintext_length,
        nonce, nonce_length, additional_data, additional_data_length, plaintext, ciphertext,
        ciphertext_length, tag, ciphertext_size - plaintext_length);
    return status;
}

psa_status_t cc3xx_decrypt_chacha20_poly1305(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *ciphertext,
    size_t ciphertext_length, uint8_t *plaintext, size_t plaintext_size,
    size_t *plaintext_length)
{
    int diff, i;
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;

    uint8_t local_tag_buffer[PSA_AEAD_TAG_MAX_SIZE];

    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_bits_t key_bits = psa_get_key_bits(attributes);
    psa_algorithm_t key_alg = psa_get_key_algorithm(attributes);

    size_t tag_length = PSA_AEAD_TAG_LENGTH(key_type, key_bits, key_alg);

    size_t ciphertext_length_without_tag = ciphertext_length - tag_length;
    const uint8_t *tag = ciphertext + ciphertext_length_without_tag;

    CC_PalMemCopy(local_tag_buffer, tag, tag_length);

    status = chacha20_poly1305_crypt_and_tag(
        CRYPTO_DIRECTION_DECRYPT, key_buffer, key_buffer_size,
        ciphertext_length_without_tag, nonce, nonce_length, additional_data,
        additional_data_length, ciphertext, plaintext, plaintext_length,
        local_tag_buffer, PSA_AEAD_TAG_MAX_SIZE);

    /* Check tag in "constant-time" */
    for (diff = 0, i = 0; i < sizeof(tag_length); i++)
        diff |= tag[i] ^ local_tag_buffer[i];

    if (diff != 0) {
        CC_PalMemSetZero(plaintext, plaintext_size);
        return (PSA_ERROR_INVALID_SIGNATURE);
    }

    return status;
}
/** @} */ // end of internal_chacha20_poly1305
