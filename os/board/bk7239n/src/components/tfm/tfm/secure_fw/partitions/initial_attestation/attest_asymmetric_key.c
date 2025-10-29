/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2018-2019, Laurence Lundblade.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "attest_key.h"
#include <stdint.h>
#include <stddef.h>
#include "tfm_plat_defs.h"
#include "tfm_plat_crypto_keys.h"
#include "tfm_plat_device_id.h"
#include "t_cose_standard_constants.h"
#include "q_useful_buf.h"
#include "qcbor.h"
#include "tfm_memory_utils.h"

#define ECC_P256_PUBLIC_KEY_SIZE PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(256)

/**
 * The size of X and Y coordinate in 2 parameter style EC public
 * key. Format is as defined in [COSE (RFC 8152)]
 * (https://tools.ietf.org/html/rfc8152) and [SEC 1: Elliptic Curve
 * Cryptography](http://www.secg.org/sec1-v2.pdf).
 *
 * This size is well-known and documented in public standards.
 */
#define ECC_P256_COORD_SIZE PSA_BITS_TO_BYTES(256) /* 256 bits -> 32 bytes */

/* 0 is defined as an invalid handle in the PSA spec, so it is used here to
 * indicate that the key isn't loaded.
 */
#define ATTEST_KEY_HANDLE_NOT_LOADED 0

/**
 * Global key handle for the attestation key. Used to prevent keys having to be
 * reloaded multiple times during a single token operation.
 */
static psa_key_handle_t attestation_key_handle = ATTEST_KEY_HANDLE_NOT_LOADED;

/**
 * The public key is kept loaded as it is both not required to be secret (and
 * hence can be kept in attestation memory) and immutable.
 */
static uint8_t  attestation_public_key[ECC_P256_PUBLIC_KEY_SIZE]; /* 65bytes */
static size_t   attestation_public_key_len = 0;
static psa_ecc_family_t attestation_key_curve;

#ifdef INCLUDE_COSE_KEY_ID
/* 32bytes */
static uint8_t attestation_key_id[PSA_HASH_LENGTH(PSA_ALG_SHA_256)];
#endif

/* Instance ID for asymmetric IAK */
static uint8_t instance_id_buf[INSTANCE_ID_MAX_SIZE];
static size_t instance_id_len = 0U;

enum psa_attest_err_t
attest_register_initial_attestation_key()
{
    enum tfm_plat_err_t plat_res;
    psa_ecc_family_t psa_curve;
    struct ecc_key_t attest_key = {0};
    uint8_t key_buf[3 * ECC_P256_COORD_SIZE]; /* priv + x_coord + y_coord */
    psa_key_handle_t key_handle = ATTEST_KEY_HANDLE_NOT_LOADED;
    psa_status_t crypto_res;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();

    if (attestation_key_handle != ATTEST_KEY_HANDLE_NOT_LOADED) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Get the initial attestation key */
    plat_res = tfm_plat_get_initial_attest_key(key_buf, sizeof(key_buf),
                                               &attest_key, &psa_curve);

    /* Check the availability of the private key */
    if (plat_res != TFM_PLAT_ERR_SUCCESS || attest_key.priv_key == NULL) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Setup the key policy for private key */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(psa_curve));

    /* Register private key to Crypto service */
    crypto_res = psa_import_key(&key_attributes,
                                attest_key.priv_key,
                                attest_key.priv_key_size,
                                &key_handle);


    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    attestation_key_handle = key_handle;

    /* If the public key length is 0 then it hasn't been loaded */
    if (attestation_public_key_len == 0) {
        crypto_res = psa_export_public_key(key_handle, attestation_public_key,
                                           ECC_P256_PUBLIC_KEY_SIZE,
                                           &attestation_public_key_len);
        if (crypto_res != PSA_SUCCESS) {
            return PSA_ATTEST_ERR_GENERAL;
        }

        attestation_key_curve = psa_curve;
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_unregister_initial_attestation_key()
{
    psa_status_t crypto_res;

    if (attestation_key_handle == ATTEST_KEY_HANDLE_NOT_LOADED) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    crypto_res = psa_destroy_key(attestation_key_handle);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    attestation_key_handle = ATTEST_KEY_HANDLE_NOT_LOADED;

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_get_signing_key_handle(psa_key_handle_t *handle)
{
    if (attestation_key_handle == ATTEST_KEY_HANDLE_NOT_LOADED) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    *handle = attestation_key_handle;

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to calculate instance id.
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t attest_calc_instance_id(void)
{
    psa_status_t crypto_res;
    psa_hash_operation_t hash = psa_hash_operation_init();

    if (!attestation_public_key_len) {
        return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
    }

    crypto_res = psa_hash_setup(&hash, PSA_ALG_SHA_256);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
    }

    crypto_res = psa_hash_update(&hash, attestation_public_key,
                                 attestation_public_key_len);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
    }

    /* The hash starts from the second byte, leaving the first free. */
    crypto_res = psa_hash_finish(&hash, instance_id_buf + 1,
                                 INSTANCE_ID_MAX_SIZE - 1,
                                 &instance_id_len);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
    }

    /* Add UEID type byte 0x01 */
    instance_id_buf[0] = 0x01;
    instance_id_len = instance_id_len + 1;

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_get_instance_id(struct q_useful_buf_c *id_buf)
{
    if (instance_id_len == 0U) {
        if (attest_calc_instance_id() != PSA_ATTEST_ERR_SUCCESS) {
            return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
        }
    }

    if (id_buf == NULL) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    id_buf->ptr = instance_id_buf;
    id_buf->len = instance_id_len;

    return PSA_ATTEST_ERR_SUCCESS;
}

#ifdef INCLUDE_COSE_KEY_ID

#define MAX_ENCODED_COSE_KEY_SIZE \
    1 + /* 1 byte to encode map */ \
    2 + /* 2 bytes to encode key type */ \
    2 + /* 2 bytes to encode curve */ \
    2 * /* the X and Y coordinates at 32 bytes each */ \
        (ECC_P256_COORD_SIZE + 1 + 2)

/**
 * \brief     Map PSA curve types to the curve type defined by RFC 8152
 *            chapter 13.1.
 *
 * \param[in]  psa_curve  PSA curve type definition \ref psa_ecc_family_t.
 *
 * \return    Return COSE curve type. If mapping is not possible then return
 *            with -1.
 */
static inline int32_t
attest_map_psa_ecc_curve_to_cose_ecc_curve(psa_ecc_family_t psa_curve)
{
    int32_t cose_curve;

    /* Note:  Mapping is not complete. */
    switch (psa_curve) {
    case PSA_ECC_FAMILY_SECP_R1:
        cose_curve = COSE_ELLIPTIC_CURVE_P_256;
        break;
    default:
        /* Initial attestation currently supports only ECDSA P256 signature
         * therefore the other options are not mapped to save object code
         */
        cose_curve = -1;
    }

    return cose_curve;
}

/**
 * \brief CBOR encode a public key as a \c COSE_Key
 *
 * \param[in]  psa_ecc_curve        PSA elliptic curve identifiers
 * \param[in]  attest_public_key    Pointer and length of the buffer, which
 *                                  holds the attestation public key encoded as
 *                                  defined by SEC1 &sect;2.3.3.
 * \param[in]  buffer_for_cose_key  Pointer and length of buffer into which
 *                                  the encoded \c COSE_Key is put.
 * \param[out] cose_key             Pointer and length of the encoded
 *                                  \c COSE_Key.
 *
 * \return This returns one of the error codes defined by \ref psa_attest_err_t.
 *
 * \c COSE_Key is the COSE-defined format for serializing a key for
 * transmission in a protocol. This function encodes an EC public key
 * expressed as an X and Y coordinate.
 */
static enum psa_attest_err_t
attest_encode_key_to_cose_key(psa_ecc_family_t        psa_ecc_curve,
                              struct q_useful_buf_c   attest_public_key,
                              struct q_useful_buf     buffer_for_cose_key,
                              struct q_useful_buf_c  *cose_key)
{
    QCBORError                 qcbor_result;
    QCBOREncodeContext         cbor_encode_ctx;
    struct q_useful_buf_c      x_coord;
    struct q_useful_buf_c      y_coord;
    struct q_useful_buf_c      encoded_key_id;
    size_t                     key_coord_len;
    int32_t                    cose_ecc_curve;
    uint8_t                    *x_coord_ptr;
    uint8_t                    *y_coord_ptr;
    Q_USEFUL_BUF_MAKE_STACK_UB(buffer_for_x_coord, ECC_P256_COORD_SIZE);
    Q_USEFUL_BUF_MAKE_STACK_UB(buffer_for_y_coord, ECC_P256_COORD_SIZE);

    /* Key is made up of a 0x4 byte and two coordinates
     * 0x04 || X_COORD || Y_COORD
     */
    key_coord_len = (attest_public_key.len - 1) / 2;
    x_coord_ptr = ((uint8_t *)attest_public_key.ptr) + 1;
    y_coord_ptr = ((uint8_t *)attest_public_key.ptr) + 1 + key_coord_len;

    /* Place they key parts into the x and y buffers. Stars at index 1 to skip
     * the 0x4 byte.
     */
    x_coord = q_useful_buf_copy_ptr(buffer_for_x_coord,
                                    x_coord_ptr,
                                    key_coord_len);

    y_coord = q_useful_buf_copy_ptr(buffer_for_y_coord,
                                    y_coord_ptr,
                                    key_coord_len);

    if (q_useful_buf_c_is_null(x_coord) || q_useful_buf_c_is_null(y_coord)) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    cose_ecc_curve = attest_map_psa_ecc_curve_to_cose_ecc_curve(psa_ecc_curve);
    if (cose_ecc_curve == -1) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Encode it into a COSE_Key structure */
    QCBOREncode_Init(&cbor_encode_ctx, buffer_for_cose_key);
    QCBOREncode_OpenMap(&cbor_encode_ctx);
    QCBOREncode_AddInt64ToMapN(&cbor_encode_ctx,
                               COSE_KEY_COMMON_KTY,
                               COSE_KEY_TYPE_EC2);
    QCBOREncode_AddInt64ToMapN(&cbor_encode_ctx,
                               COSE_KEY_PARAM_CRV,
                               cose_ecc_curve);
    QCBOREncode_AddBytesToMapN(&cbor_encode_ctx,
                               COSE_KEY_PARAM_X_COORDINATE,
                               x_coord);
    QCBOREncode_AddBytesToMapN(&cbor_encode_ctx,
                               COSE_KEY_PARAM_Y_COORDINATE,
                               y_coord);
    QCBOREncode_CloseMap(&cbor_encode_ctx);

    qcbor_result = QCBOREncode_Finish(&cbor_encode_ctx, &encoded_key_id);
    if (qcbor_result != QCBOR_SUCCESS) {
        /* Mainly means that the COSE_Key was too big for buffer_for_cose_key */
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Finish up and return */
    *cose_key = encoded_key_id;

    return PSA_ATTEST_ERR_SUCCESS;
}

/**
 * \brief Make a key ID based on the public key to go in the kid
 *        unprotected header.
 *
 * \param[in]  psa_ecc_curve             PSA elliptic curve identifiers.
 * \param[in]  attest_public_key         Pointer and length of the buffer, which
 *                                       holds the attestation public key
 *                                       encoded as defined by SEC1 &sect;2.3.3.
 * \param[in]  buffer_for_attest_key_id  Pointer and length of buffer into which
 *                                       the encoded \c COSE_Key is put.
 * \param[out] attest_key_id             Pointer and length of the attestation
 *                                       key id.
 *
 * \return This returns one of the error codes defined by \ref psa_attest_err_t.
 */
enum psa_attest_err_t
attest_get_cose_key_id(psa_ecc_family_t        psa_ecc_curve,
                       struct q_useful_buf_c   attest_public_key,
                       struct q_useful_buf     buffer_for_attest_key_id,
                       struct q_useful_buf_c  *attest_key_id)
{
    enum psa_attest_err_t      attest_res;
    psa_status_t               crypto_res;
    struct q_useful_buf_c      cose_key;
    psa_hash_operation_t       hash = psa_hash_operation_init();
    Q_USEFUL_BUF_MAKE_STACK_UB(buffer_for_cose_key, MAX_ENCODED_COSE_KEY_SIZE);

    /* Encode the attestation public key to COSE_Key structure */
    attest_res = attest_encode_key_to_cose_key(psa_ecc_curve,
                                               attest_public_key,
                                               buffer_for_cose_key,
                                               &cose_key);
    if(attest_res != PSA_ATTEST_ERR_SUCCESS) {
        return attest_res;
    }

    crypto_res = psa_hash_setup(&hash, PSA_ALG_SHA_256);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    crypto_res = psa_hash_update(&hash, cose_key.ptr, cose_key.len);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    crypto_res = psa_hash_finish(&hash,
                                  buffer_for_attest_key_id.ptr,
                                  buffer_for_attest_key_id.len,
                                 &buffer_for_attest_key_id.len);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    attest_key_id->ptr = buffer_for_attest_key_id.ptr;
    attest_key_id->len = buffer_for_attest_key_id.len;

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_get_initial_attestation_key_id(struct q_useful_buf_c *attest_key_id)
{
    enum psa_attest_err_t  attest_res;
    static uint8_t         attest_key_id_calculated;
    struct q_useful_buf_c  buffer_for_attest_public_key;
    struct q_useful_buf    buffer_for_attest_key_id;

    buffer_for_attest_key_id.ptr = attestation_key_id;
    buffer_for_attest_key_id.len = PSA_HASH_LENGTH(PSA_ALG_SHA_256);

    /* Needs to calculate only once */
    if (attest_key_id_calculated == 0) {
        buffer_for_attest_public_key.ptr = attestation_public_key;
        buffer_for_attest_public_key.len = attestation_public_key_len;

        attest_res = attest_get_cose_key_id(attestation_key_curve,
                                            buffer_for_attest_public_key,
                                            buffer_for_attest_key_id,
                                            attest_key_id);
        if (attest_res != PSA_ATTEST_ERR_SUCCESS) {
            return attest_res;
        }
        attest_key_id_calculated = 1;
    } else {
        attest_key_id->ptr = (const void *)buffer_for_attest_key_id.ptr;
        attest_key_id->len = buffer_for_attest_key_id.len;
    }

    return PSA_ATTEST_ERR_SUCCESS;
}
#endif /* INCLUDE_COSE_KEY_ID */
