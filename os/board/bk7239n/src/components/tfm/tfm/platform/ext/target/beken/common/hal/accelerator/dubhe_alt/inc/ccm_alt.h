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

#ifndef CCM_ALT_H
#define CCM_ALT_H

#ifdef __cplusplus
extern "C" {
#endif
#if defined(MBEDTLS_CCM_ALT)
#include "mbedtls/cipher.h"

#define MBEDTLS_ERR_CCM_OOM                 (-0x0011) /**< CCM Memory overflow. */
#define MBEDTLS_ERR_CCM_HW_ACCEL_FAILED     (-0x0011) /**< CCM hardware accelerator failed. */

#define MBEDTLS_CCM_DERIVED_MODEL_KEY       (0U) /**< model key as root key */
#define MBEDTLS_CCM_DERIVED_DEVICE_ROOT_KEY (1U) /**< device root key as root key */

struct arm_ce_ccm_ctx;
/**
 * \brief    The CCM context-type definition. The CCM context is passed
 *           to the APIs called.
 */
typedef struct {
    unsigned char MBEDTLS_PRIVATE(y)[16];    /*!< The Y working buffer */
    unsigned char MBEDTLS_PRIVATE(ctr)[16];  /*!< The counter buffer */
    mbedtls_cipher_context_t MBEDTLS_PRIVATE(cipher_ctx);    /*!< The cipher context used. */
    size_t MBEDTLS_PRIVATE(plaintext_len);   /*!< Total plaintext length */
    size_t MBEDTLS_PRIVATE(add_len);         /*!< Total authentication data length */
    size_t MBEDTLS_PRIVATE(tag_len);         /*!< Total tag length */
    size_t MBEDTLS_PRIVATE(processed);       /*!< Track how many bytes of input data
                                                  were processed (chunked input).
                                                  Used independently for both auth data
                                                  and plaintext/ciphertext.
                                                  This variable is set to zero after
                                                  auth data input is finished. */
    unsigned char MBEDTLS_PRIVATE(q);        /*!< The Q working value */
    unsigned char MBEDTLS_PRIVATE(mode);     /*!< The operation to perform:
                                              #MBEDTLS_CCM_ENCRYPT or
                                              #MBEDTLS_CCM_DECRYPT or
                                              #MBEDTLS_CCM_STAR_ENCRYPT or
                                              #MBEDTLS_CCM_STAR_DECRYPT. */
    int MBEDTLS_PRIVATE(state);              /*!< Working value holding context's
                                                  state. Used for chunked data
                                                  input */
    mbedtls_cipher_id_t MBEDTLS_PRIVATE(cipher);               /*!< The cipher id used. */
    struct arm_ce_ccm_ctx *MBEDTLS_PRIVATE(armccm);          /*!< The ARM CCM context used. */
} mbedtls_ccm_context;

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      Hold the CCM context.
 * \param ek1      Buffer hold the key1.
 * \param ek2      Buffer hold the key2.
 * \param ek3      Buffer hold the key3.
 * \param ek1bits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li> for both AES and SM4
 *                 <li>256 bits</li></ul> for AES only
 *
 * \return         \c 0 on success, others on failure.
 */
int mbedtls_ccm_set_derived_key( mbedtls_ccm_context *ctx,
                                 mbedtls_cipher_id_t cipher,
                                 int key_type,
                                 const unsigned char *ek1,
                                 const unsigned char *ek2,
                                 const unsigned char *ek3,
                                 unsigned int ek1bits );

#endif /* end MBEDTLS_CCM_ALT */
#ifdef __cplusplus
}
#endif

#endif
