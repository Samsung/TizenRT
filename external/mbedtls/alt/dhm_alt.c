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

/*
 *  Diffie-Hellman-Merkle key exchange
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 *  The following sources were referenced in the design of this implementation
 *  of the Diffie-Hellman-Merkle algorithm:
 *
 *  [1] Handbook of Applied Cryptography - 1997, Chapter 12
 *      Menezes, van Oorschot and Vanstone
 *
 */

#include <tinyara/config.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#include "mbedtls/common.h"

#if defined(MBEDTLS_DHM_C)

#include "mbedtls/dhm.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include <string.h>

#if defined(MBEDTLS_PEM_PARSE_C)
#include "mbedtls/pem.h"
#endif

#if defined(MBEDTLS_ASN1_PARSE_C)
#include "mbedtls/asn1.h"
#endif

#include "mbedtls/platform.h"

#if defined(MBEDTLS_DHM_ALT)
#include "mbedtls/alt/dhm_alt.h"
#include "mbedtls/alt/common.h"
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>

/*
 * Verify sanity of parameter with regards to P
 *
 * Parameter should be: 2 <= public_param <= P - 2
 *
 * This means that we need to return an error if
 *              public_param < 2 or public_param > P-2
 *
 * For more information on the attack, see:
 *  http://www.cl.cam.ac.uk/~rja14/Papers/psandqs.pdf
 *  http://web.nvd.nist.gov/view/vuln/detail?vulnId=CVE-2005-2643
 */
/* Duplicated with dhm.c */
static int dhm_check_range(const mbedtls_mpi *param, const mbedtls_mpi *P)
{
	mbedtls_mpi U;
	int ret = 0;

	mbedtls_mpi_init(&U);

	MBEDTLS_MPI_CHK(mbedtls_mpi_sub_int(&U, P, 2));

	if (mbedtls_mpi_cmp_int(param, 2) < 0 ||
		mbedtls_mpi_cmp_mpi(param, &U) > 0) {
		ret = MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

cleanup:
	mbedtls_mpi_free(&U);
	return ret;
}

static int mbedtls_supported_dhm_size_alt(int size)
{
	switch (size) {
	case DHM_1024:
	case DHM_2048:
	//case DHM_4096:
		return 1;
	default:
		return 0;
	}
	return 0;
}

static void dhm_param_cleanup_alt(hal_dh_data *d_param)
{
	if (d_param->P->data) {
		free(d_param->P->data);
	}
	if (d_param->G->data) {
		free(d_param->G->data);
	}
	if (d_param->pubkey->data) {
		free(d_param->pubkey->data);
	}
}

static int dhm_param_convert_mode_alt(hal_dh_data *d_param, int x_size)
{
	int ret = 0;

	switch (x_size) {
		case DHM_1024:
			d_param->mode = HAL_DH_1024;
			break;
		case DHM_2048:
			d_param->mode = HAL_DH_2048;
			break;
		case DHM_4096:
			d_param->mode = HAL_DH_4096;
			break;
		default:
			ret = MBEDTLS_ERR_DHM_INVALID_FORMAT;
	}

	return ret;
}

static int dhm_make_common_alt(mbedtls_dhm_context *ctx, hal_dh_data *d_param, 
								int x_size, int is_calc_secret)
{
	int ret = 0;
	unsigned int moduler = 0;
	unsigned int generator = 0;
	unsigned int pubkey = 0;

	/*
	 *  1. Initialize G, P, GX context.
	 */
	moduler = mbedtls_mpi_size(&ctx->P);
	generator = mbedtls_mpi_size(&ctx->G);
	pubkey = x_size;

	d_param->P->data_len = moduler;
	d_param->P->data = (unsigned char *)malloc(moduler);
	if (d_param->P->data == NULL) {
		ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
		goto cleanup;
	}

	d_param->G->data_len = generator;
	d_param->G->data = (unsigned char *)malloc(generator);
	if (d_param->G->data == NULL) {
		ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
		goto cleanup;
	}

	d_param->pubkey->data_len = pubkey;
	d_param->pubkey->data = (unsigned char *)malloc(pubkey);
	if (d_param->pubkey->data == NULL) {
		ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
		goto cleanup;
	}

	if (is_calc_secret) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->P, d_param->P->data, moduler));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->G, d_param->G->data, generator));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->GY, d_param->pubkey->data, pubkey));
	} else {
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->P, d_param->P->data, x_size));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->G, d_param->G->data, generator));
	}

	if (ret = dhm_param_convert_mode_alt(&d_param, mbedtls_dhm_get_len(ctx)) != 0) {
		goto cleanup;
	}

cleanup:
	return ret;
}

static int mbedtls_dhm_make_params_alt(mbedtls_dhm_context *ctx, int x_size,
										unsigned char *output, size_t *olen)
{
	int ret = 0;
	unsigned char *p;
	hal_dh_data d_param;
	sl_ctx shnd;

	memset(&d_param, 0, sizeof(hal_dh_data));

	/*
	 *  1. Initialize SecLink
	 */
	if (sl_init(shnd) != SECLINK_OK) {
		ret = MBEDTLS_ERR_DHM_FILE_IO_ERROR;
		return ret;
	}

	/*
	 *  2. Initialize common DHM parameters using shared function
	 */
	if (ret = dhm_make_common_alt(ctx, &d_param, x_size, 0) != 0) {
		goto cleanup;
	}

	/*
	 *  3. Generate X values and calculate GX from sss.
	 */
	if (sl_dh_generate_param(shnd, ctx->key_index, &d_param) != SECLINK_OK) {
		ret = MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
		goto cleanup;
	}

	/*
	 *  4. Export GX from unsigned binary data
	 */
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&ctx->GX, d_param.pubkey->data, d_param.pubkey->data_len));

	/*
	* Export P, G, GX. RFC 5246 §4.4 states that "leading zero octets are
	* not required". We omit leading zeros for compactness.
	*/
#define DHM_MPI_EXPORT(X, n)                                          \
	do {                                                                \
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary((X),               \
												p + 2,               \
												(n)));           \
		*p++ = MBEDTLS_BYTE_1(n);                                     \
		*p++ = MBEDTLS_BYTE_0(n);                                     \
		p += (n);                                                     \
	} while (0)

	/*
	 *  4. Export G, P, GX
	 */
	p = output;
	DHM_MPI_EXPORT(&ctx->P, d_param.P->data_len);
	DHM_MPI_EXPORT(&ctx->G, d_param.G->data_len);
	DHM_MPI_EXPORT(&ctx->GX, d_param.pubkey->data_len);

	*olen = p - output;
	ctx->len = d_param.P->data_len;

cleanup:
	sl_deinit(shnd);
	dhm_param_cleanup_alt(&d_param);

	return ret;
}

static int mbedtls_dhm_make_public_alt(mbedtls_dhm_context *ctx, int x_size,
									unsigned char *output, size_t olen)
{
	int ret = 0;
	hal_dh_data d_param;
	sl_ctx shnd;

	memset(&d_param, 0, sizeof(hal_dh_data));

	/*
	 *  1. Initialize SecLink
	 */
	if (sl_init(shnd) != SECLINK_OK) {
		ret = MBEDTLS_ERR_DHM_FILE_IO_ERROR;
		return ret;
	}

	/*
	 *  2. Initialize common DHM parameters using shared function
	 */
	if (ret = dhm_make_common_alt(ctx, &d_param, x_size, 0) != 0) {
		goto cleanup;
	}

	/*
	 *  3. Generate X values and calculate GX from sss.
	 */
	if (sl_dh_generate_param(shnd, ctx->key_index, &d_param) != SECLINK_OK) {
		ret = MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
		goto cleanup;
	}

	/*
	 *  4. Export GX from unsigned binary data
	 */
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&ctx->GX, d_param.pubkey->data, d_param.pubkey->data_len));

	if ((ret = dhm_check_range(&ctx->GX, &ctx->P)) != 0) {
		goto cleanup;
	}

	MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->GX, output, olen));

cleanup:
	sl_deinit(shnd);
	dhm_param_cleanup_alt(&d_param);

	return ret;
}

static int mbedtls_dhm_calc_secret_alt(mbedtls_dhm_context *ctx,
								unsigned char *output, size_t output_size, size_t *olen)
{
	int ret = 0;
	hal_dh_data d_param;
	hal_data shared_secret = {output, output_size, NULL, 0};
	sl_ctx shnd;

	/*
	 *  1. Initialize SecLink
	 */
	if (sl_init(shnd) != SECLINK_OK) {
		ret = MBEDTLS_ERR_DHM_FILE_IO_ERROR;
		return ret;
	}

	/*
	 *  2. Initialize common DHM parameters using shared function
	 */
	if (ret = dhm_make_common_alt(ctx, &d_param, mbedtls_mpi_size(&ctx->GY), 1) != 0) {
		goto cleanup;
	}

	/*
	 *  3. Calculate shared secret(K) from sss.
	 */
	if (sl_dh_compute_shared_secret(shnd, &d_param, ctx->key_index, &shared_secret) != SECLINK_OK) {
		ret = MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
		goto cleanup;
	}

	/*
	 *  4. Export K
	 */
	*olen = shared_secret.data_len;

	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&ctx->K, output, *olen));

cleanup:
	sl_deinit(shnd);
	dhm_param_cleanup_alt(&d_param);

	return ret;
}

void mbedtls_dhm_init(mbedtls_dhm_context *ctx)
{
	memset(ctx, 0, sizeof(mbedtls_dhm_context));
	ctx->key_index = MBEDTLS_DHM_KEY_INDEX_ALT;
	return;
}

/*
 * Setup and write the ServerKeyExchange parameters
 */
int mbedtls_dhm_make_params(mbedtls_dhm_context *ctx, int x_size,
							unsigned char *output, size_t *olen,
							int (*f_rng)(void *, unsigned char *, size_t),
							void *p_rng)
{
	if (mbedtls_mpi_cmp_int(&ctx->P, 0) == 0) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	if (!mbedtls_supported_dhm_size_alt(x_size)) {
		return MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
	}

	int ret = mbedtls_dhm_make_params_alt(&ctx, x_size, &output, &olen);
	if (ret != 0) {
		return (MBEDTLS_ERR_DHM_MAKE_PUBLIC_FAILED + ret);
	}

	return ret;
}

/*
 * Create own private value X and export G^X
 */
int mbedtls_dhm_make_public(mbedtls_dhm_context *ctx, int x_size,
							unsigned char *output, size_t olen,
							int (*f_rng)(void *, unsigned char *, size_t),
							void *p_rng)
{
	int ret = 0;
	int generator = 0;
	hal_dh_data d_param;
	sl_ctx shnd;

	if (ctx == NULL || olen < 1 || olen > mbedtls_dhm_get_len(ctx)) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	if (!mbedtls_supported_dhm_size_alt(x_size)) {
		return MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
	}

	ret = mbedtls_dhm_make_public_alt(&ctx, x_size, &output, olen);
	if (ret != 0) {
		return (MBEDTLS_ERR_DHM_MAKE_PUBLIC_FAILED + ret);
	}

	return ret;
}


/*
 * Derive and export the shared secret (G^Y)^X mod P
 */
int mbedtls_dhm_calc_secret(mbedtls_dhm_context *ctx,
							unsigned char *output, size_t output_size, size_t *olen,
							int (*f_rng)(void *, unsigned char *, size_t),
							void *p_rng)
{
	if (ctx == NULL || output_size < mbedtls_dhm_get_len(ctx)) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	if (!mbedtls_supported_dhm_size_alt(mbedtls_dhm_get_len(ctx))) {
		return MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
	}

	int ret = mbedtls_dhm_calc_secret_alt(&ctx, &output, output_size, &olen);
	if (ret != 0) {
		return (MBEDTLS_ERR_DHM_MAKE_PUBLIC_FAILED + ret);
	}

	return ret;
}

#endif							/* MBEDTLS_DHM_ALT & CONFIG_HW_DH_PARAM */

#endif							/* MBEDTLS_DHM_C */
