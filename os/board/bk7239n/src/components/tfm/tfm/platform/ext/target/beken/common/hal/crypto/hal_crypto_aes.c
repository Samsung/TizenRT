/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "hal.h"
#include "pal.h"
#include "hal_src_internal.h"
#include "mbedtls/aes.h"
#include "hal_config.h"

#if defined(CAL_ENABLE_AES)
#define _IS_VALID_AES_SCHEME(sch)                                              \
    ((sch == HAL_CRYPTO_CIPHER_AES_ECB) ||                                     \
     (sch == HAL_CRYPTO_CIPHER_AES_CBC) || (sch == HAL_CRYPTO_CIPHER_AES_CTR))
#define _IS_AES_SCHEME_REQUIRED_IV(sch)                                        \
    ((sch == HAL_CRYPTO_CIPHER_AES_CBC) || (sch == HAL_CRYPTO_CIPHER_AES_CTR))

#define _IS_VALID_AES_KEY(keyp)                                                \
    ((((keyp->key_type == HAL_CRYPTO_KEY_AES_128) ||                           \
       (keyp->key_type == HAL_CRYPTO_KEY_AES_256)) &&                          \
      (NULL != keyp->aes_key.aes_key)) ||                                      \
     ((keyp->key_type == HAL_CRYPTO_KEY_AES_BLOB) &&                           \
      ((keyp->key_blob.key_blob) && (keyp->key_blob.key_blob_size))))

#define _GET_AES_KEY_SIZE_BIT(keyp)                                            \
    ((keyp->key_type == HAL_CRYPTO_KEY_AES_128)                                \
         ? (128)                                                               \
            : ((keyp->key_type == HAL_CRYPTO_KEY_AES_256) ? (256) : (0)))

// clang-format off
static uint8_t _g_ek2[] = {0x49, 0xC0, 0x68, 0x83, 0xF2, 0x98, 0x63, 0x7D,
                           0x8D, 0x2F, 0x1B, 0x3C, 0xF5, 0x5D, 0x1A, 0xC4};

static uint8_t _g_ek3[] = {0x35, 0x09, 0x32, 0x0F, 0xF9, 0x69, 0x91, 0x4F,
                           0x2F, 0x76, 0x9F, 0xBC, 0x1E, 0xF1, 0x41, 0x67};
#define _DUBHE_MAX_EK1_SIZE (32)

static hal_ret_t _set_sw_aes_key(mbedtls_aes_context *ctx,
                                 hal_crypto_sch_t sch,
                                 uint8_t *key,
                                 size_t key_bits,
                                 bool is_decrypt)
{
    hal_ret_t ret = HAL_OK;

    if ((is_decrypt) && (sch != HAL_CRYPTO_CIPHER_AES_CTR)) {
        ret = mbedtls_aes_setkey_dec(ctx, key, key_bits);
    } else {
        ret = mbedtls_aes_setkey_enc(ctx, key, key_bits);
    }
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_GENERIC,
                        "mbedtls_aes_set_key %s failed, ret= -0x%x!\n",
                        (is_decrypt) ? ("decrypt") : ("encrypt"),
                        -ret);
finish:
    return ret;
}

hal_ret_t hal_crypto_aes(hal_crypto_sch_t sch,
                         hal_crypto_key_t *key,
                         bool is_decrypt,
                         const uint8_t *iv,
                         const uint8_t *src,
                         size_t src_size,
                         uint8_t *dst,
                         size_t *dst_size)
{
    hal_ret_t ret                          = HAL_OK;
    mbedtls_aes_context aes_ctx            = {0};
    bool aes_ctx_initialized               = false;
    mbedtls_aes_sec_key_t seckey           = {0};
    uint8_t *ek1                           = NULL;
    uint32_t ek1_bits                      = 0;
    uint8_t tmp_iv[HAL_CRYPTO_AES_IV_SIZE] = {0};
    size_t org_src_size                    = 0;

    HAL_CHECK_CONDITION(key && src && dst && dst_size,
                        HAL_ERR_BAD_PARAM,
                        "Parameter is NULL!\n");

    HAL_CHECK_CONDITION(_IS_VALID_AES_SCHEME(sch),
                        HAL_ERR_BAD_PARAM,
                        "Invalid scheme: 0x%x\n",
                        sch);
    HAL_CHECK_CONDITION(
        _IS_VALID_AES_KEY(key), HAL_ERR_BAD_PARAM, "Invalid AES key!\n");
    HAL_CHECK_CONDITION(((_IS_AES_SCHEME_REQUIRED_IV(sch) && (iv)) ||
                         (!_IS_AES_SCHEME_REQUIRED_IV(sch))),
                        HAL_ERR_BAD_PARAM,
                        "IV is NULL!\n");

    if (_IS_AES_SCHEME_REQUIRED_IV(sch)) {
        pal_memcpy(tmp_iv, iv, HAL_CRYPTO_AES_IV_SIZE);
    }

    if (*dst_size < src_size) {
        PAL_LOG_ERR("dst_size too small: 0x%x < 0x%x\n", *dst_size, src_size);
        *dst_size = src_size;
        return HAL_ERR_BAD_PARAM;
    }

    mbedtls_aes_init(&aes_ctx);
    aes_ctx_initialized = true;

    org_src_size = src_size;

    /* handle key blob */
    if (key->key_type == HAL_CRYPTO_KEY_AES_BLOB) {
        /* Check key blob size, must be 128 or 256 */
        if (key->key_blob.key_blob_size == 128 / 8) {
            ek1      = key->key_blob.key_blob;
            ek1_bits = 128;
        } else if (key->key_blob.key_blob_size == 256 / 8) {
            ek1      = key->key_blob.key_blob;
            ek1_bits = 256;
        } else {
            HAL_CHECK_CONDITION(NULL,
                                HAL_ERR_BAD_PARAM,
                                "Bad Key blob size: %d !\n",
                                key->key_blob.key_blob_size);
        }

        /* call keyladder to derive key, use hardcode ek2 and ek3 */
        seckey.sel = MBEDTLS_AES_KL_KEY_MODEL;
        seckey.ek1bits = ek1_bits;
        pal_memcpy(seckey.ek1, ek1, ek1_bits / 8);
        pal_memcpy(seckey.ek2, _g_ek2, sizeof(_g_ek2));
        pal_memcpy(seckey.ek3, _g_ek3, sizeof(_g_ek3));
        if (is_decrypt) {
            ret = mbedtls_aes_setseckey_dec(&aes_ctx, &seckey);
        } else {
            ret = mbedtls_aes_setseckey_enc(&aes_ctx, &seckey);
        }
        HAL_CHECK_CONDITION(0 == ret,
                            HAL_ERR_GENERIC,
                            "mbedtls set secure key failed: ret= -0x%x!\n",
                            -ret);
    } else {
        ret = _set_sw_aes_key(&aes_ctx,
                              sch,
                              key->aes_key.aes_key,
                              _GET_AES_KEY_SIZE_BIT(key),
                              is_decrypt);
        HAL_CHECK_RET("Set software aes key failed!\n");
    }

    switch (sch) {
#if defined(CAL_ENABLE_CIPHER_MODE_ECB)
    case HAL_CRYPTO_CIPHER_AES_ECB: {
        int loop = 0;
        HAL_CHECK_CONDITION(
            _HAL_INTERNAL_IS_ALIGN(src_size, HAL_CRYPTO_AES_BLOCK_SIZE),
            HAL_ERR_BAD_PARAM,
            "src_size is not AES block size aligned: 0x%x\n",
            src_size);

        while (src_size) {
            ret = mbedtls_aes_crypt_ecb(&aes_ctx,
                                        (is_decrypt) ? (MBEDTLS_AES_DECRYPT)
                                                     : (MBEDTLS_AES_ENCRYPT),
                                        src + loop * HAL_CRYPTO_AES_BLOCK_SIZE,
                                        dst + loop * HAL_CRYPTO_AES_BLOCK_SIZE);
            HAL_CHECK_CONDITION(
                0 == ret,
                HAL_ERR_GENERIC,
                "mbedtls_aes_crypt_ecb %s failed, ret= -0x%x!\n",
                (is_decrypt) ? ("decrypt") : ("encrypt"),
                -ret);

            loop++;
            src_size -= HAL_CRYPTO_AES_BLOCK_SIZE;
        }
        break;
    }
#endif /* CAL_ENABLE_CIPHER_MODE_ECB */
#if defined(CAL_ENABLE_CIPHER_MODE_CTR)
    case HAL_CRYPTO_CIPHER_AES_CTR: {
        size_t offset                                         = 0;
        unsigned char stream_block[HAL_CRYPTO_AES_BLOCK_SIZE] = {0};

        ret = mbedtls_aes_crypt_ctr(
            &aes_ctx, src_size, &offset, tmp_iv, stream_block, src, dst);
        HAL_CHECK_CONDITION(0 == ret,
                            HAL_ERR_GENERIC,
                            "mbedtls_aes_crypt_ctr failed, ret= -0x%x!\n",
                            -ret);
        break;
    }
#endif /* CAL_ENABLE_CIPHER_MODE_CTR */
    default:
        ret = HAL_ERR_NOT_SUPPORT;
        goto finish;
        break;
    }

    /* update dst size */
    *dst_size = org_src_size;

finish:
    if (aes_ctx_initialized) {
        mbedtls_aes_free(&aes_ctx);
    }
    pal_memset(&seckey, 0, sizeof(seckey));
    pal_memset(tmp_iv, 0, sizeof(tmp_iv));
    return ret;
}
#endif /* CAL_ENABLE_AES */