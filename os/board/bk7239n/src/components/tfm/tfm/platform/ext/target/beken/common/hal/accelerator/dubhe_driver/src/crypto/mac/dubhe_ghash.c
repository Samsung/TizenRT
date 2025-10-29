
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
#include "pal_string.h"
#include "pal_heap.h"
#include "dubhe_ghash.h"
#include "dubhe_sca.h"

/** get ghash private context */
#define GHASH_GET_PRIV_CTX(ctx)     ((ctx)->gctx)
/**
 *  states of ghash
 */
typedef enum {
    CE_GHASH_STATE_RAW,
    CE_GHASH_STATE_INIT,
    CE_GHASH_STATE_READY,
    CE_GHASH_STATE_START,
    CE_GHASH_STATE_UPDATE,
} ce_ghash_state_t;

/**
 *  ghash context
 */
typedef struct dbh_ghash_ctx {
    uint8_t iv[ARM_CE_SCA_BLK_SIZE];           /* initial vector */
    uint8_t npdata[ARM_CE_SCA_BLK_SIZE];       /* not processed data */
    size_t npdlen;                             /* data length in byte */
    ce_ghash_state_t state;                    /* context state */
    arm_ce_sca_context_t inter;                /* internal sca ctx */
} dbh_ghash_ctx_t;

int arm_ce_ghash_init( arm_ce_ghash_ctx_t *ctx)
{
    int ret = 0;
    dbh_ghash_ctx_t *prv = NULL;

    if ( NULL == ctx ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = (dbh_ghash_ctx_t *)pal_malloc( sizeof(*prv) );
    if (NULL == prv) {
        return DGH_GHASH_ERR_OOM;
    }
    pal_memset( prv, 0x00, sizeof(*prv) );
    pal_memset( ctx, 0x00, sizeof(*ctx) );
    ret = arm_ce_sca_init(&prv->inter);
    if (0 != ret) {
        pal_free( prv );
        return ret;
    }
    prv->inter.alg_type = ARM_CE_GHASH;
    prv->inter.sca_type = ARM_CE_GHASH_MAC;
    prv->inter.mac_type = ARM_CE_GMAC;
    prv->state = CE_GHASH_STATE_INIT;
    ctx->gctx = prv;
    return 0;
}

int arm_ce_ghash_free( arm_ce_ghash_ctx_t *ctx )
{
    uint8_t trash[ARM_CE_SCA_BLK_SIZE] = {0};
    int ret = 0;
    dbh_ghash_ctx_t *prv = NULL;

    if ( NULL == ctx ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GHASH_GET_PRIV_CTX( ctx );
    if (NULL == prv) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_GHASH_STATE_RAW:
        break;
    case CE_GHASH_STATE_INIT:
    case CE_GHASH_STATE_READY:
    case CE_GHASH_STATE_START:
    case CE_GHASH_STATE_UPDATE:
        if ( prv->inter.ctx_finish_flag == ARM_CE_SCA_CTX_NEED_FINISH ) {
            ret = arm_ce_sca_free( &prv->inter, trash );
            if ( 0 != ret ) {
                return ret;
            }
        }
        pal_memset( prv, 0x00, sizeof(*prv) );
        pal_free( prv );
        break;
    default:
        return DGH_GHASH_BAD_STATE;
    }
    pal_memset( ctx, 0x00, sizeof(*ctx) );
    return 0;
}

int arm_ce_ghash_set_key( arm_ce_ghash_ctx_t *ctx,
                          const uint8_t key[16] )
{
    int ret = 0;
    dbh_ghash_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == key) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GHASH_GET_PRIV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch ( prv->state )
    {
    case CE_GHASH_STATE_INIT:
    case CE_GHASH_STATE_READY:
        break;
    default:
    case CE_GHASH_STATE_RAW:
    case CE_GHASH_STATE_START:
    case CE_GHASH_STATE_UPDATE:
        return DGH_GHASH_BAD_STATE;
    }

    ret = arm_ce_sca_set_key( &prv->inter,
                              ARM_CE_SCA_EXTERNAL_KEY,
                              key, NULL, NULL,
                              128, 0 );
    if ( 0 != ret ) {
        return ret;
    }
    prv->state = CE_GHASH_STATE_READY;
    return 0;
}

int arm_ce_ghash_start( arm_ce_ghash_ctx_t *ctx,
                        const uint8_t iv[16] )
{
    dbh_ghash_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == iv) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GHASH_GET_PRIV_CTX( ctx );
    if (NULL == prv) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_GHASH_STATE_READY:
    case CE_GHASH_STATE_START:
        break;
    default:
    case CE_GHASH_STATE_RAW:
    case CE_GHASH_STATE_INIT:
    case CE_GHASH_STATE_UPDATE:
        return DGH_GHASH_BAD_STATE;
    }

    pal_memcpy( prv->iv, iv, ARM_CE_SCA_BLK_SIZE );
    pal_memset(prv->npdata, 0x00, ARM_CE_SCA_BLK_SIZE);
    prv->npdlen = 0;
    prv->state = CE_GHASH_STATE_START;
    return 0;
}

int arm_ce_ghash_update( arm_ce_ghash_ctx_t *ctx,
                         size_t length,
                         const uint8_t *input )
{
    int ret = 0;
    size_t padding_sz = 0;
    size_t in_left = length;
    dbh_ghash_ctx_t *prv = NULL;

    if ( (NULL == ctx) || ((NULL == input) && (length > 0)) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GHASH_GET_PRIV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch ( prv->state )
    {
    case CE_GHASH_STATE_START:
    case CE_GHASH_STATE_UPDATE:
        break;
    default:
    case CE_GHASH_STATE_RAW:
    case CE_GHASH_STATE_INIT:
    case CE_GHASH_STATE_READY:
        return DGH_GHASH_BAD_STATE;
    }

    if ( 0 == length ) {
        prv->state = CE_GHASH_STATE_UPDATE;
        return 0;
    }
    /** pad not processed data to complete block first */
    if ( (prv->npdlen > 0) && (prv->npdlen < ARM_CE_SCA_BLK_SIZE) ) {
        if ( (ARM_CE_SCA_BLK_SIZE - prv->npdlen) > length ) {
            padding_sz = length;
            pal_memcpy( prv->npdata + prv->npdlen, input, padding_sz );
            prv->npdlen += padding_sz;
            prv->state = CE_GHASH_STATE_UPDATE;
            return 0;
        } else {
            padding_sz = ARM_CE_SCA_BLK_SIZE - prv->npdlen;
            pal_memcpy( prv->npdata + prv->npdlen, input, padding_sz );
            prv->npdlen += padding_sz;
            in_left -= padding_sz;
            ret = arm_ce_sca_crypt( &prv->inter, ARM_CE_SCA_OPERATION_ENCRYPT,
                                    ARM_CE_GHASH_MAC, ARM_CE_GMAC,
                                    prv->npdlen, NULL, prv->iv, NULL,
                                    prv->npdata, NULL );
            if ( 0 != ret ) {
                goto err_pro_npdata;
            }
            /** clear proccessed npdata */
            pal_memset( prv->npdata, 0x00, prv->npdlen );
            prv->npdlen = 0;
        }
    }
    /** no data left just return OK */
    if ( 0 == in_left ) {
        prv->state = CE_GHASH_STATE_UPDATE;
        return 0;
    }

    /** save new npdata */
    prv->npdlen = in_left % ARM_CE_SCA_BLK_SIZE;
    in_left -= prv->npdlen;
    /** has complete blocks left process them */
    if ( in_left > 0 ) {
        ret = arm_ce_sca_crypt( &prv->inter, ARM_CE_SCA_OPERATION_ENCRYPT,
                                ARM_CE_GHASH_MAC, ARM_CE_GMAC,
                                in_left, NULL, prv->iv, NULL,
                                input + padding_sz, NULL );
        if ( 0 != ret ) {
            goto err_pro_blks;
        }
    }
    /** has not processed data, store them into ctx->npdata */
    if ( prv->npdlen > 0 ) {
        pal_memcpy( prv->npdata, input + padding_sz + in_left, prv->npdlen );
    }

    prv->state = CE_GHASH_STATE_UPDATE;
    return 0;
err_pro_blks:
    /** rollback npdlen to last process */
    prv->npdlen = 0;
err_pro_npdata:
    /** rollback npdlen to last process */
    prv->npdlen -= padding_sz;
    return ret;
}

int arm_ce_ghash_finish( arm_ce_ghash_ctx_t *ctx,
                         uint8_t *mac )
{
    int ret = 0;
    dbh_ghash_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == mac) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GHASH_GET_PRIV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch ( prv->state )
    {
    case CE_GHASH_STATE_UPDATE:
        break;
    default:
    case CE_GHASH_STATE_RAW:
    case CE_GHASH_STATE_INIT:
    case CE_GHASH_STATE_READY:
    case CE_GHASH_STATE_START:
        return DGH_GHASH_BAD_STATE;
    }
    /** ghash not support uncomplete blocks, so if has npdata then report an error */
    if ( prv->npdlen != 0 ) {
        return DGH_GHASH_ERR_BAD_INPUT_LENGTH;
    }

    ret = arm_ce_sca_finish( &prv->inter, mac );
    if ( 0 != ret ) {
        return ret;
    }

    prv->state = CE_GHASH_STATE_READY;
    return 0;
}
