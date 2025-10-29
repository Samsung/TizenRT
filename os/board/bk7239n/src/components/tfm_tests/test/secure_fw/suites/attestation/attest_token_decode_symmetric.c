/*
 * attest_symmetric_iat_decode.c
 *
 * Copyright (c) 2019, Laurence Lundblade.
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "attest_token_decode.h"
#include "attest.h"
#include "psa/crypto.h"
#include "q_useful_buf.h"
#include "qcbor_util.h"
#include "t_cose_common.h"
#include "t_cose_mac0_verify.h"
#include "tfm_plat_crypto_keys.h"

/* Only support HMAC as MAC algorithm in COSE_Mac0 so far */
#define SYMMETRIC_IAK_MAX_SIZE        PSA_MAC_MAX_SIZE

#if DOMAIN_NS == 1U
/*
 * Public function. See attest_token_decode.h
 * It is not allowed to let NS side fetch the symmetric IAK and perform the MAC
 * verification.
 */
enum attest_token_err_t
attest_token_decode_validate_token(struct attest_token_decode_context *me,
                                   struct q_useful_buf_c               token)
{
    enum t_cose_err_t              t_cose_error;
    enum attest_token_err_t        return_value;
    /* Decode only without signature verification */
    int32_t                        t_cose_options = T_COSE_OPT_DECODE_ONLY;
    struct t_cose_mac0_verify_ctx  verify_ctx;
    struct t_cose_key              attest_key = T_COSE_NULL_KEY;

    t_cose_mac0_verify_init(&verify_ctx, t_cose_options);

    t_cose_mac0_set_verify_key(&verify_ctx, attest_key);

    t_cose_error = t_cose_mac0_verify(&verify_ctx,
                                      token, /* COSE to verify */
                                      &me->payload, /* Payload from token */
                                      NULL
                                     );

    return_value = map_t_cose_errors(t_cose_error);
    me->last_error = return_value;

    return return_value;
}
#else /* DOMAIN_NS == 1U */
/*
 * \note The symmetric Initial Attestation key (IAK) will be fetched for
 *       authentication tag verification in secure test cases.
 *       Authentication tag verification in tests is for debug purpose only.
 *       Do not fetch the IAK outside attestation service in real products.
 */
static inline enum attest_token_err_t
decode_register_verify_key(psa_key_handle_t *verify_key_handle)
{
    uint8_t key_buf[SYMMETRIC_IAK_MAX_SIZE];
    psa_algorithm_t key_alg;
    psa_key_handle_t key_handle;
    size_t key_len;
    enum tfm_plat_err_t plat_res;
    psa_status_t psa_res;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

    /* Get the symmetric initial attestation key for HMAC operation */
    plat_res = tfm_plat_get_symmetric_iak(key_buf, sizeof(key_buf),
                                          &key_len, &key_alg);
    if (plat_res != TFM_PLAT_ERR_SUCCESS) {
        return ATTEST_TOKEN_ERR_VERIFICATION_KEY;
    }

    /*
     * Verify if HMAC algorithm is valid.
     * According to COSE (RFC 8152), only SHA-256, SHA-384 and SHA-512 are
     * supported in HMAC.
     */
    if ((key_alg != PSA_ALG_HMAC(PSA_ALG_SHA_256)) && \
        (key_alg != PSA_ALG_HMAC(PSA_ALG_SHA_384)) && \
        (key_alg != PSA_ALG_HMAC(PSA_ALG_SHA_512))) {
        return ATTEST_TOKEN_ERR_VERIFICATION_KEY;
    }

    /* Setup the key attributes */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&key_attributes, key_alg);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);

    /* Register the symmetric key to Crypto service */
    psa_res = psa_import_key(&key_attributes, key_buf, key_len, &key_handle);
    if (psa_res != PSA_SUCCESS) {
        return ATTEST_TOKEN_ERR_VERIFICATION_KEY;
    }

    *verify_key_handle = key_handle;

    return ATTEST_TOKEN_ERR_SUCCESS;
}

static inline enum attest_token_err_t
decode_unregister_verify_key(psa_key_handle_t verify_key_handle)
{
    psa_status_t status;

    status = psa_destroy_key(verify_key_handle);
    if (status == PSA_SUCCESS) {
        return ATTEST_TOKEN_ERR_SUCCESS;
    }

    return ATTEST_TOKEN_ERR_GENERAL;
}

/*
 * Public function. See attest_token_decode.h
 * Decode the received COSE_Mac0 structure and verify the tag.
 */
enum attest_token_err_t
attest_token_decode_validate_token(struct attest_token_decode_context *me,
                                   struct q_useful_buf_c               token)
{
    enum t_cose_err_t              t_cose_error;
    enum attest_token_err_t        return_value;
    int32_t                        t_cose_options = 0;
    struct t_cose_mac0_verify_ctx  verify_ctx;
    struct t_cose_key              attest_key;
    psa_key_handle_t               key_handle;

    return_value = decode_register_verify_key(&key_handle);
    if (return_value != ATTEST_TOKEN_ERR_SUCCESS) {
        return return_value;
    }

    if (me->options & TOKEN_OPT_SHORT_CIRCUIT_SIGN) {
        t_cose_options |= T_COSE_OPT_ALLOW_SHORT_CIRCUIT;
    }

    t_cose_mac0_verify_init(&verify_ctx, t_cose_options);

    attest_key.crypto_lib = T_COSE_CRYPTO_LIB_PSA;
    attest_key.k.key_handle = (uint64_t)key_handle;
    t_cose_mac0_set_verify_key(&verify_ctx, attest_key);

    t_cose_error = t_cose_mac0_verify(&verify_ctx,
                                      token, /* COSE to verify */
                                      &me->payload, /* Payload from token */
                                      NULL);

    return_value = map_t_cose_errors(t_cose_error);
    me->last_error = return_value;

    decode_unregister_verify_key(key_handle);

    return return_value;
}
#endif /* DOMAIN_NS == 1U */
