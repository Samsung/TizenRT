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

#ifndef DUBHE_CMAC_H
#define DUBHE_CMAC_H

#include <stdint.h>
#include <stddef.h>
#include "dubhe_sca.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DBH_CMAC_BAD_STATE_ERR -0x17000
#define DGH_CMAC_BAD_OOM_ERR   -0x17001

struct dbh_cmac_ctx;

/**
 * cmac context structure
 */
typedef struct arm_ce_cmac_context {
    struct dbh_cmac_ctx *cctx;  /**< cmac private context */
} arm_ce_cmac_context_t;

/**
 * \brief          This function initializes the specified ARM CMAC context.
 *                 It must be the first API called before using
 *                 the context.
 *
 * \param ctx      The ARM CMAC context to initialize.
 * \param malg     The main algorithm of AES or SM4 only.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cmac_init( arm_ce_cmac_context_t *ctx, arm_ce_sca_alg_t malg );

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      The ARM CMAC context.
 * \param key      Buffer hold the key.
 * \param keybits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>192 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cmac_set_key( arm_ce_cmac_context_t *ctx,
                         const unsigned char *key,
                         size_t keybits );

/**
 * \brief          This function sets the encryption and decryption derived key.
 *
 * \param ctx      Hold the ARM CMAC context.
 * \param type     Key type. Valid options are ARM_CE_SCA_MODEL_KEY and ARM_CE_SCA_DEVICE_ROOT_KEY
 * \param key1     Buffer hold the key1.
 * \param key2     Buffer hold the key2.
 * \param key3     Buffer hold the key3.
 * \param key1bits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cmac_set_derived_key( arm_ce_cmac_context_t *ctx,
                                 arm_ce_sca_key_type_t type,
                                 const uint8_t *key1,
                                 const uint8_t *key2,
                                 const uint8_t *key3,
                                 size_t key1bits );

/**
 * \brief          This function starts a CMAC session.
 *
 * \param ctx      The CMAC context.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cmac_start( arm_ce_cmac_context_t *ctx );

/**
 * \brief          This function feeds data into an on going CMAC session.
 *
 * \param ctx      The CMAC context.
 * \param length   The length of \p input data in Bytes.
 * \param input    The buffer hold the input data.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cmac_update( arm_ce_cmac_context_t *ctx,
                        size_t length,
                        const unsigned char *input );

/**
 * \brief          This function free the specified ARM CMAC context.
 *
 * \param ctx      The ARM CMAC context to free.
 *
 * \return         \c 0 on success, \c others on failure.
 */
int arm_ce_cmac_free( arm_ce_cmac_context_t *ctx );

/**
 * \brief          This function finishes an on going CMAC session.
 *
 * \param ctx      The ARM CMAC context.
 * \param mac      The buffer to hold the MAC.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cmac_finish( arm_ce_cmac_context_t *ctx, unsigned char *mac );

/**
 * \brief          This function resets the CMAC computation and prepares the
 *                 authentication of another message with the same key as the
 *                 previous CMAC operation.
 *
 * \param ctx      The ARM CMAC context.
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_cmac_reset( arm_ce_cmac_context_t *ctx );

#ifdef __cplusplus
}
#endif

#endif
/*************************** The End Of File*****************************/