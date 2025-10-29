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
#include "common.h"
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

#if CONFIG_SHANHAI_KEY_LADDER
static const uint8_t EK2[] = { 0x72, 0x36, 0x36, 0x36, 0xfb, 0xfa, 0xf9, 0xf8,
                              0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0,
                              0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                              0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
static const uint8_t EK3[] = { 0x20, 0x24, 0x20, 0x24, 0xfb, 0xfa, 0xf9, 0xf8,
                              0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0,
                              0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                              0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
#endif

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

#if CONFIG_SHANHAI_KEY_LADDER
    BK_ASSERT(sizeof(EK2) * 8 >= keybits);
    BK_ASSERT(sizeof(EK3) * 8 >= keybits);

    ret = arm_ce_sca_set_key(ctx, ARM_CE_SCA_DEVICE_ROOT_KEY, key, EK2, EK3, keybits, 1);
#else
    ret = arm_ce_sca_set_key(ctx, ARM_CE_SCA_EXTERNAL_KEY, key, NULL, NULL, keybits, 0);
#endif
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

#if CONFIG_SHANHAI_KEY_LADDER
        BK_ASSERT(sizeof(EK2) * 8 >= keybits);
        BK_ASSERT(sizeof(EK3) * 8 >= keybits);
    
        ret = arm_ce_sca_set_key(ctx, ARM_CE_SCA_DEVICE_ROOT_KEY, key, EK2, EK3, keybits, 1);
#else
        ret = arm_ce_sca_set_key(ctx, ARM_CE_SCA_EXTERNAL_KEY, key, NULL, NULL, keybits, 0 );
#endif

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

#if defined(MBEDTLS_CIPHER_MODE_XTS)
#define DUBHE_ERR_CORRUPTION_DETECTED -0x006E
#define MBEDTLS_EFFICIENT_UNALIGNED_ACCESS 0

/**
 * Read the unsigned 32 bits integer from the given address, which need not
 * be aligned.
 *
 * \param   p pointer to 4 bytes of data
 * \return  Data at the given address
 */
inline uint32_t mbedtls_get_unaligned_uint32(const void *p)
{
    uint32_t r;
    memcpy(&r, p, sizeof(r));
    return r;
}

/**
 * Write the unsigned 32 bits integer to the given address, which need not
 * be aligned.
 *
 * \param   p pointer to 4 bytes of data
 * \param   x data to write
 */
inline void mbedtls_put_unaligned_uint32(void *p, uint32_t x)
{
    memcpy(p, &x, sizeof(x));
}

/**
 * Perform a fast block XOR operation, such that
 * r[i] = a[i] ^ b[i] where 0 <= i < n
 *
 * \param   r Pointer to result (buffer of at least \p n bytes). \p r
 *            may be equal to either \p a or \p b, but behaviour when
 *            it overlaps in other ways is undefined.
 * \param   a Pointer to input (buffer of at least \p n bytes)
 * \param   b Pointer to input (buffer of at least \p n bytes)
 * \param   n Number of bytes to process.
 */
void dubhe_xor(unsigned char *r, const unsigned char *a, const unsigned char *b, size_t n)
{
    size_t i = 0;

#if (MBEDTLS_EFFICIENT_UNALIGNED_ACCESS)
    for (; (i + 4) <= n; i += 4) {
        uint32_t x = mbedtls_get_unaligned_uint32(a + i) ^ mbedtls_get_unaligned_uint32(b + i);
        mbedtls_put_unaligned_uint32(r + i, x);
    }
#endif

    for (; i < n; i++) {
        r[i] = a[i] ^ b[i];
    }
}

void dubhe_aes_xts_init( mbedtls_aes_xts_context *ctx )
{
    mbedtls_aes_init( &ctx->crypt );
    mbedtls_aes_init( &ctx->tweak );
}

void dubhe_aes_xts_free(mbedtls_aes_xts_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mbedtls_aes_free(&ctx->crypt);
    mbedtls_aes_free(&ctx->tweak);
}

static void dubhe_gf128mul_x_ble( unsigned char r[16],
                                    const unsigned char x[16] )
{
    uint64_t a, b, ra, rb;

    a = MBEDTLS_GET_UINT64_LE( x, 0 );
    b = MBEDTLS_GET_UINT64_LE( x, 8 );

    ra = ( a << 1 )  ^ 0x0087 >> ( 8 - ( ( b >> 63 ) << 3 ) );
    rb = ( a >> 63 ) | ( b << 1 );

    MBEDTLS_PUT_UINT64_LE( ra, r, 0 );
    MBEDTLS_PUT_UINT64_LE( rb, r, 8 );
}

static int dubhe_aes_xts_decode_keys( const unsigned char *key,
                                        unsigned int keybits,
                                        const unsigned char **key1,
                                        unsigned int *key1bits,
                                        const unsigned char **key2,
                                        unsigned int *key2bits )
{
    const unsigned int half_keybits = keybits / 2;
    const unsigned int half_keybytes = half_keybits / 8;

    switch( keybits )
    {
        case 256: break;
        case 512: break;
        default : return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    *key1bits = half_keybits;
    *key2bits = half_keybits;
    *key1 = &key[0];
    *key2 = &key[half_keybytes];

    return 0;
}

int dubhe_aes_xts_setkey_enc( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits)
{
    int ret = DUBHE_ERR_CORRUPTION_DETECTED;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    ret = dubhe_aes_xts_decode_keys(key, keybits, &key1, &key1bits,
                                      &key2, &key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set the tweak key. Always set tweak key for the encryption mode. */
    ret = dubhe_aes_setkey_enc(&ctx->tweak, key2, key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set crypt key for encryption. */
    return dubhe_aes_setkey_enc(&ctx->crypt, key1, key1bits);
}

int dubhe_aes_xts_setkey_dec( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits)
{
    int ret = DUBHE_ERR_CORRUPTION_DETECTED;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    ret = dubhe_aes_xts_decode_keys(key, keybits, &key1, &key1bits,
                                      &key2, &key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set the tweak key. Always set tweak key for encryption. */
    ret = dubhe_aes_setkey_enc(&ctx->tweak, key2, key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set crypt key for decryption. */
    return dubhe_aes_setkey_dec(&ctx->crypt, key1, key1bits);
}

/*
 * AES-XTS buffer encryption/decryption
 */
int dubhe_aes_crypt_xts( mbedtls_aes_xts_context *ctx,
                           int mode,
                           size_t length,
                           const unsigned char data_unit[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret = DUBHE_ERR_CORRUPTION_DETECTED;
    size_t blocks = length / 16;
    size_t leftover = length % 16;
    unsigned char tweak[16];
    unsigned char prev_tweak[16];
    unsigned char tmp[16];

    BK_ASSERT( ctx != NULL );
    BK_ASSERT( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );
    BK_ASSERT( data_unit != NULL );
    BK_ASSERT( input != NULL );
    BK_ASSERT( output != NULL );

    /* Data units must be at least 16 bytes long. */
    if( length < 16 )
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;

    /* NIST SP 800-38E disallows data units larger than 2**20 blocks. */
    if( length > ( 1 << 20 ) * 16 )
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;

    /* Compute the tweak. */
    ret = dubhe_aes_crypt_ecb( &ctx->tweak, MBEDTLS_AES_ENCRYPT,
                                 data_unit, tweak );
    if( ret != 0 )
        return( ret );

    while( blocks-- )
    {
        size_t i;

        if( leftover && ( mode == MBEDTLS_AES_DECRYPT ) && blocks == 0 )
        {
            /* We are on the last block in a decrypt operation that has
             * leftover bytes, so we need to use the next tweak for this block,
             * and this tweak for the lefover bytes. Save the current tweak for
             * the leftovers and then update the current tweak for use on this,
             * the last full block. */
            memcpy( prev_tweak, tweak, sizeof( tweak ) );
            dubhe_gf128mul_x_ble( tweak, tweak );
        }

        for( i = 0; i < 16; i++ )
            tmp[i] = input[i] ^ tweak[i];

        ret = dubhe_aes_crypt_ecb( &ctx->crypt, mode, tmp, tmp );
        if( ret != 0 )
            return( ret );

        for( i = 0; i < 16; i++ )
            output[i] = tmp[i] ^ tweak[i];

        /* Update the tweak for the next block. */
        dubhe_gf128mul_x_ble( tweak, tweak );

        output += 16;
        input += 16;
    }

    if( leftover )
    {
        /* If we are on the leftover bytes in a decrypt operation, we need to
         * use the previous tweak for these bytes (as saved in prev_tweak). */
        unsigned char *t = mode == MBEDTLS_AES_DECRYPT ? prev_tweak : tweak;

        /* We are now on the final part of the data unit, which doesn't divide
         * evenly by 16. It's time for ciphertext stealing. */
        size_t i;
        unsigned char *prev_output = output - 16;

        /* Copy ciphertext bytes from the previous block to our output for each
         * byte of cyphertext we won't steal. At the same time, copy the
         * remainder of the input for this final round (since the loop bounds
         * are the same). */
        for( i = 0; i < leftover; i++ )
        {
            output[i] = prev_output[i];
            tmp[i] = input[i] ^ t[i];
        }

        /* Copy ciphertext bytes from the previous block for input in this
         * round. */
        for( ; i < 16; i++ )
            tmp[i] = prev_output[i] ^ t[i];

        ret = dubhe_aes_crypt_ecb( &ctx->crypt, mode, tmp, tmp );
        if( ret != 0 )
            return ret;

        /* Write the result back to the previous block, overriding the previous
         * output we copied. */
        for( i = 0; i < 16; i++ )
            prev_output[i] = tmp[i] ^ t[i];
    }

    return( 0 );
}

/*
 * Checkup routine
 */
static const unsigned char aes_test_xts_key[][32] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
      0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 },
    { 0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8,
      0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 },
};

static const unsigned char aes_test_xts_pt32[][32] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44 },
    { 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44 },
};

static const unsigned char aes_test_xts_ct32[][32] =
{
    { 0x91, 0x7c, 0xf6, 0x9e, 0xbd, 0x68, 0xb2, 0xec,
      0x9b, 0x9f, 0xe9, 0xa3, 0xea, 0xdd, 0xa6, 0x92,
      0xcd, 0x43, 0xd2, 0xf5, 0x95, 0x98, 0xed, 0x85,
      0x8c, 0x02, 0xc2, 0x65, 0x2f, 0xbf, 0x92, 0x2e },
    { 0xc4, 0x54, 0x18, 0x5e, 0x6a, 0x16, 0x93, 0x6e,
      0x39, 0x33, 0x40, 0x38, 0xac, 0xef, 0x83, 0x8b,
      0xfb, 0x18, 0x6f, 0xff, 0x74, 0x80, 0xad, 0xc4,
      0x28, 0x93, 0x82, 0xec, 0xd6, 0xd3, 0x94, 0xf0 },
    { 0xaf, 0x85, 0x33, 0x6b, 0x59, 0x7a, 0xfc, 0x1a,
      0x90, 0x0b, 0x2e, 0xb2, 0x1e, 0xc9, 0x49, 0xd2,
      0x92, 0xdf, 0x4c, 0x04, 0x7e, 0x0b, 0x21, 0x53,
      0x21, 0x86, 0xa5, 0x97, 0x1a, 0x22, 0x7a, 0x89 },
};

static const unsigned char aes_test_xts_data_unit[][16] =
{
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

#if CONFIG_SHANHAI_KEY_LADDER
unsigned char plain_buf[64];
unsigned char enc_buf[64];
unsigned char dec_buf[64];

int dubhe_aes_xts_self_test(int verbose)
{
    int ret = 0, u = 1;
    unsigned int len;
    unsigned char key[32];
    mbedtls_aes_xts_context enc_ctx_xts;
    mbedtls_aes_xts_context dec_ctx_xts;
    const unsigned char *data_unit;

    mbedtls_aes_xts_init(&enc_ctx_xts);
    mbedtls_aes_xts_init(&dec_ctx_xts);

    memset(key, 0, sizeof(key));
    memcpy(key, aes_test_xts_key[u], 32);
    data_unit = aes_test_xts_data_unit[u];
    len = sizeof(*aes_test_xts_ct32);

    ret = dubhe_aes_xts_setkey_enc(&enc_ctx_xts, key, 256);
    if (ret != 0) {
        goto exit;
    }

    memcpy(plain_buf, aes_test_xts_pt32[u], len);
    ret = dubhe_aes_crypt_xts(&enc_ctx_xts, MBEDTLS_AES_ENCRYPT, len, data_unit,plain_buf, enc_buf);
    if (ret != 0) {
        goto exit;
    }

    ret = dubhe_aes_xts_setkey_dec(&dec_ctx_xts, key, 256);
    if (ret != 0) {
        goto exit;
    }
    ret = dubhe_aes_crypt_xts(&dec_ctx_xts, MBEDTLS_AES_DECRYPT, len, data_unit,enc_buf, dec_buf);
    if (ret != 0) {
        goto exit;
    }

    if (memcmp(plain_buf, dec_buf, len) != 0) {
        ret = 1;
        goto exit;
    }

    if (verbose != 0) {
        bk_printf("passed\n");
    }

    mbedtls_aes_xts_free(&enc_ctx_xts);
    mbedtls_aes_xts_free(&dec_ctx_xts);

    ret = 0;

exit:
    if (ret != 0 && verbose != 0) {
        bk_printf("failed\n");
    }

    return ret;
}

#else
int dubhe_aes_xts_self_test(int verbose)
{
    int ret = 0, i, u, mode;
    unsigned int len;
    unsigned char key[32];
    unsigned char buf[64];
    const unsigned char *aes_tests;
    static const int num_tests =
        sizeof(aes_test_xts_key) / sizeof(*aes_test_xts_key);
    mbedtls_aes_xts_context ctx_xts;

    mbedtls_aes_xts_init(&ctx_xts);

    for (i = 0; i < num_tests << 1; i++) {
        const unsigned char *data_unit;
        u = i >> 1;
        mode = i & 1;

        if (verbose != 0) {
            bk_printf("  AES-XTS-128 (%s): ",
                           (mode == MBEDTLS_AES_DECRYPT) ? "dec" : "enc");
        }

        memset(key, 0, sizeof(key));
        memcpy(key, aes_test_xts_key[u], 32);
        data_unit = aes_test_xts_data_unit[u];

        len = sizeof(*aes_test_xts_ct32);

        if (mode == MBEDTLS_AES_DECRYPT) {
            ret = mbedtls_aes_xts_setkey_dec(&ctx_xts, key, 256);
            if (ret != 0) {
                goto exit;
            }
            memcpy(buf, aes_test_xts_ct32[u], len);
            aes_tests = aes_test_xts_pt32[u];
        } else {
            ret = mbedtls_aes_xts_setkey_enc(&ctx_xts, key, 256);
            if (ret != 0) {
                goto exit;
            }
            memcpy(buf, aes_test_xts_pt32[u], len);
            aes_tests = aes_test_xts_ct32[u];
        }

        ret = mbedtls_aes_crypt_xts(&ctx_xts, mode, len, data_unit,
                                    buf, buf);
        if (ret != 0) {
            goto exit;
        }

        if (memcmp(buf, aes_tests, len) != 0) {
            ret = 1;
            goto exit;
        }

        if (verbose != 0) {
            bk_printf("passed\n");
        }
    }

    if (verbose != 0) {
        bk_printf("\n");
    }

    mbedtls_aes_xts_free(&ctx_xts);

    ret = 0;

exit:
    if (ret != 0 && verbose != 0) {
        mbedtls_printf("failed\n");
    }

    return ret;
}
#endif

#endif
#endif /* !MBEDTLS_AES_ALT */
