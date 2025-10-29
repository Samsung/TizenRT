/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ATTEST_KEY_H__
#define __ATTEST_KEY_H__

#include "attest.h"
#include "psa/initial_attestation.h"
#include "psa/crypto.h"
#include "q_useful_buf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Register the initial attestation private key to Crypto service. Loads
 *        the public key if the key has not already been loaded.
 *
 * \note  Private key MUST be present on the device, otherwise initial
 *        attestation token cannot be signed.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Key(s) was registered.
 * \retval  PSA_ATTEST_ERR_GENERAL   Key(s) could not be registered.
 */
enum psa_attest_err_t
attest_register_initial_attestation_key();

/**
 * \brief Unregister the initial attestation private key from Crypto service
 *        to do not occupy key slot.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Key(s) was unregistered.
 * \retval  PSA_ATTEST_ERR_GENERAL   Key(s) could not be unregistered.
 */
enum psa_attest_err_t
attest_unregister_initial_attestation_key();

/**
 * \brief Get the handle of the key for signing token
 *        In asymmetric key algorithm based initial attestation, it is the
 *        handle of the initial attestation private key.
 *        In symmetric key algorithm based initial attestation, it is the
 *        handle of symmetric initial attestation key.
 *
 * \param[out] key_handle            The handle of the key for signing token.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Key handle was successfully returned.
 * \retval  PSA_ATTEST_ERR_GENERAL   Key handle could not be returned.
 */
enum psa_attest_err_t
attest_get_signing_key_handle(psa_key_handle_t *key_handle);

/**
 * \brief Get the buffer of Instance ID data
 *
 * \param[out] id_buf  Address and length of Instance ID buffer
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS            Instance ID was successfully
 *                                            returned.
 * \retval  PSA_ATTEST_ERR_CLAIM_UNAVAILABLE  Instance ID is unavailable
 * \retval  PSA_ATTEST_ERR_GENERAL            Instance ID could not be returned.
 */
enum psa_attest_err_t
attest_get_instance_id(struct q_useful_buf_c *id_buf);

#ifdef INCLUDE_COSE_KEY_ID
/**
 * \brief Get the attestation key ID.
 *        In asymmetric key algorithm based Initial Attestation, it is the hash
 *        (SHA256) of the COSE_Key encoded attestation public key.
 *        In symmetric key algorithm based Initial Attestation, the key ID raw
 *        data is fetched from from device.
 *
 * \param[out] attest_key_id  Pointer and length of the key id.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Got key id successfully.
 * \retval  PSA_ATTEST_ERR_GENERAL   Failed to get key id.

 */
enum psa_attest_err_t
attest_get_initial_attestation_key_id(struct q_useful_buf_c *attest_key_id);
#else /* INCLUDE_COSE_KEY_ID */
static inline enum psa_attest_err_t
attest_get_initial_attestation_key_id(struct q_useful_buf_c *attest_key_id)
{
    (void)attest_key_id;

    return PSA_ATTEST_ERR_SUCCESS;
}
#endif /* INCLUDE_COSE_KEY_ID */

#ifdef __cplusplus
}
#endif

#endif /* __ATTEST_KEY_H__ */
