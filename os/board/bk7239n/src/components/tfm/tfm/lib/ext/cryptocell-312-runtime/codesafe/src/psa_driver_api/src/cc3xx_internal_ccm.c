/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_ccm.c
 *
 * This file contains the implementation of the internal functions to
 * perform AEAD using the AES-CCM algorithm
 *
 */

#define CC_PAL_LOG_CUR_COMPONENT CC_LOG_MASK_CC_API

#include "aesccm_driver.h"
#include "cc_common.h"
#include "cc_pal_abort.h"
#include "cc_pal_mem.h"
#include "cc_pal_types.h"

#include "cc3xx_internal_ccm.h"
#include <psa/crypto.h>

/*! The size of the AES CCM star nonce in bytes. */
#define CC3XX_CCM_STAR_NONCE_SIZE_BYTES 13

/* AES-CCM* Security levels (ieee-802.15.4-2011, Table 58) */
#define CC3XX_CCM_STAR_SECURITY_LEVEL_NONE 0
#define CC3XX_CCM_STAR_SECURITY_LEVEL_MIC_32 1
#define CC3XX_CCM_STAR_SECURITY_LEVEL_MIC_64 2
#define CC3XX_CCM_STAR_SECURITY_LEVEL_MIC_128 3
#define CC3XX_CCM_STAR_SECURITY_LEVEL_ENC 4
#define CC3XX_CCM_STAR_SECURITY_LEVEL_ENC_MIC_32 5
#define CC3XX_CCM_STAR_SECURITY_LEVEL_ENC_MIC_64 6
#define CC3XX_CCM_STAR_SECURITY_LEVEL_ENC_MIC_128 7

static psa_status_t ccm_setkey(
        AesCcmContext_t *ctx,
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
#if !defined(ARCH_IS_CC310)
    case 192:
        ctx->keySizeId = KEY_SIZE_192_BIT;
        break;
    case 256:
        ctx->keySizeId = KEY_SIZE_256_BIT;
        break;
#endif /* ARCH_IS_CC310 */
    default:
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    CC_PalMemCopy(ctx->keyBuf, key, PSA_BITS_TO_BYTES(key_bits));

    return PSA_SUCCESS;
}

static psa_status_t ccm_get_security_level(uint8_t SizeOfT,
                                           uint8_t SecurityField,
                                           uint8_t *pSecurityLevel)
{
    if (pSecurityLevel == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The security level field for AES-CCM* as defined in ieee-802.15.4-2011,
     * Table 58. System spec requirement CCM*-3: The CCM* shall support only the
     * security levels that include encryption (1XX values). */
    if (SecurityField < 4) {
        switch (SizeOfT) {
        case 0:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_NONE;
            break;
        case 4:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_MIC_32;
            break;
        case 8:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_MIC_64;
            break;
        case 16:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_MIC_128;
            break;
        default:
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    } else if (SecurityField < 8) {
        switch (SizeOfT) {
        case 0:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_ENC;
            break;
        case 4:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_ENC_MIC_32;
            break;
        case 8:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_ENC_MIC_64;
            break;
        case 16:
            *pSecurityLevel = CC3XX_CCM_STAR_SECURITY_LEVEL_ENC_MIC_128;
            break;
        default:
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return PSA_SUCCESS;
}

static psa_status_t ccm_init(AesCcmContext_t *context,
                             cryptoDirection_t encryptDecryptFlag,
                             size_t assocDataSize, size_t textDataSize,
                             const uint8_t *pNonce, uint8_t sizeOfN,
                             uint8_t sizeOfT, uint32_t ccmMode)
{
    uint8_t ctrStateBuf[CC_AES_BLOCK_SIZE_IN_BYTES] = {0};
    uint8_t qFieldSize = 15 - sizeOfN;
    uint8_t *tempBuff;
    drvError_t rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;
    uint8_t securityLevelField;
    uint8_t securityField;

    /* check the Encrypt / Decrypt flag validity */
    if (encryptDecryptFlag >= CRYPTO_DIRECTION_NUM_OF_ENC_MODES) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* check Nonce pointer */
    if (pNonce == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (ccmMode == CC3XX_CCM_MODE_CCM) {
        /* check CCM MAC size: [4,6,8,10,12,14,16] */
        if ((sizeOfT < 4) || (sizeOfT > 16) || ((sizeOfT & 1) != 0)) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    } else if (ccmMode == CC3XX_CCM_MODE_STAR) {
        securityField = pNonce[CC3XX_CCM_STAR_NONCE_SIZE_BYTES - 1];

        /* check CCM STAR MAC size */
        if (ccm_get_security_level(sizeOfT, securityField,
                                   &securityLevelField) != PSA_SUCCESS) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        /* check CCM STAR Nonce size. sizeOfN == 13 */
        if (sizeOfN != CC3XX_CCM_STAR_NONCE_SIZE_BYTES) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        /* check CCM STAR Security level field */
        if (pNonce[CC3XX_CCM_STAR_NONCE_SIZE_BYTES - 1] != securityLevelField) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* check the Q field size: according to our implementation qFieldSize <= 4*/
    if ((qFieldSize < 2) || (qFieldSize > 8)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* check, that actual size of TextData is not great, than its control field
     * size */
    if ((qFieldSize < 4) && ((textDataSize >> (qFieldSize * 8)) > 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* check Nonce size. */
    if (sizeOfN < 7 || sizeOfN >= CC_AES_BLOCK_SIZE_IN_BYTES) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* set current AES operation mode for AES_MAC operation on first block B0
     * and next blocks */
    context->mode = CIPHER_CBC_MAC;
    context->sizeOfN = sizeOfN;
    context->sizeOfT = sizeOfT;
    context->dir = encryptDecryptFlag;

    /* clear initial vector */
    CC_PalMemSetZero(context->ivBuf, sizeof(context->ivBuf));

    /*clear intenal buffers*/
    CC_PalMemSetZero(context->ctrStateBuf, sizeof(context->ctrStateBuf));
    CC_PalMemSetZero(context->tempBuff, sizeof(context->tempBuff));

    /* formatting the first block of CCM input B0, and encrypt with CBC-MAC */
    tempBuff = context->tempBuff;

    /* set Adata flag (bit 6 for big endian form) */
    if (assocDataSize > 0) {
        tempBuff[0] = 1 << 6;
    }

    /* next flag bits: (t-2)/2 concatenated with (q-1) */
    tempBuff[0] |= ((sizeOfT - 2) / 2) << 3; /* bits 3 - 5 */
    tempBuff[0] |= (qFieldSize - 1);         /* bits 0 - 2 */

    /* set N and Q (in big endian form) into B0 */
    CC_PalMemCopy(tempBuff + 1, pNonce, sizeOfN);
    CC_CommonReverseMemcpy(tempBuff + 16 - min(qFieldSize, 4),
                           (uint8_t *)&textDataSize, min(qFieldSize, 4));

    /* set data buffers structures */
    rc = SetDataBuffersInfo(tempBuff, CC_AES_BLOCK_SIZE_IN_BYTES, &inBuffInfo,
                            NULL, 0, &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_DATA_INVALID;
    }

    /* calculating MAC */
    rc = ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo,
                          CC_AES_BLOCK_SIZE_IN_BYTES);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("calculating MAC failed with error code %d\n", rc);
        return PSA_ERROR_DATA_INVALID;
    }

    /* set initial Counter value into ctrStateBuf buffer for AES CTR operations
     * on text data */

    /* Flags byte = (qFieldSize - 1) */
    ctrStateBuf[0] = qFieldSize - 1;
    /* copy Nonce into bytes 1...(15-qFieldSize) in big endian form */
    CC_PalMemCopy(ctrStateBuf + 1, pNonce, sizeOfN);
    /* set counter i = 1 (in byte 15) */
    ctrStateBuf[15] = 1; /* Note: value i = 0 reserved for encoding MAC value */
    CC_PalMemCopy((uint8_t *)context->ctrStateBuf, ctrStateBuf,
                  CC_AES_BLOCK_SIZE_IN_BYTES);

    return PSA_SUCCESS;
}

static psa_status_t ccm_ass_data(AesCcmContext_t *context,
                                 const uint8_t *pAssocData,
                                 size_t assocDataSize)
{
    uint32_t firstBlockRemSize = 0;
    uint8_t Asize = 0;
    drvError_t rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    if (pAssocData == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* formatting ASize and setting it into AES tempBuff buffer */
    /* clean working tempBuff */
    CC_PalMemSetZero(context->tempBuff, CC_AES_BLOCK_SIZE_IN_BYTES);

    /* set formatted ASize into temp buffer.
     Note: case ASize > 2^32 is not allowed by implementation */
    if (assocDataSize > 0xFFFFFFFF) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* if ASize > 2^16-2^8, then set 6 bytes = 0xff||0xfe||ASize */
    if (assocDataSize >=
        0xff00) { /* if ASize >= 2^16 - 2^8, then set 0xff||0xfe||ASize */
        context->tempBuff[0] = 0xff;
        context->tempBuff[1] = 0xfe;
        /* reverse copy 4 bytes */
        CC_CommonReverseMemcpy(context->tempBuff + 6 - 4,
                               (uint8_t *)&assocDataSize, 4);
        Asize = 6;
    } else if (assocDataSize > 0) { /* else if 0 < ASize < 2^16 - 2^8, then set
                                       2 bytes = ASize */
        /* reverse copy 2 bytes */
        CC_CommonReverseMemcpy(context->tempBuff + 2 - 2,
                               (uint8_t *)&assocDataSize, 2);
        Asize = 2;
    } else { /* no Adata */
        Asize = 0;
    }

    firstBlockRemSize = CC_AES_BLOCK_SIZE_IN_BYTES - Asize;

    if (assocDataSize < firstBlockRemSize) {
        firstBlockRemSize = assocDataSize;
    }

    CC_PalMemCopy(context->tempBuff + Asize, pAssocData, firstBlockRemSize);

    assocDataSize -= firstBlockRemSize;
    pAssocData += firstBlockRemSize;
    context->mode = CIPHER_CBC_MAC;

    /* set data buffers structures */
    rc = SetDataBuffersInfo(context->tempBuff, CC_AES_BLOCK_SIZE_IN_BYTES,
                            &inBuffInfo, NULL, 0, &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_DATA_INVALID;
    }

    /* encrypt a0 concatenated with the beginning of Associated data */
    rc = ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo,
                          CC_AES_BLOCK_SIZE_IN_BYTES);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("encrypt a0 concatenated with the beginning of "
                       "Associated data failed with error code %d\n",
                       rc);
        return PSA_ERROR_DATA_INVALID;
    }

    if (assocDataSize) {
        /* set data buffers structures */
        rc = SetDataBuffersInfo(pAssocData, assocDataSize, &inBuffInfo, NULL, 0,
                                &outBuffInfo);
        if (rc != 0) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_DATA_INVALID;
        }

        /* encrypt remaining Associated data */
        rc =
            ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo, assocDataSize);
        if (rc != AES_DRV_OK) {
            CC_PAL_LOG_ERR(
                "encrypt remaining Associated data failed with error code %d\n",
                rc);
            return PSA_ERROR_DATA_INVALID;
        }
    }

    return PSA_SUCCESS;
}

static psa_status_t ccm_text_data(AesCcmContext_t *context,
                                  const uint8_t *pTextDataIn,
                                  size_t textDataSize, uint8_t *pTextDataOut)
{
    drvError_t rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    /* if the users Data In pointer is illegal return an error */
    if ((pTextDataIn == NULL) && (textDataSize != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (textDataSize > 0xFFFFFFFF) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* if the users Data Out pointer is illegal return an error */
    if ((pTextDataOut == NULL) && (textDataSize != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* check overlapping of input-output buffers:
     * 1. in-placement operation is permitted, i.e. pTextDataIn = pTextDataOut
     * 2. If pTextDataIn > pTextDataOut, operation is valid since HW reads the
     *    block, perform operation and write the result to output which will not
     *    overwrite the next input operation block. And the tag/mac result is
     *    written to another temporary buffer see in ccm_finish()
     * 3. BUT,  pTextDataIn < pTextDataOut, operation is NOT valid since the
     *    output result will  overwrite the next input block, or expected tag/mac
     */
    if ((pTextDataIn < pTextDataOut) &&
        (pTextDataIn + textDataSize > pTextDataOut)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* set data buffers structures */
    rc = SetDataBuffersInfo(pTextDataIn, textDataSize, &inBuffInfo,
                            pTextDataOut, textDataSize, &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_DATA_INVALID;
    }

#if !defined(AES_NO_TUNNEL) && !defined(ARCH_IS_CC310)

    /* use HW tunnel to process CCMPE/PD on text data */

    if (context->dir == CRYPTO_DIRECTION_DECRYPT) {
        context->mode = CIPHER_CCMPD;
    } else {
        context->mode = CIPHER_CCMPE;
    }

    rc = ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo, textDataSize);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("HW tunnel processing CCMPE/PD on text data failed with "
                       "error code %d\n",
                       rc);
        return PSA_ERROR_DATA_INVALID;
    }

#else /* defined(AES_NO_TUNNEL) || defined(ARCH_IS_CC310) */

    /* invoke two separate operations in case of no tunneling is supported
     * by HW
     */

    if (context->dir == CRYPTO_DIRECTION_ENCRYPT) {
        /* set operation mode to CBC_MAC */
        context->mode = CIPHER_CBC_MAC;

        rc = ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo, textDataSize);
        if (rc != AES_DRV_OK) {
            CC_PAL_LOG_ERR("CBC_MAC on text data failed with error code %d\n",
                           rc);
            return PSA_ERROR_DATA_INVALID;
        }
    }

    /* Encrypt(decrypt) text data by AES-CTR, starting from CTR = CTR0+1 */
    context->mode = CIPHER_CTR;
    rc = ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo, textDataSize);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("AES-CTR on text data failed with error code %d\n", rc);
        return PSA_ERROR_DATA_INVALID;
    }

    if (context->dir == CRYPTO_DIRECTION_DECRYPT) {
        /* execute the AES-MAC on decrypt mode */
        context->mode = CIPHER_CBC_MAC;

        /* set data buffers structures */
        rc = SetDataBuffersInfo(pTextDataOut, textDataSize, &inBuffInfo, NULL,
                                0, &outBuffInfo);
        if (rc != 0) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_DATA_INVALID;
        }

        if (ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo,
                             textDataSize) != AES_DRV_OK) {
            CC_PAL_LOG_ERR(
                "AES-MAC on decrypt data failed with error code %d\n", rc);
            return PSA_ERROR_DATA_INVALID;
        }
    }

#endif /* defined(AES_NO_TUNNEL) || defined(ARCH_IS_CC310) */

    return PSA_SUCCESS;
}

static psa_status_t ccm_finish(AesCcmContext_t *context, unsigned char *macBuf,
                               size_t *sizeOfT)
{
    uint8_t *tempBuff;
    uint8_t qFieldSize;
    drvError_t rc;
    /* important to define different buffer for mac to support CCM with TextData
     * in-place and pTextDataIn > pTextDataOut overlapping addresses, see
     * comment in ccm_text_data()
     */
    uint8_t localMacBuf[CC_AES_BLOCK_SIZE_IN_BYTES];
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    if (sizeOfT == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* if the users Data In pointer is illegal return an error */
    if (macBuf == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    qFieldSize = 15 - context->sizeOfN;

    /* encrypt (decrypt) the CCM-MAC value */

    /* set operation to CTR mode */
    context->mode = CIPHER_CTR;

    /* set CTR value = CTR0 for encrypt(decrypt) CCM-MAC */
    CC_PalMemSetZero((uint8_t *)context->ctrStateBuf +
                         CC_AES_BLOCK_SIZE_IN_BYTES - qFieldSize,
                     qFieldSize);

    /* on CCM encrypt mode*/
    if (context->dir == CRYPTO_DIRECTION_ENCRYPT) {
        /* set data buffers structures */
        rc = SetDataBuffersInfo(
            (uint8_t *)context->ivBuf, CC_AES_BLOCK_SIZE_IN_BYTES, &inBuffInfo,
            localMacBuf, CC_AES_BLOCK_SIZE_IN_BYTES, &outBuffInfo);
        if (rc != 0) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_DATA_INVALID;
        }

        rc = ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo,
                              CC_AES_BLOCK_SIZE_IN_BYTES);
        if (rc != AES_DRV_OK) {
            CC_PAL_LOG_ERR("encryption of IV buf failed with error code %d\n",
                           rc);
            return PSA_ERROR_DATA_INVALID;
        }

        CC_PalMemCopy(macBuf, localMacBuf, context->sizeOfT);
    } else { /* on CCM decrypt mode*/
        tempBuff = context->tempBuff;
        CC_PalMemSetZero(tempBuff, sizeof(context->tempBuff));

        /* copy encrypted CCM-MAC from input to temp buff padded with zeros */
        CC_PalMemCopy(tempBuff, macBuf, context->sizeOfT);

        /* set data buffers structures */
        rc = SetDataBuffersInfo(tempBuff, CC_AES_BLOCK_SIZE_IN_BYTES,
                                &inBuffInfo, tempBuff,
                                CC_AES_BLOCK_SIZE_IN_BYTES, &outBuffInfo);
        if (rc != 0) {
            CC_PAL_LOG_ERR("illegal data buffers\n");
            return PSA_ERROR_DATA_INVALID;
        }

        /* decrypt the MAC value and save it in the context->temp buffer */
        rc = ProcessAesCcmDrv(context, &inBuffInfo, &outBuffInfo,
                              CC_AES_BLOCK_SIZE_IN_BYTES);
        if (rc != AES_DRV_OK) {
            CC_PAL_LOG_ERR("decryption of MAC buf failed with error code %d\n",
                           rc);
            return PSA_ERROR_DATA_INVALID;
        }
        /* compare calculated and decrypted MAC results */
        if (CC_PalMemCmp(context->ivBuf, tempBuff, context->sizeOfT) != 0) {
            /* if MAC results are different, return an Error */
            CC_PAL_LOG_ERR(
                "calculated and decrypted MAC results are different \n");
            return PSA_ERROR_INVALID_SIGNATURE;
        }
    }

    *sizeOfT = context->sizeOfT;

    return PSA_SUCCESS;
}

static psa_status_t
ccm_auth_crypt(const uint8_t *key_buffer, size_t key_buffer_size, size_t length,
               const unsigned char *iv, size_t iv_len, const unsigned char *add,
               size_t add_len, const unsigned char *input,
               unsigned char *output, size_t *output_length, unsigned char *tag,
               size_t tag_length, cryptoDirection_t dir, uint32_t ccmMode)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    AesCcmContext_t context;

    cc3xx_ccm_init(&context);

    status = ccm_setkey(&context, key_buffer, key_buffer_size * 8, dir);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    status = ccm_init(&context, dir, add_len, length, iv, iv_len, tag_length,
                      ccmMode);
    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* if there's no associated data, skip a0 encryption */
    if (add_len != 0) {
        status = ccm_ass_data(&context, add, add_len);
        if (status != PSA_SUCCESS) {
            goto cleanup;
        }
    }

    /* if there's no payload data, skip to authentication */
    if (length != 0) {
        status = ccm_text_data(&context, input, length, output);
        if (status != PSA_SUCCESS) {
            goto cleanup;
        }
    }

    status = ccm_finish(&context, tag, &tag_length);
    if (status != PSA_SUCCESS) {
        CC_PalMemSetZero(output, length);
    }

cleanup:
    cc3xx_ccm_free(&context);

    if (status == PSA_SUCCESS) {
        if (dir == CRYPTO_DIRECTION_ENCRYPT) {
            /* Ciphertext length is length of the given ciphertext with the tag
             * appended */
            *output_length = length + tag_length;
        } else {
            /* Plaintext length is simply the length of the given ciphertext */
            *output_length = length;
        }
    }

    return status;
}

/** \defgroup internal_ccm Internal CCM functions
 *
 *  Internal functions used by the driver to perform AEAD using AES-CCM mode
 *
 *  @{
 */
psa_status_t cc3xx_encrypt_ccm(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *plaintext,
    size_t plaintext_length, uint8_t *ciphertext, size_t ciphertext_size,
    size_t *ciphertext_length)
{
    uint8_t *tag = ciphertext + plaintext_length;

    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_bits_t key_bits = psa_get_key_bits(attributes);
    psa_algorithm_t key_alg = psa_get_key_algorithm(attributes);

    size_t tag_length = PSA_AEAD_TAG_LENGTH(key_type, key_bits, key_alg);

    /* FixMe: The PSA Crypto core layer should eventually perform this kind of
     *        checks, but for the moment, enforce them at driver level as well
     */
    if (ciphertext_size < tag_length + plaintext_length) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    uint32_t ccm_mode = CC3XX_CCM_MODE_CCM;

    return ccm_auth_crypt(key_buffer, key_buffer_size, plaintext_length, nonce,
                          nonce_length, additional_data, additional_data_length,
                          plaintext, ciphertext, ciphertext_length, tag,
                          tag_length, CRYPTO_DIRECTION_ENCRYPT, ccm_mode);
}

psa_status_t cc3xx_decrypt_ccm(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *nonce,
    size_t nonce_length, const uint8_t *additional_data,
    size_t additional_data_length, const uint8_t *ciphertext,
    size_t ciphertext_length, uint8_t *plaintext, size_t plaintext_size,
    size_t *plaintext_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    psa_key_type_t key_type = psa_get_key_type(attributes);
    psa_key_bits_t key_bits = psa_get_key_bits(attributes);
    psa_algorithm_t key_alg = psa_get_key_algorithm(attributes);

    size_t tag_length = PSA_AEAD_TAG_LENGTH(key_type, key_bits, key_alg);

    size_t ciphertext_length_without_tag = ciphertext_length - tag_length;
    const uint8_t *tag = ciphertext + ciphertext_length_without_tag;

    /* FixMe: The PSA Crypto core layer should eventually perform this kind of
     *        checks, but for the moment, enforce them at driver level as well
     */
    if (plaintext_size < ciphertext_length_without_tag) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    uint8_t local_tag_buffer[PSA_AEAD_TAG_MAX_SIZE];
    CC_PalMemCopy(local_tag_buffer, tag, tag_length);

    uint32_t ccm_mode = CC3XX_CCM_MODE_CCM;

    status = ccm_auth_crypt(key_buffer, key_buffer_size,
                            ciphertext_length_without_tag, nonce, nonce_length,
                            additional_data, additional_data_length, ciphertext,
                            plaintext, plaintext_length, local_tag_buffer,
                            tag_length, CRYPTO_DIRECTION_DECRYPT, ccm_mode);

    return status;
}

void cc3xx_ccm_init(AesCcmContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return;
    }

    CC_PalMemSetZero(ctx, sizeof(AesCcmContext_t));
}

void cc3xx_ccm_free(AesCcmContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return;
    }

    CC_PalMemSetZero(ctx, sizeof(AesCcmContext_t));
}

psa_status_t cc3xx_ccm_setkey_enc(
        AesCcmContext_t *ctx,
        const uint8_t *key,
        size_t key_bits)
{
    return ccm_setkey(ctx, key, key_bits, CRYPTO_DIRECTION_ENCRYPT);
}

psa_status_t cc3xx_ccm_setkey_dec(
        AesCcmContext_t *ctx,
        const uint8_t *key,
        size_t key_bits)
{
    return ccm_setkey(ctx, key, key_bits, CRYPTO_DIRECTION_DECRYPT);
}

psa_status_t cc3xx_ccm_set_lengths(
        AesCcmContext_t *ctx,
        size_t aadSize,
        size_t dataSize)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ctx->aadSize = aadSize;
    ctx->dataSize = dataSize;

    return PSA_SUCCESS;
}

psa_status_t cc3xx_ccm_set_nonce(
        AesCcmContext_t *ctx,
        const uint8_t *pNonce,
        size_t sizeOfN,
        size_t sizeOfT,
        uint32_t ccmMode)
{
    uint8_t ctrStateBuf[CC_AES_BLOCK_SIZE_IN_BYTES] = {0};
    uint8_t qFieldSize = 15 - sizeOfN;
    uint8_t *tempBuff;
    drvError_t rc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;
    uint8_t securityLevelField;
    uint8_t securityField;

    if (NULL == ctx || NULL == pNonce) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (ccmMode == CC3XX_CCM_MODE_CCM) {
        if ((sizeOfT < 4) || (sizeOfT > 16) || ((sizeOfT & 1) != 0)) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    } else if (ccmMode == CC3XX_CCM_MODE_STAR) {
        securityField = pNonce[CC3XX_CCM_STAR_NONCE_SIZE_BYTES - 1];

        if (ccm_get_security_level(sizeOfT, securityField,
                                   &securityLevelField) != PSA_SUCCESS) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (sizeOfN != CC3XX_CCM_STAR_NONCE_SIZE_BYTES) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (pNonce[CC3XX_CCM_STAR_NONCE_SIZE_BYTES - 1] != securityLevelField) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ((qFieldSize < 2) || (qFieldSize > 8)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ((qFieldSize < 4) && ((ctx->dataSize >> (qFieldSize * 8)) > 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (sizeOfN < 7 || sizeOfN >= CC_AES_BLOCK_SIZE_IN_BYTES) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ctx->mode = CIPHER_CBC_MAC;
    ctx->sizeOfT = sizeOfT;
    ctx->sizeOfN = sizeOfN;

    tempBuff = ctx->tempBuff;

    if (ctx->aadSize > 0) {
        tempBuff[0] = 1 << 6;
    }

    tempBuff[0] |= ((sizeOfT - 2) / 2) << 3;
    tempBuff[0] |= (qFieldSize - 1);

    CC_PalMemCopy(tempBuff + 1, pNonce, sizeOfN);
    CC_CommonReverseMemcpy(tempBuff + 16 - min(qFieldSize, 4),
                           (uint8_t *)&ctx->dataSize, min(qFieldSize, 4));

    rc = SetDataBuffersInfo(tempBuff, CC_AES_BLOCK_SIZE_IN_BYTES, &inBuffInfo,
                            NULL, 0, &outBuffInfo);
    if (rc != 0) {
        CC_PAL_LOG_ERR("illegal data buffers\n");
        return PSA_ERROR_DATA_INVALID;
    }

    rc = ProcessAesCcmDrv(ctx, &inBuffInfo, &outBuffInfo,
                          CC_AES_BLOCK_SIZE_IN_BYTES);
    if (rc != AES_DRV_OK) {
        CC_PAL_LOG_ERR("calculating MAC failed with error code %d\n", rc);
        return PSA_ERROR_DATA_INVALID;
    }

    ctrStateBuf[0] = qFieldSize - 1;
    CC_PalMemCopy(ctrStateBuf + 1, pNonce, sizeOfN);
    ctrStateBuf[15] = 1;
    CC_PalMemCopy((uint8_t *)ctx->ctrStateBuf, ctrStateBuf,
                  CC_AES_BLOCK_SIZE_IN_BYTES);

    return PSA_SUCCESS;
}

psa_status_t cc3xx_ccm_update_ad(
        AesCcmContext_t *ctx,
        const uint8_t *add,
        size_t aadSize)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = ccm_ass_data(ctx, add, aadSize);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("ccm_ass_data failed: %d", ret);
        return ret;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_ccm_update(
        AesCcmContext_t *ctx,
        size_t dataSize,
        const uint8_t *input,
        uint8_t *output)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = ccm_text_data(ctx, input, dataSize, output);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("ccm_text_data failed: %d", ret);
        return ret;
    }

    return PSA_SUCCESS;
}

psa_status_t cc3xx_ccm_finish(
        AesCcmContext_t *ctx,
        uint8_t *macBuf,
        size_t sizeOfT,
        size_t *tag_len)
{
    psa_status_t ret = PSA_ERROR_CORRUPTION_DETECTED;

    *tag_len = 0;

    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    ret = ccm_finish(ctx, macBuf, &sizeOfT);
    if (ret != PSA_SUCCESS) {
        CC_PAL_LOG_ERR("ccm_finish failed: %d", ret);
        return ret;
    }

    *tag_len = sizeOfT;

    return PSA_SUCCESS;
}

/** @} */ // end of internal_ccm
