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
 *  The SHA-1 standard was published by NIST in 1993.
 *
 *  http://www.itl.nist.gov/fipspubs/fip180-1.htm
 */
#include <stdio.h>
#include <string.h>
#include "mbedtls/sha1.h"

#if defined( MBEDTLS_SHA1_ALT )

#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */

/* Implementation that should never be optimized out by the compiler */
static void dubhe_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = (unsigned char *) v;
    while ( n-- )
        *p++ = 0;
}

void dubhe_sha1_init( mbedtls_sha1_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha1_context ) );
}

void dubhe_sha1_free( mbedtls_sha1_context *ctx )
{
    if ( ctx == NULL )
        return;

    arm_ce_hash_free( ctx );
    dubhe_zeroize( ctx, sizeof( mbedtls_sha1_context ) );
}

void dubhe_sha1_clone( mbedtls_sha1_context *dst,
                       const mbedtls_sha1_context *src )
{
    int ret         = 0;

    if ( src == NULL || dst == NULL )
    {
        mbedtls_printf( "%s src dubhe ctx is NULL\n", __func__ );
        return;
    }

    ret = arm_ce_hash_clone( dst, (mbedtls_sha1_context *) src );
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed\n", __func__ );
        return;
    }
}

/*
 * SHA-1 context setup
 */
int dubhe_sha1_starts_ret( mbedtls_sha1_context *ctx )
{
    int ret = 0;

    if ( ctx == NULL )
    {
        return ( MBEDTLS_ERR_SHA1_CTX_NULL );
    }

    ret = arm_ce_hash_init( ctx );

    if ( ret < 0 )
    {
        return ( MBEDTLS_ERR_SHA1_CE_INIT );
    }

    ret = arm_ce_hash_start( ctx,
                             ARM_HASH_VECT_DEFAULT,
                             ARM_HASH_IV_IS_ADDR,
                             NULL,
                             0,
                             ARM_HASH_MODE_SHA1 );

    return ( ret );
}

#if !defined( MBEDTLS_DEPRECATED_REMOVED )
int dubhe_sha1_starts( mbedtls_sha1_context *ctx )
{
    return dubhe_sha1_starts_ret( ctx );
}
#endif

#if !defined( MBEDTLS_SHA1_PROCESS_ALT )
int dubhe_internal_sha1_process( mbedtls_sha1_context *ctx,
                                 const unsigned char data[64] )
{
    if ( NULL == ctx )
        return ( MBEDTLS_ERR_SHA1_CTX_NULL );

    if ( NULL == data )
    {
        return 0;
    }

    return ( arm_ce_hash_update( ctx, data, 64 ) );
}

#if !defined( MBEDTLS_DEPRECATED_REMOVED )
void dubhe_sha1_process( mbedtls_sha1_context *ctx,
                         const unsigned char data[64] )
{
    dubhe_internal_sha1_process( ctx, data );
}
#endif
#endif /* !MBEDTLS_SHA1_PROCESS_ALT */

/*
 * SHA-1 process buffer
 */
int dubhe_sha1_update_ret( mbedtls_sha1_context *ctx,
                           const unsigned char *input,
                           size_t ilen )
{
    if ( ctx == NULL )
    {
        mbedtls_printf( "%s dubhe ctx is NULL\n", __func__ );
        return ( MBEDTLS_ERR_SHA1_CTX_NULL );
    }

    if ( ilen == 0 )
        return ( 0 );
    return ( arm_ce_hash_update( ctx, input, ilen ) );
}

#if !defined( MBEDTLS_DEPRECATED_REMOVED )
int dubhe_sha1_update( mbedtls_sha1_context *ctx,
                        const unsigned char *input,
                        size_t ilen )
{
    return dubhe_sha1_update_ret( ctx, input, ilen );
}
#endif

/*
 * SHA-1 final digest
 */
int dubhe_sha1_finish_ret( mbedtls_sha1_context *ctx, unsigned char output[20] )
{
    if ( ctx != NULL )
        return ( arm_ce_hash_finish( ctx, output ) );
    else
        return ( MBEDTLS_ERR_SHA1_CTX_NULL );
}

#if !defined( MBEDTLS_DEPRECATED_REMOVED )
int dubhe_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] )
{
    return dubhe_sha1_finish_ret( ctx, output );
}
#endif

#endif /* !MBEDTLS_SHA1_ALT */