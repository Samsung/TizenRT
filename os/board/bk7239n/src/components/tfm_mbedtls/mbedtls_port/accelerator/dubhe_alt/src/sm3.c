#if !defined( MBEDTLS_CONFIG_FILE )
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined( MBEDTLS_SM3_C )
#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */

#if defined( ARM_CE_DUBHE_HASH )

#include <string.h>

#include "mbedtls/sm3.h"

static void mbedtls_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = (unsigned char *) v;
    while ( n-- )
        *p++ = 0;
}

void mbedtls_sm3_init( mbedtls_sm3_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sm3_context ) );
}

void mbedtls_sm3_clone( mbedtls_sm3_context *dst,
                        const mbedtls_sm3_context *src )
{
    int ret         = 0;

    if ( src == NULL || dst == NULL )
    {
        mbedtls_printf( "%s src dubhe ctx is NULL\n", __func__ );
        return;
    }

    ret = arm_ce_hash_clone( dst, (mbedtls_sm3_context *) src );
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed\n", __func__ );
        return;
    }
}

int mbedtls_sm3_starts_ret( mbedtls_sm3_context *ctx )
{
    int ret = 0;

    if ( ctx == NULL )
    {
        return ( MBEDTLS_ERR_SM3_CTX_NULL );
    }

    ret = arm_ce_hash_init( ctx );

    if ( ret < 0 )
    {
        return ( MBEDTLS_ERR_SM3_CTX_NULL );
    }

    ret = arm_ce_hash_start( ctx,
                             ARM_HASH_VECT_DEFAULT,
                             ARM_HASH_IV_IS_ADDR,
                             NULL,
                             0,
                             ARM_HASH_MODE_SM3 );

    return ( ret );
}

int mbedtls_sm3_update_ret( mbedtls_sm3_context *ctx,
                            const unsigned char *input,
                            size_t ilen )
{
    if ( ctx == NULL )
    {
        mbedtls_printf( "%s dubhe ctx is NULL\n", __func__ );
        return ( MBEDTLS_ERR_SM3_CTX_NULL );
    }

    if ( ilen == 0 )
        return ( 0 );

    return ( arm_ce_hash_update( ctx, input, ilen ) );
}

int mbedtls_sm3_finish_ret( mbedtls_sm3_context *ctx, unsigned char output[32] )
{
    if ( ctx == NULL )
    {
        mbedtls_printf( "%s dubhe ctx is NULL\n", __func__ );
        return ( MBEDTLS_ERR_SM3_CTX_NULL );
    }

    return ( arm_ce_hash_finish( ctx, output ) );
}

void mbedtls_sm3_free( mbedtls_sm3_context *ctx )
{
    if ( ctx == NULL )
    {
        return;
    }

    arm_ce_hash_free( ctx );
    mbedtls_zeroize( ctx, sizeof( mbedtls_sm3_context ) );
}

int mbedtls_internal_sm3_process( mbedtls_sm3_context *ctx,
                                  const unsigned char data[64] )
{
    if ( ctx != NULL )
        return ( arm_ce_hash_update( ctx, data, 64 ) );
    else
        return ( MBEDTLS_ERR_SM3_CTX_NULL );
}

/*
 * output = SM3( input buffer )
 */
int mbedtls_sm3_ret( const unsigned char *input,
                     size_t ilen,
                     unsigned char output[32] )
{
    int ret;
    mbedtls_sm3_context ctx;

    mbedtls_sm3_init( &ctx );

    if ( ( ret = mbedtls_sm3_starts_ret( &ctx ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_sm3_update_ret( &ctx, input, ilen ) ) != 0 )
        goto exit;

    if ( ( ret = mbedtls_sm3_finish_ret( &ctx, output ) ) != 0 )
        goto exit;

exit:
    mbedtls_sm3_free( &ctx );

    return ( ret );
}
#endif /* ARM_CE_DUBHE_HASH */
#endif /* MBEDTLS_SM3_C */
