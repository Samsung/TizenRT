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

#ifndef _SECURITY_API_COMMON_H__
#define _SECURITY_API_COMMON_H__

#include <stdint.h>

#define SECURITY_MAX_KEY_BUF          256
#define SECURITY_MAX_CERT_BUF         2048
#define SECURITY_MAX_SS_BUF           4096
#define SECURITY_MAX_BUF              SECURITY_MAX_SS_BUF

struct security_ctx;
typedef struct security_ctx *security_handle;

typedef struct _security_data {
	void *data;
	unsigned int length;
} security_data;

typedef enum {
	SECURITY_OK,

	SECURITY_ERROR,
	SECURITY_ALLOC_ERROR,
	SECURITY_INVALID_INPUT_PARAMS,
	SECURITY_INVALID_CERT_INDEX,
	SECURITY_INVALID_KEY_INDEX,
	SECURITY_INVALID_BUFFER_SIZE,
	SECURITY_MUTEX_INIT_ERROR,
	SECURITY_MUTEX_LOCK_ERROR,
	SECURITY_MUTEX_UNLOCK_ERROR,
	SECURITY_MUTEX_FREE_ERROR,

	SECURITY_WRITE_CERT_ERROR,
	SECURITY_READ_CERT_ERROR,
	SECURITY_GET_HASH_ERROR,
	SECURITY_GET_RANDOM_ERROR,
	SECURITY_ECDSA_SIGN_ERROR,
	SECURITY_ECDSA_VERIFY_ERROR,
	SECURITY_ECDH_COMPUTE_ERROR,

	SECURITY_KEY_STORAGE_IN_USE,
	SECURITY_RSC_BUSY,
	SECURITY_NOT_SUPPORT,
} security_error;

typedef struct _security_csr {
	unsigned char	issuer_country[128];
	unsigned char	issuer_organization[128];
	unsigned char	issuer_cn[128];
	unsigned char	issuer_keyname[20];
	unsigned int	issuer_algorithm;
	unsigned char	subject_country[128];
	unsigned char	subject_organization[128];
	unsigned char	subject_cn[128];
	unsigned char	subject_keyname[20];
	unsigned int	subject_algorithm;
	unsigned int	serial;
	unsigned int	cert_years;
} security_csr;

typedef enum {
	RSASSA_PKCS1_V1_5,
	RSASSA_PKCS1_PSS_MGF1,
	RSASSA_UNKNOWN,
} security_rsa_mode;

typedef enum {
	ECDSA_BRAINPOOL_P256R1,
	ECDSA_BRAINPOOL_P384R1,
	ECDSA_BRAINPOOL_P512R1,
	ECDSA_SEC_P192R1,
	ECDSA_SEC_P224R1,
	ECDSA_SEC_P256R1,
	ECDSA_SEC_P384R1,
	ECDSA_SEC_P512R1,
	ECDSA_UNKNOWN,
} security_ecdsa_mode;

typedef enum {
	DH_1024,
	DH_2048,
	DH_4096,
	DH_UNKNOWN,
} security_dh_mode;

typedef enum {
	KEY_AES_128,
	KEY_AES_192,
	KEY_AES_256,
	KEY_RSA_1024,
	KEY_RSA_2048,
	KEY_RSA_3072,
	KEY_RSA_4096,
	KEY_ECC_BRAINPOOL_P256R1,
	KEY_ECC_BRAINPOOL_P384R1,
	KEY_ECC_BRAINPOOL_P512R1,
	KEY_ECC_SEC_P192R1,
	KEY_ECC_SEC_P224R1,
	KEY_ECC_SEC_P256R1,
	KEY_ECC_SEC_P384R1,
	KEY_ECC_SEC_P512R1,
	KEY_HMAC_MD5,
	KEY_HMAC_SHA1,
	KEY_HMAC_SHA224,
	KEY_HMAC_SHA256,
	KEY_HMAC_SHA384,
	KEY_HMAC_SHA512,
	KEY_UNKNOWN,
} security_key_type;

typedef enum {
	HMAC_MD5,
	HMAC_SHA1,
	HMAC_SHA224,
	HMAC_SHA256,
	HMAC_SHA384,
	HMAC_SHA512,
	HMAC_UNKNOWN,
} security_hmac_mode;


typedef enum {
	AES_ECB_NOPAD,
	AES_ECB_ISO9797_M1,
	AES_ECB_ISO9797_M2,
	AES_ECB_PKCS5,
	AES_ECB_PKCS7,
	AES_CBC_NOPAD,
	AES_CBC_ISO9797_M1,
	AES_CBC_ISO9797_M2,
	AES_CBC_PKCS5,
	AES_CBC_PKCS7,
	AES_CTR,
	AES_UNKNOWN,
} security_aes_mode;

typedef enum {
	HASH_MD5,
	HASH_SHA1,
	HASH_SHA224,
	HASH_SHA256,
	HASH_SHA384,
	HASH_SHA512,
	HASH_UNKNOWN,
} security_hash_mode;

typedef struct _security_rsa_param {
	security_rsa_mode rsa_a;
	security_hash_mode hash_t;
	security_hash_mode mgf;
	uint32_t salt_byte_len;
} security_rsa_param;

typedef struct _security_aes_param {
	security_aes_mode mode;
	unsigned char *iv;
	unsigned int iv_len;
} security_aes_param;

typedef struct _security_ecdsa_param {
	security_ecdsa_mode curve;
	security_hash_mode hash_t;
} security_ecdsa_param;

typedef struct _security_dh_param {
	security_dh_mode mode;
	security_data *G;
	security_data *P;
	security_data *pubkey;
} security_dh_param;

typedef struct _security_ecdh_param {
	security_ecdsa_mode curve;
	security_data *pubkey_x;
	security_data *pubkey_y;
} security_ecdh_param;

#define SEC_DATA_INITIALIZER {NULL, 0}
#define SEC_DHPARAM_INITIALIZER {DH_UNKNOWN, NULL, NULL}
/**
 * Common
 */
security_error security_init(security_handle *hnd);
security_error security_deinit(security_handle hnd);
security_error security_free_data(security_data *data);
security_error security_get_status(int *status);

#endif // _SECURITY_API_COMMON_H__
