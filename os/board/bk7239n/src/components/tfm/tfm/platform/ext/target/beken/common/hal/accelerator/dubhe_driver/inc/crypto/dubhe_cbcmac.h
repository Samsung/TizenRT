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

#ifndef DUBHE_CBCMAC_H
#define DUBHE_CBCMAC_H

#include <stdint.h>
#include <stddef.h>
#include "dubhe_sca.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DBH_CBCMAC_BAD_STATE_ERR -0x16000

typedef enum arm_ce_cbcmac_state {
    CBCMAC_STATE_RAW = 0,
    CBCMAC_STATE_INIT,
    CBCMAC_STATE_READY,
    CBCMAC_STATE_START,
    CBCMAC_STATE_UPDATE
} arm_ce_cbcmac_state_t;

typedef struct arm_ce_cbcmac_context {
    uint8_t iv[BLOCK_SIZE];      /* initial vector */
    uint8_t npdata[BLOCK_SIZE];  /* not processed data */
    uint32_t npdlen;             /* data length in byte */
    arm_ce_cbcmac_state_t state; /* context status */
    arm_ce_sca_context_t inter;  /* interal sca ctx */
} arm_ce_cbcmac_context_t;

/**
 * \brief          This function initializes the specified ARM CBCMAC context.
 *                 It must be the first API called before using
 *                 the context.
 *
 * \param ctx      The ARM CBCMAC context to initialize.
 * \param malg     The main algorithm of AES or SM4 only.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cbcmac_init( arm_ce_cbcmac_context_t *ctx, arm_ce_sca_alg_t malg );

/**
 * \brief          This function starts a CBCMAC session.
 *
 * \param ctx      The CBCMAC context.
 * \param iv[16]   The IV vector.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cbcmac_start( arm_ce_cbcmac_context_t *ctx, unsigned char iv[16] );

/**
 * \brief          This function free the specified ARM CBCMAC context.
 *
 * \param ctx      The ARM CBCMAC context to free.
 *
 * \return         \c 0 on success, \c others on failure.
 */
int arm_ce_cbcmac_free( arm_ce_cbcmac_context_t *ctx );

/**
 * \brief          This function finishes an on going CBCMAC session.
 *
 * \param ctx      The ARM CBCMAC context.
 * \param mac      The buffer to hold the MAC.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cbcmac_finish( arm_ce_cbcmac_context_t *ctx, unsigned char *mac );

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      The ARM CBCMAC context.
 * \param key      Buffer hold the key.
 * \param keybits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>192 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cbcmac_set_key( arm_ce_cbcmac_context_t *ctx,
                           const unsigned char *key,
                           size_t keybits );

/**
 * \brief          This function sets the encryption and decryption derived key.
 *
 * \param ctx      Hold the ARM CBCMAC context.
 * \param key1     Buffer hold the key1.
 * \param key2     Buffer hold the key2.
 * \param key3     Buffer hold the key3.
 * \param key1bits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cbcmac_set_derived_key( arm_ce_cbcmac_context_t *ctx,
                                   arm_ce_sca_key_type_t type,
                                   const uint8_t *key1,
                                   const uint8_t *key2,
                                   const uint8_t *key3,
                                   size_t key1bits );

/**
 * \brief          This function feeds data into an on going CBCMAC session.
 *
 * \param ctx      The CBCMAC context.
 * \param length   The length of \p input data in Bytes.
 * \param input    The buffer hold the input data.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cbcmac_update( arm_ce_cbcmac_context_t *ctx,
                          size_t length,
                          const unsigned char *input );

#ifdef __cplusplus
}
#endif

#endif
/*************************** The End Of File*****************************/