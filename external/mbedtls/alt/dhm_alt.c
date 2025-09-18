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
#include <System/config.h>
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
	int ret = 0;
	unsigned int moduler = 0;
	unsigned int generator = 0;
	unsigned char *p;
	hal_dh_data d_param;
	sl_ctx shnd;

	if (mbedtls_mpi_cmp_int(&ctx->P, 0) == 0) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	memset(&d_param, 0, sizeof(hal_dh_data));

	if (mbedtls_supported_dhm_size_alt(x_size)) {
		/*
		 *  1. Initialize G, P, GX context.
		 */
		moduler = mbedtls_mpi_size(&ctx->P);
		generator = mbedtls_mpi_size(&ctx->G);

		d_param.P->data_len = moduler;
		d_param.P->data = (unsigned char *)malloc(moduler);
		if (d_param.P->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}
		d_param.G->data_len = generator;
		d_param.G->data = (unsigned char *)malloc(generator);
		if (d_param.G->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}

		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->P, d_param.P->data, moduler));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->G, d_param.G->data, generator));

		d_param.pubkey->data = (unsigned char *)malloc(x_size);
		if (d_param.pubkey->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}
		d_param.pubkey->data_len = x_size;
		switch (x_size) {
		case DHM_1024:
			d_param.mode = HAL_DH_1024;
			break;
		case DHM_2048:
			d_param.mode = HAL_DH_2048;
			break;
		case DHM_4096:
			d_param.mode = HAL_DH_4096;
			break;
		default:
			ret = MBEDTLS_ERR_DHM_INVALID_FORMAT;
			goto cleanup;
		}

		/*
		 *  2. Generate X values and calculate GX from sss.
		 */
		ret = sl_init(&shnd);
		if (ret != SECLINK_OK) {
			ret = MBEDTLS_ERR_DHM_FILE_IO_ERROR;
			goto cleanup;
		}

		ret = sl_dh_generate_param(shnd, ctx->key_index, &d_param);
		if (ret != SECLINK_OK) {
			sl_deinit(shnd);
			ret = MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
			goto cleanup;
		}

		/*
		 *  3. Export GX from unsigned binary data
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

		sl_deinit(shnd);
	} else {
		return MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
	}
cleanup:
	if (d_param.P->data) {
		free(d_param.P->data);
	}
	if (d_param.G->data) {
		free(d_param.G->data);
	}
	if (d_param.pubkey->data) {
		free(d_param.pubkey->data);
	}

	if (ret != 0) {
		return (MBEDTLS_ERR_DHM_MAKE_PUBLIC_FAILED + ret);
	}

	return 0;
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

	memset(&d_param, 0, sizeof(hal_dh_data));

	if (mbedtls_supported_dhm_size_alt(x_size)) {
		/*
		 *  1. Initialize G, P, GX context.
		 */
		generator = mbedtls_mpi_size(&ctx->G);

		d_param.P->data_len = x_size;
		d_param.P->data = (unsigned char *)malloc(x_size);
		if (d_param.P->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}
		d_param.G->data_len = generator;
		d_param.G->data = (unsigned char *)malloc(generator);
		if (d_param.G->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}

		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->P, d_param.P->data, x_size));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->G, d_param.G->data, generator));

		d_param.pubkey->data = (unsigned char *)malloc(x_size);
		if (d_param.pubkey->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}
		d_param.pubkey->data_len = x_size;
		switch (x_size) {
		case DHM_1024:
			d_param.mode = HAL_DH_1024;
			break;
		case DHM_2048:
			d_param.mode = HAL_DH_2048;
			break;
		case DHM_4096:
			d_param.mode = HAL_DH_4096;
			break;
		default:
			ret = MBEDTLS_ERR_DHM_INVALID_FORMAT;
			goto cleanup;
		}

		/*
		 *  2. Generate X values and calculate GX from sss.
		 */
		ret = sl_init(&shnd);
		if (ret != SECLINK_OK) {
			ret = MBEDTLS_ERR_DHM_FILE_IO_ERROR;
			goto cleanup;
		}

		ret = sl_dh_generate_param(shnd, ctx->key_index, &d_param);
		if (ret != SECLINK_OK) {
			sl_deinit(shnd);
			ret = MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
			goto cleanup;
		}

		/*
		 *  3. Export GX from unsigned binary data
		 */
		MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&ctx->GX, d_param.pubkey->data, d_param.pubkey->data_len));

		if ((ret = dhm_check_range(&ctx->GX, &ctx->P)) != 0) {
			sl_deinit(shnd);
			goto cleanup;
		}

		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->GX, output, olen));
		sl_deinit(shnd);
	} else {
		return MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
	}

cleanup:
	if (d_param.P->data) {
		free(d_param.P->data);
	}
	if (d_param.G->data) {
		free(d_param.G->data);
	}
	if (d_param.pubkey->data) {
		free(d_param.pubkey->data);
	}

	if (ret != 0) {
		return (MBEDTLS_ERR_DHM_MAKE_PUBLIC_FAILED + ret);
	}

	return 0;

}


/*
 * Derive and export the shared secret (G^Y)^X mod P
 */
int mbedtls_dhm_calc_secret(mbedtls_dhm_context *ctx,
							unsigned char *output, size_t output_size, size_t *olen,
							int (*f_rng)(void *, unsigned char *, size_t),
							void *p_rng)
{
	int ret = 0;
	unsigned int moduler = 0;
	unsigned int generator = 0;
	unsigned int pubkey = 0;
	hal_dh_data d_param;
	hal_data shared_secret = {output, output_size, NULL, 0};
	sl_ctx shnd;

	if (ctx == NULL || output_size < mbedtls_dhm_get_len(ctx)) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	memset(&d_param, 0, sizeof(hal_dh_data));

	if (mbedtls_supported_dhm_size_alt(mbedtls_dhm_get_len(ctx))) {
		/*
		 *  1. Initialize G, P, GX context.
		 */
		moduler = mbedtls_mpi_size(&ctx->P);
		generator = mbedtls_mpi_size(&ctx->G);
		pubkey = mbedtls_mpi_size(&ctx->GY);

		d_param.P->data_len = moduler;
		d_param.P->data = (unsigned char *)malloc(moduler);
		if (d_param.P->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}
		d_param.G->data_len = generator;
		d_param.G->data = (unsigned char *)malloc(generator);
		if (d_param.G->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}
		d_param.pubkey->data_len = pubkey;
		d_param.pubkey->data = (unsigned char *)malloc(pubkey);
		if (d_param.pubkey->data == NULL) {
			ret = MBEDTLS_ERR_DHM_ALLOC_FAILED;
			goto cleanup;
		}

		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->P, d_param.P->data, moduler));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->G, d_param.G->data, generator));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&ctx->GY, d_param.pubkey->data, pubkey));

		switch (mbedtls_dhm_get_len(ctx)) {
		case DHM_1024:
			d_param.mode = HAL_DH_1024;
			break;
		case DHM_2048:
			d_param.mode = HAL_DH_2048;
			break;
		case DHM_4096:
			d_param.mode = HAL_DH_4096;
			break;
		default:
			ret = MBEDTLS_ERR_DHM_INVALID_FORMAT;
			goto cleanup;
		}

		/*
		 *  2. Calculate shared secret(K) from sss.
		 */
		ret = sl_init(&shnd);
		if (ret != SECLINK_OK) {
			ret = MBEDTLS_ERR_DHM_FILE_IO_ERROR;
			goto cleanup;
		}

		ret = sl_dh_compute_shared_secret(shnd, &d_param, ctx->key_index, &shared_secret);
		if (ret != SECLINK_OK) {
			sl_deinit(shnd);
			ret = MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
			goto cleanup;
		}

		/*
		 *  3. Export K
		 */
		*olen = shared_secret.data_len;

		MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&ctx->K, output, *olen));

		sl_deinit(shnd);
	} else {
		return MBEDTLS_ERR_DHM_HW_ACCEL_FAILED;
	}
cleanup:
	if (d_param.P->data) {
		free(d_param.P->data);
	}
	if (d_param.G->data) {
		free(d_param.G->data);
	}
	if (d_param.pubkey->data) {
		free(d_param.pubkey->data);
	}

	if (ret != 0) {
		return (MBEDTLS_ERR_DHM_MAKE_PUBLIC_FAILED + ret);
	}

	return 0;

}

#endif							/* MBEDTLS_DHM_ALT & CONFIG_HW_DH_PARAM */

#endif							/* MBEDTLS_DHM_C */
