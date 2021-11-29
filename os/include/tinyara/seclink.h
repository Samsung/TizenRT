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
#include <sys/ioctl.h>
#include <tinyara/security_hal.h>

/*  result */
#define SECLINK_OK 0
#define SECLINK_ERROR -1
#define SECLINK_INVALID_ARGS -2
#define SECLINK_INVALID_REQUEST -3
#define SECLINK_INVALID_SLOT_RANGE -4
#define SECLINK_INVALID_SLOT_TYPE -5 //ex. request to save key into cert slot
#define SECLINK_EMPTY_SLOT -6
#define SECLINK_BAD_KEY -7		// only public key can be returned
#define SECLINK_BAD_KEY_PAIR -8 //public and private keys do not match
#define SECLINK_BAD_CERT -9
#define SECLINK_BAD_CERTKEY_PAIR -10 //certificate and key do not match
#define SECLINK_NOT_ENOUGH_MEMORY -11
#define SECLINK_ALLOC_FAIL -12
#define SECLINK_KEY_IN_USE -13
#define SECLINK_CERT_IN_USE -14
#define SECLINK_DATA_IN_USE -15
#define SECLINK_NOT_SUPPORTED -16
#define SECLINK_NOT_IMPLEMENTED -17
#define SECLINK_BUSY -18
#define SECLINK_NOT_INITIALIZED -19

#define SECLINK_MEM_MAX_SIZE 512
#define SECLINK_MEM_PRIV_MAX_SIZE 128
#define SECLINK_MAX_RAM_KEY 64

/*  common */
#define SECLINKIOC_COMMON _SECLINKIOC(0x00)
#define SECLINKIOC_INIT _SECLINKIOC((SECLINKIOC_COMMON | 0x00))
#define SECLINKIOC_DEINIT _SECLINKIOC((SECLINKIOC_COMMON | 0x01))

/*  Crypto */
#define SECLINKIOC_CRYPTO _SECLINKIOC(0x10)
#define SECLINKIOC_AESENCRYPT _SECLINKIOC((SECLINKIOC_CRYPTO | 0x00))
#define SECLINKIOC_AESDECRYPT _SECLINKIOC((SECLINKIOC_CRYPTO | 0x01))
#define SECLINKIOC_RSAENCRYPT _SECLINKIOC((SECLINKIOC_CRYPTO | 0x02))
#define SECLINKIOC_RSADECRYPT _SECLINKIOC((SECLINKIOC_CRYPTO | 0x03))

/*  Authenticate */
#define SECLINKIOC_AUTH _SECLINKIOC(0x20)
#define SECLINKIOC_GENERATERANDOM _SECLINKIOC((SECLINKIOC_AUTH | 0x00))
#define SECLINKIOC_GETHASH _SECLINKIOC((SECLINKIOC_AUTH | 0x01))
#define SECLINKIOC_GETHMAC _SECLINKIOC((SECLINKIOC_AUTH | 0x02))
#define SECLINKIOC_RSASIGNMD _SECLINKIOC((SECLINKIOC_AUTH | 0x03))
#define SECLINKIOC_RSAVERIFYMD _SECLINKIOC((SECLINKIOC_AUTH | 0x04))
#define SECLINKIOC_ECDSASIGNMD _SECLINKIOC((SECLINKIOC_AUTH | 0x05))
#define SECLINKIOC_ECDSAVERIFYMD _SECLINKIOC((SECLINKIOC_AUTH | 0x06))
#define SECLINKIOC_DHGENERATEPARAM _SECLINKIOC((SECLINKIOC_AUTH | 0x07))
#define SECLINKIOC_DHCOMPUTESHAREDSECRET _SECLINKIOC((SECLINKIOC_AUTH | 0x08))
#define SECLINKIOC_ECDHCOMPUTESHAREDSECRET _SECLINKIOC((SECLINKIOC_AUTH | 0x09))
#define SECLINKIOC_SETCERTIFICATE _SECLINKIOC((SECLINKIOC_AUTH | 0x0a))
#define SECLINKIOC_GETCERTIFICATE _SECLINKIOC((SECLINKIOC_AUTH | 0x0b))
#define SECLINKIOC_REMOVECERTIFICATE _SECLINKIOC((SECLINKIOC_AUTH | 0x0c))
#define SECLINKIOC_GETFACTORY_KEY _SECLINKIOC((SECLINKIOC_AUTH | 0x0d))
#define SECLINKIOC_GETFACTORY_CERT _SECLINKIOC((SECLINKIOC_AUTH | 0x0e))
#define SECLINKIOC_GETFACTORY_DATA _SECLINKIOC((SECLINKIOC_AUTH | 0x0f))

/*  Secure Storage */
#define SECLINKIOC_SS _SECLINKIOC(0x40)
#define SECLINKIOC_WRITESTORAGE _SECLINKIOC((SECLINKIOC_SS | 0x00))
#define SECLINKIOC_READSTORAGE _SECLINKIOC((SECLINKIOC_SS | 0x01))
#define SECLINKIOC_DELETESTORAGE _SECLINKIOC((SECLINKIOC_SS | 0x02))

/*  Key manager */
#define SECLINKIOC_KEYMGR _SECLINKIOC(0x0080)
#define SECLINKIOC_SETKEY _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0000))
#define SECLINKIOC_GETKEY _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0001))
#define SECLINKIOC_REMOVEKEY _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0002))
#define SECLINKIOC_GENERATEKEY _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0003))

struct _seclink_s_;
typedef struct _seclink_s_ *sl_ctx;

typedef struct _seclink_mempool_s {
	void *mem;
	void *mem_priv;
} seclink_mem;

struct seclink_init_param {
	uint32_t i2c_port;
	uint32_t gpio;
};

struct seclink_key_info {
	hal_key_type mode;
	uint32_t key_idx;
	hal_data *key;
	hal_data *prikey;
};

struct seclink_auth_info {
	union {
		hal_hash_type hash_type;
		hal_hmac_type hmac_type;
		hal_rsa_mode rsa_type;
		hal_ecdsa_mode ecdsa_type;
		uint32_t random_len;
	} auth_type;
	uint32_t key_idx;
	hal_data *data;
	union {
		hal_data *data;
		hal_dh_data *dh_param;
		hal_ecdh_data *ecdh_param;
	} auth_data;
};

struct seclink_crypto_info {
	uint32_t key_idx;
	hal_data *input;
	hal_data *output;
	hal_aes_param *aes_param;
	hal_rsa_mode *rsa_mode;
};

struct seclink_ss_info {
	uint32_t key_idx;
	hal_data *data;
};

struct seclink_comm_info {
	uint8_t *priv;
};
struct seclink_req {
	union {
		struct seclink_key_info *key;
		struct seclink_auth_info *auth;
		struct seclink_crypto_info *crypto;
		struct seclink_ss_info *ss;
		struct seclink_comm_info *comm;
	} req_type;
	struct seclink_init_param *params;
	int32_t res;
};

/*  Common */
int sl_init(sl_ctx *hnd);
int sl_deinit(sl_ctx hnd);

/*  key manager */
int sl_set_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);
int sl_get_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, _OUT_ hal_data *key);
int sl_remove_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx);
int sl_generate_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx);

/*  Authenticate */
int sl_generate_random(sl_ctx hnd, uint32_t len, _OUT_ hal_data *random);
int sl_get_hash(sl_ctx hnd, hal_hash_type mode, hal_data *input, _OUT_ hal_data *hash);
int sl_get_hmac(sl_ctx hnd, hal_hmac_type mode, hal_data *input, uint32_t key_idx, _OUT_ hal_data *hmac);
int sl_rsa_sign_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign);
int sl_rsa_verify_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);
int sl_ecdsa_sign_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign);
int sl_ecdsa_verify_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);
int sl_dh_generate_param(sl_ctx hnd, uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param);
int sl_dh_compute_shared_secret(sl_ctx hnd, hal_dh_data *dh_param, uint32_t dh_idx, _OUT_ hal_data *shared_secret);
int sl_ecdh_compute_shared_secret(sl_ctx hnd, hal_ecdh_data *ecdh_mode, uint32_t key_idx, _OUT_ hal_data *shared_secret);
int sl_set_certificate(sl_ctx hnd, uint32_t cert_idx, hal_data *cert_in);
int sl_get_certificate(sl_ctx hnd, uint32_t cert_idx, _OUT_ hal_data *cert_out);
int sl_remove_certificate(sl_ctx hnd, uint32_t cert_idx);
/*  deprecated TizenRT 3.1*/
int sl_get_factory_key(sl_ctx hnd, uint32_t key_idx, hal_data *key);
/*  deprecated TizenRT 3.1*/
int sl_get_factory_cert(sl_ctx hnd, uint32_t cert_idx, hal_data *cert);
/*  deprecated TizenRT 3.1*/
int sl_get_factory_data(sl_ctx hnd, uint32_t data_idx, hal_data *data);

/*  Crypto */
int sl_aes_encrypt(sl_ctx hnd, hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *enc_data);
int sl_aes_decrypt(sl_ctx hnd, hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *dec_data);
int sl_rsa_encrypt(sl_ctx hnd, hal_data *dec_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *enc_data);
int sl_rsa_decrypt(sl_ctx hnd, hal_data *enc_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *dec_data);

/*  Secure Storage */
int sl_write_storage(sl_ctx hnd, uint32_t ss_idx, hal_data *data);
int sl_read_storage(sl_ctx hnd, uint32_t ss_idx, _OUT_ hal_data *data);
int sl_delete_storage(sl_ctx hnd, uint32_t ss_idx);

/*  Utils */
char *sl_strerror(int error);
