/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_gcm.c
 *
 * This file contains the implementation of the internal functions required to
 * perform AEAD using AES-GCM
 *
 */
#include "aesgcm_driver.h"
#include "cc_common.h"
#include "cc_pal_abort.h"
#include "cc_pal_mem.h"
#include "cc_pal_types.h"

#include "cc3xx_internal_gcm.h"
#include "psa/crypto.h"

/*! AES GCM data in maximal size in bytes. */
#define CC3XX_GCM_DATA_IN_MAX_SIZE_BYTES 0xFFFF // (64KB - 1)
/*! AES GCM IV maximal size in bytes. */
#define CC3XX_GCM_IV_MAX_SIZE_BYTES 0xFFFF // (64KB - 1)
/*! AES GCM AAD maximal size in bytes. */
#define CC3XX_GCM_AAD_MAX_SIZE_BYTES 0xFFFF // (64KB - 1)

/*! AES GCM 96 bits size IV. */
#define CC3XX_GCM_IV_96_BITS_SIZE_BYTES 12

/*! AES GCM Tag size: 4 bytes. */
#define CC3XX_GCM_TAG_SIZE_4_BYTES 4
/*! AES GCM Tag size: 8 bytes. */
#define CC3XX_GCM_TAG_SIZE_8_BYTES 8
/*! AES GCM Tag size: 12 bytes. */
#define CC3XX_GCM_TAG_SIZE_12_BYTES 12
/*! AES GCM Tag size: 13 bytes. */
#define CC3XX_GCM_TAG_SIZE_13_BYTES 13
/*! AES GCM Tag size: 14 bytes. */
#define CC3XX_GCM_TAG_SIZE_14_BYTES 14
/*! AES GCM Tag size: 15 bytes. */
#define CC3XX_GCM_TAG_SIZE_15_BYTES 15
/*! AES GCM Tag size: 16 bytes. */
#define CC3XX_GCM_TAG_SIZE_16_BYTES 16

static psa_status_t gcm_setkey(
        AesGcmContext_t *ctx,
        const uint8_t *key,
        size_t key_bits,
        cryptoDirection_t direction)
{
    if (NULL == ctx || NULL == key) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (CRYPTO_DIRECTION_NUM_OF_ENC_MODES <= direction) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ctx->dir = direction;

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
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    CC_PalMemCopy(ctx->keyBuf, key, PSA_BITS_TO_BYTES(key_bits));

    return PSA_SUCCESS;
}

static psa_status_t gcm_calc_h(AesGcmContext_t *context)
{
    drvError_t rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    /* Set process mode to 'CalcH' */
    context->processMode = DRV_AESGCM_Process_CalcH;

    /* set data buffers structures */
    rc = SetDataBuffersInfo((uint8_t *)(context->tempBuf),
                            CC_AESGCM_GHASH_DIGEST_SIZE_BYTES, &inBuffInfo,
                            (uint8_t *)(context->H), AES_128_BIT_KEY_SIZE,
                            &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");

        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Calculate H */
    rc = ProcessAesGcm(context, &inBuffInfo, &outBuffInfo,
                       CC_AES_BLOCK_SIZE_IN_BYTES);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("calculating H failed with error code %d\n", rc);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return PSA_SUCCESS;
}

static psa_status_t gcm_init(AesGcmContext_t *context,
                             cryptoDirection_t direction, const uint8_t *pIv,
                             size_t ivSize, const uint8_t *pAad, size_t aadSize,
                             const uint8_t *pDataIn, size_t dataSize,
                             const uint8_t *pDataOut, const uint8_t *pTag,
                             size_t tagSize)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    /* Check the Encrypt / Decrypt flag validity */
    if (CRYPTO_DIRECTION_NUM_OF_ENC_MODES <= direction) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check the data in size validity */
    if (CC3XX_GCM_DATA_IN_MAX_SIZE_BYTES < dataSize) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (0 != dataSize) {
        /* Check dataIn pointer */
        if (NULL == pDataIn) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        /* Check dataOut pointer */
        if (NULL == pDataOut) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    }

    /* Check the IV size validity */
    if ((CC3XX_GCM_IV_MAX_SIZE_BYTES < ivSize) || (0 == ivSize)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check iv pointer */
    if (NULL == pIv) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check the AAD size validity */
    if (CC3XX_GCM_AAD_MAX_SIZE_BYTES < aadSize) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check aad pointer */
    if ((NULL == pAad) && (aadSize != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check the tag size validity */
    if ((tagSize != CC3XX_GCM_TAG_SIZE_4_BYTES) &&
        (tagSize != CC3XX_GCM_TAG_SIZE_8_BYTES) &&
        (tagSize != CC3XX_GCM_TAG_SIZE_12_BYTES) &&
        (tagSize != CC3XX_GCM_TAG_SIZE_13_BYTES) &&
        (tagSize != CC3XX_GCM_TAG_SIZE_14_BYTES) &&
        (tagSize != CC3XX_GCM_TAG_SIZE_15_BYTES) &&
        (tagSize != CC3XX_GCM_TAG_SIZE_16_BYTES)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check Tag pointer */
    if (NULL == pTag) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Set direction of operation: enc./dec. */
    context->dir = direction;
    context->dataSize = dataSize;
    context->ivSize = ivSize;
    context->aadSize = aadSize;
    context->tagSize = tagSize;

    /******************************************************/
    /***                Calculate H                     ***/
    /******************************************************/
    status = gcm_calc_h(context);
    if (status != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("calculating H failed with error code %d\n", status);
        return status;
    }

    return status;
}

static psa_status_t gcm_process_j0(AesGcmContext_t *context, const uint8_t *pIv,
                                   size_t iv_size)
{
    drvError_t rc = 0;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    /* Check the IV size validity */
    if ((CC3XX_GCM_IV_MAX_SIZE_BYTES < iv_size) || (0 == iv_size)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Init to default value */
    context->ivSize = 0;

    if (CC3XX_GCM_IV_96_BITS_SIZE_BYTES == iv_size) {
        /* Concatenate IV||0(31)||1 */
        CC_PalMemCopy(context->J0, pIv, CC3XX_GCM_IV_96_BITS_SIZE_BYTES);
        context->J0[3] = SWAP_ENDIAN(0x00000001);
    } else {

        /***********************************************/
        /* Calculate GHASH over the first phase buffer */
        /***********************************************/
        /* Set process mode to 'CalcJ0' */
        context->processMode = DRV_AESGCM_Process_CalcJ0_FirstPhase;

        /* set data buffers structures */
        rc = SetDataBuffersInfo(pIv, iv_size, &inBuffInfo, NULL, 0,
                                &outBuffInfo);
        if (rc != 0) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_DATA_INVALID;
        }

        /* Calculate J0 - First phase */
        rc = ProcessAesGcm(context, &inBuffInfo, &outBuffInfo, iv_size);
        if (rc != AES_DRV_OK) {
            CC_PAL_LOG_ERR(
                "calculating J0 (phase 1) failed with error code 0x%X\n", rc);
            return PSA_ERROR_GENERIC_ERROR;
        }

        /*********************************************/
        /* Build & Calculate the second phase buffer */
        /*********************************************/
        CC_PalMemSetZero(context->tempBuf, sizeof(context->tempBuf));
        context->tempBuf[3] = (iv_size << 3) & CC_32BIT_MAX_VALUE;
        context->tempBuf[3] = SWAP_ENDIAN(context->tempBuf[3]);

        /* Set process mode to 'CalcJ0' */
        context->processMode = DRV_AESGCM_Process_CalcJ0_SecondPhase;

        /* set data buffers structures */
        rc = SetDataBuffersInfo((uint8_t *)(context->tempBuf),
                                CC_AESGCM_GHASH_DIGEST_SIZE_BYTES, &inBuffInfo,
                                NULL, 0, &outBuffInfo);
        if (rc != 0) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_DATA_INVALID;
        }

        /* Calculate J0 - Second phase  */
        rc = ProcessAesGcm(context, &inBuffInfo, &outBuffInfo,
                           CC_AESGCM_GHASH_DIGEST_SIZE_BYTES);
        if (rc != AES_DRV_OK) {
            CC_PAL_LOG_ERR(
                "calculating J0 (phase 2) failed with error code %d\n", rc);
            return PSA_ERROR_DATA_INVALID;
        }
    }

    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("gcm process j0 failed with error code %d\n", rc);
        return PSA_ERROR_DATA_INVALID;
    } else {
        /* On success set the iv_size of the context */
        context->ivSize = iv_size;
        return PSA_SUCCESS;
    }
}

static psa_status_t gcm_process_aad(AesGcmContext_t *context,
                                    const uint8_t *pAad, size_t aadSize)
{
    drvError_t rc = 0;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    /* Check the AAD size validity */
    if (CC3XX_GCM_AAD_MAX_SIZE_BYTES < aadSize) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Clear Ghash result buffer */
    CC_PalMemSetZero(context->ghashResBuf, sizeof(context->ghashResBuf));

    /* Init to default value */
    context->aadSize = 0;

    if (aadSize == 0) {
        /* Nothing to do */
        return PSA_SUCCESS;
    }

    /* Set process mode to 'Process_A' */
    context->processMode = DRV_AESGCM_Process_A;

    /* set data buffers structures */
    rc = SetDataBuffersInfo(pAad, aadSize, &inBuffInfo, NULL, 0,
                            &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Calculate GHASH(A) */
    rc = ProcessAesGcm(context, &inBuffInfo, &outBuffInfo, aadSize);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("processing AAD failed with error code %d\n", rc);
        return PSA_ERROR_DATA_INVALID;
    } else {
        /* Set aadSize on context only on success */
        context->aadSize = aadSize;
        return PSA_SUCCESS;
    }
}

static psa_status_t gcm_process_cipher(AesGcmContext_t *context,
                                       size_t length,
                                       const uint8_t *pTextDataIn,
                                       uint8_t *pTextDataOut)
{
    drvError_t rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    /* Must NOT perform in this case */
    if (0 == length) {
        return PSA_SUCCESS;
    }

    /* Set process mode to 'Process_DataIn' */
    context->processMode = DRV_AESGCM_Process_DataIn;

    /* set data buffers structures */
    rc = SetDataBuffersInfo(pTextDataIn,  length, &inBuffInfo,
                            pTextDataOut, length, &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    rc = ProcessAesGcm(context, &inBuffInfo, &outBuffInfo, length);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("processing cipher failed with error code %d\n", rc);
        return PSA_ERROR_DATA_INVALID;
    } else {
        return PSA_SUCCESS;
    }
}

static psa_status_t gcm_process_lenA_lenC(AesGcmContext_t *context)
{
    drvError_t rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    /* Build buffer */
    context->tempBuf[1] = (context->aadSize << 3) & CC_32BIT_MAX_VALUE;
    context->tempBuf[1] = SWAP_ENDIAN(context->tempBuf[1]);
    context->tempBuf[0] = 0;
    context->tempBuf[3] = (context->dataSize << 3) & CC_32BIT_MAX_VALUE;
    context->tempBuf[3] = SWAP_ENDIAN(context->tempBuf[3]);
    context->tempBuf[2] = 0;

    /* Set process mode to 'Process_LenA_LenC' */
    context->processMode = DRV_AESGCM_Process_LenA_LenC;

    /* set data buffers structures */
    rc = SetDataBuffersInfo((uint8_t *)(context->tempBuf),
                            CC_AESGCM_GHASH_DIGEST_SIZE_BYTES, &inBuffInfo,
                            NULL, 0, &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Calculate GHASH(LenA || LenC) */
    rc = ProcessAesGcm(context, &inBuffInfo, &outBuffInfo,
                       CC_AESGCM_GHASH_DIGEST_SIZE_BYTES);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR(
            "processing Lengths of AAD and Cipher failed with error code %d\n",
            rc);
        return PSA_ERROR_DATA_INVALID;
    } else {
        return PSA_SUCCESS;
    }
}

static psa_status_t gcm_finish(AesGcmContext_t *context,
                               uint8_t *pTag,
                               size_t tag_size)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    int rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    /* The context has tag size already set, we must check that the buffer
     * size we have passed is big enough
     */
    if (tag_size < context->tagSize) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* Set process mode to 'Process_GctrFinal' */
    context->processMode = DRV_AESGCM_Process_GctrFinal;

    /* set data buffers structures */
    rc = SetDataBuffersInfo((uint8_t *)(context->tempBuf),
                            CC_AESGCM_GHASH_DIGEST_SIZE_BYTES, &inBuffInfo,
                            context->preTagBuf,
                            CC_AESGCM_GHASH_DIGEST_SIZE_BYTES, &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Calculate Encrypt and Calc. Tag */
    rc = ProcessAesGcm(context, &inBuffInfo, &outBuffInfo,
                       CC_AESGCM_GHASH_DIGEST_SIZE_BYTES);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("Finish operation failed with error code %d\n", rc);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (CRYPTO_DIRECTION_ENCRYPT == context->dir) {
        CC_PalMemCopy(pTag, context->preTagBuf, context->tagSize);
        status = PSA_SUCCESS;
    } else {
        if (CC_PalMemCmp(context->preTagBuf, pTag, context->tagSize) == 0) {
            status = PSA_SUCCESS;
        } else {
            status = PSA_ERROR_INVALID_ARGUMENT;
        }
    }

    return status;
}

static psa_status_t gcm_crypt_and_tag(
    cryptoDirection_t direction, const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size, psa_algorithm_t alg,
    const uint8_t *nonce, size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, uint8_t *tag, size_t tag_length,
    const uint8_t *input, size_t input_length, uint8_t *output,
    size_t *output_length)
{
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;
    AesGcmContext_t context;
    cc3xx_gcm_init(&context);

    status = gcm_setkey(&context,
                        key_buffer, PSA_BYTES_TO_BITS(key_buffer_size),
                        direction);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Aes-GCM Initialization function */
    status = gcm_init(&context, direction, nonce, nonce_length, additional_data,
                      additional_data_length, input, input_length, output, tag,
                      tag_length);

    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Aes-GCM Process J0 function */
    status = gcm_process_j0(&context, nonce, context.ivSize);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Aes-GCM Process AAD function */
    status = gcm_process_aad(&context, additional_data, context.aadSize);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Aes-GCM Process Cipher function */
    status = gcm_process_cipher(&context, context.dataSize, input, output);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* Aes-GCM Process LenA||LenC function */
    status = gcm_process_lenA_lenC(&context);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    status = gcm_finish(&context, tag, tag_length);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    if (CRYPTO_DIRECTION_ENCRYPT == context.dir) {
        /* Ciphertext length should include tag */
        *output_length = input_length + tag_length;
    } else {
        /* Plaintext length should not include tag */
        *output_length = input_length;
    }

cleanup:
    if ((CRYPTO_DIRECTION_DECRYPT == context.dir) && (status != PSA_SUCCESS)) {
        CC_PalMemSetZero(output, context.dataSize);
    }
    cc3xx_gcm_free(&context);

    return status;
}

/** \defgroup internal_gcm Internal GCM functions
 *
 *  Internal functions used by the driver to perform AEAD using AES-GCM mode
 *
 *  @{
 */
psa_status_t cc3xx_gcm_encrypt(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *plaintext,
    size_t plaintext_length, uint8_t *ciphertext, size_t ciphertext_size,
    size_t *ciphertext_length)
{
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;

    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_bits_t key_bits = psa_get_key_bits(attributes);
    psa_algorithm_t key_alg = psa_get_key_algorithm(attributes);

    size_t tag_length = PSA_AEAD_TAG_LENGTH(key_type, key_bits, key_alg);

    if (ciphertext_size < plaintext_length + tag_length) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /*
     * The ciphertext will be of the same length as the plaintext input,
     * and the tag will be placed after that.
     */
    uint8_t *tag = ciphertext + plaintext_length;

    status = gcm_crypt_and_tag(
        CRYPTO_DIRECTION_ENCRYPT, attributes, key_buffer, key_buffer_size, alg,
        nonce, nonce_length, additional_data, additional_data_length, tag,
        tag_length, plaintext, plaintext_length, ciphertext, ciphertext_length);

    return status;
}

psa_status_t cc3xx_gcm_decrypt(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *ciphertext,
    size_t ciphertext_length, uint8_t *plaintext, size_t plaintext_size,
    size_t *plaintext_length)
{
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;

    uint8_t local_tag_buffer[PSA_AEAD_TAG_MAX_SIZE];

    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_bits_t key_bits = psa_get_key_bits(attributes);
    psa_algorithm_t key_alg = psa_get_key_algorithm(attributes);

    size_t tag_length = PSA_AEAD_TAG_LENGTH(key_type, key_bits, key_alg);

    size_t ciphertext_length_without_tag = ciphertext_length - tag_length;
    const uint8_t *tag = ciphertext + ciphertext_length_without_tag;

    if (plaintext_size < ciphertext_length_without_tag) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    CC_PalMemCopy(local_tag_buffer, tag, tag_length);

    status = gcm_crypt_and_tag(
        CRYPTO_DIRECTION_DECRYPT, attributes, key_buffer, key_buffer_size, alg,
        nonce, nonce_length, additional_data, additional_data_length,
        local_tag_buffer, tag_length, ciphertext, ciphertext_length_without_tag,
        plaintext, plaintext_length);

    return status;
}

void cc3xx_gcm_init(AesGcmContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return;
    }

    CC_PalMemSetZero(ctx, sizeof(AesGcmContext_t));
}

void cc3xx_gcm_free(AesGcmContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return;
    }

    CC_PalMemSetZero(ctx, sizeof(AesGcmContext_t));
}

psa_status_t cc3xx_gcm_setkey_enc(
    AesGcmContext_t *ctx,
    const uint8_t *key,
    size_t key_bits)
{
    return gcm_setkey(ctx, key, key_bits, CRYPTO_DIRECTION_ENCRYPT);
}

psa_status_t cc3xx_gcm_setkey_dec(
    AesGcmContext_t *ctx,
    const uint8_t *key,
    size_t key_bits)
{
    return gcm_setkey(ctx, key, key_bits, CRYPTO_DIRECTION_DECRYPT);
}

psa_status_t cc3xx_gcm_set_nonce(
    AesGcmContext_t *ctx,
    const uint8_t *nonce,
    size_t nonce_size,
    size_t tag_size)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    if (NULL == ctx || NULL == nonce) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check the IV size validity */
    if ((CC3XX_GCM_IV_MAX_SIZE_BYTES < nonce_size) || (0 == nonce_size)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = gcm_calc_h(ctx);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("gcm_calc_h failed: %d\n", ret);
        return ret;
    }

    ret = gcm_process_j0(ctx, nonce, nonce_size);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("gcm_process_j0 failed: %d", ret);
        return ret;
    }

    /* Check the tag size validity */
    if ((tag_size != CC3XX_GCM_TAG_SIZE_4_BYTES) &&
        (tag_size != CC3XX_GCM_TAG_SIZE_8_BYTES) &&
        (tag_size != CC3XX_GCM_TAG_SIZE_12_BYTES) &&
        (tag_size != CC3XX_GCM_TAG_SIZE_13_BYTES) &&
        (tag_size != CC3XX_GCM_TAG_SIZE_14_BYTES) &&
        (tag_size != CC3XX_GCM_TAG_SIZE_15_BYTES) &&
        (tag_size != CC3XX_GCM_TAG_SIZE_16_BYTES)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    ctx->tagSize = tag_size;

    return PSA_SUCCESS;
}

psa_status_t cc3xx_gcm_set_lengths(
    AesGcmContext_t *ctx,
    size_t aadSize,
    size_t dataSize)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ((aadSize > CC3XX_GCM_AAD_MAX_SIZE_BYTES) ||
        (dataSize > CC3XX_GCM_DATA_IN_MAX_SIZE_BYTES)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ctx->aadSize  = aadSize;
    ctx->dataSize = dataSize;

    return PSA_SUCCESS;
}

psa_status_t cc3xx_gcm_update_ad(
    AesGcmContext_t *ctx,
    const uint8_t *aad,
    size_t aad_size)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    if (NULL == ctx || NULL == aad) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = gcm_process_aad(ctx, aad, aad_size);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("gcm_process_aad failed: %d", ret);
        return ret;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_gcm_update(
    AesGcmContext_t *ctx,
    size_t size,
    const uint8_t *input,
    uint8_t *output)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    if (NULL == ctx || NULL == input || NULL == output) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = gcm_process_cipher(ctx, size, input, output);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("gcm_process_cipher failed: %d", ret);
        return ret;
    }

    ret = gcm_process_lenA_lenC(ctx);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("gcm_process_lenA_lenC failed: %d", ret);
        return ret;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_gcm_finish(
    AesGcmContext_t *ctx,
    uint8_t *tag,
    size_t tag_size,
    size_t *tag_len)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    *tag_len = 0;

    if (NULL == ctx || NULL == tag) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = gcm_finish(ctx, tag, tag_size);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("gcm_finish failed: %d", ret);
        return ret;
    }

    *tag_len = ctx->tagSize;

    return PSA_SUCCESS;
}
/** @} */ // end of internal_gcm
