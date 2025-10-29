// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_CRYPTO_H__
#define __HAL_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"
/**
 *  HASH scheme
 */
typedef enum _hal_hash_sch_t {
    HAL_HASH_SCH_INVALID = 0,
    HAL_HASH_SCH_SHA1    = 1,
    HAL_HASH_SCH_SHA224  = 2,
    HAL_HASH_SCH_SHA256  = 3,
    HAL_HASH_SCH_SHA384  = 4,
    HAL_HASH_SCH_SHA512  = 5,
    HAL_HASH_SCH_MD5     = 6,
    HAL_HASH_SCH_SM3     = 7,
} hal_hash_sch_t;

/**
 * Hash digest size
 */
typedef enum _hash_dgst_size_t {
    HAL_HASH_SHA1_DGST_SIZE   = 20,
    HAL_HASH_SHA224_DGST_SIZE = 28,
    HAL_HASH_SHA256_DGST_SIZE = 32,
    HAL_HASH_SHA384_DGST_SIZE = 48,
    HAL_HASH_SHA512_DGST_SIZE = 64,
    HAL_HASH_MD5_DGST_SIZE    = 16,
    HAL_HASH_SM3_DGST_SIZE    = 32,

    HAL_HASH_MAX_DGST_SIZE = 64,
} hash_dgst_size_t;

/**
 * Hash Crypto scheme
 */
typedef enum _hal_crypto_sch_t {
    HAL_CRYPTO_SCH_INVALID = 0,

    /* RSA sign/verify with pkcsv1.5 scheme */
    HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA1   = 1,
    HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA224 = 2,
    HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA256 = 3,
    HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA384 = 4,
    HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA512 = 5,
    HAL_CRYPTO_SCH_RSASSA_PKCS_V15_MD5    = 6,

    /**
     * RSA sign/verify with pkcs_pss_mgf scheme.
     *
     * NOTE: the hash scheme for the MGF mask generating function
     * is the same as the hash scheme for generating the digest of original
     * data. NOTE: the salt length is handled by the algorithm itself.
     */
    HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA1   = 10,
    HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA224 = 11,
    HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA256 = 12,
    HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA384 = 13,
    HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA512 = 14,

    /* ECDSA sign/verify scheme */
    HAL_CRYPTO_SCH_ECDSA_SHA1   = 101,
    HAL_CRYPTO_SCH_ECDSA_SHA224 = 102,
    HAL_CRYPTO_SCH_ECDSA_SHA256 = 103,
    HAL_CRYPTO_SCH_ECDSA_SHA384 = 104,
    HAL_CRYPTO_SCH_ECDSA_SHA512 = 105,

    /* AES cipher scheme */
    HAL_CRYPTO_CIPHER_AES_ECB = 800,
    HAL_CRYPTO_CIPHER_AES_CBC = 801,
    HAL_CRYPTO_CIPHER_AES_CTR = 802,

} hal_crypto_sch_t;

/**
 * Hash Crytpo Key Type
 */
typedef enum _hal_crypto_key_type_t {
    HAL_CRYPTO_KEY_INVALID = 0,

    /* The RSA key type */
    HAL_CRYPTO_KEY_RSA_1024 = 1,
    HAL_CRYPTO_KEY_RSA_2048 = 2,
    HAL_CRYPTO_KEY_RSA_4096 = 3,

    /* The EC key type, now only support NIST curve type */
    HAL_CRYPTO_KEY_ECP_NIST_192 = 20,
    HAL_CRYPTO_KEY_ECP_NIST_224 = 21,
    HAL_CRYPTO_KEY_ECP_NIST_256 = 22,
    HAL_CRYPTO_KEY_ECP_NIST_384 = 23,
    HAL_CRYPTO_KEY_ECP_NIST_521 = 24,

    /* The AES cipher key type */
    HAL_CRYPTO_KEY_AES_128  = 100,
    HAL_CRYPTO_KEY_AES_192  = 101,
    HAL_CRYPTO_KEY_AES_256  = 102,
    HAL_CRYPTO_KEY_AES_BLOB = 103,
} hal_crypto_key_type_t;

typedef struct _hal_crypto_key_t {
    hal_crypto_key_type_t key_type;
    union {
        struct _crypto_rsa_pubkey_t {
            uint8_t *n;
            size_t n_size;
            uint8_t *e;
            size_t e_size;
        } rsa_pubkey;
        struct _crypto_ec_pubkey_t {
            uint8_t *q;
            size_t q_size;
        } ec_pubkey;
        struct _crypto_aes_key_t {
            uint8_t *aes_key;
        } aes_key;
        /* key blob */
        struct _crypto_key_blob_t {
            uint8_t *key_blob;
            size_t key_blob_size;
        } key_blob;
    };
} hal_crypto_key_t;

#define HAL_CRYPTO_AES_BLOCK_SIZE (16)
#define HAL_CRYPTO_AES_IV_SIZE (HAL_CRYPTO_AES_BLOCK_SIZE)

#ifdef __cplusplus
}
#endif

#endif /* __HAL_CRYPTO_H__ */