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

#include <security/security_auth.h>
#include "security_internal.h"

/**
 * Authentication
 */
int auth_generate_random(unsigned int size, security_data *random)
{
	SECAPI_ENTER;

	hal_data hrand = {NULL, 0, NULL, 0};
	SECAPI_CALL(sl_generate_random(g_slink_hnd, size, &hrand));

	random->data = (unsigned char *)malloc(hrand.data_len);
	if (!random->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_H2SD_MEMCPY(hrand, random);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_generate_certificate(const char *cert_name, security_csr *csr, security_data *cert)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

int auth_set_certificate(const char *cert_name, security_data *cert)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

int auth_get_certificate(const char *cert_name, security_data *cert)
{
	SECAPI_ENTER;

    /* convert path */
	uint32_t cert_idx = 0;
	int32_t res = _convert_ptos(cert_name, &cert_idx);
	if (res < 0) {
		// it doesn't point SE slot that will generate key. so return err.r
		SECAPI_RETURN(SECURITY_INVALID_KEY_INDEX);
	}

	hal_data cert_out = {NULL, 0, NULL, 0};
	SECAPI_CALL(sl_get_certificate(g_slink_hnd, cert_idx, &cert_out));

	cert->data = (unsigned char *)malloc(cert_out.data_len);
	if (!cert->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_H2SD_MEMCPY(cert_out, cert);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_remove_certificate(const char *cert_name)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);

}
int auth_get_rsa_signature(security_rsa_param mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);

}
int auth_verify_rsa_signature(security_rsa_param mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);

}
int auth_get_ecdsa_signature(security_ecdsa_mode mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;

	hal_ecdsa_mode hmode;
	hmode.curve = _convert_curve_stoh(mode.curve);
	hmode.hash_t = mode.hash_t;

	/* convert path */
	uint32_t key_idx = 0;
	int32_t res = _convert_ptos(key_name, &key_idx);
	if (res < 0) {
		// it doesn't point SE slot that will generate key. so return err.r
		SECAPI_RETURN(SECURITY_INVALID_KEY_INDEX);
	}

	hal_data h_hash = {hash->data, hash->length, NULL, 0};
	hal_data h_sign = {NULL, 0, NULL, 0};

	SECAPI_CALL(sl_ecdsa_sign_md(g_slink_hnd, hmode, &h_hash, key_idx, &h_sign));

	sign->data = (unsigned char *)malloc(h_sign.data_len);
	if (!sign->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_H2SD_MEMCPY(h_sign, sign);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_verify_ecdsa_signature(security_ecdsa_mode mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;

	hal_ecdsa_mode hmode;
	hmode.curve = _convert_curve_stoh(mode.curve);
	hmode.hash_t = mode.hash_t;

	/* convert path */
	uint32_t key_idx = 0;
	int32_t res = _convert_ptos(key_name, &key_idx);
	if (res < 0) {
		// it doesn't point SE slot that will generate key. so return err.r
		SECAPI_RETURN(SECURITY_INVALID_KEY_INDEX);
	}

	hal_data h_hash = {hash->data, hash->length, NULL, 0};
	hal_data h_sign = {sign->data, sign->length, NULL, 0};

	SECAPI_CALL(sl_ecdsa_verify_md(g_slink_hnd, hmode, &h_hash, &h_sign, key_idx));

	SECAPI_RETURN(SECURITY_OK);
}

int auth_get_hash(security_hash_mode algo, security_data *data, security_data *hash)
{
	SECAPI_ENTER;

	hal_hash_type htype = _convert_hash_stoh(algo);
	if (htype == HAL_HASH_UNKNOWN) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

	hal_data input = {data->data, data->length, NULL, 0};
	hal_data output = {NULL, 0, NULL, 0};

	SECAPI_CALL(sl_get_hash(g_slink_hnd, htype, &input, &output));

	hash->data = (unsigned char *)malloc(output.data_len);
	if (!hash->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_H2SD_MEMCPY(output, hash);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_get_hmac(security_hmac_mode algo, const char *key_name, security_data *data, security_data *hmac)
{
	SECAPI_ENTER;

	hal_hmac_type htype = _convert_hmac_stoh(algo);
	if (htype == HAL_HMAC_UNKNOWN) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

	/* convert path */
	uint32_t key_idx = 0;
	int32_t res = _convert_ptos(key_name, &key_idx);
	if (res < 0) {
		// it doesn't point SE slot that will generate key. so return err.r
		SECAPI_RETURN(SECURITY_INVALID_KEY_INDEX);
	}

	hal_data input  = {data->data, data->length, NULL, 0};
	hal_data output = {NULL, 0, NULL, 0};

	SECAPI_CALL(sl_get_hmac(g_slink_hnd, htype, &input, key_idx, &output));

	hmac->data = (unsigned char *)malloc(output.data_len);
	if (!hmac->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_H2SD_MEMCPY(output, hmac);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_generate_dhparams(security_dh_param *params, security_data *pub)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

int auth_set_dhparams(security_dh_param *params, security_data *pub)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

int auth_compute_dhparams(security_dh_param *params, security_data *secret)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

int auth_generate_ecdhkey(security_ecdh_param *params, security_data *pub)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

int auth_compute_ecdhkey(security_ecdh_param *params, security_data *secret)
{
	SECAPI_ENTER;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}
