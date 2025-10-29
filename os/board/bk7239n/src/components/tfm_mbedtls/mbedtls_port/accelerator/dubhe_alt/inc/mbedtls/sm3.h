/**
 * \file sm3.h
 *
 * \brief This file contains ARM Dubhe engine SM3 definitions and functions.
 *
 */
#ifndef ARM_CE_DUBHE_SM3_H
#define ARM_CE_DUBHE_SM3_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(ARM_CE_DUBHE_HASH)

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "dubhe_hash.h"

#define MBEDTLS_ERR_SM3_CTX_NULL       -0x00EC

#ifdef __cplusplus
extern "C" {
#endif

typedef arm_ce_hash_context_t mbedtls_sm3_context;


/**
 * \brief          This function initializes a SM3 context.
 *
 * \param ctx      The SM3 context to initialize.
 */
void mbedtls_sm3_init( mbedtls_sm3_context *ctx );

/**
 * \brief          This function clears a SM3 context.
 *
 * \param ctx      The SM3 context to clear.
 */
void mbedtls_sm3_free( mbedtls_sm3_context *ctx );

/**
 * \brief          This function clones the state of a SM3 context.
 *
 * \param dst      The destination context.
 * \param src      The context to clone.
 */
void mbedtls_sm3_clone( mbedtls_sm3_context *dst,
                        const mbedtls_sm3_context *src );

/**
 * \brief          This function starts a SM3 checksum
 *                 calculation.
 *
 * \param ctx      The context to initialize.
 *
 * \return         \c 0 on success.
 */
int mbedtls_sm3_starts_ret( mbedtls_sm3_context *ctx );

/**
 * \brief          This function feeds an input buffer into an ongoing
 *                 SM3 checksum calculation.
 *
 * \param ctx      The SM3 context.
 * \param input    The buffer holding the data.
 * \param ilen     The length of the input data.
 *
 * \return         \c 0 on success.
 */
int mbedtls_sm3_update_ret( mbedtls_sm3_context *ctx,
                            const unsigned char *input,
                            size_t ilen );

/**
 * \brief          This function finishes the SM3 operation, and writes
 *                 the result to the output buffer.
 *
 * \param ctx      The SM3 context.
 * \param output   The SM3 checksum result.
 *
 * \return         \c 0 on success.
 */
int mbedtls_sm3_finish_ret( mbedtls_sm3_context *ctx,
                            unsigned char output[32] );
/**
 * \brief          This function calculates the SM3 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The SM3 result is calculated as
 *                 output = SM3(input buffer).
 *
 * \param input    The buffer holding the input data.
 * \param ilen     The length of the input data.
 * \param output   The SM3 checksum result.
 *
 * \return         \c 0 on success.
 *
 */
int mbedtls_sm3_ret( const unsigned char *input,
                      size_t ilen,
                      unsigned char output[32] );

/**
 * \brief          This function processes a single data block within
 *                 the ongoing SM3 computation. This function is for
 *                 internal use only.
 *
 * \param ctx      The SM3 context.
 * \param data     The buffer holding one block of data.
 *
 * \return         \c 0 on success.
 */
int mbedtls_internal_sm3_process( mbedtls_sm3_context *ctx,
                                  const unsigned char data[64] );

#ifdef __cplusplus
}
#endif

#endif /* ARM_CE_DUBHE_HASH */
#endif /* ARM_CE_DUBHE_SM3_H */
