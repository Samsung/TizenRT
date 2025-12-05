/**
 * \file cmac.c
 *
 * \brief NIST SP800-38B compliant CMAC implementation for AES and 3DES
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

/*
 * References:
 *
 * - NIST SP 800-38B Recommendation for Block Cipher Modes of Operation: The
 *      CMAC Mode for Authentication
 *   http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-38b.pdf
 *
 * - RFC 4493 - The AES-CMAC Algorithm
 *   https://tools.ietf.org/html/rfc4493
 *
 * - RFC 4615 - The Advanced Encryption Standard-Cipher-based Message
 *      Authentication Code-Pseudo-Random Function-128 (AES-CMAC-PRF-128)
 *      Algorithm for the Internet Key Exchange Protocol (IKE)
 *   https://tools.ietf.org/html/rfc4615
 *
 *   Additional test vectors: ISO/IEC 9797-1
 *
 */

#include "common.h"

#if defined( MBEDTLS_CMAC_C )

#include "mbedtls/cmac.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"
#include "mbedtls/platform.h"

#include <string.h>

#if defined( MBEDTLS_CMAC_ALT )

static int _convert_retval_to_mbedtls( int errno )
{
    int ret = 0;

    switch ( errno )
    {
        case DBH_SCA_MODE_ERR:
        case DBH_SCA_KEYBITS_INVAL_ERR:
        case DBH_SCA_KEY_TYPE_INVAL_ERR:
        case DBH_SCA_DATA_ALIGN_ERR:
        case DBH_SCA_PARAM_INVAL_ERR:
        case DBH_SCA_MAC_TYPE_ERR:
            ret = MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA;
            break;
        case DBH_CMAC_BAD_STATE_ERR:
        case DBH_SCA_MAC_INVAL_ERR:
            ret = MBEDTLS_ERR_CIPHER_INVALID_CONTEXT;
            break;
        case DBH_SCA_BUS_ERR:
        case DBH_SCA_INVALID_CMD_ERR:
            ret = MBEDTLS_ERR_CMAC_HW_ACCEL_FAILED;
            break;
        default:
            break;
    }

    return ret;
}

/*
 * Multiplication by u in the Galois field of GF(2^n)
 *
 * As explained in NIST SP 800-38B, this can be computed:
 *
 *   If MSB(p) = 0, then p = (p << 1)
 *   If MSB(p) = 1, then p = (p << 1) ^ R_n
 *   with R_64 = 0x1B and  R_128 = 0x87
 *
 * Input and output MUST NOT point to the same buffer
 * Block size must be 8 bytes or 16 bytes - the block sizes for DES and AES.
 */
static int cmac_multiply_by_u( unsigned char *output,
                               const unsigned char *input,
                               size_t blocksize )
{
    const unsigned char R_128 = 0x87;
    const unsigned char R_64 = 0x1B;
    unsigned char R_n, mask;
    unsigned char overflow = 0x00;
    int i;

    if( blocksize == MBEDTLS_AES_BLOCK_SIZE )
    {
        R_n = R_128;
    }
    else if( blocksize == MBEDTLS_DES3_BLOCK_SIZE )
    {
        R_n = R_64;
    }
    else
    {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    for( i = (int)blocksize - 1; i >= 0; i-- )
    {
        output[i] = input[i] << 1 | overflow;
        overflow = input[i] >> 7;
    }

    /* mask = ( input[0] >> 7 ) ? 0xff : 0x00
     * using bit operations to avoid branches */

    /* MSVC has a warning about unary minus on unsigned, but this is
     * well-defined and precisely what we want to do here */
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4146 )
#endif
    mask = - ( input[0] >> 7 );
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

    output[ blocksize - 1 ] ^= R_n & mask;

    return( 0 );
}

/*
 * Generate subkeys
 *
 * - as specified by RFC 4493, section 2.3 Subkey Generation Algorithm
 */
static int cmac_generate_subkeys( mbedtls_cipher_context_t *ctx,
                                  unsigned char* K1, unsigned char* K2 )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char L[MBEDTLS_CIPHER_BLKSIZE_MAX];
    size_t olen, block_size;

    mbedtls_platform_zeroize( L, sizeof( L ) );

    block_size = ctx->cipher_info->block_size;

    /* Calculate Ek(0) */
    if( ( ret = mbedtls_cipher_update( ctx, L, block_size, L, &olen ) ) != 0 )
        goto exit;

    /*
     * Generate K1 and K2
     */
    if( ( ret = cmac_multiply_by_u( K1, L , block_size ) ) != 0 )
        goto exit;

    if( ( ret = cmac_multiply_by_u( K2, K1 , block_size ) ) != 0 )
        goto exit;

exit:
    mbedtls_platform_zeroize( L, sizeof( L ) );

    return( ret );
}

static void cmac_xor_block( unsigned char *output, const unsigned char *input1,
                            const unsigned char *input2,
                            const size_t block_size )
{
    size_t idx;

    for( idx = 0; idx < block_size; idx++ )
        output[ idx ] = input1[ idx ] ^ input2[ idx ];
}

/*
 * Create padded last block from (partial) last block.
 *
 * We can't use the padding option from the cipher layer, as it only works for
 * CBC and we use ECB mode, and anyway we need to XOR K1 or K2 in addition.
 */
static void cmac_pad( unsigned char padded_block[MBEDTLS_CIPHER_BLKSIZE_MAX],
                      size_t padded_block_len,
                      const unsigned char *last_block,
                      size_t last_block_len )
{
    size_t j;

    for( j = 0; j < padded_block_len; j++ )
    {
        if( j < last_block_len )
            padded_block[j] = last_block[j];
        else if( j == last_block_len )
            padded_block[j] = 0x80;
        else
            padded_block[j] = 0x00;
    }
}

int sw_mbedtls_cmac_starts( mbedtls_cipher_context_t *ctx,
                            const unsigned char *key, size_t keybits )
{
    mbedtls_cipher_type_t type;
    mbedtls_cmac_context_t *cmac_ctx;
    int retval;

    if( ctx == NULL || ctx->cipher_info == NULL || key == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    if( ( retval = mbedtls_cipher_setkey( ctx, key, (int)keybits,
                                          MBEDTLS_ENCRYPT ) ) != 0 )
        return( retval );

    type = ctx->cipher_info->type;

    switch( type )
    {
        case MBEDTLS_CIPHER_AES_128_ECB:
        case MBEDTLS_CIPHER_AES_192_ECB:
        case MBEDTLS_CIPHER_AES_256_ECB:
        case MBEDTLS_CIPHER_DES_EDE3_ECB:
            break;
        default:
            return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    /* Allocated and initialise in the cipher context memory for the CMAC
     * context */
    cmac_ctx = mbedtls_calloc( 1, sizeof( mbedtls_cmac_context_t ) );
    if( cmac_ctx == NULL )
        return( MBEDTLS_ERR_CIPHER_ALLOC_FAILED );

    ctx->cmac_ctx = cmac_ctx;

    mbedtls_platform_zeroize( cmac_ctx->state, sizeof( cmac_ctx->state ) );

    return 0;
}

int sw_mbedtls_cmac_starts_with_derived_key( mbedtls_cipher_context_t *ctx,
                                             int key_type,
                                             const unsigned char *ek1,
                                             const unsigned char *ek2,
                                             const unsigned char *ek3,
                                             unsigned int ek1bits )
{
    ( void )ctx;
    ( void )key_type;
    ( void )ek1;
    ( void )ek2;
    ( void )ek3;
    ( void )ek1bits;
    return MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE;
}

int sw_mbedtls_cmac_update( mbedtls_cipher_context_t *ctx,
                            const unsigned char *input, size_t ilen )
{
    mbedtls_cmac_context_t* cmac_ctx;
    unsigned char *state;
    int ret = 0;
    size_t n, j, olen, block_size;

    if( ctx == NULL || ctx->cipher_info == NULL || input == NULL ||
        ctx->cmac_ctx == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    cmac_ctx = ctx->cmac_ctx;
    block_size = ctx->cipher_info->block_size;
    state = ctx->cmac_ctx->state;

    /* Is there data still to process from the last call, that's greater in
     * size than a block? */
    if( cmac_ctx->unprocessed_len > 0 &&
        ilen > block_size - cmac_ctx->unprocessed_len )
    {
        memcpy( &cmac_ctx->unprocessed_block[cmac_ctx->unprocessed_len],
                input,
                block_size - cmac_ctx->unprocessed_len );

        cmac_xor_block( state, cmac_ctx->unprocessed_block, state, block_size );

        if( ( ret = mbedtls_cipher_update( ctx, state, block_size, state,
                                           &olen ) ) != 0 )
        {
           goto exit;
        }

        input += block_size - cmac_ctx->unprocessed_len;
        ilen -= block_size - cmac_ctx->unprocessed_len;
        cmac_ctx->unprocessed_len = 0;
    }

    /* n is the number of blocks including any final partial block */
    n = ( ilen + block_size - 1 ) / block_size;

    /* Iterate across the input data in block sized chunks, excluding any
     * final partial or complete block */
    for( j = 1; j < n; j++ )
    {
        cmac_xor_block( state, input, state, block_size );

        if( ( ret = mbedtls_cipher_update( ctx, state, block_size, state,
                                           &olen ) ) != 0 )
           goto exit;

        ilen -= block_size;
        input += block_size;
    }

    /* If there is data left over that wasn't aligned to a block */
    if( ilen > 0 )
    {
        memcpy( &cmac_ctx->unprocessed_block[cmac_ctx->unprocessed_len],
                input,
                ilen );
        cmac_ctx->unprocessed_len += ilen;
    }

exit:
    return( ret );
}

int sw_mbedtls_cmac_finish( mbedtls_cipher_context_t *ctx,
                            unsigned char *output )
{
    mbedtls_cmac_context_t* cmac_ctx;
    unsigned char *state, *last_block;
    unsigned char K1[MBEDTLS_CIPHER_BLKSIZE_MAX];
    unsigned char K2[MBEDTLS_CIPHER_BLKSIZE_MAX];
    unsigned char M_last[MBEDTLS_CIPHER_BLKSIZE_MAX];
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t olen, block_size;

    if( ctx == NULL || ctx->cipher_info == NULL || ctx->cmac_ctx == NULL ||
        output == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    cmac_ctx = ctx->cmac_ctx;
    block_size = ctx->cipher_info->block_size;
    state = cmac_ctx->state;

    mbedtls_platform_zeroize( K1, sizeof( K1 ) );
    mbedtls_platform_zeroize( K2, sizeof( K2 ) );
    cmac_generate_subkeys( ctx, K1, K2 );

    last_block = cmac_ctx->unprocessed_block;

    /* Calculate last block */
    if( cmac_ctx->unprocessed_len < block_size )
    {
        cmac_pad( M_last, block_size, last_block, cmac_ctx->unprocessed_len );
        cmac_xor_block( M_last, M_last, K2, block_size );
    }
    else
    {
        /* Last block is complete block */
        cmac_xor_block( M_last, last_block, K1, block_size );
    }


    cmac_xor_block( state, M_last, state, block_size );
    if( ( ret = mbedtls_cipher_update( ctx, state, block_size, state,
                                       &olen ) ) != 0 )
    {
        goto exit;
    }

    memcpy( output, state, block_size );

exit:
    /* Wipe the generated keys on the stack, and any other transients to avoid
     * side channel leakage */
    mbedtls_platform_zeroize( K1, sizeof( K1 ) );
    mbedtls_platform_zeroize( K2, sizeof( K2 ) );

    cmac_ctx->unprocessed_len = 0;
    mbedtls_platform_zeroize( cmac_ctx->unprocessed_block,
                              sizeof( cmac_ctx->unprocessed_block ) );

    mbedtls_platform_zeroize( state, MBEDTLS_CIPHER_BLKSIZE_MAX );
    return( ret );
}

int sw_mbedtls_cmac_reset( mbedtls_cipher_context_t *ctx )
{
    mbedtls_cmac_context_t* cmac_ctx;

    if( ctx == NULL || ctx->cipher_info == NULL || ctx->cmac_ctx == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    cmac_ctx = ctx->cmac_ctx;

    /* Reset the internal state */
    cmac_ctx->unprocessed_len = 0;
    mbedtls_platform_zeroize( cmac_ctx->unprocessed_block,
                              sizeof( cmac_ctx->unprocessed_block ) );
    mbedtls_platform_zeroize( cmac_ctx->state,
                              sizeof( cmac_ctx->state ) );

    return( 0 );
}

int dubhe_cmac_starts( mbedtls_cipher_context_t *ctx,
                       const unsigned char *key, size_t keybits )
{
    int ret = 0;
    mbedtls_cmac_context_t *cmac_ctx = NULL;
    arm_ce_sca_alg_t malg = 0;
    if ( ctx == NULL || ctx->cipher_info == NULL || key == NULL ) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }
    if ( (ctx->cipher_info == NULL) || (keybits != ctx->cipher_info->key_bitlen) ) {
        return ( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    switch ( ctx->cipher_info->type )
    {
        case MBEDTLS_CIPHER_AES_128_ECB:
        case MBEDTLS_CIPHER_AES_192_ECB:
        case MBEDTLS_CIPHER_AES_256_ECB:
            malg = ARM_CE_AES;
            break;
        case MBEDTLS_CIPHER_SM4_ECB:
            malg = ARM_CE_SM4;
            break;
        case MBEDTLS_CIPHER_DES_EDE3_ECB:
        default:
            return sw_mbedtls_cmac_starts( ctx, key, keybits );
    }

    if ( ctx->cmac_ctx == NULL ||
         ctx->cmac_ctx->magic != MBEDTLS_CMAC_MAGIC ||
         ctx->cmac_ctx->cmac == NULL ) {
        cmac_ctx = ctx->cmac_ctx;
        /* allocate cmac context */
        cmac_ctx = (mbedtls_cmac_context_t *)mbedtls_calloc( 1,
                            sizeof(*cmac_ctx) + sizeof(arm_ce_cmac_context_t) );
        if ( cmac_ctx == NULL ) {
            return( MBEDTLS_ERR_CIPHER_ALLOC_FAILED );
        }
        cmac_ctx->cmac = (arm_ce_cmac_context_t *)( cmac_ctx + 1 );
        /* init cmac context */
        ret = arm_ce_cmac_init( cmac_ctx->cmac, malg );
        if ( ret ) {
            mbedtls_printf( "init cmac contex failed\n");
            goto __err__;
        }
        cmac_ctx->magic = MBEDTLS_CMAC_MAGIC;
        ctx->cmac_ctx = cmac_ctx;
    }

    ret = arm_ce_cmac_set_key( ctx->cmac_ctx->cmac,
                               key,
                               keybits );
    if ( ret ) {
        goto __err__;
    }

    ret = arm_ce_cmac_start( ctx->cmac_ctx->cmac );
    if ( ret ) {
        goto __err__;
    } else {
        goto __out__;
    }

__err__:
    if ( cmac_ctx ) {
        mbedtls_platform_zeroize( cmac_ctx->cmac, sizeof( *cmac_ctx->cmac ) );
        mbedtls_platform_zeroize( cmac_ctx, sizeof( *cmac_ctx ) );
        mbedtls_free( cmac_ctx );
        ctx->cmac_ctx = NULL;
    }
__out__:
    return _convert_retval_to_mbedtls( ret );
}

int dubhe_cmac_starts_with_derived_key( mbedtls_cipher_context_t *ctx,
                                        int key_type,
                                        const unsigned char *ek1,
                                        const unsigned char *ek2,
                                        const unsigned char *ek3,
                                        unsigned int ek1bits )
{
    int ret = 0;
    mbedtls_cmac_context_t *cmac_ctx = NULL;
    arm_ce_sca_alg_t malg = 0;

    if ( ctx == NULL || ctx->cipher_info == NULL ||
         ek1 == NULL || ek2 == NULL ||
         ek3 == NULL ) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    if ( (ctx->cipher_info == NULL) || (ek1bits != ctx->cipher_info->key_bitlen) ) {
        return ( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    if ( key_type != MBEDTLS_CMAC_MODEL_KEY && key_type != MBEDTLS_CMAC_DEVICE_ROOT_KEY ) {
        return ( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    switch ( ctx->cipher_info->type )
    {
        case MBEDTLS_CIPHER_AES_128_ECB:
        case MBEDTLS_CIPHER_AES_192_ECB:
        case MBEDTLS_CIPHER_AES_256_ECB:
            malg = ARM_CE_AES;
            break;
        case MBEDTLS_CIPHER_SM4_ECB:
            malg = ARM_CE_SM4;
            break;
        case MBEDTLS_CIPHER_DES_EDE3_ECB:
        default:
            return sw_mbedtls_cmac_starts_with_derived_key( ctx,
                                                            key_type,
                                                            ek1,
                                                            ek2,
                                                            ek3,
                                                            ek1bits );
    }

    if ( ctx->cmac_ctx == NULL ||
         ctx->cmac_ctx->magic != MBEDTLS_CMAC_MAGIC ||
         ctx->cmac_ctx->cmac == NULL ) {
        cmac_ctx = ctx->cmac_ctx;
        /* allocate cmac context */
        cmac_ctx = (mbedtls_cmac_context_t *)mbedtls_calloc( 1,
                            sizeof(*cmac_ctx) + sizeof(arm_ce_cmac_context_t) );
        if ( cmac_ctx == NULL ) {
            return( MBEDTLS_ERR_CIPHER_ALLOC_FAILED );
        }
        cmac_ctx->cmac = (arm_ce_cmac_context_t *)( cmac_ctx + 1 );
        /* init cmac context */
        ret = arm_ce_cmac_init( cmac_ctx->cmac, malg );
        if ( ret ) {
            mbedtls_printf( "init cmac contex failed\n");
            goto __err__;
        }
        cmac_ctx->magic = MBEDTLS_CMAC_MAGIC;
        ctx->cmac_ctx = cmac_ctx;
    }

    ret = arm_ce_cmac_set_derived_key( ctx->cmac_ctx->cmac,
                                       key_type,
                                       ek1,
                                       ek2,
                                       ek3,
                                       ek1bits );
    if ( ret ) {
        goto __err__;
    }

    ret = arm_ce_cmac_start( ctx->cmac_ctx->cmac );
    if ( ret ) {
        goto __err__;
    } else {
        goto __out__;
    }

__err__:
    if ( cmac_ctx ) {
        mbedtls_platform_zeroize( cmac_ctx->cmac, sizeof( *cmac_ctx->cmac ) );
        mbedtls_platform_zeroize( cmac_ctx, sizeof( *cmac_ctx ) );
        mbedtls_free( cmac_ctx );
        ctx->cmac_ctx = NULL;
    }
__out__:
    return _convert_retval_to_mbedtls( ret );
}

int dubhe_cmac_update( mbedtls_cipher_context_t *ctx,
                       const unsigned char *input, size_t ilen )
{
    mbedtls_cmac_context_t *cmac_ctx = NULL;

    if ( ctx == NULL || ctx->cipher_info == NULL || (ilen && input == NULL) ||
         ctx->cmac_ctx == NULL ) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    switch ( ctx->cipher_info->type )
    {
        case MBEDTLS_CIPHER_AES_128_ECB:
        case MBEDTLS_CIPHER_AES_192_ECB:
        case MBEDTLS_CIPHER_AES_256_ECB:
        case MBEDTLS_CIPHER_SM4_ECB:
            break;
        case MBEDTLS_CIPHER_DES_EDE3_ECB:
        default:
            return sw_mbedtls_cmac_update( ctx, input, ilen );
    }

    if (ctx->cmac_ctx->magic != MBEDTLS_CMAC_MAGIC) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    cmac_ctx = ctx->cmac_ctx;

    return _convert_retval_to_mbedtls(
            arm_ce_cmac_update( cmac_ctx->cmac, ilen, input ) );
}

int dubhe_cmac_finish( mbedtls_cipher_context_t *ctx,
                       unsigned char *output )
{
    mbedtls_cmac_context_t *cmac_ctx;

    if ( ctx == NULL || ctx->cipher_info == NULL || ctx->cmac_ctx == NULL ||
        output == NULL ) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    switch ( ctx->cipher_info->type )
    {
        case MBEDTLS_CIPHER_AES_128_ECB:
        case MBEDTLS_CIPHER_AES_192_ECB:
        case MBEDTLS_CIPHER_AES_256_ECB:
        case MBEDTLS_CIPHER_SM4_ECB:
            break;
        case MBEDTLS_CIPHER_DES_EDE3_ECB:
        default:
            return sw_mbedtls_cmac_finish( ctx, output );
    }

    if (ctx->cmac_ctx->magic != MBEDTLS_CMAC_MAGIC) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    cmac_ctx = ctx->cmac_ctx;

    return _convert_retval_to_mbedtls(
            arm_ce_cmac_finish( cmac_ctx->cmac, output ) );
}

void dubhe_cmac_free( mbedtls_cmac_context_t *ctx )
{
    if ( ( ctx == NULL ) || ( ctx->magic != MBEDTLS_CMAC_MAGIC ) ||
         ( ctx->cmac == NULL ) ) {
        return;
    }

    arm_ce_cmac_free( ctx->cmac );
    mbedtls_platform_zeroize( ctx->cmac, sizeof( *ctx->cmac ) );
    mbedtls_platform_zeroize( ctx, sizeof( *ctx ) );
}

int dubhe_cmac_reset( mbedtls_cipher_context_t *ctx )
{
    mbedtls_cmac_context_t *cmac_ctx = NULL;

    if( ctx == NULL || ctx->cipher_info == NULL || ctx->cmac_ctx == NULL ) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    switch ( ctx->cipher_info->type )
    {
        case MBEDTLS_CIPHER_AES_128_ECB:
        case MBEDTLS_CIPHER_AES_192_ECB:
        case MBEDTLS_CIPHER_AES_256_ECB:
        case MBEDTLS_CIPHER_SM4_ECB:
            break;
        case MBEDTLS_CIPHER_DES_EDE3_ECB:
        default:
            return sw_mbedtls_cmac_reset( ctx );
    }

    if (ctx->cmac_ctx->magic != MBEDTLS_CMAC_MAGIC) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    cmac_ctx = ctx->cmac_ctx;
    return _convert_retval_to_mbedtls(
                arm_ce_cmac_reset( cmac_ctx->cmac ) );
}

int dubhe_cipher_cmac( const mbedtls_cipher_info_t *cipher_info,
                       const unsigned char *key, size_t keylen,
                       const unsigned char *input, size_t ilen,
                       unsigned char *output )
{
    mbedtls_cipher_context_t ctx;
    int ret = 0;;

    if( cipher_info == NULL || key == NULL || (ilen && input == NULL) || output == NULL ) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    mbedtls_cipher_init( &ctx );

    if( ( ret = mbedtls_cipher_setup( &ctx, cipher_info ) ) != 0 ) {
        goto exit;
    }

    ret = dubhe_cmac_starts( &ctx, key, keylen );
    if( ret != 0 ) {
        goto exit;
    }

    ret = dubhe_cmac_update( &ctx, input, ilen );
    if( ret != 0 ) {
        goto exit;
    }

    ret = dubhe_cmac_finish( &ctx, output );

exit:
    mbedtls_cipher_free( &ctx );

    return( ret );
}

#if defined(MBEDTLS_AES_C)
/*
 * Implementation of AES-CMAC-PRF-128 defined in RFC 4615
 */
int dubhe_cmac_prf_128( const unsigned char *key, size_t key_length,
                        const unsigned char *input, size_t in_len,
                        unsigned char *output )
{
    int ret = 0;
    const mbedtls_cipher_info_t *cipher_info;
    unsigned char zero_key[MBEDTLS_AES_BLOCK_SIZE];
    unsigned char int_key[MBEDTLS_AES_BLOCK_SIZE];

    if( key == NULL || input == NULL || output == NULL ) {
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    cipher_info = mbedtls_cipher_info_from_type( MBEDTLS_CIPHER_AES_128_ECB );
    if( cipher_info == NULL ) {
        /* Failing at this point must be due to a build issue */
        ret = MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE;
        goto exit;
    }

    if( key_length == MBEDTLS_AES_BLOCK_SIZE ) {
        /* Use key as is */
        memcpy( int_key, key, MBEDTLS_AES_BLOCK_SIZE );
    } else {
        memset( zero_key, 0, MBEDTLS_AES_BLOCK_SIZE );

        ret = dubhe_cipher_cmac( cipher_info, zero_key, 128, key,
                                   key_length, int_key );
        if( ret != 0 ) {
            goto exit;
        }
    }

    ret = dubhe_cipher_cmac( cipher_info, int_key, 128, input, in_len,
                             output );

exit:
    mbedtls_platform_zeroize( int_key, sizeof( int_key ) );
    return( ret );
}
#endif /* MBEDTLS_AES_C */

#endif /* !MBEDTLS_CMAC_ALT */

#endif /* MBEDTLS_CMAC_C */
