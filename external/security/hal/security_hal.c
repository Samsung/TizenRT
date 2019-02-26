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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <security/hal/security_hal.h>
#define _IN_
#define _OUT_
#define _INOUT_

/*
 * NOTE: THIS FILE IS AN EMPTY SHELL.
 */

int hal_init(void)
{
	return HAL_SUCCESS;;
}

int hal_deinit(void)
{
	return HAL_SUCCESS;;
}

int hal_free_data(_IN_ hal_data *data)
{
	if(data) {
		if(data->data) {
			free(data->data);
		}
	}
	return HAL_SUCCESS;
}

int hal_get_status(void)
{
	return 0;
}

int hal_init(_IN_ hal_init_param *params)
{
	VH_ENTER;

	return 0;
}

/**
 * Key Manager
 */
int hal_set_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _IN_ hal_data *key, _IN_ hal_data *prikey)
{
	return HAL_SUCCESS;
}

int hal_get_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _OUT_ hal_data *key)
{
	return HAL_SUCCESS;
}

int hal_remove_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	return HAL_SUCCESS;
}

int hal_generate_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	return HAL_SUCCESS;
}

/**
 * Authenticate
 */
int hal_generate_random(_IN_ uint32_t len, _OUT_ hal_data *random)
{
	return HAL_SUCCESS;
}

int hal_get_hash(_IN_ hal_hash_type mode, _IN_ hal_data *input, _OUT_ hal_data *hash)
{
	return HAL_SUCCESS;
}

int hal_get_hmac(_IN_ hal_hmac_type mode, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac)
{
	return HAL_SUCCESS;
}

int hal_rsa_sign_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign)
{
	return HAL_SUCCESS;
}

int hal_rsa_verify_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	return HAL_SUCCESS;
}

int hal_ecdsa_sign_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign)
{
	return HAL_SUCCESS;
}

int hal_ecdsa_verify_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	return HAL_SUCCESS;
}

int hal_dh_generate_param(_IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param)
{
	return HAL_SUCCESS;
}

int hal_dh_compute_shared_secret(_IN_ hal_dh_data *dh_param, _IN_ uint32_t dh_idx, _OUT_ hal_data *shared_secret)
{
	return HAL_SUCCESS;
}

int hal_ecdh_compute_shared_secret(_IN_ hal_ecdh_data *ecdh_mode, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret)
{
	return HAL_SUCCESS;
}

static hal_data cert_s;
int hal_set_certificate(_IN_ uint32_t cert_idx, _IN_ hal_data *cert_in)
{
	cert_s.data = (unsigned char *)malloc(cert_in->data_len);
	memcpy(cert_s.data, cert_in->data, cert_in->data_len);
	cert_s.data_len = cert_in->data_len;

	return HAL_SUCCESS;
}

int hal_get_certificate(_IN_ uint32_t cert_idx, _OUT_ hal_data *cert_out)
{
	cert_out->data = (unsigned char *)malloc(cert_s.data_len);
	memcpy(cert_out->data, cert_s.data, cert_s.data_len);
	cert_out->data_len = cert_s.data_len;

	return HAL_SUCCESS;
}

int hal_remove_certificate(_IN_ uint32_t cert_idx)
{
	free(cert_s.data);
	cert_s.data_len = 0;

	return HAL_SUCCESS;
}

static hal_data factory_s = {"factory", 7, NULL};
int hal_get_factorykey_data(_IN_ uint32_t key_idx, _IN_ hal_data *data)
{
	data->data = (unsigned char *)malloc(7);
	memcpy(data->data, factory_s.data, 7);

	return HAL_SUCCESS;
}

/**
 * Crypto
 */
int hal_aes_encrypt(_IN_ hal_data *dec_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	enc_data->data = (unsigned char *)malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data,dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return HAL_SUCCESS;
}

int hal_aes_decrypt(_IN_ hal_data *enc_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	dec_data->data = (unsigned char *)malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data,enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return HAL_SUCCESS;
}

int hal_rsa_encrypt(_IN_ hal_data *dec_data, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	enc_data->data = (unsigned char *)malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data,dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return HAL_SUCCESS;
}
int hal_rsa_decrypt(_IN_ hal_data *enc_data, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	dec_data->data = (unsigned char *)malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data,enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return HAL_SUCCESS;
}

/**
 * Secure Storage
 */
int hal_write_storage(_IN_ uint32_t ss_idx, _IN_ hal_data *data)
{
	return HAL_SUCCESS;
}

int hal_read_storage(_IN_ uint32_t ss_idx, _OUT_ hal_data *data)
{
	return HAL_SUCCESS;
}

int hal_delete_storage(_IN_ uint32_t ss_idx)
{
	return HAL_SUCCESS;
}
