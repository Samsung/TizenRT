/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_aes.c
 *
 * This file contains the implementation of the internal functions to
 * perform symmetric encryption and decryption using the AES algorithm
 *
 */

#include "cc3xx_internal_aes.h"

#include "cc_pal_types.h"
#include "cc_pal_mem.h"
#include "cc_pal_abort.h"

/**
 * \ingroup internal_aes
 */
void cc3xx_aes_init(AesContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PalAbort("ctx cannot be NULL");
        return;
    }

    ctx->padType            = CRYPTO_PADDING_NONE;
    ctx->dataBlockType      = FIRST_BLOCK;
    ctx->inputDataAddrType  = DLLI_ADDR;
    ctx->outputDataAddrType = DLLI_ADDR;
}

/**
 * \ingroup internal_aes
 */
void cc3xx_aes_free(AesContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return;
    }

    CC_PalMemSet(ctx, 0, sizeof(AesContext_t));
}

static psa_status_t aes_setkey(
        AesContext_t *ctx,
        const uint8_t *key,
        size_t key_bits,
        cryptoDirection_t direction)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == key) {
        CC_PAL_LOG_ERR("key cannot be NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ctx->dir = direction;
    ctx->cryptoKey = USER_KEY;

    switch (key_bits) {
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
        CC_PAL_LOG_ERR("key_bits (%d) not supported\n", key_bits);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    CC_PalMemCopy(ctx->keyBuf, key, key_bits/8);

    return PSA_SUCCESS;
}

/** \defgroup internal_aes Internal AES module
 *
 *  Internal functions used by the driver to perform AES cipher encryption
 *  and decryption
 *
 *  @{
 */
psa_status_t cc3xx_aes_setkey_enc(
        AesContext_t *ctx,
        const uint8_t *key,
        size_t key_bits)
{
    return aes_setkey(ctx, key, key_bits, CRYPTO_DIRECTION_ENCRYPT);
}

psa_status_t cc3xx_aes_setkey_dec(
        AesContext_t *ctx,
        const uint8_t *key,
        size_t key_bits)
{
    return aes_setkey(ctx, key, key_bits, CRYPTO_DIRECTION_DECRYPT);
}

psa_status_t cc3xx_aes_crypt(
        AesContext_t *ctx,
        aesMode_t mode,
        size_t length,
        uint8_t iv[AES_IV_SIZE],
        const uint8_t *input,
        uint8_t *output)
{
    drvError_t drvRet;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    if (0 == length) {
        return PSA_SUCCESS;
    }

    if (NULL == ctx || NULL == input || NULL == output || NULL == iv) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (mode != CIPHER_CTR && length % AES_BLOCK_SIZE) {
        CC_PAL_LOG_ERR("Length %d not a multiple of the block size\n", length);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ctx->mode = mode;

    if (mode != CIPHER_ECB) {
        CC_PalMemCopy(ctx->ivBuf, iv, AES_IV_SIZE);
    }

    drvRet = SetDataBuffersInfo(input,  length, &inBuffInfo,
                                output, length, &outBuffInfo);
    if (drvRet != 0) {
        CC_PAL_LOG_ERR("Bad i/o buffers\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    drvRet = ProcessAesDrv(ctx, &inBuffInfo, &outBuffInfo, length);
    if (drvRet != AES_DRV_OK) {
        CC_PAL_LOG_ERR("cc3xx_aes_crypt failed: %d\n", drvRet);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (mode != CIPHER_ECB) {
        CC_PalMemCopy(iv, ctx->ivBuf, AES_IV_SIZE);
    }

    return PSA_SUCCESS;
}
/** @} */ // end of internal_aes
