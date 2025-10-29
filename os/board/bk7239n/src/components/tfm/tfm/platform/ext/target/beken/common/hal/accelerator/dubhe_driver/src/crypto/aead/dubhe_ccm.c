
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
#include "dubhe_ccm.h"
#include "dubhe_cbcmac.h"

#define COUNTER_INCREASE(iv, bs, w, delta)                          \
            do {                                                    \
                size_t ii = 0, jj = 0;                              \
                for ( ii = 0; ii < (delta); ii++ ) {                \
                    for ( jj = (bs); jj > ((bs) - (w)); jj--) {     \
                        if ( 0 != ++(iv)[jj - 1]) {                 \
                            break;                                  \
                        }                                           \
                    }                                               \
                }                                                   \
            } while( 0 )

/**< get CCM context CTR alg */
#define CE_CCM_GET_CTR_ALG(ctx) (                                   \
            ( ARM_CE_AES_CCM == (ctx)->alg ) ? ARM_CE_AES_CTR :     \
                                               ARM_CE_SM4_CTR )

/**< get CCM context CBCMAC alg */
#define CE_CCM_GET_CBCMAC_MAIN_ALG(ctx) (                           \
            ( ARM_CE_AES_CCM == (ctx)->alg ) ? ARM_CE_AES :         \
                                               ARM_CE_SM4 )

/*< get ccm private context */
#define GET_CCM_PRV_CTX(ctx) (ctx->ccm)

/**
 *  states of ccm
 */
typedef enum {
    CE_CCM_STATE_RAW = 0, /*!< no allocate RAW state maybe no need */
    CE_CCM_STATE_INIT,
    CE_CCM_STATE_READY,
    CE_CCM_STATE_START,
    CE_CCM_STATE_UPDATE_AAD,
    CE_CCM_STATE_UPDATE_PAYLOAD,
} ce_ccm_state_t;

/**
 *  CCM context
 */
typedef struct dbh_ccm_ctx {
    arm_ce_sca_type_t alg;              /*!< algorithm id */
    arm_ce_sca_context_t cctx;          /*!< ctr context */
    arm_ce_cbcmac_context_t cmctx;      /*!< cbc-mac context */
    arm_ce_sca_operation_t op;          /*!< operation mode */
    ce_ccm_state_t state;               /*!< ccm state */
    uint64_t claim_aadlen;              /*!< claimed aad length */
    uint64_t aadlen;                    /*!< aad length in fact */
    uint64_t claim_mlen;                /*!< claimed message length */
    uint64_t mlen;                      /*!< message length in fact */
    uint8_t a0[ARM_CE_SCA_BLK_SIZE];    /*!< initial counter a0 */
    uint8_t ctr[ARM_CE_SCA_BLK_SIZE];   /*!< nonce counter */
    size_t nlen;                        /*!< nonce length 7 =< nlen >= 13 */
    uint8_t stream[ARM_CE_SCA_BLK_SIZE];/*!< stream block */
    size_t strpos;                      /*!< stream block offset */
    uint8_t claim_taglen;               /*!< claimed tag length */
} dbh_ccm_ctx_t;

int arm_ce_ccm_init( arm_ce_ccm_ctx_t *ctx, int32_t alg )
{
    int ret = 0;
    dbh_ccm_ctx_t *prv = NULL;

    if ( (NULL == ctx) ||
         ((ARM_CE_AES_CCM != alg) && (ARM_CE_SM4_CCM != alg)) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    pal_memset( ctx, 0x00, sizeof(*ctx) );
    prv = (dbh_ccm_ctx_t *)pal_malloc(sizeof(*prv));
    if (NULL == prv) {
        return DBH_CCM_ERR_OOM;
    }
    pal_memset(prv, 0x00, sizeof(*prv));
    prv->alg = alg;
    ret = arm_ce_cbcmac_init( &prv->cmctx, CE_CCM_GET_CBCMAC_MAIN_ALG(prv) );
    if (0 != ret) {
        goto err_cbcmac_init;
    }
    ret = arm_ce_sca_init( &prv->cctx );
    if (0 != ret) {
        goto err_sca_init;
    }
    prv->alg = alg;
    prv->state = CE_CCM_STATE_INIT;
    ctx->ccm = prv;
    return 0;
err_sca_init:
    arm_ce_cbcmac_free(&prv->cmctx);
err_cbcmac_init:
    pal_memset(prv, 0x00, sizeof(*prv));
    pal_free( prv );
    return ret;
}

int arm_ce_ccm_free( arm_ce_ccm_ctx_t *ctx )
{
    int ret = 0;
    dbh_ccm_ctx_t *prv = NULL;

    if ( NULL == ctx ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_CCM_STATE_RAW:
        return 0;
    case CE_CCM_STATE_INIT:
    case CE_CCM_STATE_READY:
    case CE_CCM_STATE_START:
    case CE_CCM_STATE_UPDATE_AAD:
    case CE_CCM_STATE_UPDATE_PAYLOAD:
        ret = arm_ce_cbcmac_free( &prv->cmctx );
        if (0 != ret) {
            goto err_free_cmctx;
        }
        ret = arm_ce_sca_free( &prv->cctx, NULL );
        if (0 != ret) {
            goto err_free_cctx;
        }
        pal_memset( prv, 0x00, sizeof(*prv) );
        pal_free ( prv );
        pal_memset( ctx, 0x00, sizeof(*ctx) );
        return 0;
    default:
        return DBH_CCM_ERR_BAD_STATE;
    }

err_free_cctx:
err_free_cmctx:
    return ret;
}

/**
 *  check keybits requirement
 *  check if key valid for AES-CCM (128/192/256) SM4-CCM(128)
 */
static inline int ce_ccm_check_keybits( arm_ce_sca_type_t alg,
                                        size_t keybits,
                                        bool isSecKey )
{
    switch (alg)
    {
    case ARM_CE_AES_CCM:
        if ( (ARM_CE_SCA_KEY_BITS_128 != keybits) &&
             (ARM_CE_SCA_KEY_BITS_192 != keybits) &&
             (ARM_CE_SCA_KEY_BITS_256 != keybits) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        if ( isSecKey && (ARM_CE_SCA_KEY_BITS_192 == keybits) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    case ARM_CE_SM4_CCM:
        if ( (ARM_CE_SCA_KEY_BITS_128 != keybits) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    default:
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    return 0;
}

int arm_ce_ccm_set_key( arm_ce_ccm_ctx_t *ctx,
                        const uint8_t *key,
                        size_t keybits )
{
    int ret = 0;
    dbh_ccm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == key) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    /** sanity check keybits */
    ret = ce_ccm_check_keybits( prv->alg, keybits, false );
    if ( 0 != ret ) {
        return ret;
    }
    switch (prv->state)
    {
    case CE_CCM_STATE_INIT:
    case CE_CCM_STATE_READY:
        break;
    case CE_CCM_STATE_RAW:
    case CE_CCM_STATE_START:
    case CE_CCM_STATE_UPDATE_AAD:
    case CE_CCM_STATE_UPDATE_PAYLOAD:
    default:
        return DBH_CCM_ERR_BAD_STATE;
    }
    ret = arm_ce_sca_set_key( &prv->cctx,
                              ARM_CE_SCA_EXTERNAL_KEY,
                              key, NULL, NULL, keybits, 0 );
    if ( 0 != ret ) {
        return ret;
    }

    ret = arm_ce_cbcmac_set_key( &prv->cmctx, key, keybits );
    if (0 == ret) {
        prv->state = CE_CCM_STATE_READY;
    }
    return ret;
}

int arm_ce_ccm_set_derived_key( arm_ce_ccm_ctx_t *ctx,
                                int32_t type,
                                const uint8_t *key1,
                                const uint8_t *key2,
                                const uint8_t *key3,
                                size_t keybits )
{
    int ret = 0;
    dbh_ccm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == key1) || (NULL == key2) ||
         (NULL == key3) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    /** check keytype is acceptable */
    if ( (ARM_CE_SCA_MODEL_KEY != type) &&
         (ARM_CE_SCA_DEVICE_ROOT_KEY != type) ) {
        return DBH_SCA_KEY_TYPE_INVAL_ERR;
    }
    /** sanity check keybits */
    ret = ce_ccm_check_keybits( prv->alg, keybits, true );
    if ( 0 != ret ) {
        return ret;
    }
    switch (prv->state)
    {
    case CE_CCM_STATE_INIT:
    case CE_CCM_STATE_READY:
        break;
    case CE_CCM_STATE_RAW:
    case CE_CCM_STATE_START:
    case CE_CCM_STATE_UPDATE_AAD:
    case CE_CCM_STATE_UPDATE_PAYLOAD:
    default:
        return DBH_CCM_ERR_BAD_STATE;
    }
    ret = arm_ce_sca_set_key( &prv->cctx,
                              type,
                              key1, key2, key3, keybits, 1);
    if ( 0 != ret ) {
        return ret;
    }

    ret = arm_ce_cbcmac_set_derived_key( &prv->cmctx,
                                         type,
                                         key1, key2, key3, keybits );
    if (0 == ret) {
        prv->state = CE_CCM_STATE_READY;
    }
    return ret;
}

static inline void ce_ccm_read_size( uint8_t *dst, size_t offs, uint64_t size )
{
    size_t i = 0;
    for ( i = 0; i < sizeof(size); i++, size >>= 8 ) {
        if ( 0 == size ) {
            break;
        }
        dst[offs - i] = size & 0xFF;
    }
}

static int ce_ccm_gen_b0( uint8_t *b0,
                          uint64_t len,
                          const uint8_t *nonce,
                          size_t noncelen,
                          uint64_t aadlen,
                          size_t taglen )
{
    if ( (NULL == b0) || (NULL == nonce) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    if (aadlen > 0) {
        b0[0] |= 1 << 6;
    }
    b0[0] |= ((taglen - 2) / 2) << 3;
    b0[0] |= 15 - noncelen - 1;
    pal_memcpy( b0 + 1, nonce, noncelen );
    ce_ccm_read_size( b0, 15, len );
    return 0;
}

static inline int ce_ccm_gen_a0( uint8_t *a0,
                                 const uint8_t *nonce,
                                 size_t noncelen )
{
    if ( (NULL == a0) || (NULL == nonce) || (noncelen > 13) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    a0[0] = 0;
    a0[0] |= 15 - noncelen - 1;
    pal_memcpy( a0 + 1, nonce, noncelen );
    pal_memset( a0 + 1 + noncelen, 0x00, ARM_CE_SCA_BLK_SIZE - 1 - noncelen );
    return 0;
}

static int ce_ccm_pad_aadlen( uint8_t *padding,
                              uint64_t aadlen,
                              size_t *padding_sz )
{
#define AADLEN_THREASHOLD_1     ((1UL << 16) - (1UL << 8))
#define AADLEN_THREASHOLD_2     (1ULL << 32)
    if ( (NULL == padding) || (NULL == padding_sz) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    if (aadlen < AADLEN_THREASHOLD_1) {
        if ( *padding_sz < 2 ) {
            return DBH_SCA_PARAM_INVAL_ERR; /** TODO revise error code to insufficient size? */
        }
        padding[0] = (aadlen >> 8) & 0xFF;
        padding[1] = aadlen & 0xFF;
        *padding_sz = 2;
    } else if ( aadlen < AADLEN_THREASHOLD_2 ) {
        if ( *padding_sz < 6 ) {
            return DBH_SCA_PARAM_INVAL_ERR; /** TODO revise error code to insufficient size? */
        }
        padding[0] = 0xFF;
        padding[1] = 0xFE;
        padding[2] = (aadlen >> 24) & 0xFF;
        padding[3] = (aadlen >> 16) & 0xFF;
        padding[4] =  (aadlen >> 8) & 0xFF;
        padding[5] =         aadlen & 0xFF;
        *padding_sz = 6;
    } else {
        if ( *padding_sz < 10 ) {
            return DBH_SCA_PARAM_INVAL_ERR; /** TODO revise error code to insufficient size? */
        }
        padding[0] = 0xFF;
        padding[1] = 0xFF;
        padding[2] = (aadlen >> 56) & 0xFF;
        padding[3] = (aadlen >> 48) & 0xFF;
        padding[4] = (aadlen >> 40) & 0xFF;
        padding[5] = (aadlen >> 32) & 0xFF;
        padding[6] = (aadlen >> 24) & 0xFF;
        padding[7] = (aadlen >> 16) & 0xFF;
        padding[8] =  (aadlen >> 8) & 0xFF;
        padding[9] =         aadlen & 0xFF;
        *padding_sz = 10;
    }
    return 0;
}

int arm_ce_ccm_start( arm_ce_ccm_ctx_t *ctx,
                      int32_t op,
                      const uint8_t *nonce,
                      size_t nonce_len,
                      size_t taglen,
                      uint64_t aadlen,
                      uint64_t payload_len )
{
    int ret = 0;
    uint8_t b0[ARM_CE_SCA_BLK_SIZE] = {0};
    uint8_t padding[ARM_CE_SCA_BLK_SIZE] = {0};
    size_t padding_sz = 0;
    dbh_ccm_ctx_t *prv = NULL;
    int32_t i = 0;
    uint64_t len = payload_len;

    if ( (NULL == ctx) ||
         ((DBH_CCM_ENCRYPT != op) &&  (DBH_CCM_DECRYPT != op)) ||
         (NULL == nonce) || (nonce_len < 7) || (nonce_len > 13) ||
         ((0 != (taglen & 1)) || (taglen < 4) ||
                (taglen > ARM_CE_SCA_BLK_SIZE)) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    /** check payload length */
    for ( i = 0; i < (15 - nonce_len); i++ ) {
        len >>= 8;
    }
    if ( len > 0 ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_CCM_STATE_READY:
    case CE_CCM_STATE_START:
        break;
    case CE_CCM_STATE_RAW:
    case CE_CCM_STATE_INIT:
    case CE_CCM_STATE_UPDATE_AAD:
    case CE_CCM_STATE_UPDATE_PAYLOAD:
    default:
        return DBH_CCM_ERR_BAD_STATE;
    }

    ret = ce_ccm_gen_b0( b0, payload_len, nonce, nonce_len, aadlen, taglen );
    if ( 0 != ret ) {
        return ret;
    }
    /** start cbcmac with iv all zero */
    ret = arm_ce_cbcmac_start( &prv->cmctx, padding );
    if ( 0 != ret ) {
        return ret;
    }
    ret = arm_ce_cbcmac_update( &prv->cmctx, ARM_CE_SCA_BLK_SIZE, b0 );
    if ( 0 != ret ) {
        goto err_update_b0;
    }
    /** if aadlen > 0 should update aadlen padding */
    if ( aadlen > 0 ) {
        padding_sz = sizeof(padding);
        ret = ce_ccm_pad_aadlen( padding,
                                aadlen,
                                &padding_sz );
        if ( 0 !=  ret ) {
            goto err_pad_aadlen;
        }
        ret = arm_ce_cbcmac_update( &prv->cmctx, padding_sz, padding );
        if ( 0 != ret ) {
            arm_ce_cbcmac_finish( &prv->cmctx, padding );
            return ret;
        }
    }
    ret = ce_ccm_gen_a0( prv->a0, nonce, nonce_len );
    if ( 0 != ret ) {
        goto err_gen_a0;
    }
    pal_memcpy( prv->ctr, prv->a0, ARM_CE_SCA_BLK_SIZE );
    /** q = 15 - nlen */
    COUNTER_INCREASE( prv->ctr, ARM_CE_SCA_BLK_SIZE, 15 - nonce_len, 1 );
    /** Record claimed lengths aadlen/payloadlen/taglen/nonce len */
    prv->claim_aadlen = aadlen;
    prv->aadlen = 0;
    prv->claim_mlen = payload_len;
    prv->mlen = 0;
    prv->claim_taglen = taglen;
    prv->nlen = nonce_len;
    prv->strpos = 0;
    pal_memset( prv->stream, 0x00, ARM_CE_SCA_BLK_SIZE );
    prv->op = ( (DBH_CCM_ENCRYPT == op) ? ARM_CE_SCA_OPERATION_ENCRYPT :
                                          ARM_CE_SCA_OPERATION_DECRYPT );
    prv->state = CE_CCM_STATE_START;
    return 0;
err_gen_a0:
err_pad_aadlen:
err_update_b0:
    arm_ce_cbcmac_finish( &prv->cmctx, padding );
    return ret;
}

int arm_ce_ccm_update_aad( arm_ce_ccm_ctx_t *ctx,
                           size_t aadlen,
                           const uint8_t *aad )
{
    int ret = 0;
    dbh_ccm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || ((aadlen > 0) && (NULL == aad)) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_CCM_STATE_START:
    case CE_CCM_STATE_UPDATE_AAD:
        break;
    case CE_CCM_STATE_RAW:
    case CE_CCM_STATE_INIT:
    case CE_CCM_STATE_READY:
    case CE_CCM_STATE_UPDATE_PAYLOAD:
    default:
        return DBH_CCM_ERR_BAD_STATE;
    }

    if ( 0 == aadlen ) {
        prv->state = CE_CCM_STATE_UPDATE_AAD;
        return 0;
    }
    /**check claim_aadlen eq aadlen and overflow*/
    if ( ((prv->aadlen + aadlen) > prv->claim_aadlen) ||
         ((prv->aadlen + aadlen) < prv->aadlen) ||
         ((prv->aadlen + aadlen) < aadlen) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    ret = arm_ce_cbcmac_update( &prv->cmctx, aadlen, aad );
    if ( 0 != ret ) {
        return ret;
    }
    prv->aadlen += aadlen;
    prv->state = CE_CCM_STATE_UPDATE_AAD;
    return ret;
}

static int ce_ccm_finish_update_aad( arm_ce_ccm_ctx_t *ctx )
{
    size_t aadlen_sz = 0;
    size_t padding_sz = 0;
    uint8_t padding[ARM_CE_SCA_BLK_SIZE] = {0};
    dbh_ccm_ctx_t *prv = NULL;

    if (NULL == ctx) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    if (prv->aadlen != prv->claim_aadlen) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    if (prv->aadlen > 0) {
        if (prv->aadlen < AADLEN_THREASHOLD_1) {
            aadlen_sz = 2;
        } else if ( prv->aadlen < AADLEN_THREASHOLD_2 ) {
            aadlen_sz = 6;
        } else {
            aadlen_sz = 10;
        }
        padding_sz = ARM_CE_SCA_BLK_SIZE -
                     ((prv->aadlen + aadlen_sz) % ARM_CE_SCA_BLK_SIZE);
        padding_sz %= ARM_CE_SCA_BLK_SIZE;
        if ( padding_sz > 0 ) {
            return arm_ce_cbcmac_update( &prv->cmctx, padding_sz, padding );
        }
    }

    return 0;
}

int arm_ce_ccm_update( arm_ce_ccm_ctx_t *ctx,
                       size_t inlen,
                       const uint8_t *in,
                       uint8_t *out )
{
    int ret = 0;
    dbh_ccm_ctx_t *prv = NULL;
    uint8_t iv[ARM_CE_SCA_BLK_SIZE];

    if ( (NULL == ctx) || ((inlen > 0) && ((NULL == in) || (NULL == out))) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_CCM_STATE_UPDATE_AAD:
        ret = ce_ccm_finish_update_aad( ctx );
        if (0 != ret) {
            goto err_fin_upaad;
        }
        prv->state = CE_CCM_STATE_UPDATE_PAYLOAD;
        break;
    case CE_CCM_STATE_START: /**< may start with no aad data, so here still need to change state */
        prv->state = CE_CCM_STATE_UPDATE_PAYLOAD;
        break;
    case CE_CCM_STATE_UPDATE_PAYLOAD:
        break;
    case CE_CCM_STATE_RAW:
    case CE_CCM_STATE_INIT:
    case CE_CCM_STATE_READY:
    default:
        return DBH_CCM_ERR_BAD_STATE;
    }

    if ( 0 == inlen ) {
        return 0;
    }
    pal_memset( iv, 0x00, sizeof(iv) );
    /**check claim_mlen eq mlen and overflow*/
    if ( ((prv->mlen + inlen) > prv->claim_mlen) ||
         ((prv->mlen + inlen) < prv->mlen) ||
         ((prv->mlen + inlen) < inlen) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    /** q = 15 - nlen, so for ccm valid ctr bit width should be
     *  n*8 bits and n = {2,3,4,5,6,7,8} */
    pal_memcpy( iv, prv->ctr, sizeof(iv) );
    if ( ARM_CE_SCA_OPERATION_DECRYPT == prv->op ) {
        ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                                CE_CCM_GET_CTR_ALG(prv), ARM_CE_NONE_MAC,
                                inlen, &prv->strpos, iv, prv->stream,
                                in, out );
        if ( 0 != ret ) {
            return ret;
        }
        ret = arm_ce_cbcmac_update( &prv->cmctx, inlen, out );
    } else { /** DECRYPT */
        ret = arm_ce_cbcmac_update( &prv->cmctx, inlen, in );
        if ( 0 != ret ) {
            return ret;
        }
        ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                                CE_CCM_GET_CTR_ALG(prv), ARM_CE_NONE_MAC,
                                inlen, &prv->strpos, iv, prv->stream,
                                in, out );
    }
    if ( 0 != ret ) {
        return ret;
    }
    /** q = 15 - nlen, so for ccm valid ctr bit width should be
     *  n*8 bits and n = {2,3,4,5,6,7,8} */
    pal_memcpy( prv->ctr + ARM_CE_SCA_BLK_SIZE - prv->nlen,
                iv + ARM_CE_SCA_BLK_SIZE - prv->nlen,
                prv->nlen );
    prv->mlen += inlen;
    prv->state = CE_CCM_STATE_UPDATE_PAYLOAD;
    return 0;
err_fin_upaad:
    return ret;
}

int arm_ce_ccm_finish( arm_ce_ccm_ctx_t *ctx,
                       uint8_t *tag, size_t taglen )
{
    int ret = 0;
    uint8_t padding[ARM_CE_SCA_BLK_SIZE] = {0};
    size_t padding_sz = 0;
    uint8_t stream[ARM_CE_SCA_BLK_SIZE] = {0};
    size_t strpos = 0;
    dbh_ccm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == tag) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GET_CCM_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    if ( (prv->mlen != prv->claim_mlen) || (taglen != prv->claim_taglen) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_CCM_STATE_UPDATE_AAD:
        ret = ce_ccm_finish_update_aad(ctx);
        if ( 0!= ret ) {
            goto err_up_aad;
        }
        break;
    case CE_CCM_STATE_UPDATE_PAYLOAD:
        break;
    case CE_CCM_STATE_RAW:
    case CE_CCM_STATE_INIT:
    case CE_CCM_STATE_READY:
    case CE_CCM_STATE_START:
    default:
        return DBH_CCM_ERR_BAD_STATE;
    }

    /** check if need padding and handle padding */
    if ( 0 != (prv->mlen % ARM_CE_SCA_BLK_SIZE) ) {
        padding_sz = ARM_CE_SCA_BLK_SIZE - (prv->mlen % ARM_CE_SCA_BLK_SIZE);
        pal_memset( padding, 0x00, padding_sz );
        ret = arm_ce_cbcmac_update( &prv->cmctx, padding_sz, padding );
        if ( 0 != ret ) {
            goto err_cmup_padding;
        }
    }

    ret = arm_ce_cbcmac_finish( &prv->cmctx, padding );
    if ( 0 != ret ) {
        goto err_cm_fin;
    }
    ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                            CE_CCM_GET_CTR_ALG(prv), ARM_CE_NONE_MAC,
                            ARM_CE_SCA_BLK_SIZE, &strpos, prv->a0, stream,
                            padding, padding );
    if ( 0 != ret ) {
        goto err_sca_crypt_tag;
    }
    pal_memcpy(tag, padding, taglen);
    prv->state = CE_CCM_STATE_READY;
    return 0;
err_cmup_padding:
err_sca_crypt_tag:
err_cm_fin:
err_up_aad:
    return ret;
}