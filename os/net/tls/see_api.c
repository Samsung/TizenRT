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
#include <string.h>
#include <pthread.h>

#include "tls/pk.h"
#include "tls/pem.h"
#include "tls/x509_crt.h"
#include "tls/pk_internal.h"

#include "tls/see_api.h"

see_mutex_t m_handler = { PTHREAD_MUTEX_INITIALIZER, 0 };

int see_init(void)
{
	SEE_DEBUG("%s called\n", __func__);

	/* */
	if (m_handler.valid) {
		m_handler.valid++;
		return SEE_OK;
	}

	/* Get Mutex */
	if (see_mutex_init(&m_handler) != 0) {
		return SEE_ERROR;
	}

	return SEE_OK;
}

int see_free(void)
{
	SEE_DEBUG("%s called\n", __func__);

	/* */
	if (m_handler.valid > 1) {
		m_handler.valid--;
		return SEE_OK;
	}

	/* Free Mutex */
	if (see_mutex_free(&m_handler) != 0) {
		return SEE_ERROR;
	}

	memset(&m_handler, 0, sizeof(see_mutex_t));

	return SEE_OK;
}

#define SEE_BEGIN_CERT           "-----BEGIN CERTIFICATE-----\n"
#define SEE_END_CERT             "-----END CERTIFICATE-----\n"

#define PEM_BEGIN_CERT           "-----BEGIN CERTIFICATE-----"
#define PEM_END_CERT             "-----END CERTIFICATE-----"

/* Set certificate in Secure storage (support der and pem types) */
int see_set_certificate(unsigned char *cert, unsigned int cert_len, unsigned int cert_index, unsigned int cert_type)
{
	int r;
	unsigned int use_len;
	mbedtls_pem_context pem;
	SEE_DEBUG("%s called\n", __func__);

	if (cert == NULL || cert_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK if (cert_type == CERT_PEM) {
		mbedtls_pem_init(&pem);
		if ((r = mbedtls_pem_read_buffer(&pem, PEM_BEGIN_CERT, PEM_END_CERT, cert, NULL, 0, &use_len)) != 0) {
			SEE_DEBUG("mbedtls_pem_read_buffer fail %x\n", -r);
			_SEE_MUTEX_UNLOCK mbedtls_pem_free(&pem);
			return SEE_INVALID_INPUT_PARAMS;
		}

		ISP_CHECKBUSY();
		if ((r = isp_write_cert(pem.buf, pem.buflen, cert_index)) != 0) {
			SEE_DEBUG("isp_write_cert fail %x\n", r);
			isp_clear(0);
			_SEE_MUTEX_UNLOCK mbedtls_pem_free(&pem);
			return SEE_ERROR;
		}
	} else if (cert_type == CERT_DER) {
		ISP_CHECKBUSY();
		if ((r = isp_write_cert(cert, cert_len, cert_index)) != 0) {
			_SEE_MUTEX_UNLOCK SEE_DEBUG("isp_write_cert fail %x\n", r);
			isp_clear(0);
			return SEE_ERROR;
		}
	} else {
		_SEE_MUTEX_UNLOCK return SEE_INVALID_INPUT_PARAMS;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_get_certificate(unsigned char *cert, unsigned int *cert_len, unsigned int cert_index, unsigned int cert_type)
{
	int r;
	unsigned int buf_len, olen;
	unsigned char buf[4096];
	SEE_DEBUG("%s called\n", __func__);

	if (cert == NULL || cert_len == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (cert_index >= MAX_CERT_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (cert_type != CERT_DER && cert_type != CERT_PEM) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_read_cert(buf, &buf_len, cert_index)) != 0) {
		SEE_DEBUG("isp_read_cert fail %x\n", r);
		isp_clear(0);
		return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK if (cert_type == CERT_PEM) {
		if ((r = mbedtls_pem_write_buffer(SEE_BEGIN_CERT, SEE_END_CERT, buf, buf_len, cert, *cert_len, &olen)) != 0) {
			SEE_DEBUG("change der to pem format fail %x\n", r);
			return SEE_ERROR;
		}
		*cert_len = olen;
	} else {
		memcpy(cert, buf, olen);
		*cert_len = olen;
	}

	return SEE_OK;
}

int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode)
{
	int r;

        SEE_DEBUG("%s called %d %x\n",__func__, h_param->msg_byte_len, mode);

	if (hash == NULL || h_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_hash(hash, h_param, mode)) != 0) {
		SEE_DEBUG("isp_hash fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
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

	_SEE_MUTEX_LOCK
	ISP_CHECKBUSY();
	if ((r = isp_generate_random(data, len / 4)) != 0) {
		isp_clear(0);
		_SEE_MUTEX_UNLOCK
		SEE_DEBUG("isp_generate_random fail %x\n", r);
		return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK

	return SEE_OK;
}

int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX && key_index < 0xFE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_ecdsa_sign_securekey(ecc_sign, hash, hash_len, key_index)) != 0) {
		SEE_DEBUG("isp_ecdsa_sign fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_verify_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX && key_index < 0xFE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_ecdsa_verify_securekey(ecc_sign, hash, hash_len, key_index)) != 0) {
		SEE_DEBUG("isp_ecdsa_verify fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_compute_ecdh_param(struct sECC_KEY *ecc_pub, unsigned int key_index, unsigned char *output, unsigned int *olen)
{
	int r;

	if (ecc_pub == NULL || output == NULL || olen == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_check_keyindex(key_index)) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s : key_index : %d \n", __func__, key_index);

	_SEE_MUTEX_LOCK
	ISP_CHECKBUSY();
	if ((r = isp_compute_ecdh_securekey(output, olen, *ecc_pub, key_index)) != 0) {
		isp_clear(0);
		_SEE_MUTEX_UNLOCK
		SEE_DEBUG("isp_compute_ecdh_param fail %x\n", r);
		return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK

	return SEE_OK;
}

int see_mutex_init(see_mutex_t *m)
{
	if (m == NULL) {
		return -1;
	}

	if (pthread_mutex_init(&m->mutex, NULL)) {
		m->valid = 0;
		return -1;
	}

	m->valid = 1;
	return 0;
}

int see_mutex_free(see_mutex_t *m)
{
	if (m == NULL) {
		return -1;
	}

	if (pthread_mutex_destroy(&m->mutex) != 0) {
		return -1;
	}

	m->valid = 0;
	return 0;
}

int see_mutex_lock(see_mutex_t *m)
{
	if (m == NULL) {
		return -1;
	}

	if (!m->valid) {
		see_init();
	}

	if (pthread_mutex_lock(&m->mutex) != 0) {
		return -1;
	}

	return 0;
}

int see_mutex_unlock(see_mutex_t *m)
{
	if (m == NULL || !m->valid) {
		return -1;
	}

	if (pthread_mutex_unlock(&m->mutex) != 0) {
		return -1;
	}

	return 0;
}

int see_check_keyindex(unsigned int index)
{
#ifdef SEE_SUPPORT_USERKEY
	if (index < MAX_KEY_INDEX) {
		return 0;
	}
#endif

	switch (index) {
	case FACTORYKEY_ARTIK_PSK:
	case FACTORYKEY_ARTIK_DEVICE:
	case FACTORYKEY_DA_CA:
	case FACTORYKEY_DA_DEVICE:
	case FACTORYKEY_DA_PBKEY:
	case FACTORYKEY_IOTIVITY_ECC:
		return 0;
	default:
		return -1;
	}
	return -1;
}
