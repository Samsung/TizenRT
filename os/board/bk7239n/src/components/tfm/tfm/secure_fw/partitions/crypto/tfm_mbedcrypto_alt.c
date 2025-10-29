/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This file collects the alternative functions to replace the
 * implementations in mbed-crypto if the corresponding mbed-crypto
 * MBEDTLS__FUNCTION_NAME__ALT is selected.
 */

/*
 * A dummy include. Just add a dependency to make sure this file is compiled
 * after all crypto header files are installed and configuration flags are set.
 */
#include "tfm_mbedcrypto_include.h"
#if defined(MBEDTLS_AES_DECRYPT_ALT) || defined(MBEDTLS_AES_SETKEY_DEC_ALT)
#include "mbedtls/aes.h"
#include "mbedtls/error.h"
#endif

#if defined(MBEDTLS_AES_DECRYPT_ALT) && defined(MBEDTLS_CCM_C)
#pragma message("mbedtls_internal_aes_decrypt() is replaced by an empty wrapper to decrease memory footprint")
/*
 * Replace the decryption process with an empty wrapper in AES-CCM mode.
 * The decryption process is exactly the same as encryption process. Skip
 * the decryption implementation to decrease memory footprint.
 */
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    (void)ctx;
    (void)input;
    (void)output;

    return MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
}
#endif

#if defined(MBEDTLS_AES_SETKEY_DEC_ALT) && defined(MBEDTLS_CCM_C)
#pragma message("mbedtls_aes_setkey_dec() is replaced by an empty wrapper to decrease memory footprint")
/*
 * Replace the decryption process with an empty wrapper in AES-CCM mode.
 * The decryption process is exactly the same as encryption process. Skip
 * the decryption key setting to decrease memory footprint.
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    (void)ctx;
    (void)key;
    (void)keybits;

    return MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
}
#endif
