
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

#ifndef DUBHE_GHASH_H
#define DUBHE_GHASH_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>

#define DGH_GHASH_ERR_OOM                           (-0x13000)
#define DGH_GHASH_ERR_BAD_INPUT_LENGTH              (-0x13001)
#define DGH_GHASH_BAD_STATE                         (-0x16000)

struct dbh_ghash_ctx;

/**
 * ccm context structure
 */
typedef struct arm_ce_ghash_context {
    struct dbh_ghash_ctx *gctx;  /**< ghash private context */
} arm_ce_ghash_ctx_t;

/**
 * \brief          This function initializes the specified ARM GHASH context.
 *                 It must be the first API called before using
 *                 the context.
 *
 * \param ctx      The ARM GHASH context to initialize.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_ghash_init( arm_ce_ghash_ctx_t *ctx);

/**
 * \brief          This function free the specified ARM GHASH context.
 *
 * \param ctx      The ARM GHASH context to free.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int arm_ce_ghash_free( arm_ce_ghash_ctx_t *ctx );

/**
 * \brief          This function sets the key to ghash session.
 *
 * \param ctx      The ARM GHASH context.
 * \param key      Buffer hold the key, Only support 128bits.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ghash_set_key( arm_ce_ghash_ctx_t *ctx,
                          const uint8_t key[16] );

/**
 * \brief  This function starts a GHASH session.
 *
 * \param ctx           The GHASH context.
 * \param iv            Initial vector for the session.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ghash_start( arm_ce_ghash_ctx_t *ctx,
                        const uint8_t iv[16] );

/**
 * \brief  This function feeds data into an on going GHASH session.
 *
 * \param ctx           The GHASH context.
 * \param length        The length of \p input data in Bytes.
 * \param input         The buffer hold the input data.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ghash_update( arm_ce_ghash_ctx_t *ctx,
                         size_t length,
                         const uint8_t *input );

/**
 * \brief  This function finishes an on going GHASH session.
 *
 * \param ctx           The GHASH context.
 * \param mac           The buffer to hold the MAC.
 *
 * \return         \c 0 on success, others on failure.
 */
int arm_ce_ghash_finish( arm_ce_ghash_ctx_t *ctx,
                         uint8_t *mac );

#ifdef __cplusplus
}
#endif
#endif /* DUBHE_GHASH_H */