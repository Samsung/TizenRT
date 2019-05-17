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
#include <sys/types.h>
#include <tinyara/security_hal.h>
#include <security/security_keymgr.h>
#include "security_internal.h"

/**
 * Key Manager
 */
security_error keymgr_generate_key(security_handle hnd, security_key_type algo, const char *key_name)
{
	SECAPI_ENTER;

	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	// convert the security request to the seclink format.
	hal_key_type htype = HAL_KEY_UNKNOWN;
	SECAPI_CONVERT_KEYTYPE(algo, htype);

	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_generate_key(ctx->sl_hnd, htype, key_idx, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}
	SECAPI_RETURN(SECURITY_OK);
}

security_error keymgr_set_key(security_handle hnd, security_key_type algo, const char *key_name, security_data *pubkey, security_data *prikey)
{
	SECAPI_ENTER;

	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	//convert key type
	hal_key_type htype = HAL_KEY_UNKNOWN;
	SECAPI_CONVERT_KEYTYPE(algo, htype);


	// convert path
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	// convert key
	if (!pubkey || !pubkey->data) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}
	hal_data h_pubkey = {pubkey->data, pubkey->length, NULL, 0};
	hal_data h_prikey = {NULL, 0, NULL, 0};
	if (prikey) {
		h_prikey.data = prikey->data;
		h_prikey.data_len = prikey->length;
	}

	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_set_key(ctx->sl_hnd, htype, key_idx, &h_pubkey, &h_prikey, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}
	SECAPI_RETURN(SECURITY_OK);
}

security_error keymgr_get_key(security_handle hnd, security_key_type algo, const char *key_name, security_data *pubkey_x, security_data *pubkey_y)
{
	SECAPI_ENTER;

	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	// ToDo : SEE API needs algo parameter to get key before.
	// But it's possible to get key by key_name without algo
	// Therefore instead of setting algo. it's more reasonable to make algo parameter to get key information from SE.
	// it should be discussed later.

	//convert key type
	hal_key_type htype = HAL_KEY_UNKNOWN;
	SECAPI_CONVERT_KEYTYPE(algo, htype);

	// convert path
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	if (!pubkey_x) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

	// convert key
	hal_data h_pubkey = {ctx->data1, ctx->dlen1, ctx->data2, ctx->dlen2};
	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_get_key(ctx->sl_hnd, htype, key_idx, &h_pubkey, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	if (h_pubkey.priv_len > 0 && !pubkey_y) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

	pubkey_x->data = (unsigned char *)malloc(h_pubkey.data_len);
	if (!pubkey_x->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_DATA_DCOPY(h_pubkey, pubkey_x);

	// check pubkey_y for ECDH
	if (h_pubkey.priv_len == 0) {
		SECAPI_RETURN(SECURITY_OK);
	}

	pubkey_y->data = (unsigned char *)malloc(h_pubkey.priv_len);
	if (!pubkey_y->data) {
		SECAPI_FREE(pubkey_x);
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_PRIV_DCOPY(h_pubkey, pubkey_y);

	SECAPI_RETURN(SECURITY_OK);
}

security_error keymgr_remove_key(security_handle hnd, security_key_type algo, const char *key_name)
{
	SECAPI_ENTER;

	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	//convert key type
	hal_key_type htype = HAL_KEY_UNKNOWN;
	SECAPI_CONVERT_KEYTYPE(algo, htype);

	// convert path
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_remove_key(ctx->sl_hnd, htype, key_idx, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	SECAPI_RETURN(SECURITY_OK);
}
