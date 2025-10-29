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
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */
#include <stdio.h>
#include <string.h>
#include "mbedtls/aes.h"

#if defined( MBEDTLS_AES_ALT )

#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf //printf
#endif /* MBEDTLS_PLATFORM_C */

/* Implementation that should never be optimized out by the compiler */
static void dubhe_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = (unsigned char *) v;
    while ( n-- )
        *p++ = 0;
}

void dubhe_aes_init( mbedtls_aes_context *ctx )
{
    int ret = 0;

    ret = arm_ce_sca_init( ctx );
    if ( ret < 0 )
    {
        mbedtls_printf( "init sca context failed\n" );
        return;
    }
    ctx->alg_type = ARM_CE_AES;
}

void dubhe_aes_free( mbedtls_aes_context *ctx )
{
    if ( ctx == NULL )
        return;

    arm_ce_sca_free( ctx, NULL );
    dubhe_zeroize( ctx, sizeof( arm_ce_sca_context_t ) );
}

/*
 * AES key schedule (encryption)
 */
#if defined( MBEDTLS_AES_SETKEY_ENC_ALT ) // TODO wangzhilei
int dubhe_aes_setkey_enc( mbedtls_aes_context *ctx,
                          const unsigned char *key,
                          unsigned int keybits )
{
    int ret = 0;

    switch ( keybits )
    {
    case 128:
        break;
    case 192:
        break;
    case 256:
        break;
    default:
        return ( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    if ( ctx == NULL || key == NULL )
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    /* Just for ctr mode, ctr cryption interface no mode parameter. */

    ret = arm_ce_sca_set_key(
        ctx, ARM_CE_SCA_EXTERNAL_KEY, key, NULL, NULL, keybits, 0 );

    switch ( ret )
    {
    case DBH_SCA_KEYBITS_INVAL_ERR:
        ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
        break;
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
        ret = MBEDTLS_ERR_AES_INVALID_PARAM;
        break;
    default:
        ret = MBEDTLS_ERR_AES_GENERIC;
        break;
    }

    return ret;
}
#endif /* !MBEDTLS_AES_SETKEY_ENC_ALT */

/*
 * AES key schedule (decryption)
 */
#if defined( MBEDTLS_AES_SETKEY_DEC_ALT ) // TODO wangzhilei
int dubhe_aes_setkey_dec( mbedtls_aes_context *ctx,
                          const unsigned char *key,
                          unsigned int keybits )
{
    int ret = 0;

    if ( ctx == NULL || key == NULL )
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    switch ( keybits )
    {
    case 128:
        break;
    case 192:
        break;
    case 256:
        break;
    default:
        return ( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    ret                = arm_ce_sca_set_key(
        ctx, ARM_CE_SCA_EXTERNAL_KEY, key, NULL, NULL, keybits, 0 );

    switch ( ret )
    {
    case DBH_SCA_KEYBITS_INVAL_ERR:
        ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
        break;
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
        ret = MBEDTLS_ERR_AES_INVALID_PARAM;
        break;
    default:
        ret = MBEDTLS_ERR_AES_GENERIC;
        break;
    }

    return ret;
}
#endif /* !MBEDTLS_AES_SETKEY_DEC_ALT */

int dubhe_aes_set_derived_key( mbedtls_aes_context *ctx,
                               int key_type,
                               int mode,
                               const unsigned char *ek1,
                               const unsigned char *ek2,
                               const unsigned char *ek3,
                               unsigned int ek1bits )
{
    int ret = -1;

    if ( ctx == NULL || ek1 == NULL || ek2 == NULL || ek3 == NULL )
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    if ( key_type == MBEDTLS_AES_DERIVED_MODEL_KEY )
        ret = arm_ce_sca_set_key(
            ctx, ARM_CE_SCA_MODEL_KEY, ek1, ek2, ek3, ek1bits, 1 );
    else if ( key_type == MBEDTLS_AES_DERIVED_DEVICE_ROOT_KEY )
        ret = arm_ce_sca_set_key(
            ctx, ARM_CE_SCA_DEVICE_ROOT_KEY, ek1, ek2, ek3, ek1bits, 1 );
    else
    {
        mbedtls_printf( "aes set derived key failed\n" );
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    }

    switch ( ret )
    {
    case DBH_SCA_KEYBITS_INVAL_ERR:
        ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
        break;
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
        ret = MBEDTLS_ERR_AES_INVALID_PARAM;
        break;
    default:
        ret = MBEDTLS_ERR_AES_GENERIC;
        break;
    }

    return ret;
}

/*
 * AES-ECB block encryption/decryption
 */
int dubhe_aes_crypt_ecb( mbedtls_aes_context *ctx,
                         int mode,
                         const unsigned char input[16],
                         unsigned char output[16] )
{
    if ( ctx == NULL )
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    return ( arm_ce_sca_crypt( ctx,
                               ( ( MBEDTLS_AES_ENCRYPT == mode ) ?
                                 ARM_CE_SCA_OPERATION_ENCRYPT :
                                 ARM_CE_SCA_OPERATION_DECRYPT ),
                               ARM_CE_AES_ECB,
                               0,
                               ARM_CE_SCA_BLK_SIZE,
                               NULL,
                               NULL,
                               NULL,
                               input,
                               output ) );
}

#if defined( MBEDTLS_CIPHER_MODE_CBC )
/*
 * AES-CBC buffer encryption/decryption
 */
int dubhe_aes_crypt_cbc( mbedtls_aes_context *ctx,
                         int mode,
                         size_t length,
                         unsigned char iv[16],
                         const unsigned char *input,
                         unsigned char *output )
{
    if ( ctx == NULL )
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    if ( length % 16 )
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );

    return arm_ce_sca_crypt( ctx,
                             ( ( MBEDTLS_AES_DECRYPT == mode ) ?
                                 ARM_CE_SCA_OPERATION_DECRYPT :
                                 ARM_CE_SCA_OPERATION_ENCRYPT ),
                             ARM_CE_AES_CBC,
                             0,
                             length,
                             NULL,
                             iv,
                             NULL,
                             input,
                             output );
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined( MBEDTLS_CIPHER_MODE_CFB )
/*
 * AES-CFB128 buffer encryption/decryption
 */
int dubhe_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                            int mode,
                            size_t length,
                            size_t *iv_off,
                            unsigned char iv[16],
                            const unsigned char *input,
                            unsigned char *output )
{
    int c;
    size_t n = 0;
    int ret  = 0;

    if ( NULL == ctx || NULL == iv_off )
    {
        return MBEDTLS_ERR_AES_INVALID_PARAM;
    }

    n = *iv_off;
    if ( mode == MBEDTLS_AES_DECRYPT )
    {
        while ( length-- )
        {
            if ( n == 0 )
            {
                ret = dubhe_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

                if ( 0 != ret )
                {
                    return ret;
                }
            }

            c         = *input++;
            *output++ = (unsigned char) ( c ^ iv[n] );
            iv[n]     = (unsigned char) c;

            n = ( n + 1 ) & 0x0F;
        }
    }
    else
    {
        while ( length-- )
        {
            if ( n == 0 )
            {
                ret = dubhe_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

                if ( 0 != ret )
                {
                    return ret;
                }
            }

            iv[n] = *output++ = (unsigned char) ( iv[n] ^ *input++ );

            n = ( n + 1 ) & 0x0F;
        }
    }

    *iv_off = n;

    return ( 0 );
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int dubhe_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output )
{
    unsigned char c;
    unsigned char ov[17];
    int ret = 0;
    while ( length-- )
    {
        memcpy( ov, iv, 16 );
        ret = dubhe_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
        if ( 0 != ret )
        {
            return ret;
        }

        if ( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char) ( iv[0] ^ *input++ );

        if ( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }

    return ( 0 );
}
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined( MBEDTLS_CIPHER_MODE_CTR )
/*
 * AES-CTR buffer encryption/decryption
 */
int dubhe_aes_crypt_ctr( mbedtls_aes_context *ctx,
                         size_t length,
                         size_t *nc_off,
                         unsigned char nonce_counter[16],
                         unsigned char stream_block[16],
                         const unsigned char *input,
                         unsigned char *output )
{
    if ( ctx == NULL )
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    return ( arm_ce_sca_crypt( ctx,
                               ARM_CE_SCA_OPERATION_ENCRYPT,
                               ARM_CE_AES_CTR,
                               0,
                               length,
                               nc_off,
                               nonce_counter,
                               stream_block,
                               input,
                               output ) );
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
/*
 * AES-OFB (Output Feedback Mode) buffer encryption/decryption
 */
int dubhe_aes_crypt_ofb( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *iv_off,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int c;
    size_t n = 0;
    int ret  = 0;

    if ( NULL == ctx || NULL == iv_off || NULL == iv || NULL == input || NULL == output)
    {
        return MBEDTLS_ERR_AES_INVALID_PARAM;
    }

    n = *iv_off;
    if( n > 15 )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    while ( length-- )
    {
        if ( n == 0 )
        {
            ret = dubhe_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
            if ( 0 != ret )
            {
                return ret;
            }
        }
        *output++ =  *input++ ^ iv[n];

        n = ( n + 1 ) & 0x0F;
    }

    *iv_off = n;

    return ( 0 );
}
#endif
#endif /* !MBEDTLS_AES_ALT */
