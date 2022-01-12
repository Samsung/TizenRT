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
	HAL_KEY_IN_USE,
	HAL_CERT_IN_USE,
	HAL_DATA_IN_USE,
	HAL_NOT_SUPPORTED,
	HAL_NOT_IMPLEMENTED,
	HAL_BUSY,
	HAL_FAIL,
} hal_result_e;

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
	hal_data *pubkey_x;
	hal_data *pubkey_y;
} hal_ecdh_data;

typedef struct _hal_ss_info {
	unsigned int size;
	struct _hal_ss_info *next;
} hal_ss_info;

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

/**
 * Common
 */
/*
 * @brief  Initialize SE HAL
 *
 * @return HAL_SUCCESS on success
 *
 * @note   Initialize secure channel
 */
typedef int (*hal_init)(_IN_ hal_init_param *params);

/*
 * @brief  Deinitialize SE HAL
 *
 * @return HAL_SUCCESS on success
 *
 * @note   Deinitialize secure channel
 */
typedef int (*hal_deinit)(void);

/*
 * @brief Free memory allocated within HAL
 */
typedef int (*hal_free_data)(_IN_ hal_data *data);

/*
 * @brief    Get status of SE
 *
 * @return   HAL_SUCCESS on success
 *
 * @note     BUSY/IDLE check should be conducted within each HAL APIs as well.
 */
typedef int (*hal_get_status)(void);

/**
 * Key Manager
 */
/*
 * @brief    Sey key in SE
 *
 * @return   HAL_SUCCESS on success
 *
 * @note     If key type is asymmetric then the private key is assigned
 *           to prikey->data.
 */
typedef int (*hal_set_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _IN_ hal_data *key, _IN_ hal_data *prikey);

/*
 * @brief Get key from SE
 *
 * @return HAL_SUCCESS on success
 *
 * @note    Return pubkey_X to key->data and pubkey_Y to key->priv for ECC
 */
typedef int (*hal_get_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _OUT_ hal_data *key);

/*
 * @brief Remove key in SE
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_remove_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx);

/*
 * @brief Generate key in SE
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_generate_key)(_IN_ hal_key_type mode, _IN_ uint32_t key_idx);

/**
 * Authenticate
 */
/*
 * @brief Generate random
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_generate_random)(_IN_ uint32_t len, _OUT_ hal_data *random);

/*
 * @brief Get HASH
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_get_hash)(_IN_ hal_hash_type mode, _IN_ hal_data *input, _OUT_ hal_data *hash);

/*
 * @brief Get HMAC
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_get_hmac)(_IN_ hal_hmac_type mode, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac);

/*
 * @brief Get signature using RSA
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_rsa_sign_md)(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign);

/*
 * @brief Verify message digest (signature) using RSA
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_rsa_verify_md)(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx);

/*
 * @brief Get signature using ECC
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_ecdsa_sign_md)(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign);

/*
 * @brief Verify message digest (signature) using ECC
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_ecdsa_verify_md)(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx);

/*
 * @brief Generate DH parameters
 *
 * @return HAL_SUCCESS on success
 *
 * @note The function generates GX (G^X mod P) which is pubkey in dh_param
 *       with given input G, P.
 *       X will be generate and will be protected inside slot in SE
 *       X have to be removed by using hal_remove_key()
 */
typedef int (*hal_dh_generate_param)(_IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param);

/*
 * @brief Compute DH shared secret
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_dh_compute_shared_secret)(_IN_ hal_dh_data *dh_param, _IN_ uint32_t dh_idx, _OUT_ hal_data *shared_secret);

/*
 * @brief Get ECDH shared secret
 *
 * @return HAL_SUCCESS on success
 *
 * @note Pubkey denotes a public key from the target which tries to share a secret
 */
typedef int (*hal_ecdh_compute_shared_secret)(_IN_ hal_ecdh_data *ecdh_mode, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret);

/*
 * @brief Set certificate in SE
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_set_certificate)(_IN_ uint32_t cert_idx, _IN_ hal_data *cert_in);

/*
 * @brief Get certificate in SE
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_get_certificate)(_IN_ uint32_t cert_idx, _OUT_ hal_data *cert_out);
/*
 * @brief Remove certificate in SE
 *
 * @return HAL_SUCCESS on success
 */
typedef int (*hal_remove_certificate)(_IN_ uint32_t cert_idx);

/*
 * @brief Get factory key
 *
 * @return HAL_SUCCESS on success
 *
 * @note   TizenRT 3.1 doesn't use it anymore. Instead of the API, TizenRT separate
 *         slot range for factory key which is injected from 0 ~ 31 and for ram key
 *         which is volitile from 32 ~ 63. So use hal_get_key.
 */

DEPRECATED typedef int (*hal_get_factory_key)(_IN_ uint32_t key_idx, _IN_ hal_data *key);

/*
 * @brief Get factory cert
 *
 * @return HAL_SUCCESS on success
 */
DEPRECATED typedef int (*hal_get_factory_cert)(_IN_ uint32_t cert_idx, _IN_ hal_data *cert);

/*
 * @brief Get factory data
 *
 * @return HAL_SUCCESS on success
 */
DEPRECATED typedef int (*hal_get_factory_data)(_IN_ uint32_t data_idx, _IN_ hal_data *data);

/**
 * Crypto
 */
/*
 * @brief   Encrypt data using AES
 *
 * @param   dec_data    The plaintext block.
 * @param   aes_param   AES parameters
 * @param   key_idx     The key index to encrypt plaintext.
 * @param   enc_data    The output (ciphertext) block.
 *
 * @return  HAL_SUCCESS on success
 *
 * @note    The plaintext is assigned to dec_data.data and dec_data.data_len.
 *          priv and priv_len are not used to dec_data.
 *          The ciphertext should be assigned to enc_data.data and it's length to
 *          enc_data.data_len. priv and priv_len are not used to enc_data.
 *
 *          hal_aes_param need to be updated depends on aes_param.mode(hal_aes_algo).
 *          If aes_param.mode is HAL_AES_CBC_NOPAD then aes_param.iv will be updated.
 *          Similar to above operation following updates are required to the driver.
 *          If you want get more information please refer mbedtls/aes.h.
 *
 *          mode: HAL_AES_ECB_NOPAD
 *          - before: only mode in hal_aes_param is set.
 *          - after: hal_aes_param update is not required.
 *          mode: HAL_AES_CBC_NOPAD
 *          - before: mode, iv, iv_len are set.
 *          - after: iv is updated.
 *          mode: HAL_AES_CTR
 *          - before: mode, nc_off, nonce_counter, stream_block are set.
 *          - after: nc_off, ... are updated.
 *          mode: HAL_AES_CFB128
 *          - before: mode, iv, iv_len, iv_offset are set.
 *          - after: iv, iv_offset are updated.
 */
typedef int (*hal_aes_encrypt)(_IN_ hal_data *dec_data,
							   _IN_ hal_aes_param *aes_param,
							   _IN_ uint32_t key_idx,
							   _OUT_ hal_data *enc_data);

/*
 * @brief    Decrypt data using AES
 *
 * @param    enc_data    The ciphertext block.
 * @param    aes_param   AES parameters
 * @param    key_idx     The key index to decrypt ciphertext.
 * @param    dec_data    The output(plaintext) block.
 *
 * @return   HAL_SUCCESS on success
 *
 * @note    The ciphertext is assigned to enc_data.data and enc_data.data_len.
 *          priv and priv_len are not used to enc_data.
 *          The plaintext should be assigned to dec_data.data and it's length to
 *          dec_data.data_len. priv and priv_len are not used to dec_data.
 *
 *          hal_aes_param need to be updated depends on aes_param.mode(hal_aes_algo).
 *          If aes_param.mode is HAL_AES_CBC_NOPAD then aes_param.iv will be updated.
 *          Similar to above operation following updates are required to the driver.
 *          If you want get more information please refer mbedtls/aes.h.
 *
 *          mode: HAL_AES_ECB_NOPAD
 *          - before: only mode in hal_aes_param is set.
 *          - after: hal_aes_param update is not required.
 *          mode: HAL_AES_CBC_NOPAD
 *          - before: mode, iv, iv_len are set.
 *          - after: iv is updated.
 *          mode: HAL_AES_CTR
 *          - before: mode, nc_off, nonce_counter, stream_block are set.
 *          - after: nc_off, ... are updated.
 *          mode: HAL_AES_CFB128
 *          - before: mode, iv, iv_len, iv_offset are set.
 *          - after: iv, iv_offset are updated.
 */
typedef int (*hal_aes_decrypt)(_IN_ hal_data *enc_data,
							   _IN_ hal_aes_param *aes_param,
							   _IN_ uint32_t key_idx,
							   _OUT_ hal_data *dec_data);

/*
 * @brief    Encrypt data using RSA
 *
 * @return   HAL_SUCCESS on success
 */
typedef int (*hal_rsa_encrypt)(_IN_ hal_data *dec_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data);

/*
 * @brief    Decrypt data using RSA
 *
 * @return   HAL_SUCCESS on success
 */
typedef int (*hal_rsa_decrypt)(_IN_ hal_data *enc_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data);

/**
 * Secure Storage
 */
/*
 * @brief    Write data in SE
 *
 * @return   HAL_SUCCESS on success
 */
typedef int (*hal_write_storage)(_IN_ uint32_t ss_idx, _IN_ hal_data *data);

/*
 * @brief    Read data from SE
 *
 * @return   HAL_SUCCESS on success
 */
typedef int (*hal_read_storage)(_IN_ uint32_t ss_idx, _OUT_ hal_data *data);

/*
 * @brief    Delete data in secure storage of ss_idx
 *
 * @return   HAL_SUCCESS on success
 */
typedef int (*hal_delete_storage)(_IN_ uint32_t ss_idx);

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
};

int se_initialize(void);
