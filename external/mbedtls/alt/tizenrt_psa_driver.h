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
 * @file tizenrt_psa_driver.h
 * @brief PSA Crypto Driver for TizenRT Samsung SE hardware.
 *
 * This driver replaces the old MBEDTLS_*_ALT mechanisms removed in mbedtls 4.1.0.
 *
 * OPAQUE DRIVER (PSA_KEY_LOCATION_TIZENRT_SSS):
 *   ECC key pair is generated inside the hardware and the private key
 *   never leaves the device. The PSA key buffer stores only the hardware
 *   slot index.  Supported operations:
 *     - ECC key generation (for ECDH and ECDSA)
 *     - ECDH raw key agreement
 *     - ECDSA sign/verify (hash pre-computed by caller)
 *     - Export ECC public key (uncompressed: 04 || X || Y)
 *
 * TRANSPARENT CIPHER DRIVER:
 *   AES key material is provided by PSA; hardware performs encryption.
 *   Supports: ECB, CBC (no padding), CTR, CFB128.
 */

#ifndef TIZENRT_PSA_DRIVER_H
#define TIZENRT_PSA_DRIVER_H

#include <stdint.h>
#include <stddef.h>
#include "psa/crypto.h"
#include "psa/crypto_driver_common.h"
#include <tinyara/security_hal.h>
#include <tinyara/seclink.h>

/* =========================================================================
 * Opaque key location
 * =========================================================================
 * Custom locations must be in [0x800000, 0xFFFFFF] per PSA spec.
 */
#define PSA_KEY_LOCATION_TIZENRT_SSS  ((psa_key_location_t) 0x800001)

/* =========================================================================
 * Opaque key context
 * =========================================================================
 * Stored in the PSA key slot for every opaque TizenRT key.
 * 8 bytes total – fits in any PSA key buffer.
 */
typedef struct {
    uint32_t key_index;   /**< Hardware slot index (ECP_KEY_INDEX + offset) */
    uint32_t key_type;    /**< HAL key type (hal_key_type enum value)        */
} tizenrt_sss_key_ctx_t;

/*
 * tizenrt_transparent_cipher_operation_t is defined in
 * psa/crypto_driver_contexts_primitives.h (included via psa/crypto.h above).
 * It uses void* for shnd and uint32_t for key_type to avoid pulling in
 * platform headers from the public PSA header.  The cipher driver casts
 * these fields back to sl_ctx / hal_key_type when calling the HAL.
 */

/* =========================================================================
 * Opaque Driver Entry Points
 * =========================================================================
 */

/**
 * Return the key buffer size needed to store an opaque TizenRT key.
 * Always returns sizeof(tizenrt_sss_key_ctx_t) for ECC key pairs.
 */
psa_status_t tizenrt_opaque_get_key_buffer_size(
    const psa_key_attributes_t *attributes,
    size_t *key_buffer_size);

/**
 * Generate an ECC key pair in hardware.
 * Stores {key_index, key_type} in key_buffer.
 */
psa_status_t tizenrt_opaque_generate_key(
    const psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size,
    size_t *key_buffer_length);

/**
 * Export the public key from a hardware-stored ECC key pair.
 * Output format: uncompressed point = 04 || X || Y
 */
psa_status_t tizenrt_opaque_export_public_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    uint8_t *data, size_t data_size,
    size_t *data_length);

/**
 * Perform ECDH raw key agreement using a hardware-stored private key.
 * peer_key: uncompressed ECC point = 04 || X || Y
 * The hardware key is removed after the operation (single-use / ephemeral).
 */
psa_status_t tizenrt_opaque_key_agreement(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *peer_key, size_t peer_key_length,
    uint8_t *shared_secret, size_t shared_secret_size,
    size_t *shared_secret_length);

/**
 * ECDSA sign: compute signature over a pre-hashed value.
 * Output format (PSA): raw r || s, each padded to coord_size bytes.
 */
psa_status_t tizenrt_opaque_sign_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *hash, size_t hash_length,
    uint8_t *signature, size_t signature_size,
    size_t *signature_length);

/**
 * ECDSA verify: verify a signature over a pre-hashed value.
 * Input signature format (PSA): raw r || s.
 */
psa_status_t tizenrt_opaque_verify_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *hash, size_t hash_length,
    const uint8_t *signature, size_t signature_length);

/* =========================================================================
 * Transparent Cipher Driver Entry Points (AES)
 * =========================================================================
 */

psa_status_t tizenrt_transparent_cipher_encrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *iv, size_t iv_length,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size,
    size_t *output_length);

psa_status_t tizenrt_transparent_cipher_decrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size,
    size_t *output_length);

psa_status_t tizenrt_transparent_cipher_encrypt_setup(
    tizenrt_transparent_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg);

psa_status_t tizenrt_transparent_cipher_decrypt_setup(
    tizenrt_transparent_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg);

psa_status_t tizenrt_transparent_cipher_set_iv(
    tizenrt_transparent_cipher_operation_t *operation,
    const uint8_t *iv, size_t iv_length);

psa_status_t tizenrt_transparent_cipher_update(
    tizenrt_transparent_cipher_operation_t *operation,
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t output_size,
    size_t *output_length);

psa_status_t tizenrt_transparent_cipher_finish(
    tizenrt_transparent_cipher_operation_t *operation,
    uint8_t *output, size_t output_size,
    size_t *output_length);

psa_status_t tizenrt_transparent_cipher_abort(
    tizenrt_transparent_cipher_operation_t *operation);

#endif /* TIZENRT_PSA_DRIVER_H */
