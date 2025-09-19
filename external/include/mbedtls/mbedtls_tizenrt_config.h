/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/**
 * \file mbedtls_tizenrt_config.h
 */

/**
 * \def MBEDTLS_KEY_EXCHANGE_ECDH_ANON_ENABLED
 *
 * Enable the ECDHE-ANON based ciphersuite modes in SSL / TLS.
 *
 * Requires: MBEDTLS_ECDH_C
 *
 *
 * This enables the following ciphersuites (if other requisites are
 * enabled as well):
 *      MBEDTLS_TLS_ECDH_ANON_WITH_AES_128_CBC_SHA256
 */
#if defined(MBEDTLS_OCF_PATCH)
#define MBEDTLS_KEY_EXCHANGE_ECDH_ANON_ENABLED
#endif

/**
 * \def MBEDTLS_X509_EXPANDED_SUBJECT_ALT_NAME_SUPPORT
 *
 * Enable parsing of all supported subtypes of the Subject Alternative Name
 * extension. When enabled, the subject_alt_names field of mbedtls_x509_crt
 * is defined as an mbedtls_x509_subject_alt_name_sequence, each element of
 * which can describe a different subtype of the GeneralName choice as defined
 * by the standard.
 *
 * Comment this macro to only support dNSName subtypes, and to define the
 * subject_alt_names field as an mbedtls_x509_sequence. Any other subtypes will
 * be ignored. This was the behavior in earlier versions.
 */
#if defined(MBEDTLS_OCF_PATCH)
#define MBEDTLS_X509_EXPANDED_SUBJECT_ALT_NAME_SUPPORT
#endif

/**
 * \def MBEDTLS_SSL_CLIENT_RPK
 *
 * Enable support for RFC 7250 Using Raw Public Keys in Transport Layer
 * Security (TLS) and Datagram Transport Layer Security (DTLS) (client-side)
 *
 * Comment this macro to disable support for RPK.
 */
#define MBEDTLS_SSL_CLIENT_RPK

/**
 * \def MBEDTLS_SSL_SERVER_RPK
 *
 * Enable support for RFC 7250 Using Raw Public Keys in Transport Layer
 * Security (TLS) and Datagram Transport Layer Security (DTLS) (server-side)
 *
 * Comment this macro to disable support for RPK.
 */
#define MBEDTLS_SSL_SERVER_RPK


/**
 * \def MBEDTLS_LIGHT_DEVICE
 *
 * Configuration for light devices.
 */

#define MBEDTLS_LIGHT_DEVICE
#if defined(MBEDTLS_LIGHT_DEVICE)

#undef MBEDTLS_CIPHER_PADDING_PKCS7
#undef MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS
#undef MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN
#undef MBEDTLS_CIPHER_PADDING_ZEROS

#undef MBEDTLS_ECP_DP_SECP192K1_ENABLED
#undef MBEDTLS_ECP_DP_SECP224K1_ENABLED
#undef MBEDTLS_ECP_DP_SECP256K1_ENABLED
#undef MBEDTLS_ECP_DP_BP384R1_ENABLED
#undef MBEDTLS_ECP_DP_BP512R1_ENABLED
#undef MBEDTLS_ECP_DP_CURVE25519_ENABLED

#undef MBEDTLS_PK_PARSE_EC_EXTENDED
#undef MBEDTLS_ERROR_STRERROR_DUMMY
#undef MBEDTLS_GENPRIME

// #undef MBEDTLS_FS_IO
#undef MBEDTLS_MEMORY_DEBUG
#undef MBEDTLS_HAVEGE_C

#undef MBEDTLS_PK_RSA_ALT_SUPPORT
#undef MBEDTLS_SSL_DEBUG_ALL
#undef MBEDTLS_SSL_EXTENDED_MASTER_SECRET

#undef MBEDTLS_SSL_ENCRYPT_THEN_MAC
#undef MBEDTLS_SSL_EXTENDED_MASTER_SECRET
#undef MBEDTLS_SSL_FALLBACK_SCSV
#undef MBEDTLS_SSL_CBC_RECORD_SPLITTING

#undef MBEDTLS_AESNI_C
#undef MBEDTLS_ARC4_C
#undef MBEDTLS_BLOWFISH_C
#undef MBEDTLS_CAMELLIA_C
#undef MBEDTLS_DEBUG_C
#undef MBEDTLS_RIPEMD160_C
//#undef MBEDTLS_SHA512_C
#undef MBEDTLS_PKCS12_C

#undef MBEDTLS_X509_CSR_PARSE_C
//#undef MBEDTLS_X509_CREATE_C
//#undef MBEDTLS_X509_CRT_WRITE_C

#undef MBEDTLS_XTEA_C

#endif							/* MBEDTLS_LIGHT_DEVICE */

#define MBEDTLS_ALLOW_PRIVATE_ACCESS

#if defined(CONFIG_SE)
#define MBEDTLS_ENABLE_HARDWARE_ALT

/**
 * \def MBEDTLS_ENTROPY_HARDWARE_ALT
 *
 * Uncomment this macro to let mbed TLS use your own implementation of a
 * hardware entropy collector.
 *
 * Your function must be called \c mbedtls_hardware_poll(), have the same
 * prototype as declared in entropy_poll.h, and accept NULL as first argument.
 *
 * Uncomment to use your own hardware entropy collector.
 */
#if defined(CONFIG_TLS_HW_RNG)
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#endif

#if defined(CONFIG_TLS_HW_DH_PARAM)
#define MBEDTLS_DHM_ALT
#endif

#if defined(CONFIG_TLS_HW_ECDH_PARAM)
#define MBEDTLS_ECDH_GEN_PUBLIC_ALT
#define MBEDTLS_ECDH_COMPUTE_SHARED_ALT
#endif

#if defined(CONFIG_TLS_HW_ECDSA_VERIFICATION)
#define MBEDTLS_PK_ECDSA_VERIFY_ALT
#endif

#if defined(CONFIG_TLS_HW_RSA_VERIFICATION)
#define MBEDTLS_PK_RSA_VERIFY_ALT
#undef MBEDTLS_PK_RSA_ALT_SUPPORT
#endif

#if defined(CONFIG_TLS_HW_RSA_ENC)
#define MBEDTLS_PK_RSA_ENCRYPT_ALT
#undef MBEDTLS_PK_RSA_ALT_SUPPORT
#endif

#if defined(CONFIG_TLS_HW_AES_ENC)
#define MBEDTLS_AES_ALT
#define MBEDTLS_AES_SETKEY_ENC_ALT
#define MBEDTLS_AES_SETKEY_DEC_ALT
#define MBEDTLS_AES_ENCRYPT_ALT
#define MBEDTLS_AES_DECRYPT_ALT

/* HW AES does not support XTS & OFB */
#if defined(MBEDTLS_CIPHER_MODE_XTS)
#undef MBEDTLS_CIPHER_MODE_XTS
#endif
#if defined(MBEDTLS_CIPHER_MODE_OFB)
#undef MBEDTLS_CIPHER_MODE_OFB
#endif
#endif /* CONFIG_TLS_HW_AES_ENC */

#endif /* CONFIG_SE */

#if defined(CONFIG_MBEDTLS_PKCS5_C)
/* MBEDTLS_PKCS5_C should be enabled to encrypt TASH password with PBKDF2 */
#define MBEDTLS_PKCS5_C
#endif /* CONFIG_MBEDTLS_PKCS5_C */
