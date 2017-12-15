/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file see/see_api.c
/// @brief SEE api is supporting security api for using secure storage.

#include <stdio.h>
#include <pthread.h>

#include "mbedtls/see_api.h"

#ifdef CONFIG_SUPPORT_FULL_SECURITY
#include "mbedtls/pk.h"
#include "mbedtls/pem.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pk_internal.h"
#endif

see_mutex_t m_handler = { PTHREAD_MUTEX_INITIALIZER, 0 };

int see_init(void)
{
	if (m_handler.valid) {
		m_handler.valid++;
		return SEE_OK;
	}

	/* Get Mutex */
	if (see_mutex_init(&m_handler) != SEE_OK) {
		return SEE_MUTEX_INIT_ERROR;
	}

	return SEE_OK;
}

int see_free(void)
{
	if (m_handler.valid > 1) {
		m_handler.valid--;
		return SEE_OK;
	}

	/* Free Mutex */
	if (see_mutex_free(&m_handler) != SEE_OK) {
		return SEE_MUTEX_FREE_ERROR;
	}

	memset(&m_handler, 0, sizeof(see_mutex_t));

	return SEE_OK;
}

int see_get_certificate(unsigned char *cert, unsigned int *cert_len, unsigned int cert_index, unsigned int cert_type)
{
	int r;
	unsigned int buf_len = 0;
	unsigned char *buf = NULL;

	(void)cert_type;

	SEE_DEBUG("%s called\n", __func__);

	if (cert == NULL || cert_len == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_certindex(cert_index)) {
		SEE_DEBUG("wrong index %d\n", cert_index);
		return SEE_INVALID_CERT_INDEX;
	}

	buf = (unsigned char *)malloc(SEE_MAX_BUF_SIZE);

	if (buf == NULL) {
		return SEE_ALLOC_ERROR;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		r = SEE_MUTEX_LOCK_ERROR;
		goto get_cert_exit;
	}

	ISP_CHECKBUSY();
#if defined(CONFIG_SUPPORT_FULL_SECURITY)
	if (cert_index < SEE_MAX_CERT_INDEX) {
		if ((r = isp_read_cert(buf, &buf_len, cert_index)) != 0) {
			isp_clear(0);
			r = SEE_READ_CERT_ERROR;
			if (see_mutex_unlock(&m_handler) != SEE_OK) {
				r = SEE_MUTEX_UNLOCK_ERROR;
			}
			SEE_DEBUG("isp_read_cert fail %x\n", r);
			goto get_cert_exit;
		}
	} else
#endif
	{
		r = isp_get_factorykey_data(buf, &buf_len, cert_index);
		if (r != 0) {
			isp_clear(0);
			r = SEE_READ_CERT_ERROR;
			if (see_mutex_unlock(&m_handler) != SEE_OK) {
				r = SEE_MUTEX_UNLOCK_ERROR;
			}
			SEE_DEBUG("isp_read_cert fail %x\n", r);
			goto get_cert_exit;
		}
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		r = SEE_MUTEX_UNLOCK_ERROR;
		goto get_cert_exit;
	}

	if (*cert_len < buf_len) {
		SEE_DEBUG("input buffer is too small\n");
		r = SEE_INVALID_BUFFER_SIZE;
		goto get_cert_exit;
	}

	memcpy(cert, buf, buf_len);
	*cert_len = buf_len;

get_cert_exit:
	free(buf);
	return r;
}

int see_generate_random(unsigned int *data, unsigned int len)
{
	int r;

	if (data == NULL || len > SEE_MAX_RANDOM_SIZE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s len : %d\n", __func__, len);

	/* Change length to word number */
	if (len & 0x3) {
		len = len + 4 - (len & 0x3);
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	if ((r = isp_generate_random(data, len / 4)) != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_generate_random fail %x\n", r);
		return SEE_GET_RANDOM_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_KEY_INDEX;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	if ((r = isp_ecdsa_sign_md_securekey(ecc_sign, hash, hash_len, key_index)) != 0) {
		SEE_DEBUG("isp_ecdsa_sign fail %x\n", r);
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		return SEE_ECDSA_SIGN_ERROR;
	}
	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}
	return SEE_OK;
}

int see_compute_ecdh_param(struct sECC_KEY *ecc_pub, unsigned int key_index, unsigned char *output, unsigned int *olen)
{
	int r;

	if (ecc_pub == NULL || output == NULL || olen == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_KEY_INDEX;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	if ((r = isp_compute_ecdh_securekey(output, olen, *ecc_pub, key_index)) != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_compute_ecdh_param fail %x\n", r);
		return SEE_ECDH_COMPUTE_ERROR;
	}
	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_mutex_init(see_mutex_t *m)
{
	if (m == NULL) {
		return SEE_MUTEX_INIT_ERROR;
	}

	if (pthread_mutex_init(&m->mutex, NULL)) {
		m->valid = 0;
		return SEE_MUTEX_INIT_ERROR;
	}

	m->valid = 1;
	return SEE_OK;
}

int see_mutex_free(see_mutex_t *m)
{
	if (m == NULL) {
		return SEE_MUTEX_FREE_ERROR;
	}

	if (pthread_mutex_destroy(&m->mutex) != 0) {
		return SEE_MUTEX_FREE_ERROR;
	}

	m->valid = 0;
	return SEE_OK;
}

int see_mutex_lock(see_mutex_t *m)
{
	if (m == NULL) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	if (!m->valid) {
		if (see_init()) {
			return SEE_MUTEX_INIT_ERROR;
		}
	}

	if (pthread_mutex_lock(&m->mutex) != 0) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	return SEE_OK;
}

int see_mutex_unlock(see_mutex_t *m)
{
	if (m == NULL || !m->valid) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	if (pthread_mutex_unlock(&m->mutex) != 0) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_check_certindex(unsigned int index)
{
#ifdef CONFIG_SUPPORT_FULL_SECURITY
	if (index >= MIN_CERT_INDEX && index < SEE_MAX_CERT_INDEX) {
		return SEE_OK;
	}
#endif
	switch (index) {
	case FACTORYKEY_ARTIK_CERT:
		return SEE_OK;
	default:
		return SEE_INVALID_KEY_INDEX;
	}
	return SEE_INVALID_KEY_INDEX;
}

int see_check_keyindex(unsigned int index)
{
#ifdef CONFIG_SUPPORT_FULL_SECURITY
	if (index < SEE_MAX_KEY_INDEX) {
		return SEE_OK;
	}
#endif

	switch (index) {
	case FACTORYKEY_ARTIK_PSK:
	case FACTORYKEY_ARTIK_DEVICE:
		return SEE_OK;
	default:
		return SEE_INVALID_KEY_INDEX;
	}
	return SEE_INVALID_KEY_INDEX;
}

#ifdef CONFIG_SUPPORT_FULL_SECURITY
int see_set_certificate(unsigned char *cert, unsigned int cert_len, unsigned int cert_index, unsigned int cert_type)
{

	int r;
	(void)cert_type;

	if (cert == NULL || cert_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	if (cert_index >= MIN_CERT_INDEX && cert_index < MAX_CERT_INDEX) {
		r = isp_write_cert(cert, cert_len, cert_index);
		if (r != 0) {
			isp_clear(0);
			if (see_mutex_unlock(&m_handler) != SEE_OK) {
				return SEE_MUTEX_UNLOCK_ERROR;
			}
			SEE_DEBUG("isp_write_cert fail %x\n", r);
			return SEE_ERROR;
		}
	} else {
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("wrong index %d\n", cert_index);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_write_secure_storage(unsigned char *data, unsigned int data_len, unsigned int index)
{
	int r;

	if (data == NULL || !data_len) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (index >= MAX_DATA_INDEX || data_len > MAX_DATA_SIZE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_write_storage(data, data_len, index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_write_storage fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_read_secure_storage(unsigned char *data, unsigned int *data_len, unsigned int index)
{
	int r;
	unsigned int t_size;
	unsigned char *t_buf;

	if (data == NULL || !data_len) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (index >= MAX_DATA_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	t_buf = malloc(MAX_DATA_SIZE);

	if (t_buf == NULL) {
		return SEE_ALLOC_ERROR;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		free(t_buf);
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_read_storage(t_buf, &t_size, index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			free(t_buf);
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_read_storage fail %x\n", r);
		free(t_buf);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		free(t_buf);
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	if (t_size > *data_len) {
		free(t_buf);
		return SEE_INVALID_INPUT_PARAMS;
	}

	memcpy(data, t_buf, t_size);
	*data_len = t_size;

	free(t_buf);

	return SEE_OK;
}

int see_get_publickey(unsigned char *key_der, unsigned int *key_len)
{
	int r = 0;

	if (key_der == NULL || key_len == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_get_factorykey_data(key_der, key_len, FACTORYKEY_ARTIK_DEVICE);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_read_publickey fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_get_ecc_publickey(struct sECC_KEY *ecc_pub, unsigned int key_index, unsigned int object_id)
{
	unsigned int r;

	if (ecc_pub == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}
	ISP_CHECKBUSY();
	r = isp_ecdsa_get_publickey_securekey(ecc_pub, key_index, object_id);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_get_ecdsa_pubkey fail %x\n", r);
		return SEE_ERROR;
	}
	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_generate_key(unsigned int key_type, unsigned int key_index, unsigned int key_len, unsigned int pukey_e)
{
	int r = 0;
	unsigned int key = key_type & 0xFF0000;
	unsigned int object_id = key_type & 0xFF;

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	switch (key) {
	case AES_KEY:
		ISP_CHECKBUSY();
		r = isp_aes_generate_key_securekey(key_len, key_index);
		break;
	case HMAC_KEY:
		ISP_CHECKBUSY();
		r = isp_hmac_generate_key_securekey(key_len, key_index);
		break;
	case RSA_KEY:
		ISP_CHECKBUSY();
		r = isp_rsa_generate_key_securekey(key_index, object_id, pukey_e);
		break;
	case ECC_KEY:
		ISP_CHECKBUSY();
		r = isp_ecdsa_generate_key_securekey(key_index, object_id);
		break;
	default:
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	if (r) {
		SEE_DEBUG("isp_generate_key fail %x %x %x\n", r, key, object_id);
		isp_clear(0);
		return SEE_ERROR;
	}

	return SEE_OK;
}

int see_setup_key(unsigned char *key_der, unsigned int key_len, unsigned int key_type, unsigned int key_index)
{
	int r;

	if (key_der == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d key_type %d\n", __func__, key_index, key_type);

	if (key_type < SECURE_STORAGE_TYPE_KEY_AES || key_type > SECURE_STORAGE_TYPE_KEY_ECC) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_set_securekey(key_der, key_len, key_type, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}

		SEE_DEBUG("isp_set_userkey fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_remove_key(unsigned int key_index, unsigned int key_type)
{
	int r;

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_type < SECURE_STORAGE_TYPE_KEY_AES || key_type > SECURE_STORAGE_TYPE_KEY_ECC) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_remove_key(key_type, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}

		SEE_DEBUG("isp_remove_key fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_aes_encrypt(unsigned int key_index, struct sAES_PARAM *aes_param)
{
	int r;

	if (aes_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_aes_encrypt_securekey(aes_param, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}

		SEE_DEBUG("isp_aes_encrypt fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_aes_decrypt(unsigned int key_index, struct sAES_PARAM *aes_param)
{
	int r;

	if (aes_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_aes_decrypt_securekey(aes_param, key_index);
	if (r != 0) {
		isp_clear(0);

		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_aes_decrypt fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_get_hmac(struct sHMAC_MSG *hmac_msg, unsigned char *output, unsigned int object_id, unsigned int key_index)
{
	int r;

	if (!hmac_msg || !output) {
		return SEE_ERROR;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_hmac_securekey(output, hmac_msg, object_id, key_index);
	if (r != 0) {
		isp_clear(0);

		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_hmac fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode)
{
	int r;

	if (hash == NULL || h_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s\n", __func__);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_hash(hash, h_param, mode);
	if (r != 0) {
		isp_clear(0);

		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_hash fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

/* Generate G, P, GX (G^X mod P) */
int see_generate_dhm_params(struct sDH_PARAM *d_param, unsigned int key_index)
{
	int r;

	if (d_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_dh_generate_keypair_userparam_securestorage(d_param, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_generate_dh_param fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

/* Compute shared secret key = GXY ((G^Y)^X mod P) */
int see_compute_dhm_param(struct sDH_PARAM *d_param, unsigned int key_index, unsigned char *output, unsigned int *olen)
{
	int r;

	if (d_param == NULL || output == NULL || olen == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_dh_compute_shared_secret_securekey(output, olen, *d_param, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_compute_dh_param fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_rsa_decryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen)
{
	int r;

	if (input == NULL || output == NULL || inlen == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (pad_type != MBEDTLS_RSA_PKCS_V15) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_decrypt_securekey(output, outlen, input, inlen, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_rsa_decrypt fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_rsa_encryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen)
{
	int r;

	if (input == NULL || output == NULL || inlen == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (pad_type != MBEDTLS_RSA_PKCS_V15) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_encrypt_securekey(output, outlen, input, inlen, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_rsa_encrypt fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_get_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	if (rsa_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_sign_md_securekey(rsa_sign, hash, hash_len, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_rsa_sign fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_verify_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	if (rsa_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_verify_md_securekey(rsa_sign, hash, hash_len, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_rsa_verify fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_verify_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d\n", __func__, key_index);

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_ecdsa_verify_md_securekey(ecc_sign, hash, hash_len, key_index);
	if (r != 0) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_ecdsa_verify fail %x\n", r);
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_write_iotivity_storage(unsigned char *buf, unsigned int buflen)
{
	int r;
	unsigned int index = SEE_IOTIVITY_SLOT;

	/* 0. check input params */
	if (buf == NULL || buflen == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (buflen > SEE_IOTIVITY_MAXSIZE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	/* 1. lock mutex */
	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();

	/* 2. write */
	r = isp_write_cert((unsigned char *)buf, buflen, index);
	if (r) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_write_cert fail %d\n", r);
		return SEE_ERROR;
	}

	/* 3. unlock mutex */
	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_read_iotivity_storage(unsigned char *buf, unsigned int buflen, unsigned int *olen)
{
	int r;
	int index = SEE_IOTIVITY_SLOT;

	/* 0. check input params */
	if (buf == NULL || !buflen) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	*olen = 0;

	/* 1. lock mutex */
	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();

	/* 2. read */
	r = isp_read_cert((unsigned char *)buf, olen, index);
	if (r) {
		isp_clear(0);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_read_cert fail %d\n", r);
		return SEE_ERROR;
	}

	/* 3. unlock mutex */
	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	return SEE_OK;
}

int see_generate_certificate(struct cert_opt opt, unsigned char *out_buf, unsigned int *out_buflen)
{
	int r = 0;
	int cert_len, cert_buflen = 2500;
	unsigned int subject_key_len = 1024;
	unsigned char *subject_key_buf = NULL;
	unsigned char *cert_buf = NULL;

	mbedtls_pk_context subject_key;
	mbedtls_pk_context issuer_key;
	mbedtls_x509write_cert crt;
	mbedtls_mpi serial;

	if (opt.cert_index < MIN_CERT_INDEX || opt.cert_index >= MAX_CERT_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (out_buf == NULL || out_buflen == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	mbedtls_pk_init(&subject_key);
	mbedtls_pk_init(&issuer_key);

	mbedtls_x509write_crt_init(&crt);
	mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);
	mbedtls_mpi_init(&serial);

	mbedtls_pk_setup(&issuer_key, &mbedtls_rsa_info);

	/* Check mandatory params */
	if (!opt.subject_key_index && !opt.issuer_key_index) {
		opt.subject_key_index = SEE_SUBJECT_KEY_INDEX;
		opt.issuer_key_index = SEE_ISSUER_KEY_INDEX;
	}

	/* Check optional params */
	if (!opt.issuer_name || !opt.subject_name || !opt.not_before || !opt.not_after) {
		opt.issuer_name = SEE_ISSUER_NAME;
		opt.subject_name = SEE_SUBJECT_NAME;
		opt.not_before = SEE_NOT_BEFORE;
		opt.not_after = SEE_NOT_AFTER;
		opt.serial = SEE_SERIAL;
		opt.is_ca = SEE_IS_CA;
		opt.max_pathlen = SEE_MAX_PATHLEN;
		opt.key_usage = SEE_KEY_USAGE;
		opt.ns_cert_type = SEE_NS_CERT_TYPE;
	}

	subject_key_buf = malloc(subject_key_len);
	cert_buf = malloc(cert_buflen);

	if (subject_key_buf == NULL) {
		goto see_exit;
	}
	if (cert_buf == NULL) {
		goto see_exit;
	}

	/*
	 *  0. Parse serial to MPI
	 */
	r = mbedtls_mpi_read_string(&serial, 10, opt.serial);
	if (r != 0) {
		SEE_DEBUG("mpi_read_string returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	/*
	 *  1.0 Load subject key
	 */
	if (opt.subject_pub_key) {
		r = mbedtls_pk_parse_public_key(&subject_key, opt.subject_pub_key, opt.subject_pub_keylen);
		if (r != 0) {
			SEE_DEBUG("pk_parse_key returned -0x%x\n\n", -r);
			goto see_exit;
		}
	} else {
		r = isp_get_factorykey_data(subject_key_buf, &subject_key_len, FACTORYKEY_ARTIK_DEVICE);
		if (r != 0) {
			SEE_DEBUG("see_get_publickey returned %x\n\n", r);
			goto see_exit;
		}

		r = mbedtls_pk_parse_public_key(&subject_key, subject_key_buf, subject_key_len);
		if (r != 0) {
			SEE_DEBUG("pk_parse_key returned -0x%02x\n\n", -r);
			goto see_exit;
		}
	}

	/*
	 *  1.1 Set key
	 */
	if (subject_key.pk_info->type == MBEDTLS_PK_RSA) {
		((mbedtls_rsa_context *)(subject_key.pk_ctx))->key_index = opt.subject_key_index;
	}
	if (issuer_key.pk_info->type == MBEDTLS_PK_RSA) {
		((mbedtls_rsa_context *)(issuer_key.pk_ctx))->key_index = opt.issuer_key_index;
	}
	mbedtls_x509write_crt_set_subject_key(&crt, &subject_key);
	crt.issuer_key = &issuer_key;

	/*
	 *  2.0 Setting the certificate value.
	 */
	r = mbedtls_x509write_crt_set_subject_name(&crt, opt.subject_name);
	if (r != 0) {
		SEE_DEBUG("x509write_crt_set_subject_name returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	r = mbedtls_x509write_crt_set_issuer_name(&crt, opt.issuer_name);
	if (r != 0) {
		SEE_DEBUG("x509write_crt_set_issuer_name returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	r = mbedtls_x509write_crt_set_serial(&crt, &serial);
	if (r != 0) {
		SEE_DEBUG("x509write_crt_set_serial returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	r = mbedtls_x509write_crt_set_validity(&crt, opt.not_before, opt.not_after);
	if (r != 0) {
		SEE_DEBUG("x509write_crt_set_validity returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	/*
	 *  2.1 Adding the basic constraints extension.
	 */
	r = mbedtls_x509write_crt_set_basic_constraints(&crt, opt.is_ca, opt.max_pathlen);
	if (r != 0) {
		SEE_DEBUG("x509write_crt_set_basic_constraints returned -0x%02x\n\n", -r);
		goto see_exit;
	}
#if defined(MBEDTLS_SHA1_C)
	/*
	 *  2.2 Adding the Subject/Authority key identifier.
	 */
	r = mbedtls_x509write_crt_set_subject_key_identifier(&crt);
	if (r != 0) {
		SEE_DEBUG("crt_set_subject_key_identifier returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	r = mbedtls_x509write_crt_set_authority_key_identifier(&crt);
	if (r != 0) {
		SEE_DEBUG("crt_set_authority_key_identifier returned -0x%02x\n\n", -r);
		goto see_exit;
	}
#endif
	/*
	 *  2.3 Adding the key usage extension.
	 */
	if (opt.key_usage) {
		r = mbedtls_x509write_crt_set_key_usage(&crt, opt.key_usage);
		if (r != 0) {
			SEE_DEBUG("crt_set_key_usage returned -0x%02x\n\n", -r);
			goto see_exit;
		}
	}
	if (opt.ns_cert_type) {
		r = mbedtls_x509write_crt_set_ns_cert_type(&crt, opt.ns_cert_type);
		if (r != 0) {
			SEE_DEBUG("crt_set_ns_cert_type returned -0x%02x\n\n", -r);
			goto see_exit;
		}
	}

	/*
	 *  3.0 Writing the certificate.
	 */
	cert_len = mbedtls_x509write_crt_der(&crt, cert_buf, cert_buflen, NULL, NULL);
	if (cert_len <= 0) {
		SEE_DEBUG("x509write_crt_der returned -0x%02x\n\n", -cert_len);
		r = cert_len;
		goto see_exit;
	}

	r = mbedtls_pem_write_buffer(SEE_BEGIN_CERT, SEE_END_CERT, cert_buf + cert_buflen - cert_len, cert_len, out_buf, *out_buflen, out_buflen);
	if (r != 0) {
		SEE_DEBUG("mbedt pem write buffer fail %x\n", r);
		goto see_exit;
	}

	r = see_set_certificate(out_buf, *out_buflen, opt.cert_index, CERT_PEM);
	if (r != 0) {
		SEE_DEBUG("mbedt pem write buffer fail %x\n", r);
		goto see_exit;
	}

see_exit:
	mbedtls_pk_free(&subject_key);
	mbedtls_pk_free(&issuer_key);
	mbedtls_x509write_crt_free(&crt);
	mbedtls_mpi_free(&serial);

	if (cert_buf) {
		free(cert_buf);
	}
	if (subject_key_buf) {
		free(subject_key_buf);
	}

	if (r) {
		return SEE_ERROR;
	}

	return SEE_OK;
}

#endif
