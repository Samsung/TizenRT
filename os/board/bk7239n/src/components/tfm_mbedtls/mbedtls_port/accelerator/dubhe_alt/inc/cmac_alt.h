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

#ifndef CMAC_ALT_H
#define CMAC_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_CMAC_ALT)
#include "dubhe_cmac.h"
#include "dubhe_sca.h"

/**< CMAC hardware accelerator failed. */
#define MBEDTLS_ERR_CMAC_HW_ACCEL_FAILED    (-0x007A)

#define MBEDTLS_CMAC_MAGIC          (0x434D4143U) /* CMAC */

#define MBEDTLS_CMAC_MODEL_KEY       0 /**< model key as root key */
#define MBEDTLS_CMAC_DEVICE_ROOT_KEY 1 /**< device root key as root key */

struct mbedtls_cmac_context_t {
    uint32_t magic;
    arm_ce_cmac_context_t *cmac;
#if defined( MBEDTLS_DES_C )
    /** The internal state of the CMAC algorithm.  */
    unsigned char state[MBEDTLS_CIPHER_BLKSIZE_MAX];

    /** Unprocessed data - either data that was not block aligned and is still
     *  pending processing, or the final block. */
    unsigned char unprocessed_block[MBEDTLS_CIPHER_BLKSIZE_MAX];

    /** The length of data pending processing. */
    size_t unprocessed_len;
#endif
};

#define mbedtls_cipher_cmac_starts                   dubhe_cmac_starts
#define mbedtls_cipher_cmac_update                   dubhe_cmac_update
#define mbedtls_cipher_cmac_finish                   dubhe_cmac_finish
#define mbedtls_cmac_free                            dubhe_cmac_free
#define mbedtls_cipher_cmac                          dubhe_cipher_cmac
#define mbedtls_cipher_cmac_starts_with_derived_key  dubhe_cmac_starts_with_derived_key
#define mbedtls_cipher_cmac_reset                    dubhe_cmac_reset
#define mbedtls_aes_cmac_prf_128                     dubhe_cmac_prf_128

/**
 * \brief               This function sets the CMAC key, and prepares to authenticate
 *                      the input data.
 *                      Must be called with an initialized cipher context.
 *
 * \param ctx           The cipher context used for the CMAC operation, initialized
 *                      as one of the following types: MBEDTLS_CIPHER_AES_128_ECB,
 *                      MBEDTLS_CIPHER_AES_192_ECB, MBEDTLS_CIPHER_AES_256_ECB or
 *                      MBEDTLS_CIPHER_SM4_128_ECB.
 * \param type          Key type. Valid options are MBEDTLS_CMAC_MODEL_KEY and MBEDTLS_CMAC_DEVICE_ROOT_KEY
 * \param ek1           Buffer hold the key1.
 * \param ek2           Buffer hold the key2.
 * \param ek3           Buffer hold the key3.
 * \param ek1bits       The size of key1 bits. Valid options are:
 *                      <ul><li>128 bits</li>
 *                      <li>256 bits</li></ul>
 * \return              \c 0 on success.
 * \return              A cipher-specific error code on failure.
 */
int dubhe_cmac_starts_with_derived_key( mbedtls_cipher_context_t *ctx,
                                        int key_type,
                                        const unsigned char *ek1,
                                        const unsigned char *ek2,
                                        const unsigned char *ek3,
                                        unsigned int ek1bits );

/**
 * \brief          This function releases and clears the specified cmac context.
 *
 * \param ctx      The CMAC context to clear.
 *                 If this is \c NULL, this function does nothing.
 *                 Otherwise, the context must have been at least initialized.
 */
void mbedtls_cmac_free( mbedtls_cmac_context_t *ctx );

#endif /*end MBEDTLS_CMAC_ALT */
#ifdef __cplusplus
}
#endif
#endif /* end CMAC_ALT_H */