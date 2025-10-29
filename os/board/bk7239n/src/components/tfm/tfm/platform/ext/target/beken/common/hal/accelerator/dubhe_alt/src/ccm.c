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
 * Definition of CCM:
 * http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf
 * RFC 3610 "Counter with CBC-MAC (CCM)"
 *
 * Related:
 * RFC 5116 "An Interface and Algorithms for Authenticated Encryption"
 */

#include "common.h"

#if defined(MBEDTLS_CCM_C)

#include "mbedtls/ccm.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include "dubhe_sca.h"
#include "dubhe_ccm.h"

#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */
#endif /* MBEDTLS_PLATFORM_C */

#if defined(MBEDTLS_CCM_ALT)

static inline int arm_ce_errno_to_mbedtls(int errno) {
    switch (errno)
    {
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
    case DBH_SCA_KEYBITS_INVAL_ERR:
        errno = MBEDTLS_ERR_CCM_BAD_INPUT;
        break;
    case DBH_CCM_ERR_OOM:
        errno = MBEDTLS_ERR_CCM_OOM;
        break;
    default:
        errno = MBEDTLS_ERR_CCM_HW_ACCEL_FAILED;
        break;
    };

    return errno;
}

static void arm_ce_ccm_clear( mbedtls_ccm_context *ctx, size_t tag_len )
{
    unsigned char tag[16] = {0};

    arm_ce_ccm_finish( ctx->armccm, tag, tag_len );
    (void)(tag);
}

/*
 * Initialize context
 */
void mbedtls_ccm_init( mbedtls_ccm_context *ctx )
{
    arm_ce_ccm_ctx_t *armccm = NULL;

    if ( NULL == ctx ) {
        return;
    }
    memset( ctx, 0, sizeof( mbedtls_ccm_context ) );
    armccm = mbedtls_calloc(1, sizeof(*armccm));
    if ( NULL == armccm ) {
        return;
    }
    ctx->armccm = armccm;
}


int sw_compatible_mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                                      mbedtls_cipher_id_t cipher,
                                      const unsigned char *key,
                                      unsigned int keybits )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const mbedtls_cipher_info_t *cipher_info;

    cipher_info = mbedtls_cipher_info_from_values( cipher, keybits,
                                                   MBEDTLS_MODE_ECB );
    if( cipher_info == NULL )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( cipher_info->block_size != 16 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    mbedtls_cipher_free( &ctx->cipher_ctx );

    if( ( ret = mbedtls_cipher_setup( &ctx->cipher_ctx, cipher_info ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_cipher_setkey( &ctx->cipher_ctx, key, keybits,
                               MBEDTLS_ENCRYPT ) ) != 0 )
    {
        return( ret );
    }

    return( 0 );
}

int sw_compatible_mbedtls_ccm_set_derived_key( mbedtls_ccm_context *ctx,
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

    return( MBEDTLS_ERR_CCM_BAD_INPUT );
}

int mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
    int ret;
    mbedtls_ccm_context swap = {0};
    int32_t alg = 0;

    if ( ( NULL == ctx ) || ( NULL == ctx->armccm ) ||
         ( NULL == key )) {
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    }

    switch (cipher)
    {
    case MBEDTLS_CIPHER_ID_AES:
        alg = ARM_CE_AES_CCM;
        break;
#if defined(ARM_CE_DUBHE_SCA)
    case MBEDTLS_CIPHER_ID_SM4:
        alg = ARM_CE_SM4_CCM;
        break;
#endif
    default:
        return sw_compatible_mbedtls_ccm_setkey( ctx, cipher, key, keybits );
    }
    /** check if setkey previously,if not shoud init first */
    if ( MBEDTLS_CIPHER_ID_NONE == ctx->cipher ) {
        ret = arm_ce_ccm_init( ctx->armccm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        ctx->cipher = cipher;
        ret = arm_ce_ccm_set_key( ctx->armccm, key, keybits );
    } else if ( cipher != ctx->cipher) {
    /** change cipher then free previous and start with the new one */
        mbedtls_ccm_init( &swap );
        if (NULL == swap.armccm) {
            return MBEDTLS_ERR_CCM_OOM;
        }
        ret = arm_ce_ccm_init( swap.armccm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        swap.cipher = cipher;
        ret = arm_ce_ccm_set_key( swap.armccm, key, keybits );
        if ( 0 == ret ) {
            mbedtls_ccm_free( ctx );
            memcpy( ctx, &swap, sizeof(*ctx) );
            return 0;
        } else {
            mbedtls_ccm_free( &swap );
        }
    } else { /** same cipher just setkey */
        ret = arm_ce_ccm_set_key( ctx->armccm, key, keybits );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int mbedtls_ccm_set_derived_key( mbedtls_ccm_context *ctx,
                                 mbedtls_cipher_id_t cipher,
                                 int key_type,
                                 const unsigned char *ek1,
                                 const unsigned char *ek2,
                                 const unsigned char *ek3,
                                 unsigned int ek1bits )
{
#define GET_ARM_CE_KEY_TYPE(type) ( (MBEDTLS_CCM_DERIVED_MODEL_KEY == (type)) ?\
                            ARM_CE_SCA_MODEL_KEY : ARM_CE_SCA_DEVICE_ROOT_KEY )
    int ret = 0;
    mbedtls_ccm_context swap = {0};
    int32_t alg = 0;

    if ( ( NULL == ctx ) || ( NULL == ctx->armccm ) ||
         ( NULL == ek1 ) || ( NULL == ek2 ) || ( NULL == ek3 ) ||
         ( ( MBEDTLS_CCM_DERIVED_MODEL_KEY != key_type ) &&
           ( MBEDTLS_CCM_DERIVED_DEVICE_ROOT_KEY != key_type ) ) ) {
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    }

    switch ( cipher )
    {
    case MBEDTLS_CIPHER_ID_AES:
        alg = ARM_CE_AES_CCM;
        break;
    case MBEDTLS_CIPHER_ID_SM4:
        alg = ARM_CE_SM4_CCM;
        break;
    default:
        return sw_compatible_mbedtls_ccm_set_derived_key( ctx,
                                            GET_ARM_CE_KEY_TYPE( key_type ),
                                                         ek1, ek2, ek3,
                                                         ek1bits );
    }
    /** check if setkey previously,if not shoud init first */
    if ( MBEDTLS_CIPHER_ID_NONE == ctx->cipher ) {
        ret = arm_ce_ccm_init( ctx->armccm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        ctx->cipher = cipher;
        ret = arm_ce_ccm_set_derived_key( ctx->armccm,
                                     GET_ARM_CE_KEY_TYPE( key_type ),
                                     ek1, ek2, ek3, ek1bits );
    } else if ( cipher != ctx->cipher) {
    /** change cipher then free previous and start with the new one */
        mbedtls_ccm_init( &swap );
        if (NULL == swap.armccm) {
            return MBEDTLS_ERR_CCM_OOM;
        }
        ret = arm_ce_ccm_init( swap.armccm, alg );
        if ( ret != 0 ) {
            return arm_ce_errno_to_mbedtls( ret );
        }
        swap.cipher = cipher;
        ret = arm_ce_ccm_set_derived_key( swap.armccm,
                                     GET_ARM_CE_KEY_TYPE( key_type ),
                                     ek1, ek2, ek3, ek1bits );
        if ( 0 == ret ) {
            mbedtls_ccm_free( ctx );
            memcpy( ctx, &swap, sizeof(*ctx) );
            return 0;
        } else {
            mbedtls_ccm_free( &swap );
        }
    } else { /** same cipher just setkey */
        ret = arm_ce_ccm_set_derived_key( ctx->armccm,
                                     GET_ARM_CE_KEY_TYPE( key_type ),
                                     ek1, ek2, ek3, ek1bits );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

/*
 * Free context
 */
void mbedtls_ccm_free( mbedtls_ccm_context *ctx )
{
    if ( ( NULL == ctx ) || ( NULL == ctx->armccm ) ) {
        return;
    }
    mbedtls_cipher_free( &ctx->cipher_ctx );
    arm_ce_ccm_free( ctx->armccm );
    mbedtls_free( ctx->armccm );
    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_ccm_context ) );
}

#define CCM_STATE__CLEAR                0
#define CCM_STATE__STARTED              (1 << 0)
#define CCM_STATE__LENGHTS_SET          (1 << 1)
#define CCM_STATE__AUTH_DATA_STARTED    (1 << 2)
#define CCM_STATE__AUTH_DATA_FINISHED   (1 << 3)
#define CCM_STATE__ERROR                (1 << 4)

/*
 * Encrypt or decrypt a partial block with CTR
 */
static int mbedtls_ccm_crypt( mbedtls_ccm_context *ctx,
                              size_t offset, size_t use_len,
                              const unsigned char *input,
                              unsigned char *output )
{
    size_t i;
    size_t olen = 0;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char tmp_buf[16] = {0};

    if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, ctx->ctr, 16, tmp_buf,
                                       &olen ) ) != 0 )
    {
        ctx->state |= CCM_STATE__ERROR;
        mbedtls_platform_zeroize(tmp_buf, sizeof(tmp_buf));
        return ret;
    }

    for( i = 0; i < use_len; i++ )
        output[i] = input[i] ^ tmp_buf[offset + i];

    mbedtls_platform_zeroize(tmp_buf, sizeof(tmp_buf));
    return ret;
}

static void mbedtls_ccm_clear_state(mbedtls_ccm_context *ctx) {
    ctx->state = CCM_STATE__CLEAR;
    memset( ctx->y, 0, 16);
    memset( ctx->ctr, 0, 16);
}

static int ccm_calculate_first_block_if_ready(mbedtls_ccm_context *ctx)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char i;
    size_t len_left, olen;

    /* length calulcation can be done only after both
     * mbedtls_ccm_starts() and mbedtls_ccm_set_lengths() have been executed
     */
    if( !(ctx->state & CCM_STATE__STARTED) || !(ctx->state & CCM_STATE__LENGHTS_SET) )
        return 0;

    /* CCM expects non-empty tag.
     * CCM* allows empty tag. For CCM* without tag, ignore plaintext length.
     */
    if( ctx->tag_len == 0 )
    {
        if( ctx->mode == MBEDTLS_CCM_STAR_ENCRYPT || ctx->mode == MBEDTLS_CCM_STAR_DECRYPT )
        {
            ctx->plaintext_len = 0;
        }
        else
        {
            return( MBEDTLS_ERR_CCM_BAD_INPUT );
        }
    }

    /*
     * First block:
     * 0        .. 0        flags
     * 1        .. iv_len   nonce (aka iv)  - set by: mbedtls_ccm_starts()
     * iv_len+1 .. 15       length
     *
     * With flags as (bits):
     * 7        0
     * 6        add present?
     * 5 .. 3   (t - 2) / 2
     * 2 .. 0   q - 1
     */
    ctx->y[0] |= ( ctx->add_len > 0 ) << 6;
    ctx->y[0] |= ( ( ctx->tag_len - 2 ) / 2 ) << 3;
    ctx->y[0] |= ctx->q - 1;

    for( i = 0, len_left = ctx->plaintext_len; i < ctx->q; i++, len_left >>= 8 )
        ctx->y[15-i] = MBEDTLS_BYTE_0( len_left );

    if( len_left > 0 )
    {
        ctx->state |= CCM_STATE__ERROR;
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    }

    /* Start CBC-MAC with first block*/
    if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, ctx->y, 16, ctx->y, &olen ) ) != 0 )
    {
        ctx->state |= CCM_STATE__ERROR;
        return( ret );
    }

    return (0);
}

int sw_compatible_mbedtls_ccm_starts( mbedtls_ccm_context *ctx,
                        int mode,
                        const unsigned char *iv,
                        size_t iv_len )
{
    /* Also implies q is within bounds */
    if( iv_len < 7 || iv_len > 13 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    ctx->mode = mode;
    ctx->q = 16 - 1 - (unsigned char) iv_len;

    /*
     * Prepare counter block for encryption:
     * 0        .. 0        flags
     * 1        .. iv_len   nonce (aka iv)
     * iv_len+1 .. 15       counter (initially 1)
     *
     * With flags as (bits):
     * 7 .. 3   0
     * 2 .. 0   q - 1
     */
    memset( ctx->ctr, 0, 16);
    ctx->ctr[0] = ctx->q - 1;
    memcpy( ctx->ctr + 1, iv, iv_len );
    memset( ctx->ctr + 1 + iv_len, 0, ctx->q );
    ctx->ctr[15] = 1;

    /*
     * See ccm_calculate_first_block_if_ready() for block layout description
     */
    memcpy( ctx->y + 1, iv, iv_len );

    ctx->state |= CCM_STATE__STARTED;
    return ccm_calculate_first_block_if_ready(ctx);
}

int mbedtls_ccm_starts( mbedtls_ccm_context *ctx,
                        int mode,
                        const unsigned char *iv,
                        size_t iv_len )
{
    int ret = 0;

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_ccm_start( ctx->armccm,
                                    (mode == MBEDTLS_CCM_ENCRYPT || mode == MBEDTLS_CCM_STAR_ENCRYPT) ?
                                        DBH_CCM_ENCRYPT : DBH_CCM_DECRYPT,
                                    iv,
                                    iv_len,
                                    ctx->tag_len,
                                    ctx->add_len,
                                    ctx->plaintext_len );
            break;
        default:
            return sw_compatible_mbedtls_ccm_starts( ctx, mode, iv, iv_len );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int sw_compatible_mbedtls_ccm_set_lengths( mbedtls_ccm_context *ctx,
                             size_t total_ad_len,
                             size_t plaintext_len,
                             size_t tag_len )
{
    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     *
     * Also, loosen the requirements to enable support for CCM* (IEEE 802.15.4).
     */
    if( tag_len == 2 || tag_len > 16 || tag_len % 2 != 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( total_ad_len >= 0xFF00 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    ctx->plaintext_len = plaintext_len;
    ctx->add_len = total_ad_len;
    ctx->tag_len = tag_len;
    ctx->processed = 0;

    ctx->state |= CCM_STATE__LENGHTS_SET;
    return ccm_calculate_first_block_if_ready(ctx);
}

int mbedtls_ccm_set_lengths( mbedtls_ccm_context *ctx,
                             size_t total_ad_len,
                             size_t plaintext_len,
                             size_t tag_len )
{
    /*
    * Check length requirements: SP800-38C A.1
    * Additional requirement: a < 2^16 - 2^8 to simplify the code.
    * 'length' checked later (when writing it to the first block)
    *
    * Also, loosen the requirements to enable support for CCM* (IEEE 802.15.4).
    */
    if( tag_len == 2 || tag_len > 16 || tag_len % 2 != 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( total_ad_len >= 0xFF00 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ctx->plaintext_len = plaintext_len;
            ctx->add_len = total_ad_len;
            ctx->tag_len = tag_len;
            break;
        default:
            return sw_compatible_mbedtls_ccm_set_lengths( ctx, total_ad_len, plaintext_len, tag_len );
    }

    return 0;
}

int sw_compatible_mbedtls_ccm_update_ad( mbedtls_ccm_context *ctx,
                           const unsigned char *add,
                           size_t add_len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char i;
    size_t olen, use_len, offset;

    if( ctx->state & CCM_STATE__ERROR )
    {
        return MBEDTLS_ERR_CCM_BAD_INPUT;
    }

    if( add_len > 0 )
    {
        if( ctx->state & CCM_STATE__AUTH_DATA_FINISHED )
        {
            return MBEDTLS_ERR_CCM_BAD_INPUT;
        }

        if( !(ctx->state & CCM_STATE__AUTH_DATA_STARTED) )
        {
            if ( add_len > ctx->add_len )
            {
                return MBEDTLS_ERR_CCM_BAD_INPUT;
            }

            ctx->y[0] ^= (unsigned char)( ( ctx->add_len >> 8 ) & 0xFF );
            ctx->y[1] ^= (unsigned char)( ( ctx->add_len      ) & 0xFF );

            ctx->state |= CCM_STATE__AUTH_DATA_STARTED;
        }
        else if ( ctx->processed + add_len > ctx->add_len )
        {
            return MBEDTLS_ERR_CCM_BAD_INPUT;
        }

        while( add_len > 0 )
        {
            offset = (ctx->processed + 2) % 16; /* account for y[0] and y[1]
                                                 * holding total auth data length */
            use_len = 16 - offset;

            if( use_len > add_len )
                use_len = add_len;

            for( i = 0; i < use_len; i++ )
                ctx->y[i + offset] ^= add[i];

            ctx->processed += use_len;
            add_len -= use_len;
            add += use_len;

            if( use_len + offset == 16 || ctx->processed == ctx->add_len )
            {
                if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, ctx->y, 16, ctx->y, &olen ) ) != 0 )
                {
                    ctx->state |= CCM_STATE__ERROR;
                    return( ret );
                }
            }
        }

        if( ctx->processed == ctx->add_len )
        {
            ctx->state |= CCM_STATE__AUTH_DATA_FINISHED;
            ctx->processed = 0; // prepare for mbedtls_ccm_update()
        }
    }

    return (0);
}

int mbedtls_ccm_update_ad( mbedtls_ccm_context *ctx,
                           const unsigned char *add,
                           size_t add_len )
{
    int ret = 0;

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_ccm_update_aad( ctx->armccm, add_len, add );
            break;
        default:
            return sw_compatible_mbedtls_ccm_update_ad( ctx, add, add_len );
    }

    if ( 0 != ret ) {
        arm_ce_ccm_clear( ctx, ctx->tag_len );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int sw_compatible_mbedtls_ccm_update( mbedtls_ccm_context *ctx,
                        const unsigned char *input, size_t input_len,
                        unsigned char *output, size_t output_size,
                        size_t *output_len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char i;
    size_t use_len, offset, olen;

    unsigned char local_output[16];

    if( ctx->state & CCM_STATE__ERROR )
    {
        return MBEDTLS_ERR_CCM_BAD_INPUT;
    }

    /* Check against plaintext length only if performing operation with
     * authentication
     */
    if( ctx->tag_len != 0 && ctx->processed + input_len > ctx->plaintext_len )
    {
        return MBEDTLS_ERR_CCM_BAD_INPUT;
    }

    if( output_size < input_len )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    *output_len = input_len;

    ret = 0;

    while ( input_len > 0 )
    {
        offset = ctx->processed % 16;

        use_len = 16 - offset;

        if( use_len > input_len )
            use_len = input_len;

        ctx->processed += use_len;

        if( ctx->mode == MBEDTLS_CCM_ENCRYPT || \
            ctx->mode == MBEDTLS_CCM_STAR_ENCRYPT )
        {
            for( i = 0; i < use_len; i++ )
                ctx->y[i + offset] ^= input[i];

            if( use_len + offset == 16 || ctx->processed == ctx->plaintext_len )
            {
                if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, ctx->y, 16, ctx->y, &olen ) ) != 0 )
                {
                    ctx->state |= CCM_STATE__ERROR;
                    goto exit;
                }
            }

            ret = mbedtls_ccm_crypt( ctx, offset, use_len, input, output );
            if( ret != 0 )
                goto exit;
        }

        if( ctx->mode == MBEDTLS_CCM_DECRYPT || \
            ctx->mode == MBEDTLS_CCM_STAR_DECRYPT )
        {
            /* Since output may be in shared memory, we cannot be sure that
             * it will contain what we wrote to it. Therefore, we should avoid using
             * it as input to any operations.
             * Write decrypted data to local_output to avoid using output variable as
             * input in the XOR operation for Y.
             */
            ret = mbedtls_ccm_crypt( ctx, offset, use_len, input, local_output );
            if( ret != 0 )
                goto exit;

            for( i = 0; i < use_len; i++ )
                ctx->y[i + offset] ^= local_output[i];

            memcpy( output, local_output, use_len );
            mbedtls_platform_zeroize( local_output, 16 );

            if( use_len + offset == 16 || ctx->processed == ctx->plaintext_len )
            {
                if( ( ret = mbedtls_cipher_update( &ctx->cipher_ctx, ctx->y, 16, ctx->y, &olen ) ) != 0 )
                {
                    ctx->state |= CCM_STATE__ERROR;
                    goto exit;
                }
            }
        }

        if( use_len + offset == 16 || ctx->processed == ctx->plaintext_len )
        {
            for( i = 0; i < ctx->q; i++ )
            if( ++(ctx->ctr)[15-i] != 0 )
                break;
        }

        input_len -= use_len;
        input += use_len;
        output += use_len;
    }

exit:
    mbedtls_platform_zeroize( local_output, 16 );

    return ret;
}

int mbedtls_ccm_update( mbedtls_ccm_context *ctx,
                        const unsigned char *input, size_t input_len,
                        unsigned char *output, size_t output_size,
                        size_t *output_len )
{
    int ret = 0;

    if( output_size < input_len )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    *output_len = input_len;

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_ccm_update( ctx->armccm, input_len, input, output );
            break;
        default:
            return sw_compatible_mbedtls_ccm_update( ctx, input, input_len,
                                                output, output_size, output_len );
            break;
    }

    if ( 0 != ret ) {
        arm_ce_ccm_clear( ctx, ctx->tag_len );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

int sw_compatible_mbedtls_ccm_finish( mbedtls_ccm_context *ctx,
                        unsigned char *tag, size_t tag_len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char i;

    if( ctx->state & CCM_STATE__ERROR )
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    if( ctx->add_len > 0 && !( ctx->state & CCM_STATE__AUTH_DATA_FINISHED ) )
    {
        return MBEDTLS_ERR_CCM_BAD_INPUT;
    }

    if( ctx->plaintext_len > 0 && ctx->processed != ctx->plaintext_len )
    {
        return MBEDTLS_ERR_CCM_BAD_INPUT;
    }

    /*
     * Authentication: reset counter and crypt/mask internal tag
     */
    for( i = 0; i < ctx->q; i++ )
        ctx->ctr[15-i] = 0;

    ret = mbedtls_ccm_crypt( ctx, 0, 16, ctx->y, ctx->y );
    if( ret != 0 )
        return ret;
    if( tag != NULL )
        memcpy( tag, ctx->y, tag_len );
    mbedtls_ccm_clear_state(ctx);

    return( 0 );
}

int mbedtls_ccm_finish( mbedtls_ccm_context *ctx,
                        unsigned char *tag, size_t tag_len )
{
    int ret = 0;

    switch (ctx->cipher) {
        case MBEDTLS_CIPHER_ID_AES:
        case MBEDTLS_CIPHER_ID_SM4:
            ret = arm_ce_ccm_finish( ctx->armccm, tag, tag_len );
            break;
        default:
            return sw_compatible_mbedtls_ccm_finish( ctx, tag, tag_len );
    }

    if ( 0 != ret ) {
        arm_ce_ccm_clear( ctx, ctx->tag_len );
    }

    return arm_ce_errno_to_mbedtls( ret );
}

/*
 * Authenticated encryption or decryption
 */
static int ccm_auth_crypt( mbedtls_ccm_context *ctx, int mode, size_t length,
                           const unsigned char *iv, size_t iv_len,
                           const unsigned char *add, size_t add_len,
                           const unsigned char *input, unsigned char *output,
                           unsigned char *tag, size_t tag_len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t olen;

    if( ( ret = mbedtls_ccm_set_lengths( ctx, add_len, length, tag_len ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_ccm_starts( ctx, mode, iv, iv_len ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_ccm_update_ad( ctx, add, add_len ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_ccm_update( ctx, input, length,
                                    output, length, &olen ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_ccm_finish( ctx, tag, tag_len ) ) != 0 )
        return( ret );

    return( 0 );
}

/*
 * Authenticated encryption
 */
int mbedtls_ccm_star_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                         const unsigned char *iv, size_t iv_len,
                         const unsigned char *add, size_t add_len,
                         const unsigned char *input, unsigned char *output,
                         unsigned char *tag, size_t tag_len )
{
    return( ccm_auth_crypt( ctx, MBEDTLS_CCM_STAR_ENCRYPT, length, iv, iv_len,
                            add, add_len, input, output, tag, tag_len ) );
}

int mbedtls_ccm_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                         const unsigned char *iv, size_t iv_len,
                         const unsigned char *add, size_t add_len,
                         const unsigned char *input, unsigned char *output,
                         unsigned char *tag, size_t tag_len )
{
    return( ccm_auth_crypt( ctx, MBEDTLS_CCM_ENCRYPT, length, iv, iv_len,
                            add, add_len, input, output, tag, tag_len ) );
}

/*
* Authenticated decryption
*/
static int mbedtls_ccm_compare_tags(const unsigned char *tag1, const unsigned char *tag2, size_t tag_len)
{
    unsigned char i;
    int diff;

    /* Check tag in "constant-time" */
    for( diff = 0, i = 0; i < tag_len; i++ )
        diff |= tag1[i] ^ tag2[i];

    if( diff != 0 )
    {
        return( MBEDTLS_ERR_CCM_AUTH_FAILED );
    }

    return( 0 );
}

static int ccm_auth_decrypt( mbedtls_ccm_context *ctx, int mode, size_t length,
                             const unsigned char *iv, size_t iv_len,
                             const unsigned char *add, size_t add_len,
                             const unsigned char *input, unsigned char *output,
                             const unsigned char *tag, size_t tag_len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char check_tag[16];

    if( ( ret = ccm_auth_crypt( ctx, mode, length,
                                iv, iv_len, add, add_len,
                                input, output, check_tag, tag_len ) ) != 0 )
    {
        return( ret );
    }

    if( ( ret = mbedtls_ccm_compare_tags( tag, check_tag, tag_len ) ) != 0 )
    {
        mbedtls_platform_zeroize( output, length );
        return( ret );
    }

    return( 0 );
}

int mbedtls_ccm_star_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                      const unsigned char *iv, size_t iv_len,
                      const unsigned char *add, size_t add_len,
                      const unsigned char *input, unsigned char *output,
                      const unsigned char *tag, size_t tag_len )
{
    return ccm_auth_decrypt( ctx, MBEDTLS_CCM_STAR_DECRYPT, length,
                             iv, iv_len, add, add_len,
                             input, output, tag, tag_len );
}

int mbedtls_ccm_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                      const unsigned char *iv, size_t iv_len,
                      const unsigned char *add, size_t add_len,
                      const unsigned char *input, unsigned char *output,
                      const unsigned char *tag, size_t tag_len )
{
    return ccm_auth_decrypt( ctx, MBEDTLS_CCM_DECRYPT, length,
                             iv, iv_len, add, add_len,
                             input, output, tag, tag_len );
}

#endif /* MBEDTLS_CCM_ALT */
#endif
