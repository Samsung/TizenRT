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


/*
 * Common
 */

/* Enumerator */
enum hal_result_e {
	HAL_SUCCESS,

	HAL_NOT_INITIALIZED,
	HAL_INVALID_ARGS,

	HAL_INVALID_SLOT_RANGE,
	HAL_INVALID_SLOT_TYPE, //ex. request to save key into cert slot
	HAL_EMPTY_SLOT,

	HAL_BAD_KEY,
	HAL_BAD_KEY_PAIR, //public and private keys do not match
	HAL_BAD_CERT,
	HAL_BAD_CERTKEY_PAIR, //certificate and key do not match

	HAL_NOT_ENOUGH_MEMORY,
	HAL_KEY_IN_USE,
	HAL_CERT_IN_USE,
	HAL_DATA_IN_USE,

	HAL_NOT_SUPPORTED,
	HAL_NOT_IMPLEMENTED,

	HAL_BUSY,
	HAL_FAIL,
};

typedef enum {
	HAL_HASH_MD5,
	HAL_HASH_SHA1,
	HAL_HASH_SHA224,
	HAL_HASH_SHA256,
	HAL_HASH_SHA384,
	HAL_HASH_SHA512,
} hal_hash_type;

typedef enum {
	HAL_DH_1024,
	HAL_DH_2048,
} hal_dh_key_type;

typedef enum{
	HAL_RSA_1024,
	HAL_RSA_2048,
} hal_rsa_key_type;

typedef enum {
	HAL_AES_128,
	HAL_AES_192,
	HAL_AES_256,
} hal_aes_key_type;

typedef enum {
	HAL_HMAC_MD5,
	HAL_HMAC_SHA1,
	HAL_HMAC_SHA224,
	HAL_HMAC_SHA256,
	HAL_HMAC_SHA384,
	HAL_HMAC_SHA512,
} hal_hmac_type;

typedef enum {
	HAL_ECDSA_BRAINPOOL_P256R1,
	HAL_ECDSA_BRAINPOOL_P384R1,
	HAL_ECDSA_BRAINPOOL_P512R1,
	HAL_ECDSA_SEC_P256R1,
	HAL_ECDSA_SEC_P384R1,
	HAL_ECDSA_SEC_P512R1,
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
} hal_aes_algo;

typedef enum {
	HAL_RSASSA_PKCS1_V1_5,
	HAL_RSASSA_PKCS1_PSS_MGF1,
} hal_rsa_algo;

typedef enum {
	/*  AES */
	HAL_KEY_AES_128,// 128 bits aes algorithm
	HAL_KEY_AES_192, // 192 bits aes algorithm
	HAL_KEY_AES_256, // 256 bits aes algorithm
	/*  RSA */
	HAL_KEY_RSA_1024, // 1024 bits rsa algorithm
	HAL_KEY_RSA_2048, // 2048 bits rsa algorithm
	HAL_KEY_RSA_3072, // 3072 bits rsa algorithm
	HAL_KEY_RSA_4096,
	/*  ECC: it doesn't support whole algorithm that mbedTLS support. it's have to be added*/
	HAL_KEY_ECC_BRAINPOOL_P256R1, // ecc brainpool curve for p256r1
	HAL_KEY_ECC_BRAINPOOL_P384R1, // ecc brainpool curve for p384r1
	HAL_KEY_ECC_BRAINPOOL_P512R1, // ecc brainpool curve for p512r1
	HAL_KEY_ECC_SEC_P256R1, // nist curve for p256r1
	HAL_KEY_ECC_SEC_P384R1, // nist curve for p384r1
	HAL_KEY_ECC_SEC_P512R1, // nist curve for p512r1
	/*  Hmac */
	HAL_KEY_HMAC_MD5, // hmac with md5
	HAL_KEY_HMAC_SHA1, // hmac with sha1
	HAL_KEY_HMAC_SHA224, // hmac with sha224
	HAL_KEY_HMAC_SHA256, // hmac with sha256
	HAL_KEY_HMAC_SHA384, // hmac with sha384
	HAL_KEY_HMAC_SHA512, // hmac with sha512
	/* DH */
	HAL_KEY_DH_1024,
	HAL_KEY_DH_2048,
} hal_key_type;

/* Structure */
typedef struct _hal_data {
	void *data;
	uint32_t data_len;
	void *priv;
} hal_data;

typedef struct _hal_rsa_mode {
	hal_rsa_algo rsa_a;
	hal_hash_type hash_t;
} hal_rsa_mode;

typedef struct _hal_aes_param {
	hal_aes_algo mode;
	unsigned char *iv;
	unsigned int iv_len;
} hal_aes_param;

typedef struct _hal_ecdsa_mode {
	hal_ecdsa_curve curve;
	hal_hash_type hash_t;
	hal_data *r;
	hal_data *s;
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


// ======================================
// Function
// ======================================

/**
 * Common
 */

/*
 * Reference
 * Desc: Initialize HAL 
 * Artik SEE API: 
 * TizenRT SEE API: 
 * ISP:
 * NOTE: Initialize secure channel
 */
int hal_init(void);

/*
 * Reference
 * Desc: Deinitialize HAL 
 * Artik SEE API: 
 * TizenRT SEE API: 
 * ISP: 
 * NOTE: Deinitialize secure channel
 */
int hal_deinit(void);

/*
 * Reference
 * Desc: free memory allocated within HAL 
 * Artik SEE API: 
 * TizenRT SEE API: 
 * ISP: 
 */
int hal_free_data(_IN_ hal_data *data);

/*
 * Reference
 * Desc: get status of SE
 * Artik SEE API: 
 * TizenRT SEE API: 
 * ISP: 
 * return value: busy, not initialized, ... 
 * NOTE: BUSY/IDLE check should be conducted within each HAL APIs as well.
 */
int hal_get_status(void);

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
 * NOTE: Return key type in key->priv
 */
int hal_get_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _OUT_ hal_data *key);


/*
 * Reference
 * Desc: 
 * Artik SEE API: int see_remove_key(see_algorithm algo, const char *key_name);
 * TizenRT SEE API: 
 * ISP: 
 */
int hal_remove_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx);


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
int hal_generate_random(_IN_ uint32_t len, _OUT_ hal_data *random);

/*
 * Reference
 * Desc: Get HASH
 * Artik SEE API: int see_get_hash(see_algorithm algo, see_data data, see_data *hash);
 * TizenRT SEE API: int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode)
 * ISP: int isp_hash(unsigned char *hash, struct sHASH_MSG *hash_msg, unsigned int object_id);
 */
int hal_get_hash(_IN_ hal_hash_type mode, _IN_ hal_data *input, _OUT_ hal_data *hash);

/*
 * Reference
 * Desc: Get HMAC
 * Artik SEE API: int see_get_hmac(see_algorithm algo, const char *key_name, see_data data, see_data *hmac);
 * TizenRT SEE API: int see_get_hmac(struct sHMAC_MSG *hmac_msg, unsigned char *output, unsigned int object_id, unsigned int key_index)
 * ISP: int isp_hmac_securekey(unsigned char *mac, struct sHMAC_MSG *hmac_msg, unsigned int object_id, unsigned int key_index)
 * w
 */
int hal_get_hmac(_IN_ hal_hmac_type mode, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac);

/*
 * Reference
 * Desc: get signature using RSA key
 * Artik SEE API: int see_get_rsa_signature(see_rsa_mode mode, const char *key_name, see_data hash, see_data *sign)
 * TizenRT SEE API: int see_get_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
 * ISP: int isp_rsa_sign_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 */
int hal_rsa_sign_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign);

/*
 * Reference
 * Desc: 
 * Artik SEE API: int see_verify_rsa_signature(see_rsa_mode mode, const char *key_name, see_data hash, see_data sign)
 * TizenRT SEE API: int see_verify_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
 * ISP: int isp_rsa_verify_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);
 */
int hal_rsa_verify_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx);

/*
 * Reference
 * Desc: Get signature
 * Artik SEE API: int see_get_ecdsa_signature(see_ecdsa_curve curve, const char *key_name, see_data hash, see_data *sign)
 * TizenRT SEE API: int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
 * ISP: int isp_ecdsa_sign_md_securekey(struct sECC_SIGN *ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 */
int hal_ecdsa_sign_md(_IN_ hal_data *hash, _IN_ uint32_t key_idx, _INOUT_ hal_ecdsa_mode *mode, _OUT_ hal_data *sign);

/*
 * Reference
 * Artik SEE API: 
 * TizenRT SEE API: int see_verify_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
 * ISP: int isp_ecdsa_verify_md_securekey(struct sECC_SIGN *ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 */
int hal_ecdsa_verify_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx);


/*
 * Reference
 * Desc: Generate key at slot of index
 * Artik SEE API: -
 * TizenRT SEE API: int see_generate_dhm_params(struct sDH_PARAM *d_param, unsigned int key_index)
 * ISP: int isp_dh_generate_keypair_userparam_securestorage(struct sDH_PARAM *i_dh_param, unsigned int dh_param_index);
 */
/*
 * The function generates GX (G^X mod P) which is pubkey in dh_param with given G, P
 * X will be generate and will be protected inside slot in SE
 * X have to be removed by using hal_remove_key()
 */
int hal_dh_generate_param(_IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param);

/*
 * Reference
 * Desc: Get shared secret
 * Artik SEE API: -
 * TizenRT SEE API: int see_compute_dhm_param(struct sDH_PARAM *d_param, unsigned int key_index, unsigned char *output, unsigned int *olen)
 * ISP: int isp_dh_compute_shared_secret_securekey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sDH_PARAM dh_publickey, unsigned int key_index);
 */
int hal_dh_compute_shared_secret(_IN_ hal_dh_data *dh_param, _IN_ uint32_t dh_idx, _OUT_ hal_data *shared_secret);

/*
 * Reference
 * Desc: Get ECDH shared secret
 * Artik SEE API: -
 * TizenRT SEE API: int see_compute_ecdh_param(struct sECC_KEY *ecc_pub, unsigned int key_index, unsigned char *output, unsigned int *olen)
 * ISP: int isp_compute_ecdh_securekey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sECC_KEY ecc_publickey, unsigned int key_index);
 * NOTE: pubkey denotes a public key from the target which tries to share a secret
 */
int hal_ecdh_compute_shared_secret(_IN_ hal_ecdh_data *ecdh_mode, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret);

/*
 * Reference
 * Desc: Set certificate in secure storage
 * Artik SEE API: -
 * TizenRT SEE API: int see_set_certificate(unsigned char *cert, unsigned int cert_len, unsigned int cert_index, unsigned int cert_type)
 * ISP: int isp_write_cert(unsigned char *data, unsigned int data_byte_len, unsigned int index)
 * NOTE: When cert_in consists of chains, cert_in->data represents the last certificate, and then, cert_in->priv denotes the next chain formatted hal_data.
 * (Root CA has to be the end of hal_data)
 */
int hal_set_certificate(_IN_ uint32_t cert_idx, _IN_ hal_data *cert_in);

/*
 * Reference
 * Desc: Get certificate in secure storage
 * Artik SEE API: -
 * TizenRT SEE API: int see_get_certificate(unsigned char *cert, unsigned int *cert_len, unsigned int cert_index, unsigned int cert_type)
 * ISP: int isp_read_cert(unsigned char *data, unsigned int *data_byte_len, unsigned int index);
 * NOTE: When cert_out consists of chains, cert_out->data represents the last certificate, and then, cert_out->priv denotes the next chain formatted hal_data.
 * (Root CA has to be the end of hal_data)
 */
int hal_get_certificate(_IN_ uint32_t cert_idx, _OUT_ hal_data *cert_out);
/*
 * Reference
 * Desc: Remove certificate in secure storage
 * Artik SEE API: -
 * TizenRT SEE API: -
 * ISP: -
 */
int hal_remove_certificate(_IN_ uint32_t cert_idx);

/*
 * Reference
 * Desc: Get factorykey data
 * Artik SEE API: -
 * TizenRT SEE API: int see_get_publickey(unsigned char *key_der, unsigned int *key_len)
 * ISP: int isp_get_factorykey_data(unsigned char *data, unsigned int *data_byte_len, unsigned int data_id);
 */
int hal_get_factorykey_data(_IN_ uint32_t key_idx, _IN_ hal_data *data);


/**
 * Crypto
 */

/*
 * Reference
 * Desc: Encrypt data using AES
 * Artik SEE API: -
 * TizenRT SEE API: int see_aes_encrypt(unsigned int key_index, struct sAES_PARAM *aes_param)
 * ISP: int isp_aes_encrypt_securekey(struct sAES_PARAM *aes_param, unsigned int key_index)
 */
int hal_aes_encrypt(_IN_ hal_data *dec_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data);

/*
 * Reference
 * Desc: Decrypt data using AES
 * Artik SEE API: -
 * TizenRT SEE API: int see_aes_decrypt(unsigned int key_index, struct sAES_PARAM *aes_param)
 * ISP: int isp_aes_decrypt_securekey(struct sAES_PARAM *aes_param, unsigned int key_index)
 */
int hal_aes_decrypt(_IN_ hal_data *enc_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data);

/*
 * Reference
 * Desc: Encrypt data using RSA
 * Artik SEE API: -
 * TizenRT SEE API: int see_rsa_encryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen)
 * ISP: int isp_rsa_encrypt_securekey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned int key_index)
 */
int hal_rsa_encrypt(_IN_ hal_data *dec_data, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data);

/*
 * Reference
 * Desc: Decrypt data using RSA
 * Artik SEE API: -
 * TizenRT SEE API:
 * ISP:
 */
int hal_rsa_decrypt(_IN_ hal_data *enc_data, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data);


/**
 * Secure Storage
 */

/*
 * Reference
 * Desc: write data in secure storage of ss_idx
 * Artik SEE API: -
 * TizenRT SEE API: int see_read_secure_storage(unsigned char *data, unsigned int *data_len, unsigned index)
 * ISP: int isp_read_storage(unsigned char *data, unsigned int *data_byte_len, unsigned int index)
 */
int hal_write_storage(_IN_ uint32_t ss_idx, _IN_ hal_data *data);

/*
 * Reference
 * Desc: Read data from secure storage of ss_idx
 * Artik SEE API: -
 * TizenRT SEE API: int see_read_secure_storage(unsigned char *data, unsigned int *data_len, unsigned index)
 * ISP: int isp_read_storage(unsigned char *data, unsigned int *data_byte_len, unsigned int index)
 */
int hal_read_storage(_IN_ uint32_t ss_idx, _OUT_ hal_data *data);

/*
 * Reference
 * Desc: Delete data in secure storage of ss_idx
 * Artik SEE API: -
 * TizenRT SEE API: int see_read_secure_storage(unsigned char *data, unsigned int *data_len, unsigned index)
 * ISP: int isp_read_storage(unsigned char *data, unsigned int *data_byte_len, unsigned int index)
 */
int hal_delete_storage(_IN_ uint32_t ss_idx);


#endif // __SECURITY_HAL_H__
