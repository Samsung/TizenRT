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
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#include "hal.h"
#include "pal.h"
#include "hal_src_internal.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sm3.h"
#include "hal_config.h"

#define _HASH_CONTEXT_MAGIC (0x12345678)

typedef struct _hash_context_t {
    hal_hash_sch_t dgst_sch;
    uint32_t magic;
    uint8_t extra_ctx[];
} hash_context_t;

#define _IS_VALID_DGST_SCH(sch)                                                \
    ((sch == HAL_HASH_SCH_SHA1) || (sch == HAL_HASH_SCH_SHA224) ||             \
     (sch == HAL_HASH_SCH_SHA256) || (sch == HAL_HASH_SCH_SM3))

#define _GET_DGST_SIZE(sch)                                                    \
    ((sch == HAL_HASH_SCH_SHA1)                                                \
         ? (HAL_HASH_SHA1_DGST_SIZE)                                           \
         : ((sch == HAL_HASH_SCH_SHA224)                                       \
                ? (HAL_HASH_SHA224_DGST_SIZE)                                  \
                : ((sch == HAL_HASH_SCH_SHA256)                                \
                       ? (HAL_HASH_SHA256_DGST_SIZE)                           \
                       : ((sch == HAL_HASH_SCH_SM3)                            \
                              ? (HAL_HASH_SM3_DGST_SIZE)  \
                                        : (0)))))

#if (CAL_ENABLE_HASH)
hal_ret_t hal_crypto_hash(hal_hash_sch_t dgst_sch,
                          uint8_t *data,
                          size_t data_size,
                          uint8_t *dgst,
                          size_t *dgst_size)
{
    hal_ret_t ret    = HAL_OK;
    size_t hash_size = 0;

    HAL_CHECK_CONDITION(data && dgst && dgst_size, HAL_ERR_BAD_PARAM,
                        "Parameter is NULL!\n");

    HAL_CHECK_CONDITION(_IS_VALID_DGST_SCH(dgst_sch),
                        HAL_ERR_BAD_PARAM,
                        "Invalid hash scheme: 0x%x\n",
                        dgst_sch);
    hash_size = _GET_DGST_SIZE(dgst_sch);
    PAL_ASSERT_MSG(hash_size, "get hash size 0!\n");

    if (*dgst_size < hash_size) {
        PAL_LOG_ERR("dgst buffer too small: 0x%x < 0x%x\n", *dgst_size,
                    hash_size);
        *dgst_size = hash_size;
        return HAL_ERR_BAD_PARAM;
    }

    switch (dgst_sch) {
#if defined(CAL_ENABLE_SHA1)
    case HAL_HASH_SCH_SHA1:
        ret = mbedtls_sha1_ret(data, data_size, dgst);
        break;
#endif /* CAL_ENABLE_SHA1 */
#if defined(CAL_ENABLE_SHA224)
    case HAL_HASH_SCH_SHA224:
        ret = mbedtls_sha256(data, data_size, dgst, 1);
        break;
#endif /* CAL_ENABLE_SHA224 */
#if defined(CAL_ENABLE_SHA256)
    case HAL_HASH_SCH_SHA256:
        ret = mbedtls_sha256(data, data_size, dgst, 0);
        break;
#endif /* CAL_ENABLE_SHA256 */
#if defined(CAL_ENABLE_SM3)
    case HAL_HASH_SCH_SM3:
        ret = mbedtls_sm3_ret(data, data_size, dgst);
        break;
#endif /* CAL_ENABLE_SM3 */
    default:
        ret = HAL_ERR_NOT_SUPPORT;
        PAL_LOG_ERR("The dgst scheme %d not supported: 0x%x\n", dgst_sch, ret);
        goto finish;
    }
    if (0 == ret) {
        *dgst_size = hash_size;
        ret        = HAL_OK;
    } else {
        PAL_LOG_ERR("mbedtls_ret with sch %d failed: -0x%x\n", dgst_sch, -ret);
        ret = HAL_ERR_INTERNAL_FATAL;
    }

finish:
    return ret;
}

hal_ret_t hal_crypto_hash_init(hal_hash_sch_t dgst_sch, hal_hash_ctx_t *ctx)
{
    hal_ret_t ret           = HAL_OK;
    size_t extra_ctx_sz     = 0;
    hash_context_t *tmp_ctx = NULL;

    HAL_CHECK_CONDITION(ctx, HAL_ERR_BAD_PARAM, "ctx pointer is NULL\n");
    HAL_CHECK_CONDITION(_IS_VALID_DGST_SCH(dgst_sch),
                        HAL_ERR_BAD_PARAM,
                        "Invalid hash scheme: 0x%x\n",
                        dgst_sch);

    /* get the extra context size */
    switch (dgst_sch) {
#if defined(CAL_ENABLE_SHA1)
    case HAL_HASH_SCH_SHA1:
        extra_ctx_sz = sizeof(mbedtls_sha1_context);
        break;
#endif /* CAL_ENABLE_SHA1 */
#if (defined(CAL_ENABLE_SHA224) || defined(CAL_ENABLE_SHA256))
    case HAL_HASH_SCH_SHA224:
    case HAL_HASH_SCH_SHA256:
        extra_ctx_sz = sizeof(mbedtls_sha256_context);
        break;
#endif /* CAL_ENABLE_SHA224 || CAL_ENABLE_SHA256 */
#if defined(CAL_ENABLE_SM3)
    case HAL_HASH_SCH_SM3:
        extra_ctx_sz = sizeof(mbedtls_sm3_context);
        break;
#endif /* CAL_ENABLE_SM3 */
    default:
        extra_ctx_sz = 0;
        break;
    }

    HAL_CHECK_CONDITION(extra_ctx_sz,
                        HAL_ERR_NOT_SUPPORT,
                        "Hash scheme: 0x%x not support!\n",
                        dgst_sch);

    tmp_ctx = pal_malloc(extra_ctx_sz + sizeof(hash_context_t));
    HAL_CHECK_CONDITION(tmp_ctx,
                        HAL_ERR_OUT_OF_MEMORY,
                        "Malloc %d out of memory!\n",
                        extra_ctx_sz);

    switch (dgst_sch) {
#if defined(CAL_ENABLE_SHA1)
    case HAL_HASH_SCH_SHA1:
        mbedtls_sha1_init((mbedtls_sha1_context *)(tmp_ctx->extra_ctx));
        ret = mbedtls_sha1_starts_ret(
            (mbedtls_sha1_context *)(tmp_ctx->extra_ctx));
        break;
#endif /* CAL_ENABLE_SHA1 */
#if defined(CAL_ENABLE_SHA224)
    case HAL_HASH_SCH_SHA224:
        mbedtls_sha256_init((mbedtls_sha256_context *)(tmp_ctx->extra_ctx));
        ret = mbedtls_sha256_starts_ret(
            (mbedtls_sha256_context *)(tmp_ctx->extra_ctx), 1);
        break;
#endif /* CAL_ENABLE_SHA224 */
#if defined(CAL_ENABLE_SHA256)
    case HAL_HASH_SCH_SHA256:
        mbedtls_sha256_init((mbedtls_sha256_context *)(tmp_ctx->extra_ctx));
        ret = mbedtls_sha256_starts_ret(
            (mbedtls_sha256_context *)(tmp_ctx->extra_ctx), 0);
        break;
#endif /* CAL_ENABLE_SHA256 */
#if defined(CAL_ENABLE_SM3)
    case HAL_HASH_SCH_SM3:
        mbedtls_sm3_init((mbedtls_sm3_context *)(tmp_ctx->extra_ctx));
        ret =
            mbedtls_sm3_starts_ret((mbedtls_sm3_context *)(tmp_ctx->extra_ctx));
        break;
#endif /* CAL_ENABLE_SM3 */
    default:
        ret = HAL_ERR_NOT_SUPPORT;
        break;
    }
    if (0 == ret) {
        tmp_ctx->dgst_sch = dgst_sch;
        tmp_ctx->magic    = _HASH_CONTEXT_MAGIC;
        *ctx              = (hal_hash_ctx_t)(tmp_ctx);
        ret               = HAL_OK;
    } else {
        PAL_LOG_ERR("mbedtls_init with sch %d failed: -0x%x\n", dgst_sch, -ret);
        ret = HAL_ERR_INTERNAL_FATAL;
    }

finish:
    if ((HAL_OK != ret) && (tmp_ctx)) {
        pal_free(tmp_ctx);
    }
    return ret;
}
hal_ret_t hal_crypto_hash_update(hal_hash_ctx_t ctx,
                                 uint8_t *data,
                                 size_t data_size)
{
    hal_ret_t ret           = HAL_OK;
    hash_context_t *tmp_ctx = NULL;

    HAL_CHECK_CONDITION(ctx && data, HAL_ERR_BAD_PARAM, "Parameter is NULL!\n");

    tmp_ctx = (hash_context_t *)(ctx);
    HAL_CHECK_CONDITION(tmp_ctx->magic == _HASH_CONTEXT_MAGIC,
                        HAL_ERR_BAD_PARAM,
                        "Invalid context magic: 0x%x!\n",
                        tmp_ctx->magic);

    switch (tmp_ctx->dgst_sch) {
#if defined(CAL_ENABLE_SHA1)
    case HAL_HASH_SCH_SHA1:
        ret = mbedtls_sha1_update_ret(
            (mbedtls_sha1_context *)(tmp_ctx->extra_ctx), data, data_size);
        break;
#endif /* CAL_ENABLE_SHA1 */
#if (defined(CAL_ENABLE_SHA224) || defined(CAL_ENABLE_SHA256))
    case HAL_HASH_SCH_SHA224:
    case HAL_HASH_SCH_SHA256:
        ret = mbedtls_sha256_update_ret(
            (mbedtls_sha256_context *)(tmp_ctx->extra_ctx), data, data_size);
        break;
#endif /* CAL_ENABLE_SHA224 || CAL_ENABLE_SHA256 */
#if defined(CAL_ENABLE_SM3)
    case HAL_HASH_SCH_SM3:
        ret = mbedtls_sm3_update_ret(
            (mbedtls_sm3_context *)(tmp_ctx->extra_ctx), data, data_size);
        break;
#endif /* CAL_ENABLE_SM3 */
    default:
        ret = HAL_ERR_NOT_SUPPORT;
        break;
    }
    if (0 == ret) {
        ret = HAL_OK;
    } else {
        PAL_LOG_ERR("mbedtls_update_ret with sch %d failed: -0x%x\n",
                    tmp_ctx->dgst_sch, -ret);
        ret = HAL_ERR_INTERNAL_FATAL;
    }
finish:
    return ret;
}

/* This function should can handle digst == NULL and dgst_size == NULL */
hal_ret_t hal_crypto_hash_final(hal_hash_ctx_t ctx,
                                uint8_t *dgst,
                                size_t *dgst_size)
{
    hal_ret_t ret                            = HAL_OK;
    size_t hash_size                         = 0;
    hash_context_t *tmp_ctx                  = NULL;
    uint8_t tmp_dgst[HAL_HASH_MAX_DGST_SIZE] = {0};
    uint8_t *p                               = NULL;

    HAL_CHECK_CONDITION(ctx, HAL_ERR_BAD_PARAM, "Parameter is NULL!\n");

    tmp_ctx = (hash_context_t *)(ctx);
    HAL_CHECK_CONDITION(tmp_ctx->magic == _HASH_CONTEXT_MAGIC,
                        HAL_ERR_BAD_PARAM,
                        "Invalid context magic: 0x%x!\n",
                        tmp_ctx->magic);

    if (dgst == NULL) {
        HAL_CHECK_CONDITION(dgst_size == NULL, HAL_ERR_BAD_PARAM,
                            "Bad parameter: dgst is NULL, dgst_size :0x%x\n",
                            dgst_size);
        /* clean context */
        p = tmp_dgst;
    } else {
        HAL_CHECK_CONDITION(dgst_size, HAL_ERR_BAD_PARAM,
                            "Bad parameter: dgst_size is NULL\n");
        p = dgst;
    }

    hash_size = _GET_DGST_SIZE(tmp_ctx->dgst_sch);
    PAL_ASSERT_MSG(hash_size, "get hash size 0!\n");

    if (dgst_size && (*dgst_size < hash_size)) {
        PAL_LOG_ERR("dgst buffer too small: 0x%x < 0x%x\n", *dgst_size,
                    hash_size);
        *dgst_size = hash_size;
        return HAL_ERR_BAD_PARAM;
    }

    switch (tmp_ctx->dgst_sch) {
#if defined(CAL_ENABLE_SHA1)
    case HAL_HASH_SCH_SHA1:
        ret = mbedtls_sha1_finish_ret(
            (mbedtls_sha1_context *)(tmp_ctx->extra_ctx), p);
        mbedtls_sha1_free((mbedtls_sha1_context *)(tmp_ctx->extra_ctx));
        break;
#endif /* CAL_ENABLE_SHA1 */
#if (defined(CAL_ENABLE_SHA224) || defined(CAL_ENABLE_SHA256))
    case HAL_HASH_SCH_SHA224:
    case HAL_HASH_SCH_SHA256:
        ret = mbedtls_sha256_finish_ret(
            (mbedtls_sha256_context *)(tmp_ctx->extra_ctx), p);
        mbedtls_sha256_free((mbedtls_sha256_context *)(tmp_ctx->extra_ctx));
        break;
#endif /* CAL_ENABLE_SHA224 || CAL_ENABLE_SHA256 */
#if defined(CAL_ENABLE_SM3)
    case HAL_HASH_SCH_SM3:
        ret = mbedtls_sm3_finish_ret(
            (mbedtls_sm3_context *)(tmp_ctx->extra_ctx), p);
        mbedtls_sm3_free((mbedtls_sm3_context *)(tmp_ctx->extra_ctx));
        break;
#endif /* CAL_ENABLE_SM3 */
    default:
        ret = HAL_ERR_NOT_SUPPORT;
        break;
    }
    if (0 == ret) {
        if (dgst_size) {
            *dgst_size = hash_size;
        }
        ret = HAL_OK;
    } else {
        PAL_LOG_ERR("mbedtls_finish_ret with sch %d failed: -0x%x\n",
                    tmp_ctx->dgst_sch, -ret);
        ret = HAL_ERR_INTERNAL_FATAL;
    }

    if ((tmp_ctx) && (tmp_ctx->magic == _HASH_CONTEXT_MAGIC)) {
        tmp_ctx->magic = 0;
        pal_free(tmp_ctx);
    }
finish:
    return ret;
}
#endif // CAL_ENABLE_HASH
// eof

