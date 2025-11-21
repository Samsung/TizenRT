/*
 *  t_cose_sign1_verify.h
 *
 * Copyright 2019, Laurence Lundblade
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */


#ifndef __T_COSE_SIGN1_VERIFY_H__
#define __T_COSE_SIGN1_VERIFY_H__

#include <stdint.h>
#include "q_useful_buf.h"
#include "t_cose_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file t_cose_sign1_verify.h
 *
 * \brief Verify a COSE_Sign1 Message
 *
 * This verifies a \c COSE_Sign1 message in compliance with [COSE (RFC 8152)]
 * (https://tools.ietf.org/html/rfc8152). A \c COSE_Sign1 message is a CBOR
 * encoded binary blob that contains header parameters, a payload and a
 * signature. Usually the signature is made with an EC signing
 * algorithm like ECDSA.
 *
 * This implementation is intended to be small and portable to
 * different OS's and platforms. Its dependencies are:
 * - [QCBOR](https://github.com/laurencelundblade/QCBOR)
 * - <stdint.h>, <string.h>, <stddef.h>
 * - Hash functions like SHA-256
 * - Signing functions like ECDSA
 *
 * There is a cryptographic adaptation layer defined in
 * t_cose_crypto.h.  An implementation can be made of the functions in
 * it for different cryptographic libraries. This means that different
 * integrations with different cryptographic libraries may support
 * only signing with a particular set of algorithms. Integration with
 * [OpenSSL](https://www.openssl.org) is supported.  Key ID look up
 * also varies by different cryptographic library integrations.
 *
 * See t_cose_common.h for preprocessor defines to reduce object code
 * and stack use by disabling features.
 */


/**
 * Context for signature verification.  It is about 24 bytes on a
 * 64-bit machine and 12 bytes on a 32-bit machine.
 */
struct t_cose_sign1_verify_ctx {
    /* Private data structure */
    struct t_cose_key     verification_key;
    int32_t               option_flags;
};


/**
 * \brief Initialize for \c COSE_Sign1 message verification.
 *
 * \param[in,out]  context       The context to initialize.
 * \param[in]      option_flags  Options controlling the verification.
 *
 * This must be called before using the verification context.
 */
static void
t_cose_sign1_verify_init(struct t_cose_sign1_verify_ctx *context,
                         int32_t                         option_flags);


/**
 * \brief Set key for \c COSE_Sign1 message verification.
 *
 * \param[in] verification_key  The verification key to use.
 *
 * There are four main ways that the verification key is found and
 * supplied to t_cose so that t_cose_sign1_verify() succeeds.
 *
 * -# Look up by kid parameter and set by t_cose_sign1_set_verification_key()
 * -# Look up by other and set by t_cose_sign1_set_verification_key()
 * -# Determination by kid that short circuit signing is used (test only)
 * -# Look up by kid parameter in cryptographic adaptation  layer
 *
 * Note that there is no means where certificates, like X.509
 * certificates, are provided in the COSE parameters. Perhaps there
 * will be in the future but that is not in common use or supported by
 * this implementation.
 *
 * To use 1 it is necessary to call t_cose_sign1_verify_init() and
 * t_cose_sign1_verify() twice.  The first time
 * t_cose_sign1_verify_init() is called, give the \ref
 * T_COSE_OPT_DECODE_ONLY option.  Then call t_cose_sign1_verify() and
 * the kid will be returned in \c parameters. The caller finds the kid on
 * their own. Then call this to set the key. Last call
 * t_cose_sign1_verify(), again without the \ref T_COSE_OPT_DECODE_ONLY
 * option.
 *
 * To use 2 the key is somehow determined without the kid and
 * t_cose_sign1_set_verification_key() is called with it. Then
 * t_cose_sign1_verify() is called. Note that this implementation
 * cannot return non-standard header parameters, at least not yet.
 *
 * To use 3, initialize with \ref T_COSE_OPT_ALLOW_SHORT_CIRCUIT.  No
 * call to t_cose_sign1_set_verification_key() is necessary. If you do
 * call t_cose_sign1_set_verification_key(), the kid for short circuit
 * signing will be recognized and the set key will be ignored.
 *
 * To use 4, first be sure that the cryptographic adapter supports
 * look up by kid.  There's no API to determine this, so it is
 * probably determined by other system documentation (aka source
 * code).  In this mode, all that is necessary is to call
 * t_cose_sign1_verify().
 *
 * 3 always works no matter what is done in the cryptographic
 * adaptation layer because it never calls out to it. The OpenSSL
 * adaptor supports 1 and 2.
 */
static void
t_cose_sign1_set_verification_key(struct t_cose_sign1_verify_ctx *context,
                                  struct t_cose_key               verification_key);


/**
 * \brief Verify a COSE_Sign1
 *
 * \param[in] sign1         Pointer and length of CBOR encoded \c COSE_Sign1
 *                          message that is to be verified.
 * \param[out] payload      Pointer and length of the payload.
 * \param[out] parameters   Place to return parsed parameters. Maybe be \c NULL.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * See t_cose_sign1_set_verification_key() for discussion on where
 * the verification key comes from.
 *
 * Verification involves the following steps.
 *
 * - The CBOR-format COSE_Sign1 structure is parsed. It makes sure \c sign1
 * is valid CBOR and follows the required structure for \c COSE_Sign1.
 *
 * - The protected header parameters are parsed, particular the algorithm id.
 *
 * - The unprotected headers parameters are parsed, particularly the kid.
 *
 * - The payload is identified. The internals of the payload are not parsed.
 *
 * - The expected hash, the "to-be-signed" bytes are computed. The hash
 * algorithm to use comes from the signing algorithm. If the algorithm is
 * not known or not supported this will error out.
 *
 * - Finally, the signature verification is performed.
 *
 * If it is successful, the pointer to the CBOR-encoded payload is
 * returned. The parameters are returned if requested. All pointers
 * returned are to memory in the \c sign1 passed in.
 *
 * Note that this only handles standard COSE header parameters. There are no
 * facilities for custom header parameters, even though they are allowed by
 * the COSE standard.
 *
 * This will recognize the special key ID for short-circuit signing
 * and verify it if the \ref T_COSE_OPT_ALLOW_SHORT_CIRCUIT is set.
 *
 * Indefinite length CBOR strings are not supported by this
 * implementation.  \ref T_COSE_ERR_SIGN1_FORMAT will be returned if
 * they are in the input \c COSE_Sign1 messages. For example, if the
 * payload is an indefinite length byte string, this error will be
 * returned.
 */
enum t_cose_err_t t_cose_sign1_verify(struct t_cose_sign1_verify_ctx *context,
                                      struct q_useful_buf_c           sign1,
                                      struct q_useful_buf_c          *payload,
                                      struct t_cose_parameters       *parameters);




/* ------------------------------------------------------------------------
 * Inline implementations of public functions defined above.
 */
static inline void
t_cose_sign1_verify_init(struct t_cose_sign1_verify_ctx *me,
                         int32_t                option_flags)
{
    me->option_flags = option_flags;
    me->verification_key = T_COSE_NULL_KEY;
}


static inline void
t_cose_sign1_set_verification_key(struct t_cose_sign1_verify_ctx *me,
                                  struct t_cose_key               verification_key)
{
    me->verification_key = verification_key;
}
#endif /* __T_COSE_SIGN1_VERIFY_H__ */
