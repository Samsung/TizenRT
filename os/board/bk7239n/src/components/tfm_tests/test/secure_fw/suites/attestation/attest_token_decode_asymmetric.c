/*
 * attest_token_decode.c
 *
 * Copyright (c) 2019, Laurence Lundblade.
 * Copyright (c) 2020, Arm Limited.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "attest_token_decode.h"
#include "t_cose_sign1_verify.h"
#include "q_useful_buf.h"
#include "qcbor_util.h"
#include "psa/crypto.h"
#include "attest_public_key.h"
#include "attest.h"

/**
 * \file attest_token_decode.c
 *
 * \brief Attestation token decoder for COSE_Sign1.
 */

/*
 * Public function. See attest_token_decode.h
 */
enum attest_token_err_t
attest_token_decode_validate_token(struct attest_token_decode_context *me,
                                   struct q_useful_buf_c token)
{
    enum t_cose_err_t              t_cose_error;
    enum attest_token_err_t        return_value;
    enum psa_attest_err_t          attest_ret;
    int32_t                        t_cose_options = 0;
    struct t_cose_sign1_verify_ctx verify_ctx;
    struct t_cose_key              attest_key;
    psa_key_handle_t               public_key;

    /* Run the signature verification */
    if(me->options & TOKEN_OPT_SHORT_CIRCUIT_SIGN) {
        t_cose_options |= T_COSE_OPT_ALLOW_SHORT_CIRCUIT;
    }
    t_cose_sign1_verify_init(&verify_ctx, t_cose_options);

    attest_ret = attest_register_initial_attestation_public_key(&public_key);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        return ATTEST_TOKEN_ERR_VERIFICATION_KEY;
    }

    attest_key.crypto_lib = T_COSE_CRYPTO_LIB_PSA;
    attest_key.k.key_handle = public_key;

    t_cose_sign1_set_verification_key(&verify_ctx, attest_key);

    t_cose_error =  t_cose_sign1_verify(&verify_ctx,
                                        token, /* COSE to verify */
                                        &me->payload, /* Payload from token */
                                        NULL); /* Don't return parameters */

    return_value = map_t_cose_errors(t_cose_error);
    me->last_error = return_value;

    attest_ret = attest_unregister_initial_attestation_public_key(public_key);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        return ATTEST_TOKEN_ERR_GENERAL;
    }

    return return_value;
}
