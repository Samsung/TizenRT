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

#if defined( MBEDTLS_SM2KEP_C )

#include "mbedtls/sm2kep.h"
#include "mbedtls/sm2_internal.h"

#include <string.h>

#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

#define ciL ( sizeof( mbedtls_mpi_uint ) ) /* chars in limb  */
#define biL ( ciL << 3 )                   /* bits  in limb  */
#define BITS_TO_LIMBS( i ) ( ( i ) / biL + ( ( i ) % biL != 0 ) )

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = v;
    while ( n-- )
        *p++ = 0;
}

/*
 * Generate public key: simple wrapper around mbedtls_ecp_gen_keypair
 */
int mbedtls_sm2kep_gen_public( mbedtls_ecp_group *grp,
                               mbedtls_mpi *d,
                               mbedtls_ecp_point *Q,
                               int ( *f_rng )( void *,
                                               unsigned char *,
                                               size_t ),
                               void *p_rng )
{
    return mbedtls_ecp_gen_keypair( grp, d, Q, f_rng, p_rng );
}

int mbedtls_sm2kep_compute_shared( mbedtls_ecp_group *grp,
                                   mbedtls_ecp_point *P,
                                   const mbedtls_ecp_point *R,
                                   const mbedtls_ecp_point *Rp,
                                   const mbedtls_ecp_point *QP,
                                   const mbedtls_mpi *d,
                                   const mbedtls_mpi *r,
                                   int ( *f_rng )( void *,
                                                   unsigned char *,
                                                   size_t ),
                                   void *p_rng )
{
    int ret;
    mbedtls_ecp_point P1;
    mbedtls_mpi t1, t2, u1, u2;
    mbedtls_mpi_uint val, wT, bNom, wNom; /*Sizes in words and bits  */

    if(grp->nbits < 64)
    {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    (void) f_rng;
    (void) p_rng;
    mbedtls_ecp_point_init( &P1 );
    mbedtls_mpi_init( &t1 );
    mbedtls_mpi_init( &t2 );
    mbedtls_mpi_init( &u1 );
    mbedtls_mpi_init( &u2 );

    MBEDTLS_MPI_CHK( mbedtls_mpi_grow( &t1, grp->N.n ) );
    wT   = ( grp->nbits + 1 ) / 2 - 1;
    wNom = BITS_TO_LIMBS( wT );
    bNom = wT % biL; /*remain bits*/
    if ( bNom )
    {
        val = 1UL << bNom;
    }
    else
    {
        wNom++;
        val = 1UL;
    }

    t1.p[wNom - 1] = val;

    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &t2, &t1, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_and_mpi( &u1, &R->X, &t2 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &u1, &u1, &t1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &u2, &u1, r, &grp->N ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &t2, &u2, d ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &t2, &t2, &grp->N ) );
    /*
     * Make sure Q is a valid pubkey before using it
     */

    MBEDTLS_MPI_CHK( mbedtls_ecp_check_pubkey( grp, QP ) );
    MBEDTLS_MPI_CHK( mbedtls_ecp_check_pubkey( grp, Rp ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &u2, &t1, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_and_mpi( &u1, &Rp->X, &u2 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &u2, &u1, &t1 ) );
    /*P = h*t2(Q + u2*R1) = h*t2*QP + h*t2*u2*RP, the h == 1 here*/
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &u2, &u2, &t2, &grp->N ) );
    MBEDTLS_MPI_CHK( mbedtls_ecp_muladd( grp, P, &t2, QP, &u2, Rp ) );

    if ( mbedtls_ecp_is_zero( P ) )
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free( &P1 );
    mbedtls_mpi_free( &t1 );
    mbedtls_mpi_free( &t2 );
    mbedtls_mpi_free( &u1 );
    mbedtls_mpi_free( &u2 );

    return ( ret );
}

static int mbedtls_sm2kep_compute_shared_key( const mbedtls_md_info_t *md_info,
                                              size_t modsize,             /*in*/
                                              const mbedtls_ecp_point *P, /*in*/
                                              unsigned char *ZA,          /*in*/
                                              size_t ZAlen,               /*in*/
                                              unsigned char *ZB,          /*in*/
                                              size_t ZBlen,               /*in*/
                                              size_t Kresultlen,          /*in*/
                                              unsigned char *Kresult /*out*/ )
{
    int ret;
    unsigned char *DigestContext;
    DigestContext = (unsigned char *) mbedtls_calloc(
        modsize * 2 + ZAlen + ZBlen, sizeof( unsigned char ) );
    if ( DigestContext == NULL )
    {
        mbedtls_printf( "%s ERROR mbedtls_calloc failed !!!\n", __func__ );
        return MBEDTLS_ERR_ECP_ALLOC_FAILED;
    }
    if ( ( ret = mbedtls_mpi_write_binary( &P->X, DigestContext, modsize ) )
         != 0 )
        goto exit;

    if ( ( ret = mbedtls_mpi_write_binary(
               &P->Y, DigestContext + modsize, modsize ) )
         != 0 )
        goto exit;

    memcpy( DigestContext + 2 * modsize, ZA, ZAlen );
    memcpy( DigestContext + 2 * modsize + ZAlen, ZB, ZBlen );
    ret = mbedtls_sm2_kdf( md_info,
                           DigestContext,
                           2 * modsize + ZBlen + ZAlen,
                           Kresultlen,
                           Kresult );
    mbedtls_zeroize( DigestContext, 2 * modsize + ZBlen + ZAlen );
exit:
    mbedtls_free( DigestContext );
    return ret;
}

static int mbedtls_sm2kep_calc_hash( const mbedtls_md_info_t *md_info,
                                     size_t modsize,              /*in*/
                                     const mbedtls_ecp_point *P,  /*in*/
                                     unsigned char *ZA,           /*in*/
                                     size_t ZAlen,                /*in*/
                                     unsigned char *ZB,           /*in*/
                                     size_t ZBlen,                /*in*/
                                     const mbedtls_ecp_point *RA, /*in*/
                                     const mbedtls_ecp_point *RB, /*in*/
                                     unsigned char *Sresult /*out*/ )
{
    int ret;
    mbedtls_md_context_t md_ctx;
    unsigned char buf[MBEDTLS_ECP_MAX_PT_LEN];

    mbedtls_md_init( &md_ctx );

    if ( ( ret = mbedtls_md_setup( &md_ctx, md_info, 0 ) ) != 0 )
        return ( ret );

    if ( ( ret = mbedtls_md_starts( &md_ctx ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_mpi_write_binary( &P->X, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, ZA, ZAlen ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, ZB, ZBlen ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_mpi_write_binary( &RA->X, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_mpi_write_binary( &RA->Y, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_mpi_write_binary( &RB->X, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_mpi_write_binary( &RB->Y, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_finish( &md_ctx, Sresult ) ) != 0 )
        goto exit;

exit:
    mbedtls_md_free( &md_ctx );
    return ret;
}

static int
mbedtls_sm2kep_compute_optional_sum( const mbedtls_md_info_t *md_info,
                                     unsigned char sm2kep_flag,  /*in*/
                                     size_t modsize,             /*in*/
                                     const mbedtls_ecp_point *P, /*in*/
                                     unsigned char *hash,        /*in*/
                                     size_t len,                 /*in*/
                                     unsigned char *Sresult /*out*/ )
{
    int ret;
    mbedtls_md_context_t md_ctx;
    unsigned char buf[MBEDTLS_ECP_MAX_PT_LEN];

    mbedtls_md_init( &md_ctx );

    if ( ( ret = mbedtls_md_setup( &md_ctx, md_info, 0 ) ) != 0 )
        return ( ret );

    if ( ( ret = mbedtls_md_starts( &md_ctx ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update(
               &md_ctx, &sm2kep_flag, sizeof( unsigned char ) ) )
         != 0 )
        goto exit;

    if ( ( ret = mbedtls_mpi_write_binary( &P->Y, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, buf, modsize ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_update( &md_ctx, hash, len ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_md_finish( &md_ctx, Sresult ) ) != 0 )
        goto exit;

exit:
    mbedtls_md_free( &md_ctx );
    return ret;
}

/*
 * Initialize context
 */
void mbedtls_sm2kep_init( mbedtls_sm2kep_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sm2kep_context ) );
    mbedtls_ecp_group_init( &ctx->grp );
    mbedtls_mpi_init( &ctx->d );
    mbedtls_mpi_init( &ctx->r );
    mbedtls_ecp_point_init( &ctx->Q );
    mbedtls_ecp_point_init( &ctx->Qp );
    mbedtls_ecp_point_init( &ctx->R );
    mbedtls_ecp_point_init( &ctx->Rp );
    mbedtls_ecp_point_init( &ctx->P );
}

/*
 * Free context
 */
void mbedtls_sm2kep_free( mbedtls_sm2kep_context *ctx )
{
    if ( ctx == NULL )
        return;

    mbedtls_ecp_group_free( &ctx->grp );
    mbedtls_ecp_point_free( &ctx->Q );
    mbedtls_ecp_point_free( &ctx->Qp );
    mbedtls_ecp_point_free( &ctx->R );
    mbedtls_ecp_point_free( &ctx->Rp );
    mbedtls_ecp_point_free( &ctx->P );
    mbedtls_mpi_free( &ctx->d );
    mbedtls_mpi_free( &ctx->r );
}

/*
 * Setup and write the ServerKeyExhange parameters (RFC 4492)
 *      struct {
 *          ECParameters    curve_params;
 *          ECPoint         public;
 *          ECPoint         temporary public;
 *      } ServerECDHParams;
 */
int mbedtls_sm2kep_make_params( mbedtls_sm2kep_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int ( *f_rng )( void *,
                                                unsigned char *,
                                                size_t ),
                                void *p_rng )
{
    int ret;
    size_t grp_len, pt_len;

    if ( ctx == NULL || ctx->grp.pbits == 0 )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( ( ret = mbedtls_sm2kep_gen_public(
               &ctx->grp, &ctx->d, &ctx->Q, f_rng, p_rng ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_sm2kep_gen_public(
               &ctx->grp, &ctx->r, &ctx->R, f_rng, p_rng ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_ecp_tls_write_group( &ctx->grp, &grp_len, buf, blen ) )
         != 0 )
        return ( ret );

    buf += grp_len;
    blen -= grp_len;

    if ( ( ret = mbedtls_ecp_tls_write_point(
               &ctx->grp, &ctx->Q, ctx->point_format, &pt_len, buf, blen ) )
         != 0 )
        return ( ret );

    *olen = grp_len + pt_len;
    buf += pt_len;
    blen -= pt_len;
    if ( ( ret = mbedtls_ecp_tls_write_point(
               &ctx->grp, &ctx->R, ctx->point_format, &pt_len, buf, blen ) )
         != 0 )
        return ( ret );

    *olen = *olen + pt_len;
    return ( 0 );
}

/*
 * Read the ServerKeyExhange parameters (RFC 4492)
 *      struct {
 *          ECParameters    curve_params;
 *          ECPoint         public;
 *          ECPoint         temporary public;
 *      } ServerECDHParams;
 */
int mbedtls_sm2kep_read_params( mbedtls_sm2kep_context *ctx,
                                const unsigned char **buf,
                                const unsigned char *end )
{
    int ret;
    if ( ( ret = mbedtls_ecp_tls_read_group( &ctx->grp, buf, end - *buf ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_ecp_tls_read_point(
               &ctx->grp, &ctx->Qp, buf, end - *buf ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_ecp_tls_read_point(
               &ctx->grp, &ctx->Rp, buf, end - *buf ) )
         != 0 )
        return ( ret );

    return ( 0 );
}

/*
 * Setup and export the client public value (public and temporary public)
 */
int mbedtls_sm2kep_make_public( mbedtls_sm2kep_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int ( *f_rng )( void *,
                                                unsigned char *,
                                                size_t ),
                                void *p_rng )
{
    int ret;
    size_t pt_len;

    if ( ctx == NULL || ctx->grp.pbits == 0 )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( ( ret = mbedtls_sm2kep_gen_public(
               &ctx->grp, &ctx->d, &ctx->Q, f_rng, p_rng ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_sm2kep_gen_public(
               &ctx->grp, &ctx->r, &ctx->R, f_rng, p_rng ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_ecp_tls_write_point(
               &ctx->grp, &ctx->Q, ctx->point_format, &pt_len, buf, blen ) )
         != 0 )
        return ( ret );

    *olen = pt_len;
    buf += pt_len;
    blen -= pt_len;
    if ( ( ret = mbedtls_ecp_tls_write_point(
               &ctx->grp, &ctx->R, ctx->point_format, &pt_len, buf, blen ) )
         != 0 )
        return ( ret );

    *olen = *olen + pt_len;
    return ( 0 );
}

/*
 * Parse and import the client's public value and temporary public
 */
int mbedtls_sm2kep_read_public( mbedtls_sm2kep_context *ctx,
                                const unsigned char **buf,
                                const unsigned char *end )
{
    int ret;
    if ( ( ret = mbedtls_ecp_tls_read_point(
               &ctx->grp, &ctx->Qp, buf, end - *buf ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_ecp_check_pubkey( &ctx->grp, &ctx->Qp ) ) != 0 )
        return ( ret );

    if ( ( ret = mbedtls_ecp_tls_read_point(
               &ctx->grp, &ctx->Rp, buf, end - *buf ) )
         != 0 )
        return ( ret );

    return ( 0 );
}

/*
 * Derive and export the shared secret
 */
int mbedtls_sm2kep_calc_secret( mbedtls_sm2kep_context *ctx,
                                mbedtls_md_type_t md_alg,
                                unsigned char *buf,
                                size_t blen,
                                unsigned char *ZA, /*in*/
                                size_t ZAlen,      /*in*/
                                unsigned char *ZB, /*in*/
                                size_t ZBlen,      /*in*/
                                int ( *f_rng )( void *,
                                                unsigned char *,
                                                size_t ),
                                void *p_rng )
{
    int ret;
    const mbedtls_md_info_t *md_info;
    size_t olen = 0;

    if ( ctx == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( ( md_info = mbedtls_md_info_from_type( md_alg ) ) == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( ( ret = mbedtls_sm2kep_compute_shared( &ctx->grp,
                                                &ctx->P,
                                                &ctx->R,
                                                &ctx->Rp,
                                                &ctx->Qp,
                                                &ctx->d,
                                                &ctx->r,
                                                f_rng,
                                                p_rng ) )
         != 0 )
    {
        goto exit;
    }

    olen = ctx->grp.pbits / 8 + ( ( ctx->grp.pbits % 8 ) != 0 );

    if ( ( ret = mbedtls_sm2kep_compute_shared_key(
               md_info, olen, &ctx->P, ZA, ZAlen, ZB, ZBlen, blen, buf ) )
         != 0 )
    {
        goto exit;
    }
exit:
    return ret;
}

int mbedtls_sm2kep_calc_checksum( mbedtls_sm2kep_context *ctx,
                                  mbedtls_md_type_t md_alg,
                                  mbedtls_sm2kep_side side,
                                  unsigned char *SI,
                                  unsigned char *SR,
                                  unsigned char *ZA, /*in*/
                                  size_t ZAlen,      /*in*/
                                  unsigned char *ZB, /*in*/
                                  size_t ZBlen /*in*/ )
{
    int ret;
    const mbedtls_md_info_t *md_info;
    unsigned char hash[MBEDTLS_MD_MAX_SIZE] = {0};
    size_t mlen = 0, mod_size = 0;

    if ( ctx == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( side != MBEDTLS_SM2KEP_INITIATOR && side != MBEDTLS_SM2KEP_RESPONDER )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( ( md_info = mbedtls_md_info_from_type( md_alg ) ) == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    mod_size = ctx->grp.pbits / 8 + ( ( ctx->grp.pbits % 8 ) != 0 );
    mlen     = mbedtls_md_get_size( md_info );
    if ( side == MBEDTLS_SM2KEP_INITIATOR )
    {
        if ( ( ret = mbedtls_sm2kep_calc_hash( md_info,
                                               mod_size,
                                               &ctx->P,
                                               ZA,
                                               ZAlen,
                                               ZB,
                                               ZBlen,
                                               &ctx->R,
                                               &ctx->Rp,
                                               hash ) )
             != 0 )
            return ( ret );
    }
    else
    {
        if ( ( ret = mbedtls_sm2kep_calc_hash( md_info,
                                               mod_size,
                                               &ctx->P,
                                               ZA,
                                               ZAlen,
                                               ZB,
                                               ZBlen,
                                               &ctx->Rp,
                                               &ctx->R,
                                               hash ) )
             != 0 )
            return ( ret );
    }

    if ( ( ret = mbedtls_sm2kep_compute_optional_sum( md_info,
                                                      MBEDTLS_SM2KEP_INITIATOR,
                                                      mod_size,
                                                      &ctx->P,
                                                      hash,
                                                      mlen,
                                                      SI ) )
         != 0 )
        return ( ret );

    if ( ( ret = mbedtls_sm2kep_compute_optional_sum( md_info,
                                                      MBEDTLS_SM2KEP_RESPONDER,
                                                      mod_size,
                                                      &ctx->P,
                                                      hash,
                                                      mlen,
                                                      SR ) )
         != 0 )
        return ( ret );

    return ret;
}

#endif /* MBEDTLS_SM2KEP_C */
