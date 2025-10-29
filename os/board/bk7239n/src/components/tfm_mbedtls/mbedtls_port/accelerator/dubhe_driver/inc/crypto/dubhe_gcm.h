
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
#ifndef DUBHE_GCM_H
#define DUBHE_GCM_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define DBH_GCM_ERR_OOM                     (-0x13000)
#define DBH_GCM_ERR_BAD_INPUT_LENGTH        (-0x13001)
#define DBH_GCM_ERR_BAD_STATE               (-0x16000)

#define DBH_GCM_ENCRYPT                     (1U)
#define DBH_GCM_DECRYPT                     (0U)

struct dbh_gcm_ctx;

/**
 * gcm context structure
 */
typedef struct arm_ce_gcm_ctx {
    struct dbh_gcm_ctx *gcm;  /**< GCM private context */
} arm_ce_gcm_ctx_t;

/**
 * \brief          This function initializes the specified ARM GCM context.
 *                 It must be the first API called before using
 *                 the context.
 *
 * \param ctx      The ARM GCM context to initialize.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_gcm_init( arm_ce_gcm_ctx_t *ctx, int32_t alg );

/**
 * \brief          This function free the specified ARM GCM context.
 *
 * \param ctx      The ARM GCM context to free.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_gcm_free( arm_ce_gcm_ctx_t *ctx );

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      The ARM GCM context.
 * \param key      Buffer hold the key.
 * \param keybits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>192 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_gcm_set_key( arm_ce_gcm_ctx_t *ctx,
                        const uint8_t *key,
                        size_t keybits );

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      Hold the ARM GCM context.
 * \param key1     Buffer hold the key1.
 * \param key2     Buffer hold the key2.
 * \param key3     Buffer hold the key3.
 * \param keybits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_gcm_set_derived_key( arm_ce_gcm_ctx_t *ctx,
                                int32_t type,
                                const uint8_t *key1,
                                const uint8_t *key2,
                                const uint8_t *key3,
                                size_t keybits );

/**
 * \brief  This function starts a GCM encryption/decryption session.
 *
 * \param ctx           The GCM context.
 * \param op            The ARM GCM operation: encryption or decryption.
 * \param iv            Buffer hold the initial vector for GCM operation.
 * \param iv_len        The length of \p iv.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_gcm_start( arm_ce_gcm_ctx_t *ctx,
                      int32_t op,
                      const uint8_t *iv,
                      uint64_t iv_len );

/**
 * \brief  This function feeds associated data into an on going GCM session.
 *
 * \param ctx           The GCM context.
 * \param aadlen        The length of \p aad associated data in Bytes.
 * \param aad           The buffer hold the associated data.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_gcm_update_aad( arm_ce_gcm_ctx_t *ctx,
                           size_t aadlen,
                           const uint8_t *aad );

/**
 * \brief  This function feeds payload into an on going GCM session.
 *
 * \param ctx           The GCM context.
 * \param inlen         The length of input data in Bytes.
 * \param in            The buffer hold the input data.
 * \param out           The buffer hold the ouput data.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_gcm_update( arm_ce_gcm_ctx_t *ctx,
                       size_t inlen,
                       const uint8_t *in,
                       uint8_t *out );

/**
 * \brief  This function finishes an on going GCM session.
 *
 * \param ctx           The GCM context.
 * \param tag           The buffer to hold the tag.
 * \param taglen        The length of tag expected \p tag.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_gcm_finish( arm_ce_gcm_ctx_t *ctx,
                       uint8_t *tag, size_t taglen );

#ifdef __cplusplus
}
#endif
#endif /** DUBHE_GCM_H */