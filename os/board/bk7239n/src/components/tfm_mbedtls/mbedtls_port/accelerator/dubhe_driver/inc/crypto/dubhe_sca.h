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

#ifndef DUBHE_SCA_H
#define DUBHE_SCA_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DBH_SCA_INFO_WM     ( 0x12000 )
/* DUBHE SCA ERROR CODE */
#define DBH_SCA_MODE_ERR -0x12000
#define DBH_SCA_KEYBITS_INVAL_ERR -0x12001
#define DBH_SCA_KEY_TYPE_INVAL_ERR -0x12002
#define DBH_SCA_MAC_INVAL_ERR -0x12003
#define DBH_SCA_DATA_ALIGN_ERR -0x12004
#define DBH_SCA_PARAM_INVAL_ERR -0x12005
#define DBH_SCA_BUS_ERR -0x12006
#define DBH_SCA_INVALID_CMD_ERR -0x12007
#define DBH_SCA_MAC_TYPE_ERR -0x12008

#define ARM_CE_SCA_MAGIC 0xdeadbeea
#define ARM_CE_SCA_KEY_BITS_128 0x80
#define ARM_CE_SCA_KEY_BITS_192 0xc0
#define ARM_CE_SCA_KEY_BITS_256 0x100

#define ARM_CE_SCA_KEY_BITS_TYPE_128 0x0
#define ARM_CE_SCA_KEY_BITS_TYPE_192 0x1
#define ARM_CE_SCA_KEY_BITS_TYPE_256 0x2

#define ARM_CE_SCA_BLK_SIZE 0x10
#define ARM_CE_SCA_IV_BYTES ARM_CE_SCA_BLK_SIZE

#define ARM_CE_SCA_CTX_NEED_INIT 0x1
#define ARM_CE_SCA_CTX_INITIALIZED 0x2

#define ARM_CE_SCA_CTX_NEED_FINISH 0x1

#define ARM_CE_SCA_CTX_LAST_BLOCK 0x1

#define DBH_SCA_CMD_INTR 0x1
#define DBH_SCA_INVAL_CMD_INTR 0x2
#define DBH_SCA_INVAL_KEY_ERR_INTR 0x4
#define DBH_SCA_BUS_ERR_INTR 0x8
#define DBH_SCA_WM_INTR 0x10

#define DBH_SCA_QUEUE_DEPTH 0x2

#define DBH_SCA_ENGINE_BUSY -1
#define DBH_SCA_ENGINE_IDLE 0

#define DBH_SCA_MODE_ECB 0x0
#define DBH_SCA_MODE_CTR 0x1
#define DBH_SCA_MODE_CBC 0x2
#define DBH_SCA_MODE_CBC_MAC 0x3
#define DBH_SCA_MODE_CMAC 0x4
#define DBH_SCA_MODE_GHASH 0x5

#define DBH_SCA_INIT_OPCODE 0x80
#define DBH_SCA_PROCESS_OPCODE 0x40
#define DBH_SCA_FINISH_OPCODE 0x20

#define DBH_SCA_OPCODE_BIT_SHIFT 0x18
#define DBH_SCA_KEY_SRC_BIT_SHIFT 0x16
#define DBH_SCA_KEYBITS_BIT_SHIFT 0x14
#define DBH_SCA_ALOG_STD_BIT_SHIFT 0x13
#define DBH_SCA_KEY_FORMAT_BIT_SHIFT 0x12
#define DBH_SCA_LOAD_IV_BIT_SHIFT 0xF
#define DBH_SCA_IV_FORMAT_BIT_SHIFT 0xE
#define DBH_SCA_OP_MODE_BIT_SHIFT 0x5
#define DBH_SCA_LAST_BLOCK_BIT_SHIFT 0x6

#define DBH_SCA_TRIG_INTR_BLK_SIZE 0x200

#define DBH_SCA_CTR_EXTRA_VALID 0x1
#define DBH_SCA_CTR_EXTRA_INVALID 0x0

typedef enum {
    ARM_CE_SCA_ENG_IDLE,
    ARM_CE_SCA_ENG_WAIT_FOR_CMD,
    ARM_CE_SCA_ENG_NORMAL_OPTS,
} arm_ce_sca_engine_status_t;

typedef enum {
    ARM_CE_SCA_MODEL_KEY,
    ARM_CE_SCA_DEVICE_ROOT_KEY,
    ARM_CE_SCA_EXTERNAL_KEY,
} arm_ce_sca_key_type_t;

typedef enum {
    ARM_CE_AES_ECB = 0,
    ARM_CE_AES_CTR,
    ARM_CE_AES_CBC,
    ARM_CE_SM4_ECB,
    ARM_CE_SM4_CBC,
    ARM_CE_SM4_CTR,
    ARM_CE_AES_CBCMAC,
    ARM_CE_AES_CMAC,
    ARM_CE_AES_CCM,
    ARM_CE_AES_GCM,
    ARM_CE_SM4_CBCMAC,
    ARM_CE_SM4_CMAC,
    ARM_CE_SM4_CCM,
    ARM_CE_SM4_GCM,
    ARM_CE_GHASH_MAC
} arm_ce_sca_type_t;

typedef enum{
    ARM_CE_SCA_OPERATION_NONE = -1,
    ARM_CE_SCA_OPERATION_DECRYPT = 0,
    ARM_CE_SCA_OPERATION_ENCRYPT = 1,
}arm_ce_sca_operation_t;
typedef enum { ARM_CE_AES, ARM_CE_SM4, ARM_CE_GHASH } arm_ce_sca_alg_t;

typedef enum {
    ARM_CE_NONE_MAC = 0,
    ARM_CE_CBC_MAC,
    ARM_CE_CMAC,
    ARM_CE_GMAC
} arm_ce_sca_mac_type_t;

/* This structure is used for SCA mulit-session. */
#define BLOCK_SIZE ( 16 )

typedef struct arm_ce_sca_context {
    uint32_t magic;                 /* Magic number for this structure
                                     * */
    arm_ce_sca_alg_t alg_type;      /* distinguish sca algorithm aes or sm4 */
    int mode;                       /* Record crypt mode, decryption or
                                     * encryption */
    arm_ce_sca_mac_type_t mac_type; /* distinguish mac algorithm cbc_mac, cmac or ghash */
    uint8_t ctx_last_block_flag;    /* This flag indicates whether
                                     * the context process the last block */
    arm_ce_sca_key_type_t key_type; /* Include raw_key, mod_root_key and
                                     * dev_root_key */
    arm_ce_sca_type_t sca_type;
    unsigned int keybits; /* Include raw_key, mod_root_key,
                           * dev_root_key */
    union {
        struct ctr_cntx {
            uint8_t cur_iv[16];
            uint8_t cur_iv_ct[16]; /* Nonce counter cipher_text */
            size_t nc_off;         /* It is used to record padding
                                    * position. */
        } ctr_value;
        unsigned char
            last_blk[BLOCK_SIZE]; /* Valid for cbc/cbc-mac/cmac/ghash encryption. */
    } inter_value;                /* Intermediate value
                                   * for cbc or ctr. */
    /* ek2, ek3 valid when using root key */
    unsigned char ek1[32];
    unsigned char ek2[16];
    unsigned char ek3[16];
    uint8_t ctx_init_flag;   /* This flag indicates whether
                              * the context need to be re-init. */
    uint8_t ctx_finish_flag; /* This flag indicates whether
                              * the context need to be finished. */
    uint8_t retry_count;     /*in case of hardware exception and hang need to re
                                init engine to recover*/
    void *private_ptr;           /* Used to point to the relative
                              * mbedtls_aes_context for testing or
                              * other condition*/
} arm_ce_sca_context_t;

typedef struct {
    uint8_t extra_encrypt_flag; /* Used to record whether extra
                                 * data need to be encrypted. */
    size_t extra_len;           /* Extra data length. */
    size_t output_offset;       /* It is used to record the
                                 * output data offset for
                                 * cryption. */
    size_t input_offset;        /* It is used to record the
                                 * input data offsett for
                                 * cryption. */
    size_t input_data_length;   /* It is used to record actual length
                                 * for SCA cryption. */
} arm_ce_sca_crypt_info_t;

/**
 * \brief          This function initializes the Dubhe SCA module.
 */
void arm_ce_sca_driver_init( void );

/**
 * \brief          This function initializes the specified ARM SCA context.
 *
 *                 It must be the first API called before using
 *                 the context.
 *
 * \param ctx      The ARM SCA context to initialize.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_sca_init( arm_ce_sca_context_t *ctx );

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      The ARM SCA context to which is used to do multi-session.
 * \param key1     The encryption key1.
 * \param key2     The encryption key2, only valid for derived key.
 * \param key3     The encryption key3, only valid for derived key.
 * \param keybits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>192 bits, valid execpt derived key</li>
 *                 <li>256 bits</li></ul>
 * \param derived  If the key is derived by key ladder or not.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_sca_set_key( arm_ce_sca_context_t *ctx,
                        arm_ce_sca_key_type_t type,
                        const unsigned char *key1,
                        const unsigned char *key2,
                        const unsigned char *key3,
                        unsigned int key1bits,
                        uint8_t derived );
/**
 * \brief  This function performs an SCA encryption or decryption operation.
 *
 *         It performs the operation defined in the \p mode
 *         parameter (encrypt/decrypt), on the input data buffer defined in
 *         the \p input parameter.
 *
 * \note   For SCA aes ecb and cbc, the input size must be a multiple of the AES
 * block size of 16 Bytes. \note   Upon exit, the content of the IV is updated
 * so that you can call the same function again on the next block(s) of data and
 * get the same result as if it was encrypted in one call. This allows a
 * "streaming" usage. If you need to retain the contents of the IV, you should
 *         either save it manually or use the cipher module instead.
 *
 * \param ctx      The ARM SCA context to use for multi-session.
 * \param mode     The ARM SCA operation: encryption or decryption.
 * \param length   The length of the input data in Bytes.
 * \param iv       Initialization vector (updated after use).
 * \param input    The buffer holding the input data.
 * \param output   The buffer holding the output data.
 *
 * \return         \c 0 on success, -1 on failure.
 */

int arm_ce_sca_crypt( arm_ce_sca_context_t *ctx,
                      int mode,
                      arm_ce_sca_type_t type,
                      arm_ce_sca_mac_type_t mac_type,
                      size_t length,
                      size_t *nc_off,
                      unsigned char *iv,
                      unsigned char *stream_block,
                      const unsigned char *input,
                      unsigned char *output );

/**
 * \brief          This function flushes the session context,
 *                 releases and clears the specified ARM SCA context.
 *
 * \param ctx      The ARM SCA context to clear.
 * \param mac      The dump mac address
 */

int arm_ce_sca_free( arm_ce_sca_context_t *ctx, unsigned char *mac );

/**
 * \brief          This function finishes the MAC operation,
 *                 and writes the result to the output buffer.
 *
 * \param ctx      The ARM SCA context to finish.
 * \param mac      The dump mac address
 */
int arm_ce_sca_finish( arm_ce_sca_context_t *ctx, unsigned char *mac );

#ifdef __cplusplus
}
#endif

#endif
/*************************** The End Of File*****************************/
