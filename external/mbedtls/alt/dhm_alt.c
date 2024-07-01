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

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#include <string.h>

#if defined(MBEDTLS_PEM_PARSE_C)
#include "mbedtls/pem.h"
#endif

#if defined(MBEDTLS_ASN1_PARSE_C)
#include "mbedtls/asn1.h"
#endif


#include "mbedtls/alt/common.h"

#if defined(MBEDTLS_DHM_ALT)
#include "mbedtls/alt/dhm_alt.h"

#define DHM_MPI_EXPORT( X, n )                                          \
    do {                                                                \
        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( ( X ),               \
                                                   p + 2,               \
                                                   ( n ) ) );           \
        *p++ = (unsigned char)( ( n ) >> 8 );                           \
        *p++ = (unsigned char)( ( n )      );                           \
        p += ( n );                                                     \
    } while( 0 )

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

/*
 * helper to validate the mbedtls_mpi size and import it
 */
static int dhm_read_bignum(mbedtls_mpi *X, unsigned char **p, const unsigned char *end)
{
	int ret, n;

	if (end - *p < 2) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	n = ((*p)[0] << 8) | (*p)[1];
	(*p) += 2;

	if ((int)(end - *p) < n) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	if ((ret = mbedtls_mpi_read_binary(X, *p, n)) != 0) {
        return MBEDTLS_ERROR_ADD(MBEDTLS_ERR_DHM_READ_PARAMS_FAILED, ret);
	}

	(*p) += n;

	return 0;
}

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

void mbedtls_dhm_init(mbedtls_dhm_context *ctx)
{
	memset(ctx, 0, sizeof(mbedtls_dhm_context));
	ctx->key_index = MBEDTLS_DHM_KEY_INDEX_ALT;
	return;
}

size_t mbedtls_dhm_get_bitlen(const mbedtls_dhm_context *ctx)
{
	return mbedtls_mpi_bitlen(&ctx->P);
}

size_t mbedtls_dhm_get_len(const mbedtls_dhm_context *ctx)
{
	return mbedtls_mpi_size(&ctx->P);
}

int mbedtls_dhm_get_value(const mbedtls_dhm_context *ctx,
						mbedtls_dhm_parameter param,
						mbedtls_mpi *dest)
{
	const mbedtls_mpi *src = NULL;
	switch (param) {
		case MBEDTLS_DHM_PARAM_P:
			src = &ctx->P;
			break;
		case MBEDTLS_DHM_PARAM_G:
			src = &ctx->G;
			break;
		case MBEDTLS_DHM_PARAM_X:
			src = &ctx->X;
			break;
		case MBEDTLS_DHM_PARAM_GX:
			src = &ctx->GX;
			break;
		case MBEDTLS_DHM_PARAM_GY:
			src = &ctx->GY;
			break;
		case MBEDTLS_DHM_PARAM_K:
			src = &ctx->K;
			break;
		default:
			return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}
	return mbedtls_mpi_copy(dest, src);
}

/*
 * Parse the ServerKeyExchange parameters
 */
int mbedtls_dhm_read_params(mbedtls_dhm_context *ctx, unsigned char **p, const unsigned char *end)
{
	int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

	if ((ret = dhm_read_bignum(&ctx->P,  p, end)) != 0 ||
		(ret = dhm_read_bignum(&ctx->G,  p, end)) != 0 ||
		(ret = dhm_read_bignum(&ctx->GY, p, end)) != 0) {
		return ret;
	}

	if ((ret = dhm_check_range(&ctx->GY, &ctx->P)) != 0) {
		return ret;
	}

	return 0;
}

/*
 * Setup and write the ServerKeyExchange parameters
 */
int mbedtls_dhm_make_params(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
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
 * Set prime modulus and generator
 */
int mbedtls_dhm_set_group(mbedtls_dhm_context *ctx, const mbedtls_mpi *P, const mbedtls_mpi *G)
{
	int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

	if ((ret = mbedtls_mpi_copy(&ctx->P, P)) != 0 ||
		(ret = mbedtls_mpi_copy(&ctx->G, G)) != 0) {
		return MBEDTLS_ERROR_ADD(MBEDTLS_ERR_DHM_SET_GROUP_FAILED, ret);
	}

	return 0;
}

/*
 * Import the peer's public value G^Y
 */
int mbedtls_dhm_read_public(mbedtls_dhm_context *ctx, const unsigned char *input, size_t ilen)
{
	int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

	if (ilen < 1 || ilen > mbedtls_dhm_get_len(ctx)) {
		return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
	}

	if ((ret = mbedtls_mpi_read_binary(&ctx->GY, input, ilen)) != 0) {
		return MBEDTLS_ERROR_ADD(MBEDTLS_ERR_DHM_READ_PUBLIC_FAILED, ret);
	}

	return 0;
}

/*
 * Create own private value X and export G^X
 */
int mbedtls_dhm_make_public(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
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
 * Use the blinding method and optimisation suggested in section 10 of:
 *  KOCHER, Paul C. Timing attacks on implementations of Diffie-Hellman, RSA,
 *  DSS, and other systems. In : Advances in Cryptology-CRYPTO'96. Springer
 *  Berlin Heidelberg, 1996. p. 104-113.
 */
static int dhm_update_blinding(mbedtls_dhm_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret, count;

	/*
	 * Don't use any blinding the first time a particular X is used,
	 * but remember it to use blinding next time.
	 */
	if (mbedtls_mpi_cmp_mpi(&ctx->X, &ctx->pX) != 0) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&ctx->pX, &ctx->X));
		MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&ctx->Vi, 1));
		MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&ctx->Vf, 1));

		return 0;
	}

	/*
	 * Ok, we need blinding. Can we re-use existing values?
	 * If yes, just update them by squaring them.
	 */
	if (mbedtls_mpi_cmp_int(&ctx->Vi, 1) != 0) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&ctx->Vi, &ctx->Vi, &ctx->Vi));
		MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&ctx->Vi, &ctx->Vi, &ctx->P));

		MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&ctx->Vf, &ctx->Vf, &ctx->Vf));
		MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&ctx->Vf, &ctx->Vf, &ctx->P));

		return 0;
	}

	/*
	 * We need to generate blinding values from scratch
	 */

	/* Vi = random( 2, P-1 ) */
	count = 0;
	do {
		MBEDTLS_MPI_CHK(mbedtls_mpi_fill_random(&ctx->Vi, mbedtls_mpi_size(&ctx->P), f_rng, p_rng));

		while (mbedtls_mpi_cmp_mpi(&ctx->Vi, &ctx->P) >= 0) {
			MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(&ctx->Vi, 1));
		}

		if (count++ > 10) {
			return MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
		}
	} while (mbedtls_mpi_cmp_int(&ctx->Vi, 1) <= 0);

	/* Vf = Vi^-X mod P */
	MBEDTLS_MPI_CHK(mbedtls_mpi_inv_mod(&ctx->Vf, &ctx->Vi, &ctx->P));
	MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&ctx->Vf, &ctx->Vf, &ctx->X, &ctx->P, &ctx->RP));

cleanup:
	return ret;
}

/*
 * Derive and export the shared secret (G^Y)^X mod P
 */
int mbedtls_dhm_calc_secret(mbedtls_dhm_context *ctx, unsigned char *output, size_t output_size, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
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

/*
 * Free the components of a DHM key
 */
void mbedtls_dhm_free(mbedtls_dhm_context *ctx)
{
	if (ctx == NULL) {
		return;
	}

	mbedtls_mpi_free(&ctx->pX);
	mbedtls_mpi_free(&ctx->Vf);
	mbedtls_mpi_free(&ctx->Vi);
	mbedtls_mpi_free(&ctx->RP);
	mbedtls_mpi_free(&ctx->K);
	mbedtls_mpi_free(&ctx->GY);
	mbedtls_mpi_free(&ctx->GX);
	mbedtls_mpi_free(&ctx->X);
	mbedtls_mpi_free(&ctx->G);
	mbedtls_mpi_free(&ctx->P);

	mbedtls_platform_zeroize(ctx, sizeof(mbedtls_dhm_context));
}

#if defined(MBEDTLS_ASN1_PARSE_C)
/*
 * Parse DHM parameters
 */
int mbedtls_dhm_parse_dhm(mbedtls_dhm_context *dhm, const unsigned char *dhmin, size_t dhminlen)
{
	int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
	size_t len;
	unsigned char *p, *end;
#if defined(MBEDTLS_PEM_PARSE_C)
	mbedtls_pem_context pem;
#endif /* MBEDTLS_PEM_PARSE_C */

#if defined(MBEDTLS_PEM_PARSE_C)
	mbedtls_pem_init(&pem);

	/* Avoid calling mbedtls_pem_read_buffer() on non-null-terminated string */
	if (dhminlen == 0 || dhmin[dhminlen - 1] != '\0') {
		ret = MBEDTLS_ERR_PEM_NO_HEADER_FOOTER_PRESENT;
	} else {
		ret = mbedtls_pem_read_buffer(&pem, "-----BEGIN DH PARAMETERS-----", "-----END DH PARAMETERS-----", dhmin, NULL, 0, &dhminlen);
	}

	if (ret == 0) {
		/*
		 * Was PEM encoded
		 */
		dhminlen = pem.buflen;
	} else if (ret != MBEDTLS_ERR_PEM_NO_HEADER_FOOTER_PRESENT) {
		goto exit;
	}

	p = (ret == 0) ? pem.buf : (unsigned char *)dhmin;
#else
	p = (unsigned char *)dhmin;
#endif							/* MBEDTLS_PEM_PARSE_C */
	end = p + dhminlen;

	/*
	 *  DHParams ::= SEQUENCE {
	 *      prime              INTEGER,  -- P
	 *      generator          INTEGER,  -- g
	 *      privateValueLength INTEGER OPTIONAL
	 *  }
	 */
	if ((ret = mbedtls_asn1_get_tag(&p, end, &len, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)) != 0) {
		ret = MBEDTLS_ERROR_ADD(MBEDTLS_ERR_DHM_INVALID_FORMAT, ret);
		goto exit;
	}

	end = p + len;

	if ((ret = mbedtls_asn1_get_mpi(&p, end, &dhm->P)) != 0 || (ret = mbedtls_asn1_get_mpi(&p, end, &dhm->G)) != 0) {
		ret = MBEDTLS_ERROR_ADD(MBEDTLS_ERR_DHM_INVALID_FORMAT, ret);
		goto exit;
	}

	if (p != end) {
		/* This might be the optional privateValueLength.
		 * If so, we can cleanly discard it */
		mbedtls_mpi rec;
		mbedtls_mpi_init(&rec);
		ret = mbedtls_asn1_get_mpi(&p, end, &rec);
		mbedtls_mpi_free(&rec);
		if (ret != 0) {
			ret = MBEDTLS_ERROR_ADD(MBEDTLS_ERR_DHM_INVALID_FORMAT, ret);
			goto exit;
		}
		if (p != end) {
			ret = MBEDTLS_ERROR_ADD(MBEDTLS_ERR_DHM_INVALID_FORMAT, MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
			goto exit;
		}
	}

	ret = 0;

exit:
#if defined(MBEDTLS_PEM_PARSE_C)
	mbedtls_pem_free(&pem);
#endif
	if (ret != 0) {
		mbedtls_dhm_free(dhm);
	}

	return ret;
}

#if defined(MBEDTLS_FS_IO)
/*
 * Load all data from a file into a given buffer.
 *
 * The file is expected to contain either PEM or DER encoded data.
 * A terminating null byte is always appended. It is included in the announced
 * length only if the data looks like it is PEM encoded.
 */
static int load_file(const char *path, unsigned char **buf, size_t *n)
{
	FILE *f;
	long size;

	if ((f = fopen(path, "rb")) == NULL) {
		return MBEDTLS_ERR_DHM_FILE_IO_ERROR;
	}
	/* The data loaded here is public, so don't bother disabling buffering. */

	fseek(f, 0, SEEK_END);
	if ((size = ftell(f)) == -1) {
		fclose(f);
		return MBEDTLS_ERR_DHM_FILE_IO_ERROR;
	}
	fseek(f, 0, SEEK_SET);

	*n = (size_t) size;

	if (*n + 1 == 0 || (*buf = mbedtls_calloc(1, *n + 1)) == NULL) {
		fclose(f);
		return MBEDTLS_ERR_DHM_ALLOC_FAILED;
	}

	if (fread(*buf, 1, *n, f) != *n) {
		fclose(f);

		mbedtls_platform_zeroize(*buf, *n + 1);
		mbedtls_free(*buf);

		return MBEDTLS_ERR_DHM_FILE_IO_ERROR;
	}

	fclose(f);

	(*buf)[*n] = '\0';

	if (strstr((const char *)*buf, "-----BEGIN ") != NULL) {
		++ * n;
	}

	return 0;
}

/*
 * Load and parse DHM parameters
 */
int mbedtls_dhm_parse_dhmfile(mbedtls_dhm_context *dhm, const char *path)
{
	int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
	size_t n;
	unsigned char *buf;

	if ((ret = load_file(path, &buf, &n)) != 0) {
		return ret;
	}

	ret = mbedtls_dhm_parse_dhm(dhm, buf, n);

	mbedtls_platform_zeroize(buf, n);
	mbedtls_free(buf);

	return ret;
}

#endif							/* MBEDTLS_FS_IO */
#endif							/* MBEDTLS_ASN1_PARSE_C */

#endif							/* MBEDTLS_DHM_ALT & CONFIG_HW_DH_PARAM */

#endif							/* MBEDTLS_DHM_C */
