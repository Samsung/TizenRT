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
#include "mbedtls/ecdsa.h"
#include "hal_config.h"

#if (CAL_ENABLE_ECDSA)

#define _IS_VALID_ECDSA_SCHEME(sch)                                            \
    ((sch == HAL_CRYPTO_SCH_ECDSA_SHA1) ||                                     \
     (sch == HAL_CRYPTO_SCH_ECDSA_SHA224) ||                                   \
     (sch == HAL_CRYPTO_SCH_ECDSA_SHA256))

#define _ECDSA_REQUIRED_DGST_SIZE(sch)                                         \
    ((sch == HAL_CRYPTO_SCH_ECDSA_SHA1)                                        \
         ? (HAL_HASH_SHA1_DGST_SIZE)                                           \
         : ((sch == HAL_CRYPTO_SCH_ECDSA_SHA224)                               \
                ? (HAL_HASH_SHA224_DGST_SIZE)                                  \
                : ((sch == HAL_CRYPTO_SCH_ECDSA_SHA256)                        \
                       ? (HAL_HASH_SHA256_DGST_SIZE)                           \
                                       : (0))))

#define _IS_VALID_EC_NIST_PUBKEY(keyp)                                         \
    (((keyp->key_type == HAL_CRYPTO_KEY_ECP_NIST_256) ||                       \
      (keyp->key_type == HAL_CRYPTO_KEY_ECP_NIST_521)) &&                      \
      ((keyp->ec_pubkey.q) && (keyp->ec_pubkey.q_size)))

#define _ECDSA_KEY_TYPE_TO_MBEDTLS_GROUP_ID(key_type)                          \
    ((key_type == HAL_CRYPTO_KEY_ECP_NIST_256)                                 \
            ? (MBEDTLS_ECP_DP_SECP256R1)                                       \
            : ((key_type == HAL_CRYPTO_KEY_ECP_NIST_521)                       \
                        ? (MBEDTLS_ECP_DP_SECP521R1)                           \
                         : (MBEDTLS_ECP_DP_NONE)))

hal_ret_t hal_crypto_ecdsa_verify_digest(hal_crypto_sch_t sch,
                                         hal_crypto_key_t *key,
                                         uint8_t *dgst,
                                         size_t dgst_size,
                                         uint8_t *sig_r,
                                         size_t sig_r_size,
                                         uint8_t *sig_s,
                                         size_t sig_s_size)
{
    hal_ret_t ret               = HAL_OK;
    uint8_t q_z                 = 1;
    size_t key_size             = 0;
    size_t sch_required_dgst_sz = 0;
    mbedtls_ecp_group_id grp_id;
    mbedtls_mpi sig_r_mpi;
    mbedtls_mpi sig_s_mpi;
    mbedtls_ecdsa_context ecdsa_ctx;

    /* init mbedtls context for safe free */
    mbedtls_ecdsa_init(&ecdsa_ctx);
    mbedtls_mpi_init(&sig_r_mpi);
    mbedtls_mpi_init(&sig_s_mpi);

    HAL_CHECK_CONDITION(key && dgst && sig_r && sig_s,
                        HAL_ERR_BAD_PARAM,
                        "Parameter is NULL!\n");

    HAL_CHECK_CONDITION(_IS_VALID_ECDSA_SCHEME(sch),
                        HAL_ERR_BAD_PARAM,
                        "Invalid scheme: 0x%x\n",
                        sch);
    HAL_CHECK_CONDITION(_IS_VALID_EC_NIST_PUBKEY(key),
                        HAL_ERR_BAD_PARAM,
                        "Invalid EC NIST Pubkey key!\n");

    sch_required_dgst_sz = _ECDSA_REQUIRED_DGST_SIZE(sch);
    PAL_ASSERT_MSG(sch_required_dgst_sz, "ecdsa required digest size is 0!\n");

    grp_id = _ECDSA_KEY_TYPE_TO_MBEDTLS_GROUP_ID(key->key_type);
    PAL_ASSERT_MSG(MBEDTLS_ECP_DP_NONE != grp_id, "get group id failed!\n");

    HAL_CHECK_CONDITION(sch_required_dgst_sz == dgst_size,
                        HAL_ERR_BAD_PARAM,
                        "dst_size not match with ecdsa scheme required size: "
                        "0x%x != 0x%x, sch: 0x%x\n",
                        sch_required_dgst_sz,
                        dgst_size,
                        sch);

    /* read sig r to bignumber */
    ret = mbedtls_mpi_read_binary(&sig_r_mpi, sig_r, sig_r_size);
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Read Signature R to mpi failed: -0x%x\n",
                        -ret);
    /* read sig s to bignumber */
    ret = mbedtls_mpi_read_binary(&sig_s_mpi, sig_s, sig_s_size);
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Read Signature S to mpi failed: -0x%x\n",
                        -ret);

    /* load group */
    ret = mbedtls_ecp_group_load(&(ecdsa_ctx.grp), grp_id);
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Load Group 0x%x failed: -0x%x\n",
                        grp_id,
                        -ret);

    /* read pubeky to ecp point Q */
    key_size = (ecdsa_ctx.grp.pbits + 7) / 8;
    HAL_CHECK_CONDITION(key->ec_pubkey.q_size == (key_size * 2 + 1),
                        HAL_ERR_BAD_PARAM,
                        "Invalid EC key size: 0x%x\n",
                        key->ec_pubkey.q_size);
    HAL_CHECK_CONDITION(key->ec_pubkey.q[0] == 0x04,
                        HAL_ERR_BAD_PARAM,
                        "Invalid EC key\n");

    ret = mbedtls_mpi_read_binary(&(ecdsa_ctx.Q.X), key->ec_pubkey.q + 1, key_size);
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Call mbedtls_mpi_read_binary failed: -0x%x\n",
                        -ret);

    ret = mbedtls_mpi_read_binary(&(ecdsa_ctx.Q.Y), key->ec_pubkey.q + 1 + key_size, key_size);
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Call mbedtls_mpi_read_binary failed: -0x%x\n",
                        -ret);

    ret = mbedtls_mpi_read_binary(&(ecdsa_ctx.Q.Z), &q_z, 1);
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Call mbedtls_mpi_read_binary failed: -0x%x\n",
                        -ret);

    /* check ec pubkey */
    ret = mbedtls_ecp_check_pubkey(&(ecdsa_ctx.grp), &(ecdsa_ctx.Q));
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_INTERNAL_FATAL,
                        "Check EC Pubkey failed: -0x%x\n",
                        -ret);

    /* do the ecdsa verify */

    ret = mbedtls_ecdsa_verify(&(ecdsa_ctx.grp),
                               dgst,
                               dgst_size,
                               &(ecdsa_ctx.Q),
                               &sig_r_mpi,
                               &sig_s_mpi);
    HAL_CHECK_CONDITION(0 == ret,
                        HAL_ERR_VRFY_FAILED,
                        "Call mbedtls_ecdsa_verify failed: -0x%x\n",
                        -ret);

    ret = HAL_OK;
finish:
    mbedtls_mpi_free(&sig_r_mpi);
    mbedtls_mpi_free(&sig_s_mpi);
    mbedtls_ecdsa_free(&ecdsa_ctx);
    return ret;
}

#endif /* CAL_ENABLE_ECDSA */

