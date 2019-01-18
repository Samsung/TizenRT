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

/// @file see/see_internal.c
/// @brief security api for using in TLS internal

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "mbedtls/see_internal.h"



int see_setup_key_internal(unsigned char *key_der, unsigned int key_len, unsigned int key_type, unsigned char *key_buf)
{
	int r;

	SEE_DEBUG("%s IN\n", __func__);

	if (key_der == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_type < SECURE_STORAGE_TYPE_KEY_AES || key_type > SECURE_STORAGE_TYPE_KEY_ECC) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_set_encryptedkey(key_der, key_len, key_type, key_buf);
	if (r != 0) {
		isp_clear(0);
		SEE_DEBUG("isp_set_userkey fail %x\n", r);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	SEE_DEBUG("%s OUT\n", __func__);
	return SEE_OK;
}

#if defined(CONFIG_HW_ECDSA_VERIFICATION)
int see_verify_ecdsa_signature_internal(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned char *key_buf)
{
	int r;

	SEE_DEBUG("%s IN\n", __func__);

	if (ecc_sign == NULL || hash == NULL || hash_len == 0 || key_buf == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_ecdsa_verify_md_encryptedkey(ecc_sign, hash, hash_len, key_buf);
	if (r != 0) {
		isp_clear(0);
		SEE_DEBUG("isp_ecdsa_verify fail %x\n", r);
		if (see_mutex_unlock(&m_handler) != SEE_OK) {
			return SEE_MUTEX_UNLOCK_ERROR;
		}
		return SEE_ERROR;
	}

	if (see_mutex_unlock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_UNLOCK_ERROR;
	}

	SEE_DEBUG("%s OUT\n", __func__);
	return SEE_OK;
}
#endif							/* CONFIG_HW_ECDSA_VERIFICATION */

#if defined(CONFIG_HW_RSA_VERIFICATION)
int see_verify_rsa_signature_internal(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned char *key_buf)
{
	int r;

	SEE_DEBUG("%s IN\n", __func__);

	if (rsa_sign == NULL || hash == NULL || hash_len == 0 || key_buf == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_verify_md_encryptedkey(rsa_sign, hash, hash_len, key_buf);
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

	SEE_DEBUG("%s OUT\n", __func__);
	return SEE_OK;
}
#endif							/* CONFIG_HW_RSA_VERIFICATION */

#if defined(CONFIG_HW_RSA_ENC)
int see_rsa_encryption_internal(unsigned char *key_buf, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen)
{
	int r;

	SEE_DEBUG("%s IN\n", __func__);

	if (input == NULL || output == NULL || inlen == 0 || key_buf == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (pad_type != MBEDTLS_RSA_PKCS_V15) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (see_mutex_lock(&m_handler) != SEE_OK) {
		return SEE_MUTEX_LOCK_ERROR;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_encrypt_encryptedkey(output, outlen, input, inlen, key_buf);
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

	SEE_DEBUG("%s OUT\n", __func__);
	return SEE_OK;
}
#endif							/* CONFIG_HW_RSA_ENC */
