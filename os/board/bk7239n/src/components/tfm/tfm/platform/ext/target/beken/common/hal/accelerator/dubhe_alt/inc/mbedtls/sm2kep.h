/**
 * \file sm2kep.h
 *
 * \brief This file contains SM2KEP(SM2 Key Exchange Protocol) definitions and
 *        functions.
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

#ifndef MBEDTLS_SM2KEP_H
#define MBEDTLS_SM2KEP_H

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines the source of the key exchange.
 */
typedef enum {
    MBEDTLS_SM2KEP_INITIATOR = 0x02, /**< initiator of the key exchange. */
    MBEDTLS_SM2KEP_RESPONDER = 0x03, /**< The responder of the key exchange. */
} mbedtls_sm2kep_side;

/**
 * \brief           The SM2KEP context structure.
 */
typedef struct {
    const mbedtls_md_info_t *md_info;
    mbedtls_ecp_group grp; /*!< The elliptic curve used. */
    mbedtls_mpi d;         /*!< The private key. */
    mbedtls_ecp_point Q;   /*!< The public key. */
    mbedtls_ecp_point Qp;  /*!< The value of the public key of the peer. */
    int point_format;      /*!< The format of point export in TLS messages. */
    mbedtls_mpi r;         /*!< The temporary private key. */
    mbedtls_ecp_point R;   /*!< The temporary public key. */
    mbedtls_ecp_point
        Rp; /*!< The value of the temporary public key of the peer. */
    mbedtls_ecp_point P; /*!< The shared secret point. */
} mbedtls_sm2kep_context;

/**
 * \brief           This function generates an SM2KEP keypair on an elliptic
 *                  curve.
 *
 *                  This function performs the first of two core computations
 *                  implemented during the SM2KEP key exchange. The second core
 *                  computation is performed by mbedtls_sm2kep_compute_shared().
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
int mbedtls_sm2kep_gen_public( mbedtls_ecp_group *grp,
                               mbedtls_mpi *d,
                               mbedtls_ecp_point *Q,
                               int ( *f_rng )( void *,
                                               unsigned char *,
                                               size_t ),
                               void *p_rng );

/**
 * \brief           This function computes destination point which used to
 *                  compute shared secret.
 *
 *                  This function performs the second of two core computations
 *                  implemented during the SM2KEP key exchange. The first core
 *                  computation is performed by mbedtls_sm2kep_gen_public().
 *
 * \see             ecp.h
 *
 * \note            If \p f_rng is not NULL, it is used to implement
 *                  countermeasures against side-channel attacks.
 *                  For more information, see mbedtls_ecp_mul().
 *
 * \param grp       The ECP group.
 * \param P         The destination point (used to compute shared secret).
 * \param R         The temporary public key.
 * \param Rp        The value of the temporary public key of the peer.
 * \param Qp        The value of the public key of the peer.
 * \param d         Our secret exponent (private key).
 * \param r         The temporary private key.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX or
 *                  \c MBEDTLS_MPI_XXX error code on failure.
 */
int mbedtls_sm2kep_compute_shared( mbedtls_ecp_group *grp,
                                   mbedtls_ecp_point *P,
                                   const mbedtls_ecp_point *R,
                                   const mbedtls_ecp_point *Rp,
                                   const mbedtls_ecp_point *QP,
                                   const mbedtls_mpi *d,
                                   const mbedtls_mpi *r,
                                   int ( *f_rng )( void *,
                                                   unsigned char *,
                                                   size_t ),
                                   void *p_rng );

/**
 * \brief           This function initializes an SM2KEP context.
 *
 * \param ctx       The SM2KEP context to initialize.
 */
void mbedtls_sm2kep_init( mbedtls_sm2kep_context *ctx );

/**
 * \brief           This function frees a context.
 *
 * \param ctx       The context to free.
 */
void mbedtls_sm2kep_free( mbedtls_sm2kep_context *ctx );

/**
 * \brief           This function generates two SM2KEP keypair((d,Q) and
 *                  temporary (r,R)) and a TLS ServerKeyExchange payload.
 *
 *                  This is the first function used by a TLS server for SM2KEP
 *                  ciphersuites.
 *
 * \note            This function assumes that the ECP group (grp) of the
 *                  \p ctx context has already been properly set,
 *                  for example, using mbedtls_ecp_group_load().
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2KEP context.
 * \param olen      The number of characters written.
 * \param buf       The destination buffer.
 * \param blen      The length of the destination buffer.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 */
int mbedtls_sm2kep_make_params( mbedtls_sm2kep_context *ctx,
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
 *                  This is the first function used by a TLS client for SM2KEP
 *                  ciphersuites.
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2KEP context.
 * \param buf       The pointer to the start of the input buffer.
 * \param end       The address for one Byte past the end of the buffer.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 *
 */
int mbedtls_sm2kep_read_params( mbedtls_sm2kep_context *ctx,
                                const unsigned char **buf,
                                const unsigned char *end );

/**
 * \brief           This function generates a public key and a TLS
 *                  ClientKeyExchange payload.
 *
 *                  This is the second function used by a TLS client for SM2KEP
 *                  ciphersuites.
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2KEP context.
 * \param olen      The number of Bytes written.
 * \param buf       The destination buffer.
 * \param blen      The size of the destination buffer.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 */
int mbedtls_sm2kep_make_public( mbedtls_sm2kep_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int ( *f_rng )( void *,
                                                unsigned char *,
                                                size_t ),
                                void *p_rng );

/**
 * \brief       This function parses and processes a TLS ClientKeyExchange
 *              payload.
 *
 *              This is the second function used by a TLS server for SM2KEP
 *              ciphersuites.
 *
 * \see         ecp.h
 *
 * \param ctx   The SM2KEP context.
 * \param buf   The start of the input buffer.
 * \param blen  The length of the input buffer.
 *
 * \return      \c 0 on success.
 * \return      An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 */
int mbedtls_sm2kep_read_public( mbedtls_sm2kep_context *ctx,
                                const unsigned char **buf,
                                const unsigned char *end );

/**
 * \brief           This function derives and exports the shared secret.
 *
 *
 * \note            If \p f_rng is not NULL, it is used to implement
 *                  countermeasures against side-channel attacks.
 *                  For more information, see mbedtls_ecp_mul().
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2KEP context.
 * \param md_alg    The message digest that was used to do hash.
 * \param buf       The destination buffer.
 * \param blen      The length of the destination buffer.
 * \param ZA        The hash value of user A's identifiable identifier, partial
 *                  elliptic curve system parameters, and user A's public key,
 *                  please refer to SM2 spec.
 * \param ZAlen     The length of ZA.
 * \param ZB        The hash value of user B's identifiable identifier,
 *                  partial elliptic curve system parameters,and user B's
 *                  public key,please refer to SM2 spec.
 * \param ZBlen     The length of ZB.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.
 */
int mbedtls_sm2kep_calc_secret( mbedtls_sm2kep_context *ctx,
                                mbedtls_md_type_t md_alg,
                                unsigned char *buf,
                                size_t blen,
                                unsigned char *ZA, /*in*/
                                size_t ZAlen,      /*in*/
                                unsigned char *ZB, /*in*/
                                size_t ZBlen,      /*in*/
                                int ( *f_rng )( void *,
                                                unsigned char *,
                                                size_t ),
                                void *p_rng );
/**
 * \brief           This function compute the SM2KEP checksum.
 *
 *                  This is the last function used by both TLS client
 *                  and servers.
 *
 * \note            The size of  \p SI and \p SR should equal to digest size of
 *                  \p md_alg
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2KEP context.
 * \param md_alg    The message digest that was used to do hash.
 * \param side      Defines the source of the key exchange,refer to enum
 *                  "mbedtls_sm2kep_side".
 * \param SI        The checksum of INITIATOR,if we are INITIATOR,we need to
 *                  keep the checksum,if we are RESPONDOR, we need to send
 *                  the checksum to INITIATOR.
 * \param SR        The checksum of RESPONDOR,if we are RESPONDOR,we need to
 *                  keep the checksum, if we are INITIATOR, we need to send
 *                  the checksum to RESPONDOR.
 * \param ZA        The hash value of user A's identifiable identifier, partial
 *                  elliptic curve system parameters, and user A's public key,
 *                  please refer to SM2 spec.
 * \param ZAlen     The length of ZA.
 * \param ZB        The hash value of user B's identifiable identifier, partial
 *                  elliptic curve system parameters, and user B's public key,
 *                  please refer to SM2 spec.
 * \param ZBlen     The length of ZB.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX error code on failure.

 */

int mbedtls_sm2kep_calc_checksum( mbedtls_sm2kep_context *ctx,
                                  mbedtls_md_type_t md_alg,
                                  mbedtls_sm2kep_side side,
                                  unsigned char *SI,
                                  unsigned char *SR,
                                  unsigned char *ZA, /*in*/
                                  size_t ZAlen,      /*in*/
                                  unsigned char *ZB, /*in*/
                                  size_t ZBlen /*in*/ );

#ifdef __cplusplus
}
#endif

#endif /* sm2kep.h */
