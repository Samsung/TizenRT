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

#include "dubhe_cmac.h"
#include "pal_log.h"
#include "pal_string.h"
#include "pal_heap.h"

/** get cmac private context */
#define CMAC_GET_PRIV_CTX(ctx)     ((ctx)->cctx)

#define CE_MAX_SCA_KEY ( 32 )
#define CE_SEC_SCA_KEY ( 16 )

typedef struct arm_ce_sca_key {
    arm_ce_sca_key_type_t type;   /* key type */
    uint8_t key1[CE_MAX_SCA_KEY]; /* The encryption key1 */
    uint8_t key2[CE_SEC_SCA_KEY]; /* The encryption key2, only valid for derived key */
    uint8_t key3[CE_SEC_SCA_KEY]; /* The encryption key3, only valid for derived key */
    uint32_t key1bits;            /* The size of key1 bits. Valid options are: 128/192/256 */
    uint8_t derived;              /* If the key is derived by key ladder or not */
} arm_ce_sca_key_t;

typedef enum arm_ce_cmac_state {
    CMAC_STATE_RAW = 0,
    CMAC_STATE_INIT,
    CMAC_STATE_READY,
    CMAC_STATE_START,
    CMAC_STATE_UPDATE,
    CMAC_STATE_LAST
} arm_ce_cmac_state_t;

typedef struct dbh_cmac_ctx {
    uint8_t iv[BLOCK_SIZE];     /* initial vector */
    uint8_t npdata[BLOCK_SIZE]; /* not processed data */
    uint32_t npdlen;            /* data length in byte */
    size_t mlen;                /* processed message length in byte */
    arm_ce_sca_key_t key;       /* cipher key */
    arm_ce_cmac_state_t state;  /* context status */
    arm_ce_sca_context_t inter; /* interal sca ctx */
} dbh_cmac_ctx_t;

static void dubhe_clear( dbh_cmac_ctx_t *ctx )
{
    if ( ctx != NULL ) {
        ctx->npdlen = 0;
        ctx->mlen = 0;
        pal_memset( ctx->npdata, 0x00, ARM_CE_SCA_BLK_SIZE );
        pal_memset( ctx->iv, 0x00, ARM_CE_SCA_BLK_SIZE );
    }
}

static inline int dubhe_cmac_check_keybits( arm_ce_sca_type_t alg,
                                            size_t keybits )
{
    switch ( alg )
    {
    case ARM_CE_AES_CMAC:
        if ( ( ARM_CE_SCA_KEY_BITS_128 != keybits ) &&
             ( ARM_CE_SCA_KEY_BITS_192 != keybits ) &&
             ( ARM_CE_SCA_KEY_BITS_256 != keybits ) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    case ARM_CE_SM4_CMAC:
        if ( ( ARM_CE_SCA_KEY_BITS_128 != keybits ) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    default:
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    return 0;
}

int arm_ce_cmac_init( arm_ce_cmac_context_t *ctx, arm_ce_sca_alg_t malg )
{
    int ret = 0;
    dbh_cmac_ctx_t *prv = NULL;

    if ( ctx == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    if ( malg != ARM_CE_AES && malg != ARM_CE_SM4 ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    prv = ( dbh_cmac_ctx_t * )pal_malloc( sizeof( *prv ) );
    if ( prv == NULL ) {
        return ( DGH_CMAC_BAD_OOM_ERR );
    }
    pal_memset( prv, 0x00, sizeof( *prv ) );
    pal_memset( ctx, 0x00, sizeof( *ctx ) );

    ret = arm_ce_sca_init( &prv->inter );
    if ( ret ) {
        PAL_LOG_ERR( "arm_ce_sca_init failed(-%X)\n", -ret );
        pal_free( prv );
        return ret;
    }

    if ( malg == ARM_CE_AES ) {
        prv->inter.sca_type = ARM_CE_AES_CMAC;
    } else if ( malg == ARM_CE_SM4 ) {
        prv->inter.sca_type = ARM_CE_SM4_CMAC;
    }

    prv->inter.alg_type = malg;
    prv->state = CMAC_STATE_INIT;
    ctx->cctx = prv;

    return 0;
}

int arm_ce_cmac_start( arm_ce_cmac_context_t *ctx )
{
    int ret = 0;
    dbh_cmac_ctx_t *prv = NULL;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ctx == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    prv = CMAC_GET_PRIV_CTX( ctx );
    if ( prv == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    switch ( prv->state ) {
        default:
        case CMAC_STATE_RAW:
        case CMAC_STATE_INIT:
            return ( DBH_CMAC_BAD_STATE_ERR );
        case CMAC_STATE_UPDATE:
            prv->inter.ctx_last_block_flag = ARM_CE_SCA_CTX_LAST_BLOCK;
            ret = arm_ce_sca_crypt( &prv->inter,
                                    ARM_CE_SCA_OPERATION_ENCRYPT,
                                    prv->inter.sca_type,
                                    ARM_CE_CMAC,
                                    ARM_CE_SCA_BLK_SIZE,
                                    NULL,
                                    prv->iv,
                                    NULL,
                                    prv->npdata,
                                    NULL );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_crypt last block failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_LAST;
        case CMAC_STATE_LAST:
            ret = arm_ce_sca_finish( &prv->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n" );
                return ret;
            }
            prv->state = CMAC_STATE_READY;
            break;
        case CMAC_STATE_START:
        case CMAC_STATE_READY:
            break;
    }

    dubhe_clear( prv );

    prv->state = CMAC_STATE_START;

    return 0;
}

int arm_ce_cmac_free( arm_ce_cmac_context_t *ctx )
{
    int ret = 0;
    dbh_cmac_ctx_t *prv = NULL;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ctx == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    prv = CMAC_GET_PRIV_CTX( ctx );
    if ( prv == NULL ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    switch ( prv->state ) {
        default:
            return ( DBH_CMAC_BAD_STATE_ERR );
        case CMAC_STATE_RAW:
        case CMAC_STATE_INIT:
        case CMAC_STATE_START:
        case CMAC_STATE_READY:
            break;
        case CMAC_STATE_UPDATE:
            prv->inter.ctx_last_block_flag = ARM_CE_SCA_CTX_LAST_BLOCK;
            ret = arm_ce_sca_crypt( &prv->inter,
                                    ARM_CE_SCA_OPERATION_ENCRYPT,
                                    prv->inter.sca_type,
                                    ARM_CE_CMAC,
                                    ARM_CE_SCA_BLK_SIZE,
                                    NULL,
                                    prv->iv,
                                    NULL,
                                    prv->npdata,
                                    NULL );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_crypt last block failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_LAST;
        case CMAC_STATE_LAST:
            ret = arm_ce_sca_finish( &prv->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n" );
            }
            prv->state = CMAC_STATE_READY;
            break;
    }

    pal_memset( prv, 0x00, sizeof( *prv ) );
    pal_free( prv );
    pal_memset( ctx, 0x00, sizeof( *ctx ) );

    return ret;
}

int arm_ce_cmac_set_key( arm_ce_cmac_context_t *ctx,
                         const unsigned char *key,
                         size_t keybits )
{
    int ret = 0;
    dbh_cmac_ctx_t *prv = NULL;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ( ctx == NULL ) || ( key == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    prv = CMAC_GET_PRIV_CTX( ctx );
    if ( prv == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    ret = dubhe_cmac_check_keybits( prv->inter.sca_type, keybits );
    if ( ret ) {
        return ret;
    }

    switch ( prv->state ) {
        default:
        case CMAC_STATE_RAW:
            return ( DBH_SCA_PARAM_INVAL_ERR );
        case CMAC_STATE_UPDATE:
            prv->inter.ctx_last_block_flag = ARM_CE_SCA_CTX_LAST_BLOCK;
            ret = arm_ce_sca_crypt( &prv->inter,
                                    ARM_CE_SCA_OPERATION_ENCRYPT,
                                    prv->inter.sca_type,
                                    ARM_CE_CMAC,
                                    ARM_CE_SCA_BLK_SIZE,
                                    NULL,
                                    prv->iv,
                                    NULL,
                                    prv->npdata,
                                    NULL );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_crypt last block failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_LAST;
        case CMAC_STATE_LAST:
            ret = arm_ce_sca_finish( &prv->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_READY;
            dubhe_clear( prv );
            break;
        case CMAC_STATE_INIT:
        case CMAC_STATE_START:
        case CMAC_STATE_READY:
            break;
    }

    ret = arm_ce_sca_set_key( &prv->inter,
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

    prv->key.type =  ARM_CE_SCA_EXTERNAL_KEY;
    prv->key.derived = 0;
    prv->key.key1bits = keybits;
    pal_memcpy( prv->key.key1, key, keybits / 8);

    prv->state = CMAC_STATE_READY;

    return 0;
}

int arm_ce_cmac_set_derived_key( arm_ce_cmac_context_t *ctx,
                                 arm_ce_sca_key_type_t type,
                                 const uint8_t *key1,
                                 const uint8_t *key2,
                                 const uint8_t *key3,
                                 size_t key1bits )
{
    int ret = 0;
    dbh_cmac_ctx_t *prv = NULL;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ( ctx == NULL ) || ( key1 == NULL ) || ( key2 == NULL ) ||
         ( key3 == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    prv = CMAC_GET_PRIV_CTX( ctx );
    if ( prv == NULL ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    if ( key1bits == ARM_CE_SCA_KEY_BITS_192 ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    ret = dubhe_cmac_check_keybits( prv->inter.sca_type, key1bits );
    if ( ret ) {
        return ret;
    }

    switch ( prv->state ) {
        default:
        case CMAC_STATE_RAW:
            return ( DBH_SCA_PARAM_INVAL_ERR );
        case CMAC_STATE_UPDATE:
            prv->inter.ctx_last_block_flag = ARM_CE_SCA_CTX_LAST_BLOCK;
            ret = arm_ce_sca_crypt( &prv->inter,
                                    ARM_CE_SCA_OPERATION_ENCRYPT,
                                    prv->inter.sca_type,
                                    ARM_CE_CMAC,
                                    ARM_CE_SCA_BLK_SIZE,
                                    NULL,
                                    prv->iv,
                                    NULL,
                                    prv->npdata,
                                    NULL );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_crypt last block failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_LAST;
        case CMAC_STATE_LAST:
            ret = arm_ce_sca_finish( &prv->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_READY;
            dubhe_clear( prv );
            break;
        case CMAC_STATE_INIT:
        case CMAC_STATE_START:
        case CMAC_STATE_READY:
            break;
    }

    ret = arm_ce_sca_set_key( &prv->inter,
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

    prv->key.type =  type;
    prv->key.derived = 1;
    prv->key.key1bits = key1bits;
    pal_memcpy( prv->key.key1, key1, key1bits / 8);
    pal_memcpy( prv->key.key2, key2, CE_SEC_SCA_KEY );
    pal_memcpy( prv->key.key3, key3, CE_SEC_SCA_KEY );

    prv->state = CMAC_STATE_READY;

    return 0;
}

int arm_ce_cmac_update( arm_ce_cmac_context_t *ctx,
                        size_t length,
                        const unsigned char *input )
{
    int ret          = 0;
    size_t local_len = 0;
    size_t pad_len   = 0;
    size_t remainder = 0;
    dbh_cmac_ctx_t *prv    = NULL;
    const uint8_t *in_data = NULL;

    if ( ( ctx == NULL ) || ( ( input == NULL ) && ( length ) ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    prv = CMAC_GET_PRIV_CTX( ctx );
    if ( prv == NULL ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    if ( length == 0 ) {
        return 0;
    }

    local_len = length;
    in_data = input;

    switch ( prv->state ) {
        default:
        case CMAC_STATE_RAW:
        case CMAC_STATE_INIT:
        case CMAC_STATE_READY:
        case CMAC_STATE_LAST:
            return ( DBH_CMAC_BAD_STATE_ERR );
        case CMAC_STATE_START:
        case CMAC_STATE_UPDATE:
            break;
    }

    /* npdata cached */
    if ( prv->npdlen > 0 ) {
        pad_len = ( local_len > ( ARM_CE_SCA_BLK_SIZE - prv->npdlen ) ) \
                  ? ( ARM_CE_SCA_BLK_SIZE - prv->npdlen ) : local_len;
        /* fill up npdata */
        pal_memcpy( prv->npdata + prv->npdlen, in_data, pad_len );
        local_len -= pad_len;
        in_data += pad_len;
        prv->npdlen += pad_len;

        /* exit for save last blcok */
        if ( local_len == 0 ) {
            goto __out__;
        }

        /* process npdata if it's full */
        if ( prv->npdlen == ARM_CE_SCA_BLK_SIZE ) {
            ret = arm_ce_sca_crypt( &prv->inter,
                                    ARM_CE_SCA_OPERATION_ENCRYPT,
                                    prv->inter.sca_type,
                                    ARM_CE_CMAC,
                                    prv->npdlen,
                                    NULL,
                                    prv->iv,
                                    NULL,
                                    prv->npdata,
                                    NULL );
        if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_crypt failed(-%X)\n", -ret );
                prv->npdlen -= pad_len;
                pal_memset( prv->npdata + prv->npdlen, 0x00, pad_len);
                return ret;
            }
            prv->npdlen = 0;
        }
    }

    /* process input data if needed */
    remainder = local_len % ARM_CE_SCA_BLK_SIZE;
    if ( remainder == 0 ) {
        /* save last block */
        remainder = ARM_CE_SCA_BLK_SIZE;
    }
    local_len -= remainder;
    if ( local_len > 0 ) {
        ret = arm_ce_sca_crypt( &prv->inter,
                                ARM_CE_SCA_OPERATION_ENCRYPT,
                                prv->inter.sca_type,
                                ARM_CE_CMAC,
                                local_len,
                                NULL,
                                prv->iv,
                                NULL,
                                in_data,
                                NULL );
        if ( ret ) {
            PAL_LOG_ERR("arm_ce_sca_crypt failed(-%X)\n", -ret );
            return ret;
        }
    }

    /* handle last incomplete block */
    if ( 0 < remainder ) {
        prv->npdlen = remainder;
        pal_memcpy( prv->npdata, in_data + local_len, remainder );
    }

__out__:
    prv->mlen += length;
    prv->state = CMAC_STATE_UPDATE;
    return 0;
}

static void xor_block( uint8_t *a, uint8_t *b, uint8_t *out, size_t len )
{
    size_t i;
    for (i = 0; i < len; i++) {
        out[i] = a[i] ^ b[i];
    }
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
static int cmac_multiply_by_u( uint8_t *output,
                               const uint8_t *input,
                               arm_ce_sca_context_t *ctx )
{
    const uint8_t R_128 = 0x87;
    uint8_t R_n = 0;
    uint8_t mask = 0;
    uint8_t overflow = 0x00;
    int i = 0;

    if ( ( ctx == NULL ) || ( input == NULL ) || (  output == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    if ( ( ctx->alg_type == ARM_CE_AES )
         || ( ctx->alg_type == ARM_CE_SM4 ) ) {
        R_n = R_128;
    } else {
        PAL_LOG_ERR( "invalid algorithm type\n" );
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    for ( i = (int) ARM_CE_SCA_BLK_SIZE - 1; i >= 0; i-- ) {
        output[i] = input[i] << 1 | overflow;
        overflow = input[i] >> 7;
    }

    mask = - ( input[0] >> 7 );
    output[ ARM_CE_SCA_BLK_SIZE - 1 ] ^= R_n & mask;

    return 0;
}

static int cmac_generate_subkey( arm_ce_sca_context_t *ctx,
                                 uint8_t *K1,
                                 uint8_t *K2 )
{
    int ret = 0;
    uint8_t L[ARM_CE_SCA_BLK_SIZE] = {0};
    uint8_t Z[ARM_CE_SCA_BLK_SIZE] = {0};
    arm_ce_sca_type_t algo;

    if (ctx == NULL) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    switch (ctx->alg_type)
    {
    case ARM_CE_AES:
        algo = ARM_CE_AES_ECB;
        break;
    case ARM_CE_SM4:
        algo = ARM_CE_SM4_ECB;
        break;
    default:
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    ret = arm_ce_sca_crypt( ctx,
                            ARM_CE_SCA_OPERATION_ENCRYPT,
                            algo,
                            0,
                            ARM_CE_SCA_BLK_SIZE,
                            NULL,
                            NULL,
                            NULL,
                            Z,
                            L );
    if ( ret ) {
        PAL_LOG_ERR( "cmac_generate_subkey failed\n" );
        return ret;
    }
    /*
     * Generate K1 and K2
     */
    if( ( ret = cmac_multiply_by_u( K1, L , ctx ) ) != 0 ) {
        return ret;
    }

    if( ( ret = cmac_multiply_by_u( K2, K1 , ctx ) ) != 0 ) {
        return ret;
    }

    return 0;
}

static int cmac_zero_length_message( dbh_cmac_ctx_t *ctx,
                                     uint8_t *mac )
{
    int ret = 0;
    uint8_t K1[ARM_CE_SCA_BLK_SIZE] = {0};
    uint8_t K2[ARM_CE_SCA_BLK_SIZE] = {0};
    uint8_t m_last[ARM_CE_SCA_BLK_SIZE] = {0};
    arm_ce_sca_context_t sca_ctx = {0};
    arm_ce_sca_type_t algo;

    if ( ( ctx == NULL ) || ( mac == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    ret = arm_ce_sca_init( &sca_ctx );
    if ( ret ) {
        return ret;
    }
    sca_ctx.alg_type = ctx->inter.alg_type;
    switch (sca_ctx.alg_type)
    {
    case ARM_CE_AES:
        algo = ARM_CE_AES_ECB;
        break;
    case ARM_CE_SM4:
        algo = ARM_CE_SM4_ECB;
        break;
    default:
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    ret = arm_ce_sca_set_key (
        &sca_ctx, ctx->key.type, ctx->key.key1,
        ctx->key.key2, ctx->key.key3,
        ctx->key.key1bits, ctx->key.derived );
    if ( ret ) {
        goto __out__;
    }

    ret = cmac_generate_subkey( &sca_ctx, K1, K2 );
    if ( ret ) {
        goto __out__;
    }

    m_last[0] = 0x80;
    xor_block( m_last, K2, m_last, ARM_CE_SCA_BLK_SIZE );
    ret = arm_ce_sca_crypt( &sca_ctx,
                            ARM_CE_SCA_OPERATION_ENCRYPT,
                            algo,
                            0,
                            ARM_CE_SCA_BLK_SIZE,
                            NULL,
                            NULL,
                            NULL,
                            m_last,
                            m_last );
    if ( ret ) {
        goto __out__;
    }
    pal_memcpy( mac, m_last, ARM_CE_SCA_BLK_SIZE );

__out__:
    arm_ce_sca_free( &sca_ctx, NULL );
    return ret;
}

int arm_ce_cmac_finish( arm_ce_cmac_context_t *ctx, unsigned char *mac )
{
    int ret = 0;
    dbh_cmac_ctx_t *prv = NULL;

    if ( ( ctx == NULL ) || ( mac == NULL ) ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    prv = CMAC_GET_PRIV_CTX( ctx );
    if ( prv == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    switch ( prv->state ) {
        default:
        case CMAC_STATE_RAW:
        case CMAC_STATE_INIT:
            return ( DBH_CMAC_BAD_STATE_ERR );
        case CMAC_STATE_START:
        case CMAC_STATE_READY: /** continious finish support and treat following finish as cmac(inle(0)) */
        case CMAC_STATE_UPDATE:
            break;
        case CMAC_STATE_LAST:
            goto __finish__;
    }

    if ( prv->mlen == 0 ) {
        ret = cmac_zero_length_message( prv, mac );
        if ( ret ) {
            PAL_LOG_ERR( "cmac_zero_length_message failed\n");
            return ret;
        }
        goto __out__;
    }

    if ( prv->npdlen > 0 ) {
        prv->inter.ctx_last_block_flag = ARM_CE_SCA_CTX_LAST_BLOCK;
        ret = arm_ce_sca_crypt( &prv->inter,
                                ARM_CE_SCA_OPERATION_ENCRYPT,
                                prv->inter.sca_type,
                                ARM_CE_CMAC,
                                prv->npdlen,
                                NULL,
                                prv->iv,
                                NULL,
                                prv->npdata,
                                NULL );
        if ( ret ) {
            PAL_LOG_ERR( "arm_ce_sca_crypt last block failed\n");
            return ret;
        }
        prv->state = CMAC_STATE_LAST;
    } else {
        PAL_LOG_ERR( "arm_ce_sca_crypt last block failed, npdlen <= 0\n");
        while(true);
    }

__finish__:
    ret = arm_ce_sca_finish( &prv->inter, mac );
    if ( ret ) {
        PAL_LOG_ERR("arm_ce_cmac_finish failed(-%X)\n", -ret );
        return ret;
    }

__out__:
    dubhe_clear( prv );
    prv->state = CMAC_STATE_READY;
    return ret;
}

int arm_ce_cmac_reset( arm_ce_cmac_context_t *ctx )
{
    int ret = 0;
    dbh_cmac_ctx_t *prv = NULL;
    uint8_t tmp_mac[ARM_CE_SCA_BLK_SIZE] = {0};

    if ( ctx == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }
    prv = CMAC_GET_PRIV_CTX( ctx );
    if ( prv == NULL ) {
        return ( DBH_SCA_PARAM_INVAL_ERR );
    }

    switch ( prv->state ) {
        default:
        case CMAC_STATE_RAW:
        case CMAC_STATE_INIT:
            return ( DBH_CMAC_BAD_STATE_ERR );
        case CMAC_STATE_START:
        case CMAC_STATE_READY:
            break;
        case CMAC_STATE_UPDATE:
            prv->inter.ctx_last_block_flag = ARM_CE_SCA_CTX_LAST_BLOCK;
            ret = arm_ce_sca_crypt( &prv->inter,
                                    ARM_CE_SCA_OPERATION_ENCRYPT,
                                    prv->inter.sca_type,
                                    ARM_CE_CMAC,
                                    ARM_CE_SCA_BLK_SIZE,
                                    NULL,
                                    prv->iv,
                                    NULL,
                                    prv->npdata,
                                    NULL );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_crypt last block failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_LAST;
        case CMAC_STATE_LAST:
            ret = arm_ce_sca_finish( &prv->inter, tmp_mac );
            if ( ret ) {
                PAL_LOG_ERR( "arm_ce_sca_finish failed\n");
                return ret;
            }
            prv->state = CMAC_STATE_READY;
            break;
    }

    dubhe_clear( prv );

    prv->state = CMAC_STATE_START;

    return 0;
}