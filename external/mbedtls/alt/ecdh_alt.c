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
 *  Elliptic curve Diffie-Hellman
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
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 * RFC 4492
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECDH_C)

#include "mbedtls/ecdh.h"

#include <string.h>

<<<<<<< HEAD
#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT) || defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)

=======
#include "mbedtls/alt/common.h"

#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT) || defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM
#include "mbedtls/alt/ecdh_alt.h"

/*
 * Free context
 */
void mbedtls_ecdh_free( mbedtls_ecdh_context *ctx )
{
<<<<<<< HEAD
    if( ctx == NULL )
        return;
=======
    if( ctx == NULL ) {
        return;
	}
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM

	if( ctx->grp.key_buf ) {
		memset( ctx->grp.key_buf, 0, MBEDTLS_MAX_ENCRYPTED_KEY_SIZE_ALT );
		free( ctx->grp.key_buf );
		ctx->grp.key_buf = NULL;
	}

    mbedtls_ecp_group_free( &ctx->grp );
    mbedtls_ecp_point_free( &ctx->Q   );
    mbedtls_ecp_point_free( &ctx->Qp  );
    mbedtls_ecp_point_free( &ctx->Vi  );
    mbedtls_ecp_point_free( &ctx->Vf  );
    mbedtls_mpi_free( &ctx->d  );
    mbedtls_mpi_free( &ctx->z  );
    mbedtls_mpi_free( &ctx->_d );
}

<<<<<<< HEAD
#if defined(CONFIG_TLS_WITH_SSS)
	ctx->grp.key_index = key->key_index;
#endif

	return( 0 );
}
=======

/*
 * Get parameters from a keypair
 */
int mbedtls_ecdh_get_params( mbedtls_ecdh_context *ctx, const mbedtls_ecp_keypair *key,
                     mbedtls_ecdh_side side )
{
    int ret;

    if( ( ret = mbedtls_ecp_group_copy( &ctx->grp, &key->grp ) ) != 0 )
        return( ret );

    /* If it's not our key, just import the public part as Qp */
    if( side == MBEDTLS_ECDH_THEIRS )
        return( mbedtls_ecp_copy( &ctx->Qp, &key->Q ) );

    /* Our key: import public (as Q) and private parts */
    if( side != MBEDTLS_ECDH_OURS )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if( ( ret = mbedtls_ecp_copy( &ctx->Q, &key->Q ) ) != 0 ||
        ( ret = mbedtls_mpi_copy( &ctx->d, &key->d ) ) != 0 ) {
        return( ret );
	}

	ctx->grp.key_index = key->key_index;

    return( 0 );
}

>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM
#endif /* MBEDTLS_ECDH_GEN_PUBLIC_ALT | MBEDTLS_ECDH_COMPUTE_SHARED_ALT */

#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT)
int mbedtls_ecdh_gen_public( mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q,
                     int (*f_rng)(void *, unsigned char *, size_t),
                     void *p_rng )
{
    return( mbedtls_ecp_gen_keypair_alt( grp, d, Q ) );
}

int mbedtls_ecp_gen_keypair_alt( mbedtls_ecp_group *grp, mbedtls_mpi *d, 
							mbedtls_ecp_point *Q)
{
	unsigned int ret;
	unsigned int key_type = ECC_KEY;
	unsigned int curve;
<<<<<<< HEAD
	struct sECC_KEY ecc_pub;

	memset( &ecc_pub, 0, sizeof( struct sECC_KEY ) );
=======
	struct mbedtls_sECC_KEY ecc_pub;

	memset( &ecc_pub, 0, sizeof( struct mbedtls_sECC_KEY ) );
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM

	switch( grp->id ) {
	case MBEDTLS_ECP_DP_SECP192R1:
		curve = OID_ECC_P192;
		break;
	case MBEDTLS_ECP_DP_SECP224R1:
		curve = OID_ECC_P224;
		break;
	case MBEDTLS_ECP_DP_SECP256R1:
		curve = OID_ECC_P256;
		break;
	case MBEDTLS_ECP_DP_SECP384R1:
		curve = OID_ECC_P384;
		break;
	case MBEDTLS_ECP_DP_SECP521R1:
		curve = OID_ECC_P521;
		break;
	case MBEDTLS_ECP_DP_BP256R1:
		curve = OID_ECC_BP256;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
		goto cleanup;
	}

	if( grp->key_buf == NULL )
	{
		grp->key_buf = (unsigned char *)malloc( MBEDTLS_MAX_ENCRYPTED_KEY_SIZE_ALT );
		if (grp->key_buf == NULL)
			return( MBEDTLS_ERR_ECP_ALLOC_FAILED );
	}

	if( ( ret = mbedtls_generate_key_alt( key_type | curve, grp->key_buf, 0, 0 ) ) != 0 ) {
		return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );
	}

	ecc_pub.publickey_x = (unsigned char *)malloc( MBEDTLS_MAX_ECP_KEY_SIZE_ALT );
	if( ecc_pub.publickey_x == NULL )
		return( MBEDTLS_ERR_ECP_ALLOC_FAILED );

	ecc_pub.publickey_y = (unsigned char *)malloc( MBEDTLS_MAX_ECP_KEY_SIZE_ALT );
	if( ecc_pub.publickey_y == NULL )
	{
		free( ecc_pub.publickey_x );
		return( MBEDTLS_ERR_ECP_ALLOC_FAILED );
	}

	/* Get Public value from sss */
	if( ( ret = mbedtls_get_ecc_publickey_alt( &ecc_pub, grp->key_buf, curve ) ) != 0 )
	{
		ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
		goto cleanup;
	}

	/* Copy pub value to Q */
	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &Q->X, ecc_pub.publickey_x, ecc_pub.x_byte_len ) );
	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &Q->Y, ecc_pub.publickey_y, ecc_pub.y_byte_len ) );
	MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &Q->Z, 1 ) );

cleanup:
	if( ecc_pub.publickey_x )
		free( ecc_pub.publickey_x );

	if( ecc_pub.publickey_y )
		free( ecc_pub.publickey_y );

	return( ret );
}

int mbedtls_generate_key_alt( unsigned int key_type, unsigned char *key_buf,
							unsigned int key_len, unsigned int pukey_e )
{
	int r = 0;
	unsigned int key = key_type & 0xFF0000;
	unsigned int object_id = key_type & 0xFF;

	switch (key) {
	case ECC_KEY:
		ISP_CHECKBUSY();
		r = isp_ecdsa_generate_key_encryptedkey(object_id, key_buf);
		break;
	default:
		return( MBEDTLS_ERR_ECDH_BAD_INPUT_DATA);
	}

	if (r) {
		isp_clear(0);
		return( MBEDTLS_ERR_ECDH_HW_ACCEL_FAILED );
	}

	return( 0 );
}

<<<<<<< HEAD
int mbedtls_get_ecc_publickey_alt( struct sECC_KEY *ecc_pub, unsigned char *key_buf,
=======
int mbedtls_get_ecc_publickey_alt( struct mbedtls_sECC_KEY *ecc_pub, unsigned char *key_buf,
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM
							unsigned int object_id )
{
	unsigned int r;

	if (ecc_pub == NULL || key_buf == NULL) {
		return( MBEDTLS_ERR_ECDH_BAD_INPUT_DATA ); 
	}

	ISP_CHECKBUSY();
<<<<<<< HEAD
	r = isp_ecdsa_get_publickey_encryptedkey(ecc_pub, object_id, key_buf);
=======
	r = isp_ecdsa_get_publickey_encryptedkey( (struct sECC_KEY *)ecc_pub, object_id, key_buf );
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM
	if (r != 0) {
		isp_clear(0);
		return( MBEDTLS_ERR_ECDH_HW_ACCEL_FAILED );
	}

	return( 0 );
}
#endif /* MBEDTLS_ECDH_GEN_PUBLIC_ALT */

#if defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
int mbedtls_ecdh_compute_shared( mbedtls_ecp_group *grp, mbedtls_mpi *z,
                         const mbedtls_ecp_point *Q, const mbedtls_mpi *d,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng )
{
	return( mbedtls_ecdh_compute_shared_alt( grp, z, Q ) );
}

int mbedtls_ecdh_compute_shared_alt( mbedtls_ecp_group *grp,
							mbedtls_mpi *z, const mbedtls_ecp_point *Q )
{
	int ret;
<<<<<<< HEAD
	struct sECC_KEY ecc_pub;
	unsigned int olen = MBEDTLS_MAX_ECP_KEY_SIZE_ALT;
	unsigned char output[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];

	memset( &ecc_pub, 0, sizeof( struct sECC_KEY ) );
=======
	struct mbedtls_sECC_KEY ecc_pub;
	unsigned int olen = MBEDTLS_MAX_ECP_KEY_SIZE_ALT;
	unsigned char output[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];

	memset( &ecc_pub, 0, sizeof( struct mbedtls_sECC_KEY ) );
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM

	ecc_pub.x_byte_len = mbedtls_mpi_size( &Q->X );
	ecc_pub.y_byte_len = mbedtls_mpi_size( &Q->Y );

	if( !( ecc_pub.publickey_x = (unsigned char *)malloc( ecc_pub.x_byte_len ) ) )
		return( MBEDTLS_ERR_ECP_ALLOC_FAILED );

	if( !( ecc_pub.publickey_y = (unsigned char *)malloc( ecc_pub.y_byte_len ) ) )
	{
		free( ecc_pub.publickey_x );
		return( MBEDTLS_ERR_ECP_ALLOC_FAILED );
	}

	MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &Q->X, ecc_pub.publickey_x, ecc_pub.x_byte_len ) );
	MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &Q->Y, ecc_pub.publickey_y, ecc_pub.y_byte_len ) );

	switch( grp->id ) {
	case MBEDTLS_ECP_DP_SECP192R1:
		ecc_pub.curve |= OID_ECC_P192;
		break;
	case MBEDTLS_ECP_DP_SECP224R1:
		ecc_pub.curve |= OID_ECC_P224;
		break;
	case MBEDTLS_ECP_DP_SECP256R1:
		ecc_pub.curve |= OID_ECC_P256;
		break;
	case MBEDTLS_ECP_DP_SECP384R1:
		ecc_pub.curve |= OID_ECC_P384;
		break;
	case MBEDTLS_ECP_DP_SECP521R1:
		ecc_pub.curve |= OID_ECC_P521;
		break;
	case MBEDTLS_ECP_DP_BP256R1:
		ecc_pub.curve |= OID_ECC_BP256;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

<<<<<<< HEAD
#if defined(CONFIG_TLS_WITH_SSS)
=======
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM
	/* Compute P  = d * Q */
	if( grp->key_buf == NULL )
	{
		/* compute ECC shared secret with stored key (permanent) */
		if( ( ret = mbedtls_compute_ecdh_param_alt( &ecc_pub, grp->key_index, output, &olen ) ) != 0 )
			goto cleanup;
	} else
<<<<<<< HEAD
#endif
	{
		/* compute ECC shared secret with generated key (temporary) */
		if( ( ret = mbedtls_compute_ecdh_param_alt( &ecc_pub, grp->key_buf, output, &olen ) ) != 0 )
			goto cleanup;
	}
=======
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM

	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( z, output, olen ) );

cleanup:
	if( ecc_pub.publickey_x )
		free( ecc_pub.publickey_x );

	if ( ecc_pub.publickey_y )
		free( ecc_pub.publickey_y );

	return( ret );
}

<<<<<<< HEAD
int mbedtls_compute_ecdh_param_alt( struct sECC_KEY *ecc_pub, unsigned char *key_buf,
=======
int mbedtls_compute_ecdh_param_alt( struct mbedtls_sECC_KEY *ecc_pub, unsigned int key_index,
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM
							unsigned char *output, unsigned int *olen )
{
	int r;

<<<<<<< HEAD
	if (ecc_pub == NULL || output == NULL || olen == NULL || key_buf == NULL) {
		return( MBEDTLS_ERR_ECDH_BAD_INPT_DATA );
	}

	ISP_CHECKBUSY();
	r = isp_compute_ecdh_encryptedkey(output, olen, *ecc_pub, key_buf);
	if (r != 0) {
		isp_clear(0);
=======
	if( ecc_pub == NULL || output == NULL || olen == NULL ) {
		return( MBEDTLS_ERR_ECDH_BAD_INPUT_DATA );
	}

	ISP_CHECKBUSY();
	r = isp_compute_ecdh_securekey(output, olen, *(struct sECC_KEY *)ecc_pub, key_index);
	if( r != 0 ) {
		isp_clear( 0 );
>>>>>>> ae4043bd6... external/mbedtls: Modify dependency of CONFIG_HW_ECDH_PARAM
		return( MBEDTLS_ERR_ECDH_HW_ACCEL_FAILED );
	}

	return( 0 );
}
#endif /* MBEDTLS_ECDH_COMPUTE_SHARED_ALT */



#endif /* MBEDTLS_ECDH_C */
