/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "tfm_mbedcrypto_include.h"

#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"
#include "tfm_crypto_private.h"

/*!
 * \defgroup public_psa Public functions, PSA
 *
 */

/*!@{*/
psa_status_t tfm_crypto_mac_sign_setup(psa_invec in_vec[],
                                       size_t in_len,
                                       psa_outvec out_vec[],
                                       size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;
    psa_mac_operation_t *operation = NULL;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 1, 1);

    if ((out_vec[0].len != sizeof(uint32_t)) ||
        (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    psa_key_id_t key_id = iov->key_id;
    psa_algorithm_t alg = iov->alg;
    mbedtls_svc_key_id_t encoded_key;

    /* Init the handle in the operation with the one passed from the iov */
    *handle_out = iov->op_handle;

    /* Allocate the operation context in the secure world */
    status = tfm_crypto_operation_alloc(TFM_CRYPTO_MAC_OPERATION,
                                        &handle,
                                        (void **)&operation);
    if (status != PSA_SUCCESS) {
        return status;
    }

    *handle_out = handle;

    status = tfm_crypto_encode_id_and_owner(key_id, &encoded_key);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    status = psa_mac_sign_setup(operation, encoded_key, alg);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    return status;

exit:
    /* Release the operation context, ignore if the operation fails. */
    (void)tfm_crypto_operation_release(handle_out);
    return status;
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}

psa_status_t tfm_crypto_mac_verify_setup(psa_invec in_vec[],
                                         size_t in_len,
                                         psa_outvec out_vec[],
                                         size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;
    psa_mac_operation_t *operation = NULL;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 1, 1);

    if ((out_vec[0].len != sizeof(uint32_t)) ||
        (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    psa_key_id_t key_id = iov->key_id;
    psa_algorithm_t alg = iov->alg;
    mbedtls_svc_key_id_t encoded_key;

    /* Init the handle in the operation with the one passed from the iov */
    *handle_out = iov->op_handle;

    /* Allocate the operation context in the secure world */
    status = tfm_crypto_operation_alloc(TFM_CRYPTO_MAC_OPERATION,
                                        &handle,
                                        (void **)&operation);
    if (status != PSA_SUCCESS) {
        return status;
    }

    *handle_out = handle;

    status = tfm_crypto_encode_id_and_owner(key_id, &encoded_key);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    status = psa_mac_verify_setup(operation, encoded_key, alg);
    if (status != PSA_SUCCESS) {
        goto exit;
    }

    return status;

exit:
    /* Release the operation context, ignore if the operation fails. */
    (void)tfm_crypto_operation_release(handle_out);
    return status;
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}

psa_status_t tfm_crypto_mac_update(psa_invec in_vec[],
                                   size_t in_len,
                                   psa_outvec out_vec[],
                                   size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;
    psa_mac_operation_t *operation = NULL;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 2, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(uint32_t))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    const uint8_t *input = in_vec[1].base;
    size_t input_length = in_vec[1].len;

    /* Init the handle in the operation with the one passed from the iov */
    *handle_out = iov->op_handle;

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_MAC_OPERATION,
                                         handle,
                                         (void **)&operation);
    if (status != PSA_SUCCESS) {
        return status;
    }

    return psa_mac_update(operation, input, input_length);
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}

psa_status_t tfm_crypto_mac_sign_finish(psa_invec in_vec[],
                                        size_t in_len,
                                        psa_outvec out_vec[],
                                        size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;
    psa_mac_operation_t *operation = NULL;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 1, 2);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(uint32_t))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    uint8_t *mac = out_vec[1].base;
    size_t mac_size = out_vec[1].len;

    /* Init the handle in the operation with the one passed from the iov */
    *handle_out = iov->op_handle;

    /* Initialise mac_length to zero */
    out_vec[1].len = 0;

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_MAC_OPERATION,
                                         handle,
                                         (void **)&operation);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_mac_sign_finish(operation, mac, mac_size, &out_vec[1].len);
    if (status == PSA_SUCCESS) {
        /* Release the operation context, ignore if the operation fails. */
        (void)tfm_crypto_operation_release(handle_out);
    }

    return status;
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}

psa_status_t tfm_crypto_mac_verify_finish(psa_invec in_vec[],
                                          size_t in_len,
                                          psa_outvec out_vec[],
                                          size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;
    psa_mac_operation_t *operation = NULL;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 2, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(uint32_t))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    const uint8_t *mac = in_vec[1].base;
    size_t mac_length = in_vec[1].len;

    /* Init the handle in the operation with the one passed from the iov */
    *handle_out = iov->op_handle;

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_MAC_OPERATION,
                                         handle,
                                         (void **)&operation);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_mac_verify_finish(operation, mac, mac_length);
    if (status == PSA_SUCCESS) {
        /* Release the operation context, ignore if the operation fails. */
        (void)tfm_crypto_operation_release(handle_out);
    }

    return status;
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}

psa_status_t tfm_crypto_mac_abort(psa_invec in_vec[],
                                  size_t in_len,
                                  psa_outvec out_vec[],
                                  size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;
    psa_mac_operation_t *operation = NULL;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(uint32_t))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;

    /* Init the handle in the operation with the one passed from the iov */
    *handle_out = iov->op_handle;

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_MAC_OPERATION,
                                         handle,
                                         (void **)&operation);
    if (status != PSA_SUCCESS) {
        /* Operation does not exist, so abort has no effect */
        return PSA_SUCCESS;
    }

    status = psa_mac_abort(operation);

    if (status != PSA_SUCCESS) {
        /* Release the operation context, ignore if the operation fails. */
        (void)tfm_crypto_operation_release(handle_out);
        return status;
    }

    return tfm_crypto_operation_release(handle_out);
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}

psa_status_t tfm_crypto_mac_compute(psa_invec in_vec[],
                                    size_t in_len,
                                    psa_outvec out_vec[],
                                    size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 2, out_len, 1, 1);

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_key_id_t key_id = iov->key_id;
    psa_algorithm_t alg = iov->alg;
    const uint8_t *input = in_vec[1].base;
    size_t input_length = in_vec[1].len;
    uint8_t *mac = out_vec[0].base;
    size_t mac_size = out_vec[0].len;
    mbedtls_svc_key_id_t encoded_key;

    status = tfm_crypto_encode_id_and_owner(key_id, &encoded_key);
    if (status != PSA_SUCCESS) {
        return status;
    }

    return psa_mac_compute(encoded_key, alg, input, input_length, mac, mac_size,
                           &out_vec[0].len);
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}

psa_status_t tfm_crypto_mac_verify(psa_invec in_vec[],
                                   size_t in_len,
                                   psa_outvec out_vec[],
                                   size_t out_len)
{
#ifdef TFM_CRYPTO_MAC_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status = PSA_SUCCESS;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 3, out_len, 0, 0);

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_key_id_t key_id = iov->key_id;
    psa_algorithm_t alg = iov->alg;
    const uint8_t *input = in_vec[1].base;
    size_t input_length = in_vec[1].len;
    const uint8_t *mac = in_vec[2].base;
    size_t mac_length = in_vec[2].len;
    mbedtls_svc_key_id_t encoded_key;

    status = tfm_crypto_encode_id_and_owner(key_id, &encoded_key);
    if (status != PSA_SUCCESS) {
        return status;
    }

    return psa_mac_verify(encoded_key, alg, input, input_length, mac,
                          mac_length);
#endif /* TFM_CRYPTO_MAC_MODULE_DISABLED */
}
/*!@}*/
