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
#include "mbedtls/rsa.h"
#include "hal_config.h"

#if defined(CAL_ENABLE_RSA)

#define _IS_RSA_SCHEME_PKCS_V15(sch)                                           \
    ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA1) ||                           \
     (sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA224) ||                         \
     (sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA256))

#define _IS_RSA_SCHEME_PKCS_PSS(sch)                                           \
    ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA1) ||                      \
     (sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA224) ||                    \
     (sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA256))

#define _IS_VALID_RSA_SCHEME(sch)                                              \
    (_IS_RSA_SCHEME_PKCS_V15(sch) || _IS_RSA_SCHEME_PKCS_PSS(sch))

#define _RSA_REQUIRED_DGST_SIZE(sch)                                              \
    ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA1 ||                               \
      sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA1)                            \
         ? (HAL_HASH_SHA1_DGST_SIZE)                                              \
         : ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA224 ||                      \
             sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA224)                   \
                ? (HAL_HASH_SHA224_DGST_SIZE)                                     \
                : ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA256 ||               \
                    sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA256)            \
                       ? (HAL_HASH_SHA256_DGST_SIZE)                              \
                                : (0))))

#define _RSA_SCHEME_TO_MEBDTLS_HASH_ID(sch)                                       \
    ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA1 ||                               \
      sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA1)                            \
         ? (MBEDTLS_MD_SHA1)                                                      \
         : ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA224 ||                      \
             sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA224)                   \
                ? (MBEDTLS_MD_SHA224)                                             \
                : ((sch == HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA256 ||               \
                    sch == HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA256)            \
                       ? (MBEDTLS_MD_SHA256)                                      \
                       : (MBEDTLS_MD_NONE))))

#define _IS_VALID_RSA_PUBKEY(keyp)                                             \
    (((keyp->key_type == HAL_CRYPTO_KEY_RSA_1024) ||                           \
      (keyp->key_type == HAL_CRYPTO_KEY_RSA_2048) ||                           \
      (keyp->key_type == HAL_CRYPTO_KEY_RSA_4096)) &&                          \
     ((keyp->rsa_pubkey.n) && (keyp->rsa_pubkey.e) &&                          \
      (keyp->rsa_pubkey.n_size) && (keyp->rsa_pubkey.e_size)))

#define _GET_RSA_KEY_SIZE_BIT(keyp)                                            \
    ((keyp->key_type == HAL_CRYPTO_KEY_RSA_1024)                               \
         ? (1024)                                                              \
         : ((keyp->key_type == HAL_CRYPTO_KEY_RSA_2048)                        \
                ? (2048)                                                       \
                : ((keyp->key_type == HAL_CRYPTO_KEY_RSA_4096) ? (4096)        \
                                                               : (0))))

hal_ret_t hal_crypto_rsa_verify_digest(hal_crypto_sch_t sch,
                                       hal_crypto_key_t *key,
                                       uint8_t *dgst,
                                       size_t dgst_size,
                                       uint8_t *sig,
                                       size_t sig_size)
{
    hal_ret_t ret               = HAL_OK;
    int mbedtls_ret             = 0;
    size_t sch_required_dgst_sz = 0;
    size_t key_bits             = 0;
    mbedtls_rsa_context rsa;
    mbedtls_md_type_t hash_id;

    /* reset rsa context to 0 for safe free */
    pal_memset(&rsa, 0, sizeof(mbedtls_rsa_context));

    PAL_LOG_DEBUG("sch: 0x%x dgst_size: 0x%x, sig_size: 0x%x\n",
                  sch,
                  dgst_size,
                  sig_size);

    HAL_CHECK_CONDITION(
        key && dgst && sig, HAL_ERR_BAD_PARAM, "Parameter is NULL!\n");

    HAL_CHECK_CONDITION(_IS_VALID_RSA_SCHEME(sch),
                        HAL_ERR_BAD_PARAM,
                        "Invalid scheme: 0x%x\n",
                        sch);
    HAL_CHECK_CONDITION(_IS_VALID_RSA_PUBKEY(key),
                        HAL_ERR_BAD_PARAM,
                        "Invalid RSA Pubkey key!\n");

    sch_required_dgst_sz = _RSA_REQUIRED_DGST_SIZE(sch);
    PAL_ASSERT_MSG(sch_required_dgst_sz, "rsa required digest size is 0!\n");

    HAL_CHECK_CONDITION(sch_required_dgst_sz == dgst_size,
                        HAL_ERR_BAD_PARAM,
                        "dst_size not match with key type: 0x%x != 0x%x\n",
                        sch_required_dgst_sz,
                        dgst_size);

    key_bits = _GET_RSA_KEY_SIZE_BIT(key);
    PAL_ASSERT_MSG(key_bits, "rsa get key bits 0!\n");

    HAL_CHECK_CONDITION(
        sig_size == (key_bits >> 3),
        HAL_ERR_BAD_PARAM,
        "signature size not match with key size: 0x%x != 0x%x\n",
        (key_bits >> 3),
        sig_size);

    hash_id = _RSA_SCHEME_TO_MEBDTLS_HASH_ID(sch);
    PAL_ASSERT_MSG(MBEDTLS_MD_NONE != hash_id,
                   "rsa scheme to mbedtls hash id failed, sch: 0x%x!\n",
                   sch);

    /* start to call mbedtls do verify */
    /* init one rsa context */
    if (_IS_RSA_SCHEME_PKCS_V15(sch)) {
        //TODO wangzhilei mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);
        mbedtls_rsa_init(&rsa);
    } else if (_IS_RSA_SCHEME_PKCS_PSS(sch)) {
        //TODO wangzhileimbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V21, hash_id);
		mbedtls_rsa_init(&rsa);
    } else {
        ret = HAL_ERR_NOT_SUPPORT;
        PAL_LOG_ERR("Unsupported scheme: 0x%x\n", sch);
        goto finish;
    }

    /* inport rsa pubkey */
    mbedtls_ret = mbedtls_rsa_import_raw(&rsa,
                                         key->rsa_pubkey.n,
                                         key->rsa_pubkey.n_size,
                                         NULL,
                                         0, /* P */
                                         NULL,
                                         0, /* Q */
                                         NULL,
                                         0, /* D */
                                         key->rsa_pubkey.e,
                                         key->rsa_pubkey.e_size);
    HAL_CHECK_CONDITION(0 == mbedtls_ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Import RSA Pubkey failed!: -0x%x\n",
                        -mbedtls_ret);

    /* check the pubkey */
    mbedtls_ret = mbedtls_rsa_check_pubkey(&rsa);
    HAL_CHECK_CONDITION(0 == mbedtls_ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Check RSA Pubkey failed!: -0x%x\n",
                        -mbedtls_ret);

    /* do the pkcs1 verify */
    mbedtls_ret = mbedtls_rsa_pkcs1_verify(&rsa,
                                           hash_id,
                                           dgst_size,
                                           (const unsigned char *)dgst,
                                           sig);
    HAL_CHECK_CONDITION(0 == mbedtls_ret,
                        HAL_ERR_VRFY_FAILED,
                        "Call mbedtls_rsa_pkcs1_verify failed: -0x%x\n",
                        -mbedtls_ret);

    ret = HAL_OK;

finish:
    mbedtls_rsa_free(&rsa);
    return ret;
}

#endif /* CAL_ENABLE_RSA */
