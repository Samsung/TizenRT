/**
* @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
* @endif
 */

#ifndef GCM_ALT_H
#define GCM_ALT_H

#ifdef __cplusplus
extern "C" {
#endif
#if defined(MBEDTLS_GCM_ALT)
#include "mbedtls/cipher.h"

#define MBEDTLS_ERR_GCM_OOM                 (-0x0011) /**< GCM Memory overflow. */
#define MBEDTLS_ERR_GCM_HW_ACCEL_FAILED     (-0x0013) /**< GCM hardware accelerator failed. */

#define MBEDTLS_GCM_DERIVED_MODEL_KEY       (0U) /**< model key as root key */
#define MBEDTLS_GCM_DERIVED_DEVICE_ROOT_KEY (1U) /**< device root key as root key */

struct arm_ce_gcm_ctx;
/**
 * \brief    The GCM context-type definition. The GCM context is passed
 *           to the APIs called.
 */
typedef struct {
    mbedtls_cipher_id_t cipher;             /*!< The cipher id used. */
    mbedtls_cipher_context_t cipher_ctx;    /*!< The cipher used. */
    uint64_t HL[16];                        /*!< Precalculated HTable low. */
    uint64_t HH[16];                        /*!< Precalculated HTable high. */
    uint64_t len;                           /*!< The total length of the encrypted data. */
    uint64_t add_len;                       /*!< The total length of the additional data. */
    unsigned char base_ectr[16];            /*!< The first ECTR for tag. */
    unsigned char y[16];                    /*!< The Y working value. */
    unsigned char buf[16];                  /*!< The buf working value. */
    int mode;                               /*!< The operation to perform:
                                               #MBEDTLS_GCM_ENCRYPT or
                                               #MBEDTLS_GCM_DECRYPT. */
    struct arm_ce_gcm_ctx *armgcm;          /*!< The ARM GCM context used. */
} mbedtls_gcm_context;

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      Hold the GCM context.
 * \param ek1      Buffer hold the key1.
 * \param ek2      Buffer hold the key2.
 * \param ek3      Buffer hold the key3.
 * \param ek1bits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li> for both AES and SM4
 *                 <li>256 bits</li></ul> for AES only
 *
 * \return         \c 0 on success, others on failure.
 */
int mbedtls_gcm_set_derived_key( mbedtls_gcm_context *ctx,
                                 mbedtls_cipher_id_t cipher,
                                 int key_type,
                                 const unsigned char *ek1,
                                 const unsigned char *ek2,
                                 const unsigned char *ek3,
                                 unsigned int ek1bits );
#endif /* end MBEDTLS_GCM_ALT */
#ifdef __cplusplus
}
#endif
#endif/* END GCM_ALT_H */
