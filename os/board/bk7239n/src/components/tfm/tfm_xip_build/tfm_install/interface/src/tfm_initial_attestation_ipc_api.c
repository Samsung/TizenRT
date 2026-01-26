/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/initial_attestation.h"
#include "tfm_ns_interface.h"
#include "psa/client.h"
#include "psa/crypto_types.h"
#include "psa_manifest/sid.h"
#include "tfm_attest_defs.h"

psa_status_t
psa_initial_attest_get_token(const uint8_t *auth_challenge,
                             size_t         challenge_size,
                             uint8_t       *token_buf,
                             size_t         token_buf_size,
                             size_t        *token_size)
{
    psa_status_t status;

    psa_invec in_vec[] = {
        {auth_challenge, challenge_size}
    };
    psa_outvec out_vec[] = {
        {token_buf, token_buf_size}
    };

    status = psa_call(TFM_ATTESTATION_SERVICE_HANDLE, TFM_ATTEST_GET_TOKEN,
                      in_vec, IOVEC_LEN(in_vec),
                      out_vec, IOVEC_LEN(out_vec));

    if (status == PSA_SUCCESS) {
        *token_size = out_vec[0].len;
    }

    return status;
}

psa_status_t
psa_initial_attest_get_token_size(size_t  challenge_size,
                                  size_t *token_size)
{
    psa_status_t status;
    psa_invec in_vec[] = {
        {&challenge_size, sizeof(challenge_size)}
    };
    psa_outvec out_vec[] = {
        {token_size, sizeof(size_t)}
    };

    status = psa_call(TFM_ATTESTATION_SERVICE_HANDLE, TFM_ATTEST_GET_TOKEN_SIZE,
                      in_vec, IOVEC_LEN(in_vec),
                      out_vec, IOVEC_LEN(out_vec));

    return status;
}
