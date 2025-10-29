/**
 * @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
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

#include "dubhe_cbcmac.h"
#include "pal_log.h"
#include "pal_string.h"

static void dubhe_clear( arm_ce_cbcmac_context_t *ctx )
{
    if ( ctx != NULL ) {
        ctx->npdlen = 0;
        pal_memset( ctx->npdata, 0x00, ARM_CE_SCA_BLK_SIZE );
        pal_memset( ctx->iv, 0x00, ARM_CE_SCA_BLK_SIZE );
    }
}

static inline int dubhe_cbcmac_check_keybits( arm_ce_sca_type_t alg,
                                              size_t keybits )
{
    switch ( alg )
    {
    case ARM_CE_AES_CBCMAC:
        if ( ( ARM_CE_SCA_KEY_BITS_128 != keybits ) &&
             ( ARM_CE_SCA_KEY_BITS_192 != keybits ) &&
             ( ARM_CE_SCA_KEY_BITS_256 != keybits ) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    case ARM_CE_SM4_CBCMAC:
        if ( ( ARM_CE_SCA_KEY_BITS_128 != keybits ) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    default:
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    return 0;
}

int arm_ce_cbcmac_init( arm_ce_cbcmac_context_t *ctx, arm_ce_sca_alg_t malg )
{
    int ret = 0;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ctx == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    if ( malg != ARM_CE_AES && malg != ARM_CE_SM4 ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    if ( ctx->state == CBCMAC_STATE_UPDATE ) {
        ret = arm_ce_sca_finish( &ctx->inter, tmp_mac );
        if ( ret ) {
            PAL_LOG_ERR( "arm_ce_sca_finish failed\n" );
            return ret;
        }
        ctx->state = CBCMAC_STATE_READY;
    }

    dubhe_clear( ctx );
    ret = arm_ce_sca_init( &ctx->inter );
    if ( ret ) {
        PAL_LOG_ERR("arm_ce_sca_init failed(-%X)\n", -ret );
        return ret;
    }

    if ( malg == ARM_CE_AES ) {
        ctx->inter.sca_type = ARM_CE_AES_CBCMAC;
    } else if ( malg == ARM_CE_SM4 ) {
        ctx->inter.sca_type = ARM_CE_SM4_CBCMAC;
    }

    ctx->inter.alg_type = malg;
    ctx->state = CBCMAC_STATE_INIT;

    return 0;
}

int arm_ce_cbcmac_start( arm_ce_cbcmac_context_t *ctx, unsigned char iv[16] )
{
    int ret = 0;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ( ctx == NULL ) || ( iv == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    switch ( ctx->state ) {
        default:
        case CBCMAC_STATE_RAW:
        case CBCMAC_STATE_INIT:
            return ( DBH_CBCMAC_BAD_STATE_ERR );
        case CBCMAC_STATE_UPDATE:
            ret = arm_ce_sca_finish( &ctx->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n");
                return ret;
            }
            ctx->state = CBCMAC_STATE_READY;
            break;
        case CBCMAC_STATE_START:
        case CBCMAC_STATE_READY:
            break;
    }

    dubhe_clear( ctx );
    pal_memcpy( ctx->iv, iv, ARM_CE_SCA_BLK_SIZE );

    ctx->state = CBCMAC_STATE_START;

    return 0;
}

int arm_ce_cbcmac_free( arm_ce_cbcmac_context_t *ctx )
{
    int ret = 0;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ctx == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    switch ( ctx->state ) {
        default:
            return ( DBH_CBCMAC_BAD_STATE_ERR );
        case CBCMAC_STATE_RAW:
        case CBCMAC_STATE_INIT:
        case CBCMAC_STATE_START:
        case CBCMAC_STATE_READY:
            break;
        case CBCMAC_STATE_UPDATE:
            ret = arm_ce_sca_finish( &ctx->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n" );
            }
            ctx->state = CBCMAC_STATE_READY;
            break;
    }

    pal_memset( ctx, 0x00, sizeof( arm_ce_cbcmac_context_t ) );

    return ret;
}

int arm_ce_cbcmac_set_key( arm_ce_cbcmac_context_t *ctx,
                           const unsigned char *key,
                           size_t keybits )
{
    int ret = 0;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ( ctx == NULL ) || ( key == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    ret = dubhe_cbcmac_check_keybits( ctx->inter.sca_type, keybits );
    if ( ret ) {
        return ret;
    }

    switch ( ctx->state ) {
        default:
        case CBCMAC_STATE_RAW:
            return ( DBH_CBCMAC_BAD_STATE_ERR );
        case CBCMAC_STATE_UPDATE:
            ret = arm_ce_sca_finish( &ctx->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n");
                return ret;
            }
            ctx->state = CBCMAC_STATE_READY;
            dubhe_clear( ctx );
            break;
        case CBCMAC_STATE_INIT:
        case CBCMAC_STATE_START:
        case CBCMAC_STATE_READY:
            break;
    }

    ret = arm_ce_sca_set_key( &ctx->inter,
                              ARM_CE_SCA_EXTERNAL_KEY,
                              key,
                              NULL,
                              NULL,
                              keybits,
                              0 );
    if ( ret ) {
        PAL_LOG_ERR("arm_ce_cbcmac_setkey failed(-%X)\n", -ret );
        return ret;
    }

    ctx->state = CBCMAC_STATE_READY;

    return 0;
}

int arm_ce_cbcmac_set_derived_key( arm_ce_cbcmac_context_t *ctx,
                                   arm_ce_sca_key_type_t type,
                                   const uint8_t *key1,
                                   const uint8_t *key2,
                                   const uint8_t *key3,
                                   size_t key1bits )
{
    int ret = 0;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ( ctx == NULL ) || ( key1 == NULL ) || ( key2 == NULL ) ||
         ( key3 == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    if ( key1bits == ARM_CE_SCA_KEY_BITS_192 ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    ret = dubhe_cbcmac_check_keybits( ctx->inter.sca_type, key1bits );
    if ( ret ) {
        return ret;
    }

    switch ( ctx->state ) {
        default:
        case CBCMAC_STATE_RAW:
            return ( DBH_CBCMAC_BAD_STATE_ERR );
        case CBCMAC_STATE_UPDATE:
            ret = arm_ce_sca_finish( &ctx->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n");
                return ret;
            }
            ctx->state = CBCMAC_STATE_READY;
            dubhe_clear( ctx );
            break;
        case CBCMAC_STATE_INIT:
        case CBCMAC_STATE_START:
        case CBCMAC_STATE_READY:
            break;
    }

    ret = arm_ce_sca_set_key( &ctx->inter,
                              type,
                              key1,
                              key2,
                              key3,
                              key1bits,
                              1 );
    if ( ret ) {
        PAL_LOG_ERR("arm_ce_cbcmac_setkey failed(-%X)\n", -ret );
        return ret;
    }

    ctx->state = CBCMAC_STATE_READY;

    return 0;
}

int arm_ce_cbcmac_update( arm_ce_cbcmac_context_t *ctx,
                          size_t length,
                          const unsigned char *input )
{
    int ret          = 0;
    size_t pad_len   = 0;
    size_t local_len = 0;
    size_t remainder = 0;

    if ( ( ctx == NULL ) || ( ( input == NULL ) && ( length ) ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    if ( length == 0 ) {
        return 0;
    }

    local_len = length;

    switch ( ctx->state ) {
        default:
        case CBCMAC_STATE_RAW:
        case CBCMAC_STATE_INIT:
        case CBCMAC_STATE_READY:
            return ( DBH_CBCMAC_BAD_STATE_ERR );
        case CBCMAC_STATE_START:
        case CBCMAC_STATE_UPDATE:
            break;
    }

    /* npdata cached */
    if ( ctx->npdlen > 0 ) {
        pad_len = ( local_len > ( ARM_CE_SCA_BLK_SIZE - ctx->npdlen ) ) \
                  ? ( ARM_CE_SCA_BLK_SIZE - ctx->npdlen ) : local_len;
        /* fill up npdata */
        pal_memcpy( ctx->npdata + ctx->npdlen, input, pad_len );
        local_len -= pad_len;
        input += pad_len;
        ctx->npdlen += pad_len;

        /* process npdata if it's full */
        if ( ctx->npdlen == ARM_CE_SCA_BLK_SIZE ) {
            ret = arm_ce_sca_crypt( &ctx->inter,
                                    ARM_CE_SCA_OPERATION_ENCRYPT,
                                    ctx->inter.sca_type,
                                    ARM_CE_CBC_MAC,
                                    ctx->npdlen,
                                    NULL,
                                    ctx->iv,
                                    NULL,
                                    ctx->npdata,
                                    NULL );
            if ( ret ) {
                    PAL_LOG_ERR( "arm_ce_sca_crypt failed(-%X)\n", -ret );
                    ctx->npdlen -= pad_len;
                    pal_memset( ctx->npdata + ctx->npdlen, 0x00, pad_len);
                    return ret;
            }
            ctx->npdlen = 0;
        }
    }

    /* process input if needed */
    if ( local_len == 0 ) {
        ctx->state = CBCMAC_STATE_UPDATE;
        return 0;
    } else {
        remainder = local_len % ARM_CE_SCA_BLK_SIZE;
        local_len -= remainder;

        ret = arm_ce_sca_crypt( &ctx->inter,
                                ARM_CE_SCA_OPERATION_ENCRYPT,
                                ctx->inter.sca_type,
                                ARM_CE_CBC_MAC,
                                local_len,
                                NULL,
                                ctx->iv,
                                NULL,
                                input,
                                NULL );
       if ( ret ) {
            PAL_LOG_ERR("arm_ce_sca_crypt failed(-%X)\n", -ret );
            return ret;
        }
    }

    /* handle last incomplete block */
    if ( 0 < remainder ) {
        ctx->npdlen = remainder;
        pal_memcpy( ctx->npdata, input + local_len, remainder );
    }

    ctx->state = CBCMAC_STATE_UPDATE;

    return 0;
}

int arm_ce_cbcmac_finish( arm_ce_cbcmac_context_t *ctx, unsigned char *mac )
{
    int ret = 0;

    if ( ( ctx == NULL ) || ( mac == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    switch ( ctx->state ) {
        default:
        case CBCMAC_STATE_RAW:
        case CBCMAC_STATE_INIT:
        case CBCMAC_STATE_START:
        case CBCMAC_STATE_READY:
            return ( DBH_CBCMAC_BAD_STATE_ERR );
        case CBCMAC_STATE_UPDATE:
            break;
    }

    if ( ctx->npdlen > 0 ) {
        PAL_LOG_ERR("block not aligned!\n");
        return ( DBH_SCA_DATA_ALIGN_ERR );
    }

    ret = arm_ce_sca_finish( &ctx->inter, mac );
    if ( ret ) {
        PAL_LOG_ERR("arm_ce_cbcmac_finish failed(-%X)\n", -ret );
        return ret;
    }

    dubhe_clear( ctx );

    ctx->state = CBCMAC_STATE_READY;
    return 0;
}