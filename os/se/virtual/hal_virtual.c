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

#define _IN_
#define _OUT_
#define _INOUT_

#define VH_TAG "[VH]"

#define VH_LOG printf

#define VH_ENTER														\
	do {																\
		VH_LOG(VH_TAG"[INFO] %s %s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
	} while (0)

#define VH_ERR(fd)												\
	do {														\
		VH_LOG("[ERR:%s] %s %s:%d ret(%d) code(%s)\n",			\
			   VH_TAG, __FUNCTION__, __FILE__, __LINE__, fd);	\
	} while (0)

#define SECLINK_PATH "/dev/seclink"

#define V_DEFINE_DATA(name, text)							\
	hal_data name##_data = {text, sizeof(text), NULL, 0};	\
	hal_data *name = &name##_data

#define V_DEFINE_ECC(name, text, text2)									\
	hal_data name##_data = {text, sizeof(text), text2, sizeof(text2)};	\
	hal_data *name = &name##_data

#define V_COPY_DATA(dest, src)								\
	do {													\
		(dest)->data_len = (src)->data_len;					\
		memcpy(dest->data, src->data, (dest)->data_len);	\
	} while (0)

#define V_COPY_ECC(dest, src)								\
	do {													\
		(dest)->data_len = (src)->data_len;					\
		memcpy(dest->data, src->data, (dest)->data_len);	\
		(dest)->priv_len = (src)->priv_len;					\
		memcpy(dest->priv, src->priv, (dest)->priv_len);	\
	} while (0)

static hal_key_type g_asym_hal_key_table[] = {
	/*  RSA */
	HAL_KEY_RSA_1024, // 1024 bits rsa algorithm
	HAL_KEY_RSA_2048, // 2048 bits rsa algorithm
	HAL_KEY_RSA_3072, // 3072 bits rsa algorithm
	HAL_KEY_RSA_4096,
	/*  ECC: it doesn't support whole algorithm that mbedTLS support. it's have to be added*/
	HAL_KEY_ECC_BRAINPOOL_P256R1, // ecc brainpool curve for p256r1
	HAL_KEY_ECC_BRAINPOOL_P384R1, // ecc brainpool curve for p384r1
	HAL_KEY_ECC_BRAINPOOL_P512R1, // ecc brainpool curve for p512r1
	HAL_KEY_ECC_SEC_P192R1, // nist curve for p192r1
	HAL_KEY_ECC_SEC_P224R1, // nist curve for p224r1
	HAL_KEY_ECC_SEC_P256R1, // nist curve for p256r1
	HAL_KEY_ECC_SEC_P384R1, // nist curve for p384r1
	HAL_KEY_ECC_SEC_P512R1, // nist curve for p512r1
	/* DH */
	HAL_KEY_DH_1024,
	HAL_KEY_DH_2048,
	HAL_KEY_DH_4096,
};

static hal_key_type g_ecc_hal_key_table[] = {
	HAL_KEY_ECC_BRAINPOOL_P256R1, // ecc brainpool curve for p256r1
	HAL_KEY_ECC_BRAINPOOL_P384R1, // ecc brainpool curve for p384r1
	HAL_KEY_ECC_BRAINPOOL_P512R1, // ecc brainpool curve for p512r1
	HAL_KEY_ECC_SEC_P192R1, // nist curve for p192r1
	HAL_KEY_ECC_SEC_P224R1, // nist curve for p224r1
	HAL_KEY_ECC_SEC_P256R1, // nist curve for p256r1
	HAL_KEY_ECC_SEC_P384R1, // nist curve for p384r1
	HAL_KEY_ECC_SEC_P512R1, // nist curve for p512r1
};

V_DEFINE_DATA(g_virtual_sym, "Virtual Symmetric Key");
V_DEFINE_DATA(g_virtual_pubkey, "Virtual Public Key");
V_DEFINE_DATA(g_virtual_priv, "Virtual Private Key");
V_DEFINE_ECC(g_virtual_ecckey, "Virtual ECC Pub1 Key", "Virtual ECC Pub2 Key");
V_DEFINE_DATA(g_virtual_dh_G, "Virtual DH G");
V_DEFINE_DATA(g_virtual_dh_P, "Virtual DH P");
V_DEFINE_DATA(g_virtual_dh_pub, "Virtual DH public key");

static int _virtual_is_asymmetric(hal_key_type type)
{
	int i = 0;
	for (; i < sizeof(g_asym_hal_key_table)/sizeof(hal_key_type); i++) {
		if (type == g_asym_hal_key_table[i]) {
			return 1;
		}
	}
	return 0;
}

static int _virtual_is_ecc(hal_key_type type)
{
	int i = 0;
	for (; i < sizeof(g_ecc_hal_key_table)/sizeof(hal_key_type); i++) {
		if (type == g_ecc_hal_key_table[i])
			return 1;
	}
	return 0;
}

/**
 * Common
 */
//NOTE: THIS FILE IS AN EMPTY SHELL.
int virtual_hal_free_data(_IN_ hal_data *data)
{
	if (data) {
		if (data->data) {
			free(data->data);
		}
	}
	return 0;
}

int virtual_hal_get_status(void)
{
	return 0;
}

int virtual_hal_init(_IN_ hal_init_param *params)
{
	VH_ENTER;

	return 0;
}

int virtual_hal_deinit(void)
{
	VH_ENTER;

	return 0;
}

/**
 * Key Manager
 */
int virtual_hal_set_key(_IN_ hal_key_type type, _IN_ uint32_t key_idx, _IN_ hal_data *key, _IN_ hal_data *prikey)
{
	VH_ENTER;
	VH_LOG("type(%d) key index(%d)\n", type, key_idx);
	int is_asym = _virtual_is_asymmetric(type);
	if (is_asym) {
		VH_LOG("Asymmetric Key");
		VH_LOG("Key1 (%p) Key2 (%p)\n", key, prikey);
	} else {
		VH_LOG("Symmetric Key");
		VH_LOG("Key1 (%p) Key2 (%p)\n", key, prikey);
	}

	return 0;
}

int virtual_hal_get_key(_IN_ hal_key_type type, _IN_ uint32_t key_idx, _OUT_ hal_data *key)
{
	VH_ENTER;

	VH_LOG("mode(%d) key index(%d) key(%p)\n", type, key_idx, key);

	// return dummy key data to check data is sent well.
	int is_ecc = _virtual_is_ecc(type);
	if (is_ecc) {
		V_COPY_ECC(key, g_virtual_ecckey);
	} else {
		V_COPY_DATA(key, g_virtual_sym);
		key->priv_len = 0;
	}
	VH_LOG("key(%p), len(%d)\n", key->data, key->data_len);

	return 0;
}

int virtual_hal_remove_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	VH_LOG("mode(%d) key index(%d)\n", mode, key_idx);

	return 0;
}

int virtual_hal_generate_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	VH_LOG("mode(%d) key index(%d)\n", mode, key_idx);

	return 0;
}

/**
 * Authenticate
 */
static const char *virtual_random = "random generated value";
int virtual_hal_generate_random(_IN_ uint32_t len, _OUT_ hal_data *random)
{
	VH_ENTER;
	if (random->data_len < len) {
		return -1;
	}
	random->data_len = strlen(virtual_random) + 1;
	memcpy(random->data, virtual_random, random->data_len);

	return 0;
}

int virtual_hal_get_hash(_IN_ hal_hash_type mode, _IN_ hal_data *input, _OUT_ hal_data *hash)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_get_hmac(_IN_ hal_hmac_type mode, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_rsa_sign_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *md)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_rsa_verify_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_ecdsa_sign_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_ecdsa_verify_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_dh_generate_param(_IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param)
{
	VH_ENTER;

	V_COPY_DATA(dh_param->G, g_virtual_dh_G);
	V_COPY_DATA(dh_param->P, g_virtual_dh_P);
	V_COPY_DATA(dh_param->pubkey, g_virtual_dh_pub);

	return 0;
}

int virtual_hal_dh_compute_shared_secret(_IN_ hal_dh_data *param, _IN_ uint32_t dh_idx, _OUT_ hal_data *shared_secret)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_ecdh_compute_shared_secret(_IN_ hal_ecdh_data *ecdh_mode, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret)
{
	VH_ENTER;
	return 0;
}



int virtual_hal_set_certificate(_IN_ uint32_t cert_idx, _IN_ hal_data *cert_in)
{
	VH_ENTER;

	if (!cert_in || !cert_in->data) {
		return -1;
	}
	VH_LOG("cert len(%d)\n", cert_in->data_len);

	return 0;
}

int virtual_hal_get_certificate(_IN_ uint32_t cert_idx, _OUT_ hal_data *cert_out)
{
	VH_ENTER;

	char *cert_data = {"cert_ccccccccccccceeeeeeeeeeeerrrrrrrrrrrttttttttttt"};
	hal_data cert_s;
	cert_s.data = (void *)cert_data;
	cert_s.data_len = sizeof(cert_data);

	//cert_out->data = (unsigned char *)malloc(cert_s.data_len);
	memcpy(cert_out->data, cert_s.data, cert_s.data_len);
	cert_out->data_len = cert_s.data_len;

	return 0;
}

int virtual_hal_remove_certificate(_IN_ uint32_t cert_idx)
{
	VH_ENTER;

	return 0;
}

int virtual_hal_get_factory_key(_IN_ uint32_t key_idx, _IN_ hal_data *key)
{
	VH_ENTER;

	hal_data factory_s = {"factory", 7, NULL};

	key->data = (unsigned char *)malloc(7);
	memcpy(key->data, factory_s.data, 7);

	return 0;
}

int virtual_hal_get_factory_cert(_IN_ uint32_t cert_idx, _IN_ hal_data *cert)
{
	VH_ENTER;

	hal_data factory_s = {"factory", 7, NULL};

	cert->data = (unsigned char *)malloc(7);
	memcpy(cert->data, factory_s.data, 7);

	return 0;
}

int virtual_hal_get_factory_data(_IN_ uint32_t data_idx, _IN_ hal_data *data)
{
	VH_ENTER;

	hal_data factory_s = {"factory", 7, NULL};

	data->data = (unsigned char *)malloc(7);
	memcpy(data->data, factory_s.data, 7);

	return 0;
}

/**
 * Crypto
 */
int virtual_hal_aes_encrypt(_IN_ hal_data *dec_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	VH_ENTER;

	enc_data->data = (unsigned char *)malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data, dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return 0;
}

int virtual_hal_aes_decrypt(_IN_ hal_data *enc_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	VH_ENTER;

	dec_data->data = (unsigned char *)malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data, enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return 0;
}

int virtual_hal_rsa_encrypt(_IN_ hal_data *dec_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	VH_ENTER;

	enc_data->data = (unsigned char *)malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data, dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return 0;
}

int virtual_hal_rsa_decrypt(_IN_ hal_data *enc_data, _IN_ hal_rsa_mode *mode, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	VH_ENTER;

	dec_data->data = (unsigned char *)malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data, enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return 0;
}

/**
 * Secure Storage
 */
int virtual_hal_write_storage(_IN_ uint32_t ss_idx, _IN_ hal_data *data)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_read_storage(_IN_ uint32_t ss_idx, _OUT_ hal_data *data)
{
	VH_ENTER;
	return 0;
}

int virtual_hal_delete_storage(_IN_ uint32_t ss_idx)
{
	VH_ENTER;
	return 0;
}

static struct sec_ops_s g_virtual_ops = {
	virtual_hal_init,
	virtual_hal_deinit,
	virtual_hal_free_data,
	virtual_hal_get_status,
	virtual_hal_set_key,
	virtual_hal_get_key,
	virtual_hal_remove_key,
	virtual_hal_generate_key,
	virtual_hal_generate_random,
	virtual_hal_get_hash,
	virtual_hal_get_hmac,
	virtual_hal_rsa_sign_md,
	virtual_hal_rsa_verify_md,
	virtual_hal_ecdsa_sign_md,
	virtual_hal_ecdsa_verify_md,
	virtual_hal_dh_generate_param,
	virtual_hal_dh_compute_shared_secret,
	virtual_hal_ecdh_compute_shared_secret,
	virtual_hal_set_certificate,
	virtual_hal_get_certificate,
	virtual_hal_remove_certificate,
	virtual_hal_get_factory_key,
	virtual_hal_get_factory_cert,
	virtual_hal_get_factory_data,
	virtual_hal_aes_encrypt,
	virtual_hal_aes_decrypt,
	virtual_hal_rsa_encrypt,
	virtual_hal_rsa_decrypt,
	virtual_hal_write_storage,
	virtual_hal_read_storage,
	virtual_hal_delete_storage,
};

static struct sec_lowerhalf_s g_virtual_lower = {&g_virtual_ops, NULL};

#ifndef LINUX
int se_initialize(void)
{
	int res = se_register(SECLINK_PATH, &g_virtual_lower);
	if (res != 0) {
		VH_ERR(res);
		return -1;
	}

	return 0;
}
#endif

/*
 * It should not be used to general case.
 * It's for testing HAL APIs in user space.
 * Furthermore it wouldn't be used if binary separation is enabled.
 * Because user space couldn't access kernel space variable directly.
 * So it is protected by CONFIG after binary separation is applied.
 */
struct sec_lowerhalf_s *se_get_device(void)
{
	return &g_virtual_lower;
}
