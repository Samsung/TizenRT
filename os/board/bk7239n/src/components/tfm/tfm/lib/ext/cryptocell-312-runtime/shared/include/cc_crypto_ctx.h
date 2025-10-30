/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef _CC_CRYPTO_CTX_H_
#define _CC_CRYPTO_CTX_H_

#ifdef __KERNEL__
#include <linux/types.h>
#define INT32_MAX 0x7FFFFFFFL
#else
#include <stdint.h>
#endif


#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

/* context size */
#ifndef CC_CTX_SIZE_LOG2
#if (CC_SUPPORT_SHA > 256)
#define CC_CTX_SIZE_LOG2 8
#else
#define CC_CTX_SIZE_LOG2 7
#endif
#endif
#define CC_CTX_SIZE (1<<CC_CTX_SIZE_LOG2)
#define CC_DRV_CTX_SIZE_WORDS (CC_CTX_SIZE >> 2)

#define CC_DRV_DES_IV_SIZE 8
#define CC_DRV_DES_BLOCK_SIZE 8

#define CC_DRV_DES_ONE_KEY_SIZE 8
#define CC_DRV_DES_DOUBLE_KEY_SIZE 16
#define CC_DRV_DES_TRIPLE_KEY_SIZE 24
#define CC_DRV_DES_KEY_SIZE_MAX CC_DRV_DES_TRIPLE_KEY_SIZE

#define CC_AES_IV_SIZE 16
#define CC_AES_IV_SIZE_WORDS (CC_AES_IV_SIZE >> 2)

#define CC_AES_BLOCK_SIZE 16
#define CC_AES_BLOCK_SIZE_WORDS 4

#define CC_AES_128_BIT_KEY_SIZE 16
#define CC_AES_128_BIT_KEY_SIZE_WORDS   (CC_AES_128_BIT_KEY_SIZE >> 2)
#define CC_AES_192_BIT_KEY_SIZE 24
#define CC_AES_192_BIT_KEY_SIZE_WORDS   (CC_AES_192_BIT_KEY_SIZE >> 2)
#define CC_AES_256_BIT_KEY_SIZE 32
#define CC_AES_256_BIT_KEY_SIZE_WORDS   (CC_AES_256_BIT_KEY_SIZE >> 2)
#define CC_AES_KEY_SIZE_MAX         CC_AES_256_BIT_KEY_SIZE
#define CC_AES_KEY_SIZE_WORDS_MAX       (CC_AES_KEY_SIZE_MAX >> 2)

#define CC_MD5_DIGEST_SIZE  16
#define CC_SHA1_DIGEST_SIZE     20
#define CC_SHA224_DIGEST_SIZE   28
#define CC_SHA256_DIGEST_SIZE   32
#define CC_SHA256_DIGEST_SIZE_IN_WORDS 8
#define CC_SHA384_DIGEST_SIZE   48
#define CC_SHA512_DIGEST_SIZE   64

#define CC_SHA1_BLOCK_SIZE 64
#define CC_SHA1_BLOCK_SIZE_IN_WORDS 16
#define CC_MD5_BLOCK_SIZE 64
#define CC_MD5_BLOCK_SIZE_IN_WORDS 16
#define CC_SHA224_BLOCK_SIZE 64
#define CC_SHA256_BLOCK_SIZE 64
#define CC_SHA256_BLOCK_SIZE_IN_WORDS 16
#define CC_SHA1_224_256_BLOCK_SIZE 64
#define CC_SHA384_BLOCK_SIZE 128
#define CC_SHA512_BLOCK_SIZE 128

#if (CC_SUPPORT_SHA > 256)
#define CC_DIGEST_SIZE_MAX CC_SHA512_DIGEST_SIZE
#define CC_HASH_BLOCK_SIZE_MAX CC_SHA512_BLOCK_SIZE /*1024b*/
#define DRV_HASH_LENGTH_WORDS       4
#else /* Only up to SHA256 */
#define CC_DIGEST_SIZE_MAX CC_SHA256_DIGEST_SIZE
#define CC_HASH_BLOCK_SIZE_MAX CC_SHA256_BLOCK_SIZE /*512b*/
#define DRV_HASH_LENGTH_WORDS       2
#endif

#define CC_HMAC_BLOCK_SIZE_MAX CC_HASH_BLOCK_SIZE_MAX

#define CC_MULTI2_SYSTEM_KEY_SIZE       32
#define CC_MULTI2_DATA_KEY_SIZE         8
#define CC_MULTI2_SYSTEM_N_DATA_KEY_SIZE    (CC_MULTI2_SYSTEM_KEY_SIZE + CC_MULTI2_DATA_KEY_SIZE)
#define CC_MULTI2_BLOCK_SIZE                    8
#define CC_MULTI2_IV_SIZE                   8
#define CC_MULTI2_MIN_NUM_ROUNDS                8
#define CC_MULTI2_MAX_NUM_ROUNDS                128

#define CC_DRV_ALG_MAX_BLOCK_SIZE CC_HASH_BLOCK_SIZE_MAX

enum drv_engine_type {
    DRV_ENGINE_NULL = 0,
    DRV_ENGINE_AES = 1,
    DRV_ENGINE_DES = 2,
    DRV_ENGINE_HASH = 3,
    DRV_ENGINE_RC4 = 4,
    DRV_ENGINE_DOUT = 5,
    DRV_ENGINE_RESERVE32B = INT32_MAX,
};

enum drv_crypto_alg {
    DRV_CRYPTO_ALG_NULL = -1,
    DRV_CRYPTO_ALG_AES  = 0,
    DRV_CRYPTO_ALG_DES  = 1,
    DRV_CRYPTO_ALG_HASH = 2,
    DRV_CRYPTO_ALG_C2   = 3,
    DRV_CRYPTO_ALG_HMAC = 4,
    DRV_CRYPTO_ALG_AEAD = 5,
    DRV_CRYPTO_ALG_BYPASS = 6,
    DRV_CRYPTO_ALG_NUM = 7,
    DRV_CRYPTO_ALG_RESERVE32B = INT32_MAX
};

enum drv_crypto_direction {
    DRV_CRYPTO_DIRECTION_NULL = -1,
    DRV_CRYPTO_DIRECTION_ENCRYPT = 0,
    DRV_CRYPTO_DIRECTION_DECRYPT = 1,
    DRV_CRYPTO_DIRECTION_DECRYPT_ENCRYPT = 3,
    DRV_CRYPTO_DIRECTION_RESERVE32B = INT32_MAX
};

enum drv_cipher_mode {
    DRV_CIPHER_NULL_MODE = -1,
    DRV_CIPHER_ECB = 0,
    DRV_CIPHER_CBC = 1,
    DRV_CIPHER_CTR = 2,
    DRV_CIPHER_CBC_MAC = 3,
    DRV_CIPHER_XTS = 4,
    DRV_CIPHER_XCBC_MAC = 5,
    DRV_CIPHER_OFB = 6,
    DRV_CIPHER_CMAC = 7,
    DRV_CIPHER_CCM = 8,
    DRV_CIPHER_CBC_CTS = 11,
    DRV_CIPHER_GCTR = 12,
    DRV_CIPHER_ESSIV = 13,
    DRV_CIPHER_BITLOCKER = 14,
    DRV_CIPHER_RESERVE32B = INT32_MAX
};

enum drv_hash_mode {
    DRV_HASH_NULL = -1,
    DRV_HASH_SHA1 = 0,
    DRV_HASH_SHA256 = 1,
    DRV_HASH_SHA224 = 2,
    DRV_HASH_SHA512 = 3,
    DRV_HASH_SHA384 = 4,
    DRV_HASH_MD5 = 5,
    DRV_HASH_CBC_MAC = 6,
    DRV_HASH_XCBC_MAC = 7,
    DRV_HASH_CMAC = 8,
    DRV_HASH_MODE_NUM = 9,
    DRV_HASH_RESERVE32B = INT32_MAX
};

enum drv_hash_hw_mode {
    DRV_HASH_HW_MD5 = 0,
    DRV_HASH_HW_SHA1 = 1,
    DRV_HASH_HW_SHA256 = 2,
    DRV_HASH_HW_SHA224 = 10,
    DRV_HASH_HW_SHA512 = 4,
    DRV_HASH_HW_SHA384 = 12,
    DRV_HASH_HW_GHASH = 6,
    DRV_HASH_HW_RESERVE32B = INT32_MAX
};

enum drv_multi2_mode {
    DRV_MULTI2_NULL = -1,
    DRV_MULTI2_ECB = 0,
    DRV_MULTI2_CBC = 1,
    DRV_MULTI2_OFB = 2,
    DRV_MULTI2_RESERVE32B = INT32_MAX
};


/* drv_crypto_key_type[1:0] is mapped to cipher_do[1:0] */
/* drv_crypto_key_type[2] is mapped to cipher_config2 */
enum drv_crypto_key_type {
    DRV_NULL_KEY = -1,
    DRV_USER_KEY = 0,       /* 0x000 */
    DRV_ROOT_KEY = 1,       /* 0x001 */
    DRV_PROVISIONING_KEY = 2,   /* 0x010 */
    DRV_SESSION_KEY = 3,        /* 0x011 */
    DRV_APPLET_KEY = 4,     /* NA */
    DRV_PLATFORM_KEY = 5,       /* 0x101 */
    DRV_CUSTOMER_KEY = 6,       /* 0x110 */
    DRV_END_OF_KEYS = INT32_MAX,
};

enum drv_crypto_padding_type {
    DRV_PADDING_NONE = 0,
    DRV_PADDING_PKCS7 = 1,
    DRV_PADDING_RESERVE32B = INT32_MAX
};


typedef enum DrvAeadCcmFlow {
    DRV_AEAD_FLOW_NULL = 0,
    DRV_AEAD_FLOW_ADATA_INIT,
    DRV_AEAD_FLOW_ADATA_PROCESS,
    DRV_AEAD_FLOW_TEXT_DATA_INIT,
    DRV_AEAD_FLOW_TEXT_DATA_PROCESS,
    DRV_AEAD_FLOW_RESERVE32B = INT32_MAX,
} DrvAeadCcmFlow_e;

typedef enum DataBlockType {
    FIRST_BLOCK,
    MIDDLE_BLOCK,
    LAST_BLOCK,
    RESERVE32B_BLOCK = INT32_MAX
}DataBlockType_t;

typedef enum DrvAesCoreEngine {
     DRV_AES_ENGINE1,
     DRV_AES_ENGINE2,
     DRV_AES_ENGINES_RESERVE32B = INT32_MAX
}DrvAesCoreEngine_t;

typedef enum TunnelOp {
     TUNNEL_OP_INVALID = -1,
     TUNNEL_OFF = 0,
     TUNNEL_ON = 1,
     TunnelOp_OPTIONS,
     TunnelOp_END = INT32_MAX,
} TunnelOp_t;

/*******************************************************************/
/***************** DESCRIPTOR BASED CONTEXTS ***********************/
/*******************************************************************/

struct drv_ctx_hash {
    uint8_t  digest[CC_DIGEST_SIZE_MAX];
    uint32_t CurrentDigestedLength[DRV_HASH_LENGTH_WORDS];
    uint32_t k0[CC_HMAC_BLOCK_SIZE_MAX/sizeof(uint32_t)]; /* not used in hash operation */
    uint32_t k0_size;
    enum drv_crypto_alg alg; /* ssi_drv_crypto_alg_HASH */
    enum drv_hash_mode mode;
    uint32_t dataCompleted;
    uint32_t hmacFinalization;
    /* reserve to end of allocated context size */
    uint32_t reserved[CC_DRV_CTX_SIZE_WORDS - 5 -
            DRV_HASH_LENGTH_WORDS - CC_DIGEST_SIZE_MAX/sizeof(uint32_t) - CC_HMAC_BLOCK_SIZE_MAX/sizeof(uint32_t)];
};

struct drv_ctx_cipher {
    /* block_state is the AES engine block state.
    *  It is used by the host to pass IV or counter at initialization.
    *  It is used by SeP for intermediate block chaining state and for
    *  returning MAC algorithms results.           */
    uint8_t block_state[CC_AES_BLOCK_SIZE];
    uint8_t key[CC_AES_KEY_SIZE_MAX];
    uint8_t xex_key[CC_AES_KEY_SIZE_MAX];
    enum drv_crypto_alg alg; /* DRV_CRYPTO_ALG_AES */
    enum drv_cipher_mode mode;
    enum drv_crypto_direction direction;
    enum drv_crypto_key_type crypto_key_type;
    enum drv_crypto_padding_type padding_type;
    uint32_t key_size; /* numeric value in bytes   */
    uint32_t data_unit_size; /* required for XTS */
     /* this flag indicates whether the user processed at least
    one data block:
    "0" no data blocks processed
    "1" at least one data block processed */
    DataBlockType_t dataBlockType;
    TunnelOp_t isTunnelOp;
    DrvAesCoreEngine_t engineCore;
    uint32_t tunnetDir;
    /* reserve to end of allocated context size */
    uint32_t reserved[CC_DRV_CTX_SIZE_WORDS - 11 -
        CC_AES_BLOCK_SIZE/sizeof(uint32_t) - 2 *
        (CC_AES_KEY_SIZE_MAX/sizeof(uint32_t))];
};

/* authentication and encryption with associated data class */
struct drv_ctx_aead {
    /* block_state1/2 is the AES engine block state */
    uint8_t block_state[CC_AES_BLOCK_SIZE];
    uint8_t key[CC_AES_KEY_SIZE_MAX];
    uint8_t mac_state[CC_AES_BLOCK_SIZE]; /* MAC result */
    uint8_t nonce[CC_AES_BLOCK_SIZE]; /* nonce buffer */
    enum drv_crypto_alg alg; /* ssi_drv_crypto_alg_AES */
    enum drv_cipher_mode mode;
    enum drv_crypto_direction direction;
    uint32_t key_size; /* numeric value in bytes   */
    uint32_t nonce_size; /* nonce size (octets) */
    uint32_t header_size; /* finit additional data size (octets) */
    uint32_t text_size; /* finit text data size (octets) */
    uint32_t tag_size; /* mac size, element of {4, 6, 8, 10, 12, 14, 16} */
    uint32_t internalMode; /* auth/encrypt/decrypt modes */
    uint32_t q; /* an element of {2, 3, 4, 5, 6, 7, 8}; */
    uint32_t headerRemainingBytes; /* associated data remaining bytes */
    DrvAeadCcmFlow_e nextProcessingState; /* points to the next machine state */
    /* reserve to end of allocated context size */
    uint32_t reserved[CC_DRV_CTX_SIZE_WORDS - 12 -
        3 * (CC_AES_BLOCK_SIZE/sizeof(uint32_t)) -
        CC_AES_KEY_SIZE_MAX/sizeof(uint32_t)];
};

/*******************************************************************/
/***************** MESSAGE BASED CONTEXTS **************************/
/*******************************************************************/


/* Get the address of a @member within a given @ctx address
   @ctx: The context address
   @type: Type of context structure
   @member: Associated context field */
#define GET_CTX_FIELD_ADDR(ctx, type, member) (ctx + offsetof(type, member))

#endif /* _CC_CRYPTO_CTX_H_ */

