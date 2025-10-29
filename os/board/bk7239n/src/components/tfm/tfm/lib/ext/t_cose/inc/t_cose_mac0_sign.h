/*
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 * Copyright (c) 2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __T_COSE_MAC0_SIGN_H_
#define __T_COSE_MAC0_SIGN_H_

#include <stdint.h>
#include "qcbor.h"
#include "t_cose_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * This selects a test mode called _short_ _circuit_ _tagging_.
 * This mode is useful when the symmetric key is unavailable
 * or unable to be accessed, perhaps because it has not been provisioned or
 * configured for the particular device.
 *
 * It has no value for security at all. Data signed this way MUST NOT
 * be trusted as anyone can sign like this.
 *
 * In this mode, the tag is the hash of that which would normally be MACed by
 * a symmetric key.
 *
 * This mode is very useful for testing because all the code except
 * the actual MAC algorithm is run exactly as it would if a proper
 * MAC algorithm was run. This can be used for end-end system
 * testing all the way to a server or relying party, not just for
 * testing device code as t_cose_mac0_verify() supports it too.
 */
#define T_COSE_OPT_SHORT_CIRCUIT_TAG 0x00000004


/**
 * This is the context for creating a \c COSE_Mac0 structure. The caller
 * should allocate it and pass it to the functions here.  This is
 * about 32 bytes so it fits easily on the stack.
 */
struct t_cose_mac0_sign_ctx {
    /* Private data structure */
    uint8_t                protected_parameters_buffer[
                                    T_COSE_MAC0_MAX_SIZE_PROTECTED_PARAMETERS];
    struct q_useful_buf_c  protected_parameters; /* The encoded protected parameters */
    int32_t                cose_algorithm_id;
    struct t_cose_key      signing_key;
    int32_t                option_flags;
    struct q_useful_buf_c  kid;
#ifndef T_COSE_DISABLE_CONTENT_TYPE
    uint32_t               content_type_uint;
    const char            *content_type_tstr;
#endif
};


/**
 * \brief  Initialize to start creating a \c COSE_Mac0.
 *
 * \param[in] context            The t_cose signing context.
 * \param[in] option_flags       One of \c T_COSE_OPT_XXXX.
 * \param[in] cose_algorithm_id  The algorithm to generate the authentication
 *                               tag, for example
 *                               \ref T_COSE_ALGORITHM_HMAC256.
 *
 * Initialize the \ref t_cose_mac0_sign_ctx context. Typically, no
 * \c option_flags are needed and 0 is passed. A \c cose_algorithm_id
 * must always be given. See \ref T_COSE_OPT_SHORT_CIRCUIT_TAG and
 * related for possible option flags.
 *
 * The algorithm ID space is from
 * [COSE (RFC8152)](https://tools.ietf.org/html/rfc8152) and the
 * [IANA COSE Registry](https://www.iana.org/assignments/cose/cose.xhtml).
 * \ref T_COSE_ALGORITHM_HMAC256 is defined here for convenience.
 * So far, only HMAC is supported in \c COSE_Mac0.
 *
 * Errors such as the passing of an unsupported \c cose_algorithm_id
 * are reported when t_cose_mac0_encode_parameters() is called.
 */
static void
t_cose_mac0_sign_init(struct t_cose_mac0_sign_ctx *me,
                      int32_t                      option_flags,
                      int32_t                      cose_algorithm_id);

/**
 * \brief  Set the key and kid (key ID) for signing.
 *
 * \param[in] context      The t_cose signing context.
 * \param[in] signing_key  The signing key to use or \ref T_COSE_NULL_KEY.
 * \param[in] kid          COSE key ID parameter or \c NULL_Q_USEFUL_BUF_C.
 *
 * This needs to be called to set the signing key to use. The \c kid
 * may be omitted by giving \c NULL_Q_USEFUL_BUF_C.
 *
 * If short-circuit signing is used,
 * \ref T_COSE_OPT_SHORT_CIRCUIT_TAG, then this does not need to be
 * called.
 */
static void
t_cose_mac0_set_signing_key(struct t_cose_mac0_sign_ctx *context,
                            struct t_cose_key            signing_key,
                            struct q_useful_buf_c        kid);

/**
 * \brief  Output first part and parameters for a \c COSE_Mac0 message.
 *
 * \param[in] context          The t_cose signing context.
 * \param[in] cbor_encode_ctx  Encoding context to output to.
 *
 * t_cose_mac0_sign_init() and t_cose_mac0_set_signing_key() must be
 * called before calling this.
 *
 * When this is called, the opening parts of the \c COSE_Mac0 message
 * are output to the \c cbor_encode_ctx.
 *
 * After this is called, the CBOR-formatted payload must be written to
 * the \c cbor_encode_ctx by calling all the various
 * \c QCBOREncode_AddXxx calls. It can be as simple or complex as needed.
 *
 * To complete the \c COSE_Mac0 call t_cose_mac0_encode_tag().
 *
 * The \c cbor_encode_ctx must have been initialized with an output
 * buffer to hold the \c COSE_Mac0 header parameters, the payload and the
 * signature.
 *
 * This and t_cose_mac0_encode_tag() can be used to calculate
 * the size of the \c COSE_Mac0 in the way \c QCBOREncode is usually
 * used to calculate sizes. In this case the \c t_cose_mac0_sign_ctx must
 * be initialized with the options, algorithm, key and kid just as
 * normal as these are needed to calculate the size. Then set up the
 * QCBOR encoder context with a \c NULL pointer and large length like
 * \c UINT32_MAX.  Call t_cose_mac0_encode_parameters(), then format
 * the payload into the encoder context, then call
 * t_cose_mac0_encode_tag().  Finally call \c
 * QCBOREncode_FinishGetSize() to get the length.
 */
enum t_cose_err_t
t_cose_mac0_encode_parameters(struct t_cose_mac0_sign_ctx *context,
                              QCBOREncodeContext          *cbor_encode_ctx);

/**
 * \brief Finish a \c COSE_Mac0 message by outputting the authentication tag.
 *
 * \param[in] context          The t_cose signing context.
 * \param[in] cbor_encode_ctx  Encoding context to output to.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * Call this to complete creation of a tagged \c COSE_Mac0 started
 * with t_cose_mac0_encode_parameters().
 *
 * This is when the cryptographic MAC algorithm is run.
 *
 * The completed \c COSE_Mac0 message is retrieved from the
 * \c cbor_encode_ctx by calling \c QCBOREncode_Finish().
 */
enum t_cose_err_t
t_cose_mac0_encode_tag(struct t_cose_mac0_sign_ctx *context,
                       QCBOREncodeContext          *cbor_encode_ctx);


#ifndef T_COSE_DISABLE_CONTENT_TYPE
/**
 * \brief Set the payload content type using CoAP content types.
 *
 * \param[in] context      The t_cose signing context.
 * \param[in] content_type The content type of the payload as defined
 *                         in the IANA CoAP Content-Formats registry.
 *
 * It is not allowed to have both a CoAP and MIME content type. This
 * error will show up when t_cose_mac0_encode_parameters() is called
 * as no error is returned by this function.
 *
 * The IANA CoAP Content-Formats registry is found
 * [here](https://www.iana.org/assignments/core-parameters/core-parameters.xhtml#content-formats).
 */
static inline void
t_cose_mac0_set_content_type_uint(struct t_cose_mac0_sign_ctx *context,
                                  uint16_t                     content_type);

/**
 * \brief Set the payload content type using MIME content types.
 *
 * \param[in] context      The t_cose signing context.
 * \param[in] content_type The content type of the payload as defined
 *                         in the IANA Media Types registry.

 *
 * It is not allowed to have both a CoAP and MIME content type. This
 * error will show up when t_cose_mac0_encode_parameters() is called.
 *
 * The IANA Media Types registry can be found
 * [here](https://www.iana.org/assignments/media-types/media-types.xhtml).
 * These have been known as MIME types in the past.
 */
static inline void
t_cose_mac0_set_content_type_tstr(struct t_cose_mac0_sign_ctx *context,
                                  const char                  *content_type);
#endif /* T_COSE_DISABLE_CONTENT_TYPE */

/* ------------------------------------------------------------------------
 * Inline implementations of public functions defined above.
 */
static inline void
t_cose_mac0_sign_init(struct t_cose_mac0_sign_ctx *me,
                      int32_t                      option_flags,
                      int32_t                      cose_algorithm_id)
{
    memset(me, 0, sizeof(*me));

#ifndef T_COSE_DISABLE_CONTENT_TYPE
    /* Only member for which 0 is not the empty state */
    me->content_type_uint = T_COSE_EMPTY_UINT_CONTENT_TYPE;
#endif

    me->cose_algorithm_id = cose_algorithm_id;
    me->option_flags      = option_flags;
}

static inline void
t_cose_mac0_set_signing_key(struct t_cose_mac0_sign_ctx *me,
                            struct t_cose_key            signing_key,
                            struct q_useful_buf_c        kid)
{
    me->kid         = kid;
    me->signing_key = signing_key;
}


#ifndef T_COSE_DISABLE_CONTENT_TYPE
static inline void
t_cose_mac0_set_content_type_uint(struct t_cose_mac0_sign_ctx *me,
                                  uint16_t                     content_type)
{
    me->content_type_uint = content_type;
}

static inline void
t_cose_mac0_set_content_type_tstr(struct t_cose_mac0_sign_ctx *me,
                                  const char                  *content_type)
{
    me->content_type_tstr = content_type;
}
#endif /* T_COSE_DISABLE_CONTENT_TYPE */

#ifdef __cplusplus
}
#endif

#endif /* __T_COSE_MAC0_SIGN_H_ */
