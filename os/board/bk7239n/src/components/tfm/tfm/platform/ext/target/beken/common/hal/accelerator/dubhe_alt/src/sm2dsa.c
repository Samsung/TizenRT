/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#include "common.h"

#if !defined( MBEDTLS_CONFIG_FILE )
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined( MBEDTLS_SM2DSA_C )

#include "mbedtls/sm2dsa.h"
#include "mbedtls/asn1write.h"

#include <string.h>

#if defined( MBEDTLS_SM2DSA_DETERMINISTIC )
#include "mbedtls/hmac_drbg.h"
#endif

#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

/*
 * Derive a suitable integer for group grp from a buffer of length len
 * SEC1 4.1.3 step 5 aka SEC1 4.1.4 step 3
 */
static int derive_mpi( const mbedtls_ecp_group *grp,
                       mbedtls_mpi *x,
                       const unsigned char *buf,
                       size_t blen )
{
    int ret;
    size_t n_size   = ( grp->nbits + 7 ) / 8;
    size_t use_size = blen > n_size ? n_size : blen;

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( x, buf, use_size ) );
    if ( use_size * 8 > grp->nbits )
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( x, use_size * 8 - grp->nbits ) );

    /* While at it, reduce modulo N */
    if ( mbedtls_mpi_cmp_mpi( x, &grp->N ) >= 0 )
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( x, x, &grp->N ) );

cleanup:
    return ( ret );
}

/*
 * Compute SM2DSA signature of a hashed message
 */
int mbedtls_sm2dsa_sign( mbedtls_ecp_group *grp,
                         mbedtls_mpi *r,
                         mbedtls_mpi *s,
                         const mbedtls_mpi *d,
                         const unsigned char *buf,
                         size_t blen,
                         int ( *f_rng )( void *, unsigned char *, size_t ),
                         void *p_rng )
{
    int ret, key_tries, sign_tries;
#if defined( MBEDTLS_ENBALE_TIMING_ATTACK_MITIGATION )
    int blind_tries;
#endif
    mbedtls_ecp_point R;
    mbedtls_mpi k, e, t;

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if ( grp->N.p == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /* Make sure d is in range 1..n-1 */
    if ( mbedtls_mpi_cmp_int( d, 1 ) < 0
         || mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
        return ( MBEDTLS_ERR_ECP_INVALID_KEY );

    mbedtls_ecp_point_init( &R );
    mbedtls_mpi_init( &k );
    mbedtls_mpi_init( &e );
    mbedtls_mpi_init( &t );

    sign_tries = 0;
    do
    {
        /*
         * Steps 1-5: generate a suitable ephemeral keypair
         * and set r = e+xR mod n
         */
        MBEDTLS_MPI_CHK( derive_mpi( grp, &e, buf, blen ) );

        key_tries = 0;
        do
        {
            MBEDTLS_MPI_CHK(
                mbedtls_ecp_gen_keypair( grp, &k, &R, f_rng, p_rng ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( r, &e, &R.X ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( r, r, &grp->N ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &t, r, &k ) );

            if ( key_tries++ > 10 )
            {
                ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
                goto cleanup;
            }
        } while ( ( mbedtls_mpi_cmp_int( r, 0 ) == 0 )
                  || ( mbedtls_mpi_cmp_mpi( &t, &grp->N ) == 0 ) );

#if defined( MBEDTLS_ENBALE_TIMING_ATTACK_MITIGATION )
        /*
         * Generate a random value to blind inv_mod in next step,
         * avoiding a potential timing leak.
         */
        blind_tries = 0;
        do
        {
            size_t n_size = ( grp->nbits + 7 ) / 8;
            MBEDTLS_MPI_CHK(
                mbedtls_mpi_fill_random( &t, n_size, f_rng, p_rng ) );
            MBEDTLS_MPI_CHK(
                mbedtls_mpi_shift_r( &t, 8 * n_size - grp->nbits ) );

            /* See mbedtls_ecp_gen_keypair() */
            if ( ++blind_tries > 30 )
                return ( MBEDTLS_ERR_ECP_RANDOM_FAILED );
        } while ( mbedtls_mpi_cmp_int( &t, 1 ) < 0
                  || mbedtls_mpi_cmp_mpi( &t, &grp->N ) >= 0 );
        /*
         * Step 6: compute s = (k-r*d) / (1 + d) = t*(k-r*d) / ((1 + d)*t) mod n
         */
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &e, r, d, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &k, &k, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &e, &k, &e ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &e, &e, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &e, &e, &t, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_int( &k, d, 1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &k, &k, &t, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( s, &k, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( s, s, &e, &grp->N ) );
#else

        /*
         * Step 6: compute s = (k-r*d) / (1 + d) = t*(k-r*d) / ((1 + d)*t) mod n
         */
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &e, r, d, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &k, &k, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &e, &k, &e ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &e, &e, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_int( &k, d, 1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( s, &k, &grp->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( s, s, &e, &grp->N ) );
#endif
        if ( sign_tries++ > 10 )
        {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }
    } while ( mbedtls_mpi_cmp_int( s, 0 ) == 0 );

cleanup:
    mbedtls_ecp_point_free( &R );
    mbedtls_mpi_free( &k );
    mbedtls_mpi_free( &e );
    mbedtls_mpi_free( &t );

    return ( ret );
}

#if defined( MBEDTLS_SM2DSA_DETERMINISTIC )
/*
 * Deterministic signature wrapper
 */
int mbedtls_sm2dsa_sign_det( mbedtls_ecp_group *grp,
                             mbedtls_mpi *r,
                             mbedtls_mpi *s,
                             const mbedtls_mpi *d,
                             const unsigned char *buf,
                             size_t blen,
                             mbedtls_md_type_t md_alg )
{
    int ret;
    mbedtls_hmac_drbg_context rng_ctx;
    unsigned char data[2 * MBEDTLS_ECP_MAX_BYTES];
    size_t grp_len = ( grp->nbits + 7 ) / 8;
    const mbedtls_md_info_t *md_info;
    mbedtls_mpi h;

    if ( ( md_info = mbedtls_md_info_from_type( md_alg ) ) == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    mbedtls_mpi_init( &h );
    mbedtls_hmac_drbg_init( &rng_ctx );

    /* Use private key and message hash (reduced) to initialize HMAC_DRBG */
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, data, grp_len ) );
    MBEDTLS_MPI_CHK( derive_mpi( grp, &h, buf, blen ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &h, data + grp_len, grp_len ) );
    mbedtls_hmac_drbg_seed_buf( &rng_ctx, md_info, data, 2 * grp_len );
    ret = mbedtls_sm2dsa_sign(
        grp, r, s, d, buf, blen, mbedtls_hmac_drbg_random, &rng_ctx );

cleanup:
    mbedtls_hmac_drbg_free( &rng_ctx );
    mbedtls_mpi_free( &h );

    return ( ret );
}
#endif /* MBEDTLS_SM2DSA_DETERMINISTIC */

/*
 * Verify ECDSA signature of hashed message (SEC1 4.1.4)
 * Obviously, compared to SEC1 4.1.3, we skip step 2 (hash message)
 */
int mbedtls_sm2dsa_verify( mbedtls_ecp_group *grp,
                           const unsigned char *buf,
                           size_t blen,
                           const mbedtls_ecp_point *Q,
                           const mbedtls_mpi *r,
                           const mbedtls_mpi *s )
{
    int ret;
    mbedtls_mpi e, u1, u2;
    mbedtls_ecp_point R1;

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if ( grp->N.p == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    mbedtls_ecp_point_init( &R1 );
    mbedtls_mpi_init( &e );
    mbedtls_mpi_init( &u1 );
    mbedtls_mpi_init( &u2 );

    /*
     * Step 1: make sure r and s are in range 1..n-1
     */
    if ( mbedtls_mpi_cmp_int( r, 1 ) < 0
         || mbedtls_mpi_cmp_mpi( r, &grp->N ) >= 0
         || mbedtls_mpi_cmp_int( s, 1 ) < 0
         || mbedtls_mpi_cmp_mpi( s, &grp->N ) >= 0 )
    {
        mbedtls_printf(
            "%s %d MBEDTLS_ERR_ECP_VERIFY_FAILED\n", __func__, __LINE__ );
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

    /*
     * Additional precaution: make sure Q is valid
     */
    MBEDTLS_MPI_CHK( mbedtls_ecp_check_pubkey( grp, Q ) );

    /*
     * Step 3: derive MPI from hashed message
     */
    MBEDTLS_MPI_CHK( derive_mpi( grp, &e, buf, blen ) );

    /*
     * Step 4: t = (r+s)mod n
     */

    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &u2, r, s ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &u2, &u2, &grp->N ) );

    if ( mbedtls_mpi_cmp_int( &u2, 0 ) == 0 )
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

    /*
     * Step 5: R = s G + u2 Q
     *
     */
    MBEDTLS_MPI_CHK( mbedtls_ecp_muladd( grp, &R1, s, &grp->G, &u2, Q ) );
    /*
     * Step 6: ( xR + e ) mod n
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &u1, &R1.X, &e ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &u1, &u1, &grp->N ) );

    /*
     * Step 7: check if v (that is, u1) is equal to r
     */
    if ( mbedtls_mpi_cmp_mpi( &u1, r ) != 0 )
    {
        mbedtls_printf(
            "%s %d MBEDTLS_ERR_ECP_VERIFY_FAILED\n", __func__, __LINE__ );
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free( &R1 );
    mbedtls_mpi_free( &e );
    mbedtls_mpi_free( &u1 );
    mbedtls_mpi_free( &u2 );

    return ( ret );
}

/*
 * Convert a signature (given by context) to ASN.1
 */
static int sm2dsa_signature_to_asn1( const mbedtls_mpi *r,
                                     const mbedtls_mpi *s,
                                     unsigned char *sig,
                                     size_t *slen )
{
    int ret;
    unsigned char buf[MBEDTLS_SM2DSA_MAX_LEN];
    unsigned char *p = buf + sizeof( buf );
    size_t len       = 0;

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, s ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, r ) );

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_len( &p, buf, len ) );
    MBEDTLS_ASN1_CHK_ADD(
        len,
        mbedtls_asn1_write_tag(
            &p, buf, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE ) );

    memcpy( sig, p, len );
    *slen = len;

    return ( 0 );
}

/*
 * Compute and write signature
 */
int mbedtls_sm2dsa_write_signature( mbedtls_sm2dsa_context *ctx,
                                    mbedtls_md_type_t md_alg,
                                    const unsigned char *hash,
                                    size_t hlen,
                                    unsigned char *sig,
                                    size_t *slen,
                                    int ( *f_rng )( void *,
                                                    unsigned char *,
                                                    size_t ),
                                    void *p_rng )
{
    int ret;
    mbedtls_mpi r, s;

    mbedtls_mpi_init( &r );
    mbedtls_mpi_init( &s );

#if defined( MBEDTLS_SM2DSA_DETERMINISTIC )
    (void) f_rng;
    (void) p_rng;

    MBEDTLS_MPI_CHK( mbedtls_sm2dsa_sign_det(
        &ctx->grp, &r, &s, &ctx->d, hash, hlen, md_alg ) );
#else
    (void) md_alg;

    MBEDTLS_MPI_CHK( mbedtls_sm2dsa_sign(
        &ctx->grp, &r, &s, &ctx->d, hash, hlen, f_rng, p_rng ) );
#endif

    MBEDTLS_MPI_CHK( sm2dsa_signature_to_asn1( &r, &s, sig, slen ) );

cleanup:
    mbedtls_mpi_free( &r );
    mbedtls_mpi_free( &s );

    return ( ret );
}

/*
 * Read and check signature
 */
int mbedtls_sm2dsa_read_signature( mbedtls_sm2dsa_context *ctx,
                                   const unsigned char *hash,
                                   size_t hlen,
                                   const unsigned char *sig,
                                   size_t slen )
{
    int ret;
    unsigned char *p         = (unsigned char *) sig;
    const unsigned char *end = sig + slen;
    size_t len;
    mbedtls_mpi r, s;

    mbedtls_mpi_init( &r );
    mbedtls_mpi_init( &s );

    if ( ( ret = mbedtls_asn1_get_tag( &p,
                                       end,
                                       &len,
                                       MBEDTLS_ASN1_CONSTRUCTED
                                           | MBEDTLS_ASN1_SEQUENCE ) )
         != 0 )
    {
        ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    if ( p + len != end )
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA + MBEDTLS_ERR_ASN1_LENGTH_MISMATCH;
        goto cleanup;
    }

    if ( ( ret = mbedtls_asn1_get_mpi( &p, end, &r ) ) != 0
         || ( ret = mbedtls_asn1_get_mpi( &p, end, &s ) ) != 0 )
    {
        ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }
    // mbedtls_printf(" \n verify 1\n");
    if ( ( ret =
               mbedtls_sm2dsa_verify( &ctx->grp, hash, hlen, &ctx->Q, &r, &s ) )
         != 0 )
    {
        goto cleanup;
    }
    // mbedtls_printf(" verify 2\n");
    /* At this point we know that the buffer starts with a valid signature.
     * Return 0 if the buffer just contains the signature, and a specific
     * error code if the valid signature is followed by more data. */
    if ( p != end )
        ret = MBEDTLS_ERR_ECP_SIG_LEN_MISMATCH;

cleanup:
    mbedtls_mpi_free( &r );
    mbedtls_mpi_free( &s );

    return ( ret );
}

/*
 * Generate key pair
 */
int mbedtls_sm2dsa_genkey( mbedtls_sm2dsa_context *ctx,
                           mbedtls_ecp_group_id gid,
                           int ( *f_rng )( void *, unsigned char *, size_t ),
                           void *p_rng )
{
    return ( mbedtls_ecp_group_load( &ctx->grp, gid )
             || mbedtls_ecp_gen_keypair(
                 &ctx->grp, &ctx->d, &ctx->Q, f_rng, p_rng ) );
}

/*
 * Set context from an mbedtls_ecp_keypair
 */
int mbedtls_sm2dsa_from_keypair( mbedtls_sm2dsa_context *ctx,
                                 const mbedtls_ecp_keypair *key )
{
    int ret;

    if ( ( ret = mbedtls_ecp_group_copy( &ctx->grp, &key->grp ) ) != 0
         || ( ret = mbedtls_mpi_copy( &ctx->d, &key->d ) ) != 0
         || ( ret = mbedtls_ecp_copy( &ctx->Q, &key->Q ) ) != 0 )
    {
        mbedtls_sm2dsa_free( ctx );
    }

    return ( ret );
}

/*
 * Initialize context
 */
void mbedtls_sm2dsa_init( mbedtls_sm2dsa_context *ctx )
{
    mbedtls_ecp_keypair_init( ctx );
}

/*
 * Free context
 */
void mbedtls_sm2dsa_free( mbedtls_sm2dsa_context *ctx )
{
    mbedtls_ecp_keypair_free( ctx );
}

#endif /* MBEDTLS_SM2DSA_C */
