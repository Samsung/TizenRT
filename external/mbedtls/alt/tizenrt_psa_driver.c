/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @file tizenrt_psa_driver.c
 * @brief PSA Opaque Crypto Driver for TizenRT Samsung SE hardware.
 *
 * Replaces the old MBEDTLS_ECDH_ALT / MBEDTLS_ECDSA_SIGN_ALT mechanisms
 * removed in mbedtls 4.1.0.  All ECC private-key operations (ECDH, ECDSA
 * sign) are forwarded to the hardware via the seclink/HAL API.  Private
 * keys never leave the hardware; only the public key and shared secrets
 * are exported.
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>

#include "mbedtls/build_info.h"
#include "psa/crypto.h"
#include "psa/crypto_sizes.h"

#include "mbedtls/alt/common.h"
#include "alt_utils.h"
#include "tizenrt_psa_driver.h"

/* mbedtls_ecdsa_raw_to_der / mbedtls_ecdsa_der_to_raw */
#include "mbedtls/psa_util.h"

/* =========================================================================
 * Internal helpers – PSA type → HAL type mapping
 * =========================================================================
 */

/**
 * Map PSA ECC key type + bit-size to the HAL key type constant.
 * Returns HAL_KEY_UNKNOWN when the combination is unsupported.
 */
static hal_key_type tizenrt_psa_to_hal_key_type(psa_key_type_t key_type,
                                                 size_t key_bits)
{
    if (!PSA_KEY_TYPE_IS_ECC(key_type)) {
        return HAL_KEY_UNKNOWN;
    }

    psa_ecc_family_t family = PSA_KEY_TYPE_ECC_GET_FAMILY(key_type);

    if (family == PSA_ECC_FAMILY_SECP_R1) {
        switch (key_bits) {
        case 256: return HAL_KEY_ECC_SEC_P256R1;
        case 384: return HAL_KEY_ECC_SEC_P384R1;
        case 521: return HAL_KEY_ECC_SEC_P512R1;  /* HAL uses P512 for P-521 */
        default:  return HAL_KEY_UNKNOWN;
        }
    }

    if (family == PSA_ECC_FAMILY_BRAINPOOL_P_R1) {
        switch (key_bits) {
        case 256: return HAL_KEY_ECC_BRAINPOOL_P256R1;
        case 384: return HAL_KEY_ECC_BRAINPOOL_P384R1;
        case 512: return HAL_KEY_ECC_BRAINPOOL_P512R1;
        default:  return HAL_KEY_UNKNOWN;
        }
    }

    return HAL_KEY_UNKNOWN;
}

/**
 * Map PSA ECC key type + bit-size to the HAL ECDSA curve constant.
 */
static hal_ecdsa_curve tizenrt_psa_to_hal_ecdsa_curve(psa_key_type_t key_type,
                                                       size_t key_bits)
{
    if (!PSA_KEY_TYPE_IS_ECC(key_type)) {
        return HAL_ECDSA_UNKNOWN;
    }

    psa_ecc_family_t family = PSA_KEY_TYPE_ECC_GET_FAMILY(key_type);

    if (family == PSA_ECC_FAMILY_SECP_R1) {
        switch (key_bits) {
        case 256: return HAL_ECDSA_SEC_P256R1;
        case 384: return HAL_ECDSA_SEC_P384R1;
        case 521: return HAL_ECDSA_SEC_P521R1;
        default:  return HAL_ECDSA_UNKNOWN;
        }
    }

    if (family == PSA_ECC_FAMILY_BRAINPOOL_P_R1) {
        switch (key_bits) {
        case 256: return HAL_ECDSA_BRAINPOOL_P256R1;
        case 384: return HAL_ECDSA_BRAINPOOL_P384R1;
        case 512: return HAL_ECDSA_BRAINPOOL_P512R1;
        default:  return HAL_ECDSA_UNKNOWN;
        }
    }

    return HAL_ECDSA_UNKNOWN;
}

/**
 * Map a PSA hash algorithm value to the HAL hash type constant.
 * PSA_ALG_GET_HASH(alg) extracts the hash part of a composite algorithm.
 */
static hal_hash_type tizenrt_psa_to_hal_hash(psa_algorithm_t hash_alg)
{
    switch (hash_alg) {
    case PSA_ALG_SHA_1:   return HAL_HASH_SHA1;
    case PSA_ALG_SHA_224: return HAL_HASH_SHA224;
    case PSA_ALG_SHA_256: return HAL_HASH_SHA256;
    case PSA_ALG_SHA_384: return HAL_HASH_SHA384;
    case PSA_ALG_SHA_512: return HAL_HASH_SHA512;
    default:              return HAL_HASH_UNKNOWN;
    }
}

/* =========================================================================
 * Opaque Driver – Key Buffer Size
 * =========================================================================
 */

psa_status_t tizenrt_opaque_get_key_buffer_size(
    const psa_key_attributes_t *attributes,
    size_t *key_buffer_size)
{
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    if (!PSA_KEY_TYPE_IS_ECC_KEY_PAIR(key_type)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (tizenrt_psa_to_hal_key_type(key_type, key_bits) == HAL_KEY_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    *key_buffer_size = sizeof(tizenrt_sss_key_ctx_t);
    return PSA_SUCCESS;
}

/* =========================================================================
 * Opaque Driver – Key Generation
 * =========================================================================
 */

psa_status_t tizenrt_opaque_generate_key(
    const psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size,
    size_t *key_buffer_length)
{
    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    if (!PSA_KEY_TYPE_IS_ECC_KEY_PAIR(key_type)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_key_type hal_key = tizenrt_psa_to_hal_key_type(key_type, key_bits);
    if (hal_key == HAL_KEY_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (key_buffer_size < sizeof(tizenrt_sss_key_ctx_t)) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    sl_ctx shnd;
    if (sl_init(&shnd) != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    int key_idx = alt_gen_key(shnd, hal_key, ECP_KEY_INDEX);
    sl_deinit(shnd);

    if (key_idx == -1) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    tizenrt_sss_key_ctx_t *ctx = (tizenrt_sss_key_ctx_t *)key_buffer;
    ctx->key_index = (uint32_t)key_idx;
    ctx->key_type  = (uint32_t)hal_key;

    *key_buffer_length = sizeof(tizenrt_sss_key_ctx_t);
    return PSA_SUCCESS;
}

/* =========================================================================
 * Opaque Driver – Export Public Key
 * =========================================================================
 */

psa_status_t tizenrt_opaque_export_public_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    uint8_t *data, size_t data_size,
    size_t *data_length)
{
    if (key_buffer_size < sizeof(tizenrt_sss_key_ctx_t)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    const tizenrt_sss_key_ctx_t *ctx =
        (const tizenrt_sss_key_ctx_t *)key_buffer;

    size_t key_bits   = psa_get_key_bits(attributes);
    size_t coord_size = PSA_BITS_TO_BYTES(key_bits);

    /* Uncompressed point: 04 || X || Y */
    size_t required = 1 + 2 * coord_size;
    if (data_size < required) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    unsigned char key_x[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];
    unsigned char key_y[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];
    hal_data hal_key_data = {
        key_x, sizeof(key_x),
        key_y, sizeof(key_y)
    };

    sl_ctx shnd;
    if (sl_init(&shnd) != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    int ret = sl_get_key(shnd, (hal_key_type)ctx->key_type,
                         (unsigned int)ctx->key_index, &hal_key_data);
    sl_deinit(shnd);

    if (ret != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    /* Write uncompressed point prefix */
    data[0] = 0x04;

    /* Pad X coordinate to coord_size (hardware may return fewer bytes) */
    size_t x_pad = coord_size - hal_key_data.data_len;
    memset(data + 1,             0,                      x_pad);
    memcpy(data + 1 + x_pad,     hal_key_data.data,      hal_key_data.data_len);

    /* Pad Y coordinate */
    size_t y_pad = coord_size - hal_key_data.priv_len;
    memset(data + 1 + coord_size,          0,                      y_pad);
    memcpy(data + 1 + coord_size + y_pad,  hal_key_data.priv,      hal_key_data.priv_len);

    *data_length = required;
    return PSA_SUCCESS;
}

/* =========================================================================
 * Opaque Driver – ECDH Key Agreement
 * =========================================================================
 */

psa_status_t tizenrt_opaque_key_agreement(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *peer_key, size_t peer_key_length,
    uint8_t *shared_secret, size_t shared_secret_size,
    size_t *shared_secret_length)
{
    if (!PSA_ALG_IS_ECDH(alg)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (key_buffer_size < sizeof(tizenrt_sss_key_ctx_t)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    const tizenrt_sss_key_ctx_t *ctx =
        (const tizenrt_sss_key_ctx_t *)key_buffer;

    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);
    size_t coord_size = PSA_BITS_TO_BYTES(key_bits);

    /* peer_key must be uncompressed: 04 || X || Y */
    if (peer_key_length != 1 + 2 * coord_size || peer_key[0] != 0x04) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (shared_secret_size < coord_size) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    hal_ecdsa_curve curve =
        tizenrt_psa_to_hal_ecdsa_curve(key_type, key_bits);
    if (curve == HAL_ECDSA_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Build hal_ecdh_data with peer's X and Y coordinates */
    const uint8_t *peer_x = peer_key + 1;
    const uint8_t *peer_y = peer_key + 1 + coord_size;

    hal_data pubkey_x_data = {(void *)peer_x, coord_size, NULL, 0};
    hal_data pubkey_y_data = {(void *)peer_y, coord_size, NULL, 0};

    hal_ecdh_data ecc_pub;
    memset(&ecc_pub, 0, sizeof(ecc_pub));
    ecc_pub.curve    = curve;
    ecc_pub.pubkey_x = &pubkey_x_data;
    ecc_pub.pubkey_y = &pubkey_y_data;

    unsigned char shared_buf[MBEDTLS_MAX_KEY_SIZE_ALT];
    hal_data shared_hal = {shared_buf, sizeof(shared_buf), NULL, 0};

    sl_ctx shnd;
    if (sl_init(&shnd) != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    int ret = sl_ecdh_compute_shared_secret(shnd, &ecc_pub,
                                             ctx->key_index, &shared_hal);

    /*
     * Remove the hardware key after use.
     * ECDH ephemeral keys are single-use; this matches the original
     * ecdh_alt.c behaviour and ensures the hardware slot is freed.
     */
    (void)sl_remove_key(shnd, (hal_key_type)ctx->key_type, ctx->key_index);
    sl_deinit(shnd);

    if (ret != SECLINK_OK) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    size_t out_len = shared_hal.data_len;
    if (out_len > shared_secret_size) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    memcpy(shared_secret, shared_hal.data, out_len);
    *shared_secret_length = out_len;
    return PSA_SUCCESS;
}

/* =========================================================================
 * Opaque Driver – ECDSA Sign
 * =========================================================================
 */

psa_status_t tizenrt_opaque_sign_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *hash, size_t hash_length,
    uint8_t *signature, size_t signature_size,
    size_t *signature_length)
{
    if (!PSA_ALG_IS_ECDSA(alg)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (key_buffer_size < sizeof(tizenrt_sss_key_ctx_t)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    const tizenrt_sss_key_ctx_t *ctx =
        (const tizenrt_sss_key_ctx_t *)key_buffer;

    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    hal_ecdsa_curve curve =
        tizenrt_psa_to_hal_ecdsa_curve(key_type, key_bits);
    if (curve == HAL_ECDSA_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Hash algorithm for the HAL (identifies the hash type used) */
    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(alg);
    hal_hash_type hal_hash = tizenrt_psa_to_hal_hash(hash_alg);
    if (hal_hash == HAL_HASH_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_ecdsa_mode ecdsa_mode;
    ecdsa_mode.curve  = curve;
    ecdsa_mode.hash_t = hal_hash;

    hal_data t_hash = {(void *)hash, hash_length, NULL, 0};

    /*
     * HAL sl_ecdsa_sign_md produces a DER-encoded signature.
     * We need a temporary buffer large enough for DER then convert
     * to PSA raw r||s format.
     */
    size_t coord_size = PSA_BITS_TO_BYTES(key_bits);
    /* DER ECDSA = SEQUENCE { INTEGER r, INTEGER s }
     * Max DER size: 2 + 2*(2 + 1 + coord_size) = 6 + 4 + 2*coord_size */
    size_t der_max = 8 + 2 * (coord_size + 1);
    unsigned char *der_buf = (unsigned char *)malloc(der_max);
    if (!der_buf) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    hal_data sign = {der_buf, 0, NULL, 0};

    sl_ctx shnd;
    psa_status_t psa_ret = PSA_ERROR_HARDWARE_FAILURE;

    if (sl_init(&shnd) != SECLINK_OK) {
        free(der_buf);
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    int ret = sl_ecdsa_sign_md(shnd, ecdsa_mode, &t_hash,
                                ctx->key_index, &sign);
    sl_deinit(shnd);

    if (ret != SECLINK_OK) {
        free(der_buf);
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    /* Convert DER signature → PSA raw r||s format */
    size_t raw_len = 0;
    int conv_ret = mbedtls_ecdsa_der_to_raw(
        key_bits, der_buf, sign.data_len,
        signature, signature_size, &raw_len);
    free(der_buf);

    if (conv_ret != 0) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    *signature_length = raw_len;
    return PSA_SUCCESS;
}

/* =========================================================================
 * Opaque Driver – ECDSA Verify
 * =========================================================================
 */

psa_status_t tizenrt_opaque_verify_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *hash, size_t hash_length,
    const uint8_t *signature, size_t signature_length)
{
    if (!PSA_ALG_IS_ECDSA(alg)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (key_buffer_size < sizeof(tizenrt_sss_key_ctx_t)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    const tizenrt_sss_key_ctx_t *ctx =
        (const tizenrt_sss_key_ctx_t *)key_buffer;

    psa_key_type_t key_type = psa_get_key_type(attributes);
    size_t key_bits = psa_get_key_bits(attributes);

    hal_ecdsa_curve curve =
        tizenrt_psa_to_hal_ecdsa_curve(key_type, key_bits);
    if (curve == HAL_ECDSA_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(alg);
    hal_hash_type hal_hash = tizenrt_psa_to_hal_hash(hash_alg);
    if (hal_hash == HAL_HASH_UNKNOWN) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    hal_ecdsa_mode ecdsa_mode;
    ecdsa_mode.curve  = curve;
    ecdsa_mode.hash_t = hal_hash;

    hal_data t_hash = {(void *)hash, hash_length, NULL, 0};

    /* Convert PSA raw r||s → DER for the HAL */
    size_t coord_size = PSA_BITS_TO_BYTES(key_bits);
    size_t der_max = 8 + 2 * (coord_size + 1);
    unsigned char *der_buf = (unsigned char *)malloc(der_max);
    if (!der_buf) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    size_t der_len = 0;
    int conv_ret = mbedtls_ecdsa_raw_to_der(
        key_bits, signature, signature_length,
        der_buf, der_max, &der_len);
    if (conv_ret != 0) {
        free(der_buf);
        return PSA_ERROR_INVALID_SIGNATURE;
    }

    hal_data t_sig = {der_buf, der_len, NULL, 0};

    sl_ctx shnd;
    if (sl_init(&shnd) != SECLINK_OK) {
        free(der_buf);
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    /* Verify using the hardware-stored key pair (hardware uses the public part) */
    int ret = sl_ecdsa_verify_md(shnd, ecdsa_mode, &t_hash,
                                  &t_sig, ctx->key_index);
    sl_deinit(shnd);
    free(der_buf);

    if (ret != SECLINK_OK) {
        return PSA_ERROR_INVALID_SIGNATURE;
    }

    return PSA_SUCCESS;
}
