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
#include <tinyara/config.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/seclink_drv.h>
#include <tinyara/security_hal.h>
#include <tinyara/kmalloc.h>

#define _IN_
#define _OUT_
#define _INOUT_

#define KN_LOG printf
#define KN_TAG "KONAI"
#define KN_ENTER														\
	do {																\
		KN_LOG(KN_TAG"[INFO] %s %s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
	} while (0)

//NOTE: THIS FILE IS AN EMPTY SHELL.
int konai_free_data(_IN_ hal_data *data)
{
	if (data) {
		if (data->data) {
			kmm_free(data->data);
		}
	}
	return 0;
}

int konai_get_status(void)
{
	return 0;
}

int konai_init(_IN_ hal_init_param *params)
{
	KN_ENTER;

	return 0;
}

int konai_deinit(void)
{
	KN_ENTER;

	return 0;
}


/**
 * Key Manager
 */
int konai_set_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _IN_ hal_data *key, _IN_ hal_data *prikey)
{
	KN_ENTER;

	return 0;
}

int konai_get_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _OUT_ hal_data *key)
{
	KN_ENTER;

	return 0;
}

int konai_remove_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	KN_ENTER;

	return 0;
}

int konai_generate_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	KN_ENTER;

	return 0;
}

/**
 * Authenticate
 */
int konai_generate_random(_IN_ uint32_t len, _OUT_ hal_data *random)
{
	KN_ENTER;

	return 0;
}

int konai_get_hash(_IN_ hal_hash_type mode, _IN_ hal_data *input, _OUT_ hal_data *hash)
{
	KN_ENTER;

	return 0;
}

int konai_get_hmac(_IN_ hal_hmac_type mode, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac)
{
	KN_ENTER;

	return 0;
}

int konai_rsa_sign_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign)
{
	KN_ENTER;

	return 0;
}

int konai_rsa_verify_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	KN_ENTER;

	return 0;
}

int konai_ecdsa_sign_md(_IN_ hal_data *hash, _IN_ uint32_t key_idx, _INOUT_ hal_ecdsa_mode *mode, _OUT_ hal_data *sign)
{
	KN_ENTER;

	return 0;
}

int konai_ecdsa_verify_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	KN_ENTER;

	return 0;
}

int konai_dh_generate_param(_IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param)
{
	KN_ENTER;

	return 0;
}

int konai_dh_compute_shared_secret(_IN_ hal_dh_data *dh_param, _IN_ uint32_t dh_idx, _OUT_ hal_data *shared_secret)
{
	KN_ENTER;

	return 0;
}

int konai_ecdh_compute_shared_secret(_IN_ hal_ecdh_data *ecdh_mode, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret)
{
	KN_ENTER;

	return 0;
}

static hal_data cert_s;
int konai_set_certificate(_IN_ uint32_t cert_idx, _IN_ hal_data *cert_in)
{
	KN_ENTER;

	cert_s.data = (unsigned char *)kmm_malloc(cert_in->data_len);
	memcpy(cert_s.data, cert_in->data, cert_in->data_len);
	cert_s.data_len = cert_in->data_len;

	return 0;
}

int konai_get_certificate(_IN_ uint32_t cert_idx, _OUT_ hal_data *cert_out)
{
	KN_ENTER;

	cert_out->data = (unsigned char *)kmm_malloc(cert_s.data_len);
	memcpy(cert_out->data, cert_s.data, cert_s.data_len);
	cert_out->data_len = cert_s.data_len;

	return 0;
}

int konai_remove_certificate(_IN_ uint32_t cert_idx)
{
	KN_ENTER;

	kmm_free(cert_s.data);
	cert_s.data_len = 0;

	return 0;
}

static hal_data factory_s = {"factory", 7, NULL};

int konai_get_factory_key(_IN_ uint32_t key_idx, _IN_ hal_data *key)
{
	key->data = (unsigned char *)kmm_malloc(7);
	memcpy(key->data, factory_s.data, 7);

	return 0;
}

int konai_get_factory_cert(_IN_ uint32_t cert_idx, _IN_ hal_data *cert)
{
	KN_ENTER;

	cert->data = (unsigned char *)kmm_malloc(7);
	memcpy(cert->data, factory_s.data, 7);

	return 0;
}

int konai_get_factory_data(_IN_ uint32_t data_idx, _IN_ hal_data *data)
{
	KN_ENTER;

	data->data = (unsigned char *)kmm_malloc(7);
	memcpy(data->data, factory_s.data, 7);

	return 0;
}

/**
 * Crypto
 */
int konai_aes_encrypt(_IN_ hal_data *dec_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	KN_ENTER;

	enc_data->data = (unsigned char *)kmm_malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data, dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return 0;
}

int konai_aes_decrypt(_IN_ hal_data *enc_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	KN_ENTER;

	dec_data->data = (unsigned char *)kmm_malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data, enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return 0;
}

int konai_rsa_encrypt(_IN_ hal_data *dec_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	KN_ENTER;

	enc_data->data = (unsigned char *)kmm_malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data, dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return 0;
}
int konai_rsa_decrypt(_IN_ hal_data *enc_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	KN_ENTER;

	dec_data->data = (unsigned char *)kmm_malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data, enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return 0;
}

/**
 * Secure Storage
 */
int konai_write_storage(_IN_ uint32_t ss_idx, _IN_ hal_data *data)
{
	KN_ENTER;

	return 0;
}

int konai_read_storage(_IN_ uint32_t ss_idx, _OUT_ hal_data *data)
{
	KN_ENTER;

	return 0;
}

int konai_delete_storage(_IN_ uint32_t ss_idx)
{
	KN_ENTER;

	return 0;
}

static struct sec_ops_s g_konai_ops = {
	konai_init,
	konai_deinit,
	konai_free_data,
	konai_get_status,
	konai_set_key,
	konai_get_key,
	konai_remove_key,
	konai_generate_key,
	konai_generate_random,
	konai_get_hash,
	konai_get_hmac,
	konai_rsa_sign_md,
	konai_rsa_verify_md,
	konai_ecdsa_sign_md,
	konai_ecdsa_verify_md,
	konai_dh_generate_param,
	konai_dh_compute_shared_secret,
	konai_ecdh_compute_shared_secret,
	konai_set_certificate,
	konai_get_certificate,
	konai_remove_certificate,
	konai_get_factory_key,
	konai_get_factory_cert,
	konai_get_factory_data,
	konai_aes_encrypt,
	konai_aes_decrypt,
	konai_rsa_encrypt,
	konai_rsa_decrypt,
	konai_write_storage,
	konai_read_storage,
	konai_delete_storage,
};

static struct sec_lowerhalf_s g_konai_lower = {&g_konai_ops, NULL};

int se_initialize(void)
{
	int res = se_register(SECLINK_PATH, &g_konai_lower);
	if (res != 0) {
		return -1;
	}

	return 0;
}

/*
 * It should not be used to general case.
 * It's for testing HAL APIs in user space.
 * Furthermore it wouldn't be used if binary separation is enabled.
 * Because user space couldn't access kernel space variable directly.
 * So it is protected by CONFIG after binary separation is applied.
 */
struct sec_lowerhalf_s *se_get_device(void)
{
	return &g_konai_lower;
}
