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

#ifndef __SECURITY_HAL_H__
#define __SECURITY_HAL_H__

#include <stdint.h>
#define _IN_
#define _OUT_

// from sECC_KEY
typedef struct _hal_ecc_key {
	unsigned int curve;
	unsigned char *privatekey;
	unsigned char *publickey_x;
	unsigned char x_byte_len;
	unsigned char *publickey_y;
	unsigned char y_byte_len;
} hal_ecc_key;

typedef enum {
	HAL_DH_1024,
	HAL_DH_2048,
} hal_dh_algo;

// from sDH_PARAM 
typedef struct _hal_dh_data {
	hal_dh_algo mode;
	unsigned int modules_p_byte_len;
	unsigned char *modules_p;
	unsigned int generator_g_byte_len;
	unsigned char *generator_g;
	unsigned int order_q_byte_len;
	unsigned char *order_q;
	unsigned int publickey_byte_len;
	unsigned char *publickey;
} hal_dh_data;

typedef enum {
	HAL_ECDSA_BRAINPOOL_P256R1,
	HAL_ECDSA_BRAINPOOL_P384R1,
	HAL_ECDSA_BRAINPOOL_P512R1,
	HAL_ECDSA_SEC_P256R1,
	HAL_ECDSA_SEC_P384R1,
	HAL_ECDSA_SEC_P521R1,
	HAL_ECDSA_BRAINPOOL_P256R1, // for Post Provision : Not used now
	HAL_ECDSA_SEC_P256R1,
	HAL_ECDSA_BRAINPOOL_P256R1, // for Artik credential
} hal_ecdsa_curve;

typedef enum{
	HAL_RSA_1024,
	HAL_RSA_2048,
} hal_rsa_algo;

typedef enum {
	HAL_AES_128,
	HAL_AES_192,
	HAL_AES_256,
} hal_aes_algo;

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
} hal_aes_mode;

// from sAES_PARAM
typedef struct _hal_aes_param {
	hal_aes_mode mode;

	unsigned char *iv;
	unsigned int iv_len;

	unsigned char *aad;
	unsigned int aad_len;

	unsigned char *tag;
	unsigned int tag_len;
} hal_aes_param;

// from sRSA_KEY
typedef struct _hal_rsa_key{
	hal_rsa_algo mode;
	unsigned char *modules;
	unsigned int modules_byte_len;
	unsigned char *publickey;
	unsigned int publickey_byte_len;
	unsigned char *privatekey;
} hal_rsa_key;

typedef enum {
	HAL_HASH_MD5,
	HAL_HASH_SHA1,
	HAL_HASH_SHA224,
	HAL_HASH_SHA256,
	HAL_HASH_SHA384,
	HAL_HASH_SHA512,
} hal_hash_algo;

typedef enum {
	HAL_HMAC_MD5,
	HAL_HMAC_SHA1,
	HAL_HMAC_SHA224,
	HAL_HMAC_SHA256,
	HAL_HMAC_SHA384,
	HAL_HMAC_SHA512,
} hal_hmac_algo;

typedef enum {
	HAL_RSASSA_PKCS1_V1_5_MD5,
	HAL_RSASSA_PKCS1_V1_5_SHA1,
	HAL_RSASSA_PKCS1_V1_5_SHA224,
	HAL_RSASSA_PKCS1_V1_5_SHA256,
	HAL_RSASSA_PKCS1_V1_5_SHA384,
	HAL_RSASSA_PKCS1_V1_5_SHA512,
	HAL_RSASSA_PKCS1_PSS_MGF1_SHA1,
	HAL_RSASSA_PKCS1_PSS_MGF1_SHA224,
	HAL_RSASSA_PKCS1_PSS_MGF1_SHA256,
	HAL_RSASSA_PKCS1_PSS_MGF1_SHA384,
	HAL_RSASSA_PKCS1_PSS_MGF1_SHA512,
	HAL_RSASSA_PKCS1_V1_5_SHA256, // for Post Provision
	HAL_RSASSA_PKCS1_PSS_MGF1_SHA256,
} hal_rsa_mode;


typedef struct _hal_ss_info {
	unsigned int size;
	struct _hal_ss_info *next;
} hal_ss_info;

typedef struct _hal_data {
	void *data;
	uint32_t data_len;
} hal_data;

// ======================================
// Function
// ======================================

/**
 * Check Status
 */
// Is checking status necessary?

/**
 * Key Manager
 */
int hal_set_key(_IN_ uint8_t *key, _IN_ uint8_t key_byte_len, _IN_ uint32_t key_type, _IN_ uint32_t key_idx);

int hal_remove_key(_IN_ uint32_t key_type, _IN_ uint32_t key_idx);

int hal_hmac_generate_key(_IN_ uint32_t key_byte_len, _IN_ uint32_t key_idx, _OUT_ uint8_t *key);

int hal_rsa_generate_key(_IN_ hal_rsa_algo mode, _IN_ uint32_t key_idx, _OUT_ uint8_t *key);

int hal_ecdsa_get_pubkey(_IN_ hal_ecc_key *ecc_pubkey, _IN_ uint32_t obj_id, _IN_ uint32_t key_idx, _OUT_ uint8_t *key);

int hal_rsa_get_pubkey(_IN_ uint32_t key_idx, _OUT_ hal_rsa_key *rsa_key);

/**
 * Crypto
 */
int hal_aes_decrypt(_IN_ hal_data *enc_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data);

int hal_aes_encrypt(_IN_ hal_data *dec_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data);

int hal_rsa_decrypt(_IN_ hal_data *enc_data, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data);

int hal_rsa_encrypt(_IN_ hal_data *dec_data, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data);

/**
 * Secure Storage
 */
int hal_read_storage(_IN_ uint32_t ss_idx, _OUT_ hal_data *data);

int hal_write_storage(_IN_ uint32_t ss_idx, _IN_ hal_data *data);

int hal_delete_storage(_IN_ uint32_t ss_idx);

/**
 * Authenticate
 */
// arTik see_compute_ecdh key doesn't require slot
int hal_compute_ecdh(_IN_ hal_ecc_key *key, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret);

int hal_dh_compute_shared_secret(_IN_ hal_dh_algo dh_pubkey, _IN_ hal_dh_data, param, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret);

/* Generate key at slot of index */
int hal_dh_generate_param(_IN_ hal_dh_algo mode, _IN_ uint32_t dh_idx, _OUT_ hal_dh_data *dh_param);

int hal_ecdsa_sign_md(_IN_ hal_ecdsa_curve curve, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign);

int hal_ecdsa_verify_md(_IN_ hal_ecdsa_curve curve, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _IN_ hal_data *sign);

int hal_generate_random(_IN_ uint32_t len, _OUT_ uint8_t *random);

int hal_get_hash(_IN_ hal_hash_algo algo, _IN_ hal_data *input, _OUT_ hal_data *hash);

int hal_get_hmac(_IN_ hal_hmac_algo algo, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac);

int hal_rsa_sign_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint8_t key_idx, _OUT_ hal_data *md);

int hal_rsa_verify_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign);

int hal_get_factorykey_data(_IN_ uint32_t key_id, _IN_ uint8_t *data);
