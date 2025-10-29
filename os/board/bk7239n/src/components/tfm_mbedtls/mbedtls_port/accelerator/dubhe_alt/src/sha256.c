/**
 * @if copyright_display
 *      Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
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
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

#include <stdio.h>
#include <string.h>
#include "mbedtls/sha256.h"

#if defined( MBEDTLS_SHA256_ALT )

#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */

/* Implementation that should never be optimized out by the compiler */
static void dubhe_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = v;
    while ( n-- )
        *p++ = 0;
}

void dubhe_sha256_init( mbedtls_sha256_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
}

void dubhe_sha256_free( mbedtls_sha256_context *ctx )
{
    if ( ctx != NULL )
    {
        arm_ce_hash_free( ctx );
        dubhe_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
    }
}

void dubhe_sha256_clone( mbedtls_sha256_context *dst,
                         const mbedtls_sha256_context *src )
{
    int ret         = 0;

    if ( src == NULL || NULL == dst )
    {
        mbedtls_printf( "%s src dubhe ctx is NULL\n", __func__ );
        return;
    }

    ret = arm_ce_hash_clone( dst, (mbedtls_sha256_context *) src );

    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed\n", __func__ );
        return;
    }
}

/*
 * SHA-256 context setup
 */
int dubhe_sha256_starts_ret( mbedtls_sha256_context *ctx, int is224 )
{
    int ret = 0;
    arm_ce_hash_mode_t hash_mode;

    ret = arm_ce_hash_init( ctx );
    if ( ret < 0 )
    {
        return ( MBEDTLS_ERR_SHA256_CE_INIT );
    }

    if ( is224 == 1 )
        hash_mode = ARM_HASH_MODE_SHA224;
    else
        hash_mode = ARM_HASH_MODE_SHA256;

    return ( arm_ce_hash_start(
        ctx, ARM_HASH_VECT_DEFAULT, ARM_HASH_IV_IS_ADDR, NULL, 0, hash_mode ) );
}

#if !defined( MBEDTLS_DEPRECATED_REMOVED )
int dubhe_sha256_starts( mbedtls_sha256_context *ctx, int is224 )
{
    return dubhe_sha256_starts_ret( ctx, is224 );
}
#endif

int dubhe_internal_sha256_process( mbedtls_sha256_context *ctx,
                                   const unsigned char data[64] )
{

    if ( NULL == ctx )
    {
        return MBEDTLS_ERR_SHA256_CTX_NULL;
    }

    if ( NULL == data )
    {
        return 0;
    }

    return ( arm_ce_hash_update( ctx, data, 64 ) );
}
#if !defined( MBEDTLS_SHA256_PROCESS_ALT )
#if !defined( MBEDTLS_DEPRECATED_REMOVED )
void dubhe_sha256_process( mbedtls_sha256_context *ctx,
                           const unsigned char data[64] )
{
    if ( dubhe_sha256_starts_ret( ctx, 1 ) != 0 )
        return;
    dubhe_internal_sha256_process( ctx, data );
}
#endif /* !MBEDTLS_SHA256_PROCESS_ALT */
#endif

/*
 * SHA-256 process buffer
 */
int dubhe_sha256_update_ret( mbedtls_sha256_context *ctx,
                             const unsigned char *input,
                             size_t ilen )
{
    int ret;

    if ( NULL == ctx )
    {
        return MBEDTLS_ERR_SHA256_CTX_NULL;
    }
    ret = arm_ce_hash_update( ctx, input, ilen );

    return ( ret );
}

#if !defined( MBEDTLS_DEPRECATED_REMOVED )
int dubhe_sha256_update( mbedtls_sha256_context *ctx,
                          const unsigned char *input,
                          size_t ilen )
{
    return dubhe_sha256_update_ret( ctx, input, ilen );
}
#endif

/*
 * SHA-256 final digest
 */
int dubhe_sha256_finish_ret( mbedtls_sha256_context *ctx,
                             unsigned char output[32] )
{
    int ret;

    if ( NULL == ctx )
    {
        return MBEDTLS_ERR_SHA256_CTX_NULL;
    }
    ret = arm_ce_hash_finish( ctx, output );

    return ( ret );
}

#if !defined( MBEDTLS_DEPRECATED_REMOVED )
int dubhe_sha256_finish( mbedtls_sha256_context *ctx,
                          unsigned char output[32] )
{
    return dubhe_sha256_finish_ret( ctx, output );
}
#endif

#endif /* MBEDTLS_SHA256_ALT */
