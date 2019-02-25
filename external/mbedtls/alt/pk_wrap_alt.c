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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <security/hal/security_hal.h>

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

#include <limits.h>

#include "mbedtls/asn1.h"

#include "mbedtls/alt/common.h"

int mbedtls_setup_key_alt(unsigned char *key_der, unsigned int key_len, unsigned int key_type, unsigned char *key_buf)
{
	int r;

	if (key_der == NULL) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	if (key_type < SECURE_STORAGE_TYPE_KEY_AES || key_type > SECURE_STORAGE_TYPE_KEY_ECC) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;;
	}

	ISP_CHECKBUSY();
	r = isp_set_encryptedkey(key_der, key_len, key_type, key_buf);
	if (r != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}

	return( 0 );
}

int mbedtls_get_ecdsa_signature_alt( struct mbedtls_sECC_SIGN *ecc_sign, unsigned char *hash,
				unsigned int hash_len, unsigned int key_index )
{
	int r;

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	ISP_CHECKBUSY();
	if ((r = isp_ecdsa_sign_md_securekey( (struct sECC_SIGN *)ecc_sign, hash, hash_len, key_index)) != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}
	
	return( 0 );
}

int mbedtls_get_rsa_signature_alt( struct mbedtls_sRSA_SIGN *rsa_sign, unsigned char *hash,
				unsigned int hash_len, unsigned int key_index )
{
	int r;

	if (rsa_sign == NULL || hash == NULL || hash_len == 0) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_sign_md_securekey( (struct sRSA_SIGN *)rsa_sign, hash, hash_len, key_index);
	if (r != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}

	return( 0 );
}

int mbedtls_rsa_decryption_alt( unsigned int key_index, unsigned int pad_type,
				unsigned char *output, unsigned int *outlen,
				unsigned char *input, unsigned int inlen )
{
	int r;

	if (input == NULL || output == NULL || inlen == 0) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	if (pad_type != MBEDTLS_RSA_PKCS_V15) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_decrypt_securekey(output, outlen, input, inlen, key_index);
	if (r != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}

	return( 0 );
}

#if defined(MBEDTLS_RSA_C)

#if defined(MBEDTLS_PK_RSA_VERIFY_ALT)
int mbedtls_verify_rsa_signature_alt( struct mbedtls_sRSA_SIGN *rsa_sign,
				unsigned char *hash, unsigned int hash_len,
				unsigned char *key_buf )
{
	int r;

	if (rsa_sign == NULL || hash == NULL || hash_len == 0 || key_buf == NULL) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}
	
	ISP_CHECKBUSY();
	r = isp_rsa_verify_md_encryptedkey( (struct sRSA_SIGN *)rsa_sign, hash, hash_len, key_buf);
	if (r != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
#if defined(MBEDTLS_RSA_C)

#if defined(MBEDTLS_PK_RSA_VERIFY_ALT)
static int rsa_get_mode(mbedtls_md_type_t md_alg, hal_rsa_mode *rsa_mode)
{
	switch( md_alg ) {
	case MBEDTLS_MD_NONE:
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		break;
	case MBEDTLS_MD_MD5:
		rsa_mode->hash_t = HAL_HASH_MD5;
		break;
	case MBEDTLS_MD_SHA1:
		rsa_mode->hash_t = HAL_HASH_SHA1;
		break;
	case MBEDTLS_MD_SHA224:
		rsa_mode->hash_t = HAL_HASH_SHA224;
		break;
	case MBEDTLS_MD_SHA256:
		rsa_mode->hash_t = HAL_HASH_SHA256;
		break;
	case MBEDTLS_MD_SHA384:
		rsa_mode->hash_t = HAL_HASH_SHA384;
		break;
	case MBEDTLS_MD_SHA512:
		rsa_mode->hash_t = HAL_HASH_SHA512;
		break;
	default:
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

#if defined(MBEDTLS_PKCS1_V15)
	rsa_mode->rsa_a = HAL_RSASSA_PKCS1_V1_5;
#else //MBEDTLS_PKCS1_V21
	rsa_mode->rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
#endif

	return( 0 );
}

static int rsa_verify_wrap( void *ctx, mbedtls_md_type_t md_alg,
					const unsigned char *hash, size_t hash_len,
					const unsigned char *sig, size_t sig_len )
{
	int ret;
	unsigned char *pubkey_buf = NULL;
	unsigned int pubkey_buflen = MBEDTLS_MAX_BUF_SIZE_ALT;
	unsigned int key_idx;
	hal_rsa_mode rsa_mode;
	( (void) md_alg );

#if SIZE_MAX > UINT_MAX
	if( md_alg == MBEDTLS_MD_NONE && UINT_MAX < hash_len ) {
		return( MBEDTLS_ERR_PK_BAD_INPUT_DATA);
	}
#endif

	key_idx = ((mbedtls_rsa_context *) ctx)->key_index;

	if( ctx == NULL ) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	t_pk.pk_info = mbedtls_pk_info_from_type( MBEDTLS_PK_RSA );
	t_pk.pk_ctx = (mbedtls_rsa_context *) ctx;
	pubkey_buf = (unsigned char *)malloc( pubkey_buflen );
	if( pubkey_buf == NULL ) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	int len = mbedtls_pk_write_pubkey_der( &t_pk, pubkey_buf, pubkey_buflen );
	if( len <= 0 ) {
		ret = len;
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	hal_data pubkey;
	pubkey.data_len = len;
	pubkey.data = (unsigned char *)malloc( len );
	if( pubkey.data == NULL ) {
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}
	memcpy(pubkey.data, pubkey_buf + pubkey_buflen - len, len);

	switch( len ) {
	case 1024:
		ret = hal_set_key(HAL_KEY_RSA_1024, key_idx, &pubkey, NULL);
		break;
	case 2048:
		ret = hal_set_key(HAL_KEY_RSA_2048, key_idx, &pubkey, NULL);
		break;
	case 4096:
		ret = hal_set_key(HAL_KEY_RSA_4096, key_idx, &pubkey, NULL);
		break;
	default:
		free(pubkey_buf);
		free(pubkey.data);
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}
	free(pubkey_buf);
	free(pubkey.data);
	if( ret != HAL_SUCCESS ) {
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	/*
	 * 2. Choose digest algorithm.
	 */
	ret = rsa_get_mode(md_alg, &rsa_mode);
	if(ret) {
		return ret;
	}

	/*
	 *  3. Verify signature with public key.
	 */
	hal_data signature;
	hal_data t_hash;

	signature.data = (unsigned char *)sig;
	signature.data_len = sig_len;

	t_hash.data = (unsigned char *)hash;
	t_hash.data_len = hash_len;

	ret = hal_rsa_verify_md(rsa_mode, &t_hash, &signature, key_idx);

	if( ret != HAL_SUCCESS ) {
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	return( 0 );

}

static int rsa_sign_wrap( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
	int ret;
	unsigned int key_idx;
	hal_data t_hash;
	hal_data signature;
	hal_rsa_mode rsa_mode;

#if SIZE_MAX > UINT_MAX
    if( md_alg == MBEDTLS_MD_NONE && UINT_MAX < hash_len ) {
		return( MBEDTLS_ERR_PK_BAD_INPUT_DATA );
	}
#endif

	t_hash.data = (unsigned char *)hash;
	t_hash.data_len = hash_len;
	key_idx = ((mbedtls_rsa_context *) ctx)->key_index;

	ret = rsa_get_mode(md_alg, &rsa_mode);
	if(ret) {
		return ret;
	}

	ret = hal_rsa_sign_md(rsa_mode, &t_hash, key_idx, &signature);

	if( ret != HAL_SUCCESS) {
		free( t_hash.data );
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	*sig_len = signature.data_len;
	sig = signature.data;

    return( 0 );
}

#else /* MBEDTLS_PK_RSA_VERIFY_ALT */

static int rsa_verify_wrap( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   const unsigned char *sig, size_t sig_len )
{
    int ret;
    mbedtls_rsa_context * rsa = (mbedtls_rsa_context *) ctx;
    size_t rsa_len = mbedtls_rsa_get_len( rsa );

#if SIZE_MAX > UINT_MAX
    if( md_alg == MBEDTLS_MD_NONE && UINT_MAX < hash_len )
        return( MBEDTLS_ERR_PK_BAD_INPUT_DATA );
#endif /* SIZE_MAX > UINT_MAX */

    if( sig_len < rsa_len )
        return( MBEDTLS_ERR_RSA_VERIFY_FAILED );

    if( ( ret = mbedtls_rsa_pkcs1_verify( rsa, NULL, NULL,
                                  MBEDTLS_RSA_PUBLIC, md_alg,
                                  (unsigned int) hash_len, hash, sig ) ) != 0 )
        return( ret );

    /* The buffer contains a valid signature followed by extra data.
     * We have a special error code for that so that so that callers can
     * use mbedtls_pk_verify() to check "Does the buffer start with a
     * valid signature?" and not just "Does the buffer contain a valid
     * signature?". */
    if( sig_len > rsa_len )
        return( MBEDTLS_ERR_PK_SIG_LEN_MISMATCH );

    return( 0 );
}

static int rsa_sign_wrap( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    mbedtls_rsa_context * rsa = (mbedtls_rsa_context *) ctx;

#if SIZE_MAX > UINT_MAX
    if( md_alg == MBEDTLS_MD_NONE && UINT_MAX < hash_len )
        return( MBEDTLS_ERR_PK_BAD_INPUT_DATA );
#endif /* SIZE_MAX > UINT_MAX */

    *sig_len = mbedtls_rsa_get_len( rsa );

    return( mbedtls_rsa_pkcs1_sign( rsa, f_rng, p_rng, MBEDTLS_RSA_PRIVATE,
                md_alg, (unsigned int) hash_len, hash, sig ) );
}

#endif /* MBEDTLS_PK_RSA_VERIFY_ALT */

#if defined(MBEDTLS_PK_RSA_ENCRYPT_ALT)
static int rsa_decrypt_wrap( void *ctx,
                    const unsigned char *input, size_t ilen,
                    unsigned char *output, size_t *olen, size_t osize,
                    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
	int ret;
    unsigned int key_idx = ((mbedtls_rsa_context *) ctx)->key_index;
	unsigned int  padding = ( (mbedtls_rsa_context *) ctx )->padding;

	if( ctx == NULL || padding != MBEDTLS_RSA_PKCS_V15 ) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

    if( ilen != mbedtls_rsa_get_len( (mbedtls_rsa_context *)ctx ) ) {
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
	}

	hal_data enc_data;
	hal_data dec_data;

	enc_data.data = (unsigned char *)input;
	enc_data.data_len = ilen;

	ret = hal_rsa_decrypt(&enc_data, key_idx, &dec_data);

	if (ret != HAL_SUCCESS) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	output = dec_data.data;
	*olen = dec_data.data_len;

    return( 0 );
}

static int rsa_encrypt_wrap( void *ctx, const unsigned char *input, size_t ilen,
			unsigned char *output, size_t *olen, size_t osize,
			int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
	int ret;
	unsigned char *pubkey_buf = NULL;
	unsigned int pubkey_buflen = MBEDTLS_MAX_BUF_SIZE_ALT;
	unsigned int key_idx;
	unsigned int  padding = ( (mbedtls_rsa_context *) ctx )->padding;

	if( ctx == NULL || padding != MBEDTLS_RSA_PKCS_V15 ) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	key_idx = ((mbedtls_rsa_context *) ctx)->key_index;

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	t_pk.pk_info = mbedtls_pk_info_from_type( MBEDTLS_PK_RSA );
	t_pk.pk_ctx = (mbedtls_rsa_context *) ctx;

	pubkey_buf = (unsigned char *)malloc( pubkey_buflen );
	if( pubkey_buf == NULL ) {
		return MBEDTLS_ERR_RSA_HW_ACCEL_FAILED;
	}

	int len = mbedtls_pk_write_pubkey_der( &t_pk, pubkey_buf, pubkey_buflen );
	if( len <= 0 ) {
		ret = len;
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	hal_data pubkey;
	pubkey.data_len = len;
	pubkey.data = (unsigned char *)malloc( len );
	if ( pubkey.data == NULL ) {
		free(pubkey_buf);
		return MBEDTLS_ERR_RSA_HW_ACCEL_FAILED;
	}
	memcpy(pubkey.data, pubkey_buf + pubkey_buflen - len, len);

	switch( len ) {
	case 1024:
		ret = hal_set_key(HAL_KEY_RSA_1024, key_idx, &pubkey, NULL);
		break;
	case 2048:
		ret = hal_set_key(HAL_KEY_RSA_2048, key_idx, &pubkey, NULL);
		break;
	case 4096:
		ret = hal_set_key(HAL_KEY_RSA_4096, key_idx, &pubkey, NULL);
		break;
	default:
		free(pubkey_buf);
		free(pubkey.data);
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}
	free(pubkey_buf);
	free(pubkey.data);
	if( ret != HAL_SUCCESS ) {
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	/*
	 *  2. Encrypt data with public key.
	 */
	hal_data dec_data;
	hal_data enc_data;

	dec_data.data = (unsigned char *)input;
	dec_data.data_len = ilen;

	ret = hal_rsa_encrypt(&dec_data, key_idx, &enc_data);

	if (ret != HAL_SUCCESS) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

	output = enc_data.data;
	*olen = enc_data.data_len;

	return( 0 );
}

#else /* MBEDTLS_PK_RSA_ENCRYPT_ALT */

static int rsa_decrypt_wrap( void *ctx,
                    const unsigned char *input, size_t ilen,
                    unsigned char *output, size_t *olen, size_t osize,
                    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    mbedtls_rsa_context * rsa = (mbedtls_rsa_context *) ctx;

    if( ilen != mbedtls_rsa_get_len( rsa ) )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );

    return( mbedtls_rsa_pkcs1_decrypt( rsa, f_rng, p_rng,
                MBEDTLS_RSA_PRIVATE, olen, input, output, osize ) );
}

static int rsa_encrypt_wrap( void *ctx,
                    const unsigned char *input, size_t ilen,
                    unsigned char *output, size_t *olen, size_t osize,
                    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    mbedtls_rsa_context * rsa = (mbedtls_rsa_context *) ctx;
    *olen = mbedtls_rsa_get_len( rsa );

    if( *olen > osize )
        return( MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE );

    return( mbedtls_rsa_pkcs1_encrypt( rsa, f_rng, p_rng, MBEDTLS_RSA_PUBLIC,
                                       ilen, input, output ) );
}

#endif /* MBEDTLS_PK_RSA_ENCRYPT_ALT */

#if defined(MBEDTLS_PK_RSA_VERIFY_ALT) || defined(MBEDTLS_PK_RSA_ENCRYPT_ALT)

static int rsa_can_do( mbedtls_pk_type_t type )
{
    return( type == MBEDTLS_PK_RSA ||
            type == MBEDTLS_PK_RSASSA_PSS );
}

static size_t rsa_get_bitlen( const void *ctx )
{
    const mbedtls_rsa_context * rsa = (const mbedtls_rsa_context *) ctx;
    return( 8 * mbedtls_rsa_get_len( rsa ) );
}

static int rsa_check_pair_wrap( const void *pub, const void *prv )
{
    return( mbedtls_rsa_check_pub_priv( (const mbedtls_rsa_context *) pub,
                                (const mbedtls_rsa_context *) prv ) );
}

static void *rsa_alloc_wrap( void )
{
    void *ctx = mbedtls_calloc( 1, sizeof( mbedtls_rsa_context ) );

    if( ctx != NULL )
        mbedtls_rsa_init( (mbedtls_rsa_context *) ctx, 0, 0 );

    return( ctx );
}

static void rsa_free_wrap( void *ctx )
{
    mbedtls_rsa_free( (mbedtls_rsa_context *) ctx );
    mbedtls_free( ctx );
}

static void rsa_debug( const void *ctx, mbedtls_pk_debug_item *items )
{
    items->type = MBEDTLS_PK_DEBUG_MPI;
    items->name = "rsa.N";
    items->value = &( ((mbedtls_rsa_context *) ctx)->N );

    items++;

    items->type = MBEDTLS_PK_DEBUG_MPI;
    items->name = "rsa.E";
    items->value = &( ((mbedtls_rsa_context *) ctx)->E );
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
static int ecdsa_get_mode(mbedtls_md_type_t md_alg, unsigned int curve, hal_ecdsa_mode *ecdsa_mode)
{
	switch( md_alg ) {
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

	switch( curve ) {
		//case MBEDTLS_ECP_DP_SECP192R1:
		//case MBEDTLS_ECP_DP_SECP224R1:
		case MBEDTLS_ECP_DP_SECP256R1:
			ecdsa_mode->curve = HAL_ECDSA_SEC_P256R1;
			break;
		case MBEDTLS_ECP_DP_SECP384R1:
			ecdsa_mode->curve = HAL_ECDSA_SEC_P384R1;
			break;
		case MBEDTLS_ECP_DP_SECP521R1:
			ecdsa_mode->curve = HAL_ECDSA_SEC_P512R1;
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

	return( 0 );
}

/*
 * Convert a signature (given by context) to ASN.1
 */
static int ecdsa_signature_to_asn1( const mbedtls_mpi *r, const mbedtls_mpi *s,
                                    unsigned char *sig, size_t *slen )
{
	int ret;
    unsigned char buf[MBEDTLS_ECDSA_MAX_LEN];
    unsigned char *p = buf + sizeof( buf );
    size_t len = 0;

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, s ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, r ) );

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_len( &p, buf, len ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_tag( &p, buf,
                                       MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE ) );

    memcpy( sig, p, len );
    *slen = len;

    return( 0 );
}

/*
 * Generic EC key
 */
static int eckey_can_do( mbedtls_pk_type_t type )
{
    return( type == MBEDTLS_PK_ECKEY ||
            type == MBEDTLS_PK_ECKEY_DH ||
            type == MBEDTLS_PK_ECDSA );
}

static size_t eckey_get_bitlen( const void *ctx )
{
    return( ((mbedtls_ecp_keypair *) ctx)->grp.pbits );
}


static int eckey_check_pair( const void *pub, const void *prv )
{
    return( mbedtls_ecp_check_pub_priv( (const mbedtls_ecp_keypair *) pub,
                                (const mbedtls_ecp_keypair *) prv ) );
}

static void *eckey_alloc_wrap( void )
{
    void *ctx = mbedtls_calloc( 1, sizeof( mbedtls_ecp_keypair ) );

    if( ctx != NULL )
        mbedtls_ecp_keypair_init( ctx );

    return( ctx );
}

static void eckey_free_wrap( void *ctx )
{
    mbedtls_ecp_keypair_free( (mbedtls_ecp_keypair *) ctx );
    mbedtls_free( ctx );
}

static void eckey_debug( const void *ctx, mbedtls_pk_debug_item *items )
{
    items->type = MBEDTLS_PK_DEBUG_ECP;
    items->name = "eckey.Q";
    items->value = &( ((mbedtls_ecp_keypair *) ctx)->Q );
}

int eckey_verify_wrap( void *ctx, mbedtls_md_type_t md_alg,
                       const unsigned char *hash, size_t hash_len,
                       const unsigned char *sig, size_t sig_len )
{
	int ret;
	unsigned char *pubkey_buf = NULL;
	unsigned int pubkey_buflen = MBEDTLS_MAX_BUF_SIZE_ALT;
	unsigned int key_idx;
	hal_ecdsa_mode ecdsa_mode;

	( (void) md_alg );

	if( ctx == NULL ) {
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

	key_idx = ((mbedtls_ecdsa_context *) ctx)->key_index;

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	unsigned int curve = ( (mbedtls_ecdsa_context *) ctx )->grp.id;

	t_pk.pk_info = mbedtls_pk_info_from_type( MBEDTLS_PK_ECKEY );
	t_pk.pk_ctx = (mbedtls_ecdsa_context *) ctx;

	pubkey_buf = (unsigned char *)malloc( pubkey_buflen );
	if( pubkey_buf == NULL ) {
		return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
	}
	int len = mbedtls_pk_write_pubkey_der( &t_pk, pubkey_buf, pubkey_buflen);

	if( ( len <= 0 ) ) {
		free(pubkey_buf);
		return( MBEDTLS_ERR_ECP_INVALID_KEY );
	}

	hal_data pubkey;
	pubkey.data_len = len;
	pubkey.data = (unsigned char *)malloc( len );
	if ( pubkey.data == NULL ) {
		free(pubkey_buf);
		return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
	}
	memcpy(pubkey.data, pubkey_buf + pubkey_buflen - len, len);

	switch( curve ) {
		//case MBEDTLS_ECP_DP_SECP192R1:
		//case MBEDTLS_ECP_DP_SECP224R1:
		case MBEDTLS_ECP_DP_SECP256R1:
			ret = hal_set_key(HAL_KEY_ECC_SEC_P256R1, key_idx, &pubkey, NULL);
			break;
		case MBEDTLS_ECP_DP_SECP384R1:
			ret = hal_set_key(HAL_KEY_ECC_SEC_P384R1, key_idx, &pubkey, NULL);
			break;
		case MBEDTLS_ECP_DP_SECP521R1:
			ret = hal_set_key(HAL_KEY_ECC_SEC_P512R1, key_idx, &pubkey, NULL);
			break;
		case MBEDTLS_ECP_DP_BP256R1:
			ret = hal_set_key(HAL_KEY_ECC_BRAINPOOL_P256R1, key_idx, &pubkey, NULL);
			break;
		case MBEDTLS_ECP_DP_BP384R1:
			ret = hal_set_key(HAL_KEY_ECC_BRAINPOOL_P384R1, key_idx, &pubkey, NULL);
			break;
		case MBEDTLS_ECP_DP_BP512R1:
			ret = hal_set_key(HAL_KEY_ECC_BRAINPOOL_P512R1, key_idx, &pubkey, NULL);
			break;
		//case MBEDTLS_ECP_DP_CURVE25519:
		//case MBEDTLS_ECP_DP_SECP192K1:
		//case MBEDTLS_ECP_DP_SECP224K1:
		//case MBEDTLS_ECP_DP_SECP256K1:
		default:
			free(pubkey_buf);
			free(pubkey.data);
			return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}
	free(pubkey_buf);
	free(pubkey.data);
	if( ret != HAL_SUCCESS ) {
		return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
	}

	/*
	 * 2. Choose digest algorithm and curve type.
	 */
	ret = ecdsa_get_mode( md_alg, curve, &ecdsa_mode );
	if(ret) {
		return ret;
	}

	/*
	 *  3. Verify signature with public key.
	 */
	hal_data t_hash;
	hal_data signature;
	t_hash.data = (unsigned char *)hash;
	t_hash.data_len = hash_len;
	signature.data = (unsigned char *)sig;
	signature.data_len = sig_len;

	ret = hal_ecdsa_verify_md(ecdsa_mode, &t_hash, &signature, key_idx);

	if (ret != HAL_SUCCESS) {
		return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION;
	}

    return( 0 );
}

int eckey_sign_wrap( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret;
    mbedtls_ecdsa_context ecdsa;
    mbedtls_ecdsa_init( &ecdsa );

	if( ( ret = mbedtls_ecdsa_from_keypair( &ecdsa, ctx ) ) == 0 )
    {
		unsigned int key_idx = ( ( mbedtls_ecp_keypair *) ctx )->key_index;
		unsigned int curve = ecdsa.grp.id;
		hal_ecdsa_mode ecdsa_mode;
		hal_data t_hash;

		t_hash.data = (unsigned char *)hash;
		t_hash.data_len = hash_len;

		mbedtls_ecdsa_free( &ecdsa );
		/*
		 * 1. Check hash algorithm and sign curve
		 */

		ret = ecdsa_get_mode( md_alg, curve, &ecdsa_mode );
		if(ret) {
			return ret;
		}

		/*
		 * 2. Sign
		 */
		hal_data sign;
		sign.data = NULL;
		sign.data_len = 0;
		ret = hal_ecdsa_sign_md( &t_hash, key_idx, &ecdsa_mode, &sign );
		if( ret != HAL_SUCCESS ) {
			return MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		}

		/* Use r and s to generate signature */
		if( sign.data == NULL ) {
			mbedtls_mpi r, s;
			mbedtls_mpi_init( &r );
			mbedtls_mpi_init( &s );

			ret = mbedtls_mpi_read_binary( &r, ecdsa_mode.r->data, ecdsa_mode.r->data_len );
			if(ret) {
				hal_free_data( ecdsa_mode.r );
				hal_free_data( ecdsa_mode.s );
				mbedtls_mpi_free( &r );
				mbedtls_mpi_free( &s );
				return ret;
			}
			ret = mbedtls_mpi_read_binary( &s, ecdsa_mode.s->data, ecdsa_mode.s->data_len );
			if(ret) {
				hal_free_data( ecdsa_mode.r );
				hal_free_data( ecdsa_mode.s );
				mbedtls_mpi_free( &r );
				mbedtls_mpi_free( &s );
				return ret;
			}

			ecdsa_signature_to_ans1( &r, &s, sig, sig_len );
			hal_free_data( ecdsa_mode.r );
			hal_free_data( ecdsa_mode.s );
			mbedtls_mpi_free( &r );
			mbedtls_mpi_free( &s );
		} else {
			sig = sign.data;
			*sig_len = sign.data_len;
		}

		return( ret );
	} 
	mbedtls_ecdsa_free( &ecdsa );

	return( MBEDTLS_ERR_ECP_INVALID_KEY );
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
static int eckeydh_can_do( mbedtls_pk_type_t type )
{
    return( type == MBEDTLS_PK_ECKEY ||
            type == MBEDTLS_PK_ECKEY_DH );
}

const mbedtls_pk_info_t mbedtls_eckeydh_info = {
    MBEDTLS_PK_ECKEY_DH,
    "EC_DH",
    eckey_get_bitlen,         /* Same underlying key structure */
    eckeydh_can_do,
    NULL,
    NULL,
    NULL,
    NULL,
    eckey_check_pair,
    eckey_alloc_wrap,       /* Same underlying key structure */
    eckey_free_wrap,        /* Same underlying key structure */
    eckey_debug,            /* Same underlying key structure */
};
#endif /* MBEDTLS_ECP_C */

#if defined(MBEDTLS_ECDSA_C)
static int ecdsa_can_do( mbedtls_pk_type_t type )
{
    return( type == MBEDTLS_PK_ECDSA );
}

static int ecdsa_verify_wrap( void *ctx, mbedtls_md_type_t md_alg,
                       const unsigned char *hash, size_t hash_len,
                       const unsigned char *sig, size_t sig_len )
{
    int ret;
    ((void) md_alg);

    ret = mbedtls_ecdsa_read_signature( (mbedtls_ecdsa_context *) ctx,
                                hash, hash_len, sig, sig_len );

    if( ret == MBEDTLS_ERR_ECP_SIG_LEN_MISMATCH )
        return( MBEDTLS_ERR_PK_SIG_LEN_MISMATCH );

    return( ret );
}

static int ecdsa_sign_wrap( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    return( mbedtls_ecdsa_write_signature( (mbedtls_ecdsa_context *) ctx,
                md_alg, hash, hash_len, sig, sig_len, f_rng, p_rng ) );
}

static void *ecdsa_alloc_wrap( void )
{
    void *ctx = mbedtls_calloc( 1, sizeof( mbedtls_ecdsa_context ) );

    if( ctx != NULL )
        mbedtls_ecdsa_init( (mbedtls_ecdsa_context *) ctx );

    return( ctx );
}

static void ecdsa_free_wrap( void *ctx )
{
    mbedtls_ecdsa_free( (mbedtls_ecdsa_context *) ctx );
    mbedtls_free( ctx );
}

const mbedtls_pk_info_t mbedtls_ecdsa_info = {
    MBEDTLS_PK_ECDSA,
    "ECDSA",
    eckey_get_bitlen,     /* Compatible key structures */
    ecdsa_can_do,
    ecdsa_verify_wrap,
    ecdsa_sign_wrap,
    NULL,
    NULL,
    eckey_check_pair,   /* Compatible key structures */
    ecdsa_alloc_wrap,
    ecdsa_free_wrap,
    eckey_debug,        /* Compatible key structures */
};
#endif /* MBEDTLS_ECDSA_C */

#endif /* MBEDTLS_PK_ECDSA_VERIFY_ALT */

#endif /* MBEDTLS_PK_C */
