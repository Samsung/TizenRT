/**
 * \file sm2pke.h
 *
 * \brief This file contains ECDH definitions and functions.
 *
 */
/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
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

#ifndef MBEDTLS_SM2PKE_H
#define MBEDTLS_SM2PKE_H

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           The SM2PKE context structure.
 */
typedef struct {
    const mbedtls_md_info_t *md_info;
    mbedtls_ecp_group grp; /*!< The elliptic curve used. */
    mbedtls_mpi d;         /*!< The private key. */
    mbedtls_ecp_point Q;   /*!< The public key. */
    mbedtls_ecp_point Qp;  /*!< The value of the public key of the peer. */
    int point_format;      /*!< The format of point export in TLS messages. */
} mbedtls_sm2pke_context;

/**
 * \brief           This function generates an SM2PKE keypair on an elliptic
 *                  curve.
 *
 *                  This function performs the first of two core computations
 *                  implemented during the SM2PKE key exchange. The second core
 *                  computation is performed by mbedtls_sm2dh_compute_shared().
 *
 * \see             ecp.h
 *
 * \param grp       The ECP group.
 * \param d         The destination MPI (private key).
 * \param Q         The destination point (public key).
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX or
 *                  \c MBEDTLS_MPI_XXX error code on failure.
 *
 */
int mbedtls_sm2pke_gen_public( mbedtls_ecp_group *grp,
                               mbedtls_mpi *d,
                               mbedtls_ecp_point *Q,
                               int ( *f_rng )( void *,
                                               unsigned char *,
                                               size_t ),
                               void *p_rng );

/**
 * \brief           This function initializes an SM2PKE context.
 *
 * \param ctx       The SM2PKE context to initialize.
 */
void mbedtls_sm2pke_init( mbedtls_sm2pke_context *ctx );

/**
 * \brief           This function frees a context.
 *
 * \param ctx       The context to free.
 */
void mbedtls_sm2pke_free( mbedtls_sm2pke_context *ctx );

/**
 * \brief           This function generates a public key and a TLS
 *                  ServerKeyExchange payload.
 *
 *                  This is the first function used by a TLS server for SM2PKE
 *                  ciphersuites.
 *
 * \note            This function assumes that the ECP group (grp) of the
 *                  \p ctx context has already been properly set,
 *                  for example, using mbedtls_ecp_group_load().
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2PKE context.
 * \param olen      The number of characters written.
 * \param buf       The destination buffer.
 * \param blen      The length of the destination buffer.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 */
int mbedtls_sm2pke_make_params( mbedtls_sm2pke_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int ( *f_rng )( void *,
                                                unsigned char *,
                                                size_t ),
                                void *p_rng );

/**
 * \brief           This function parses and processes a TLS ServerKeyExhange
 *                  payload.
 *
 *                  This is the first function used by a TLS client for SM2PKE
 *                  ciphersuites.
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2PKE context.
 * \param buf       The pointer to the start of the input buffer.
 * \param end       The address for one Byte past the end of the buffer.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 *
 */
int mbedtls_sm2pke_read_params( mbedtls_sm2pke_context *ctx,
                                const unsigned char **buf,
                                const unsigned char *end );
/**
 * \brief           This function performs the SM2PKE encryption operation.
 *
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2PKE context.
 * \param md_alg    The message digest that was used to do hash.
 * \param output    The encrypted data buf.
 * \param olen      The encrypted data length.
 * \param input     The plaintext data buf.
 * \param ilen      The plaintext data length.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 */
int mbedtls_sm2pke_encrypt( mbedtls_sm2pke_context *ctx,
                            mbedtls_md_type_t md_alg,
                            unsigned char *output,
                            size_t *olen,
                            unsigned char *input, /*in*/
                            size_t ilen,          /*in*/
                            int ( *f_rng )( void *, unsigned char *, size_t ),
                            void *p_rng );
/**
 * \brief           This function performs the SM2PKE encryption operation.
 *
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2PKE context.
 * \param md_alg    The message digest that was used to do hash.
 * \param output    The decrypted data buf.
 * \param olen      The decrypted data length.
 * \param input     The encrypted data buf.
 * \param ilen      The encrypted data length.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 */
int mbedtls_sm2pke_decrypt( mbedtls_sm2pke_context *ctx,
                            mbedtls_md_type_t md_alg,
                            unsigned char *output,
                            size_t *olen,
                            unsigned char *input, /*in*/
                            size_t ilen,          /*in*/
                            int ( *f_rng )( void *, unsigned char *, size_t ),
                            void *p_rng );

#ifdef __cplusplus
}
#endif

#endif /* sm2pke.h */
