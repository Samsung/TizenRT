/**
* @if copyright_display
 *      Copyright (C), 2018-2020, Arm Technology (China) Co., Ltd.
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
 * http://csrc.nist.gov/publications/nistpubs/800-38D/SP-800-38D.pdf
 *
 * See also:
 * [MGV] http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-revised-spec.pdf
 *
 * We use the algorithm described as Shoup's method with 4-bit tables in
 * [MGV] 4.1, pp. 12-13, to enhance speed without using too much memory.
 */

#include "common.h"

#if defined(MBEDTLS_GCM_C)

#include "mbedtls/gcm.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include "dubhe_sca.h"
#include "dubhe_gcm.h"

#include <string.h>

#if defined(MBEDTLS_AESNI_C)
#include "aesni.h"
#endif

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
#include "mbedtls/aes.h"
#include "mbedtls/platform.h"
#if !defined(MBEDTLS_PLATFORM_C)
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */

#if defined(MBEDTLS_GCM_ALT)

/* Parameter validation macros */
#define GCM_VALIDATE_RET( cond ) \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_GCM_BAD_INPUT )
#define GCM_VALIDATE( cond ) \
    MBEDTLS_INTERNAL_VALIDATE( cond )

static inline int arm_ce_errno_to_mbedtls(int errno) {
    switch (errno)
    {
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
    case DBH_SCA_KEYBITS_INVAL_ERR:
        errno = MBEDTLS_ERR_GCM_BAD_INPUT;
        break;
    case DBH_GCM_ERR_OOM:
        errno = MBEDTLS_ERR_GCM_OOM;
        break;
    default:
        errno = MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
        break;
    };

    return errno;
}

static void arm_ce_gcm_clear( mbedtls_gcm_context *ctx )
{
    size_t olen;
    __attribute__((unused)) unsigned char tag[16] = {0};

    mbedtls_gcm_finish( ctx, NULL, 0, &olen, tag, sizeof( tag ) );
}

/*
 * Initialize context
 */
void mbedtls_gcm_init( mbedtls_gcm_context *ctx )
{
    arm_ce_gcm_ctx_t *armgcm = NULL;

    if ( NULL == ctx ) {
        return;
    }
    memset( ctx, 0, sizeof( *ctx ) );
    armgcm = mbedtls_calloc(1, sizeof(*armgcm));
    if ( NULL == armgcm ) {
        return;
    }
    ctx->armgcm = armgcm;
}

/*
 * Precompute small multiples of H, that is set
 *      HH[i] || HL[i] = H times i,
 * where i is seen as a field element as in [MGV], ie high-order bits
 * correspond to low powers of P. The result is stored in the same way, that
 * is the high-order bit of HH corresponds to P^0 and the low-order bit of HL
 * corresponds to P^127.
 */
static int gcm_gen_table( mbedtls_gcm_context *ctx )
{
    int ret, i, j;
    uint64_t hi, lo;
    uint64_t vl, vh;
    unsigned char h[16];
    size_t olen = 0;

    memset( h, 0, 16 );
    if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, h, 16, h, &olen ) ) != 0 )
        return( ret );

    /* pack h as two 64-bits ints, big-endian */
    hi = MBEDTLS_GET_UINT32_BE( h,  0  );
    lo = MBEDTLS_GET_UINT32_BE( h,  4  );
    vh = (uint64_t) hi << 32 | lo;

    hi = MBEDTLS_GET_UINT32_BE( h,  8  );
    lo = MBEDTLS_GET_UINT32_BE( h,  12 );
    vl = (uint64_t) hi << 32 | lo;

    /* 8 = 1000 corresponds to 1 in GF(2^128) */
    ctx->HL[8] = vl;
    ctx->HH[8] = vh;

#if defined(MBEDTLS_AESNI_C) && defined(MBEDTLS_HAVE_X86_64)
    /* With CLMUL support, we need only h, not the rest of the table */
    if( mbedtls_aesni_has_support( MBEDTLS_AESNI_CLMUL ) )
        return( 0 );
#endif

    /* 0 corresponds to 0 in GF(2^128) */
    ctx->HH[0] = 0;
    ctx->HL[0] = 0;

    for( i = 4; i > 0; i >>= 1 )
    {
        uint32_t T = ( vl & 1 ) * 0xe1000000U;
        vl  = ( vh << 63 ) | ( vl >> 1 );
        vh  = ( vh >> 1 ) ^ ( (uint64_t) T << 32);

        ctx->HL[i] = vl;
        ctx->HH[i] = vh;
    }

    for( i = 2; i <= 8; i *= 2 )
    {
        uint64_t *HiL = ctx->HL + i, *HiH = ctx->HH + i;
        vh = *HiH;
        vl = *HiL;
        for( j = 1; j < i; j++ )
        {
            HiH[j] = vh ^ ctx->HH[j];
            HiL[j] = vl ^ ctx->HL[j];
        }
    }

    return( 0 );
}

int sw_compatible_mbedtls_gcm_setkey( mbedtls_gcm_context *ctx,
                                      mbedtls_cipher_id_t cipher,
                                      const unsigned char *key,
                                      unsigned int keybits )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const mbedtls_cipher_info_t *cipher_info;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( key != NULL );
    GCM_VALIDATE_RET( keybits == 128 || keybits == 192 || keybits == 256 );

    cipher_info = mbedtls_cipher_info_from_values( cipher, keybits,
                                                   MBEDTLS_MODE_ECB );
    if( cipher_info == NULL )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    if( cipher_info->block_size != 16 )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    mbedtls_cipher_free( &ctx->cipher_ctx );

    if( ( ret = mbedtls_cipher_setup( &ctx->cipher_ctx, cipher_info ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_cipher_setkey( &ctx->cipher_ctx, key, keybits,
                               MBEDTLS_ENCRYPT ) ) != 0 )
    {
        return( ret );
    }

    if( ( ret = gcm_gen_table( ctx ) ) != 0 )
        return( ret );

    return( 0 );
}

int sw_compatible_mbedtls_gcm_set_derived_key( mbedtls_gcm_context *ctx,
                                               int key_type,
                                               const unsigned char *ek1,
                                               const unsigned char *ek2,
                                               const unsigned char *ek3,
                                               unsigned int ek1bits )
{
    (void) ctx;
    (void) key_type;
    (void) ek1;
    (void) ek2;
    (void) ek3;
    (void) ek1bits;

    return( MBEDTLS_ERR_GCM_BAD_INPUT );
}

int mbedtls_gcm_setkey( mbedtls_gcm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
    int ret;
    mbedtls_gcm_context swap = {0};
    int32_t alg = 0;

    if ( ( NULL == ctx ) || ( NULL == ctx->armgcm ) ||
         ( NULL == key )) {
        return( MBEDTLS_ERR_GCM_BAD_INPUT );
    }

    switch (cipher)
    {
    case MBEDTLS_CIPHER_ID_AES:
        alg = ARM_CE_AES_GCM;
        break;
    case MBEDTLS_CIPHER_ID_SM4:
        alg = ARM_CE_SM4_GCM;
        break;
    default:
        return sw_compatible_mbedtls_gcm_setkey( ctx, cipher, key, keybits );
    }
    /** check if setkey previously,if not shoud init first */
    if ( MBEDTLS_CIPHER_ID_NONE == ctx->cipher ) {
        ret = arm_ce_gcm_init( ctx->armgcm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        ctx->cipher = cipher;
        ret = arm_ce_gcm_set_key( ctx->armgcm, key, keybits );
    } else if ( cipher != ctx->cipher) {
    /** change cipher then free previous and start with the new one */
        mbedtls_gcm_init( &swap );
        if (NULL == swap.armgcm) {
            return MBEDTLS_ERR_GCM_OOM;
        }
        ret = arm_ce_gcm_init( swap.armgcm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        swap.cipher = cipher;
        ret = arm_ce_gcm_set_key( swap.armgcm, key, keybits );
        if ( 0 == ret ) {
            mbedtls_gcm_free( ctx );
            memcpy( ctx, &swap, sizeof(*ctx) );
            return 0;
        } else {
            mbedtls_gcm_free( &swap );
        }
    } else { /** same cipher just setkey */
        ret = arm_ce_gcm_set_key( ctx->armgcm, key, keybits );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int mbedtls_gcm_set_derived_key( mbedtls_gcm_context *ctx,
                                 mbedtls_cipher_id_t cipher,
                                 int key_type,
                                 const unsigned char *ek1,
                                 const unsigned char *ek2,
                                 const unsigned char *ek3,
                                 unsigned int ek1bits )
{
#define GET_ARM_CE_KEY_TYPE(type) ( (MBEDTLS_GCM_DERIVED_MODEL_KEY == (type)) ?\
                        ARM_CE_SCA_MODEL_KEY : ARM_CE_SCA_DEVICE_ROOT_KEY )
    int ret = 0;
    mbedtls_gcm_context swap = {0};
    int32_t alg = 0;

    if ( ( NULL == ctx ) || ( NULL == ctx->armgcm ) ||
         ( NULL == ek1 ) || ( NULL == ek2 ) || ( NULL == ek3 ) ||
         ( ( MBEDTLS_GCM_DERIVED_MODEL_KEY != key_type ) &&
           ( MBEDTLS_GCM_DERIVED_DEVICE_ROOT_KEY != key_type ) ) ) {
        return( MBEDTLS_ERR_GCM_BAD_INPUT );
    }

    switch (cipher)
    {
    case MBEDTLS_CIPHER_ID_AES:
        alg = ARM_CE_AES_GCM;
        break;
#if defined(ARM_CE_DUBHE_SCA)
    case MBEDTLS_CIPHER_ID_SM4:
        alg = ARM_CE_SM4_GCM;
        break;
#endif
    default:
        return sw_compatible_mbedtls_gcm_set_derived_key( ctx,
                                            GET_ARM_CE_KEY_TYPE( key_type ),
                                                         ek1, ek2, ek3,
                                                         ek1bits );
    }
    /** check if setkey previously,if not shoud init first */
    if ( MBEDTLS_CIPHER_ID_NONE == ctx->cipher ) {
        ret = arm_ce_gcm_init( ctx->armgcm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        ctx->cipher = cipher;
        ret = arm_ce_gcm_set_derived_key( ctx->armgcm,
                                    GET_ARM_CE_KEY_TYPE( key_type ),
                                    ek1, ek2, ek3, ek1bits );
    } else if ( cipher != ctx->cipher) {
    /** change cipher then free previous and start with the new one */
        mbedtls_gcm_init( &swap );
        if (NULL == swap.armgcm) {
            return MBEDTLS_ERR_GCM_OOM;
        }
        ret = arm_ce_gcm_init( swap.armgcm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        swap.cipher = cipher;
        ret = arm_ce_gcm_set_derived_key( swap.armgcm,
                                     GET_ARM_CE_KEY_TYPE( key_type ),
                                     ek1, ek2, ek3, ek1bits );
        if ( 0 == ret ) {
            mbedtls_gcm_free( ctx );
            memcpy( ctx, &swap, sizeof(*ctx) );
            return 0;
        } else {
            mbedtls_gcm_free( &swap );
        }
    } else { /** same cipher just setkey */
        ret = arm_ce_gcm_set_derived_key( ctx->armgcm,
                                     GET_ARM_CE_KEY_TYPE( key_type ),
                                     ek1, ek2, ek3, ek1bits );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

/*
 * Shoup's method for multiplication use this table with
 *      last4[x] = x times P^128
 * where x and last4[x] are seen as elements of GF(2^128) as in [MGV]
 */
static const uint64_t last4[16] =
{
    0x0000, 0x1c20, 0x3840, 0x2460,
    0x7080, 0x6ca0, 0x48c0, 0x54e0,
    0xe100, 0xfd20, 0xd940, 0xc560,
    0x9180, 0x8da0, 0xa9c0, 0xb5e0
};

/*
 * Sets output to x times H using the precomputed tables.
 * x and output are seen as elements of GF(2^128) as in [MGV].
 */
static void gcm_mult( mbedtls_gcm_context *ctx, const unsigned char x[16],
                      unsigned char output[16] )
{
    int i = 0;
    unsigned char lo, hi, rem;
    uint64_t zh, zl;

#if defined(MBEDTLS_AESNI_C) && defined(MBEDTLS_HAVE_X86_64)
    if( mbedtls_aesni_has_support( MBEDTLS_AESNI_CLMUL ) ) {
        unsigned char h[16];

        MBEDTLS_PUT_UINT32_BE( ctx->HH[8] >> 32, h,  0 );
        MBEDTLS_PUT_UINT32_BE( ctx->HH[8],       h,  4 );
        MBEDTLS_PUT_UINT32_BE( ctx->HL[8] >> 32, h,  8 );
        MBEDTLS_PUT_UINT32_BE( ctx->HL[8],       h, 12 );

        mbedtls_aesni_gcm_mult( output, x, h );
        return;
    }
#endif /* MBEDTLS_AESNI_C && MBEDTLS_HAVE_X86_64 */

    lo = x[15] & 0xf;

    zh = ctx->HH[lo];
    zl = ctx->HL[lo];

    for( i = 15; i >= 0; i-- )
    {
        lo = x[i] & 0xf;
        hi = x[i] >> 4;

        if( i != 15 )
        {
            rem = (unsigned char) zl & 0xf;
            zl = ( zh << 60 ) | ( zl >> 4 );
            zh = ( zh >> 4 );
            zh ^= (uint64_t) last4[rem] << 48;
            zh ^= ctx->HH[lo];
            zl ^= ctx->HL[lo];

        }

        rem = (unsigned char) zl & 0xf;
        zl = ( zh << 60 ) | ( zl >> 4 );
        zh = ( zh >> 4 );
        zh ^= (uint64_t) last4[rem] << 48;
        zh ^= ctx->HH[hi];
        zl ^= ctx->HL[hi];
    }

    MBEDTLS_PUT_UINT32_BE( zh >> 32, output, 0 );
    MBEDTLS_PUT_UINT32_BE( zh, output, 4 );
    MBEDTLS_PUT_UINT32_BE( zl >> 32, output, 8 );
    MBEDTLS_PUT_UINT32_BE( zl, output, 12 );
}

int sw_compatible_mbedtls_gcm_starts( mbedtls_gcm_context *ctx,
                                      int mode,
                                      const unsigned char *iv,
                                      size_t iv_len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char work_buf[16];
    size_t i;
    const unsigned char *p;
    size_t use_len, olen = 0;
    uint64_t iv_bits;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );

    /* IV is limited to 2^64 bits, so 2^61 bytes */
    /* IV is not allowed to be zero length */
    if( iv_len == 0 || (uint64_t) iv_len >> 61 != 0 )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    memset( ctx->y, 0x00, sizeof(ctx->y) );
    memset( ctx->buf, 0x00, sizeof(ctx->buf) );

    ctx->mode = mode;
    ctx->len = 0;
    ctx->add_len = 0;

    if( iv_len == 12 )
    {
        memcpy( ctx->y, iv, iv_len );
        ctx->y[15] = 1;
    }
    else
    {
        memset( work_buf, 0x00, 16 );
        iv_bits = (uint64_t)iv_len * 8;
        MBEDTLS_PUT_UINT64_BE( iv_bits, work_buf, 8 );

        p = iv;
        while( iv_len > 0 )
        {
            use_len = ( iv_len < 16 ) ? iv_len : 16;

            for( i = 0; i < use_len; i++ )
                ctx->y[i] ^= p[i];

            gcm_mult( ctx, ctx->y, ctx->y );

            iv_len -= use_len;
            p += use_len;
        }

        for( i = 0; i < 16; i++ )
            ctx->y[i] ^= work_buf[i];

        gcm_mult( ctx, ctx->y, ctx->y );
    }

    if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, ctx->y, 16,
                                       ctx->base_ectr, &olen ) ) != 0 )
    {
        return( ret );
    }

    return( 0 );
}

int mbedtls_gcm_starts( mbedtls_gcm_context *ctx,
                        int mode,
                      const unsigned char *iv, size_t iv_len )
{
    int ret;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );

    /* IV and AD are limited to 2^64 bits, so 2^61 bytes */
    /* IV is not allowed to be zero length */
    if( iv_len == 0 ||
      ( (uint64_t) iv_len  ) >> 61 != 0 )
    {
        return( MBEDTLS_ERR_GCM_BAD_INPUT );
    }

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_gcm_start( ctx->armgcm,
                                    (MBEDTLS_GCM_ENCRYPT == mode) ?
                                        ARM_CE_SCA_OPERATION_ENCRYPT :
                                        ARM_CE_SCA_OPERATION_DECRYPT,
                                    iv,
                                    iv_len);
        break;
        default:
            return sw_compatible_mbedtls_gcm_starts( ctx, mode,
                                                     iv, iv_len );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int sw_compatible_mbedtls_gcm_update_ad( mbedtls_gcm_context *ctx,
                           const unsigned char *add, size_t add_len )
{
    const unsigned char *p;
    size_t use_len, i, offset;

    offset = ctx->add_len % 16;
    p = add;

    if( offset != 0 )
    {
        use_len = 16 - offset;
        if( use_len > add_len )
            use_len = add_len;

        for( i = 0; i < use_len; i++ )
            ctx->buf[i+offset] ^= p[i];

        if( offset + use_len == 16 )
            gcm_mult( ctx, ctx->buf, ctx->buf );

        ctx->add_len += use_len;
        add_len -= use_len;
        p += use_len;
    }

    ctx->add_len += add_len;

    while( add_len >= 16 )
    {
        for( i = 0; i < 16; i++ )
            ctx->buf[i] ^= p[i];

        gcm_mult( ctx, ctx->buf, ctx->buf );

        add_len -= 16;
        p += 16;
    }

    if( add_len > 0 )
    {
        for( i = 0; i < add_len; i++ )
            ctx->buf[i] ^= p[i];
    }

    return( 0 );
}

int mbedtls_gcm_update_ad( mbedtls_gcm_context *ctx,
                           const unsigned char *add, size_t add_len )
{
    int ret;

    GCM_VALIDATE_RET( add_len == 0 || add != NULL );

    /* IV is limited to 2^64 bits, so 2^61 bytes */
    if( (uint64_t) add_len >> 61 != 0 )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_gcm_update_aad(ctx->armgcm, add_len, add);
            break;
        default:
            return sw_compatible_mbedtls_gcm_update_ad(ctx, add, add_len);
            break;
    }

    return arm_ce_errno_to_mbedtls( ret );;
}

/* Increment the counter. */
static void gcm_incr( unsigned char y[16] )
{
    size_t i;
    for( i = 16; i > 12; i-- )
        if( ++y[i - 1] != 0 )
            break;
}

/* Calculate and apply the encryption mask. Process use_len bytes of data,
* starting at position offset in the mask block. */
static int gcm_mask( mbedtls_gcm_context *ctx,
                      unsigned char ectr[16],
                      size_t offset, size_t use_len,
                      const unsigned char *input,
                      unsigned char *output )
{
    size_t i;
    size_t olen = 0;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, ctx->y, 16, ectr,
                                       &olen ) ) != 0 )
    {
        mbedtls_platform_zeroize( ectr, 16 );
        return( ret );
    }

    for( i = 0; i < use_len; i++ )
    {
        if( ctx->mode == MBEDTLS_GCM_DECRYPT )
            ctx->buf[offset + i] ^= input[i];
        output[i] = ectr[offset + i] ^ input[i];
        if( ctx->mode == MBEDTLS_GCM_ENCRYPT )
            ctx->buf[offset + i] ^= output[i];
    }
    return( 0 );
}

int sw_compatible_mbedtls_gcm_update( mbedtls_gcm_context *ctx,
                        const unsigned char *input, size_t input_length,
                        unsigned char *output, size_t output_size,
                        size_t *output_length )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const unsigned char *p = input;
    unsigned char *out_p = output;
    size_t offset;
    unsigned char ectr[16];

    if( ctx->len == 0 && ctx->add_len % 16 != 0 )
    {
        gcm_mult( ctx, ctx->buf, ctx->buf );
    }

    offset = ctx->len % 16;
    if( offset != 0 )
    {
        size_t use_len = 16 - offset;
        if( use_len > input_length )
        use_len = input_length;

        if( ( ret = gcm_mask( ctx, ectr, offset, use_len, p, out_p ) ) != 0 )
            return( ret );

        if( offset + use_len == 16 )
            gcm_mult( ctx, ctx->buf, ctx->buf );

        ctx->len += use_len;
        input_length -= use_len;
        p += use_len;
        out_p += use_len;
    }

    ctx->len += input_length;

    while( input_length >= 16 )
    {
        gcm_incr( ctx->y );
        if( ( ret = gcm_mask( ctx, ectr, 0, 16, p, out_p ) ) != 0 )
            return( ret );

        gcm_mult( ctx, ctx->buf, ctx->buf );

        input_length -= 16;
        p += 16;
        out_p += 16;
    }

    if( input_length > 0 )
    {
        gcm_incr( ctx->y );
        if( ( ret = gcm_mask( ctx, ectr, 0, input_length, p, out_p ) ) != 0 )
            return( ret );
    }

    mbedtls_platform_zeroize( ectr, sizeof( ectr ) );
    return( 0 );
}

int mbedtls_gcm_update( mbedtls_gcm_context *ctx,
                        const unsigned char *input, size_t input_length,
                        unsigned char *output, size_t output_size,
                        size_t *output_length )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if( output_size < input_length )
        return( MBEDTLS_ERR_GCM_BUFFER_TOO_SMALL );
    GCM_VALIDATE_RET( output_length != NULL );
    *output_length = input_length;

    /* Exit early if input_length==0 so that we don't do any pointer arithmetic
    * on a potentially null pointer.
    * Returning early also means that the last partial block of AD remains
    * untouched for mbedtls_gcm_finish */
    if( input_length == 0 )
        return( 0 );

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( input != NULL );
    GCM_VALIDATE_RET( output != NULL );

    if( output > input && (size_t) ( output - input ) < input_length )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    /* Total length is restricted to 2^39 - 256 bits, ie 2^36 - 2^5 bytes
    * Also check for possible overflow */
    if( ctx->len + input_length < ctx->len ||
        (uint64_t) ctx->len + input_length > 0xFFFFFFFE0ull )
    {
        return( MBEDTLS_ERR_GCM_BAD_INPUT );
    }

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_gcm_update( ctx->armgcm,
                                     input_length, input, output );
        break;
        default:
            return sw_compatible_mbedtls_gcm_update( ctx, input, input_length,
                                                     output, output_size, output_length );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int sw_compatible_mbedtls_gcm_finish( mbedtls_gcm_context *ctx,
                        unsigned char *output, size_t output_size,
                        size_t *output_length,
                        unsigned char *tag, size_t tag_len )
{
    unsigned char work_buf[16];
    size_t i;
    uint64_t orig_len;
    uint64_t orig_add_len;

    orig_len = ctx->len * 8;
    orig_add_len = ctx->add_len * 8;

    if( ctx->len == 0 && ctx->add_len % 16 != 0 )
    {
        gcm_mult( ctx, ctx->buf, ctx->buf );
    }

    if( tag_len > 16 || tag_len < 4 )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    if( ctx->len % 16 != 0 )
        gcm_mult( ctx, ctx->buf, ctx->buf );

    memcpy( tag, ctx->base_ectr, tag_len );

    if( orig_len || orig_add_len )
    {
        memset( work_buf, 0x00, 16 );

        MBEDTLS_PUT_UINT32_BE( ( orig_add_len >> 32 ), work_buf, 0  );
        MBEDTLS_PUT_UINT32_BE( ( orig_add_len       ), work_buf, 4  );
        MBEDTLS_PUT_UINT32_BE( ( orig_len     >> 32 ), work_buf, 8  );
        MBEDTLS_PUT_UINT32_BE( ( orig_len           ), work_buf, 12 );

        for( i = 0; i < 16; i++ )
            ctx->buf[i] ^= work_buf[i];

        gcm_mult( ctx, ctx->buf, ctx->buf );

        for( i = 0; i < tag_len; i++ )
            tag[i] ^= ctx->buf[i];
    }

    return( 0 );
}

int mbedtls_gcm_finish( mbedtls_gcm_context *ctx,
                     unsigned char *output, size_t output_size,
                     size_t *output_length,
                     unsigned char *tag, size_t tag_len )
{
    int ret = 0;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( tag != NULL );

    /* We never pass any output in finish(). The output parameter exists only
     * for the sake of alternative implementations. */
    (void) output;
    (void) output_size;
    *output_length = 0;

    if( tag_len > 16 || tag_len < 4 )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_gcm_finish( ctx->armgcm, tag, tag_len );
        break;
        default:
            return sw_compatible_mbedtls_gcm_finish( ctx, output, output_size,
                                                    output_length, tag, tag_len );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int mbedtls_gcm_crypt_and_tag( mbedtls_gcm_context *ctx,
                       int mode,
                       size_t length,
                       const unsigned char *iv,
                       size_t iv_len,
                       const unsigned char *add,
                       size_t add_len,
                       const unsigned char *input,
                       unsigned char *output,
                       size_t tag_len,
                       unsigned char *tag )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t olen;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );
    GCM_VALIDATE_RET( tag != NULL );

    if( ( ret = mbedtls_gcm_starts( ctx, mode, iv, iv_len ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_gcm_update_ad( ctx, add, add_len ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_gcm_update( ctx, input, length, output, length, &olen ) ) != 0 ) {
        arm_ce_gcm_clear( ctx );
        return( ret );
    }

    if( ( ret = mbedtls_gcm_finish( ctx, NULL, 0, &olen, tag, tag_len ) ) != 0 ) {
        arm_ce_gcm_clear( ctx );
        return( ret );
    }

    return( 0 );
}

int mbedtls_gcm_auth_decrypt( mbedtls_gcm_context *ctx,
                      size_t length,
                      const unsigned char *iv,
                      size_t iv_len,
                      const unsigned char *add,
                      size_t add_len,
                      const unsigned char *tag,
                      size_t tag_len,
                      const unsigned char *input,
                      unsigned char *output )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char check_tag[16];
    size_t i;
    int diff;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( tag != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );

    if( ( ret = mbedtls_gcm_crypt_and_tag( ctx, MBEDTLS_GCM_DECRYPT, length,
                                   iv, iv_len, add, add_len,
                                   input, output, tag_len, check_tag ) ) != 0 )
    {
        return( ret );
    }

    /* Check tag in "constant-time" */
    for( diff = 0, i = 0; i < tag_len; i++ )
        diff |= tag[i] ^ check_tag[i];

    if( diff != 0 )
    {
        mbedtls_platform_zeroize( output, length );
        return( MBEDTLS_ERR_GCM_AUTH_FAILED );
    }

    return( 0 );
}

/*
* Free context
*/
void mbedtls_gcm_free( mbedtls_gcm_context *ctx )
{
    if ( ( NULL == ctx ) || ( NULL == ctx->armgcm ) ) {
        return;
    }
    mbedtls_cipher_free( &ctx->cipher_ctx );
    arm_ce_gcm_free( ctx->armgcm );
    mbedtls_free( ctx->armgcm );
    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_gcm_context ) );
}

#endif /* MBEDTLS_GCM_ALT */
#endif
