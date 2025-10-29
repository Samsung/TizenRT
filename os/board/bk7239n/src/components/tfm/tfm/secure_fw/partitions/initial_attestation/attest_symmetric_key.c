/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2018-2019, Laurence Lundblade.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "attest_key.h"
#include "tfm_plat_crypto_keys.h"
#include "psa/crypto.h"
#include "tfm_memory_utils.h"

/* Only support HMAC as MAC algorithm in COSE_Mac0 so far */
#define SYMMETRIC_IAK_MAX_SIZE        PSA_MAC_MAX_SIZE

/* Hash algorithm for calculating Instance ID */
#define INSTANCE_ID_HASH_ALG          PSA_ALG_SHA_256

/* Length of kid buffer */
#define KID_BUF_LEN                   32

/* Symmetric IAK handle */
static psa_key_handle_t symmetric_iak_handle = 0;

/* Instance ID for symmetric IAK */
static uint8_t instance_id_buf[PSA_HASH_LENGTH(INSTANCE_ID_HASH_ALG) + 1];
static size_t instance_id_len = 0;

#ifdef INCLUDE_COSE_KEY_ID
/* kid buffer */
static uint8_t kid_buf[KID_BUF_LEN];
/* Actual kid length */
static size_t kid_len = 0;
#endif

static psa_status_t destroy_iak(psa_key_handle_t *iak_handle)
{
    psa_status_t res;

    res = psa_destroy_key(*iak_handle);

    *iak_handle = 0;
    return res;
}

/**
 * \brief Static function to execute hash computation of symmetric IAK once.
 *
 * \param[in]  iak_buf      Buffer containing the IAK raw data
 * \param[in]  iak_len      The length of IAK raw data
 * \param[out] hash_buf     The buffer to be written with hash data
 * \param[in]  hash_size    The size of hash_buf
 * \param[out] hash_len     The actual length of hash data
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
static psa_status_t symmetric_iak_hash(const uint8_t *iak_buf,
                                       size_t        iak_len,
                                       uint8_t       *hash_buf,
                                       size_t        hash_size,
                                       size_t        *hash_len)
{
    psa_hash_operation_t hash_op = psa_hash_operation_init();
    psa_status_t status;

    if (!iak_buf || !hash_buf || !hash_len) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    status = psa_hash_setup(&hash_op, INSTANCE_ID_HASH_ALG);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_hash_update(&hash_op, iak_buf, iak_len);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_hash_finish(&hash_op, hash_buf, hash_size, hash_len);

    return status;
}

/*
 * Hash a symmetric Initial Attestation Key (IAK) twice to get the Instance ID.
 *
 * \note Please note that this function will corrupt the original IAK data in
 *       iak_buf.
 *       It can save a 32-byte buffer to put the intermediate data of the first
 *       hash into iak_buf.
 */
static psa_status_t calc_instance_id(uint8_t *iak_buf, size_t iak_len)
{
    psa_status_t status;
    /* Leave the first byte for UEID type byte */
    uint8_t *id_ptr = instance_id_buf + 1;
    size_t id_len = sizeof(instance_id_buf) - 1;

    if (!iak_buf) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    status = symmetric_iak_hash(iak_buf, iak_len, id_ptr, id_len,
                                &instance_id_len);
    if (status != PSA_SUCCESS) {
        instance_id_len = 0;
        return status;
    }

    /*
     * instance_id_len = SHA-256 block size < key_len <= key_buf size
     * It should be safe to directly copy without boundary check.
     */
    tfm_memcpy(iak_buf, id_ptr, instance_id_len);

    status = symmetric_iak_hash(iak_buf, instance_id_len, id_ptr, id_len,
                                &instance_id_len);
    if (status == PSA_SUCCESS) {
        /* Add UEID type byte 0x01 */
        instance_id_buf[0] = 0x01;
        instance_id_len++;
    } else {
        instance_id_len = 0;
    }

    return status;
}

enum psa_attest_err_t attest_register_initial_attestation_key(void)
{
    uint8_t key_buf[SYMMETRIC_IAK_MAX_SIZE];
    psa_algorithm_t key_alg;
    psa_key_handle_t key_handle;
    size_t key_len;
    enum tfm_plat_err_t plat_res;
    psa_status_t psa_res;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

    if (symmetric_iak_handle) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Get the symmetric initial attestation key for HMAC operation */
    plat_res = tfm_plat_get_symmetric_iak(key_buf, sizeof(key_buf),
                                          &key_len, &key_alg);
    /* In case the buffer size was not checked, although unlikely */
    if (sizeof(key_buf) < key_len) {
        /*
         * Something critical following key_buf may be overwritten.
         * Directly jump into fatal error handling.
         *
         * TODO: Should be replaced by a call to psa_panic() when it
         * becomes available.
         */
        while (1) {
            ;
        }
    }
    if (plat_res != TFM_PLAT_ERR_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /*
     * Verify if HMAC algorithm is valid.
     * According to COSE (RFC 8152), only SHA-256, SHA-384 and SHA-512 are
     * supported in HMAC.
     */
    if ((key_alg != PSA_ALG_HMAC(PSA_ALG_SHA_256)) &&
        (key_alg != PSA_ALG_HMAC(PSA_ALG_SHA_384)) &&
        (key_alg != PSA_ALG_HMAC(PSA_ALG_SHA_512))) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Setup the key attributes */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_algorithm(&key_attributes, key_alg);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);

    /* Register the symmetric key to Crypto service */
    psa_res = psa_import_key(&key_attributes, key_buf, key_len, &key_handle);
    if (psa_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    symmetric_iak_handle = key_handle;

    /*
     * Calculate the Instance ID.
     * Since Instance ID is generated from symmetric IAK, achieve it now to
     * protect critical IAK raw data from being repeatedly fetched.
     * IAK in key_buf will be corrupted. Therefore, this step must be called
     * at the end.
     */
    psa_res = calc_instance_id(key_buf, key_len);
    if (psa_res != PSA_SUCCESS) {
        destroy_iak(&symmetric_iak_handle);
        return PSA_ATTEST_ERR_GENERAL;
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t attest_unregister_initial_attestation_key(void)
{
    if (!symmetric_iak_handle) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    destroy_iak(&symmetric_iak_handle);

    /* Invalidate the Instance ID as well */
    instance_id_len = 0;

#ifdef INCLUDE_COSE_KEY_ID
    /* Invalidate the corresponding kid as well */
    kid_len = 0;
#endif

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_get_signing_key_handle(psa_key_handle_t *key_handle)
{
    if (!symmetric_iak_handle) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    *key_handle = symmetric_iak_handle;

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_get_instance_id(struct q_useful_buf_c *id_buf)
{
    if (!id_buf) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    if (!instance_id_len) {
        return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
    }

    id_buf->ptr = instance_id_buf;
    id_buf->len = instance_id_len;

    return PSA_ATTEST_ERR_SUCCESS;
}

#ifdef INCLUDE_COSE_KEY_ID
enum psa_attest_err_t
attest_get_initial_attestation_key_id(struct q_useful_buf_c *attest_key_id)
{
    enum tfm_plat_err_t plat_res;

    if (!attest_key_id) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* The kid has not been fetched previously */
    if (!kid_len) {
        plat_res = tfm_plat_get_symmetric_iak_id(kid_buf,
                                                 sizeof(kid_buf),
                                                 &kid_len);
        /* In case the buffer size was not checked, although unlikely */
        if (sizeof(kid_buf) < kid_len) {
            /*
             * Something critical following kid_buf may be overwritten.
             * Directly jump into fatal error handling.
             *
             * TODO: Should be replaced by a call to psa_panic() when it
             * becomes available.
             */
            while (1) {
                ;
            }
        }

        if (plat_res != TFM_PLAT_ERR_SUCCESS) {
            return PSA_ATTEST_ERR_GENERAL;
        }
    }

    attest_key_id->ptr = (const void *)&kid_buf;
    attest_key_id->len = kid_len;

    return PSA_ATTEST_ERR_SUCCESS;
}
#endif /* INCLUDE_COSE_KEY_ID */
