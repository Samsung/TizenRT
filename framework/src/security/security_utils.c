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

#include <stdlib.h>
#include <stdint.h>
#include <security/security_api.h>
#include <tinyara/seclink.h>
#include <security/security_common.h>
#include "security_internal.h"

#define SS_PATH "ss/"


/**
 * Private
 */

//hal_key_type _convert_algo_stoh(security_key_type sec)
hal_key_type secutils_convert_key_s2h(security_key_type sec)
{
	switch (sec) {
	case KEY_AES_128:
		return HAL_KEY_AES_128;
	case KEY_AES_192:
		return HAL_KEY_AES_192;
	case KEY_AES_256:
		return HAL_KEY_AES_256;
	case KEY_RSA_1024:
		return HAL_KEY_RSA_1024;
	case KEY_RSA_2048:
		return HAL_KEY_RSA_2048;
	case KEY_RSA_3072:
		return HAL_KEY_RSA_3072;
	case KEY_RSA_4096:
		return HAL_KEY_RSA_4096;
	case KEY_ECC_BRAINPOOL_P256R1:
		return HAL_KEY_ECC_BRAINPOOL_P256R1;
	case KEY_ECC_BRAINPOOL_P384R1:
		return HAL_KEY_ECC_BRAINPOOL_P384R1;
	case KEY_ECC_BRAINPOOL_P512R1:
		return HAL_KEY_ECC_BRAINPOOL_P512R1;
	case KEY_ECC_SEC_P192R1:
		return HAL_KEY_ECC_SEC_P192R1;
	case KEY_ECC_SEC_P224R1:
		return HAL_KEY_ECC_SEC_P224R1;
	case KEY_ECC_SEC_P256R1:
		return HAL_KEY_ECC_SEC_P256R1;
	case KEY_ECC_SEC_P384R1:
		return HAL_KEY_ECC_SEC_P384R1;
	case KEY_ECC_SEC_P521R1:
		return HAL_KEY_ECC_SEC_P512R1;
	case KEY_HMAC_MD5:
		return HAL_KEY_HMAC_MD5;
	case KEY_HMAC_SHA1:
		return HAL_KEY_HMAC_SHA1;
	case KEY_HMAC_SHA224:
		return HAL_KEY_HMAC_SHA224;
	case KEY_HMAC_SHA256:
		return HAL_KEY_HMAC_SHA256;
	case KEY_HMAC_SHA384:
		return HAL_KEY_HMAC_SHA384;
	case KEY_HMAC_SHA512:
		return HAL_KEY_HMAC_SHA512;
	case KEY_UNKNOWN:
		return HAL_KEY_UNKNOWN;
	}
	return HAL_KEY_UNKNOWN;
}


/* Convert path to slot index of SE */
// To do: security api only support to storing key in secure storage only.
// it'll provide the feature that is storing key in file system later.

int secutils_convert_path_s2h(const char *path, uint32_t *slot)
{
	if (!strncmp(path, SS_PATH, sizeof(SS_PATH) - 1)) {
		*slot = atoi(&path[3]);
		return 0;
	}
	// ToDo: check the stored location which is not SE.
	return -1;
}


security_error secutils_convert_error_h2s(hal_result_e herr)
{
	switch (herr) {
	case HAL_SUCCESS:
		return SECURITY_OK;
	case HAL_NOT_INITIALIZED:
		return SECURITY_ERROR;
	case HAL_INVALID_ARGS:
		return SECURITY_INVALID_INPUT_PARAMS;
	case HAL_INVALID_SLOT_RANGE:
		return SECURITY_ERROR;
	case HAL_INVALID_SLOT_TYPE:
		return SECURITY_ERROR;
	case HAL_EMPTY_SLOT:
		return SECURITY_ERROR;
	case HAL_BAD_KEY:
		return SECURITY_INVALID_KEY_INDEX;
	case HAL_BAD_KEY_PAIR:
		return SECURITY_ERROR;
	case HAL_BAD_CERT:
		return SECURITY_INVALID_CERT_INDEX;
	case HAL_BAD_CERTKEY_PAIR:
		return SECURITY_ERROR;
	case HAL_NOT_ENOUGH_MEMORY:
		return SECURITY_ALLOC_ERROR;
	case HAL_ALLOC_FAIL:
		return SECURITY_ALLOC_ERROR;
	case HAL_KEY_IN_USE:
		return SECURITY_KEY_STORAGE_IN_USE;
	case HAL_CERT_IN_USE:
		return SECURITY_KEY_STORAGE_IN_USE;
	case HAL_DATA_IN_USE:
		return SECURITY_KEY_STORAGE_IN_USE;
	case HAL_NOT_SUPPORTED:
		return SECURITY_NOT_SUPPORT;
	case HAL_NOT_IMPLEMENTED:
		return SECURITY_NOT_SUPPORT;
	case HAL_BUSY:
		return SECURITY_RSC_BUSY;
	case HAL_FAIL:
		return SECURITY_ERROR;
	}
	return SECURITY_ERROR;
}

static hal_aes_algo _convert_aesmode_s2h(security_aes_mode mode)
{
	switch (mode) {
	case AES_ECB_NOPAD:
		return HAL_AES_ECB_NOPAD;
	case AES_ECB_ISO9797_M1:
		return HAL_AES_ECB_ISO9797_M1;
	case AES_ECB_ISO9797_M2:
		return HAL_AES_ECB_ISO9797_M2;
	case AES_ECB_PKCS5:
		return HAL_AES_ECB_PKCS5;
	case AES_ECB_PKCS7:
		return HAL_AES_ECB_PKCS7;
	case AES_CBC_NOPAD:
		return HAL_AES_CBC_NOPAD;
	case AES_CBC_ISO9797_M1:
		return HAL_AES_CBC_ISO9797_M1;
	case AES_CBC_ISO9797_M2:
		return HAL_AES_CBC_ISO9797_M2;
	case AES_CBC_PKCS5:
		return HAL_AES_CBC_PKCS5;
	case AES_CBC_PKCS7:
		return HAL_AES_CBC_PKCS7;
	case AES_CTR:
		return HAL_AES_CTR;
	case AES_UNKNWON:
		return HAL_AES_UNKNOWN;
	}
	return HAL_AES_UNKNOWN;
}

int secutils_convert_aesparam_s2h(security_aes_param *sparam, hal_aes_param *hparam)
{
	if (!hparam || !hparam->iv) {
		return -1;
	}

	if (!sparam) {
		return -1;
	}
	hparam->mode = _convert_aesmode_s2h(sparam->mode);
	hparam->iv = sparam->iv;
	hparam->iv_len = sparam->iv_len;

	return 0;
}

int secutils_convert_rsaparam_s2h(security_rsa_param *sparam, hal_rsa_mode *hparam)
{
	if (!hparam || !sparam) {
		return -1;
	}
	hparam->rsa_a = sparam->rsa_a;
	hparam->hash_t = sparam->hash_t;
	hparam->mgf = sparam->mgf;
	hparam->salt_byte_len = sparam->salt_byte_len;

	return 0;
}
