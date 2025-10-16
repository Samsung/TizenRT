/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#pragma once

#include <stdint.h>

#define _IN_
#define _OUT_
#define _INOUT_
#define DEPRECATED

#define HAL_DATA_INITIALIZER {NULL, 0, NULL, 0}

/* Security HAL Return Value */
typedef enum {
	HAL_SUCCESS,
	HAL_NOT_INITIALIZED,
	HAL_INVALID_ARGS,
	HAL_INVALID_REQUEST,
	HAL_INVALID_SLOT_RANGE,
	HAL_INVALID_SLOT_TYPE, // ex. request to save key into cert slot
	HAL_EMPTY_SLOT,
	HAL_BAD_KEY,	  // only public key can be returned
	HAL_BAD_KEY_PAIR, // public and private keys do not match
	HAL_BAD_KEY_TYPE, // the type when key was set and the type to get key are not matched.
	HAL_BAD_CERT,
	HAL_BAD_CERTKEY_PAIR, // certificate and key do not match
	HAL_NOT_ENOUGH_MEMORY,
	HAL_ALLOC_FAIL,
	HAL_KEY_IN_USE,	// key slot is already in use, return HAL_KEY_IN_USE instead of overwriting key.
	HAL_CERT_IN_USE, // cert slot is already in use, return HAL_CERT_IN_USE instead of overwriting cert.
	HAL_DATA_IN_USE, // data slot is already in use, return HAL_DATA_IN_USE instead of overwriting data.
	HAL_NOT_SUPPORTED,
	HAL_NOT_IMPLEMENTED,
	HAL_BUSY,
	HAL_FAIL,
} hal_result_e;

/**
 * Structure & Enums
 */
typedef enum {
	HAL_HASH_MD5,
	HAL_HASH_SHA1,
	HAL_HASH_SHA224,
	HAL_HASH_SHA256,
	HAL_HASH_SHA384,
	HAL_HASH_SHA512,
	HAL_HASH_UNKNOWN,
} hal_hash_type;

typedef enum {
	HAL_DH_1024,
	HAL_DH_2048,
	HAL_DH_4096,
	HAL_DH_UNKNOWN,
} hal_dh_key_type;

typedef enum {
	HAL_HMAC_MD5,
	HAL_HMAC_SHA1,
	HAL_HMAC_SHA224,
	HAL_HMAC_SHA256,
	HAL_HMAC_SHA384,
	HAL_HMAC_SHA512,
	HAL_HMAC_UNKNOWN,
} hal_hmac_type;

typedef enum {
	HAL_ECDSA_BRAINPOOL_P256R1,
	HAL_ECDSA_BRAINPOOL_P384R1,
	HAL_ECDSA_BRAINPOOL_P512R1,
	HAL_ECDSA_SEC_P192R1,
	HAL_ECDSA_SEC_P224R1,
	HAL_ECDSA_SEC_P256R1,
	HAL_ECDSA_SEC_P384R1,
	HAL_ECDSA_SEC_P521R1,
	HAL_ECDSA_CURVE_25519,
	HAL_ECDSA_UNKNOWN,
} hal_ecdsa_curve;

typedef enum {
	HAL_AES_ECB_NOPAD,
	HAL_AES_ECB_ISO9797_M1,
	HAL_AES_ECB_ISO9797_M2,
	HAL_AES_ECB_PKCS5,
	HAL_AES_ECB_PKCS7,
	HAL_AES_CBC_NOPAD,
	HAL_AES_CBC_ISO9797_M1,
	HAL_AES_CBC_ISO9797_M2,
	HAL_AES_CBC_PKCS5,
	HAL_AES_CBC_PKCS7,
	HAL_AES_CTR,
	HAL_AES_CFB128,
	HAL_AES_UNKNOWN,
} hal_aes_algo;

typedef enum {
	HAL_RSASSA_PKCS1_V1_5,
	HAL_RSASSA_PKCS1_PSS_MGF1,
	HAL_RSASSA_UNKNOWN,
} hal_rsa_algo;

typedef enum {
	/*  AES */
	HAL_KEY_AES_128, // 128 bits aes algorithm
	HAL_KEY_AES_192, // 192 bits aes algorithm
	HAL_KEY_AES_256, // 256 bits aes algorithm
	/*  RSA */
	HAL_KEY_RSA_1024, // 1024 bits rsa algorithm
	HAL_KEY_RSA_2048, // 2048 bits rsa algorithm
	HAL_KEY_RSA_3072, // 3072 bits rsa algorithm
	HAL_KEY_RSA_4096,
	/* ECC */
	/* it doesn't support whole algorithm that mbedTLS support. */
	HAL_KEY_ECC_BRAINPOOL_P256R1, // ecc brainpool curve for p256r1
	HAL_KEY_ECC_BRAINPOOL_P384R1, // ecc brainpool curve for p384r1
	HAL_KEY_ECC_BRAINPOOL_P512R1, // ecc brainpool curve for p512r1
	HAL_KEY_ECC_SEC_P192R1,		  // nist curve for p192r1
	HAL_KEY_ECC_SEC_P224R1,		  // nist curve for p224r1
	HAL_KEY_ECC_SEC_P256R1,		  // nist curve for p256r1
	HAL_KEY_ECC_SEC_P384R1,		  // nist curve for p384r1
	HAL_KEY_ECC_SEC_P512R1,		  // nist curve for p512r1
	HAL_KEY_ECC_25519,			  // Montgomery curve for key exchange
	HAL_KEY_ED_25519, // Twisted Edwards curve for signing and verification
	/* HMAC */
	HAL_KEY_HMAC_MD5,	 // hmac with md5
	HAL_KEY_HMAC_SHA1,	 // hmac with sha1
	HAL_KEY_HMAC_SHA224, // hmac with sha224
	HAL_KEY_HMAC_SHA256, // hmac with sha256
	HAL_KEY_HMAC_SHA384, // hmac with sha384
	HAL_KEY_HMAC_SHA512, // hmac with sha512
	/* DH */
	HAL_KEY_DH_1024,
	HAL_KEY_DH_2048,
	HAL_KEY_DH_4096,
	/* COMMON */
	HAL_KEY_UNKNOWN,
} hal_key_type;

typedef enum {
	HAL_GCM_AES,
	HAL_GCM_UNKNOWN,
} hal_gcm_type;

typedef struct _hal_init_param {
	uint32_t i2c_port;
	uint32_t gpio;
} hal_init_param;

typedef struct _hal_data {
	void *data;
	uint32_t data_len;
	void *priv;
	uint32_t priv_len;
} hal_data;

typedef struct _hal_rsa_mode {
	hal_rsa_algo rsa_a;
	hal_hash_type hash_t;
	hal_hash_type mgf;
	uint32_t salt_byte_len;
} hal_rsa_mode;

typedef struct _hal_aes_param {
	hal_aes_algo mode;
	unsigned char *iv;
	unsigned int iv_len;
	unsigned int *iv_offset;
	unsigned int *nc_off;
	/* The 128-bit nonce and counter.
	 * Refer mbedtls_aes_crypt_ctr() */
	unsigned char *nonce_counter;
	/* The saved stream block for resuming. This is
   * overwritten by the function.
	 * Refer mbedtls_aes_crypt_ctr() */
	unsigned char *stream_block;
} hal_aes_param;

typedef struct _hal_ecdsa_mode {
	hal_ecdsa_curve curve;
	hal_hash_type hash_t;
} hal_ecdsa_mode;

typedef struct _hal_dh_data {
	hal_dh_key_type mode;
	hal_data *G;
	hal_data *P;
	hal_data *pubkey;
} hal_dh_data;

typedef struct _hal_ecdh_data {
	hal_ecdsa_curve curve;
	hal_data *pubkey_x;	// peer's public key
	hal_data *pubkey_y; // peer's public key
} hal_ecdh_data;

/*
 * @brief   Secure Storage Information
 *
 * @note    TizenRT doesn't use it anymore.
 *          SS slot size is fixed by chipset.
 *          Also, ss data size will be provided by hal_data(parameter of hal_write_storage/hal_read_storage).
 */
DEPRECATED typedef struct _hal_ss_info {
	unsigned int size;
	struct _hal_ss_info *next;
} hal_ss_info;

typedef struct _hal_gcm_param {
	hal_gcm_type cipher;
	unsigned char *iv;
	unsigned int iv_len;
	/* Additional Authentication Data to generate tag */
	unsigned char *aad; 
	unsigned int aad_len;
	/* Authenentication Tag (This value is overwritten by the encryption function) */
	unsigned char *tag; 
	unsigned int tag_len;
} hal_gcm_param;

#define HAL_AES_PARAM_INITIALIZER                        \
	{                                                    \
		HAL_AES_UNKNOWN, NULL, 0, NULL, NULL, NULL, NULL \
	}
#define HAL_INIT_AES_PARAM(param) \
	hal_aes_param param = HAL_AES_PARAM_INITIALIZER

#define HAL_RSA_PARAM_INITIALIZER                                 \
	{                                                             \
		HAL_RSASSA_UNKNOWN, HAL_HASH_UNKNOWN, HAL_HASH_UNKNOWN, 0 \
	}
#define HAL_INIT_RSA_PARAM(param) \
	hal_rsa_mode param = HAL_RSA_PARAM_INITIALIZER

#define HAL_ECDSA_PARAM_INITIALIZER         \
	{                                       \
		HAL_ECDSA_UNKNOWN, HAL_HASH_UNKNOWN \
	}
#define HAL_INIT_ECDSA_PARAM(param) \
	hal_ecdsa_mode param = HAL_ECDSA_PARAM_INITIALIZER

#define HAL_INIT_DATA(data) \
	hal_data data = HAL_DATA_INITIALIZER

#define HAL_DH_INITIALIZER               \
	{                                    \
		HAL_DH_UNKNOWN, NULL, NULL, NULL \
	}
#define HAL_INIT_DH(dh) \
	hal_dh_data dh = HAL_DH_INITIALIZER

#define HAL_ECDH_INITIALIZER          \
	{                                 \
		HAL_ECDSA_UNKNOWN, NULL, NULL \
	}
#define HAL_INIT_ECDH(ecdh) \
	hal_ecdh_data ecdh = HAL_ECDH_INITIALIZER

#define HAL_GCM_PARAM_INITIALIZER                         \
	{                                                     \
		HAL_GCM_UNKNOWN, NULL, 0, NULL, 0, NULL, 0 \
	}

#define HAL_INIT_GCM_PARAM(param) \
	hal_gcm_param param = HAL_GCM_PARAM_INITIALIZER

/**
 * Common
 */
/**
 * @brief  Initialize SE HAL
 *
 * @param[in]   params   A pointer to the parameter structure for SE initialization.
 *                       - i2c_port: The I2C port number to be used.
 *                       - gpio: The GPIO pin number to be used.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully initialized.
 * @retval  HAL_NOT_INITIALIZED       Required resources for initialization are not ready.
 * @retval  HAL_INVALID_ARGS          Input parameters are invalid.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note   This function is responsible for setting up the secure channel.
 *         For example, it performs tasks such as opening GPIO/I2C used by the SE
 *         and creating a secure channel with the AP.
 *         GPIO/I2C is necessary if SE module is in external chipset.
 *         If SE module is in internal chipset, then no need to allocate GPIO/I2C param,
 *         but init/deinit process is necessary to use SE HAL modules.
 */
typedef int (*hal_init)(_IN_ hal_init_param *params);

/**
 * @brief  Deinitialize SE HAL
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully deinitialized.
 * @retval  HAL_FAIL                  An error occurred during deinitialization.
 *
 * @note   This function is responsible for deinitializing the secure channel.
 *         It releases allocated resources and closes the communication channel.
 */
typedef int (*hal_deinit)(void);

/**
 * @brief  Free memory allocated within HAL
 *
 * @param[in]   data    A pointer to the HAL data structure to be freed.
 *                      - data: Pointer to the data buffer to be freed.
 *                      - data_len: Length of the data buffer.
 *                      - priv: Pointer to the private data buffer to be freed.
 *                      - priv_len: Length of the private data buffer.
 *                      The internal buffers (data, priv) pointed to by this structure will be freed.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully freed memory.
 * @retval  HAL_INVALID_ARGS          Input parameters are invalid.
 * @retval  HAL_FAIL                  An error occurred during memory deallocation.
 *
 * @note   This function is used to safely free memory dynamically allocated within the HAL layer.
 *         The caller should use this function to free data buffers within the hal_data structure.
 */
typedef int (*hal_free_data)(_IN_ hal_data *data);

/**
 * @brief    Get the current status of the SE (Secure Element)
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               SE is in a normal (IDLE) state.
 * @retval  HAL_BUSY                  SE is currently in use (BUSY).
 * @retval  HAL_NOT_INITIALIZED       SE has not been initialized.
 * @retval  HAL_FAIL                  An error occurred while checking the status.
 *
 * @note    This function checks the current operational status of the SE.
 *          While each HAL API should also perform BUSY/IDLE checks internally,
 *          this function allows the caller to check the SE's status in advance.
 */
typedef int (*hal_get_status)(void);

/**
 * Key Manager
 */
/**
 * @brief   Set a key in the SE (Secure Element)
 *
 * @param[in]   mode    The type of the key to be stored (refer to hal_key_type enum).
 *                      e.g., HAL_KEY_AES_256, HAL_KEY_RSA_2048, HAL_KEY_ECC_SEC_P256R1
 * @param[in]   key_idx The key index within the SE where the key will be saved.
 *                      - Available key index : 32 ~ 63
 * @param[in]   key     The public key(or the key itself for symmetric keys) data to be saved.
 *                      - data: Pointer to the key data buffer.
 *                      - data_len: Length of the key data buffer.
 * @param[in]   prikey  The private key data to be saved (only used for asymmetric keys).
 *                      - data: Pointer to the private key data buffer.
 *                      - data_len: Length of the private key data buffer.
 *                      Used when storing private keys for asymmetric key algorithms like RSA or ECC.
 * 
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully stored the key.
 * @retval  HAL_KEY_IN_USE            The key_idx slot is already in use.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, key_idx, key) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    key_idx is out of the valid slot range.
 * @retval  HAL_INVALID_SLOT_TYPE     The requested key type cannot be stored in that slot.
 * @retval  HAL_NOT_ENOUGH_MEMORY     Not enough memory to store the key.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note     If the key type is asymmetric (RSA, ECC, etc.), the private key must be assigned to prikey->data.
 *          For symmetric keys (AES, HMAC, etc.), prikey can be passed as NULL.
 */
typedef int (*hal_set_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _IN_ hal_data *key, _IN_ hal_data *prikey);

/**
 * @brief   Get a public key from the SE (Secure Element)
 *
 * @param[in]   mode    The type of the key to be retrieved (refer to hal_key_type enum).
 * @param[in]   key_idx The key index within the SE from which the key will be extracted.
 *                      - Available key index : 0 ~ 63
 * @param[out]  key     A pointer to the structure where the retrieved public key data will be saved.
 *                      - data: Pointer to the buffer where the public key data will be stored.
 *                      The caller must allocate this buffer.
 *                      - data_len: Length of the retrieved public key data.
 *                      For ECC keys, the X coordinate is stored in key->data and the Y coordinate in key->priv.
 * 
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully retrieved the key.
 * @retval  HAL_EMPTY_SLOT            The key_idx slot is empty.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, key_idx, key) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    key_idx is out of the valid slot range.
 * @retval  HAL_BAD_KEY               The key in the slot does not match the requested type or is corrupted.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer is insufficient to store the key data.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note    For ECC keys, the X coordinate of the public key is stored in key->data and the Y coordinate in key->priv.
 *          This function only retrieves the public key; the private key is not extracted from the SE.
 */
typedef int (*hal_get_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _OUT_ hal_data *key);

/**
 * @brief   Remove a key in the SE (Secure Element)
 *
 * @param[in]   mode    The type of the key to be removed (refer to hal_key_type enum).
 * @param[in]   key_idx The key index within the SE where the key to be removed is stored.
 *                      - Available key index : 32 ~ 63
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully removed the key.
 * @retval  HAL_EMPTY_SLOT            The key_idx slot is already empty.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, key_idx) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    key_idx is out of the valid slot range.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note    This function permanently removes the key stored in the specified slot (key_idx).
 *          Once removed, the key cannot be recovered.
 */
typedef int (*hal_remove_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx);

/**
 * @brief    Generate a key within the SE (Secure Element)
 *
 * @param[in]   mode    The type of the key to be generated (refer to hal_key_type enum).
 *                      e.g., HAL_KEY_AES_256, HAL_KEY_RSA_2048, HAL_KEY_ECC_SEC_P256R1
 * @param[in]   key_idx The key index within the SE where the generated key will be stored.
 *                      - Available key index : 32 ~ 63
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully generated and stored the key.
 * @retval  HAL_KEY_IN_USE            The key_idx slot is already in use.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, key_idx) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    key_idx is out of the valid slot range.
 * @retval  HAL_INVALID_SLOT_TYPE     The requested key type cannot be generated in that slot.
 * @retval  HAL_NOT_SUPPORTED         Generation of that key type is not supported.
 * @retval  HAL_FAIL                  An internal error occurred during key generation.
 *
 * @note    This function securely generates a key inside the SE.
 *          The generated key is not exposed outside the SE and is stored in the specified slot (key_idx).
 *          For asymmetric keys (RSA, ECC, etc.), a public/private key pair is generated together.
 */
typedef int (*hal_generate_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx);

/**
 * Authenticate
 */
/**
 * @brief Generate random data
 *
 * @param[in]   len     The length of the random data to be generated in bytes.
 * @param[out]  random  A pointer to the structure where the generated random data will be stored.
 *                      - data: Pointer to the buffer where the random data will be stored.
 *                              The caller must allocate this buffer.
 *                      - data_len: Length of the generated random data (should be equal to `len`).
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully generated random data.
 * @retval  HAL_INVALID_ARGS          Input parameters (len, random) are invalid.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (random->data) is insufficient to store the random data.
 * @retval  HAL_FAIL                  An internal error occurred during random number generation.
 *
 * @note    This function utilizes the SE's hardware random number generator to create
 *          cryptographically secure random data.
 */
typedef int (*hal_generate_random)(_IN_ uint32_t len, _OUT_ hal_data *random);

/**
 * @brief Get the hash of input data
 *
 * @param[in]   mode    The hash algorithm to be used (refer to hal_hash_type enum).
 *                      e.g., HAL_HASH_SHA256, HAL_HASH_SHA512
 * @param[in]   input   A pointer to the structure containing the input data to be hashed.
 *                      - data: Pointer to the input data buffer.
 *                      - data_len: Length of the input data buffer.
 * @param[out]  hash    A pointer to the structure where the resulting hash will be stored.
 *                      - data: Pointer to the buffer where the hash will be stored.
 *                              The caller must allocate this buffer. The required size depends on the hash algorithm.
 *                      - data_len: Length of the generated hash.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully computed the hash.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, input, hash) are invalid.
 * @retval  HAL_NOT_SUPPORTED         The requested hash algorithm is not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (hash->data) is insufficient to store the hash.
 * @retval  HAL_FAIL                  An internal error occurred during hashing.
 *
 * @note    The caller must ensure that the `hash->data` buffer is large enough to hold
 *          the output of the specified hash algorithm (e.g., 32 bytes for SHA-256, 64 bytes for SHA-512).
 */
typedef int (*hal_get_hash)(_IN_ hal_hash_type mode, _IN_ hal_data *input, _OUT_ hal_data *hash);

/**
 * @brief Get the HMAC (Hash-based Message Authentication Code) of input data
 *
 * @param[in]   mode    The HMAC algorithm to be used (refer to hal_hmac_type enum).
 *                      e.g., HAL_HMAC_SHA256, HAL_HMAC_SHA512
 * @param[in]   input   A pointer to the structure containing the input data.
 *                      - data: Pointer to the input data buffer.
 *                      - data_len: Length of the input data buffer.
 * @param[in]   key_idx The key index of the secret key to be used for HMAC calculation.
 *                      This key must have been previously stored (using `hal_set_key`).
 * 					    Also, secret key is basically symmetric key.
 *                      - Available key index : 0 ~ 63
 * @param[out]  hmac    A pointer to the structure where the resulting HMAC will be stored.
 *                      - data: Pointer to the buffer where the HMAC will be stored.
 *                              The caller must allocate this buffer. The required size depends on the HMAC algorithm.
 *                      - data_len: Length of the generated HMAC.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully computed the HMAC.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, input, key_idx, hmac) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not suitable for the requested HMAC operation.
 * @retval  HAL_NOT_SUPPORTED         The requested HMAC algorithm is not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (hmac->data) is insufficient to store the HMAC.
 * @retval  HAL_FAIL                  An internal error occurred during HMAC calculation.
 *
 * @note    The caller must ensure that the `hmac->data` buffer is large enough to hold
 *          the output of the specified HMAC algorithm (e.g., 32 bytes for HMAC-SHA256).
 */
typedef int (*hal_get_hmac)(_IN_ hal_hmac_type mode, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac);

/**
 * @brief Get a digital signature for a message digest using RSA
 *
 * @param[in]   mode    A pointer to the structure defining RSA signing parameters.
 *                      - rsa_a: The RSA signature algorithm (e.g., HAL_RSASSA_PKCS1_V1_5).
 *                      - hash_t: The hash algorithm used to create the message digest.
 *                      - mgf: The mask generation function (for PSS).
 *                      - salt_byte_len: The length of the salt in bytes (for PSS).
 * @param[in]   hash    A pointer to the structure containing the message digest to be signed.
 *                      - data: Pointer to the message digest buffer.
 *                      - data_len: Length of the message digest buffer.
 * @param[in]   key_idx The key index of the RSA private key to be used for signing.
 *                      - Available key index : 0 ~ 63 
 * @param[out]  sign    A pointer to the structure where the generated signature will be stored.
 *                      - data: Pointer to the buffer where the signature will be stored.
 *                              The caller must allocate this buffer.
 *                      - data_len: Length of the generated signature.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully generated the signature.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, hash, key_idx, sign) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an RSA private key.
 * @retval  HAL_NOT_SUPPORTED         The requested RSA signature algorithm or parameters are not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (sign->data) is insufficient to store the signature.
 * @retval  HAL_FAIL                  An internal error occurred during signing.
 *
 * @note    The input `hash` is the message digest, not the original message.
 *          The message digest should be computed using the hash algorithm specified in `mode->hash_t`.
 *          The size of the `sign->data` buffer should be at least the size of the RSA key modulus (e.g., 256 bytes for RSA-2048).
 */
typedef int (*hal_rsa_sign_md)(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign);

/**
 * @brief Verify an RSA digital signature for a message digest
 *
 * @param[in]   mode    A pointer to the structure defining RSA verification parameters.
 *                      - rsa_a: The RSA signature algorithm (e.g., HAL_RSASSA_PKCS1_V1_5).
 *                      - hash_t: The hash algorithm used to create the message digest.
 *                      - mgf: The mask generation function (for PSS).
 *                      - salt_byte_len: The length of the salt in bytes (for PSS).
 * @param[in]   hash    A pointer to the structure containing the message digest.
 *                      - data: Pointer to the message digest buffer.
 *                      - data_len: Length of the message digest buffer.
 * @param[in]   sign    A pointer to the structure containing the signature to be verified.
 *                      - data: Pointer to the signature buffer.
 *                      - data_len: Length of the signature buffer.
 * @param[in]   key_idx The key index of the RSA public key to be used for verification.
 *                      - Available key index : 0 ~ 63 
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               The signature is valid.
 * @retval  HAL_FAIL                  The signature is invalid.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, hash, sign, key_idx) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an RSA public key.
 * @retval  HAL_NOT_SUPPORTED         The requested RSA signature algorithm or parameters are not supported.
 * @retval  HAL_BAD_KEY_PAIR          The public key does not match the private key used for signing (if applicable).
 *
 * @note    This function verifies the signature against the provided message digest.
 *          The message digest should be computed using the hash algorithm specified in `mode->hash_t`.
 */
typedef int (*hal_rsa_verify_md)(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx);

/**
 * @brief Get a digital signature for a message digest using ECDSA
 *
 * @param[in]   mode    A pointer to the structure defining ECDSA signing parameters.
 *                      - curve: The ECDSA curve to be used (e.g., HAL_ECDSA_SEC_P256R1).
 *                      - hash_t: The hash algorithm used to create the message digest.
 * @param[in]   hash    A pointer to the structure containing the message digest to be signed.
 *                      - data: Pointer to the message digest buffer.
 *                      - data_len: Length of the message digest buffer.
 * @param[in]   key_idx The key index of the ECDSA private key to be used for signing.
 *                      - Available key index : 0 ~ 63 
 * @param[out]  sign    A pointer to the structure where the generated signature will be stored.
 *                      - data: Pointer to the buffer where the signature will be stored.
 *                              The caller must allocate this buffer. The signature typically consists of (r, s) values.
 *                      - data_len: Length of the generated signature.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully generated the signature.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, hash, key_idx, sign) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an ECDSA private key or does not match the specified curve.
 * @retval  HAL_NOT_SUPPORTED         The requested ECDSA curve or hash algorithm is not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (sign->data) is insufficient to store the signature.
 * @retval  HAL_FAIL                  An internal error occurred during signing.
 *
 * @note    The input `hash` is the message digest, not the original message.
 *          The message digest should be computed using the hash algorithm specified in `mode->hash_t`.
 *          The size of the `sign->data` buffer depends on the curve (e.g., 64 bytes for P-256 for r and s).
 */
typedef int (*hal_ecdsa_sign_md)(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign);

/**
 * @brief Verify an ECDSA digital signature for a message digest
 *
 * @param[in]   mode    A pointer to the structure defining ECDSA verification parameters.
 *                      - curve: The ECDSA curve to be used (e.g., HAL_ECDSA_SEC_P256R1).
 *                      - hash_t: The hash algorithm used to create the message digest.
 * @param[in]   hash    A pointer to the structure containing the message digest.
 *                      - data: Pointer to the message digest buffer.
 *                      - data_len: Length of the message digest buffer.
 * @param[in]   sign    A pointer to the structure containing the signature to be verified.
 *                      - data: Pointer to the signature buffer (contains r and s values).
 *                      - data_len: Length of the signature buffer.
 * @param[in]   key_idx The key index of the ECDSA public key to be used for verification.
 *                      - Available key index : 0 ~ 63 
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               The signature is valid.
 * @retval  HAL_FAIL                  The signature is invalid.
 * @retval  HAL_INVALID_ARGS          Input parameters (mode, hash, sign, key_idx) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an ECDSA public key or does not match the specified curve.
 * @retval  HAL_NOT_SUPPORTED         The requested ECDSA curve or hash algorithm is not supported.
 * @retval  HAL_BAD_KEY_PAIR          The public key does not match the private key used for signing.
 *
 * @note    This function verifies the signature against the provided message digest.
 *          The message digest should be computed using the hash algorithm specified in `mode->hash_t`.
 */
typedef int (*hal_ecdsa_verify_md)(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx);

/**
 * @brief Generate Diffie-Hellman (DH) parameters
 *
 * @param[in]     dh_idx  The key index where the generated private key X will be stored.
 *                        - Available dh index : 32 ~ 63 
 * @param[in,out] dh_param A pointer to the structure for DH parameters.
 *                      - mode: [in] The DH key type (e.g., HAL_DH_2048).
 *                      - G: [in] A pointer to the structure containing the generator G.
 *                      - P: [in] A pointer to the structure containing the prime modulus P.
 *                      - pubkey: [out] A pointer to the structure where the public key G^X mod P will be stored.
 *                                The caller must allocate the data buffer for pubkey.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully generated DH parameters and public key.
 * @retval  HAL_INVALID_ARGS          Input parameters (dh_idx, dh_param, G, P) are invalid.
 * @retval  HAL_KEY_IN_USE            The dh_idx slot is already in use.
 * @retval  HAL_INVALID_SLOT_RANGE    dh_idx is out of the valid slot range.
 * @retval  HAL_NOT_SUPPORTED         The requested DH key type is not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (dh_param->pubkey->data) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during parameter generation.
 *
 * @note    This function generates a private key X, protects it inside the SE at slot `dh_idx`,
 *          and computes the public key G^X mod P, which is stored in `dh_param->pubkey`.
 *          The generator G and prime modulus P must be provided as input.
 *          The private key X stored in the SE must be removed using `hal_remove_key()` when no longer needed.
 */
typedef int (*hal_dh_generate_param)(_IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param);

/**
 * @brief Compute the Diffie-Hellman (DH) shared secret
 *
 * @param[in]   dh_param       A pointer to the structure containing DH parameters from the peer.
 *                             - mode: The DH key type (must match the one used in `hal_dh_generate_param`).
 *                             - pubkey: A pointer to the structure containing the peer's public key G^Y mod P.
 * @param[in]   dh_idx         The key index where the local private key X is stored (generated by `hal_dh_generate_param` or pre-stored).
 *                             - Available dh index : 0 ~ 63
 * @param[out]  shared_secret  A pointer to the structure where the computed shared secret will be stored.
 *                             - data: Pointer to the buffer where the shared secret (G^XY mod P) will be stored.
 *                                     The caller must allocate this buffer.
 *                             - data_len: Length of the computed shared secret.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully computed the shared secret.
 * @retval  HAL_INVALID_ARGS          Input parameters (dh_param, dh_idx, shared_secret) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified dh_idx slot is empty (local private key X not found).
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not a DH key or does not match dh_param->mode.
 * @retval  HAL_NOT_SUPPORTED         The DH operation is not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (shared_secret->data) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during shared secret computation.
 *
 * @note    This function uses the local private key X (from `dh_idx`) and the peer's public key
 *          (from `dh_param->pubkey`) to compute the shared secret G^XY mod P.
 */
typedef int (*hal_dh_compute_shared_secret)(_IN_ hal_dh_data *dh_param, _IN_ uint32_t dh_idx, _OUT_ hal_data *shared_secret);

/**
 * @brief Compute the Elliptic Curve Diffie-Hellman (ECDH) shared secret
 *
 * @param[in]   ecdh_mode      A pointer to the structure defining ECDH parameters and the peer's public key.
 *                             - curve: The ECDSA curve to be used (e.g., HAL_ECDSA_SEC_P256R1).
 *                             - pubkey_x: A pointer to the structure containing the X coordinate of the peer's public key.
 *                             - pubkey_y: A pointer to the structure containing the Y coordinate of the peer's public key.
 * @param[in]   key_idx        The key index of the local ECDH private key.
 *                             - Available key index : 0 ~ 63
 * @param[out]  shared_secret  A pointer to the structure where the computed shared secret will be stored.
 *                             - data: Pointer to the buffer where the shared secret will be stored.
 *                                     The caller must allocate this buffer.
 *                             - data_len: Length of the computed shared secret.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully computed the shared secret.
 * @retval  HAL_INVALID_ARGS          Input parameters (ecdh_mode, key_idx, shared_secret) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty (local private key not found).
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an ECDH private key or does not match ecdh_mode->curve.
 * @retval  HAL_NOT_SUPPORTED         The ECDH operation for the specified curve is not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (shared_secret->data) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during shared secret computation.
 *
 * @note    This function uses the local private key (from `key_idx`) and the peer's public key
 *          (from `ecdh_mode->pubkey_x` and `ecdh_mode->pubkey_y`) to compute the shared secret.
 *          The `pubkey` in `ecdh_mode` denotes the public key from the target with which the secret is shared.
 */
typedef int (*hal_ecdh_compute_shared_secret)(_IN_ hal_ecdh_data *ecdh_mode, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret);

/**
 * @brief Set a certificate in the SE (Secure Element)
 *
 * @param[in]   cert_idx The certificate index within the SE where the certificate will be saved.
 *                       - Available cert index : 32 ~ 63
 * @param[in]   cert_in  A pointer to the structure containing the certificate data to be stored.
 *                       - data: Pointer to the certificate data buffer (e.g., DER encoded X.509 certificate).
 *                       - data_len: Length of the certificate data buffer.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully stored the certificate.
 * @retval  HAL_CERT_IN_USE           The cert_idx slot is already in use.
 * @retval  HAL_INVALID_ARGS          Input parameters (cert_idx, cert_in) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    cert_idx is out of the valid certificate slot range.
 * @retval  HAL_BAD_CERT              The provided certificate data is corrupted or malformed.
 * @retval  HAL_NOT_ENOUGH_MEMORY     Not enough memory to store the certificate.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note    The certificate data should typically be in DER format.
 */
typedef int (*hal_set_certificate)(_IN_ uint32_t cert_idx, _IN_ hal_data *cert_in);

/**
 * @brief Get a certificate from the SE (Secure Element)
 *
 * @param[in]   cert_idx The certificate index within the SE from which the certificate will be retrieved.
 *                       - Available cert index : 0 ~ 63
 * @param[out]  cert_out A pointer to the structure where the retrieved certificate data will be saved.
 *                       - data: Pointer to the buffer where the certificate data will be stored.
 *                               The caller must allocate this buffer.
 *                       - data_len: Length of the retrieved certificate data.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully retrieved the certificate.
 * @retval  HAL_EMPTY_SLOT            The cert_idx slot is empty.
 * @retval  HAL_INVALID_ARGS          Input parameters (cert_idx, cert_out) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    cert_idx is out of the valid certificate slot range.
 * @retval  HAL_BAD_CERT              The certificate in the slot is corrupted.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (cert_out->data) is insufficient to store the certificate.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 */
typedef int (*hal_get_certificate)(_IN_ uint32_t cert_idx, _OUT_ hal_data *cert_out);

/**
 * @brief Remove a certificate in the SE (Secure Element)
 *
 * @param[in]   cert_idx The certificate index within the SE where the certificate to be removed is stored.
 *                       - Available cert index : 32 ~ 63
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully removed the certificate.
 * @retval  HAL_EMPTY_SLOT            The cert_idx slot is already empty.
 * @retval  HAL_INVALID_ARGS          Input parameter (cert_idx) is invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    cert_idx is out of the valid certificate slot range.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note    This function permanently removes the certificate stored in the specified slot (cert_idx).
 */
typedef int (*hal_remove_certificate)(_IN_ uint32_t cert_idx);

/*
 * @brief Get factory key
 *
 * @return HAL_SUCCESS(0) on success, otherwise a positive error value.
 *
 * @note   TizenRT 3.1 doesn't use it anymore. Instead of the API, TizenRT separate
 *         slot range for factory key which is injected from 0 ~ 31 and for ram key
 *         which is volitile from 32 ~ 63. So use hal_get_key.
 */

DEPRECATED typedef int (*hal_get_factory_key)(_IN_ uint32_t key_idx, _IN_ hal_data *key);

/*
 * @brief Get factory cert
 *
 * @return HAL_SUCCESS(0) on success, otherwise a positive error value.
 */
DEPRECATED typedef int (*hal_get_factory_cert)(_IN_ uint32_t cert_idx, _IN_ hal_data *cert);

/*
 * @brief Get factory data
 *
 * @return HAL_SUCCESS(0) on success, otherwise a positive error value.
 */
DEPRECATED typedef int (*hal_get_factory_data)(_IN_ uint32_t data_idx, _IN_ hal_data *data);

/**
 * Crypto
 */
/**
 * @brief   Encrypt data using AES
 *
 * @param[in]   dec_data    A pointer to the structure containing the plaintext data to be encrypted.
 *                          - data: Pointer to the plaintext data buffer.
 *                          - data_len: Length of the plaintext data buffer.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 * @param[in]   aes_param   A pointer to the structure defining AES encryption parameters.
 *                          - mode: The AES algorithm mode (e.g., HAL_AES_CBC_NOPAD, HAL_AES_CTR).
 *                          - iv: Pointer to the initialization vector (IV) buffer.
 *                          - iv_len: Length of the IV buffer.
 *                          - iv_offset: Pointer to the current offset in the IV (for CFB mode).
 *                          - nc_off: Pointer to the nonce counter offset (for CTR mode).
 *                          - nonce_counter: Pointer to the 128-bit nonce and counter buffer (for CTR mode).
 *                          - stream_block: Pointer to the saved stream block for resuming (for CTR mode).
 * @param[in]   key_idx     The key index of the AES key to be used for encryption.
 *                          - Available key index : 0 ~ 63
 * @param[out]  enc_data    A pointer to the structure where the resulting ciphertext will be stored.
 *                          - data: Pointer to the buffer where the ciphertext will be stored.
 *                                  The caller must allocate this buffer.
 *                          - data_len: Length of the resulting ciphertext.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully encrypted the data.
 * @retval  HAL_INVALID_ARGS          Input parameters (dec_data, aes_param, key_idx, enc_data) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an AES key or does not match the operation.
 * @retval  HAL_NOT_SUPPORTED         The requested AES mode or parameters are not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (enc_data->data) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during encryption.
 *
 * @note    The `hal_aes_param` structure may be updated by this function depending on the `aes_param->mode`.
 *          - If `aes_param->mode` is HAL_AES_CBC_NOPAD, `aes_param->iv` will be updated.
 *          - If `aes_param->mode` is HAL_AES_CTR, `aes_param->nc_off`, `aes_param->nonce_counter`, and `aes_param->stream_block` will be updated.
 *          - If `aes_param->mode` is HAL_AES_CFB128, `aes_param->iv` and `aes_param->iv_offset` will be updated.
 *          If hal_aes_algo is _NOPAD, input data should be aligned to 16 bytes blocks.
 *          All hal_aes_algo except _NOPAD, padding should be added while encryption process.
 *          (It's unnecessary to remove padding while decryption process)
 *          For more detailed information on parameter requirements and updates, refer to mbedtls/aes.h.
 */
typedef int (*hal_aes_encrypt)(_IN_ hal_data *dec_data,
							   _IN_ hal_aes_param *aes_param,
							   _IN_ uint32_t key_idx,
							   _OUT_ hal_data *enc_data);

/**
 * @brief    Decrypt data using AES
 *
 * @param[in]   enc_data    A pointer to the structure containing the ciphertext data to be decrypted.
 *                          - data: Pointer to the ciphertext data buffer.
 *                          - data_len: Length of the ciphertext data buffer.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 * @param[in]   aes_param   A pointer to the structure defining AES decryption parameters.
 *                          - mode: The AES algorithm mode (e.g., HAL_AES_CBC_NOPAD, HAL_AES_CTR).
 *                          - iv: Pointer to the initialization vector (IV) buffer.
 *                          - iv_len: Length of the IV buffer.
 *                          - iv_offset: Pointer to the current offset in the IV (for CFB mode).
 *                          - nc_off: Pointer to the nonce counter offset (for CTR mode).
 *                          - nonce_counter: Pointer to the 128-bit nonce and counter buffer (for CTR mode).
 *                          - stream_block: Pointer to the saved stream block for resuming (for CTR mode).
 * @param[in]   key_idx     The key index of the AES key to be used for decryption.
 *                          - Available key index : 0 ~ 63
 * @param[out]  dec_data    A pointer to the structure where the resulting plaintext will be stored.
 *                          - data: Pointer to the buffer where the plaintext will be stored.
 *                                  The caller must allocate this buffer.
 *                          - data_len: Length of the resulting plaintext.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 *
 * @return   HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully decrypted the data.
 * @retval  HAL_INVALID_ARGS          Input parameters (enc_data, aes_param, key_idx, dec_data) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an AES key or does not match the operation.
 * @retval  HAL_NOT_SUPPORTED         The requested AES mode or parameters are not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (dec_data->data) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during decryption.
 *
 * @note    The `hal_aes_param` structure may be updated by this function depending on the `aes_param->mode`.
 *          - If `aes_param->mode` is HAL_AES_CBC_NOPAD, `aes_param->iv` will be updated.
 *          - If `aes_param->mode` is HAL_AES_CTR, `aes_param->nc_off`, `aes_param->nonce_counter`, and `aes_param->stream_block` will be updated.
 *          - If `aes_param->mode` is HAL_AES_CFB128, `aes_param->iv` and `aes_param->iv_offset` will be updated.
 *          If padding was already added while encryption, hal_aes_decrypt is unnecessary to remove the padding.
 *          For more detailed information on parameter requirements and updates, refer to mbedtls/aes.h.
 */
typedef int (*hal_aes_decrypt)(_IN_ hal_data *enc_data,
							   _IN_ hal_aes_param *aes_param,
							   _IN_ uint32_t key_idx,
							   _OUT_ hal_data *dec_data);

/**
 * @brief    Encrypt data using RSA
 *
 * @param[in]   dec_data    A pointer to the structure containing the plaintext data to be encrypted.
 *                          - data: Pointer to the plaintext data buffer.
 *                          - data_len: Length of the plaintext data buffer.
 *                          The length of the plaintext must not exceed the RSA key size minus padding overhead.
 * @param[in]   mode        A pointer to the structure defining RSA encryption parameters.
 *                          - rsa_a: The RSA encryption algorithm
 *                          - hash_t: The hash algorithm
 *                          - mgf: The mask generation function
 *                          - salt_byte_len: The length of the salt in bytes
 * @param[in]   key_idx     The key index of the RSA public key to be used for encryption.
 *                          - Available key index : 0 ~ 63
 * @param[out]  enc_data    A pointer to the structure where the resulting ciphertext will be stored.
 *                          - data: Pointer to the buffer where the ciphertext will be stored.
 *                                  The caller must allocate this buffer. The size should be at least the RSA key modulus size (e.g., 256 bytes for RSA-2048).
 *                          - data_len: Length of the resulting ciphertext (typically the RSA key modulus size).
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully encrypted the data.
 * @retval  HAL_INVALID_ARGS          Input parameters (dec_data, mode, key_idx, enc_data) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an RSA public key.
 * @retval  HAL_NOT_SUPPORTED         The requested RSA encryption algorithm or parameters are not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (enc_data->data) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during encryption.
 *
 * @note    RSA is typically used to encrypt small amounts of data, such as symmetric keys for bulk data encryption.
 *          The plaintext size is limited by the RSA key size and the chosen padding scheme.
 */
typedef int (*hal_rsa_encrypt)(_IN_ hal_data *dec_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data);

/**
 * @brief    Decrypt data using RSA
 *
 * @param[in]   enc_data    A pointer to the structure containing the ciphertext data to be decrypted.
 *                          - data: Pointer to the ciphertext data buffer.
 *                          - data_len: Length of the ciphertext data buffer (typically the RSA key modulus size).
 * @param[in]   mode        A pointer to the structure defining RSA decryption parameters.
 *                          - rsa_a: The RSA encryption algorithm
 *                          - hash_t: The hash algorithm
 *                          - mgf: The mask generation function
 *                          - salt_byte_len: The length of the salt in bytes
 * @param[in]   key_idx     The key index of the RSA private key to be used for decryption.
 *                          - Available key index : 0 ~ 63
 * @param[out]  dec_data    A pointer to the structure where the resulting plaintext will be stored.
 *                          - data: Pointer to the buffer where the plaintext will be stored.
 *                                  The caller must allocate this buffer.
 *                          - data_len: Length of the resulting plaintext.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully decrypted the data.
 * @retval  HAL_INVALID_ARGS          Input parameters (enc_data, mode, key_idx, dec_data) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an RSA private key.
 * @retval  HAL_NOT_SUPPORTED         The requested RSA decryption algorithm or parameters are not supported.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (dec_data->data) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during decryption.
 *
 * @note    RSA is typically used to decrypt small amounts of data, such as symmetric keys that were encrypted for bulk data decryption.
 */
typedef int (*hal_rsa_decrypt)(_IN_ hal_data *enc_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data);

/**
 * @brief   Encrypt data with GCM (Galois/Counter Mode)
 *
 * @param[in]   dec_data    A pointer to the structure containing the plaintext data to be encrypted.
 *                          - data: Pointer to the plaintext data buffer.
 *                          - data_len: Length of the plaintext data buffer.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 * @param[in,out] gcm_param   A pointer to the structure defining GCM encryption parameters.
 *                          - cipher: The cipher type (e.g., HAL_GCM_AES).
 *                          - iv: Pointer to the initialization vector (IV) buffer.
 *                          - iv_len: Length of the IV buffer.
 *                          - aad: Pointer to the Additional Authenticated Data (AAD) buffer.
 *                          - aad_len: Length of the AAD buffer.
 *                          - tag: [out] Pointer to the buffer where the authentication tag will be stored.
 *                                  The caller must allocate this buffer.
 *                          - tag_len: [out] Length of the authentication tag.
 * @param[in]   key_idx     The key index of the AES key to be used for GCM encryption.
 *                          - Available key index : 0 ~ 63
 * @param[out]  enc_data    A pointer to the structure where the resulting ciphertext will be stored.
 *                          - data: Pointer to the buffer where the ciphertext will be stored.
 *                                  The caller must allocate this buffer.
 *                          - data_len: Length of the resulting ciphertext.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully encrypted the data and generated the authentication tag.
 * @retval  HAL_INVALID_ARGS          Input parameters (dec_data, gcm_param, key_idx, enc_data) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an AES key.
 * @retval  HAL_NOT_SUPPORTED         The requested GCM cipher type is not supported (currently only AES is supported).
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (enc_data->data) or tag buffer (gcm_param->tag) is insufficient.
 * @retval  HAL_FAIL                  An internal error occurred during encryption.
 *
 * @note    GCM is an authenticated encryption mode that provides both confidentiality and integrity.
 *          - Before encryption: `gcm_param->cipher`, `gcm_param->iv`, `gcm_param->iv_len`, (optional)`gcm_param->aad`, (optional)`gcm_param->aad_len` should be set.
 *          - After encryption: `gcm_param->tag` and `gcm_param->tag_len` are updated by the function.
 *          `gcm_param->aad`, `gcm_param->aad_len` is optional. (Even though `gcm_param->aad` == NULL and gcm_param->aad_len == 0, encryption works)
 *          The `tag` and `tag_len` do not need to be set by the caller before encryption.
 *          GCM currently only supports AES as the underlying cipher.
 */
typedef int (*hal_gcm_encrypt)(_IN_ hal_data *dec_data, _INOUT_ hal_gcm_param *gcm_param, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data);

/**
 * @brief   Decrypt data with GCM (Galois/Counter Mode)
 *
 * @param[in]   enc_data    A pointer to the structure containing the ciphertext data to be decrypted.
 *                          - data: Pointer to the ciphertext data buffer.
 *                          - data_len: Length of the ciphertext data buffer.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 * @param[in,out] gcm_param   A pointer to the structure defining GCM decryption parameters.
 *                          - cipher: The cipher type (e.g., HAL_GCM_AES).
 *                          - iv: Pointer to the initialization vector (IV) buffer (must be the same as used during encryption).
 *                          - iv_len: Length of the IV buffer.
 *                          - aad: Pointer to the Additional Authenticated Data (AAD) buffer (must be the same as used during encryption).
 *                          - aad_len: Length of the AAD buffer.
 *                          - tag: Pointer to the authentication tag buffer (generated by `hal_gcm_encrypt`).
 *                          - tag_len: Length of the authentication tag.
 * @param[in]   key_idx     The key index of the AES key to be used for GCM decryption.
 *                          - Available key index : 0 ~ 63
 * @param[out]  dec_data    A pointer to the structure where the resulting plaintext will be stored.
 *                          - data: Pointer to the buffer where the plaintext will be stored.
 *                                  The caller must allocate this buffer.
 *                          - data_len: Length of the resulting plaintext.
 *                          The `priv` and `priv_len` fields are not used for this parameter.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully decrypted the data and verified the authentication tag.
 * @retval  HAL_FAIL                  Decryption failed or authentication tag verification failed.
 * @retval  HAL_INVALID_ARGS          Input parameters (enc_data, gcm_param, key_idx, dec_data) are invalid.
 * @retval  HAL_EMPTY_SLOT            The specified key_idx slot is empty.
 * @retval  HAL_BAD_KEY_TYPE          The key in the slot is not an AES key.
 * @retval  HAL_NOT_SUPPORTED         The requested GCM cipher type is not supported (currently only AES is supported).
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (dec_data->data) is insufficient.
 *
 * @note    GCM is an authenticated encryption mode. This function decrypts the data and verifies the integrity
 *          and authenticity using the provided authentication tag.
 *          - Before decryption: `gcm_param->cipher`, `gcm_param->iv`, `gcm_param->iv_len`, (optional)`gcm_param->aad`, (optional)`gcm_param->aad_len`, `gcm_param->tag`, `gcm_param->tag_len` should be set.
 *          - After decryption: No fields in `gcm_param` are updated by this function.
 *          The `tag` and `tag_len` must be the same as those generated by `hal_gcm_encrypt`.
 *          `gcm_param->aad`, `gcm_param->aad_len` is optional. (Even though `gcm_param->aad` == NULL and gcm_param->aad_len == 0, decryption works)
 *          GCM currently only supports AES as the underlying cipher.
 */
typedef int (*hal_gcm_decrypt)(_IN_ hal_data *enc_data, _INOUT_ hal_gcm_param *gcm_param, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data);

/**
 * Secure Storage
 */
/**
 * @brief    Write data to the Secure Element's (SE) secure storage
 *
 * @param[in]   ss_idx  The secure storage index within the SE where the data will be written.
 *                      - Available ss index : 0 ~ 32
 * @param[in]   data    A pointer to the structure containing the data to be written.
 *                      - data: Pointer to the data buffer to be written.
 *                      - data_len: Length of the data buffer.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully wrote the data to secure storage.
 * @retval  HAL_DATA_IN_USE           The ss_idx slot is already in use.
 * @retval  HAL_INVALID_ARGS          Input parameters (ss_idx, data) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    ss_idx is out of the valid secure storage slot range.
 * @retval  HAL_NOT_ENOUGH_MEMORY     Not enough memory in the SE's secure storage to write the data.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note    This function writes general-purpose data to a protected area within the SE.
 *          The data is stored persistently.
 */
typedef int (*hal_write_storage)(_IN_ uint32_t ss_idx, _IN_ hal_data *data);

/**
 * @brief    Read data from the Secure Element's (SE) secure storage
 *
 * @param[in]   ss_idx  The secure storage index within the SE from which the data will be read.
 *                      - Available ss index : 0 ~ 32
 * @param[out]  data    A pointer to the structure where the retrieved data will be stored.
 *                      - data: Pointer to the buffer where the data from secure storage will be stored.
 *                              The caller must allocate this buffer.
 *                      - data_len: Length of the retrieved data.
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully read the data from secure storage.
 * @retval  HAL_EMPTY_SLOT            The ss_idx slot is empty.
 * @retval  HAL_INVALID_ARGS          Input parameters (ss_idx, data) are invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    ss_idx is out of the valid secure storage slot range.
 * @retval  HAL_NOT_ENOUGH_MEMORY     The output buffer (data->data) is insufficient to store the data.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 */
typedef int (*hal_read_storage)(_IN_ uint32_t ss_idx, _OUT_ hal_data *data);

/**
 * @brief    Delete data in the Secure Element's (SE) secure storage
 *
 * @param[in]   ss_idx  The secure storage index within the SE where the data to be deleted is stored.
 *                      - Available ss index : 0 ~ 32
 *
 * @return  HAL_SUCCESS(0) on success, otherwise a positive error value.
 * @retval  HAL_SUCCESS               Successfully deleted the data from secure storage.
 * @retval  HAL_EMPTY_SLOT            The ss_idx slot is already empty.
 * @retval  HAL_INVALID_ARGS          Input parameter (ss_idx) is invalid.
 * @retval  HAL_INVALID_SLOT_RANGE    ss_idx is out of the valid secure storage slot range.
 * @retval  HAL_FAIL                  Other internal errors occurred.
 *
 * @note    This function permanently deletes the data stored in the specified secure storage slot (ss_idx).
 */
typedef int (*hal_delete_storage)(_IN_ uint32_t ss_idx);

/**
 * @brief    Seclink driver operation
 * 
 * @note    Chipset vendor should fill out below ops structure
 *          to make sure that seclink driver can map the command to the proper SE HAL APIs.
 */
struct sec_ops_s {
	hal_init init;
	hal_deinit deinit;
	hal_free_data free_data;
	hal_get_status get_status;
	hal_set_key set_key;
	hal_get_key get_key;
	hal_remove_key remove_key;
	hal_generate_key generate_key;
	hal_generate_random generate_random;
	hal_get_hash get_hash;
	hal_get_hmac get_hmac;
	hal_rsa_sign_md rsa_sign_md;
	hal_rsa_verify_md rsa_verify_md;
	hal_ecdsa_sign_md ecdsa_sign_md;
	hal_ecdsa_verify_md ecdsa_verify_md;
	hal_dh_generate_param dh_generate_param;
	hal_dh_compute_shared_secret dh_compute_shared_secret;
	hal_ecdh_compute_shared_secret ecdh_compute_shared_secret;
	hal_set_certificate set_certificate;
	hal_get_certificate get_certificate;
	hal_remove_certificate remove_certificate;
	hal_get_factory_key get_factory_key;
	hal_get_factory_cert get_factory_cert;
	hal_get_factory_data get_factory_data;
	hal_aes_encrypt aes_encrypt;
	hal_aes_decrypt aes_decrypt;
	hal_rsa_encrypt rsa_encrypt;
	hal_rsa_decrypt rsa_decrypt;
	hal_write_storage write_storage;
	hal_read_storage read_storage;
	hal_delete_storage delete_storage;
	hal_gcm_encrypt gcm_encrypt;
	hal_gcm_decrypt gcm_decrypt;
};

/**
 * @brief    Register seclink driver with lower half driver
 * 
 * @note     Chipset vendor should register the lower half driver to SECLINK_PATH
 *           Refer sec_lowerhalf_s in seclink_drv.h
 */
int se_initialize(void);
