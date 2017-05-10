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

#include "tls/see_api.h"

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
#if defined(SEE_SUPPORT_USERCERT)
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
	if ((r = isp_get_factorykey_data(buf, &buf_len, cert_index)) != 0) {
		isp_clear(0);
		r = SEE_READ_CERT_ERROR;
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			r = SEE_MUTEX_UNLOCK_ERROR;
		}
		SEE_DEBUG("isp_read_cert fail %x\n", r);
		goto get_cert_exit;
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
	if ((r = isp_ecdsa_sign_securekey(ecc_sign, hash, hash_len, key_index)) != 0) {
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

	SEE_DEBUG("%s : key_index : %d \n", __func__, key_index);

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
#ifdef SEE_SUPPORT_USERCERT
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
#ifdef SEE_SUPPORT_USERKEY
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
