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

<<<<<<< HEAD
#include "mbedtls/alt/common.h"

=======
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
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
#include <stdint.h>

#include "mbedtls/asn1.h"

<<<<<<< HEAD
=======
#include "mbedtls/alt/common.h"
#include "mbedtls/alt/pk_alt.h"

>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
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

<<<<<<< HEAD
int mbedtls_get_ecdsa_signature_alt( struct sECC_SIGN *ecc_sign, unsigned char *hash,
=======
int mbedtls_get_ecdsa_signature_alt( struct mbedtls_sECC_SIGN *ecc_sign, unsigned char *hash,
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
				unsigned int hash_len, unsigned int key_index )
{
	int r;

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	ISP_CHECKBUSY();
<<<<<<< HEAD
	if ((r = isp_ecdsa_sign_md_securekey(ecc_sign, hash, hash_len, key_index)) != 0) {
=======
	if ((r = isp_ecdsa_sign_md_securekey( (struct sECC_SIGN *)ecc_sign, hash, hash_len, key_index)) != 0) {
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}
	
	return( 0 );
}

<<<<<<< HEAD
int mbedtls_get_rsa_signature_alt( struct sRSA_SIGN *rsa_sign, unsigned char *hash,
=======
int mbedtls_get_rsa_signature_alt( struct mbedtls_sRSA_SIGN *rsa_sign, unsigned char *hash,
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
				unsigned int hash_len, unsigned int key_index )
{
	int r;

	if (rsa_sign == NULL || hash == NULL || hash_len == 0) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	ISP_CHECKBUSY();
<<<<<<< HEAD
	r = isp_rsa_sign_md_securekey(rsa_sign, hash, hash_len, key_index);
=======
	r = isp_rsa_sign_md_securekey( (struct sRSA_SIGN *)rsa_sign, hash, hash_len, key_index);
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
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
<<<<<<< HEAD
int mbedtls_verify_rsa_signature_alt( struct sRSA_SIGN *rsa_sign,
=======
int mbedtls_verify_rsa_signature_alt( struct mbedtls_sRSA_SIGN *rsa_sign,
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
				unsigned char *hash, unsigned int hash_len,
				unsigned char *key_buf )
{
	int r;

	if (rsa_sign == NULL || hash == NULL || hash_len == 0 || key_buf == NULL) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}
	
	ISP_CHECKBUSY();
<<<<<<< HEAD
	r = isp_rsa_verify_md_encryptedkey(rsa_sign, hash, hash_len, key_buf);
=======
	r = isp_rsa_verify_md_encryptedkey( (struct sRSA_SIGN *)rsa_sign, hash, hash_len, key_buf);
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
	if (r != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}
	
	return( 0 );
}

static int rsa_verify_wrap( void *ctx, mbedtls_md_type_t md_alg,
					const unsigned char *hash, size_t hash_len,
					const unsigned char *sig, size_t sig_len )
{
	int ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	unsigned char *key_buf = NULL;
	unsigned char *der_buf = NULL;
	unsigned char *t_hash = (unsigned char *) hash;
<<<<<<< HEAD
	struct sRSA_SIGN rsa_sign;
=======
	struct mbedtls_sRSA_SIGN rsa_sign;
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
	unsigned int padding;

	( (void) md_alg );

	if( ctx == NULL ) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	padding = ( (mbedtls_rsa_context *) ctx )->padding;

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	int len = 0;
	unsigned int der_buflen = 2048;

	t_pk.pk_info = mbedtls_pk_info_from_type( MBEDTLS_PK_RSA );
	t_pk.pk_ctx = (mbedtls_rsa_context *) ctx;

	der_buf = (unsigned char *)malloc( der_buflen );

	if( der_buf == NULL ) {
		return MBEDTLS_ERR_PK_ALLOC_FAILED;
	}

	len = mbedtls_pk_write_pubkey_der( &t_pk, der_buf, der_buflen );
	if( len < 0 ) {
		ret = len;
		goto cleanup;
	}

	key_buf = (unsigned char *)malloc( MBEDTLS_MAX_ENCRYPTED_KEY_SIZE_ALT );

	if( key_buf == NULL ) {
		ret = MBEDTLS_ERR_PK_ALLOC_FAILED;
		goto cleanup;
	}

	ret = mbedtls_setup_key_alt( der_buf + der_buflen - len, len, SECURE_STORAGE_TYPE_KEY_RSA, key_buf );
	if( ret != 0 ) {
		goto cleanup;
	}

<<<<<<< HEAD
	memset( &rsa_sign, 0, sizeof(struct sRSA_SIGN) );
=======
	memset( &rsa_sign, 0, sizeof(struct mbedtls_sRSA_SIGN) );
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs

	/*
	 * 2. Choose digest algorithm.
	 */
	switch( md_alg ) {
	case MBEDTLS_MD_NONE:		/* NOT SUPPORTED IN HW */
		ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		goto cleanup;
	case MBEDTLS_MD_SHA1:
		rsa_sign.alg_type = SHA1_160 | (padding ^ 0x1);	/* 0x110X */
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

	rsa_sign.signature = (unsigned char *) sig;
	rsa_sign.signature_byte_len = sig_len;

	/*
	 *  3. Verify signature with public key.
	 */
#if defined(MBEDTLS_PKCS1_V15) || defined(MBEDTLS_PKCS1_V21)
	ret = mbedtls_verify_rsa_signature_alt( &rsa_sign, t_hash, hash_len, key_buf );
#else
	ret = MBEDTLS_ERR_RSA_INVALID_PADDING;
#endif

cleanup:
	if( der_buf ) {
		free( der_buf );
	}

	if( key_buf ) {
		free( key_buf );
	}

	return ret;
}

static int rsa_sign_wrap( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    mbedtls_rsa_context * rsa = (mbedtls_rsa_context *) ctx;
	int ret;
<<<<<<< HEAD
	unsigned int padding = ctx->padding;
	unsigned int key_index;
	struct sRSA_SIGN rsa_sign;
	unsigned char *t_hash = (unsigned char *) hash;

	memset( &rsa_sign, 0, sizeof( struct sRSA_SIGN ) );
=======
	unsigned int padding = rsa->padding;
	unsigned int key_index;
	struct mbedtls_sRSA_SIGN rsa_sign;
	unsigned char *t_hash = (unsigned char *) hash;

	memset( &rsa_sign, 0, sizeof( struct mbedtls_sRSA_SIGN ) );
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs

#if SIZE_MAX > UINT_MAX
    if( md_alg == MBEDTLS_MD_NONE && UINT_MAX < hash_len ) {
		return( MBEDTLS_ERR_PK_BAD_INPUT_DATA );
	}
#endif /* SIZE_MAX > UINT_MAX */

	key_index = ((mbedtls_rsa_context *) ctx)->key_index;
	switch( md_alg ) {
		case MBEDTLS_MD_NONE:
			ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
			goto sign_exit;
		case MBEDTLS_MD_SHA1:
			rsa_sign.alg_type = SHA1_160 | ( padding ^ 0x1 );	/* 0x110X */
			break;
		case MBEDTLS_MD_SHA256:
			rsa_sign.alg_type = SHA2_256 | ( padding ^ 0x1 );	/* 0x230X */
			break;
		case MBEDTLS_MD_SHA384:
			rsa_sign.alg_type = SHA2_384 | ( padding ^ 0x1 );	/* 0x240X */
			break;
		case MBEDTLS_MD_SHA512:
			rsa_sign.alg_type = SHA2_512 | ( padding ^ 0x1 );	/* 0x250X */
			break;
		default:
			ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
			goto sign_exit;
	}

	rsa_sign.signature = sig;

#if defined(MBEDTLS_PKCS1_V15) || defined(MBEDTLS_PKCS1_V21)
		ret = mbedtls_get_rsa_signature_alt( &rsa_sign, t_hash, hash_len, key_index );
#else
		ret = MBEDTLS_ERR_RSA_INVALID_PADDING;
#endif
		
		*sig_len = rsa_sign.signature_byte_len;

		return ret;

sign_exit:
	return ret;
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

int mbedtls_rsa_encryption_alt( unsigned char *key_buf, unsigned int pad_type,
				unsigned char *output, unsigned int *outlen,
				unsigned char *input, unsigned int inlen )
{
	int r;

	if (input == NULL || output == NULL || inlen == 0 || key_buf == NULL) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	if (pad_type != MBEDTLS_RSA_PKCS_V15) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	ISP_CHECKBUSY();
	r = isp_rsa_encrypt_encryptedkey(output, outlen, input, inlen, key_buf);
	if (r != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}

	return( 0 );
}

static int rsa_decrypt_wrap( void *ctx,
                    const unsigned char *input, size_t ilen,
                    unsigned char *output, size_t *olen, size_t osize,
                    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    mbedtls_rsa_context * rsa = (mbedtls_rsa_context *) ctx;
<<<<<<< HEAD
	unsigned int padding = ctx->padding;
=======
	unsigned int padding = rsa->padding;
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
	unsigned char *t_input = (unsigned char *) input;
    unsigned int key_index;

    if( ilen != mbedtls_rsa_get_len( rsa ) )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );

    key_index = ( ( mbedtls_rsa_context *) ctx )->key_index;

	return( mbedtls_rsa_decryption_alt( key_index, padding, output, olen, t_input, ilen ) );
}

static int rsa_encrypt_wrap( void *ctx, const unsigned char *input, size_t ilen,
			unsigned char *output, size_t *olen, size_t osize,
			int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
	int ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	unsigned char *key_buf = NULL;
	unsigned char *der_buf = NULL;
	unsigned int padding;
	unsigned char *t_input = (unsigned char *) input;

	if( ctx == NULL ) {
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	padding = ( (mbedtls_rsa_context *) ctx )->padding;

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	int len = 0;
	unsigned int der_buflen = 1024;

	t_pk.pk_info = mbedtls_pk_info_from_type( MBEDTLS_PK_RSA );
	t_pk.pk_ctx = (mbedtls_rsa_context *) ctx;

	der_buf = (unsigned char *)malloc( der_buflen );

	if( der_buf == NULL ) {
		return MBEDTLS_ERR_PK_ALLOC_FAILED;
	}

	len = mbedtls_pk_write_pubkey_der( &t_pk, der_buf, der_buflen );
	if( len < 0 ) {
		ret = len;
		goto cleanup;
	}

	key_buf = (unsigned char *)malloc( MBEDTLS_MAX_ENCRYPTED_KEY_SIZE_ALT );

	if ( key_buf == NULL ) {
		ret = MBEDTLS_ERR_PK_ALLOC_FAILED;
		goto cleanup;
	}

	ret = mbedtls_setup_key_alt( der_buf + der_buflen - len, len, SECURE_STORAGE_TYPE_KEY_RSA, key_buf );
	if( ret  != 0 ) {
		goto cleanup;
	}

	/*
	 *  2. Encrypt data with public key.
	 */
#if defined(MBEDTLS_PKCS1_V15) || defined(MBEDTLS_PKCS1_V21)
	ret = mbedtls_rsa_encryption_alt( key_buf, padding, output, olen, t_input, ilen );
#endif

cleanup:
	if( der_buf ) {
		free( der_buf );
	}

	if( key_buf ) {
		free( key_buf );
	}

	return ret;
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


<<<<<<< HEAD
#if defined(MBEDTLS_ECP_C) && defined(MBEDTLS_PK_ECDSA_VERIFY_ALT)
int mbedtls_verify_ecdsa_signature_alt( struct sECC_SIGN *ecc_sign,
=======
#if defined(MBEDTLS_PK_ECDSA_VERIFY_ALT)
#if defined(MBEDTLS_ECP_C)
int mbedtls_verify_ecdsa_signature_alt( struct mbedtls_sECC_SIGN *ecc_sign,
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
                                        unsigned char *hash, unsigned int hash_len,
                                        unsigned char *key_buf )
{
	int r;

	if (ecc_sign == NULL || hash == NULL || hash_len == 0 || key_buf == NULL) {
		return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
	}

	ISP_CHECKBUSY();
<<<<<<< HEAD
	r = isp_ecdsa_verify_md_encryptedkey(ecc_sign, hash, hash_len, key_buf);
=======
	r = isp_ecdsa_verify_md_encryptedkey( (struct sECC_SIGN *)ecc_sign, hash, hash_len, key_buf);
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
	if (r != 0) {
		isp_clear(0);
		return MBEDTLS_ERR_PK_HW_ACCEL_FAILED;
	}

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
	int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	unsigned char *der_buf = NULL;
	unsigned char *key_buf = NULL;
	const unsigned char *end = sig + sig_len;
	unsigned char *p = (unsigned char *) sig;
	unsigned char *t_hash = (unsigned char *) hash;
<<<<<<< HEAD
	struct sECC_SIGN ecc_sign;
=======
	struct mbedtls_sECC_SIGN ecc_sign;
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs

	( (void) md_alg );

	if( ctx == NULL ) {
		return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
	}

	/*
	 * 1. Encrypt publickey for using HW accelator.
	 */
	mbedtls_pk_context t_pk;
	unsigned int len, curve = ( (mbedtls_ecdsa_context *) ctx)->grp.id, der_buflen = 1024;

	t_pk.pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
	t_pk.pk_ctx = (mbedtls_ecdsa_context *) ctx;

	der_buf = (unsigned char *)malloc( der_buflen );

	if( der_buf == NULL ) {
		return MBEDTLS_ERR_ECP_ALLOC_FAILED;
	}

	if( ( len = mbedtls_pk_write_pubkey_der( &t_pk, der_buf, der_buflen ) ) == 0 ) {
		ret = MBEDTLS_ERR_ECP_INVALID_KEY;
		goto cleanup;
	}

	key_buf = (unsigned char *)malloc( MBEDTLS_MAX_ENCRYPTED_KEY_SIZE_ALT );

	if( key_buf == NULL ) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	if( ( ret = mbedtls_setup_key_alt( der_buf + der_buflen - len, len, SECURE_STORAGE_TYPE_KEY_ECC, key_buf ) ) != 0 ) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	/*
	 * 2. Seperate 'r' and 's' from received signature.
	 */
<<<<<<< HEAD
	memset( &ecc_sign, 0, sizeof(struct sECC_SIGN) );
=======
	memset( &ecc_sign, 0, sizeof(struct mbedtls_sECC_SIGN) );
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs

	if( mbedtls_asn1_get_tag( &p, end, &len, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE ) ) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	if( p + len != end ) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA + MBEDTLS_ERR_ASN1_LENGTH_MISMATCH;
		goto cleanup;
	}

	unsigned int n, tmp = 0;

	/*
	 * 2-1 Get 'r' value
	 */
	if( mbedtls_asn1_get_tag( &p, end, &tmp, MBEDTLS_ASN1_INTEGER ) ) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	for( n = 0; n < tmp; n++ ) {
		if( p[n] != 0 ) {
			break;
		}
	}

	ecc_sign.r = p + n;
	ecc_sign.r_byte_len = tmp - n;
	p += tmp;

	/*
	 * 2-2 Get 's' value
	 */
	if( mbedtls_asn1_get_tag( &p, end, &tmp, MBEDTLS_ASN1_INTEGER ) ) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	for( n = 0; n < tmp; n++ ) {
		if( p[n] != 0 ) {
			break;
		}
	}

	ecc_sign.s = p + n;
	ecc_sign.s_byte_len = tmp - n;
	p += tmp;

	/*
	 * 3. Choose digest algorithm and curve type.
	 */
	switch( md_alg ) {
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

	switch( curve ) {
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
	ret = mbedtls_verify_ecdsa_signature_alt( &ecc_sign, t_hash, hash_len, key_buf );

cleanup:
	free( der_buf );

	if( key_buf ) {
		free( key_buf );
	}

	return ret;
}

int eckey_sign_wrap( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret;
    mbedtls_ecdsa_context ecdsa;

    mbedtls_ecdsa_init( &ecdsa );

<<<<<<< HEAD
    if( ( ret = mbedtls_ecdsa_from_keypair( &ecdsa, ctx ) ) == 0 )
=======
	mbedtls_mpi r, s;

	if( ( ret = mbedtls_ecdsa_from_keypair( &ecdsa, ctx ) ) == 0 )
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
    {
		unsigned int key_index = ( ( mbedtls_ecp_keypair *) ctx )->key_index;
		unsigned int curve = ecdsa.grp.id;
		unsigned char s_buf[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];
		unsigned char r_buf[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];
		unsigned char *t_hash = (unsigned char *)hash;

		/*
		 * 1. Check hash algorithm and sign curve
		 */
<<<<<<< HEAD
		mbedtls_mpi r, s;
		struct sECC_SIGN ecc_sign;
		memset( &ecc_sign, 0, sizeof( struct sECC_SIGN ) );
=======

		struct mbedtls_sECC_SIGN ecc_sign;
		memset( &ecc_sign, 0, sizeof( struct mbedtls_sECC_SIGN ) );
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs

		ecc_sign.s = s_buf;
		ecc_sign.r = r_buf;

		switch( md_alg ) {
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
		switch( curve ) {
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

		mbedtls_mpi_init( &r );
		mbedtls_mpi_init( &s );

		/*
		 * 2. Sign
		 */
		if( ( ret = mbedtls_get_ecdsa_signature_alt( &ecc_sign, t_hash, hash_len, key_index ) ) != 0 ) {
			ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
			goto cleanup;
		}

		if( ( ret = mbedtls_mpi_read_binary( &r, ecc_sign.r, ecc_sign.r_byte_len ) ) != 0 ) {
			goto cleanup;
		}
		if( ( ret = mbedtls_mpi_read_binary( &s, ecc_sign.s, ecc_sign.s_byte_len ) ) != 0 ) {
			goto cleanup;
		}

		MBEDTLS_MPI_CHK( ecdsa_signature_to_asn1( &r, &s, sig, sig_len ) );

		return( ret );

	} 
cleanup:
    mbedtls_ecdsa_free( &ecdsa );
	mbedtls_mpi_free( &r );
	mbedtls_mpi_free( &s );

	return( ret );
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

<<<<<<< HEAD
=======
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

>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs
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

<<<<<<< HEAD
#endif /* MBEDTLS_PK_ECDSA_VERIFY_ALT &  MBEDTLS_ECP_C */
=======
#endif /* MBEDTLS_PK_ECDSA_VERIFY_ALT */
>>>>>>> 3b68429fb... external/mbedtls: Modify dependency of remaming HW accel configs

#endif /* MBEDTLS_PK_C */
