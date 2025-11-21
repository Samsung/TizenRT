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

#if defined( MBEDTLS_SM2PKE_C )

#include "mbedtls/sm2pke.h"
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
int mbedtls_sm2pke_gen_public( mbedtls_ecp_group *grp,
                               mbedtls_mpi *d,
                               mbedtls_ecp_point *Q,
                               int ( *f_rng )( void *,
                                               unsigned char *,
                                               size_t ),
                               void *p_rng )
{
    return mbedtls_ecp_gen_keypair( grp, d, Q, f_rng, p_rng );
}

/*
 * Initialize context
 */
void mbedtls_sm2pke_init( mbedtls_sm2pke_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sm2pke_context ) );
    mbedtls_ecp_group_init( &ctx->grp );
    mbedtls_mpi_init( &ctx->d );
    mbedtls_ecp_point_init( &ctx->Q );
    mbedtls_ecp_point_init( &ctx->Qp );
}

/*
 * Free context
 */
void mbedtls_sm2pke_free( mbedtls_sm2pke_context *ctx )
{
    if ( ctx == NULL )
        return;

    mbedtls_ecp_group_free( &ctx->grp );
    mbedtls_ecp_point_free( &ctx->Q );
    mbedtls_ecp_point_free( &ctx->Qp );
    mbedtls_mpi_free( &ctx->d );
}

/*
 * Setup and write the ServerKeyExhange parameters (RFC 4492)
 *      struct {
 *          ECParameters    curve_params;
 *          ECPoint         public;
 *      } ServerECDHParams;
 */
int mbedtls_sm2pke_make_params( mbedtls_sm2pke_context *ctx,
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

    if ( ( ret = mbedtls_sm2pke_gen_public(
               &ctx->grp, &ctx->d, &ctx->Q, f_rng, p_rng ) )
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
    return ( 0 );
}

/*
 * Read the ServerKeyExhange parameters (RFC 4492)
 *      struct {
 *          ECParameters    curve_params;
 *          ECPoint         public;
 *      } ServerECDHParams;
 */
int mbedtls_sm2pke_read_params( mbedtls_sm2pke_context *ctx,
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

    return ( 0 );
}

int mbedtls_sm2pke_encrypt( mbedtls_sm2pke_context *ctx,
                            mbedtls_md_type_t md_alg,
                            unsigned char *output,
                            size_t *olen,
                            unsigned char *input, /*in*/
                            size_t ilen,          /*in*/
                            int ( *f_rng )( void *, unsigned char *, size_t ),
                            void *p_rng )
{
    int ret;
    const mbedtls_md_info_t *md_info;
    unsigned char *tmpbuf1 = NULL, *tmpbuf2 = NULL;
    size_t plen = 0, mlen = 0;
    size_t pt_len, i;
    mbedtls_mpi k;
    mbedtls_ecp_point C1, R;
    unsigned char *c2, *c3;

    if ( ctx == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( f_rng == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( ( md_info = mbedtls_md_info_from_type( md_alg ) ) == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    plen = ctx->grp.pbits / 8 + ( ( ctx->grp.pbits % 8 ) != 0 );
    mlen = mbedtls_md_get_size( md_info );

    if ( ctx->point_format == MBEDTLS_ECP_PF_UNCOMPRESSED )
    {
        if ( *olen < 2 * plen + ilen + mlen + 2 )
        {
            *olen = 2 * plen + ilen + mlen + 2;
            return ( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );
        }
    }
    else if ( ctx->point_format == MBEDTLS_ECP_PF_COMPRESSED )
    {
        if ( *olen < plen + ilen + mlen + 2 )
        {
            *olen = plen + ilen + mlen + 2;
            return ( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );
        }
    }

    tmpbuf1 = (unsigned char *) mbedtls_calloc( 2 * plen + ilen,
                                                sizeof( unsigned char ) );
    if ( tmpbuf1 == NULL )
    {
        mbedtls_printf( "%s ERROR mbedtls_calloc failed !!!\n", __func__ );
        return MBEDTLS_ERR_ECP_ALLOC_FAILED;
    }

    tmpbuf2 =
        (unsigned char *) mbedtls_calloc( 2 * plen, sizeof( unsigned char ) );
    if ( tmpbuf2 == NULL )
    {
        mbedtls_printf( "%s ERROR mbedtls_calloc failed !!!\n", __func__ );
        ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
        goto cleanup1;
    }

    mbedtls_mpi_init( &k );
    mbedtls_ecp_point_init( &C1 );
    mbedtls_ecp_point_init( &R );

    if ( ( ret = mbedtls_sm2pke_gen_public( &ctx->grp, &k, &C1, f_rng, p_rng ) )
         != 0 )
        goto cleanup;

    if ( ( ret = mbedtls_ecp_point_write_binary(
               &ctx->grp, &C1, ctx->point_format, &pt_len, output, *olen ) )
         != 0 )
        goto cleanup;

    c2 = output + pt_len;
    if ( ( ret = mbedtls_ecp_mul( &ctx->grp, &R, &k, &ctx->Qp, f_rng, p_rng ) )
         != 0 )
        goto cleanup;
    if ( ( ret = mbedtls_mpi_write_binary( &R.X, tmpbuf2, plen ) ) != 0 )
        goto cleanup;

    if ( ( ret = mbedtls_mpi_write_binary( &R.Y, tmpbuf2 + plen, plen ) ) != 0 )
        goto cleanup;

    if ( ( ret = mbedtls_sm2_kdf( md_info, tmpbuf2, 2 * plen, ilen, tmpbuf1 ) )
         != 0 )
        goto cleanup;

    /*need to check if tmpbuf1 is all 0*/

    for ( i = 0; i < ilen; i++ )
    {
        c2[i] = input[i] ^ tmpbuf1[i];
    }
    mbedtls_zeroize( tmpbuf1, ilen );

    if ( ( ret = mbedtls_mpi_write_binary( &R.X, tmpbuf1, plen ) ) != 0 )
        goto cleanup;

    memcpy( tmpbuf1 + plen, input, ilen );

    if ( ( ret = mbedtls_mpi_write_binary( &R.Y, tmpbuf1 + plen + ilen, plen ) )
         != 0 )
        goto cleanup;

    c3 = c2 + ilen;

    if ( ( ret = mbedtls_md( md_info, tmpbuf1, 2 * plen + ilen, c3 ) ) != 0 )
        goto cleanup;

    *olen = pt_len + ilen + mlen;
cleanup:
    mbedtls_mpi_free( &k );
    mbedtls_ecp_point_free( &C1 );
    mbedtls_ecp_point_free( &R );
cleanup1:
    if ( ret )
    {
        mbedtls_zeroize( output, *olen );
    }
    if ( tmpbuf2 != NULL )
    {
        mbedtls_zeroize( tmpbuf2, 2 * plen );
        mbedtls_free( tmpbuf2 );
    }
    if ( tmpbuf1 != NULL )
    {
        mbedtls_zeroize( tmpbuf1, 2 * plen + ilen );
        mbedtls_free( tmpbuf1 );
    }
    return ret;
}

int mbedtls_sm2pke_decrypt( mbedtls_sm2pke_context *ctx,
                            mbedtls_md_type_t md_alg,
                            unsigned char *output,
                            size_t *olen,
                            unsigned char *input, /*in*/
                            size_t ilen,          /*in*/
                            int ( *f_rng )( void *, unsigned char *, size_t ),
                            void *p_rng )
{
    int ret;
    const mbedtls_md_info_t *md_info;
    unsigned char *tmpbuf1 = NULL, *tmpbuf2 = NULL;
    size_t plen = 0, mlen = 0;
    size_t pt_len, t_olen, i;
    mbedtls_ecp_point C1, R;
    const unsigned char *c1;
    unsigned char *c2, *c3;

    if ( ctx == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if ( ( md_info = mbedtls_md_info_from_type( md_alg ) ) == NULL )
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    plen   = ctx->grp.pbits / 8 + ( ( ctx->grp.pbits % 8 ) != 0 );
    mlen   = mbedtls_md_get_size( md_info );
    pt_len = 2 * plen + 1;

    if ( ilen <= mlen + pt_len )
    {
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    }

    tmpbuf1 = (unsigned char *) mbedtls_calloc( ilen, sizeof( unsigned char ) );
    if ( tmpbuf1 == NULL )
    {
        mbedtls_printf( "%s ERROR mbedtls_calloc failed !!!\n", __func__ );
        return MBEDTLS_ERR_ECP_ALLOC_FAILED;
    }
    tmpbuf2 = (unsigned char *) mbedtls_calloc( ilen, sizeof( unsigned char ) );
    if ( tmpbuf2 == NULL )
    {
        mbedtls_printf( "%s ERROR mbedtls_calloc failed !!!\n", __func__ );
        ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
        goto cleanup1;
    }
    mbedtls_ecp_point_init( &C1 );
    mbedtls_ecp_point_init( &R );

    c1 = input;
    if ( ( ret = mbedtls_ecp_point_read_binary( &ctx->grp, &C1, c1, pt_len ) )
         != 0 )
        goto cleanup;

    mbedtls_zeroize( tmpbuf1, ilen );

    if ( ( ret = mbedtls_ecp_check_pubkey( &ctx->grp, &C1 ) ) != 0 )
        goto cleanup;

    c2 = input + pt_len;

    if ( *olen + pt_len + mlen < ilen )
    {
        *olen = ilen - pt_len - mlen;
        ret   = MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
        goto cleanup;
    }
    t_olen = ilen - pt_len - mlen;

    if ( ( ret = mbedtls_ecp_mul( &ctx->grp, &R, &ctx->d, &C1, f_rng, p_rng ) )
         != 0 )
        goto cleanup;
    if ( ( ret = mbedtls_mpi_write_binary( &R.X, tmpbuf2, plen ) ) != 0 )
        goto cleanup;

    if ( ( ret = mbedtls_mpi_write_binary( &R.Y, tmpbuf2 + plen, plen ) ) != 0 )
        goto cleanup;

    if ( ( ret =
               mbedtls_sm2_kdf( md_info, tmpbuf2, 2 * plen, t_olen, tmpbuf1 ) )
         != 0 )
        goto cleanup;

    /*need to check if tmpbuf1 is all 0*/

    for ( i = 0; i < t_olen; i++ )
    {
        output[i] = c2[i] ^ tmpbuf1[i];
    }
    mbedtls_zeroize( tmpbuf1, ilen );
    mbedtls_zeroize( tmpbuf2, ilen );
    if ( ( ret = mbedtls_mpi_write_binary( &R.X, tmpbuf2, plen ) ) != 0 )
        goto cleanup;

    memcpy( tmpbuf2 + plen, output, t_olen );

    if ( ( ret =
               mbedtls_mpi_write_binary( &R.Y, tmpbuf2 + plen + t_olen, plen ) )
         != 0 )
        goto cleanup;

    if ( ( ret = mbedtls_md( md_info, tmpbuf2, 2 * plen + t_olen, tmpbuf1 ) )
         != 0 )
        goto cleanup;

    c3 = c2 + t_olen;
    if ( ( ret = memcmp( c3, tmpbuf1, mlen ) ) != 0 )
    {
        ret = MBEDTLS_ERR_ECP_IN_PROGRESS;
        goto cleanup;
    }

    *olen = t_olen;
cleanup:
    mbedtls_ecp_point_free( &C1 );
    mbedtls_ecp_point_free( &R );
cleanup1:
    if ( ret )
    {
        mbedtls_zeroize( output, *olen );
    }
    if ( tmpbuf2 != NULL )
    {
        mbedtls_zeroize( tmpbuf2, ilen );
        mbedtls_free( tmpbuf2 );
    }
    if ( tmpbuf1 != NULL )
    {
        mbedtls_zeroize( tmpbuf1, ilen );
        mbedtls_free( tmpbuf1 );
    }
    return ret;
}

#endif /* MBEDTLS_SM2PKE_C */
