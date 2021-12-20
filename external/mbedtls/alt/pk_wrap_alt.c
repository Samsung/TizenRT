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
 *	Public Key abstraction layer: wrapper functions
 *
 *	Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *	SPDX-License-Identifier: Apache-2.0
 *
 *	Licensed under the Apache License, Version 2.0 (the "License"); you may
 *	not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *	WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 *	This file is part of mbed TLS (https://tls.mbed.org)
 */
#include <tinyara/config.h>
#include <string.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

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
#include "mbedtls/asn1write.h"
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

#include <limits.h>

#include "mbedtls/asn1.h"
#include "mbedtls/asn1write.h"

#include "mbedtls/alt/common.h"
#include "alt_utils.h"

#define ALT_DEBUG 0

#if defined(MBEDTLS_RSA_C)
#if defined(MBEDTLS_PK_RSA_VERIFY_ALT)

static int rsa_get_mode(mbedtls_md_type_t md_alg, hal_rsa_mode *rsa_mode)
{
	switch (md_alg) {
	case MBEDTLS_MD_NONE:
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		break;
	case MBEDTLS_MD_MD5:
		rsa_mode->hash_t = HAL_HASH_MD5;
		rsa_mode->salt_byte_len = 32;
		break;
	case MBEDTLS_MD_SHA1:
		rsa_mode->hash_t = HAL_HASH_SHA1;
		rsa_mode->salt_byte_len = 32;
		break;
	case MBEDTLS_MD_SHA224:
		rsa_mode->hash_t = HAL_HASH_SHA224;
		rsa_mode->salt_byte_len = 32;
		break;
	case MBEDTLS_MD_SHA256:
		rsa_mode->hash_t = HAL_HASH_SHA256;
		rsa_mode->salt_byte_len = 32;
		break;
	case MBEDTLS_MD_SHA384:
		rsa_mode->hash_t = HAL_HASH_SHA384;
		rsa_mode->salt_byte_len = 64;
		break;
	case MBEDTLS_MD_SHA512:
		rsa_mode->hash_t = HAL_HASH_SHA512;
		rsa_mode->salt_byte_len = 64;
		break;
	default:
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

#if defined(MBEDTLS_PKCS1_V15)
	rsa_mode->rsa_a = HAL_RSASSA_PKCS1_V1_5;
#else //MBEDTLS_PKCS1_V21
	rsa_mode->rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
	rsa_mode->mgf = rsa_mode->hash_t;
#endif

	return 0;
}

static int rsa_verify_wrap(void *ctx,
						   mbedtls_md_type_t md_alg,
						   const unsigned char *hash,
						   size_t hash_len,
						   const unsigned char *sig,
						   size_t sig_len)
{
	int ret;
	unsigned char *pubkey_buf = NULL;
	unsigned int pubkey_buflen = MBEDTLS_MAX_BUF_SIZE_ALT;
	unsigned int key_idx;
	hal_key_type key_type;
	hal_rsa_mode rsa_mode;

	sl_ctx shnd;
#if SIZE_MAX > UINT_MAX
	if ((md_alg == MBEDTLS_MD_NONE) && (UINT_MAX < hash_len)) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}
#endif

	key_idx = ((mbedtls_rsa_context *)ctx)->key_index;

	if (ctx == NULL) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}
	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	t_pk.pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_RSA);
	t_pk.pk_ctx = (mbedtls_rsa_context *)ctx;
	pubkey_buf = (unsigned char *)malloc(pubkey_buflen);
	if (pubkey_buf == NULL) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	int len = mbedtls_pk_write_pubkey_der(&t_pk, pubkey_buf, pubkey_buflen);
	if (len <= 0) {
		ret = len;
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	hal_data pubkey = {
		0,
	};
	pubkey.data_len = len;
	pubkey.data = (unsigned char *)malloc(len);
	if (pubkey.data == NULL) {
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}
	memcpy(pubkey.data, pubkey_buf + pubkey_buflen - len, len);

	if ((len * 8) <= 1024) {
		key_type = HAL_KEY_RSA_1024;
	} else {
		key_type = HAL_KEY_RSA_2048;
	}

	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		free(pubkey_buf);
		free(pubkey.data);
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}
	ret = sl_set_key(shnd, key_type, key_idx, &pubkey, NULL);
	free(pubkey_buf);
	free(pubkey.data);
	if (ret != SECLINK_OK) {
		sl_deinit(shnd);
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	/*
	 * 2. Choose digest algorithm.
	 */
	ret = rsa_get_mode(md_alg, &rsa_mode);
	if (ret) {
		sl_deinit(shnd);
		return ret;
	}

	/*
	 *	3. Verify signature with public key.
	 */
	hal_data t_hash = {
		0,
	};
	t_hash.data = (unsigned char *)malloc(hash_len);
	if (t_hash.data == NULL) {
		sl_deinit(shnd);
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}
	memcpy(t_hash.data, hash, hash_len);
	t_hash.data_len = hash_len;

	hal_data t_sig = {
		0,
	};
	t_sig.data = (unsigned char *)malloc(sig_len);
	if (t_sig.data == NULL) {
		free(t_hash.data);
		sl_deinit(shnd);
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}
	memcpy(t_sig.data, sig, sig_len);
	t_sig.data_len = sig_len;

	ret = sl_rsa_verify_md(shnd, rsa_mode, &t_hash, &t_sig, key_idx);
	free(t_hash.data);
	free(t_sig.data);
	sl_deinit(shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	return 0;
}

static int rsa_sign_wrap(void *ctx,
						 mbedtls_md_type_t md_alg,
						 const unsigned char *hash,
						 size_t hash_len,
						 unsigned char *sig,
						 size_t *sig_len,
						 int (*f_rng)(void *, unsigned char *, size_t),
						 void *p_rng)
{
	int ret;
	unsigned int key_idx;

	hal_data t_hash = {
		0,
	};
	hal_data t_sig = {sig, 0, NULL, 0};
	hal_rsa_mode rsa_mode;
	sl_ctx shnd;

#if SIZE_MAX > UINT_MAX
	if (md_alg == MBEDTLS_MD_NONE && UINT_MAX < hash_len) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}
#endif

	t_hash.data = (unsigned char *)malloc(hash_len);
	if (t_hash.data == NULL) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}
	memcpy(t_hash.data, hash, hash_len);
	t_hash.data_len = hash_len;

	key_idx = ((mbedtls_rsa_context *)ctx)->key_index;

	ret = rsa_get_mode(md_alg, &rsa_mode);
	if (ret) {
		free(t_hash.data);
		return ret;
	}

	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		free(t_hash.data);
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	ret = sl_rsa_sign_md(shnd, rsa_mode, &t_hash, key_idx, &t_sig);
	free(t_hash.data);
	sl_deinit(shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	*sig_len = t_sig.data_len;

	return 0;
}

#endif /* MBEDTLS_PK_RSA_VERIFY_ALT */

#if defined(MBEDTLS_PK_RSA_ENCRYPT_ALT)
static int rsa_decrypt_wrap(void *ctx, const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen, size_t osize, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	hal_rsa_mode rsa_mode;
	sl_ctx shnd;
	unsigned int key_idx = ((mbedtls_rsa_context *)ctx)->key_index;

	if (((mbedtls_rsa_context *)ctx)->padding == MBEDTLS_RSA_PKCS_V15) {
		rsa_mode.rsa_a = HAL_RSASSA_PKCS1_V1_5;
	} else {
		rsa_mode.rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
	}

	ret = rsa_get_mode(((mbedtls_rsa_context *)ctx)->hash_id, &rsa_mode);
	if (ret) {
		return ret;
	}

	if (ilen != mbedtls_rsa_get_len((mbedtls_rsa_context *)ctx)) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	hal_data enc_data = {
		0,
	};
	hal_data dec_data = {output, osize, NULL, 0};

	enc_data.data = (unsigned char *)input;
	enc_data.data_len = ilen;

	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	ret = sl_rsa_decrypt(shnd, &enc_data, &rsa_mode, key_idx, &dec_data);
	sl_deinit(shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	*olen = dec_data.data_len;
	return 0;
}

static int rsa_encrypt_wrap(void *ctx, const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen, size_t osize, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	unsigned char *pubkey_buf = NULL;
	unsigned int pubkey_buflen = MBEDTLS_MAX_BUF_SIZE_ALT;
	hal_key_type key_type;
	hal_rsa_mode rsa_mode;
	sl_ctx shnd;
	unsigned int key_idx = ((mbedtls_rsa_context *)ctx)->key_index;

	if (((mbedtls_rsa_context *)ctx)->padding == MBEDTLS_RSA_PKCS_V15) {
		rsa_mode.rsa_a = HAL_RSASSA_PKCS1_V1_5;
	} else {
		rsa_mode.rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
	}

	ret = rsa_get_mode(((mbedtls_rsa_context *)ctx)->hash_id, &rsa_mode);
	if (ret) {
		return ret;
	}

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	t_pk.pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_RSA);
	t_pk.pk_ctx = (mbedtls_rsa_context *)ctx;

	pubkey_buf = (unsigned char *)malloc(pubkey_buflen);
	if (pubkey_buf == NULL) {
		return MBEDTLS_ERR_RSA_HW_ACCEL_FAILED;
	}

	int len = mbedtls_pk_write_pubkey_der(&t_pk, pubkey_buf, pubkey_buflen);
	if (len <= 0) {
		ret = len;
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	hal_data pubkey = {NULL, 0, NULL, 0};

	pubkey.data_len = len;
	pubkey.data = (unsigned char *)malloc(len);
	if (pubkey.data == NULL) {
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_HW_ACCEL_FAILED;
	}
	memcpy(pubkey.data, pubkey_buf + pubkey_buflen - len, len);

	if (len <= 1024) {
		key_type = HAL_KEY_RSA_1024;
	} else if (len <= 2048) {
		key_type = HAL_KEY_RSA_2048;
	} else if (len <= 4096) {
		key_type = HAL_KEY_RSA_4096;
	} else {
		free(pubkey_buf);
		free(pubkey.data);
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		free(pubkey_buf);
		free(pubkey.data);
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	ret = sl_set_key(shnd, key_type, key_idx, &pubkey, NULL);
	free(pubkey_buf);
	free(pubkey.data);
	if (ret != SECLINK_OK) {
		sl_deinit(shnd);
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	/*
	 *	2. Encrypt data with public key.
	 */
	hal_data dec_data = {
		0,
	};
	hal_data enc_data = {output, osize, NULL, 0};

	dec_data.data = (unsigned char *)input;
	dec_data.data_len = ilen;

	ret = sl_rsa_encrypt(shnd, &dec_data, &rsa_mode, key_idx, &enc_data);
	sl_deinit(shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	*olen = enc_data.data_len;
	return 0;
}

#endif /* MBEDTLS_PK_RSA_ENCRYPT_ALT */

#if defined(MBEDTLS_PK_RSA_VERIFY_ALT) || defined(MBEDTLS_PK_RSA_ENCRYPT_ALT)

static int rsa_can_do(mbedtls_pk_type_t type)
{
	return (type == MBEDTLS_PK_RSA || type == MBEDTLS_PK_RSASSA_PSS);
}

static size_t rsa_get_bitlen(const void *ctx)
{
	const mbedtls_rsa_context *rsa = (const mbedtls_rsa_context *)ctx;
	return (8 * mbedtls_rsa_get_len(rsa));
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

	return ctx;
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
	rsa_verify_wrap,
	rsa_sign_wrap,
	rsa_decrypt_wrap,
	rsa_encrypt_wrap,
	rsa_check_pair_wrap,
	rsa_alloc_wrap,
	rsa_free_wrap,
	rsa_debug,
};

#endif /* MBEDTLS_PK_RSA_VERIFY_ALT | MBEDTLS_PK_RSA_ENCRYPT_ALT */
#endif /* MBEDTLS_RSA_C */

#if defined(MBEDTLS_PK_ECDSA_VERIFY_ALT)
#if defined(MBEDTLS_ECP_C)
static int _eckey_pk_write_ec_pubkey(unsigned char **p, unsigned char *start,
									 mbedtls_ecp_keypair *ec)
{
	int ret;
	size_t len = 0;
	unsigned char buf[MBEDTLS_ECP_MAX_PT_LEN];

	if ((ret = mbedtls_ecp_point_write_binary(&ec->grp, &ec->Q,
											  MBEDTLS_ECP_PF_UNCOMPRESSED,
											  &len, buf, sizeof(buf))) != 0) {
		return (ret);
	}

	if (*p < start || (size_t)(*p - start) < len)
		return (MBEDTLS_ERR_ASN1_BUF_TOO_SMALL);

	*p -= len;
	memcpy(*p, buf, len);

	return ((int)len);
}

static int _eckey_write_ec_param(unsigned char **p, unsigned char *start,
								 mbedtls_ecp_keypair *ec)
{
	int ret;
	size_t len = 0;
	const char *oid;
	size_t oid_len;

	if ((ret = mbedtls_oid_get_oid_by_ec_grp(ec->grp.id, &oid, &oid_len)) != 0)
		return (ret);

	MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_oid(p, start, oid, oid_len));

	return ((int)len);
}

/*	apply it from int mbedtls_pk_write_key_der( mbedtls_pk_context *key, unsigned char *buf, size_t size ) */
static int _eckey_write_key_der(mbedtls_ecp_keypair *ec, unsigned char *buf, size_t size)
{
	size_t len = 0;
	int ret = 0;
	unsigned char *c = buf + size;
	//mbedtls_ecp_keypair *ec = mbedtls_pk_ec(*key);
	size_t pub_len = 0, par_len = 0;

	/*
				 * RFC 5915, or SEC1 Appendix C.4
				 *
				 * ECPrivateKey ::= SEQUENCE {
				 *			version				 INTEGER { ecPrivkeyVer1(1) } (ecPrivkeyVer1),
				 *			privateKey		 OCTET STRING,
				 *			parameters [0] ECParameters {{ NamedCurve }} OPTIONAL,
				 *			publicKey	 [1] BIT STRING OPTIONAL
				 *		}
				 */

	/* publicKey */
	MBEDTLS_ASN1_CHK_ADD(pub_len, _eckey_pk_write_ec_pubkey(&c, buf, ec));

	if (c - buf < 1)
		return (MBEDTLS_ERR_ASN1_BUF_TOO_SMALL);
	*--c = 0;
	pub_len += 1;

	MBEDTLS_ASN1_CHK_ADD(pub_len, mbedtls_asn1_write_len(&c, buf, pub_len));
	MBEDTLS_ASN1_CHK_ADD(pub_len, mbedtls_asn1_write_tag(&c, buf, MBEDTLS_ASN1_BIT_STRING));

	MBEDTLS_ASN1_CHK_ADD(pub_len, mbedtls_asn1_write_len(&c, buf, pub_len));
	MBEDTLS_ASN1_CHK_ADD(pub_len, mbedtls_asn1_write_tag(&c, buf,
														 MBEDTLS_ASN1_CONTEXT_SPECIFIC | MBEDTLS_ASN1_CONSTRUCTED | 1));
	len += pub_len;

	/* parameters */
	MBEDTLS_ASN1_CHK_ADD(par_len, _eckey_write_ec_param(&c, buf, ec));

	MBEDTLS_ASN1_CHK_ADD(par_len, mbedtls_asn1_write_len(&c, buf, par_len));
	MBEDTLS_ASN1_CHK_ADD(par_len, mbedtls_asn1_write_tag(&c, buf,
														 MBEDTLS_ASN1_CONTEXT_SPECIFIC | MBEDTLS_ASN1_CONSTRUCTED | 0));
	len += par_len;

	/* privateKey: write as MPI then fix tag */
	MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_mpi(&c, buf, &ec->d));
	*c = MBEDTLS_ASN1_OCTET_STRING;

	/* version */
	MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_int(&c, buf, 1));

	MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_len(&c, buf, len));
	MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_tag(&c, buf, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE));
	return len;
}

static int _eckey_get_ecdsa_mode(mbedtls_md_type_t md_alg,
								 unsigned int curve,
								 hal_ecdsa_mode *ecdsa_mode)
{
	switch (md_alg) {
	case MBEDTLS_MD_NONE:
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		break;
	case MBEDTLS_MD_MD5:
		ecdsa_mode->hash_t = HAL_HASH_MD5;
		break;
	case MBEDTLS_MD_SHA1:
		ecdsa_mode->hash_t = HAL_HASH_SHA1;
		break;
	case MBEDTLS_MD_SHA224:
		ecdsa_mode->hash_t = HAL_HASH_SHA224;
		break;
	case MBEDTLS_MD_SHA256:
		ecdsa_mode->hash_t = HAL_HASH_SHA256;
		break;
	case MBEDTLS_MD_SHA384:
		ecdsa_mode->hash_t = HAL_HASH_SHA384;
		break;
	case MBEDTLS_MD_SHA512:
		ecdsa_mode->hash_t = HAL_HASH_SHA512;
		break;
	default:
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

	switch (curve) {
	//case MBEDTLS_ECP_DP_SECP192R1:
	//case MBEDTLS_ECP_DP_SECP224R1:
	case MBEDTLS_ECP_DP_SECP256R1:
		ecdsa_mode->curve = HAL_ECDSA_SEC_P256R1;
		break;
	case MBEDTLS_ECP_DP_SECP384R1:
		ecdsa_mode->curve = HAL_ECDSA_SEC_P384R1;
		break;
	case MBEDTLS_ECP_DP_SECP521R1:
		ecdsa_mode->curve = HAL_ECDSA_SEC_P521R1;
		break;
	case MBEDTLS_ECP_DP_BP256R1:
		ecdsa_mode->curve = HAL_ECDSA_BRAINPOOL_P256R1;
		break;
	case MBEDTLS_ECP_DP_BP384R1:
		ecdsa_mode->curve = HAL_ECDSA_BRAINPOOL_P384R1;
		break;
	case MBEDTLS_ECP_DP_BP512R1:
		ecdsa_mode->curve = HAL_ECDSA_BRAINPOOL_P512R1;
		break;
	//case MBEDTLS_ECP_DP_CURVE25519:
	//case MBEDTLS_ECP_DP_SECP192K1:
	//case MBEDTLS_ECP_DP_SECP224K1:
	//case MBEDTLS_ECP_DP_SECP256K1:
	default:
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

	return 0;
}

/*
 * Convert a signature (given by context) to ASN.1
 */

/*
 * Generic EC key
 */
static int eckey_can_do(mbedtls_pk_type_t type)
{
	return (type == MBEDTLS_PK_ECKEY || type == MBEDTLS_PK_ECKEY_DH || type == MBEDTLS_PK_ECDSA);
}

static size_t eckey_get_bitlen(const void *ctx)
{
	return ((mbedtls_ecp_keypair *)ctx)->grp.pbits;
}

static int eckey_check_pair(const void *pub, const void *prv)
{
	return mbedtls_ecp_check_pub_priv((const mbedtls_ecp_keypair *)pub, (const mbedtls_ecp_keypair *)prv);
}

static void *eckey_alloc_wrap(void)
{
	void *ctx = mbedtls_calloc(1, sizeof(mbedtls_ecp_keypair));

	if (ctx != NULL) {
		mbedtls_ecp_keypair_init(ctx);
	}
	((mbedtls_ecp_keypair *)ctx)->key_index = KEY_INDEX_INVALID;
	return ctx;
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

int eckey_verify_wrap(void *ctx, mbedtls_md_type_t md_alg, const unsigned char *hash, size_t hash_len, const unsigned char *sig, size_t sig_len)
{
	int ret = 0;
	unsigned char *pubkey_buf = NULL;
	unsigned int pubkey_buflen = MBEDTLS_MAX_BUF_SIZE_ALT;
	unsigned int key_idx;
	hal_ecdsa_mode ecdsa_mode;
	sl_ctx shnd;

	((void)md_alg);

	if (ctx == NULL) {
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

	key_idx = ((mbedtls_ecdsa_context *)ctx)->key_index;
	if (key_idx == KEY_INDEX_INVALID) {
		/*  key wasn't set by an application so allocate ram key slot */
		key_idx = ECP_KEY_INDEX;
	}

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	unsigned int curve = ((mbedtls_ecdsa_context *)ctx)->grp.id;

	t_pk.pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
	t_pk.pk_ctx = (mbedtls_ecdsa_context *)ctx;

	pubkey_buf = (unsigned char *)malloc(pubkey_buflen);
	if (pubkey_buf == NULL) {
		return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
	}

	int len = mbedtls_pk_write_pubkey_der(&t_pk, pubkey_buf, pubkey_buflen);
	if ((len <= 0)) {
		free(pubkey_buf);
		return MBEDTLS_ERR_ECP_INVALID_KEY;
	}

	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
	}

	hal_data pubkey = {NULL, 0, NULL, 0};
	pubkey.data_len = len;
	pubkey.data = (unsigned char *)malloc(len);
	if (pubkey.data == NULL) {
		free(pubkey_buf);
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		goto FREE;
	}
#if ALT_DEBUG
	alt_print_buffer(pubkey.data, pubkey.data_len, "public key");
#endif
	memcpy(pubkey.data, pubkey_buf + pubkey_buflen - len, len);
	free(pubkey_buf);
	hal_key_type key_type = alt_get_keytype(curve);
	if (key_type == HAL_KEY_UNKNOWN) {
		free(pubkey.data);
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto FREE;
	}

	key_idx = alt_set_key(shnd, key_type, &pubkey, NULL, key_idx);
	free(pubkey.data);
	if (key_idx == -1) {
		goto FREE;
	}

	/*
	 * 2. Choose digest algorithm and curve type.
	 */
	ret = _eckey_get_ecdsa_mode(md_alg, curve, &ecdsa_mode);
	if (ret) {
		goto FREE_WITH_KEY;
	}

	/*
	 *	3. Verify signature with public key.
	 */
	hal_data t_hash = {NULL, 0, NULL, 0};
	t_hash.data = (unsigned char *)malloc(hash_len);
	if (t_hash.data == NULL) {
		ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		goto FREE_WITH_KEY;
	}

	memcpy(t_hash.data, hash, hash_len);
	t_hash.data_len = hash_len;

	hal_data t_sig = {NULL, 0, NULL, 0};
	t_sig.data = (unsigned char *)malloc(sig_len);
	if (t_sig.data == NULL) {
		free(t_hash.data);
		ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		goto FREE_WITH_KEY;
	}
	memcpy(t_sig.data, sig, sig_len);
	t_sig.data_len = sig_len;

	ret = sl_ecdsa_verify_md(shnd, ecdsa_mode, &t_hash, &t_sig, key_idx);
	free(t_hash.data);
	free(t_sig.data);

FREE_WITH_KEY:
	(void)sl_remove_key(shnd, key_type, key_idx);

FREE:
	sl_deinit(shnd);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	return ret;
}

int eckey_sign_wrap(void *ctx, mbedtls_md_type_t md_alg,
					const unsigned char *hash, size_t hash_len,
					unsigned char *sig, size_t *sig_len,
					int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	mbedtls_ecdsa_context ecdsa;
	hal_ecdsa_mode ecdsa_mode;
	sl_ctx shnd;
	hal_key_type key_type;
	int key_idx = 32;
	uint8_t is_injected_key = 1;
	unsigned int pubkey_buflen = MBEDTLS_MAX_BUF_SIZE_ALT;
	unsigned char *prikey_buf = NULL;
	unsigned int curve = MBEDTLS_ECP_DP_NONE;
	hal_data prikey = {NULL, 0, NULL, 0};
	hal_data t_hash = {NULL, 0, NULL, 0};
	hal_data sign = {sig, 0, NULL, 0};

	/*  Get ECDSA curve */
	mbedtls_ecdsa_init(&ecdsa);
	if ((ret = mbedtls_ecdsa_from_keypair(&ecdsa, ctx)) != 0) {
		mbedtls_ecdsa_free(&ecdsa);
		return MBEDTLS_ERR_ECP_INVALID_KEY;
	}
	curve = ecdsa.grp.id;
	mbedtls_ecdsa_free(&ecdsa);

	/*
	 * 1. Check hash algorithm and sign curve
	 */
	if ((ret = _eckey_get_ecdsa_mode(md_alg, curve, &ecdsa_mode)) != 0) {
		return ret;
	}

	if ((key_type = alt_get_keytype(curve)) == HAL_KEY_UNKNOWN) {
		return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
	}

	/* If key index is not in injected key range  0 ~ 31
	 * then key is set by mbedtls_pk_parse_key() */
	if ((((mbedtls_ecp_keypair *)ctx)->key_index) == 0xff) {
		is_injected_key = 0;
	}

	if (!is_injected_key) {
		/* application doesn't set injected key.
		 * So the private key are inserted by buffer. */
		prikey_buf = (unsigned char *)malloc(pubkey_buflen);
		if (prikey_buf == NULL) {
			return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		}
		/* Convert a private key to DER*/
		int len = _eckey_write_key_der((mbedtls_ecp_keypair *)ctx, prikey_buf, pubkey_buflen);
		if (len <= 0) {
			free(prikey_buf);
			return MBEDTLS_ERR_ECP_INVALID_KEY;
		}
		prikey.data_len = len;
		prikey.data = (unsigned char *)malloc(len);
		if (!prikey.data) {
			free(prikey_buf);
			return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		}
		memcpy(prikey.data, prikey_buf + pubkey_buflen - len, len);
		free(prikey_buf);
#if ALT_DEBUG
		alt_print_buffer(prikey.data, prikey.data_len, "private key");
#endif
	}

	t_hash.data = (unsigned char *)malloc(hash_len);
	if (t_hash.data == NULL) {
		if (!is_injected_key) {
			free(prikey.data);
		}
		return MBEDTLS_ERR_ECP_ALLOC_FAILED;
	}
	memcpy(t_hash.data, hash, hash_len);
	t_hash.data_len = hash_len;

	/*
	 * 2. Sign
	 */
	if ((ret = sl_init(&shnd)) != SECLINK_OK) {
		if (!is_injected_key) {
			free(prikey.data);
		}
		free(t_hash.data);
		return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
	}

	if (!is_injected_key) {
		key_idx = alt_set_key(shnd, key_type, NULL, &prikey, 32);
		free(prikey.data);
		if (key_idx < 0) {
			free(t_hash.data);
			sl_deinit(shnd);
			return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		}
	}

	ret = sl_ecdsa_sign_md(shnd, ecdsa_mode, &t_hash, key_idx, &sign);
	if (!is_injected_key) {
		sl_remove_key(shnd, key_type, key_idx);
	}
	sl_deinit(shnd);
	free(t_hash.data);

	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
	}

	/* Use r and s to generate signature */
	*sig_len = sign.data_len;
	return ret;
}

const mbedtls_pk_info_t mbedtls_eckey_info = {
	MBEDTLS_PK_ECKEY,
	"EC",
	eckey_get_bitlen,
	eckey_can_do,
	eckey_verify_wrap,
	eckey_sign_wrap,
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
	eckey_get_bitlen, /* Same underlying key structure */
	eckeydh_can_do,
	NULL,
	NULL,
	NULL,
	NULL,
	eckey_check_pair,
	eckey_alloc_wrap, /* Same underlying key structure */
	eckey_free_wrap,  /* Same underlying key structure */
	eckey_debug,	  /* Same underlying key structure */
};
#endif /* MBEDTLS_ECP_C */

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
		return MBEDTLS_ERR_PK_SIG_LEN_MISMATCH;
	}

	return ret;
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

	return ctx;
}

static void ecdsa_free_wrap(void *ctx)
{
	mbedtls_ecdsa_free((mbedtls_ecdsa_context *)ctx);
	mbedtls_free(ctx);
}

const mbedtls_pk_info_t mbedtls_ecdsa_info = {
	MBEDTLS_PK_ECDSA,
	"ECDSA",
	eckey_get_bitlen, /* Compatible key structures */
	ecdsa_can_do,
	ecdsa_verify_wrap,
	ecdsa_sign_wrap,
	NULL,
	NULL,
	eckey_check_pair, /* Compatible key structures */
	ecdsa_alloc_wrap,
	ecdsa_free_wrap,
	eckey_debug, /* Compatible key structures */
};
#endif /* MBEDTLS_ECDSA_C */
#endif /* MBEDTLS_PK_ECDSA_VERIFY_ALT */
#endif /* MBEDTLS_PK_C */
