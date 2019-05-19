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
#include <security/security_ss.h>
#include "security_internal.h"

/**
 * Secure Storage
 */
security_error ss_read_secure_storage(security_handle hnd, const char *ss_name, unsigned int offset, security_data *data)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	// ToDo: offset is not supported yet
	if (offset > 0 || !data) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

    /* convert path */
	uint32_t ss_idx = 0;
	SECAPI_CONVERT_PATH(ss_name, &ss_idx);

	hal_data ss = {ctx->data1, ctx->dlen1, NULL, 0};

	hal_result_e hres = HAL_FAIL;
	SECAPI_CALL(sl_read_storage(ctx->sl_hnd, ss_idx, &ss, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	data->data = (unsigned char *)malloc(ss.data_len);
	if (!data->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_DATA_DCOPY(ss, data);

	SECAPI_RETURN(SECURITY_OK);
}

security_error ss_write_secure_storage(security_handle hnd, const char *ss_name, unsigned int offset, security_data *input)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	// ToDo: offset is not supported yet
	if (offset > 0 || !input) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

    /* convert path */
	uint32_t ss_idx = 0;
	SECAPI_CONVERT_PATH(ss_name, &ss_idx);

	hal_data ss = {input->data, input->length, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_write_storage(ctx->sl_hnd, ss_idx, &ss, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	SECAPI_RETURN(SECURITY_OK);
}

security_error ss_delete_secure_storage(security_handle hnd, const char *ss_name)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	/* convert path */
	uint32_t ss_idx = 0;
	SECAPI_CONVERT_PATH(ss_name, &ss_idx);

	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_delete_storage(ctx->sl_hnd, ss_idx, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	SECAPI_RETURN(SECURITY_OK);
}

security_error ss_get_size_secure_storage(security_handle hnd, const char *ss_name, unsigned int *size)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;
	if (!size) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

	SECAPI_LOG("handle (%p)\n", ctx);

	uint32_t ss_idx = 0;
	SECAPI_CONVERT_PATH(ss_name, &ss_idx);

	// ToDo
	*size = 0;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

security_error ss_get_list_secure_storage(security_handle hnd, security_storage_list *list)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	if (!list) {
		SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);
	}

	SECAPI_LOG("handle (%p)\n", ctx);
	// ToDo
	*list = NULL;

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}
