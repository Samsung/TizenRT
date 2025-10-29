/**
* @if copyright_display
 *      Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
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

#ifndef AES_ALT_H
#define AES_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_AES_ALT)
#include "dubhe_sca.h"
typedef arm_ce_sca_context_t mbedtls_aes_context;


#define MBEDTLS_ERR_AES_INVALID_PARAM                -0x002F
#define MBEDTLS_ERR_AES_GENERIC                      -0x0030

#define MBEDTLS_AES_DERIVED_MODEL_KEY       0
#define MBEDTLS_AES_DERIVED_DEVICE_ROOT_KEY 1

int dubhe_aes_set_derived_key( mbedtls_aes_context *ctx,
                          int key_type,
                          int mode,
                          const unsigned char *ek1,
                          const unsigned char *ek2,
                          const unsigned char *ek3,
                          unsigned int ek1bits );

#define mbedtls_aes_init                dubhe_aes_init
#define mbedtls_aes_free                dubhe_aes_free
#define mbedtls_aes_setkey_enc          dubhe_aes_setkey_enc
#define mbedtls_aes_set_derived_key     dubhe_aes_set_derived_key
#define mbedtls_aes_setkey_dec          dubhe_aes_setkey_dec
#define mbedtls_aes_crypt_ecb           dubhe_aes_crypt_ecb
#define mbedtls_aes_crypt_cbc           dubhe_aes_crypt_cbc
#define mbedtls_aes_crypt_cfb128        dubhe_aes_crypt_cfb128
#define mbedtls_aes_crypt_cfb8          dubhe_aes_crypt_cfb8
#define mbedtls_aes_crypt_ctr           dubhe_aes_crypt_ctr
#define mbedtls_aes_crypt_ofb           dubhe_aes_crypt_ofb
#define mbedtls_internal_aes_encrypt    dubhe_internal_aes_encrypt
#define mbedtls_internal_aes_decrypt    dubhe_internal_aes_decrypt

#endif /*end MBEDTLS_AES_ALT */
#ifdef __cplusplus
}
#endif
#endif /* end AES_ALT_H */