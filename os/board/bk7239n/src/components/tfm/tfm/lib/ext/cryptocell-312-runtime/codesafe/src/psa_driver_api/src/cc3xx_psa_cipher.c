/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_cipher.c
 *
 * This file contains the implementation of the entry points associated to the
 * cipher capability (single-part and multipart) as described by the PSA
 * Cryptoprocessor Driver interface specification
 *
 */

#include "cc3xx_psa_cipher.h"
#include "cc3xx_crypto_primitives_private.h"
#include "cc3xx_internal_aes.h"
#include "cc3xx_internal_chacha20.h"
#include "cc_pal_mem.h"
#include "cc_pal_log.h"

/**
 * \brief By default, the driver interface enables Chacha20
 */
#ifndef CC3XX_CONFIG_SUPPORT_CHACHA20
#define CC3XX_CONFIG_SUPPORT_CHACHA20
#endif /* CC3XX_CONFIG_SUPPORT_CHACHA20 */

/* FixMe: The strategy to configure the CC3XX driver layer is not finalised,
 *        so for the time being we just use the mbed TLS config file to
 *        understand if we need to set CC3XX specific config defines.
 */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* FixMe: Temporary way of bridging mbed TLS based configuration
 *        with specific CC3XX driver configuration defines
 */
#ifndef MBEDTLS_CHACHA20_C
#undef CC3XX_CONFIG_SUPPORT_CHACHA20
#endif

static psa_status_t add_pkcs_padding(
        uint8_t *output,
        size_t output_size,
        size_t   data_size)
{
    if (NULL == output) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    uint8_t padding_size = output_size - data_size;

    for (size_t i = 0; i < padding_size; i++) {
        output[data_size + i] = padding_size;
    }

    return PSA_SUCCESS;
}

static psa_status_t get_pkcs_padding(
        const uint8_t *input,
        size_t input_len,
        size_t *data_len)
{
    size_t i, pad_idx;
    uint8_t padding_len, bad = 0;

    if(NULL == input || NULL == data_len) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    padding_len = input[input_len - 1];
    *data_len = input_len - padding_len;

    /* Avoid logical || since it results in a branch */
    bad |= padding_len > input_len;
    bad |= padding_len == 0;

    /* The number of bytes checked must be independent of padding_len,
     * so pick input_len, which is usually 8 or 16 (one block) */
    pad_idx = input_len - padding_len;
    for( i = 0; i < input_len; i++ ) {
        bad |= ( input[i] ^ padding_len ) * ( i >= pad_idx );
    }

    return( PSA_ERROR_INVALID_PADDING * ( bad != 0 ) );
}

static psa_status_t cipher_setup(
        cc3xx_cipher_operation_t *operation,
        const psa_key_attributes_t *attributes,
        const uint8_t *key, size_t key_length,
        psa_algorithm_t alg,
        psa_encrypt_or_decrypt_t dir)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    if (PSA_BYTES_TO_BITS(key_length) != key_bits) {
        /* The attributes don't match the buffer given as input */
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (!(PSA_ALG_IS_CIPHER(alg) || PSA_ALG_IS_BLOCK_CIPHER_MAC(alg))) {
     return PSA_ERROR_INVALID_ARGUMENT;
    }

    CC_PalMemSetZero(operation, sizeof(cc3xx_cipher_operation_t));

    operation->alg        = alg;
    operation->key_type   = key_type;
    operation->dir        = dir;
    operation->iv_size    = PSA_CIPHER_IV_LENGTH(key_type, alg);
    operation->block_size = (PSA_ALG_IS_STREAM_CIPHER(alg) ? 1 :
                             PSA_BLOCK_CIPHER_BLOCK_LENGTH(key_type));

    switch (operation->key_type) {
    case PSA_KEY_TYPE_AES:
        cc3xx_aes_init(&operation->ctx.aes);

        switch (operation->dir) {
        case PSA_CRYPTO_DRIVER_ENCRYPT:
            if (( ret = cc3xx_aes_setkey_enc(
                    &operation->ctx.aes,
                    key,
                    key_bits) )
                != PSA_SUCCESS) {
                return ret;
            }

            break;
        case PSA_CRYPTO_DRIVER_DECRYPT:
            if (( ret = cc3xx_aes_setkey_dec(
                    &operation->ctx.aes,
                    key,
                    key_bits) )
                != PSA_SUCCESS) {
                return ret;
            }

            break;
        default:
            return PSA_ERROR_NOT_SUPPORTED;
        }

        break;
#if defined(CC3XX_CONFIG_SUPPORT_CHACHA20)
    case PSA_KEY_TYPE_CHACHA20:
        cc3xx_chacha20_init(&operation->ctx.chacha);

        if (( ret = cc3xx_chacha20_setkey(
                &operation->ctx.chacha,
                key,
                key_length) )
            != PSA_SUCCESS) {
            return ret;
        }
        break;
#endif /* CC3XX_CONFIG_SUPPORT_CHACHA20 */

    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (operation->alg == PSA_ALG_CBC_PKCS7) {
        operation->add_padding = add_pkcs_padding;
        operation->get_padding = get_pkcs_padding;
    } else {
        operation->add_padding = NULL;
        operation->get_padding = NULL;
    }

    return ret;
}

/** \defgroup psa_cipher PSA driver entry points for symmetric cipher
 *
 *  Entry points for symmetric cipher encryption and decryption as described
 *  by the PSA Cryptoprocessor Driver interface specification
 *
 *  @{
 */
psa_status_t cc3xx_cipher_encrypt_setup(
        cc3xx_cipher_operation_t *operation,
        const psa_key_attributes_t *attributes,
        const uint8_t *key, size_t key_length,
        psa_algorithm_t alg)
{
    return cipher_setup(operation,
                        attributes,
                        key, key_length,
                        alg,
                        PSA_CRYPTO_DRIVER_ENCRYPT);
}

psa_status_t cc3xx_cipher_decrypt_setup(
        cc3xx_cipher_operation_t *operation,
        const psa_key_attributes_t *attributes,
        const uint8_t *key, size_t key_length,
        psa_algorithm_t alg)
{
    return cipher_setup(operation,
                        attributes,
                        key, key_length,
                        alg,
                        PSA_CRYPTO_DRIVER_DECRYPT);
}

psa_status_t cc3xx_cipher_set_iv(
        cc3xx_cipher_operation_t *operation,
        const uint8_t *iv, size_t iv_length)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    if (iv_length > AES_IV_SIZE) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    switch (operation->key_type) {
#if defined(CC3XX_CONFIG_SUPPORT_CHACHA20)
    case  PSA_KEY_TYPE_CHACHA20:
    {
        uint8_t *iv_pnt = (uint8_t *)iv;
        uint32_t counter = 0; /* Use zero as specified by PSA Crypto spec */
        /* When the IV is 16 bytes the first four bytes contain the counter in
         * little endian. When the IV is 12 bytes it contains only the IV and
         * the counter should be set to 0. More information in ChaCha20 section
         * here: https://armmbed.github.io/mbed-crypto/html/api/ops/ciphers.html
         */
        switch (iv_length) {
        case 16:
            /* Read the counter value in little endian */
            counter = (iv[3] << 24) | (iv[2] << 16) | (iv[1] << 8) | iv[0];
            iv_pnt += 4;
            iv_length -= 4;
        case 12:
            ret = cc3xx_chacha20_set_counter(&operation->ctx.chacha,
                                             counter);
            if (ret == PSA_SUCCESS) {
                ret = cc3xx_chacha20_set_nonce(&operation->ctx.chacha,
                                               iv_pnt,
                                               iv_length);
            }
            break;
        case 8:
            /* Original Chacha, i.e. 64 bit nonce and 64 bit counter */
            ret = PSA_ERROR_NOT_SUPPORTED;
            break;
        default:
            ret = PSA_ERROR_INVALID_ARGUMENT;
            break;
        }
    }
    break;
#endif /* CC3XX_CONFIG_SUPPORT_CHACHA20 */

    case PSA_KEY_TYPE_AES:

        if (iv_length != AES_IV_SIZE) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        CC_PalMemCopy(operation->iv, iv, iv_length);
        operation->iv_size = iv_length;
        ret = PSA_SUCCESS;
        break;

    default:
        ret = PSA_ERROR_NOT_SUPPORTED;
    }

    return ret;
}

psa_status_t cc3xx_cipher_update(
        cc3xx_cipher_operation_t *operation,
        const uint8_t *input, size_t input_length,
        uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    size_t block_size = operation->block_size;
    size_t expected_output_size;

    if (!PSA_ALG_IS_STREAM_CIPHER(operation->alg)) {
        expected_output_size =
                ( operation->unprocessed_size + input_length )
                / operation->block_size * operation->block_size;
    } else {
        expected_output_size = input_length;
    }

    if (output_size < expected_output_size) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    *output_length = 0;

    size_t copy_len;

    switch (operation->key_type) {
    case PSA_KEY_TYPE_AES:
        switch (operation->alg) {
        case PSA_ALG_CBC_NO_PADDING:
        case PSA_ALG_CBC_PKCS7:
            if ((operation->dir == PSA_CRYPTO_DRIVER_DECRYPT &&
                 NULL != operation->add_padding              &&
                 input_length   <= block_size -
                 operation->unprocessed_size)                ||
                (operation->dir == PSA_CRYPTO_DRIVER_DECRYPT &&
                 NULL == operation->add_padding              &&
                 input_length    < block_size -
                 operation->unprocessed_size)                ||
                (operation->dir == PSA_CRYPTO_DRIVER_ENCRYPT &&
                 input_length    < block_size -
                 operation->unprocessed_size)) {
                CC_PalMemCopy(&(operation->unprocessed_data
                [operation->unprocessed_size]),
                              input,
                              input_length);

                operation->unprocessed_size += input_length;
                return PSA_SUCCESS;
            }

            if (0 != operation->unprocessed_size) {
                copy_len = block_size
                           - operation->unprocessed_size;

                CC_PalMemCopy(&(operation->unprocessed_data
                               [operation->unprocessed_size]),
                              input,
                              copy_len);

                if (( ret = cc3xx_aes_crypt(
                        &operation->ctx.aes,
                        CIPHER_CBC,
                        block_size,
                        operation->iv,
                        operation->unprocessed_data,
                        output) )
                    != PSA_SUCCESS) {
                    return ret;
                }

                operation->unprocessed_size = 0;

                *output_length += block_size;
                output         += block_size;

                input          += copy_len;
                input_length   -= copy_len;
            }

            if (0 != input_length) {
                copy_len = input_length % block_size;

                if (copy_len == 0 &&
                    operation->dir==PSA_CRYPTO_DRIVER_DECRYPT &&
                    operation->add_padding != NULL) {
                    copy_len = block_size;
                }

                CC_PalMemCopy(operation->unprocessed_data,
                              &(input[input_length - copy_len]),
                              copy_len);

                operation->unprocessed_size += copy_len;
                input_length   -= copy_len;
            }

            if (0 != input_length) {
                if (( ret = cc3xx_aes_crypt(
                        &operation->ctx.aes,
                        CIPHER_CBC,
                        input_length,
                        operation->iv,
                        input,
                        output) )
                    != PSA_SUCCESS) {
                    return ret;
                }

                *output_length = input_length;
            }

            break;
        case PSA_ALG_ECB_NO_PADDING:
            if (( ret = cc3xx_aes_crypt(
                    &operation->ctx.aes,
                    CIPHER_ECB,
                    input_length,
                    operation->iv,
                    input,
                    output) )
                != PSA_SUCCESS) {
                return ret;
            }

            *output_length = input_length;

            break;
        case PSA_ALG_CTR:
            if (( ret = cc3xx_aes_crypt(
                    &operation->ctx.aes,
                    CIPHER_CTR,
                    input_length,
                    operation->iv,
                    input,
                    output) )
                != PSA_SUCCESS) {
                return ret;
            }

            *output_length = input_length;

            break;
        case PSA_ALG_OFB:
            if (( ret = cc3xx_aes_crypt(
                    &operation->ctx.aes,
                    CIPHER_OFB,
                    input_length,
                    operation->iv,
                    input,
                    output) )
                != PSA_SUCCESS) {
                return ret;
            }

            *output_length = input_length;

            break;
        default:
            return PSA_ERROR_NOT_SUPPORTED;
        } /* operation->alg */

        if (*output_length > output_size) {
            return PSA_ERROR_CORRUPTION_DETECTED;
        }
        break;

#if defined(CC3XX_CONFIG_SUPPORT_CHACHA20)
    case PSA_KEY_TYPE_CHACHA20:
        if (( ret = cc3xx_chacha20_update(
                &operation->ctx.chacha,
                input,
                input_length,
                output,
                output_size,
                output_length) )
            != PSA_SUCCESS) {
            return ret;
        }
        break;
#endif /* CC3XX_CONFIG_SUPPORT_CHACHA20 */

    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return ret;
}

psa_status_t cc3xx_cipher_finish(
        cc3xx_cipher_operation_t *operation,
        uint8_t *output,
        size_t output_size,
        size_t *output_length)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    /* Buffer for encypted/decrypted data if we have unprocessed data. This
     * is neccesary when PKCS7 padding is used so that we don't write the
     * actual padding data to the output buffer when performing decryption.
     */
    uint8_t temp_buff[AES_BLOCK_SIZE];
    /* The length of the data without the padding, this is updated for PKCS7
     * decryption only.
     */
    size_t data_len = operation->block_size;

    if(data_len > AES_BLOCK_SIZE){
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    *output_length = 0;

    switch (operation->key_type) {
    case PSA_KEY_TYPE_AES:
        switch (operation->alg) {
        case PSA_ALG_CBC_NO_PADDING:
        case PSA_ALG_CBC_PKCS7:
            if (operation->dir == PSA_CRYPTO_DRIVER_ENCRYPT) {
                if (operation->add_padding == NULL) {
                    if (operation->unprocessed_size != 0) {
                        /* Return PSA_ERROR_INVALID_ARGUMENT as this condition
                         * implies that a previous call to psa_cipher_update()
                         * has not been performed to completely align to a
                         * block of data and mode is PSA_ALG_CBC_NO_PADDING
                         */
                        return PSA_ERROR_INVALID_ARGUMENT;
                    }

                    return PSA_SUCCESS;
                }

                if (( ret = operation->add_padding(
                        operation->unprocessed_data,
                        operation->block_size,
                        operation->unprocessed_size) )
                    != PSA_SUCCESS) {
                    return ret;
                }
            } else if (operation->block_size !=
                       operation->unprocessed_size) {
                if (operation->add_padding == NULL &&
                    operation->unprocessed_size == 0) {
                    return PSA_SUCCESS;
                }

                /* Return PSA_ERROR_INVALID_ARGUMENT as this condition implies
                 * that a previous call to psa_cipher_update() has not been
                 * performed to completely align to a block of data and mode is
                 * PSA_ALG_CBC_NO_PADDING
                 */
                return PSA_ERROR_INVALID_ARGUMENT;
            }

            if (( ret = cc3xx_aes_crypt(
                    &operation->ctx.aes,
                    CIPHER_CBC,
                    operation->block_size,
                    operation->iv,
                    operation->unprocessed_data,
                    temp_buff))
                != PSA_SUCCESS) {
                return ret;
            }

            /* Set output size for decryption */
            if (operation->dir == PSA_CRYPTO_DRIVER_DECRYPT){
                /* get_padding() is initialised for CBC_PKCS7 only */
                if (operation->get_padding == NULL) {
                    return PSA_ERROR_CORRUPTION_DETECTED;
                }

                ret = operation->get_padding(
                            temp_buff,
                            operation->block_size,
                            &data_len);

                if (ret != PSA_SUCCESS){
                    return ret;
                }

            }

            if (output_size < data_len) {
                return PSA_ERROR_BUFFER_TOO_SMALL;
            }

            CC_PalMemCopy(output, temp_buff, data_len);
            *output_length = data_len;

            break;
        case PSA_ALG_ECB_NO_PADDING:
        case PSA_ALG_CTR:
        case PSA_ALG_OFB:
            return PSA_SUCCESS;
        default:
            return PSA_ERROR_NOT_SUPPORTED;
        } /* operation->alg */
        break;

#if defined(CC3XX_CONFIG_SUPPORT_CHACHA20)
    case PSA_KEY_TYPE_CHACHA20:
        ret = cc3xx_chacha20_finish(&operation->ctx.chacha, output,
                                    output_size, output_length);
        cc3xx_chacha20_free(&operation->ctx.chacha);
        break;
#endif /* CC3XX_CONFIG_SUPPORT_CHACHA20 */

    default:
        ret = PSA_ERROR_NOT_SUPPORTED;
    }

    return ret;
}

psa_status_t cc3xx_cipher_abort(cc3xx_cipher_operation_t *operation)
{
    switch (operation->key_type) {
    case PSA_KEY_TYPE_AES:
        cc3xx_aes_free(&operation->ctx.aes);
        break;
#if defined(CC3XX_CONFIG_SUPPORT_CHACHA20)
    case PSA_KEY_TYPE_CHACHA20:
        cc3xx_chacha20_free(&operation->ctx.chacha);
        break;
#endif /* CC3XX_CONFIG_SUPPORT_CHACHA20 */
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    CC_PalMemSetZero(operation, sizeof(cc3xx_cipher_operation_t));

    return PSA_SUCCESS;
}

psa_status_t cc3xx_cipher_encrypt(
        const psa_key_attributes_t *attributes,
        const uint8_t *key_buffer,
        size_t key_buffer_size,
        psa_algorithm_t alg,
        const uint8_t *iv,
        size_t iv_length,
        const uint8_t *input,
        size_t input_length,
        uint8_t *output,
        size_t output_size,
        size_t *output_length)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    cc3xx_cipher_operation_t operation = {0};
    size_t olength, accumulated_length = 0;

    if ( (ret = cc3xx_cipher_encrypt_setup(
            &operation,
            attributes,
            key_buffer,
            key_buffer_size,
            alg) )
         != PSA_SUCCESS) {
        cc3xx_cipher_abort(&operation);
        return ret;
    }

    /* ECB doesn't not expect an IV */
    if (alg != PSA_ALG_ECB_NO_PADDING) {
        if (operation.iv_size != iv_length) {
            cc3xx_cipher_abort(&operation);
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if ( (ret = cc3xx_cipher_set_iv(
                &operation,
                iv,
                iv_length) )
             != PSA_SUCCESS) {
            cc3xx_cipher_abort(&operation);
            return ret;
        }
    }

    if ( (ret = cc3xx_cipher_update(
            &operation,
            input,
            input_length,
            output,
            output_size,
            &olength) )
         != PSA_SUCCESS) {
        cc3xx_cipher_abort(&operation);
        return ret;
    }

    accumulated_length += olength;

    if ( (ret = cc3xx_cipher_finish(
            &operation,
            output + accumulated_length,
            output_size - accumulated_length,
            &olength) )
         != PSA_SUCCESS) {
        cc3xx_cipher_abort(&operation);
        return ret;
    }

    *output_length = accumulated_length + olength;

    ret = cc3xx_cipher_abort(&operation);

    return ret;
}

psa_status_t cc3xx_cipher_decrypt(
        const psa_key_attributes_t *attributes,
        const uint8_t *key_buffer,
        size_t key_buffer_size,
        psa_algorithm_t alg,
        const uint8_t *input,
        size_t input_length,
        uint8_t *output,
        size_t output_size,
        size_t *output_length)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;
    cc3xx_cipher_operation_t operation = {0};
    size_t olength, accumulated_length;

    if ( (ret = cc3xx_cipher_decrypt_setup(
            &operation,
            attributes,
            key_buffer,
            key_buffer_size,
            alg) )
         != PSA_SUCCESS) {
        cc3xx_cipher_abort(&operation);
        return ret;
    }

    /* ECB doesn't not expect an IV */
    if (alg != PSA_ALG_ECB_NO_PADDING) {
        if ( (ret = cc3xx_cipher_set_iv(
                &operation,
                input,
                operation.iv_size) )
             != PSA_SUCCESS) {
            cc3xx_cipher_abort(&operation);
            return ret;
        }
    }

    if ( (ret = cc3xx_cipher_update(
            &operation,
            input + operation.iv_size,
            input_length - operation.iv_size,
            output,
            output_size,
            &olength) )
         != PSA_SUCCESS) {
        cc3xx_cipher_abort(&operation);
        return ret;
    }

    accumulated_length = olength;

    if ( (ret = cc3xx_cipher_finish(
            &operation,
            output + accumulated_length,
            output_size - accumulated_length,
            &olength) )
         != PSA_SUCCESS) {
        cc3xx_cipher_abort(&operation);
        return ret;
    }

    *output_length = accumulated_length + olength;

    ret = cc3xx_cipher_abort(&operation);

    return ret;
}
/** @} */ // end of psa_cipher
