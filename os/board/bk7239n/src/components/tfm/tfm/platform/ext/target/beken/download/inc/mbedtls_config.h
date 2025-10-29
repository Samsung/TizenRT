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

#ifndef __MBEDTLS_CONFIG_H__
#define __MBEDTLS_CONFIG_H__


#define CONFIG_MBEDTLS_DEBUG
#define CONFIG_MBEDTLS_DEBUG_LEVEL 4

#undef CONFIG_MBEDTLS_TEST
/*********************************************/
/* System support */
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS
#define MBEDTLS_PLATFORM_EXIT_ALT
#define MBEDTLS_PLATFORM_PRINTF_ALT
#define MBEDTLS_PLATFORM_SNPRINTF_ALT
//#define MBEDTLS_MEMORY_BUFFER_ALLOC_C
//#define MBEDTLS_NO_PLATFORM_ENTROPY
//#define MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
//#define MBEDTLS_PLATFORM_PRINTF_ALT

#if defined(CONFIG_MBEDTLS_TEST)
#define MBEDTLS_SELF_TEST
#define MBEDTLS_DEBUG_C
#else
//#define MBEDTLS_ENTROPY_C
#endif

#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_MODE_CTR

/* mbed TLS modules */
#define MBEDTLS_AES_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CAMELLIA_C
#define MBEDTLS_CMAC_C
#define MBEDTLS_GCM_C
#define MBEDTLS_CCM_C
#define MBEDTLS_DES_C

#define MBEDTLS_CTR_DRBG_C
//#define MBEDTLS_ENTROPY_C
#define MBEDTLS_MD_C
//#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA256_C
//#define MBEDTLS_SHA512_C
#define MBEDTLS_MD5_C

#define MBEDTLS_BASE64_C

#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_C
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
#define MBEDTLS_ECP_NIST_OPTIM

#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_CIPHER_MODE_CTR
#define MBEDTLS_RSA_C
#define MBEDTLS_PK_RSA_ALT_SUPPORT
#define MBEDTLS_PK_PARSE_EC_EXTENDED

#define MBEDTLS_PKCS1_V15
#define MBEDTLS_PKCS1_V21
#define MBEDTLS_OID_C
//#define MBEDTLS_ECDH_C

#define MBEDTLS_CIPHER_PADDING_PKCS7
#define MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS
#define MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN
#define MBEDTLS_CIPHER_PADDING_ZEROS

#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_PEM_WRITE_C

#if defined(CONFIG_MBEDTLS_DEBUG)
#define MBEDTLS_ERROR_C
#define MBEDTLS_DEBUG_C
#define MBEDTLS_SSL_DEBUG_ALL
#define MBEDTLS_SSL_ALL_ALERT_MESSAGES
#endif

/* Save RAM at the expense of ROM */
#define MBEDTLS_AES_ROM_TABLES

/*
 * You should adjust this to the exact number of sources you're using: default
 * is the "platform_entropy_poll" source, but you may want to add other ones
 * Minimum is 2 for the entropy test suite.
 */
#define MBEDTLS_ENTROPY_MAX_SOURCES 2

/**
 *  remove the dependency of libc
 *  if not defined, the following error is generate:
 *
 * modules/mbedtls/library/bignum.c:1268: undefined reference to `__aeabi_uldivmod'
 **/
#define MBEDTLS_NO_UDBL_DIVISION

#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_C



/**
 * \def MBEDTLS_AES_ALT
 *
 * MBEDTLS__MODULE_NAME__ALT: Uncomment a macro to let mbed TLS use your
 * alternate core implementation of a symmetric crypto, an arithmetic or hash
 * module (e.g. platform specific assembly optimized implementations). Keep
 * in mind that the function prototypes should remain the same.
 *
 * This replaces the whole module. If you only want to replace one of the
 * functions, use one of the MBEDTLS__FUNCTION_NAME__ALT flags.
 *
 * Example: In case you uncomment MBEDTLS_AES_ALT, mbed TLS will no longer
 * provide the "struct mbedtls_aes_context" definition and omit the base
 * function declarations and implementations. "aes_alt.h" will be included from
 * "aes.h" to include the new function definitions.
 *
 * Uncomment a macro to enable alternate implementation of the corresponding
 * module.
 *
 * \warning   MD2, MD4, MD5, ARC4, DES and SHA-1 are considered weak and their
 *            use constitutes a security risk. If possible, we recommend
 *            avoiding dependencies on them, and considering stronger message
 *            digests and ciphers instead.
 *
 */

/*add config for Dubhe*/
#define MBEDTLS_SM2DSA_C
#define MBEDTLS_SM2KEP_C
#define MBEDTLS_SM2PKE_C
#define MBEDTLS_SM2DSA_DETERMINISTIC
#define MBEDTLS_ECP_DP_SM2P256V1_ENABLED

#if ( MBEDTLS_WITH_DUBHE )
//#define MBEDTLS_SM3_C
//#define MBEDTLS_SM4_C
#define MBEDTLS_AES_ALT
//#define MBEDTLS_SHA1_ALT
#define MBEDTLS_SHA256_ALT
#define MBEDTLS_GCM_ALT
#define MBEDTLS_CCM_ALT
#define MBEDTLS_CMAC_ALT
#endif

#define MBEDTLS_ENTROPY_FORCE_SHA256
#define MBEDTLS_CTR_DRBG_MAX_REQUEST             (10 * 1024) /**< Maximum number of requested bytes per call */

#endif /* __MBEDTLS_CONFIG_H__ */
