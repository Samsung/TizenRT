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

/*
 *  Public Key abstraction layer: wrapper functions
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

#include "mbedtls/config.h"
#if defined(MBEDTLS_PK_C)
#include "mbedtls/pk_internal.h"

/* Even if RSA not activated, for the sake of RSA-alt */
#include "mbedtls/rsa.h"

#include <string.h>

#if defined(MBEDTLS_ECP_C)
#include "mbedtls/ecp.h"
#endif

#if defined(MBEDTLS_ECDSA_C)
#include "mbedtls/ecdsa.h"
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#if defined(CONFIG_TLS_WITH_SSS)
#include "mbedtls/see_api.h"
#include "mbedtls/see_internal.h"

#include "mbedtls/asn1.h"
#endif

#if defined(MBEDTLS_PK_RSA_ALT_SUPPORT)
/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void *v, size_t n)
{
	volatile unsigned char *p = v;
	while (n--) {
		*p++ = 0;
	}
}
#endif

#if defined(MBEDTLS_RSA_C)
static int rsa_can_do(mbedtls_pk_type_t type)
{
	return (type == MBEDTLS_PK_RSA || type == MBEDTLS_PK_RSASSA_PSS);
}

static size_t rsa_get_bitlen(const void *ctx)
{
	return (8 * ((const mbedtls_rsa_context *)ctx)->len);
}

static int rsa_verify_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, const unsigned char *sig, size_t sig_len)
{
	int ret;

	if (sig_len < ((mbedtls_rsa_context *)ctx)->len) {
		return (MBEDTLS_ERR_RSA_VERIFY_FAILED);
	}

	ret = mbedtls_rsa_pkcs1_verify((mbedtls_rsa_context *)ctx, NULL, NULL, MBEDTLS_RSA_PUBLIC, md_alg, (unsigned int)hash_len, hash, sig);
	if (ret != 0) {
		return (ret);
	}

	if (sig_len > ((mbedtls_rsa_context *)ctx)->len) {
		return (MBEDTLS_ERR_PK_SIG_LEN_MISMATCH);
	}

	return (0);
}

static int rsa_sign_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, unsigned char *sig, size_t *sig_len, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	
#if defined(CONFIG_TLS_WITH_SSS) && defined(CONFIG_SUPPORT_FULL_SECURITY)
	if (!see_check_keyindex(((mbedtls_rsa_context *)ctx)->key_index)) {
		unsigned int key_index = ((mbedtls_rsa_context *)ctx)->key_index;

		ret = hw_rsa_sign_wrap((mbedtls_rsa_context *)ctx, md_alg, hash, hash_len, sig, sig_len, key_index);
	} else
#endif
	{
		*sig_len = ((mbedtls_rsa_context *)ctx)->len;

		ret = mbedtls_rsa_pkcs1_sign((mbedtls_rsa_context *)ctx, f_rng, p_rng, MBEDTLS_RSA_PRIVATE, md_alg, (unsigned int)hash_len, hash, sig);
	}

	return ret;
}

static int rsa_decrypt_wrap(void *ctx, const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen, size_t osize, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	
	if (ilen != ((mbedtls_rsa_context *)ctx)->len) {
		return (MBEDTLS_ERR_RSA_BAD_INPUT_DATA);
	}
#if defined(CONFIG_TLS_WITH_SSS) && defined(CONFIG_SUPPORT_FULL_SECURITY)
	if (!see_check_keyindex(((mbedtls_rsa_context *)ctx)->key_index)) {
		unsigned int key_index = ((mbedtls_rsa_context *)ctx)->key_index;

		ret = hw_rsa_decrypt_wrap((mbedtls_rsa_context *)ctx, input, ilen, output, olen, osize, key_index);
	} else
#endif
	{
		ret = mbedtls_rsa_pkcs1_decrypt((mbedtls_rsa_context *)ctx, f_rng, p_rng, MBEDTLS_RSA_PRIVATE, olen, input, output, osize);
	}

	return ret;
}

static int rsa_encrypt_wrap(void *ctx, const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen, size_t osize, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	*olen = ((mbedtls_rsa_context *)ctx)->len;

	if (*olen > osize) {
		return (MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE);
	}

	return mbedtls_rsa_pkcs1_encrypt((mbedtls_rsa_context *)ctx, f_rng, p_rng, MBEDTLS_RSA_PUBLIC, ilen, input, output);
}

static int rsa_check_pair_wrap(const void *pub, const void *prv)
{
	return mbedtls_rsa_check_pub_priv((const mbedtls_rsa_context *)pub, (const mbedtls_rsa_context *)prv);
}

static void *rsa_alloc_wrap(void)
{
	void *ctx = mbedtls_calloc(1, sizeof(mbedtls_rsa_context));

	if (ctx != NULL) {
		mbedtls_rsa_init((mbedtls_rsa_context *)ctx, 0, 0);
	}

	return (ctx);
}

static void rsa_free_wrap(void *ctx)
{
	mbedtls_rsa_free((mbedtls_rsa_context *)ctx);
	mbedtls_free(ctx);
}

static void rsa_debug(const void *ctx, mbedtls_pk_debug_item *items)
{
	items->type = MBEDTLS_PK_DEBUG_MPI;
	items->name = "rsa.N";
	items->value = &(((mbedtls_rsa_context *)ctx)->N);

	items++;

	items->type = MBEDTLS_PK_DEBUG_MPI;
	items->name = "rsa.E";
	items->value = &(((mbedtls_rsa_context *)ctx)->E);
}

const mbedtls_pk_info_t mbedtls_rsa_info = {
	MBEDTLS_PK_RSA,
	"RSA",
	rsa_get_bitlen,
	rsa_can_do,
#if defined(CONFIG_HW_RSA_VERIFICATION)
	hw_rsa_verify_wrap,
#else	
	rsa_verify_wrap,
#endif
	rsa_sign_wrap,
	rsa_decrypt_wrap,
#if defined(CONFIG_HW_RSA_ENC)
	hw_rsa_encrypt_wrap,
#else
	rsa_encrypt_wrap,
#endif
	rsa_check_pair_wrap,
	rsa_alloc_wrap,
	rsa_free_wrap,
	rsa_debug,
};
#endif							/* MBEDTLS_RSA_C */

#if defined(MBEDTLS_ECP_C)
/*
 * Generic EC key
 */
static int eckey_can_do(mbedtls_pk_type_t type)
{
	return (type == MBEDTLS_PK_ECKEY || type == MBEDTLS_PK_ECKEY_DH || type == MBEDTLS_PK_ECDSA);
}

static size_t eckey_get_bitlen(const void *ctx)
{
	return (((mbedtls_ecp_keypair *)ctx)->grp.pbits);
}

#if defined(MBEDTLS_ECDSA_C)
/* Forward declarations */
static int ecdsa_verify_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, const unsigned char *sig, size_t sig_len);

static int ecdsa_sign_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, unsigned char *sig, size_t *sig_len, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);

static int eckey_verify_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, const unsigned char *sig, size_t sig_len)
{
	int ret;
	mbedtls_ecdsa_context ecdsa;

	mbedtls_ecdsa_init(&ecdsa);

	if ((ret = mbedtls_ecdsa_from_keypair(&ecdsa, ctx)) == 0) {
		ret = ecdsa_verify_wrap(&ecdsa, md_alg, hash, hash_len, sig, sig_len);
	}

	mbedtls_ecdsa_free(&ecdsa);

	return (ret);
}

static int eckey_sign_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, unsigned char *sig, size_t *sig_len, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	mbedtls_ecdsa_context ecdsa;

	mbedtls_ecdsa_init(&ecdsa);

	if ((ret = mbedtls_ecdsa_from_keypair(&ecdsa, ctx)) == 0) {
#if defined(CONFIG_TLS_WITH_SSS)
		if (!see_check_keyindex(((mbedtls_ecp_keypair *)ctx)->key_index)) {
			unsigned int key_index = ((mbedtls_ecp_keypair *)ctx)->key_index;
			ret = hw_ecdsa_sign_wrap(&ecdsa, md_alg, hash, hash_len, sig, sig_len, key_index);
		} else
#endif
		{
			ret = ecdsa_sign_wrap(&ecdsa, md_alg, hash, hash_len, sig, sig_len, f_rng, p_rng);
		}
	}

	mbedtls_ecdsa_free(&ecdsa);

	return (ret);
}

#endif							/* MBEDTLS_ECDSA_C */

static int eckey_check_pair(const void *pub, const void *prv)
{
	return (mbedtls_ecp_check_pub_priv((const mbedtls_ecp_keypair *)pub, (const mbedtls_ecp_keypair *)prv));
}

static void *eckey_alloc_wrap(void)
{
	void *ctx = mbedtls_calloc(1, sizeof(mbedtls_ecp_keypair));

	if (ctx != NULL) {
		mbedtls_ecp_keypair_init(ctx);
	}

	return (ctx);
}

static void eckey_free_wrap(void *ctx)
{
	mbedtls_ecp_keypair_free((mbedtls_ecp_keypair *)ctx);
	mbedtls_free(ctx);
}

static void eckey_debug(const void *ctx, mbedtls_pk_debug_item *items)
{
	items->type = MBEDTLS_PK_DEBUG_ECP;
	items->name = "eckey.Q";
	items->value = &(((mbedtls_ecp_keypair *)ctx)->Q);
}

const mbedtls_pk_info_t mbedtls_eckey_info = {
	MBEDTLS_PK_ECKEY,
	"EC",
	eckey_get_bitlen,
	eckey_can_do,
#if defined(MBEDTLS_ECDSA_C)
#if defined(CONFIG_HW_ECDSA_VERIFICATION)
hw_eckey_verify_wrap,
#else
eckey_verify_wrap,
#endif
	eckey_sign_wrap,
#else
	NULL,
	NULL,
#endif
	NULL,
	NULL,
	eckey_check_pair,
	eckey_alloc_wrap,
	eckey_free_wrap,
	eckey_debug,
};

/*
 * EC key restricted to ECDH
 */
static int eckeydh_can_do(mbedtls_pk_type_t type)
{
	return (type == MBEDTLS_PK_ECKEY || type == MBEDTLS_PK_ECKEY_DH);
}

const mbedtls_pk_info_t mbedtls_eckeydh_info = {
	MBEDTLS_PK_ECKEY_DH,
	"EC_DH",
	eckey_get_bitlen,			/* Same underlying key structure */
	eckeydh_can_do,
	NULL,
	NULL,
	NULL,
	NULL,
	eckey_check_pair,
	eckey_alloc_wrap,			/* Same underlying key structure */
	eckey_free_wrap,			/* Same underlying key structure */
	eckey_debug,				/* Same underlying key structure */
};
#endif							/* MBEDTLS_ECP_C */

#if defined(MBEDTLS_ECDSA_C)
static int ecdsa_can_do(mbedtls_pk_type_t type)
{
	return (type == MBEDTLS_PK_ECDSA);
}

static int ecdsa_verify_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, const unsigned char *sig, size_t sig_len)
{
	int ret;
	((void)md_alg);

	ret = mbedtls_ecdsa_read_signature((mbedtls_ecdsa_context *)ctx, hash, hash_len, sig, sig_len);

	if (ret == MBEDTLS_ERR_ECP_SIG_LEN_MISMATCH) {
		return (MBEDTLS_ERR_PK_SIG_LEN_MISMATCH);
	}

	return (ret);
}

static int ecdsa_sign_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, unsigned char *sig, size_t *sig_len, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	return mbedtls_ecdsa_write_signature((mbedtls_ecdsa_context *)ctx, md_alg, hash, hash_len, sig, sig_len, f_rng, p_rng);
}

static void *ecdsa_alloc_wrap(void)
{
	void *ctx = mbedtls_calloc(1, sizeof(mbedtls_ecdsa_context));

	if (ctx != NULL) {
		mbedtls_ecdsa_init((mbedtls_ecdsa_context *)ctx);
	}

	return (ctx);
}

static void ecdsa_free_wrap(void *ctx)
{
	mbedtls_ecdsa_free((mbedtls_ecdsa_context *)ctx);
	mbedtls_free(ctx);
}

const mbedtls_pk_info_t mbedtls_ecdsa_info = {
	MBEDTLS_PK_ECDSA,
	"ECDSA",
	eckey_get_bitlen,			/* Compatible key structures */
	ecdsa_can_do,
	ecdsa_verify_wrap,
	ecdsa_sign_wrap,
	NULL,
	NULL,
	eckey_check_pair,			/* Compatible key structures */
	ecdsa_alloc_wrap,
	ecdsa_free_wrap,
	eckey_debug,				/* Compatible key structures */
};
#endif							/* MBEDTLS_ECDSA_C */

#if defined(MBEDTLS_PK_RSA_ALT_SUPPORT)
/*
 * Support for alternative RSA-private implementations
 */

static int rsa_alt_can_do(mbedtls_pk_type_t type)
{
	return (type == MBEDTLS_PK_RSA);
}

static size_t rsa_alt_get_bitlen(const void *ctx)
{
	const mbedtls_rsa_alt_context *rsa_alt = (const mbedtls_rsa_alt_context *)ctx;

	return (8 * rsa_alt->key_len_func(rsa_alt->key));
}

static int rsa_alt_sign_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, unsigned char *sig, size_t *sig_len, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	mbedtls_rsa_alt_context *rsa_alt = (mbedtls_rsa_alt_context *)ctx;

	*sig_len = rsa_alt->key_len_func(rsa_alt->key);

	return (rsa_alt->sign_func(rsa_alt->key, f_rng, p_rng, MBEDTLS_RSA_PRIVATE, md_alg, (unsigned int)hash_len, hash, sig));
}

static int rsa_alt_decrypt_wrap(void *ctx, const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen, size_t osize, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	mbedtls_rsa_alt_context *rsa_alt = (mbedtls_rsa_alt_context *)ctx;

	((void)f_rng);
	((void)p_rng);

	if (ilen != rsa_alt->key_len_func(rsa_alt->key)) {
		return (MBEDTLS_ERR_RSA_BAD_INPUT_DATA);
	}

	return (rsa_alt->decrypt_func(rsa_alt->key, MBEDTLS_RSA_PRIVATE, olen, input, output, osize));
}

#if defined(MBEDTLS_RSA_C)
static int rsa_alt_check_pair(const void *pub, const void *prv)
{
	unsigned char sig[MBEDTLS_MPI_MAX_SIZE];
	unsigned char hash[32];
	size_t sig_len = 0;
	int ret;

	if (rsa_alt_get_bitlen(prv) != rsa_get_bitlen(pub)) {
		return (MBEDTLS_ERR_RSA_KEY_CHECK_FAILED);
	}

	memset(hash, 0x2a, sizeof(hash));

	if ((ret = rsa_alt_sign_wrap((void *)prv, MBEDTLS_MD_NONE, hash, sizeof(hash), sig, &sig_len, NULL, NULL)) != 0) {
		return (ret);
	}

	if (rsa_verify_wrap((void *)pub, MBEDTLS_MD_NONE, hash, sizeof(hash), sig, sig_len) != 0) {
		return (MBEDTLS_ERR_RSA_KEY_CHECK_FAILED);
	}

	return (0);
}
#endif							/* MBEDTLS_RSA_C */

static void *rsa_alt_alloc_wrap(void)
{
	void *ctx = mbedtls_calloc(1, sizeof(mbedtls_rsa_alt_context));

	if (ctx != NULL) {
		memset(ctx, 0, sizeof(mbedtls_rsa_alt_context));
	}

	return (ctx);
}

static void rsa_alt_free_wrap(void *ctx)
{
	mbedtls_zeroize(ctx, sizeof(mbedtls_rsa_alt_context));
	mbedtls_free(ctx);
}

const mbedtls_pk_info_t mbedtls_rsa_alt_info = {
	MBEDTLS_PK_RSA_ALT,
	"RSA-alt",
	rsa_alt_get_bitlen,
	rsa_alt_can_do,
	NULL,
	rsa_alt_sign_wrap,
	rsa_alt_decrypt_wrap,
	NULL,
#if defined(MBEDTLS_RSA_C)
	rsa_alt_check_pair,
#else
	NULL,
#endif
	rsa_alt_alloc_wrap,
	rsa_alt_free_wrap,
	NULL,
};

#endif							/* MBEDTLS_PK_RSA_ALT_SUPPORT */

#if defined(CONFIG_TLS_WITH_SSS)
int hw_ecdsa_sign_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, unsigned char *sig, size_t *sig_len, unsigned int key_index)
{
	int ret;
	unsigned int curve = ((mbedtls_ecdsa_context *)ctx)->grp.id;
	unsigned char s_buf[SEE_MAX_ECP_KEY_SIZE];
	unsigned char r_buf[SEE_MAX_ECP_KEY_SIZE];
	unsigned char *t_hash = (unsigned char *)hash;

	/*
	 * 1. Check hash algorithm and sign curve
	 */
	mbedtls_mpi r, s;
	struct sECC_SIGN ecc_sign;
	memset(&ecc_sign, 0, sizeof(struct sECC_SIGN));

	ecc_sign.s = s_buf;
	ecc_sign.r = r_buf;

	switch (md_alg) {
	case MBEDTLS_MD_SHA1:
		ecc_sign.sign_type |= OID_SHA1_160;
		break;
	case MBEDTLS_MD_SHA256:
		ecc_sign.sign_type |= OID_SHA2_256;
		break;
	case MBEDTLS_MD_SHA384:
		ecc_sign.sign_type |= OID_SHA2_384;
		break;
	case MBEDTLS_MD_SHA512:
		ecc_sign.sign_type |= OID_SHA2_512;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}
	switch (curve) {
	case MBEDTLS_ECP_DP_SECP192R1:
		ecc_sign.sign_type |= OID_ECC_P192;
		break;
	case MBEDTLS_ECP_DP_SECP224R1:
		ecc_sign.sign_type |= OID_ECC_P224;
		break;
	case MBEDTLS_ECP_DP_SECP256R1:
		ecc_sign.sign_type |= OID_ECC_P256;
		break;
	case MBEDTLS_ECP_DP_SECP384R1:
		ecc_sign.sign_type |= OID_ECC_P384;
		break;
	case MBEDTLS_ECP_DP_SECP521R1:
		ecc_sign.sign_type |= OID_ECC_P521;
		break;
	case MBEDTLS_ECP_DP_BP256R1:
		ecc_sign.sign_type |= OID_ECC_BP256;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
		goto cleanup;
	}

	mbedtls_mpi_init(&r);
	mbedtls_mpi_init(&s);

	/*
	 * 2. Sign
	 */
	if ((ret = see_get_ecdsa_signature(&ecc_sign, t_hash, hash_len, key_index)) != 0) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	mbedtls_mpi_read_binary(&r, ecc_sign.r, ecc_sign.r_byte_len);
	mbedtls_mpi_read_binary(&s, ecc_sign.s, ecc_sign.s_byte_len);

	MBEDTLS_MPI_CHK(ecdsa_signature_to_asn1(&r, &s, sig, sig_len));

cleanup:
	mbedtls_mpi_free(&r);
	mbedtls_mpi_free(&s);

	return ret;
}

#if defined(CONFIG_SUPPORT_FULL_SECURITY)
int hw_rsa_sign_wrap(mbedtls_rsa_context *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, unsigned char *sig, size_t *sig_len, unsigned int key_index)
{
	int ret;
	unsigned int padding = ctx->padding;
	struct sRSA_SIGN rsa_sign;
	unsigned char *t_hash = (unsigned char *)hash;

	memset(&rsa_sign, 0, sizeof(struct sRSA_SIGN));

	switch (md_alg) {
	case MBEDTLS_MD_NONE:
		ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		goto sign_exit;
	case MBEDTLS_MD_SHA1:
		rsa_sign.alg_type = SHA1_160 | (padding ^ 0x1);	/* 0x110X */
		break;
	case MBEDTLS_MD_SHA224:
		rsa_sign.alg_type = SHA2_224 | (padding ^ 0x1);	/* 0x220X */
		break;
	case MBEDTLS_MD_SHA256:
		rsa_sign.alg_type = SHA2_256 | (padding ^ 0x1);	/* 0x230X */
		break;
	case MBEDTLS_MD_SHA384:
		rsa_sign.alg_type = SHA2_384 | (padding ^ 0x1);	/* 0x240X */
		break;
	case MBEDTLS_MD_SHA512:
		rsa_sign.alg_type = SHA2_512 | (padding ^ 0x1);	/* 0x250X */
		break;
	default:
		ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		goto sign_exit;
	}

	rsa_sign.signature = sig;

	switch (padding) {
#if defined(MBEDTLS_PKCS1_V15)
	case MBEDTLS_RSA_PKCS_V15:	/* PKCS */
		ret = see_get_rsa_signature(&rsa_sign, t_hash, hash_len, key_index);
		break;
#endif
#if defined(MBEDTLS_PKCS1_V21)
	case MBEDTLS_RSA_PKCS_V21:	/* PSS */
		ret = see_get_rsa_signature(&rsa_sign, t_hash, hash_len, key_index);
		break;
#endif
	default:
		ret = MBEDTLS_ERR_RSA_INVALID_PADDING;
		break;
	}
	*sig_len = rsa_sign.signature_byte_len;

sign_exit:
	return ret;
}

int hw_rsa_decrypt_wrap(mbedtls_rsa_context *ctx, const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen, size_t osize, unsigned int key_index)
{
	int ret;
	unsigned int padding = ctx->padding;
	unsigned char *t_input = (unsigned char *)input;

	ret = see_rsa_decryption(key_index, padding, output, olen, t_input, ilen);

	return ret;
}
#endif							/* CONFIG_SUPPORT_FULL_SECURITY */
#endif							/* CONFIG_TLS_WITH_SSS */

#if defined(CONFIG_HW_ECDSA_VERIFICATION)
int hw_eckey_verify_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, const unsigned char *sig, size_t sig_len)
{
	int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	unsigned char *der_buf = NULL;
	unsigned char *key_buf = NULL;
	const unsigned char *end = sig + sig_len;
	unsigned char *p = (unsigned char *)sig;
	unsigned char *t_hash = (unsigned char *)hash;
	struct sECC_SIGN ecc_sign;

	((void)md_alg);

	if (ctx == NULL) {
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

	/*
	 * 1. Encrypt publickey for using SSS accelator.
	 */
	mbedtls_pk_context t_pk;
	unsigned int len, curve = ((mbedtls_ecdsa_context *)ctx)->grp.id, der_buflen = 1024;

	t_pk.pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
	t_pk.pk_ctx = (mbedtls_ecdsa_context *)ctx;

	der_buf = (unsigned char *)malloc(der_buflen);

	if (der_buf == NULL) {
		return MBEDTLS_ERR_ECP_ALLOC_FAILED;
	}

	if ((len = mbedtls_pk_write_pubkey_der(&t_pk, der_buf, der_buflen)) == 0) {
		ret = MBEDTLS_ERR_ECP_INVALID_KEY;
		goto cleanup;
	}

	key_buf = (unsigned char *)malloc(SEE_MAX_ENCRYPTED_KEY_SIZE);

	if (key_buf == NULL) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	if ((ret = see_setup_key_internal(der_buf + der_buflen - len, len, SECURE_STORAGE_TYPE_KEY_ECC, key_buf)) != 0) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	/*
	 * 2. Seperate 'r' and 's' from received signature.
	 */
	memset(&ecc_sign, 0, sizeof(struct sECC_SIGN));

	if (mbedtls_asn1_get_tag(&p, end, &len, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	if (p + len != end) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA + MBEDTLS_ERR_ASN1_LENGTH_MISMATCH;
		goto cleanup;
	}

	unsigned int n, tmp = 0;

	/*
	 * 2-1 Get 'r' value
	 */
	if (mbedtls_asn1_get_tag(&p, end, &tmp, MBEDTLS_ASN1_INTEGER)) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	for (n = 0; n < tmp; n++) {
		if (p[n] != 0) {
			break;
		}
	}

	ecc_sign.r = p + n;
	ecc_sign.r_byte_len = tmp - n;
	p += tmp;

	/*
	 * 2-2 Get 's' value
	 */
	if (mbedtls_asn1_get_tag(&p, end, &tmp, MBEDTLS_ASN1_INTEGER)) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	for (n = 0; n < tmp; n++) {
		if (p[n] != 0) {
			break;
		}
	}

	ecc_sign.s = p + n;
	ecc_sign.s_byte_len = tmp - n;
	p += tmp;

	/*
	 * 3. Choose digest algorithm and curve type.
	 */
	switch (md_alg) {
	case MBEDTLS_MD_SHA1:
		ecc_sign.sign_type |= OID_SHA1_160;
		break;
	case MBEDTLS_MD_SHA256:
		ecc_sign.sign_type |= OID_SHA2_256;
		break;
	case MBEDTLS_MD_SHA384:
		ecc_sign.sign_type |= OID_SHA2_384;
		break;
	case MBEDTLS_MD_SHA512:
		ecc_sign.sign_type |= OID_SHA2_512;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	switch (curve) {
	case MBEDTLS_ECP_DP_SECP192R1:
		ecc_sign.sign_type |= OID_ECC_P192;
		break;
	case MBEDTLS_ECP_DP_SECP224R1:
		ecc_sign.sign_type |= OID_ECC_P224;
		break;
	case MBEDTLS_ECP_DP_SECP256R1:
		ecc_sign.sign_type |= OID_ECC_P256;
		break;
	case MBEDTLS_ECP_DP_SECP384R1:
		ecc_sign.sign_type |= OID_ECC_P384;
		break;
	case MBEDTLS_ECP_DP_SECP521R1:
		ecc_sign.sign_type |= OID_ECC_P521;
		break;
	case MBEDTLS_ECP_DP_BP256R1:
		ecc_sign.sign_type |= OID_ECC_BP256;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	/*
	 *  4. Verify signature with public key.
	 */
	ret = see_verify_ecdsa_signature_internal(&ecc_sign, t_hash, hash_len, key_buf);

cleanup:
	free(der_buf);

	if (key_buf) {
		free(key_buf);
	}

	return ret;
}
#endif							/* CONFIG_HW_ECDSA_VERIFICATION */

#if defined(CONFIG_HW_RSA_VERIFICATION)
int hw_rsa_verify_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, const unsigned char *sig, size_t sig_len)
{
	int ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	unsigned char *key_buf = NULL;
	unsigned char *der_buf = NULL;
	unsigned char *t_hash = (unsigned char *)hash;
	struct sRSA_SIGN rsa_sign;
	unsigned int padding = ((mbedtls_rsa_context *)ctx)->padding;

	((void)md_alg);

	if (ctx == NULL) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	/*
	 * 1. Encrypt publickey for using SSS accelator.
	 */
	mbedtls_pk_context t_pk;
	unsigned int len, der_buflen = 2048;

	t_pk.pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_RSA);
	t_pk.pk_ctx = (mbedtls_rsa_context *)ctx;

	der_buf = (unsigned char *)malloc(der_buflen);

	if (der_buf == NULL) {
		return MBEDTLS_ERR_PK_ALLOC_FAILED;
	}

	len = mbedtls_pk_write_pubkey_der(&t_pk, der_buf, der_buflen);
	if (len < 0) {
		ret = len;
		goto cleanup;
	}

	key_buf = (unsigned char *)malloc(SEE_MAX_ENCRYPTED_KEY_SIZE);

	if (key_buf == NULL) {
		ret = MBEDTLS_ERR_PK_ALLOC_FAILED;
		goto cleanup;
	}

	ret = see_setup_key_internal(der_buf + der_buflen - len, len, SECURE_STORAGE_TYPE_KEY_RSA, key_buf);
	if (ret != 0) {
		goto cleanup;
	}

	memset(&rsa_sign, 0, sizeof(struct sRSA_SIGN));

	/*
	 * 2. Choose digest algorithm.
	 */
	switch (md_alg) {
	case MBEDTLS_MD_NONE:		/* NOT SUPPORTED IN SSS */
		ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		goto cleanup;
	case MBEDTLS_MD_SHA1:
		rsa_sign.alg_type = SHA1_160 | (padding ^ 0x1);	/* 0x110X */
		break;
	case MBEDTLS_MD_SHA224:
		rsa_sign.alg_type = SHA2_224 | (padding ^ 0x1);	/* 0x220X */
		break;
	case MBEDTLS_MD_SHA256:
		rsa_sign.alg_type = SHA2_256 | (padding ^ 0x1);	/* 0x230X */
		break;
	case MBEDTLS_MD_SHA384:
		rsa_sign.alg_type = SHA2_384 | (padding ^ 0x1);	/* 0x240X */
		break;
	case MBEDTLS_MD_SHA512:
		rsa_sign.alg_type = SHA2_512 | (padding ^ 0x1);	/* 0x250X */
		break;
	default:
		ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		goto cleanup;
	}

	rsa_sign.signature = (unsigned char *)sig;
	rsa_sign.signature_byte_len = sig_len;

	/*
	 *  3. Verify signature with public key.
	 */
	switch (padding) {
#if defined(MBEDTLS_PKCS1_V15)
	case MBEDTLS_RSA_PKCS_V15:	/* PKCS */
		ret = see_verify_rsa_signature_internal(&rsa_sign, t_hash, hash_len, key_buf);
		break;
#endif
#if defined(MBEDTLS_PKCS1_V21)
	case MBEDTLS_RSA_PKCS_V21:	/* PSS */
		ret = see_verify_rsa_signature_internal(&rsa_sign, t_hash, hash_len, key_buf);
		break;
#endif
	default:
		ret = MBEDTLS_ERR_RSA_INVALID_PADDING;
	}

cleanup:
	if (der_buf) {
		free(der_buf);
	}

	if (key_buf) {
		free(key_buf);
	}

	return ret;
}
#endif							/* CONFIG_HW_RSA_VERIFICATION */

#if defined(CONFIG_HW_RSA_ENC)
int hw_rsa_encrypt_wrap(void *ctx, const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen, size_t osize, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	unsigned char *key_buf = NULL;
	unsigned char *der_buf = NULL;
	unsigned int padding = ((mbedtls_rsa_context *)ctx)->padding;
	unsigned char *t_input = (unsigned char *)input;

	if (ctx == NULL) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	/*
	 * 1. Encrypt publickey for using SSS accelator.
	 */
	mbedtls_pk_context t_pk;
	unsigned int len, der_buflen = 1024;

	t_pk.pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_RSA);
	t_pk.pk_ctx = (mbedtls_rsa_context *)ctx;

	der_buf = (unsigned char *)malloc(der_buflen);

	if (der_buf == NULL) {
		return MBEDTLS_ERR_PK_ALLOC_FAILED;
	}

	len = mbedtls_pk_write_pubkey_der(&t_pk, der_buf, der_buflen);
	if (len < 0) {
		ret = len;
		goto cleanup;
	}

	key_buf = (unsigned char *)malloc(SEE_MAX_ENCRYPTED_KEY_SIZE);

	if (key_buf == NULL) {
		ret = MBEDTLS_ERR_PK_ALLOC_FAILED;
		goto cleanup;
	}

	ret = see_setup_key_internal(der_buf + der_buflen - len, len, SECURE_STORAGE_TYPE_KEY_RSA, key_buf);
	if (ret  != 0) {
		goto cleanup;
	}

	/*
	 *  2. Encrypt data with public key.
	 */
#if defined(MBEDTLS_PKCS1_V15) || defined(MBEDTLS_PKCS1_V21)
	ret = see_rsa_encryption_internal(key_buf, padding, output, olen, t_input, ilen);
#endif

cleanup:
	if (der_buf) {
		free(der_buf);
	}

	if (key_buf) {
		free(key_buf);
	}

	return ret;
}
#endif							/* CONFIG_HW_RSA_ENC */
#endif							/* MBEDTLS_PK_C */
