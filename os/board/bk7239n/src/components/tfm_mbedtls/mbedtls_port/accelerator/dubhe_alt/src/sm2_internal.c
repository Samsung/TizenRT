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

#if !defined( MBEDTLS_CONFIG_FILE )
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined( MBEDTLS_SM2PKE_C ) || defined( MBEDTLS_SM2KEP_C )

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

#define SM2_KDF_COUNTER_SIZE ( 4 )

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = v;
    while ( n-- )
        *p++ = 0;
}

int mbedtls_sm2_kdf( const mbedtls_md_info_t *md_info,
                     const unsigned char *cdata,
                     unsigned int datalen,
                     unsigned int keylen,
                     unsigned char *keydata )
{
    int ret;
    unsigned char *pData;

    unsigned char cdgst[MBEDTLS_MD_MAX_SIZE] = {0};  // Digest
    unsigned char cCnt[SM2_KDF_COUNTER_SIZE] = {
        0};                        // counter in memory representation
    unsigned int nCnt  = 1;        // counter
    unsigned int nDgst = 0, mlen;  // digest length

    unsigned int nTimes =
        0;  //(keylen+31)/32; //The number of times you need to compute
    unsigned int i = 0;

    if ( cdata == NULL || datalen == 0 || keylen == 0 || keydata == NULL
         || md_info == NULL )
    {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    pData = (unsigned char *) mbedtls_calloc( datalen + SM2_KDF_COUNTER_SIZE,
                                              sizeof( unsigned char ) );
    if ( pData == NULL )
    {
        mbedtls_printf( "%s ERROR mbedtls_calloc failed !!!\n", __func__ );
        return MBEDTLS_ERR_ECP_ALLOC_FAILED;
    }

    mlen   = mbedtls_md_get_size( md_info );
    nDgst  = mlen;
    nTimes = ( keylen + mlen - 1 ) / mlen;
    memcpy( pData, cdata, datalen );
    for ( i = 0; i < nTimes; i++ )
    {
        // cCnt
        {
            cCnt[0] = ( nCnt >> 24 ) & 0xFF;
            cCnt[1] = ( nCnt >> 16 ) & 0xFF;
            cCnt[2] = ( nCnt >> 8 ) & 0xFF;
            cCnt[3] = (nCnt) &0xFF;
        }
        memcpy( pData + datalen, cCnt, SM2_KDF_COUNTER_SIZE );
        if ( ( ret = mbedtls_md(
                   md_info, pData, datalen + SM2_KDF_COUNTER_SIZE, cdgst ) )
             != 0 )
            goto exit;

        if ( i == nTimes - 1 )  // the last compute, According to keylen / mlen,
                                // the value of the summary is truncated
        {
            if ( keylen % mlen != 0 )
            {
                nDgst = keylen % mlen;
            }
        }
        memcpy( keydata + mlen * i, cdgst, nDgst );

        nCnt++;
    }
exit:
    mbedtls_zeroize( pData, datalen + SM2_KDF_COUNTER_SIZE );
    mbedtls_free( pData );
    return 0;
}

#endif /* MBEDTLS_SM2PKE_C || MBEDTLS_SM2KEP_C*/
