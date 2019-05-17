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
	case KEY_ECC_SEC_P512R1:
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

hal_hash_type secutils_convert_hashmode_s2h(security_hash_mode mode)
{
	switch (mode) {
	case HASH_MD5:
		return HAL_HASH_MD5;
	case HASH_SHA1:
		return HAL_HASH_SHA1;
	case HASH_SHA224:
		return HAL_HASH_SHA224;
	case HASH_SHA256:
		return HAL_HASH_SHA256;
	case HASH_SHA384:
		return HAL_HASH_SHA384;
	case HASH_SHA512:
		return HAL_HASH_SHA512;
	default:
		return HAL_HASH_UNKNOWN;
	}
	return HAL_HASH_UNKNOWN;
}

hal_hmac_type secutils_convert_hmacmode_s2h(security_hmac_mode mode)
{
	switch (mode) {
	case HMAC_MD5:
		return HAL_HMAC_MD5;
	case HMAC_SHA1:
		return HAL_HMAC_SHA1;
	case HMAC_SHA224:
		return HAL_HMAC_SHA224;
	case HMAC_SHA256:
		return HAL_HMAC_SHA256;
	case HMAC_SHA384:
		return HAL_HMAC_SHA384;
	case HMAC_SHA512:
		return HAL_HMAC_SHA512;
	default:
		return HAL_HMAC_UNKNOWN;
	}
	return HAL_HMAC_UNKNOWN;
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

hal_aes_algo secutils_convert_aesmode_s2h(security_aes_mode mode)
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
	case AES_UNKNOWN:
		return HAL_AES_UNKNOWN;
	}
	return HAL_AES_UNKNOWN;
}

hal_rsa_algo secutils_convert_rsamode_s2h(security_rsa_mode mode)
{
	switch (mode) {
	case RSASSA_PKCS1_V1_5:
		return HAL_RSASSA_PKCS1_V1_5;
	case RSASSA_PKCS1_PSS_MGF1:
		return HAL_RSASSA_PKCS1_PSS_MGF1;
	default:
		return HAL_RSASSA_UNKNOWN;
	}
	return HAL_RSASSA_UNKNOWN;
}

hal_ecdsa_curve secutils_convert_ecdsamode_s2h(security_ecdsa_mode mode)
{
	switch (mode) {
	case ECDSA_BRAINPOOL_P256R1:
		return HAL_ECDSA_BRAINPOOL_P256R1;
	case ECDSA_BRAINPOOL_P384R1:
		return HAL_ECDSA_BRAINPOOL_P384R1;
	case ECDSA_BRAINPOOL_P512R1:
		return HAL_ECDSA_BRAINPOOL_P512R1;
	case ECDSA_SEC_P192R1:
		return HAL_ECDSA_SEC_P192R1;
	case ECDSA_SEC_P224R1:
		return HAL_ECDSA_SEC_P224R1;
	case ECDSA_SEC_P256R1:
		return HAL_ECDSA_SEC_P256R1;
	case ECDSA_SEC_P384R1:
		return HAL_ECDSA_SEC_P384R1;
	case ECDSA_SEC_P512R1:
		return HAL_ECDSA_SEC_P512R1;
	default:
		return HAL_ECDSA_UNKNOWN;
	}
	return HAL_ECDSA_UNKNOWN;
}

hal_dh_key_type secutils_convert_dhmode_s2h(security_dh_mode mode)
{
	switch (mode) {
	case DH_1024:
		return HAL_DH_1024;
	case DH_2048:
		return HAL_DH_2048;
	case DH_4096:
		return HAL_DH_4096;
	default:
		return HAL_DH_UNKNOWN;
	}
	return HAL_DH_UNKNOWN;
}


/* Convert path to slot index of SE */
// To do: security api only support to storing key in secure storage only.
// it'll provide the feature that is storing key in file system later.

int secutils_convert_path_s2h(const char *path, uint32_t *slot)
{
	if (!path) {
		return -1;
	}

	if (!strncmp(path, SS_PATH, sizeof(SS_PATH) - 1)) {
		*slot = atoi(&path[3]);
		return 0;
	}
	// ToDo: check the stored location which is not SE.
	return -1;
}

int secutils_convert_aesparam_s2h(security_aes_param *sparam, hal_aes_param *hparam)
{
	if (!hparam || !sparam) {
		return -1;
	}

	hparam->mode = secutils_convert_aesmode_s2h(sparam->mode);
	if (hparam->mode == HAL_AES_UNKNOWN) {
		return -1;
	}
	hparam->iv = sparam->iv;
	hparam->iv_len = sparam->iv_len;

	return 0;
}

int secutils_convert_rsaparam_s2h(security_rsa_param *sparam, hal_rsa_mode *hparam)
{
	if (!hparam || !sparam) {
		return -1;
	}
	hparam->rsa_a = secutils_convert_rsamode_s2h(sparam->rsa_a);
	if (hparam->rsa_a == HAL_RSASSA_UNKNOWN) {
		return -1;
	}
	hparam->hash_t = secutils_convert_hashmode_s2h(sparam->hash_t);
	if (hparam->hash_t == HAL_HASH_UNKNOWN) {
		return -1;
	}
	hparam->mgf = secutils_convert_hashmode_s2h(sparam->mgf);
	if (hparam->mgf == HAL_HASH_UNKNOWN) {
		return -1;
	}
	hparam->salt_byte_len = sparam->salt_byte_len;

	return 0;
}

int secutils_convert_ecdsaparam_s2h(security_ecdsa_param *eparam, hal_ecdsa_mode *hmode)
{
	if (!eparam || !hmode) {
		return -1;
	}
	hmode->curve = secutils_convert_ecdsamode_s2h(eparam->curve);
	hmode->hash_t = secutils_convert_hashmode_s2h(eparam->hash_t);;

	return 0;
}

int secutils_convert_dhparam_s2h(security_dh_param *dparam, hal_dh_data *hdata)
{
	if (!hdata) {
		return -1;
	}

	if (!dparam || !dparam->G || !dparam->P || !dparam->pubkey) {
		return -1;
	}
	hdata->mode = secutils_convert_dhmode_s2h(dparam->mode);
	if (hdata->mode == HAL_DH_UNKNOWN) {
		return -1;
	}
	hdata->G->data = dparam->G->data;
	hdata->G->data_len = dparam->G->length;
	hdata->P->data = dparam->P->data;
	hdata->P->data_len = dparam->P->length;
	if (dparam->pubkey->data != NULL) {
		hdata->pubkey->data = dparam->pubkey->data;
		hdata->pubkey->data_len = dparam->pubkey->length;
	}
	return 0;
}

int secutils_convert_ecdhparam_s2h(security_ecdh_param *eparam, hal_ecdh_data *hdata)
{
	if (!eparam || !hdata) {
		return -1;
	}
	if (!eparam->pubkey_x || !eparam->pubkey_y) {
		return -1;
	}
	hdata->curve = secutils_convert_ecdsamode_s2h(eparam->curve);
	if (hdata->curve == HAL_ECDSA_UNKNOWN) {
		return -1;
	}

	hdata->pubkey_x->data = eparam->pubkey_x->data;
	hdata->pubkey_x->data_len = eparam->pubkey_x->length;
	hdata->pubkey_y->data = eparam->pubkey_y->data;
	hdata->pubkey_y->data_len = eparam->pubkey_y->length;

	return 0;
}

