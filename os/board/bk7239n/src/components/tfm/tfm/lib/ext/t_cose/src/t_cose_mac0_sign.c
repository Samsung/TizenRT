/*
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "qcbor.h"
#include "t_cose_crypto.h"
#include "t_cose_mac0_sign.h"
#include "t_cose_util.h"

/**
 * \file t_cose_mac0_sign.c
 *
 * \brief This creates t_cose Mac authentication structure without a recipient
 *        structure.
 *        Only HMAC is supported so far.
 */

/**
 * \brief  Makes the protected header parameters for COSE.
 *
 * \param[in] cose_algorithm_id      The COSE algorithm ID to put in the
 *                                   header parameters.
 * \param[in] buffer_for_parameters  Pointer and length of buffer into which
 *                                   the resulting encoded protected
 *                                   parameters is put. See return value.
 *
 * \return   The pointer and length of the encoded protected
 *           parameters is returned, or \c NULL_Q_USEFUL_BUF_C if this fails.
 *           This will have the same pointer as \c buffer_for_parameters,
 *           but the pointer is conts and the length is that of the valid
 *           data, not of the size of the buffer.
 *
 * The protected parameters are returned in fully encoded CBOR format as
 * they are added to the \c COSE_Mac0 message as a binary string. This is
 * different from the unprotected parameters which are not handled this
 * way.
 *
 * This returns \c NULL_Q_USEFUL_BUF_C if buffer_for_parameters was too
 * small. See also definition of \c T_COSE_MAC0_MAX_SIZE_PROTECTED_PARAMETERS
 */
static inline struct q_useful_buf_c
encode_protected_parameters(int32_t             cose_algorithm_id,
                            struct q_useful_buf buffer_for_parameters)
{
    /* approximate stack use on 32-bit machine:
     *   CBOR encode context 148
     *   local use: 20
     *   total: 168
     */
    struct q_useful_buf_c protected_parameters;
    QCBORError            qcbor_result;
    QCBOREncodeContext    cbor_encode_ctx;
    struct q_useful_buf_c return_value;

    QCBOREncode_Init(&cbor_encode_ctx, buffer_for_parameters);
    QCBOREncode_OpenMap(&cbor_encode_ctx);
    QCBOREncode_AddInt64ToMapN(&cbor_encode_ctx,
                               COSE_HEADER_PARAM_ALG,
                               cose_algorithm_id);
    QCBOREncode_CloseMap(&cbor_encode_ctx);
    qcbor_result = QCBOREncode_Finish(&cbor_encode_ctx, &protected_parameters);

    if(qcbor_result == QCBOR_SUCCESS) {
        return_value = protected_parameters;
    } else {
        return_value = NULL_Q_USEFUL_BUF_C;
    }

    return return_value;
}

/**
 * \brief Add the unprotected parameters to a CBOR encoding context
 *
 * \param[in] me               The t_cose signing context.
 * \param[in] kid              The key ID.
 * \param[in] cbor_encode_ctx  CBOR encoding context to output to
 *
 * No error is returned. If an error occurred it will be returned when
 * \c QCBOR_Finish() is called on \c cbor_encode_ctx.
 *
 * The unprotected parameters added by this are the kid and content type.
 */
static inline enum t_cose_err_t
add_unprotected_parameters(const struct t_cose_mac0_sign_ctx *me,
                           const struct q_useful_buf_c        kid,
                           QCBOREncodeContext                *cbor_encode_ctx)
{
    QCBOREncode_OpenMap(cbor_encode_ctx);

    if(!q_useful_buf_c_is_null_or_empty(kid)) {
        QCBOREncode_AddBytesToMapN(cbor_encode_ctx,
                                   COSE_HEADER_PARAM_KID,
                                   kid);
    }

#ifndef T_COSE_DISABLE_CONTENT_TYPE
    if(me->content_type_uint != T_COSE_EMPTY_UINT_CONTENT_TYPE &&
       me->content_type_tstr != NULL) {
        /* Both the string and int content types are not allowed */
        return T_COSE_ERR_DUPLICATE_PARAMETER;
    }

    if(me->content_type_uint != T_COSE_EMPTY_UINT_CONTENT_TYPE) {
        QCBOREncode_AddUInt64ToMapN(cbor_encode_ctx,
                                    COSE_HEADER_PARAM_CONTENT_TYPE,
                                    me->content_type_uint);
    }

    if(me->content_type_tstr != NULL) {
        QCBOREncode_AddSZStringToMapN(cbor_encode_ctx,
                                      COSE_HEADER_PARAM_CONTENT_TYPE,
                                      me->content_type_tstr);
    }
#else
    (void)me; /* avoid unused parameter warning */
#endif

    QCBOREncode_CloseMap(cbor_encode_ctx);

    return T_COSE_SUCCESS;
}

#ifndef T_COSE_DISABLE_SHORT_CIRCUIT_SIGN
/**
 * \brief Create a short-circuit tag
 *
 * \param[in] cose_alg_id  Algorithm ID. This is used only to make
 *                         the short-circuit tag the same size as the
 *                         real tag would be for the particular algorithm.
 * \param[in] header       The Header of COSE_Mac0.
 * \param[in] payload      The payload of COSE_Mac0
 * \param[in] tag_buffer   Pointer and length of buffer into which
 *                         the resulting tag is put.
 * \param[out] tag         Pointer and length of the tag returned.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * This creates the short-circuit tag that is actually a hash of input bytes.
 * This is a test mode only has it has no security value. This is retained in
 * commercial production code as a useful test or demo that can run
 * even if key material is not set up or accessible.
 */
static inline enum t_cose_err_t
short_circuit_tag(int32_t               cose_alg_id,
                  struct q_useful_buf_c header,
                  struct q_useful_buf_c payload,
                  struct q_useful_buf   tag_buffer,
                  struct q_useful_buf_c *tag)
{
    /* approximate stack use on 32-bit machine: local use: 16 bytes */
    enum t_cose_err_t         return_value;
    struct t_cose_crypto_hash hash_ctx;
    size_t                    tag_size;
    int32_t                   hash_alg_id;

    /*
     * The length of Hash result equals that of HMAC result
     * with the same Hash algorithm.
     */
    tag_size = t_cose_tag_size(cose_alg_id);

    /* Check the tag length against buffer size */
    if(tag_size == INT32_MAX) {
        return_value = T_COSE_ERR_UNSUPPORTED_SIGNING_ALG;
        goto Done;
    }

    if(tag_size > tag_buffer.len) {
        /* Buffer too small for this tag */
        return_value = T_COSE_ERR_SIG_BUFFER_SIZE;
        goto Done;
    }

    hash_alg_id = t_cose_hmac_to_hash_alg_id(cose_alg_id);
    if(hash_alg_id == INT32_MAX) {
        return_value = T_COSE_ERR_UNSUPPORTED_SIGNING_ALG;
        goto Done;
    }

    return_value = t_cose_crypto_hash_start(&hash_ctx, hash_alg_id);
    if(return_value != T_COSE_SUCCESS) {
        goto Done;
    }

    /* Hash the Header */
    t_cose_crypto_hash_update(&hash_ctx, q_useful_buf_head(header, header.len));

    /* Hash the payload */
    t_cose_crypto_hash_update(&hash_ctx, payload);

    return_value = t_cose_crypto_hash_finish(&hash_ctx, tag_buffer, tag);

Done:
    return return_value;
}
#endif /* T_COSE_DISABLE_SHORT_CIRCUIT_SIGN */

/*
 * Public function. See t_cose_mac0.h
 */
enum t_cose_err_t
t_cose_mac0_encode_parameters(struct t_cose_mac0_sign_ctx *me,
                              QCBOREncodeContext          *cbor_encode_ctx)

{
    size_t                       tag_len;
    enum t_cose_err_t            return_value;
    struct q_useful_buf          buffer_for_protected_parameters;
    struct q_useful_buf_c        kid;

    /*
     * Check the algorithm now by getting the algorithm as an early
     * error check even though it is not used until later.
     */
    tag_len = t_cose_tag_size(me->cose_algorithm_id);
    if(tag_len == INT32_MAX) {
        return T_COSE_ERR_UNSUPPORTED_SIGNING_ALG;
    }

    /* Add the CBOR tag indicating COSE_Mac0 */
    if(!(me->option_flags & T_COSE_OPT_OMIT_CBOR_TAG)) {
        QCBOREncode_AddTag(cbor_encode_ctx, CBOR_TAG_COSE_MAC0);
    }

    /* Get started with the tagged array that holds the parts of
     * a COSE_Mac0 message
     */
    QCBOREncode_OpenArray(cbor_encode_ctx);

    /* The protected headers, which are added as a wrapped bstr  */
    buffer_for_protected_parameters =
        Q_USEFUL_BUF_FROM_BYTE_ARRAY(me->protected_parameters_buffer);
    me->protected_parameters =
        encode_protected_parameters(me->cose_algorithm_id,
                                    buffer_for_protected_parameters);
    if(q_useful_buf_c_is_null(me->protected_parameters)) {
        /* The sizing of storage for protected headers is
         * off (should never happen in tested, released code)
         */
        return_value = T_COSE_ERR_MAKING_PROTECTED;
        goto Done;
    }
    /* The use of _AddBytes here achieves the bstr wrapping */
    QCBOREncode_AddBytes(cbor_encode_ctx, me->protected_parameters);

    /* The Unprotected parameters */
    if(me->option_flags & T_COSE_OPT_SHORT_CIRCUIT_TAG) {
#ifndef T_COSE_DISABLE_SHORT_CIRCUIT_SIGN
        kid = NULL_Q_USEFUL_BUF_C;
#else
        return_value = T_COSE_ERR_SHORT_CIRCUIT_SIG_DISABLED;
        goto Done;
#endif
    } else {
        /* Get the kid because it goes into the parameters that are about
         * to be made.
         */
        kid = me->kid;
    }

    return_value = add_unprotected_parameters(me, kid, cbor_encode_ctx);
    if(return_value != T_COSE_SUCCESS) {
        goto Done;
    }

    QCBOREncode_BstrWrap(cbor_encode_ctx);

    /*
     * Any failures in CBOR encoding will be caught in finish
     * when the CBOR encoding is closed off. No need to track
     * here as the CBOR encoder tracks it internally.
     */

Done:
    return return_value;
}

/*
 * Public function. See t_cose_mac0.h
 */
enum t_cose_err_t
t_cose_mac0_encode_tag(struct t_cose_mac0_sign_ctx *me,
                       QCBOREncodeContext           *cbor_encode_ctx)

{
    enum t_cose_err_t            return_value;
    QCBORError                   cbor_err;
    /* Pointer and length of the completed tag */
    struct q_useful_buf_c        tag;
    /* Buffer for the actual tag */
    Q_USEFUL_BUF_MAKE_STACK_UB(  tag_buf,
                                 T_COSE_CRYPTO_HMAC_TAG_MAX_SIZE);
    struct q_useful_buf_c        tbm_first_part;
    /* Buffer for the ToBeMaced */
    UsefulBuf_MAKE_STACK_UB(     tbm_first_part_buf,
                                 T_COSE_SIZE_OF_TBM);
    struct t_cose_crypto_hmac    hmac_ctx;
    struct q_useful_buf_c        maced_payload;

    QCBOREncode_CloseBstrWrap(cbor_encode_ctx, &maced_payload);

    /* Check that there are no CBOR encoding errors before proceeding
     * with hashing and tagging. This is not actually necessary as the
     * errors will be caught correctly later, but it does make it a
     * bit easier for the caller to debug problems.
     */
    cbor_err = QCBOREncode_GetErrorState(cbor_encode_ctx);
    if(cbor_err == QCBOR_ERR_BUFFER_TOO_SMALL) {
        return_value = T_COSE_ERR_TOO_SMALL;
        goto Done;
    } else if(cbor_err != QCBOR_SUCCESS) {
        return_value = T_COSE_ERR_CBOR_FORMATTING;
        goto Done;
    }

    if(QCBOREncode_IsBufferNULL(cbor_encode_ctx)) {
        /* Just calculating sizes. All that is needed is the tag size. */
        tag.ptr = NULL;
        tag.len = t_cose_tag_size(me->cose_algorithm_id);

        return_value = T_COSE_SUCCESS;
        goto CloseArray;
    }

    /* Create the hash of the ToBeMaced bytes. Inputs to the
     * MAC are the protected parameters, the payload that is
     * getting MACed.
     */
    return_value = create_tbm(tbm_first_part_buf,
                              me->protected_parameters,
                              &tbm_first_part,
                              T_COSE_TBM_PAYLOAD_IS_BSTR_WRAPPED,
                              maced_payload);
    if(return_value) {
        goto Done;
    }

    /*
     * Start the HMAC.
     * Calculate the tag of the first part of ToBeMaced and the wrapped
     * payload, to save a bigger buffer containing the entire ToBeMaced.
     *
     * Short-circuit tagging is invoked if requested. It does no HMAC operation
     * and requires no key. It is just a test mode that works without accessing
     * any device asset.
     */
    if(me->option_flags & T_COSE_OPT_SHORT_CIRCUIT_TAG) {
#ifndef T_COSE_DISABLE_SHORT_CIRCUIT_SIGN
        /* Short-circuit tag. Hash is used to generated tag instead of HMAC */
        return_value = short_circuit_tag(me->cose_algorithm_id,
                                         tbm_first_part,
                                         maced_payload,
                                         tag_buf,
                                         &tag);
        if(return_value) {
            goto Done;
        }

        goto CloseArray;
#else
        return_value = T_COSE_ERR_SHORT_CIRCUIT_SIG_DISABLED;
        goto Done;
#endif
    }

    return_value = t_cose_crypto_hmac_sign_setup(&hmac_ctx,
                                                 me->signing_key,
                                                 me->cose_algorithm_id);
    if(return_value) {
        goto Done;
    }

    /* Compute the tag of the first part. */
    return_value = t_cose_crypto_hmac_update(&hmac_ctx,
                                             q_useful_buf_head(tbm_first_part,
                                                           tbm_first_part.len));
    if(return_value) {
        goto Done;
    }

    /*
     * It is assumed that the context payload has been wrapped in a byte
     * string in CBOR format.
     */
    return_value = t_cose_crypto_hmac_update(&hmac_ctx, maced_payload);
    if(return_value) {
        goto Done;
    }

    return_value = t_cose_crypto_hmac_sign_finish(&hmac_ctx, tag_buf, &tag);
    if(return_value) {
        goto Done;
    }

CloseArray:
    /* Add tag to CBOR and close out the array */
    QCBOREncode_AddBytes(cbor_encode_ctx, tag);
    QCBOREncode_CloseArray(cbor_encode_ctx);

    /* CBOR encoding errors are tracked in the CBOR encoding context
     * and handled in the layer above this
     */

Done:
    return return_value;
}
