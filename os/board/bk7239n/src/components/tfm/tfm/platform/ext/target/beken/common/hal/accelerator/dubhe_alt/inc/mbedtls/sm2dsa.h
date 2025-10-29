/**
 * \file sm2dsa.h
 *
 * \brief This file contains SM2DSA definitions and functions.
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

#ifndef MBEDTLS_SM2DSA_H
#define MBEDTLS_SM2DSA_H

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"

/*
 *
 *     Sm2dsa-Sig-Value ::= SEQUENCE {
 *         r       INTEGER,
 *         s       INTEGER
 *     }
 *
 * Size is at most
 *    1 (tag) + 1 (len) + 1 (initial 0) + ECP_MAX_BYTES for each of r and s,
 *    twice that + 1 (tag) + 2 (len) for the sequence
 * (assuming ECP_MAX_BYTES is less than 126 for r and s,
 * and less than 124 (total len <= 255) for the sequence)
 */
#if MBEDTLS_ECP_MAX_BYTES > 124
#error                                                                         \
    "MBEDTLS_ECP_MAX_BYTES bigger than expected, please fix MBEDTLS_SM2DSA_MAX_LEN"
#endif
/** The maximal size of an SM2DSA signature in Bytes. */
#define MBEDTLS_SM2DSA_MAX_LEN ( 3 + 2 * ( 3 + MBEDTLS_ECP_MAX_BYTES ) )

/**
 * \brief           The SM2DSA context structure.
 */
typedef mbedtls_ecp_keypair mbedtls_sm2dsa_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           This function computes the SM2DSA signature of a
 *                  previously-hashed message.
 *
 * \note            The deterministic version is usually preferred.
 *
 * \note            If the bitlength of the message hash is larger than the
 *                  bitlength of the group order, then the hash is truncated
 *                  as defined in <em>Standards for Efficient Cryptography Group
 *                  (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                  4.1.3, step 5.
 *
 * \see             ecp.h
 *
 * \param grp       The ECP group.
 * \param r         The first output integer.
 * \param s         The second output integer.
 * \param d         The private signing key.
 * \param buf       The message(the combination of Z and Message, please refer
 *                  to SM2 spec) hash.
 * \param blen      The length of \p buf.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX
 *                  or \c MBEDTLS_MPI_XXX error code on failure.
 */
int mbedtls_sm2dsa_sign( mbedtls_ecp_group *grp,
                         mbedtls_mpi *r,
                         mbedtls_mpi *s,
                         const mbedtls_mpi *d,
                         const unsigned char *buf,
                         size_t blen,
                         int ( *f_rng )( void *, unsigned char *, size_t ),
                         void *p_rng );

/**
 * \brief           This function verifies the SM2DSA signature of a
 *                  previously-hashed message.
 *
 * \note            If the bitlength of the message hash is larger than the
 *                  bitlength of the group order, then the hash is truncated as
 *                  defined in <em>Standards for Efficient Cryptography Group
 *                  (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                  4.1.4, step 3.
 *
 * \see             ecp.h
 *
 * \param grp       The ECP group.
 * \param buf       The message(the combination of Z and Message, please refer
 *                  to SM2 spec) hash.
 * \param blen      The length of \p buf.
 * \param Q         The public key to use for verification.
 * \param r         The first integer of the signature.
 * \param s         The second integer of the signature.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if the signature
 *                  is invalid.
 * \return          An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_MPI_XXX
 *                  error code on failure for any other reason.
 */
int mbedtls_sm2dsa_verify( mbedtls_ecp_group *grp,
                           const unsigned char *buf,
                           size_t blen,
                           const mbedtls_ecp_point *Q,
                           const mbedtls_mpi *r,
                           const mbedtls_mpi *s );

/**
 * \brief           This function computes the SM2DSA signature and writes it
 *                  to a buffer, serialized as defined in <em>RFC-4492:
 *                  Elliptic Curve Cryptography (ECC) Cipher Suites for
 *                  Transport Layer Security (TLS)</em>.
 *
 * \warning         It is not thread-safe to use the same context in
 *                  multiple threads.
 *
 * \note            The deterministic version is used if
 *                  #MBEDTLS_SM2DSA_DETERMINISTIC is defined. For more
 *                  information, see <em>RFC-6979: Deterministic Usage
 *                  of the Digital Signature Algorithm (DSA) and Elliptic
 *                  Curve Digital Signature Algorithm (ECDSA)</em>.
 *
 * \note            The \p sig buffer must be at least twice as large as the
 *                  size of the curve used, plus 9. For example, 73 Bytes if
 *                  a 256-bit curve is used. A buffer length of
 *                  #MBEDTLS_SM2DSA_MAX_LEN is always safe.
 *
 * \note            If the bitlength of the message hash is larger than the
 *                  bitlength of the group order, then the hash is truncated as
 *                  defined in <em>Standards for Efficient Cryptography Group
 *                  (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                  4.1.3, step 5.
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2DSA context.
 * \param md_alg    The message digest that was used to hash the message.
 * \param hash      The message(the combination of Z and Message, please refer
 *                  to SM2 spec) hash.
 * \param hlen      The length of the hash.
 * \param sig       The buffer that holds the signature.
 * \param slen      The length of the signature written.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX, \c MBEDTLS_ERR_MPI_XXX or
 *                  \c MBEDTLS_ERR_ASN1_XXX error code on failure.
 */
int mbedtls_sm2dsa_write_signature( mbedtls_sm2dsa_context *ctx,
                                    mbedtls_md_type_t md_alg,
                                    const unsigned char *hash,
                                    size_t hlen,
                                    unsigned char *sig,
                                    size_t *slen,
                                    int ( *f_rng )( void *,
                                                    unsigned char *,
                                                    size_t ),
                                    void *p_rng );

/**
 * \brief           This function reads and verifies an SM2DSA signature.
 *
 * \note            If the bitlength of the message hash is larger than the
 *                  bitlength of the group order, then the hash is truncated as
 *                  defined in <em>Standards for Efficient Cryptography Group
 *                  (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                  4.1.4, step 3.
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2DSA context.
 * \param hash      The message(the combination of Z and Message, please refer
 *                  to SM2 spec) hash.
 * \param hlen      The size of the hash.
 * \param sig       The signature to read and verify.
 * \param slen      The size of \p sig.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if signature is invalid.
 * \return          #MBEDTLS_ERR_ECP_SIG_LEN_MISMATCH if there is a valid
 *                  signature in \p sig, but its length is less than \p siglen.
 * \return          An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_ERR_MPI_XXX
 *                  error code on failure for any other reason.
 */
int mbedtls_sm2dsa_read_signature( mbedtls_sm2dsa_context *ctx,
                                   const unsigned char *hash,
                                   size_t hlen,
                                   const unsigned char *sig,
                                   size_t slen );

/**
 * \brief          This function generates an SM2DSA keypair on the given curve.
 *
 * \see            ecp.h
 *
 * \param ctx      The SM2DSA context to store the keypair in.
 * \param gid      The elliptic curve to use. One of the various
 *                 \c MBEDTLS_ECP_DP_XXX macros depending on configuration.
 * \param f_rng    The RNG function.
 * \param p_rng    The RNG context.
 *
 * \return         \c 0 on success.
 * \return         An \c MBEDTLS_ERR_ECP_XXX code on failure.
 */
int mbedtls_sm2dsa_genkey( mbedtls_sm2dsa_context *ctx,
                           mbedtls_ecp_group_id gid,
                           int ( *f_rng )( void *, unsigned char *, size_t ),
                           void *p_rng );

/**
 * \brief           This function sets an SM2DSA context from an EC key pair.
 *
 * \see             ecp.h
 *
 * \param ctx       The SM2DSA context to set.
 * \param key       The EC key to use.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX code on failure.
 */
int mbedtls_sm2dsa_from_keypair( mbedtls_sm2dsa_context *ctx,
                                 const mbedtls_ecp_keypair *key );

/**
 * \brief           This function initializes an SM2DSA context.
 *
 * \param ctx       The SM2DSA context to initialize.
 */
void mbedtls_sm2dsa_init( mbedtls_sm2dsa_context *ctx );

/**
 * \brief           This function frees an SM2DSA context.
 *
 * \param ctx       The SM2DSA context to free.
 */
void mbedtls_sm2dsa_free( mbedtls_sm2dsa_context *ctx );

#ifdef __cplusplus
}
#endif

#endif /* sm2dsa.h */
