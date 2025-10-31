/*
 * attest_token_encode.c
 *
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 * Copyright (c) 2020-2022, Arm Limited.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "attest_token.h"
#include "qcbor.h"
#ifdef SYMMETRIC_INITIAL_ATTESTATION
#include "t_cose_mac0_sign.h"
#else
#include "t_cose_sign1_sign.h"
#endif
#include "t_cose_common.h"
#include "q_useful_buf.h"
#include "psa/crypto.h"
#include "attest_key.h"


/**
 * \file attest_token_encode.c
 *
 * \brief Attestation token creation implementation
 */

/**
 * \brief Map t_cose error to attestation token error.
 *
 * \param[in] err   The t_cose error to map.
 *
 * \return the attestation token error.
 */
static enum attest_token_err_t t_cose_err_to_attest_err(enum t_cose_err_t err)
{
    switch(err) {

    case T_COSE_SUCCESS:
        return ATTEST_TOKEN_ERR_SUCCESS;

    case T_COSE_ERR_UNSUPPORTED_HASH:
        return ATTEST_TOKEN_ERR_HASH_UNAVAILABLE;

    case T_COSE_ERR_TOO_SMALL:
        return ATTEST_TOKEN_ERR_TOO_SMALL;

    default:
        /* A lot of the errors are not mapped because they are
         * primarily internal errors that should never happen. They
         * end up here.
         */
        return ATTEST_TOKEN_ERR_GENERAL;
    }
}

#ifdef SYMMETRIC_INITIAL_ATTESTATION
/*
 * Outline of token creation. Much of this occurs inside
 * t_cose_mac0_encode_parameters() and t_cose_mac0_encode_tag().
 *
 * - Create encoder context
 * - Open the CBOR array that hold the \c COSE_Mac0
 * - Write COSE Headers
 *   - Protected Header
 *      - Algorithm ID
 *   - Unprotected Headers
 *     - Key ID
 * - Open payload bstr
 *   - Write payload data, maybe lots of it
 *   - Get bstr that is the encoded payload
 * - Compute tag
 *   - Create a separate encoder context for \c MAC_structure
 *     - Encode CBOR context identifier
 *     - Encode protected headers
 *     - Encode an empty bstr for external_aad
 *     - Add one more empty bstr that is a "fake payload"
 *     - Close off \c MAC_structure
 *   - Call MAC API to compute the tag of all but "fake payload" of
 *     \c MAC_structure
 *   - Get payload bstr ptr and length
 *   - Update the real encoded payload into MAC operation
 *   - Complete MAC operation
 * - Write tag into the CBOR output
 * - Close CBOR array holding the \c COSE_Mac0
 */

/*
 * Public function. See attest_token.h
 */
enum attest_token_err_t
attest_token_encode_start(struct attest_token_encode_ctx *me,
                          uint32_t opt_flags,
                          int32_t key_select,
                          int32_t cose_alg_id,
                          const struct q_useful_buf *out_buf)
{
    psa_key_handle_t key_handle = 0;
    struct t_cose_key attest_key;
    enum psa_attest_err_t attest_ret;
    enum t_cose_err_t cose_ret;
    int32_t t_cose_options = 0;
    enum attest_token_err_t return_value = ATTEST_TOKEN_ERR_SUCCESS;
    struct q_useful_buf_c attest_key_id = NULL_Q_USEFUL_BUF_C;

    /* Remember some of the configuration values */
    me->opt_flags  = opt_flags;
    me->key_select = key_select;

    if (opt_flags & TOKEN_OPT_SHORT_CIRCUIT_SIGN) {
        t_cose_options |= T_COSE_OPT_SHORT_CIRCUIT_TAG;
    }

    t_cose_mac0_sign_init(&(me->mac_ctx), t_cose_options, cose_alg_id);

    attest_ret = attest_get_signing_key_handle(&key_handle);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        return ATTEST_TOKEN_ERR_SIGNING_KEY;
    }
    attest_key.crypto_lib = T_COSE_CRYPTO_LIB_PSA;
    attest_key.k.key_handle = (uint64_t)key_handle;

    attest_ret = attest_get_initial_attestation_key_id(&attest_key_id);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        return ATTEST_TOKEN_ERR_GENERAL;
    } else if (!attest_key_id.ptr || !attest_key_id.len) {
        /* In case kid value is invalid, set it to NULL */
        attest_key_id = NULL_Q_USEFUL_BUF_C;
    }

    t_cose_mac0_set_signing_key(&(me->mac_ctx),
                                attest_key,
                                attest_key_id);

    /* Spin up the CBOR encoder */
    QCBOREncode_Init(&(me->cbor_enc_ctx), *out_buf);

    /* This will cause the cose headers to be encoded and written into
     *  out_buf using me->cbor_enc_ctx
     */
    cose_ret = t_cose_mac0_encode_parameters(&(me->mac_ctx),
                                             &(me->cbor_enc_ctx));
    if (cose_ret != T_COSE_SUCCESS) {
        return_value = t_cose_err_to_attest_err(cose_ret);
    }

    QCBOREncode_OpenMap(&(me->cbor_enc_ctx));

    return return_value;
}

/*
 * Public function. See attest_token.h
 */
enum attest_token_err_t
attest_token_encode_finish(struct attest_token_encode_ctx *me,
                           struct q_useful_buf_c *completed_token)
{
    enum attest_token_err_t return_value = ATTEST_TOKEN_ERR_SUCCESS;
    /* The completed and tagged encoded COSE_Mac0 */
    struct q_useful_buf_c   completed_token_ub;
    QCBORError              qcbor_result;
    enum t_cose_err_t       cose_return_value;

    QCBOREncode_CloseMap(&(me->cbor_enc_ctx));

    /* -- Finish up the COSE_Mac0. This is where the MAC happens -- */
    cose_return_value = t_cose_mac0_encode_tag(&(me->mac_ctx),
                                               &(me->cbor_enc_ctx));
    if (cose_return_value) {
        /* Main errors are invoking the tagging */
        return_value = t_cose_err_to_attest_err(cose_return_value);
        goto Done;
    }

    /* Finally close off the CBOR formatting and get the pointer and length
     * of the resulting COSE_Mac0
     */
    qcbor_result = QCBOREncode_Finish(&(me->cbor_enc_ctx), &completed_token_ub);
    if (qcbor_result == QCBOR_ERR_BUFFER_TOO_SMALL) {
           return_value = ATTEST_TOKEN_ERR_TOO_SMALL;
    } else if (qcbor_result != QCBOR_SUCCESS) {
        /* likely from array not closed, too many closes, ... */
        return_value = ATTEST_TOKEN_ERR_CBOR_FORMATTING;
    } else {
        *completed_token = completed_token_ub;
    }

Done:
    return return_value;
}
#else /* SYMMETRIC_INITIAL_ATTESTATION */
/*
 * Outline of token creation. Much of this occurs inside
 * t_cose_sign1_encode_parameters() and t_cose_sign1_encode_signature().
 *
 * - Create encoder context
 * - Open the CBOR array that hold the \c COSE_Sign1
 * - Write COSE Headers
 *   - Protected Header
 *      - Algorithm ID
 *   - Unprotected Headers
 *     - Key ID
 * - Open payload bstr
 *   - Write payload data, maybe lots of it
 *   - Get bstr that is the encoded payload
 * - Compute signature
 *   - Create a separate encoder context for \c Sig_structure
 *     - Encode CBOR context identifier
 *     - Encode protected headers
 *     - Encode two empty bstr
 *     - Add one more empty bstr that is a "fake payload"
 *     - Close off \c Sig_structure
 *   - Hash all but "fake payload" of \c Sig_structure
 *   - Get payload bstr ptr and length
 *   - Continue hash of the real encoded payload
 *   - Run ECDSA
 * - Write signature into the CBOR output
 * - Close CBOR array holding the \c COSE_Sign1
 */

/*
 * Public function. See attest_token.h
 */
enum attest_token_err_t
attest_token_encode_start(struct attest_token_encode_ctx *me,
                          uint32_t opt_flags,
                          int32_t key_select,
                          int32_t cose_alg_id,
                          const struct q_useful_buf *out_buf)
{
    enum t_cose_err_t cose_ret;
    enum attest_token_err_t return_value = ATTEST_TOKEN_ERR_SUCCESS;
    enum psa_attest_err_t   attest_ret;
    int32_t                 t_cose_options = 0;
    struct t_cose_key attest_key;
    psa_key_handle_t private_key;
    struct q_useful_buf_c attest_key_id = NULL_Q_USEFUL_BUF_C;

    /* Remember some of the configuration values */
    me->opt_flags  = opt_flags;
    me->key_select = key_select;


    if (opt_flags & TOKEN_OPT_SHORT_CIRCUIT_SIGN) {
        t_cose_options |= T_COSE_OPT_SHORT_CIRCUIT_SIG;
    } else {
#ifdef INCLUDE_COSE_KEY_ID
        attest_ret = attest_get_initial_attestation_key_id(&attest_key_id);
        if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
            return ATTEST_TOKEN_ERR_GENERAL;
        }
#endif /* INCLUDE_COSE_KEY_ID */
    }

    t_cose_sign1_sign_init(&(me->signer_ctx), t_cose_options, cose_alg_id);

    attest_ret = attest_get_signing_key_handle(&private_key);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        return ATTEST_TOKEN_ERR_SIGNING_KEY;
    }
    attest_key.crypto_lib = T_COSE_CRYPTO_LIB_PSA;
    attest_key.k.key_handle = private_key;

    t_cose_sign1_set_signing_key(&(me->signer_ctx),
                                 attest_key,
                                 attest_key_id);

    /* Spin up the CBOR encoder */
    QCBOREncode_Init(&(me->cbor_enc_ctx), *out_buf);

    /* This will cause the cose headers to be encoded and written into
     *  out_buf using me->cbor_enc_ctx
     */
    cose_ret = t_cose_sign1_encode_parameters(&(me->signer_ctx),
                                              &(me->cbor_enc_ctx));
    if (cose_ret) {
        return_value = t_cose_err_to_attest_err(cose_ret);
    }

    QCBOREncode_OpenMap(&(me->cbor_enc_ctx));

    return return_value;
}

/*
 * Public function. See attest_token.h
 */
enum attest_token_err_t
attest_token_encode_finish(struct attest_token_encode_ctx *me,
                           struct q_useful_buf_c *completed_token)
{
    enum attest_token_err_t return_value = ATTEST_TOKEN_ERR_SUCCESS;
    /* The completed and signed encoded cose_sign1 */
    struct q_useful_buf_c   completed_token_ub;
    QCBORError              qcbor_result;
    enum t_cose_err_t       cose_return_value;

    QCBOREncode_CloseMap(&(me->cbor_enc_ctx));

    /* -- Finish up the COSE_Sign1. This is where the signing happens -- */
    cose_return_value = t_cose_sign1_encode_signature(&(me->signer_ctx),
                                                      &(me->cbor_enc_ctx));
    if (cose_return_value) {
        /* Main errors are invoking the hash or signature */
        return_value = t_cose_err_to_attest_err(cose_return_value);
        goto Done;
    }

    /* Finally close off the CBOR formatting and get the pointer and length
     * of the resulting COSE_Sign1
     */
    qcbor_result = QCBOREncode_Finish(&(me->cbor_enc_ctx), &completed_token_ub);
    if (qcbor_result == QCBOR_ERR_BUFFER_TOO_SMALL) {
           return_value = ATTEST_TOKEN_ERR_TOO_SMALL;
       } else if (qcbor_result != QCBOR_SUCCESS) {
           /* likely from array not closed, too many closes, ... */
           return_value = ATTEST_TOKEN_ERR_CBOR_FORMATTING;
       } else {
           *completed_token = completed_token_ub;
       }

Done:
        return return_value;
}
#endif /* SYMMETRIC_INITIAL_ATTESTATION */

/*
 * Public function. See attest_token.h
 */
QCBOREncodeContext *
attest_token_encode_borrow_cbor_cntxt(struct attest_token_encode_ctx *me)
{
    return &(me->cbor_enc_ctx);
}


/*
 * Public function. See attest_token.h
 */
void attest_token_encode_add_integer(struct attest_token_encode_ctx *me,
                                     int32_t label,
                                     int64_t Value)
{
    QCBOREncode_AddInt64ToMapN(&(me->cbor_enc_ctx), label, Value);
}


/*
 * Public function. See attest_token.h
 */
void attest_token_encode_add_bstr(struct attest_token_encode_ctx *me,
                                  int32_t label,
                                  const struct q_useful_buf_c *bstr)
{
    QCBOREncode_AddBytesToMapN(&(me->cbor_enc_ctx),
                               label,
                               *bstr);
}


/*
 * Public function. See attest_token.h
 */
void attest_token_encode_add_tstr(struct attest_token_encode_ctx *me,
                                  int32_t label,
                                  const struct q_useful_buf_c *tstr)
{
    QCBOREncode_AddTextToMapN(&(me->cbor_enc_ctx), label, *tstr);
}


/*
 * Public function. See attest_token.h
 */
void attest_token_encode_add_cbor(struct attest_token_encode_ctx *me,
                                  int32_t label,
                                  const struct q_useful_buf_c *encoded)
{
    QCBOREncode_AddEncodedToMapN(&(me->cbor_enc_ctx), label, *encoded);
}
