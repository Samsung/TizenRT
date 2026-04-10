/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @file tizenrt_psa_cipher_driver.c
 * @brief PSA Transparent Cipher Driver for TizenRT Samsung SE AES hardware.
 *
 * Replaces the old MBEDTLS_AES_ALT mechanism removed in mbedtls 4.1.0.
 * AES key material is provided by PSA (LOCAL_STORAGE) and loaded into
 * hardware for the duration of the operation.
 *
 * Supported algorithms:
 *   PSA_ALG_ECB_NO_PADDING  – block-aligned only
 *   PSA_ALG_CBC_NO_PADDING  – block-aligned only (used by TLS 1.2)
 *   PSA_ALG_CTR             – arbitrary length
 *   PSA_ALG_CFB             – arbitrary length (128-bit feedback)
 *
 * PSA_ALG_CBC_PKCS7 padding is NOT handled here; fall through to software.
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>

#include "mbedtls/build_info.h"
#include "psa/crypto.h"
#include "psa/crypto_sizes.h"
#include "mbedtls/platform_util.h"

#include "mbedtls/alt/common.h"
#include "alt_utils.h"
#include "tizenrt_psa_driver.h"

/* AES block size in bytes */
#define AES_BLOCK_SIZE 16

/* =========================================================================
 * Internal helpers
 * =========================================================================
 */

/** Map key bit-length to HAL AES key type. */
static hal_key_type tizenrt_aes_key_type(size_t key_bits)
{
    switch (key_bits) {
    case 128: return HAL_KEY_AES_128;
    case 192: return HAL_KEY_AES_192;
    case 256: return HAL_KEY_AES_256;
    default:  return HAL_KEY_UNKNOWN;
    }
}

/** Map PSA algorithm to HAL AES mode. Returns 0 if unsupported. */
static hal_aes_mode tizenrt_aes_hal_mode(psa_algorithm_t alg)
{
    if (alg == PSA_ALG_ECB_NO_PADDING)  return HAL_AES_ECB_NOPAD;
    if (alg == PSA_ALG_CBC_NO_PADDING)  return HAL_AES_CBC_NOPAD;
    if (alg == PSA_ALG_CTR)             return HAL_AES_CTR;
    if (alg == PSA_ALG_CFB)             return HAL_AES_CFB128;
    return (hal_aes_mode)0;  /* unsupported */
}

/** Return 1 if the algorithm needs an IV (all except ECB). */
static int tizenrt_aes_needs_iv(psa_algorithm_t alg)
{
    return (alg != PSA_ALG_ECB_NO_PADDING);
}

/** Return 1 if the algorithm is stream-like (arbitrary length input). */
static int tizenrt_aes_is_stream(psa_algorithm_t alg)
{
    return (alg == PSA_ALG_CTR || alg == PSA_ALG_CFB);
}

/*
 * Accessor helpers for the operation context.
 *
 * tizenrt_transparent_cipher_operation_t stores shnd as void* and key_type
 * as uint32_t to avoid pulling platform headers into the public PSA header
 * (crypto_driver_contexts_primitives.h).  Cast back to native types here.
 */
#define OP_SHND(op)   ((sl_ctx)(op)->shnd)
#define OP_KTYPE(op)  ((hal_key_type)(op)->key_type)

/**
 * Load an AES key into the hardware and return the slot index.
 * Caller must call sl_remove_key() when done.
 */
static int tizenrt_load_aes_key(sl_ctx shnd, hal_key_type key_type,
                                  const uint8_t *key_buf, size_t key_buf_size)
{
    hal_data aeskey = HAL_DATA_INITIALIZER;
    aeskey.data     = (void *)key_buf;
    aeskey.data_len = key_buf_size;

    return alt_set_key(shnd, key_type, &aeskey, NULL, AES_KEY_SLOT_START);
}

/* The starting slot for AES keys (same as ECP but AES uses lower indices) */
#define AES_KEY_SLOT_START  32

/* =========================================================================
 * One-shot cipher_encrypt
 * =========================================================================
 */

psa_status_t tizenrt_transparent_cipher_encrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *iv, size_t iv_length,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size,
    size_t *output_length)
{
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    if (!PSA_KEY_TYPE_IS_UNSTRUCTURED(key_type)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_aes_mode hal_mode = tizenrt_aes_hal_mode(alg);
    if (hal_mode == 0) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_key_type aes_ktype = tizenrt_aes_key_type(key_bits);
    if (aes_ktype == HAL_KEY_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Block-aligned check for block modes */
    if (!tizenrt_aes_is_stream(alg) && (input_length % AES_BLOCK_SIZE) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (output_size < input_length) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    sl_ctx shnd;
    if (sl_init(&shnd) != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    int key_idx = tizenrt_load_aes_key(shnd, aes_ktype,
                                        key_buffer, key_buffer_size);
    if (key_idx == -1) {
        sl_deinit(shnd);
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    hal_data aes_in  = {(void *)input,  input_length,  NULL, 0};
    hal_data aes_out = {output,         output_size,   NULL, 0};

    /* iv_buf: mutable copy of IV (HAL may update it for CBC chaining) */
    uint8_t iv_buf[AES_BLOCK_SIZE] = {0};
    if (tizenrt_aes_needs_iv(alg) && iv_length > 0) {
        size_t copy_len = iv_length < AES_BLOCK_SIZE ? iv_length : AES_BLOCK_SIZE;
        memcpy(iv_buf, iv, copy_len);
    }

    hal_aes_param aes_param;
    memset(&aes_param, 0, sizeof(aes_param));
    aes_param.mode = hal_mode;
    if (tizenrt_aes_needs_iv(alg)) {
        aes_param.iv      = iv_buf;
        aes_param.iv_len  = AES_BLOCK_SIZE;
    }

    int ret = sl_aes_encrypt(shnd, &aes_in, &aes_param, key_idx, &aes_out);
    (void)sl_remove_key(shnd, aes_ktype, key_idx);
    sl_deinit(shnd);

    if (ret != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    *output_length = aes_out.data_len;
    if (*output_length == 0) {
        /* Some HAL implementations may not update data_len; assume same as input */
        *output_length = input_length;
    }
    return PSA_SUCCESS;
}

/* =========================================================================
 * One-shot cipher_decrypt
 * =========================================================================
 */

psa_status_t tizenrt_transparent_cipher_decrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size,
    size_t *output_length)
{
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    if (!PSA_KEY_TYPE_IS_UNSTRUCTURED(key_type)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_aes_mode hal_mode = tizenrt_aes_hal_mode(alg);
    if (hal_mode == 0) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_key_type aes_ktype = tizenrt_aes_key_type(key_bits);
    if (aes_ktype == HAL_KEY_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /*
     * For one-shot decrypt, the input contains: IV || ciphertext
     * PSA psa_cipher_decrypt prepends the IV to the buffer before calling
     * the driver wrapper.
     */
    size_t iv_len = tizenrt_aes_needs_iv(alg) ? AES_BLOCK_SIZE : 0;
    if (input_length < iv_len) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    const uint8_t *iv_ptr    = tizenrt_aes_needs_iv(alg) ? input : NULL;
    const uint8_t *data_ptr  = input + iv_len;
    size_t         data_len  = input_length - iv_len;

    if (!tizenrt_aes_is_stream(alg) && (data_len % AES_BLOCK_SIZE) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (output_size < data_len) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    sl_ctx shnd;
    if (sl_init(&shnd) != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    int key_idx = tizenrt_load_aes_key(shnd, aes_ktype,
                                        key_buffer, key_buffer_size);
    if (key_idx == -1) {
        sl_deinit(shnd);
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    uint8_t iv_buf[AES_BLOCK_SIZE] = {0};
    if (iv_ptr) {
        memcpy(iv_buf, iv_ptr, AES_BLOCK_SIZE);
    }

    hal_data aes_in  = {(void *)data_ptr, data_len,    NULL, 0};
    hal_data aes_out = {output,           output_size, NULL, 0};

    hal_aes_param aes_param;
    memset(&aes_param, 0, sizeof(aes_param));
    aes_param.mode = hal_mode;
    if (tizenrt_aes_needs_iv(alg)) {
        aes_param.iv     = iv_buf;
        aes_param.iv_len = AES_BLOCK_SIZE;
    }

    int ret = sl_aes_decrypt(shnd, &aes_in, &aes_param, key_idx, &aes_out);
    (void)sl_remove_key(shnd, aes_ktype, key_idx);
    sl_deinit(shnd);

    if (ret != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    *output_length = aes_out.data_len;
    if (*output_length == 0) {
        *output_length = data_len;
    }
    return PSA_SUCCESS;
}

/* =========================================================================
 * Multi-part setup (shared logic)
 * =========================================================================
 */

static psa_status_t tizenrt_cipher_setup(
    tizenrt_transparent_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    int is_encrypt)
{
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    if (!PSA_KEY_TYPE_IS_UNSTRUCTURED(key_type)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_aes_mode hal_mode = tizenrt_aes_hal_mode(alg);
    if (hal_mode == 0) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_key_type aes_ktype = tizenrt_aes_key_type(key_bits);
    if (aes_ktype == HAL_KEY_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    memset(operation, 0, sizeof(*operation));

    /* sl_init takes sl_ctx* but the struct field is void*; use a local temp */
    sl_ctx tmp_shnd;
    if (sl_init(&tmp_shnd) != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }
    operation->shnd = (void *)tmp_shnd;

    int key_idx = tizenrt_load_aes_key(OP_SHND(operation), aes_ktype,
                                        key_buffer, key_buffer_size);
    if (key_idx == -1) {
        sl_deinit(OP_SHND(operation));
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    operation->key_idx      = key_idx;
    operation->key_type     = (uint32_t)aes_ktype;
    operation->alg          = alg;
    operation->is_encrypt   = is_encrypt;
    operation->iv_length    = AES_BLOCK_SIZE;
    operation->iv_set       = !tizenrt_aes_needs_iv(alg); /* ECB has no IV */
    operation->buffer_length = 0;
    operation->is_active    = 1;

    return PSA_SUCCESS;
}

psa_status_t tizenrt_transparent_cipher_encrypt_setup(
    tizenrt_transparent_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    return tizenrt_cipher_setup(operation, attributes,
                                 key_buffer, key_buffer_size, alg, 1);
}

psa_status_t tizenrt_transparent_cipher_decrypt_setup(
    tizenrt_transparent_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg)
{
    return tizenrt_cipher_setup(operation, attributes,
                                 key_buffer, key_buffer_size, alg, 0);
}

/* =========================================================================
 * Set IV
 * =========================================================================
 */

psa_status_t tizenrt_transparent_cipher_set_iv(
    tizenrt_transparent_cipher_operation_t *operation,
    const uint8_t *iv, size_t iv_length)
{
    if (!operation->is_active) {
        return PSA_ERROR_BAD_STATE;
    }
    if (iv_length > AES_BLOCK_SIZE) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    memset(operation->iv, 0, AES_BLOCK_SIZE);
    memcpy(operation->iv, iv, iv_length);
    operation->iv_set = 1;
    return PSA_SUCCESS;
}

/* =========================================================================
 * Multi-part update
 * =========================================================================
 * Accumulates input data, processes complete AES blocks through hardware,
 * and keeps the remainder (< 16 bytes) in the buffer.
 * =========================================================================
 */

psa_status_t tizenrt_transparent_cipher_update(
    tizenrt_transparent_cipher_operation_t *operation,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size,
    size_t *output_length)
{
    *output_length = 0;

    if (!operation->is_active || !operation->iv_set) {
        return PSA_ERROR_BAD_STATE;
    }

    if (input_length == 0) {
        return PSA_SUCCESS;
    }

    psa_algorithm_t alg = operation->alg;

    /* For stream ciphers, process everything immediately */
    if (tizenrt_aes_is_stream(alg)) {
        if (output_size < input_length) {
            return PSA_ERROR_BUFFER_TOO_SMALL;
        }

        hal_data aes_in  = {(void *)input, input_length, NULL, 0};
        hal_data aes_out = {output, output_size, NULL, 0};

        hal_aes_param aes_param;
        memset(&aes_param, 0, sizeof(aes_param));
        aes_param.mode   = tizenrt_aes_hal_mode(alg);
        aes_param.iv     = operation->iv;
        aes_param.iv_len = AES_BLOCK_SIZE;

        int ret;
        if (operation->is_encrypt) {
            ret = sl_aes_encrypt(OP_SHND(operation), &aes_in, &aes_param,
                                  operation->key_idx, &aes_out);
        } else {
            ret = sl_aes_decrypt(OP_SHND(operation), &aes_in, &aes_param,
                                  operation->key_idx, &aes_out);
        }

        if (ret != SECLINK_OK) {
            return PSA_ERROR_HARDWARE_FAILURE;
        }

        /* Update IV to reflect the current counter/state */
        if (aes_param.iv) {
            memcpy(operation->iv, aes_param.iv, AES_BLOCK_SIZE);
        }

        *output_length = (aes_out.data_len > 0) ? aes_out.data_len : input_length;
        return PSA_SUCCESS;
    }

    /* Block-mode: accumulate in buffer, process complete blocks */
    size_t total_avail = operation->buffer_length + input_length;
    size_t blocks_out  = (total_avail / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    if (blocks_out == 0) {
        /* Not enough data for a full block yet – just buffer it */
        if (operation->buffer_length + input_length > AES_BLOCK_SIZE) {
            return PSA_ERROR_BUFFER_TOO_SMALL; /* should not happen */
        }
        memcpy(operation->buffer + operation->buffer_length, input, input_length);
        operation->buffer_length += input_length;
        return PSA_SUCCESS;
    }

    if (output_size < blocks_out) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* Build a contiguous input: buffered bytes + new input (up to blocks_out bytes) */
    size_t from_new   = blocks_out - operation->buffer_length;
    size_t remaining  = input_length - from_new;

    /* Temporary buffer for the full contiguous input */
    unsigned char *tmp = NULL;
    const unsigned char *process_input;

    if (operation->buffer_length > 0) {
        tmp = (unsigned char *)malloc(blocks_out);
        if (!tmp) {
            return PSA_ERROR_INSUFFICIENT_MEMORY;
        }
        memcpy(tmp, operation->buffer, operation->buffer_length);
        memcpy(tmp + operation->buffer_length, input, from_new);
        process_input = tmp;
    } else {
        /* Buffer was empty: process directly from input */
        process_input = input;
        remaining = input_length % AES_BLOCK_SIZE;
        from_new  = input_length - remaining;
        blocks_out = from_new;
    }

    hal_data aes_in  = {(void *)process_input, blocks_out, NULL, 0};
    hal_data aes_out = {output, output_size, NULL, 0};

    hal_aes_param aes_param;
    memset(&aes_param, 0, sizeof(aes_param));
    aes_param.mode   = tizenrt_aes_hal_mode(alg);
    if (tizenrt_aes_needs_iv(alg)) {
        aes_param.iv     = operation->iv;
        aes_param.iv_len = AES_BLOCK_SIZE;
    }

    int ret;
    if (operation->is_encrypt) {
        ret = sl_aes_encrypt(OP_SHND(operation), &aes_in, &aes_param,
                              operation->key_idx, &aes_out);
    } else {
        ret = sl_aes_decrypt(OP_SHND(operation), &aes_in, &aes_param,
                              operation->key_idx, &aes_out);
    }

    if (tmp) {
        free(tmp);
    }

    if (ret != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    /* Update IV: for CBC it's the last ciphertext block */
    if (alg == PSA_ALG_CBC_NO_PADDING) {
        if (operation->is_encrypt) {
            /* IV = last output block */
            memcpy(operation->iv, output + blocks_out - AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        } else {
            /* IV = last input ciphertext block (before decryption) */
            memcpy(operation->iv, process_input + blocks_out - AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        }
    }

    /* Save remaining bytes to buffer */
    size_t new_buf_start = (operation->buffer_length > 0) ? from_new : 0;
    operation->buffer_length = remaining;
    if (remaining > 0) {
        memcpy(operation->buffer, input + new_buf_start, remaining);
    }

    *output_length = (aes_out.data_len > 0) ? aes_out.data_len : blocks_out;
    return PSA_SUCCESS;
}

/* =========================================================================
 * Multi-part finish
 * =========================================================================
 */

psa_status_t tizenrt_transparent_cipher_finish(
    tizenrt_transparent_cipher_operation_t *operation,
    uint8_t *output, size_t output_size,
    size_t *output_length)
{
    *output_length = 0;

    if (!operation->is_active) {
        return PSA_ERROR_BAD_STATE;
    }

    /* For stream ciphers, any remaining buffered bytes should not exist
     * (stream update processes everything immediately). */
    if (tizenrt_aes_is_stream(operation->alg)) {
        /* Nothing to flush */
        return PSA_SUCCESS;
    }

    /* For block ciphers, the buffer must be empty at finish
     * (PSA_ALG_CBC_NO_PADDING / PSA_ALG_ECB_NO_PADDING require block-aligned input). */
    if (operation->buffer_length != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return PSA_SUCCESS;
}

/* =========================================================================
 * Abort
 * =========================================================================
 */

psa_status_t tizenrt_transparent_cipher_abort(
    tizenrt_transparent_cipher_operation_t *operation)
{
    if (!operation->is_active) {
        return PSA_SUCCESS;
    }

    (void)sl_remove_key(OP_SHND(operation), OP_KTYPE(operation), operation->key_idx);
    sl_deinit(OP_SHND(operation));

    mbedtls_platform_zeroize(operation, sizeof(*operation));
    return PSA_SUCCESS;
}
