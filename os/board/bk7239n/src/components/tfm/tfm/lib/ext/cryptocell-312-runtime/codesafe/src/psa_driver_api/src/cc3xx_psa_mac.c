/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_mac.c
 *
 * This file contains the implementation of the entry points associated to the
 * mac capability (single-part and multipart) as described by the PSA
 * Cryptoprocessor Driver interface specification
 *
 */

#include "cc3xx_psa_mac.h"
#include "cc3xx_crypto_primitives_private.h"
#include "cc3xx_psa_cipher.h"
#include "cc3xx_psa_hash.h"
#include "aes_driver.h"
#include "cc_pal_log.h"
#include "cc_pal_mem.h"

/* SHA512 is not supported, this is the only size we will need */
#define MD_MAX_SIZE 32

/** \brief Return the block size for different hashing algorithms
 *
 */
static size_t psa_get_hash_block_size(psa_algorithm_t alg)
{
    switch (alg) {
    case PSA_ALG_MD5:
        return (64);
    case PSA_ALG_SHA_1:
        return (64);
    case PSA_ALG_SHA_224:
        return (64);
    case PSA_ALG_SHA_256:
        return (64);
    default:
        return (0);
    }
}

/** \brief Initialize a context
 *
 */
static psa_status_t cmac_init(AesContext_t *ctx)
{
    if (ctx == NULL) {
        CC_PAL_LOG_ERR("NULL pointer exception\n");
        return (PSA_ERROR_INVALID_ARGUMENT);
    }

    CC_PalMemSetZero(ctx, sizeof(AesContext_t));

    ctx->mode               = CIPHER_CMAC;
    ctx->padType            = CRYPTO_PADDING_NONE;
    ctx->dir                = CRYPTO_DIRECTION_ENCRYPT;
    ctx->inputDataAddrType  = DLLI_ADDR;
    ctx->outputDataAddrType = DLLI_ADDR;
    ctx->dataBlockType      = FIRST_BLOCK;

    return PSA_SUCCESS;
}

/** \brief Set the key on a multipart CMAC context
 *
 */
static psa_status_t cmac_setkey(AesContext_t *ctx, const unsigned char *key,
                                unsigned int keybits)
{
    if (ctx == NULL || key == NULL) {
        CC_PAL_LOG_ERR("Null pointer, ctx or key are NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    switch (keybits) {
    case 128:
        ctx->keySizeId = KEY_SIZE_128_BIT;
        break;
    case 192:
        ctx->keySizeId = KEY_SIZE_192_BIT;
        break;
    case 256:
        ctx->keySizeId = KEY_SIZE_256_BIT;
        break;
    default:
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* update key information in the context */
    ctx->cryptoKey = USER_KEY;

    /* Copy user key to context */
    CC_PalMemCopy(ctx->keyBuf, key, PSA_BITS_TO_BYTES(keybits));

    return PSA_SUCCESS;
}

/** \brief Setup a multipart CMAC context
 *
 */
static psa_status_t cmac_setup(cc3xx_cipher_operation_t *cmac,
                               const psa_key_attributes_t *attributes,
                               const uint8_t *key_buffer,
                               size_t key_buffer_size, psa_algorithm_t alg)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    /* call psa cipher setup */
    status = cc3xx_cipher_encrypt_setup(cmac, attributes, key_buffer,
                                        key_buffer_size, alg);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = cmac_init(&(cmac->ctx.aes));
    if (status != PSA_SUCCESS) {
        CC_PAL_LOG_ERR(" 'cmac_init' failed with psa return code %d\n", status);
        return status;
    }

    status = cmac_setkey(&(cmac->ctx.aes), key_buffer,
                         psa_get_key_bits(attributes));
    if (status != PSA_SUCCESS) {
        CC_PAL_LOG_ERR(" 'cmac_setkey' failed with psa return code %d\n",
                       status);
        return status;
    }
    return PSA_SUCCESS;
}

/** \brief Update a multipart CMAC operation with new inut data
 *
 */
static psa_status_t cmac_update(cc3xx_cipher_operation_t *cmac_ctx,
                                const unsigned char *input, size_t ilen)
{
    unsigned int block_size;
    size_t blocks_num;
    size_t main_chunk_in_bytes;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;
    block_size = cmac_ctx->block_size;

    if ((cmac_ctx->unprocessed_size > 0) &&
        (ilen > (block_size - cmac_ctx->unprocessed_size))) {
        CC_PalMemCopy(
            &(cmac_ctx)->unprocessed_data[cmac_ctx->unprocessed_size],
            input, block_size - cmac_ctx->unprocessed_size);

        drvError_t ret = SetDataBuffersInfo(
            (const uint8_t *)&(cmac_ctx)->unprocessed_data, block_size,
            &inBuffInfo, NULL, 0, &outBuffInfo);
        if (ret != CC_OK) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        ret = ProcessAesDrv(&(cmac_ctx->ctx.aes), &inBuffInfo, &outBuffInfo,
                            block_size);

        if (ret != AES_DRV_OK) {
            CC_PAL_LOG_ERR("ProcessAesDrv failed with return code %d\n", ret);
            return PSA_ERROR_GENERIC_ERROR;
        }

        input += (block_size - cmac_ctx->unprocessed_size);
        ilen -= (block_size - cmac_ctx->unprocessed_size);
        cmac_ctx->unprocessed_size = 0;
    }

    /* blocks_num is the number of blocks including any final partial block */
    blocks_num = (ilen + block_size - 1) / block_size;
    if (1 < blocks_num) {
        main_chunk_in_bytes = (blocks_num - 1) * block_size;

        drvError_t ret = SetDataBuffersInfo(input, main_chunk_in_bytes,
                                            &inBuffInfo, NULL, 0, &outBuffInfo);
        if (ret != CC_OK) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        /* Process the input data, excluding any final partial or complete block
         */
        ret = ProcessAesDrv(&(cmac_ctx->ctx.aes), &inBuffInfo, &outBuffInfo,
                            main_chunk_in_bytes);
        if (ret != AES_DRV_OK) {
            CC_PAL_LOG_ERR("ProcessAesDrv failed with return code %d\n", ret);
            return PSA_ERROR_GENERIC_ERROR;
        }

        ilen -= main_chunk_in_bytes;
        input += main_chunk_in_bytes;
    }

    /* If there is data left over that wasn't aligned to a block */
    if (ilen > 0) {
        CC_PalMemCopy(
            &(cmac_ctx)->unprocessed_data[cmac_ctx->unprocessed_size],
            input, ilen);
        cmac_ctx->unprocessed_size += ilen;
    }

    return PSA_SUCCESS;
}

/** \brief Finalize a multipart CMAC operation by producing the
 *         corresponding MAC
 */
static psa_status_t cmac_finish(cc3xx_cipher_operation_t *cmac_ctx,
                                unsigned char *output)
{
    drvError_t ret = CC_FAIL;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    if (cmac_ctx == NULL || output == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = SetDataBuffersInfo((const uint8_t *)&(cmac_ctx)->unprocessed_data,
                             cmac_ctx->unprocessed_size, &inBuffInfo, NULL,
                             0, &outBuffInfo);
    if (ret != CC_OK) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = FinishAesDrv(&(cmac_ctx->ctx.aes), &inBuffInfo, &outBuffInfo,
                       (uint32_t)(cmac_ctx)->unprocessed_size);

    if (ret != AES_DRV_OK) {
        CC_PAL_LOG_ERR("FinishAesDrv failed with return code 0x%x\n", ret);
        return PSA_ERROR_GENERIC_ERROR;
    }

    CC_PalMemCopy(output, cmac_ctx->ctx.aes.ivBuf, AES_IV_SIZE);

    CC_PalMemSetZero(cmac_ctx, sizeof(cc3xx_cipher_operation_t));

    return PSA_SUCCESS;
}

/** \brief Setup an HMAC operation context with a given key and hash
 *         algorithm
 */
static psa_status_t hmac_setup(cc3xx_mac_operation_t *operation,
                               const uint8_t *key_buffer,
                               size_t key_buffer_size, psa_algorithm_t hash_alg)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    uint8_t ipad[PSA_HMAC_MAX_HASH_BLOCK_SIZE];
    size_t i;
    size_t hash_size  = PSA_HASH_LENGTH(hash_alg);
    size_t block_size = psa_get_hash_block_size(hash_alg);
    cc3xx_hash_operation_t *hmac = &(operation->hmac);

    switch (hash_alg) {
#ifdef CC3XX_CONFIG_SUPPORT_SHA1
    case PSA_ALG_SHA_1:
        hmac->ctx.mode = HASH_SHA1;
        break;
#endif /* CC3XX_CONFIG_SUPPORT_SHA1 */
    case PSA_ALG_SHA_224:
        hmac->ctx.mode = HASH_SHA224;
        break;
    case PSA_ALG_SHA_256:
        hmac->ctx.mode = HASH_SHA256;
        break;
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (block_size > sizeof(ipad)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    if (block_size > sizeof(operation->opad)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    if (block_size < hash_size) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (key_buffer_size > block_size) {
        status = cc3xx_hash_compute(hash_alg, key_buffer, key_buffer_size, ipad,
                                    sizeof(ipad), &key_buffer_size);
        if (status != PSA_SUCCESS) {
            goto cleanup;
        }
    } else if (key_buffer_size != 0) {
        CC_PalMemCopy(ipad, key_buffer, key_buffer_size);
    }

    for (i = 0; i < key_buffer_size; i++) {
        ipad[i] ^= 0x36;
    }
    CC_PalMemSet(ipad + key_buffer_size, 0x36, block_size - key_buffer_size);

    for (i = 0; i < key_buffer_size; i++) {
        operation->opad[i] = ipad[i] ^ 0x36 ^ 0x5C;
    }
    CC_PalMemSet(operation->opad + key_buffer_size, 0x5C,
                 block_size - key_buffer_size);

    status = cc3xx_hash_setup(hmac, hash_alg);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    status = cc3xx_hash_update(hmac, ipad, block_size);

cleanup:
    CC_PalMemSetZero(ipad, sizeof(ipad));

    return status;
}

/** \brief Finalize a multipart HMAC operation by producing the
 *         corresponding MAC
 */
static psa_status_t hmac_finish(cc3xx_mac_operation_t *operation, uint8_t *mac,
                                size_t mac_size)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    uint8_t tmp[MD_MAX_SIZE];
    cc3xx_hash_operation_t *hmac = &(operation->hmac);
    psa_algorithm_t hash_alg;
    size_t hash_size  = 0;
    size_t block_size;

    switch (hmac->ctx.mode) {
#ifdef CC3XX_CONFIG_SUPPORT_SHA1
    case HASH_SHA1:
        hash_alg = PSA_ALG_SHA_1;
        break;
#endif /* CC3XX_CONFIG_SUPPORT_SHA1 */
    case HASH_SHA224:
        hash_alg = PSA_ALG_SHA_224;
        break;
    case HASH_SHA256:
        hash_alg = PSA_ALG_SHA_256;
        break;
    default:
        return PSA_ERROR_BAD_STATE;
    }

    block_size = psa_get_hash_block_size(hash_alg);

    status = cc3xx_hash_finish(hmac, tmp, sizeof(tmp), &hash_size);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = cc3xx_hash_setup(hmac, hash_alg);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    status = cc3xx_hash_update(hmac, operation->opad, block_size);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    status = cc3xx_hash_update(hmac, tmp, hash_size);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    status = cc3xx_hash_finish(hmac, tmp, sizeof(tmp), &hash_size);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    CC_PalMemCopy(mac, tmp, mac_size);

exit:
    CC_PalMemSetZero(tmp, hash_size);
    return status;
}

/** \brief Setup a multipart MAC operation context with given key and algorithm
 *
 */
static psa_status_t mac_setup(cc3xx_mac_operation_t *operation,
                              const psa_key_attributes_t *attributes,
                              const uint8_t *key_buffer, size_t key_buffer_size,
                              psa_algorithm_t alg)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    if (operation->alg != 0) {
        return PSA_ERROR_BAD_STATE;
    }

    CC_PalMemSetZero(operation, sizeof(cc3xx_mac_operation_t));
    if (PSA_ALG_FULL_LENGTH_MAC(alg) == PSA_ALG_CMAC) {
        status = cmac_setup(&(operation->cmac), attributes, key_buffer,
                            key_buffer_size, alg);
    } else if (PSA_ALG_IS_HMAC(alg)) {
        status = hmac_setup(operation, key_buffer, key_buffer_size,
                            PSA_ALG_HMAC_GET_HASH(alg));
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (status != PSA_SUCCESS) {
        CC_PalMemSetZero(operation, sizeof(cc3xx_mac_operation_t));
    } else {
        operation->alg = alg;
    }
    return status;
}

/** \defgroup psa_mac PSA driver entry points for MAC operations
 *
 *  Entry points for MAC computation and verification as described by the PSA
 *  Cryptoprocessor Driver interface specification
 *
 *  @{
 */
psa_status_t cc3xx_mac_sign_setup(cc3xx_mac_operation_t *operation,
                                  const psa_key_attributes_t *attributes,
                                  const uint8_t *key_buffer,
                                  size_t key_buffer_size, psa_algorithm_t alg)
{
    return mac_setup(operation, attributes, key_buffer, key_buffer_size, alg);
}

psa_status_t cc3xx_mac_verify_setup(cc3xx_mac_operation_t *operation,
                                    const psa_key_attributes_t *attributes,
                                    const uint8_t *key_buffer,
                                    size_t key_buffer_size, psa_algorithm_t alg)
{
    return mac_setup(operation, attributes, key_buffer, key_buffer_size, alg);
}

psa_status_t cc3xx_mac_update(cc3xx_mac_operation_t *operation,
                              const uint8_t *input, size_t input_length)
{
    if (operation == NULL || input == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    if (PSA_ALG_FULL_LENGTH_MAC(operation->alg) == PSA_ALG_CMAC) {
        return cmac_update(&(operation->cmac), input, input_length);
    } else if (PSA_ALG_IS_HMAC(operation->alg)) {
        return cc3xx_hash_update(&(operation->hmac), input, input_length);
    } else {
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t cc3xx_mac_sign_finish(cc3xx_mac_operation_t *operation,
                                   uint8_t *mac, size_t mac_size,
                                   size_t *mac_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    if (PSA_ALG_FULL_LENGTH_MAC(operation->alg) == PSA_ALG_CMAC) {
        status = cmac_finish(&(operation->cmac), mac);
    } else if (PSA_ALG_IS_HMAC(operation->alg)) {
        status = hmac_finish(operation, mac, mac_size);
    } else {
        return PSA_ERROR_BAD_STATE;
    }

    if (status != PSA_SUCCESS) {
        CC_PalMemSetZero(mac, *mac_length);
        return status;
    } else {
        *mac_length = mac_size;
    }
    return PSA_SUCCESS;
}

psa_status_t cc3xx_mac_verify_finish(cc3xx_mac_operation_t *operation,
                                     const uint8_t *mac, size_t mac_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    uint8_t actual_mac[PSA_MAC_MAX_SIZE];

    if (mac_length > sizeof(actual_mac)) {
        return (PSA_ERROR_INVALID_ARGUMENT);
    }
    if (PSA_ALG_FULL_LENGTH_MAC(operation->alg) == PSA_ALG_CMAC) {
        status = cmac_finish(&(operation->cmac), actual_mac);
    } else if (PSA_ALG_IS_HMAC(operation->alg)) {
        status = hmac_finish(operation, actual_mac, mac_length);
    }
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    if (CC_PalMemCmp(mac, actual_mac, mac_length)) {
        status = PSA_ERROR_INVALID_SIGNATURE;
    }

cleanup:
    CC_PalMemSetZero(actual_mac, sizeof(actual_mac));
    return status;
}

psa_status_t cc3xx_mac_abort(cc3xx_mac_operation_t *operation)
{
    /* if alg is not set the operation has not been setup yet, nothing to do */
    if (operation->alg == 0) {
        return PSA_SUCCESS;
    }
    CC_PalMemSetZero(operation, sizeof(cc3xx_mac_operation_t));
    return PSA_SUCCESS;
}

psa_status_t cc3xx_mac_compute(const psa_key_attributes_t *attributes,
                               const uint8_t *key_buffer,
                               size_t key_buffer_size, psa_algorithm_t alg,
                               const uint8_t *input, size_t input_length,
                               uint8_t *mac, size_t mac_size,
                               size_t *mac_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    cc3xx_mac_operation_t operation = {0};

    status = cc3xx_mac_sign_setup(&operation, attributes, key_buffer,
                                  key_buffer_size, alg);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    if (input_length > 0) {
        status = cc3xx_mac_update(&operation, input, input_length);
    }
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    status = cc3xx_mac_sign_finish(&operation, mac, mac_size, mac_length);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

exit:
    cc3xx_mac_abort(&operation);
    return status;
}
/** @} */ // end of psa_mac
