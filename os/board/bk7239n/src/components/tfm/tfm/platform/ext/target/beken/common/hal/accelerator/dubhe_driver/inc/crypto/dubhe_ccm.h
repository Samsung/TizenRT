
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
#ifndef DUBHE_CCM_H
#define DUBHE_CCM_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define DBH_CCM_ENCRYPT                  (1U)
#define DBH_CCM_DECRYPT                  (0U)

#define DBH_CCM_ERR_OOM                  (-0x13000)
#define DBH_CCM_ERR_BAD_STATE            (-0x16000)

struct dbh_ccm_ctx;

/**
 * ccm context structure
 */
typedef struct arm_ce_ccm_ctx {
    struct dbh_ccm_ctx *ccm;  /**< CCM private context */
} arm_ce_ccm_ctx_t;

/**
 * \brief          This function initializes the specified ARM CCM context.
 *                 It must be the first API called before using
 *                 the context.
 *
 * \param ctx      The ARM CCM context to initialize.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_ccm_init( arm_ce_ccm_ctx_t *ctx, int32_t alg );

/**
 * \brief          This function free the specified ARM CCM context.
 *
 * \param ctx      The ARM CCM context to free.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_ccm_free( arm_ce_ccm_ctx_t *ctx );

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      The ARM CCM context.
 * \param key      Buffer hold the key.
 * \param keybits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>192 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ccm_set_key( arm_ce_ccm_ctx_t *ctx,
                       const uint8_t *key,
                       size_t keybits );

/**
 * \brief          This function sets the encryption and decryption key.
 *
 * \param ctx      Hold the ARM CCM context.
 * \param key1     Buffer hold the key1.
 * \param key2     Buffer hold the key2.
 * \param key3     Buffer hold the key3.
 * \param keybits  The size of key1 bits. Valid options are:
 *                 <ul><li>128 bits</li>
 *                 <li>256 bits</li></ul>
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ccm_set_derived_key( arm_ce_ccm_ctx_t *ctx,
                                int32_t type,
                                const uint8_t *key1,
                                const uint8_t *key2,
                                const uint8_t *key3,
                                size_t keybits );

/**
 * \brief  This function starts a CCM encryption/decryption session.
 *
 * \param ctx           The CCM context.
 * \param op            The ARM CCM operation: encryption or decryption.
 * \param nonce         Buffer hold the nonce for CCM operation.
 * \param nonce_len     The length of \p nonce.
 * \param taglen        The length of the expected Tag.
 * \param aadlen        The length of the overall associated data in Bytes.
 * \param payload_len   The length of the overall payload in Bytes.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ccm_start( arm_ce_ccm_ctx_t *ctx,
                      int32_t op,
                      const uint8_t *nonce,
                      size_t nonce_len,
                      size_t taglen,
                      uint64_t aadlen,
                      uint64_t payload_len );

/**
 * \brief  This function feeds associated data into an on going CCM session.
 *
 * \param ctx           The CCM context.
 * \param aadlen        The length of \p aad associated data in Bytes.
 * \param aad           The buffer hold the associated data.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ccm_update_aad( arm_ce_ccm_ctx_t *ctx,
                           size_t aadlen,
                           const uint8_t *aad );

/**
 * \brief  This function feeds payload into an on going CCM session.
 *
 * \param ctx           The CCM context.
 * \param inlen         The length of input data in Bytes.
 * \param in            The buffer hold the input data.
 * \param out           The buffer hold the ouput data.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ccm_update( arm_ce_ccm_ctx_t *ctx,
                       size_t inlen,
                       const uint8_t *in,
                       uint8_t *out );

/**
 * \brief  This function finishes an on going CCM session.
 *
 * \param ctx           The CCM context.
 * \param tag           The buffer to hold the tag.
 * \param taglen        The length of tag expected \p tag.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ccm_finish( arm_ce_ccm_ctx_t *ctx,
                       uint8_t *tag, size_t taglen );

#ifdef __cplusplus
}
#endif
#endif /* DUBHE_CCM_H */