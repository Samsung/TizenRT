/**************************************************************************************************
*
* Copyright 2020 Samsung Electronics All Rights Reserved.
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
***************************************************************************************************/

#include <sys/types.h>
#include <string.h>
#include <stdint.h>

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <semaphore.h>
#include <tinyara/seclink_drv.h>
#include <tinyara/kmalloc.h>
#include "rtl_se_crypto_function.h"

#ifdef CONFIG_AMEBAD_TRUSTZONE
#include <arch/chip/amebad_nsc.h>
#endif
#ifdef CONFIG_AMEBALITE_TRUSTZONE
#include <arch/chip/amebalite_nsc.h>
#endif
#include <device_lock.h>

static uint8_t *ns_buf = NULL;
extern void *rtl_set_ns_func(void);

/**
 * Helper function
 */
static int se_ameba_index_chk(uint32_t key_idx)
{
	int ret = HAL_SUCCESS;

	/* Check if in valid Factory range */
	if ((key_idx >= USABLE_FACTORY_KEY_INDEX) && (key_idx < FACTORY_KEY_INDEX_MAX)) {
		ret = HAL_INVALID_SLOT_RANGE;
	}

	return ret;
}

int se_ameba_hal_init(hal_init_param *params)
{
	AWRAP_ENTER;

	int ret = HAL_SUCCESS;
	int count = 0;
	factory_struc input_data;
	ns_passin_struc ns_passin;

	/* Assign Pass in function and buff */
	ns_buf = (uint8_t *)kmm_malloc(NS_BUF_LEN);
	if (ns_buf == NULL) {
		ret = HAL_NOT_ENOUGH_MEMORY;
		goto exit;
	}
	ns_passin.ns_func_s = rtl_set_ns_func();
	ns_passin.buf = ns_buf;
	ns_passin.buf_len = NS_BUF_LEN;

	/* Factory Slot Key type */
	for (count = 3; count < USABLE_FACTORY_KEY_INDEX; count++) {
		input_data.factory_slot_key_type[count] = HAL_KEY_UNKNOWN;
	}
	input_data.factory_slot_key_type[0] = HAL_KEY_ECC_SEC_P256R1;
	input_data.factory_slot_key_type[1] = HAL_KEY_AES_128;
	input_data.factory_slot_key_type[2] = HAL_KEY_AES_128;

	/* Factory Key and Cert address (Flash) */
	input_data.factory_cert_addr = FACTORY_CERT_ADDR;
	input_data.factory_key_addr = FACTORY_KEY_ADDR;

	/* Secure Storage Base address (Flash) */
	input_data.sstorage_addr = SEC_STORE_OFFSET;

	/* Secure Storage Key Address (efuse) */
	input_data.samsung_key_addr = SAMSUNG_KEY_ADDR;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_init(params, &input_data, &ns_passin);
	up_free_secure_context();
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

exit:
	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_deinit(void)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_deinit();
	up_free_secure_context();

	if (ret == HAL_SUCCESS) {
		if (ns_buf != NULL) {
			kmm_free(ns_buf);
			ns_buf = NULL;
		}
	} else {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_free_data(hal_data *data)
{
	AWRAP_ENTER;

	if (data) {
		if (data->data) {
			kmm_free(data->data);
			data->data = NULL;
		}
		if (data->priv) {
			kmm_free(data->priv);
			data->priv = NULL;
		}
	}
	return HAL_SUCCESS;
}

int se_ameba_hal_get_status(void)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_status();
	up_free_secure_context();

	return ret;
}

int se_ameba_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc pub_prikey;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	pub_prikey.input_data = key;
	pub_prikey.output_data = prikey;
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_set_key(mode, key_idx, &pub_prikey);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_key(mode, key_idx, key);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_remove_key(hal_key_type mode, uint32_t key_idx)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_remove_key(mode, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_generate_key(hal_key_type mode, uint32_t key_idx)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_generate_key(mode, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_generate_random(uint32_t len, hal_data *random)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	if (len > HAL_MAX_RANDOM_SIZE) {
		return HAL_INVALID_ARGS;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_generate_random(len, random);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_get_hash(hal_hash_type mode, hal_data *input, hal_data *hash)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_hash(mode, input, hash);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_get_hmac(hal_hmac_type mode, hal_data *input, uint32_t key_idx, hal_data *hmac)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = input;
	in_output.output_data = hmac;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_hmac(mode, &in_output, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_rsa_sign_md(hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = hash;
	in_output.output_data = sign;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_rsa_sign_md(mode, &in_output, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_rsa_verify_md(hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = hash;
	in_output.output_data = sign;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_rsa_verify_md(mode, &in_output, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_ecdsa_sign_md(hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = hash;
	in_output.output_data = sign;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_ecdsa_sign_md(mode, &in_output, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = hash;
	in_output.output_data = sign;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_ecdsa_verify_md(mode, &in_output, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_dh_generate_param(uint32_t dh_idx, hal_dh_data *dh_param)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(dh_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_dh_generate_param(dh_idx, dh_param);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_dh_compute_shared_secret(hal_dh_data *dh_param, uint32_t dh_idx, hal_data *shared_secret)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(dh_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_dh_compute_shared_secret(dh_param, dh_idx, shared_secret);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_ecdh_compute_shared_secret(hal_ecdh_data *ecdh_param, uint32_t key_idx, hal_data *shared_secret)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_ecdh_compute_shared_secret(ecdh_param, key_idx, shared_secret);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_set_certificate(uint32_t cert_idx, hal_data *cert_in)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(cert_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_set_certificate(cert_idx, cert_in);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_get_certificate(uint32_t cert_idx, hal_data *cert_out)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(cert_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_certificate(cert_idx, cert_out);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_remove_certificate(uint32_t cert_idx)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	ret = se_ameba_index_chk(cert_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_remove_certificate(cert_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_get_factory_key(uint32_t key_idx, hal_data *key)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_factory_key(key_idx, key);
	up_free_secure_context();
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_get_factory_cert(uint32_t cert_idx, hal_data *cert)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_factory_cert(cert_idx, cert);
	up_free_secure_context();
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_get_factory_data(uint32_t data_idx, hal_data *data)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_get_factory_data(data_idx, data);
	up_free_secure_context();
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_aes_encrypt(hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *enc_data)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = dec_data;
	in_output.output_data = enc_data;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_aes_encrypt(&in_output, aes_param, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_aes_decrypt(hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *dec_data)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = enc_data;
	in_output.output_data = dec_data;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_aes_decrypt(&in_output, aes_param, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_rsa_encrypt(hal_data *dec_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, hal_data *enc_data)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = dec_data;
	in_output.output_data = enc_data;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_rsa_encrypt(&in_output, rsa_mode, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_rsa_decrypt(hal_data *enc_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, hal_data *dec_data)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;
	inout_struc in_output;

	ret = se_ameba_index_chk(key_idx);
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	in_output.input_data = enc_data;
	in_output.output_data = dec_data;

	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_rsa_decrypt(&in_output, rsa_mode, key_idx);
	up_free_secure_context();

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_write_storage(uint32_t ss_idx, hal_data *data)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	if (ss_idx >= SEC_STORE_MAX_SLOT) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	ss_idx = ss_idx + 1;	/* Change Range to Slot 1-33 */
	if (data->data_len > (SECTOR_SIZE * 2)) {
		/* If data len more than 2 Sector (8KB) */
		return HAL_FAIL;
	} else {
		/* Only Slot 1 can write 8KB, other is less then 4KB */
		if (ss_idx != 1) {
			/* If data len more Sector - Tag size and not Slot 1 */
			if ((data->data_len > (SECTOR_SIZE - 32)) && (ss_idx != 1)) {
				return HAL_FAIL;
			}
		}
	}

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_write_storage(ss_idx, data);
	up_free_secure_context();
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_read_storage(uint32_t ss_idx, hal_data *data)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	if (ss_idx >= SEC_STORE_MAX_SLOT) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if (data->data == NULL) {
		return HAL_INVALID_ARGS;
	}

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	ss_idx = ss_idx + 1;	/* Change Range to Slot 1-33 */
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_read_storage(ss_idx, data);
	up_free_secure_context();
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

int se_ameba_hal_delete_storage(uint32_t ss_idx)
{
	AWRAP_ENTER;
	int ret = HAL_SUCCESS;

	if (ss_idx >= SEC_STORE_MAX_SLOT) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	ss_idx = ss_idx + 1;	/* Change Range to Slot 1-33 */
	up_allocate_secure_context(CONFIG_SE_SECURE_CONTEXT_SIZE);
	ret = ameba_hal_delete_storage(ss_idx);
	up_free_secure_context();
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
	}
	return ret;
}

static struct sec_ops_s g_ameba_ops_s = {
	se_ameba_hal_init,
	se_ameba_hal_deinit,
	se_ameba_hal_free_data,
	se_ameba_hal_get_status,
	se_ameba_hal_set_key,
	se_ameba_hal_get_key,
	se_ameba_hal_remove_key,
	se_ameba_hal_generate_key,
	se_ameba_hal_generate_random,
	se_ameba_hal_get_hash,
	se_ameba_hal_get_hmac,
	se_ameba_hal_rsa_sign_md,
	se_ameba_hal_rsa_verify_md,
	se_ameba_hal_ecdsa_sign_md,
	se_ameba_hal_ecdsa_verify_md,
	se_ameba_hal_dh_generate_param,
	se_ameba_hal_dh_compute_shared_secret,
	se_ameba_hal_ecdh_compute_shared_secret,
	se_ameba_hal_set_certificate,
	se_ameba_hal_get_certificate,
	se_ameba_hal_remove_certificate,
	se_ameba_hal_get_factory_key,
	se_ameba_hal_get_factory_cert,
	se_ameba_hal_get_factory_data,
	se_ameba_hal_aes_encrypt,
	se_ameba_hal_aes_decrypt,
	se_ameba_hal_rsa_encrypt,
	se_ameba_hal_rsa_decrypt,
	se_ameba_hal_write_storage,
	se_ameba_hal_read_storage,
	se_ameba_hal_delete_storage,
};

static struct sec_lowerhalf_s g_ameba_lower = {&g_ameba_ops_s, NULL};

#ifndef LINUX
int se_initialize(void)
{
	int res = se_register(SECLINK_PATH, &g_ameba_lower);
	if (res != 0) {
		lldbg("se3\n");
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
	return &g_ameba_lower;
}
