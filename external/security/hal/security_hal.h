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
#define _INOUT_


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
} hal_ecdsa_curve;

typedef struct _hal_ecc_sign {
	hal_ecdsa_curve sign_type;
	unsigned char *r;
	unsigned char r_byte_len;
	unsigned char *s;
	unsigned char s_byte_len;
} hal_ecc_sign;

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
	unsigned char *modules;  // ???
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
} hal_rsa_mode;

typedef struct _hal_ss_info {
	unsigned int size;
	struct _hal_ss_info *next;
} hal_ss_info;

typedef struct _hal_data {
	void *data;
	uint32_t data_len;
	void *priv;
} hal_data;

typedef enum {
	HAL_ECP_DP_SECP192R1,	/*!< 192-bits NIST curve  */
	HAL_ECP_DP_SECP224R1,	/*!< 224-bits NIST curve  */
	HAL_ECP_DP_SECP256R1,	/*!< 256-bits NIST curve  */
	HAL_ECP_DP_SECP384R1,	/*!< 384-bits NIST curve  */
	HAL_ECP_DP_SECP521R1,	/*!< 521-bits NIST curve  */
	HAL_ECP_DP_BP256R1,	/*!< 256-bits Brainpool curve */
	HAL_ECP_DP_BP384R1,	/*!< 384-bits Brainpool curve */
	HAL_ECP_DP_BP512R1,	/*!< 512-bits Brainpool curve */
	HAL_ECP_DP_CURVE25519,	/*!< Curve25519               */
	HAL_ECP_DP_SECP192K1,	/*!< 192-bits "Koblitz" curve */
	HAL_ECP_DP_SECP224K1,	/*!< 224-bits "Koblitz" curve */
	HAL_ECP_DP_SECP256K1,	/*!< 256-bits "Koblitz" curve */
} hal_ecc_algo;

typedef enum {
	/*  AES */
	HAL_KEY_AES_128,// 128 bits aes algorithm
	HAL_KEY_AES_192, // 192 bits aes algorithm
	HAL_KEY_AES_256, // 256 bits aes algorithm
	/*  RSA */
	HAL_KEY_RSA_1024, // 1024 bits rsa algorithm
	HAL_KEY_RSA_2048, // 2048 bits rsa algorithm
	HAL_KEY_RSA_3072, // 3072 bits rsa algorithm
	/*  ECC: it doesn't support whole algorithm that mbedTLS support. it's have to be added*/
	HAL_KEY_ECC_BRAINPOOL_P256R1, // ecc brainpool curve for p256r1
	HAL_KEY_ECC_BRAINPOOL_P384R1, // ecc brainpool curve for p384r1
	HAL_KEY_ECC_BRAINPOOL_P512R1, // ecc brainpool curve for p512r1
	HAL_KEY_ECC_SEC_P256R1, // nist curve for p256r1
	HAL_KEY_ECC_SEC_P384R1, // nist curve for p384r1
	HAL_KEY_ECC_SEC_P521R1, // nist curve for p521r1
	/*  Hash */
	HAL_KEY_HASH_MD5, // md5 hash algorithm
	HAL_KEY_HASH_SHA1, // sha1 hash algorithm``
	HAL_KEY_HASH_SHA224, // sha224 hash algorithm
	HAL_KEY_HASH_SHA256, // sha256 hash algorithm
	HAL_KEY_HASH_SHA384, // sha384 hash algorithm
	HAL_KEY_HASH_SHA512, // sha512 hash algorithm
	/*  Hmac */
	HAL_KEY_HMAC_MD5, // hmac with md5
	HAL_KEY_HMAC_SHA1, // hmac with sha1
	HAL_KEY_HMAC_SHA224, // hmac with sha224
	HAL_KEY_HMAC_SHA256, // hmac with sha256
	HAL_KEY_HMAC_SHA384, // hmac with sha384
	HAL_KEY_HMAC_SHA512, // hmac with sha512
} hal_key_type;

// ======================================
// Function
// ======================================

/**
 * Common
 */
int hal_data_free(_IN_ hal_data *data);

/*
 * return value: busy, not initialized, ...
 */

int hal_get_status(void);

// Is checking status necessary?

/**
 * Key Manager
 */
/*
 * Reference
 * Desc: If key type is asymmetric then private key can be stored.
 * Artik SEE API: int see_set_key(see_algorithm algo, const char *key_name, see_data key);
 * TizenRT SEE API: 
 * ISP: 
 */
int hal_set_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _IN_ hal_data *key, _IN_ hal_data *prikey);


/*
 * Reference
 * Desc: 
 * Artik SEE API: int see_get_pubkey(see_algorithm algo, const char *key_name, see_data *pub_key);
 * TizenRT SEE API: 
 * ISP: 
 */
int hal_get_key(_IN_ uint32_t key_idx, _OUT_ hal_data *key);


/*
 * Reference
 * Desc: 
 * Artik SEE API: int see_remove_key(see_algorithm algo, const char *key_name);
 * TizenRT SEE API: 
 * ISP: 
 */
int hal_remove_key(_IN_ hal_key_type key_type, _IN_ uint32_t key_idx);


/*
 * Reference
 * Desc: 
 * Artik SEE API: int see_generate_key(see_algorithm algo, const char *key_name, see_data *pub_key)
 * TizenRT SEE API: 
 * ISP: 
 */
int hal_generate_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx);


/**
 * Authenticate
 */

/*
 * Reference
 * Desc: Generate random
 * Artik SEE API: int see_generate_random(unsigned int size, see_data *random)
 * TizenRT SEE API: int see_generate_random(unsigned int *data, unsigned int len)
 * ISP: int isp_generate_random(unsigned int *random, unsigned int wlen);
 */
int hal_generate_random(_IN_ uint32_t len, _OUT_ uint8_t *random);

/*
 * Reference
 * Desc: Get HASH
 * Artik SEE API: int see_get_hash(see_algorithm algo, see_data data, see_data *hash);
 * TizenRT SEE API: int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode)
 * ISP: int isp_hash(unsigned char *hash, struct sHASH_MSG *hash_msg, unsigned int object_id);
 */
int hal_get_hash(_IN_ hal_hash_algo algo, _IN_ hal_data *input, _OUT_ hal_data *hash);

/*
 * Reference
 * Desc: Get HMAC
 * Artik SEE API: int see_get_hmac(see_algorithm algo, const char *key_name, see_data data, see_data *hmac);
 * TizenRT SEE API: int see_get_hmac(struct sHMAC_MSG *hmac_msg, unsigned char *output, unsigned int object_id, unsigned int key_index)
 * ISP: int isp_hmac_securekey(unsigned char *mac, struct sHMAC_MSG *hmac_msg, unsigned int object_id, unsigned int key_index)
 */
int hal_get_hmac(_IN_ hal_hmac_algo algo, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac);

/*
 * Reference
 * Desc: 
 * Artik SEE API: int see_get_rsa_signature(see_rsa_mode mode, const char *key_name, see_data hash, see_data *sign)
 * TizenRT SEE API: int see_get_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
 * ISP: int isp_rsa_sign_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 */
int hal_rsa_sign_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint8_t key_idx, _OUT_ hal_data *md);

int hal_rsa_verify_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign);

/*
 * Reference
 * Desc: Get signature
 * Artik SEE API: int see_get_ecdsa_signature(see_ecdsa_curve curve, const char *key_name, see_data hash, see_data *sign)
 * TizenRT SEE API: int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
 * ISP: int isp_ecdsa_sign_md_securekey(struct sECC_SIGN *ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 */
int hal_ecdsa_sign_md(_IN_ hal_ecdsa_curve curve, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign);

/*
 * Reference
 * Artik SEE API: 
 * TizenRT SEE API: int see_generate_dhm_params(struct sDH_PARAM *d_param, unsigned int key_index)
 * ISP: isp_ecdsa_verify_md_securekey()
 */
int hal_ecdsa_verify_md(_IN_ hal_ecdsa_curve curve, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _IN_ hal_data *sign);

int hal_compute_ecdh(_IN_ hal_ecc_key *key, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret);

/*
 * Reference
 * Desc: Generate key at slot of index
 * Artik SEE API: -
 * TizenRT SEE API: int see_generate_dhm_params(struct sDH_PARAM *d_param, unsigned int key_index)
 * ISP: int isp_dh_generate_keypair_userparam_securestorage(struct sDH_PARAM *i_dh_param, unsigned int dh_param_index);
 */
int hal_dh_generate_param(_IN_ hal_dh_algo mode, _IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param);

/*
 * Reference
 * Desc: Get shared secret
 * Artik SEE API: -
 * TizenRT SEE API: int see_compute_dhm_param(struct sDH_PARAM *d_param, unsigned int key_index, unsigned char *output, unsigned int *olen)
 * ISP: int isp_dh_compute_shared_secret_securekey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sDH_PARAM dh_publickey, unsigned int key_index);
 */
int hal_dh_compute_shared_secret(_IN_ hal_dh_algo dh_pubkey, _IN_ hal_dh_data param, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret);

int hal_get_factorykey_data(_IN_ uint32_t key_id, _IN_ uint8_t *data);


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


#endif // __SECURITY_HAL_H__
