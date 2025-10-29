
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
#include "dubhe_sca.h"
#include "dubhe_ghash.h"
#include "dubhe_gcm.h"

#define IV_THRESHOLD        (12U)
#define GTR_SIZE            (ARM_CE_SCA_BLK_SIZE - IV_THRESHOLD)

/**< get GCM context ECB alg */
#define CE_GCM_GET_ECB_ALG(ctx) (                               \
            ( (ctx)->alg == ARM_CE_AES_GCM ) ? ARM_CE_AES_ECB : \
                                               ARM_CE_SM4_ECB )

/**< get GCM context CTR alg */
#define CE_GCM_GET_CTR_ALG(ctx) (                               \
            ( (ctx)->alg == ARM_CE_AES_GCM ) ? ARM_CE_AES_CTR : \
                                               ARM_CE_SM4_CTR )

/**< increase counter */
#define COUNTER_INCREASE(iv, bs, w, delta)                      \
            do {                                                \
                size_t ii = 0, jj = 0;                          \
                for ( ii = 0; ii < (delta); ii++ ) {            \
                    for ( jj = (bs); jj > ((bs) - (w)); jj--) { \
                        if ( 0 != ++(iv)[jj - 1]) {             \
                            break;                              \
                        }                                       \
                    }                                           \
                }                                               \
            } while( 0 )

#define CE_GCM_READ_COUNTER(iv) __extension__({                 \
                                    uint32_t counter = 0;       \
                                    counter = iv[15];           \
                                    counter |= iv[14] << 8;     \
                                    counter |= iv[13] << 16;    \
                                    counter |= iv[12] << 24;    \
                                    counter;                    \
                                })

/**< get GCM private context */
#define GCM_GET_PRV_CTX(ctx)    ((ctx)->gcm)

/**
 *  GCM states
 */
typedef enum {
    CE_GCM_STATE_RAW = 0,
    CE_GCM_STATE_INIT,
    CE_GCM_STATE_READY,
    CE_GCM_STATE_START,
    CE_GCM_STATE_UPDATE_AAD,
    CE_GCM_STATE_UPDATE_PAYLOAD,
} ce_gcm_state_t;

/**
 *  GCM  context
 */
typedef struct dbh_gcm_ctx {
    arm_ce_sca_type_t alg;              /*!< algorithm id */
    arm_ce_sca_context_t cctx;          /*!< ctr context */
    arm_ce_ghash_ctx_t gctx;            /*!< ghash context */
    arm_ce_sca_operation_t op;          /*!< operation mode */
    ce_gcm_state_t state;               /*!< gcm state */
    uint64_t aadlen;                    /*!< aad length in fact */
    uint64_t mlen;                      /*!< message length in fact */
    uint8_t j0[16];                     /*!< initial counter j0 */
    uint8_t ctr[16];                    /*!< nonce counter */
    uint8_t stream[16];                 /*!< stream block */
    size_t strpos;                      /*!< stream block offset */
} dbh_gcm_ctx_t;


int arm_ce_gcm_init( arm_ce_gcm_ctx_t *ctx, int32_t alg )
{
    int ret = 0;
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) ||
         ((ARM_CE_AES_GCM != alg) && (ARM_CE_SM4_GCM != alg)) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = pal_malloc( sizeof(*prv) );
    if ( NULL == prv ) {
        return DBH_GCM_ERR_OOM;
    }
    pal_memset( prv, 0x00, sizeof(*prv) );
    ret = arm_ce_ghash_init( &prv->gctx );
    if (0 != ret) {
        goto err_ghash_init;
    }
    ret = arm_ce_sca_init( &prv->cctx );
    if (0 != ret) {
        goto err_sca_init;
    }
    prv->alg = alg;
    prv->state = CE_GCM_STATE_INIT;
    ctx->gcm = prv;
    return 0;
err_sca_init:
    arm_ce_ghash_free(&prv->gctx);
err_ghash_init:
    pal_memset( prv, 0x00, sizeof(*prv) );
    pal_free( prv );
    return ret;
}

int arm_ce_gcm_free( arm_ce_gcm_ctx_t *ctx )
{
    int ret = 0;
    dbh_gcm_ctx_t *prv = NULL;

    if ( NULL == ctx ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_GCM_STATE_RAW:
        return 0;
    case CE_GCM_STATE_INIT:
    case CE_GCM_STATE_READY:
    case CE_GCM_STATE_START:
    case CE_GCM_STATE_UPDATE_AAD:
    case CE_GCM_STATE_UPDATE_PAYLOAD:
        ret = arm_ce_ghash_free( &prv->gctx );
        if (0 != ret) {
            goto err_free_gctx;
        }
        ret = arm_ce_sca_free( &prv->cctx, NULL );
        if (0 != ret) {
            goto err_free_cctx;
        }
        pal_memset( prv, 0x00, sizeof(*prv) );
        pal_free( prv );
        pal_memset( ctx, 0x00, sizeof(*ctx) );
        return 0;
    default:
        return DBH_GCM_ERR_BAD_STATE;
    }
err_free_cctx:
err_free_gctx:
    return ret;
}

/**
 *  check keybits requirement
 *  check if key valid for AES-GCM (128/192/256) SM4-GCM(128)
 */
static inline int ce_gcm_check_keybits( arm_ce_sca_type_t alg,
                                        size_t keybits,
                                        bool isSecKey )
{
    switch (alg)
    {
    case ARM_CE_AES_GCM:
        if ( (ARM_CE_SCA_KEY_BITS_128 != keybits) &&
             (ARM_CE_SCA_KEY_BITS_192 != keybits) &&
             (ARM_CE_SCA_KEY_BITS_256 != keybits) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        if ( isSecKey && (ARM_CE_SCA_KEY_BITS_192 == keybits) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    case ARM_CE_SM4_GCM:
        if ( (ARM_CE_SCA_KEY_BITS_128 != keybits) ) {
            return DBH_SCA_KEYBITS_INVAL_ERR;
        }
        break;
    default:
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    return 0;
}

int arm_ce_gcm_set_key( arm_ce_gcm_ctx_t *ctx,
                       const uint8_t *key,
                       size_t keybits )
{
    int ret = 0;
    uint8_t ghash_key[ARM_CE_SCA_BLK_SIZE] = {0};
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == key) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    /** sanity check keybits */
    ret = ce_gcm_check_keybits( prv->alg, keybits, false );
    if ( 0 != ret ) {
        return ret;
    }
    switch (prv->state)
    {
    case CE_GCM_STATE_INIT:
    case CE_GCM_STATE_READY:
        break;
    case CE_GCM_STATE_RAW:
    case CE_GCM_STATE_START:
    case CE_GCM_STATE_UPDATE_AAD:
    case CE_GCM_STATE_UPDATE_PAYLOAD:
    default:
        return DBH_GCM_ERR_BAD_STATE;
    }
    ret = arm_ce_sca_set_key( &prv->cctx,
                              ARM_CE_SCA_EXTERNAL_KEY,
                              key, NULL, NULL, keybits, 0 );
    if ( 0 != ret ) {
        return ret;
    }
    /* set ghash key */
    ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                            CE_GCM_GET_ECB_ALG(prv), ARM_CE_NONE_MAC,
                            ARM_CE_SCA_BLK_SIZE, NULL, NULL, NULL,
                            ghash_key, ghash_key );
    if ( 0 != ret ) {
        return ret;
    }
    ret = arm_ce_ghash_set_key( &prv->gctx, ghash_key );
    if (0 == ret) {
        prv->state = CE_GCM_STATE_READY;
    }
    return ret;
}

int arm_ce_gcm_set_derived_key( arm_ce_gcm_ctx_t *ctx,
                                int32_t type,
                                const unsigned char *key1,
                                const unsigned char *key2,
                                const unsigned char *key3,
                                size_t keybits )

{
    int ret = 0;
    uint8_t ghash_key[ARM_CE_SCA_BLK_SIZE] = {0};
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == key1) || (NULL == key2) ||
         (NULL == key3) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    /** check keytype is acceptable */
    if ( (ARM_CE_SCA_MODEL_KEY != type) &&
         (ARM_CE_SCA_DEVICE_ROOT_KEY != type) ) {
        return DBH_SCA_KEY_TYPE_INVAL_ERR;
    }
    /** sanity check keybits */
    ret = ce_gcm_check_keybits( prv->alg, keybits, true );
    if ( 0 != ret ) {
        return ret;
    }
    switch (prv->state)
    {
    case CE_GCM_STATE_INIT:
    case CE_GCM_STATE_READY:
        break;
    case CE_GCM_STATE_RAW:
    case CE_GCM_STATE_START:
    case CE_GCM_STATE_UPDATE_AAD:
    case CE_GCM_STATE_UPDATE_PAYLOAD:
    default:
        return DBH_GCM_ERR_BAD_STATE;
    }
    ret = arm_ce_sca_set_key( &prv->cctx,
                              type,
                              key1, key2, key3, keybits, 1);
    if ( 0 != ret ) {
        return ret;
    }
    /* set ghash key */
    ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                            CE_GCM_GET_ECB_ALG(prv), ARM_CE_NONE_MAC,
                            ARM_CE_SCA_BLK_SIZE, NULL, NULL, NULL,
                            ghash_key, ghash_key );
    if ( 0 != ret ) {
        return ret;
    }
    ret = arm_ce_ghash_set_key( &prv->gctx, ghash_key );
    if (0 == ret) {
        prv->state = CE_GCM_STATE_READY;
    }
    return ret;
}

static inline void ce_gcm_read_size( uint8_t *dst, size_t offs, uint64_t size )
{
    size_t i = 0;
    for ( i = 0; i < sizeof(size); i++, size >>= 8 ) {
        if ( 0 == size ) {
            break;
        }
        dst[offs - i] = size & 0xFF;
    }
}

static int ce_gcm_format_j0( arm_ce_gcm_ctx_t *ctx, uint8_t *j0,
                             const uint8_t *iv, uint64_t ivlen )
{
    uint8_t ghash_iv[ARM_CE_SCA_BLK_SIZE] = {0};
    uint8_t trash[ARM_CE_SCA_BLK_SIZE] = {0};
    uint8_t padding[ARM_CE_SCA_BLK_SIZE * 2] = {0};
    int ret = 0;
    size_t padding_sz = 0;
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == j0) || (NULL == iv) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    if (IV_THRESHOLD == ivlen) {
        pal_memcpy( j0, iv, ivlen );
        pal_memset( j0 + ivlen, 0x00, ARM_CE_SCA_BLK_SIZE - IV_THRESHOLD );
        j0[15] = 1;
    } else {
        ret = arm_ce_ghash_start( &prv->gctx, ghash_iv );
        if ( 0 != ret ) {
            return ret;
        }
        ret = arm_ce_ghash_update( &prv->gctx, ivlen, iv );
        if ( 0 != ret ) {
            goto err_update_iv;
        }
        /** do padding stuff */
        if ( ivlen % ARM_CE_SCA_BLK_SIZE != 0 ) {
            padding_sz = ARM_CE_SCA_BLK_SIZE - (ivlen % ARM_CE_SCA_BLK_SIZE);
        }
        /** pad iv length */
        padding_sz += ARM_CE_SCA_BLK_SIZE;
        ce_gcm_read_size( padding, padding_sz - 1, ivlen * 8 );
        ret = arm_ce_ghash_update( &prv->gctx, padding_sz, padding );
        if ( 0 != ret ) {
            goto err_update_padding;
        }
        ret = arm_ce_ghash_finish(&prv->gctx, j0);
    }

err_update_padding:
err_update_iv:
    arm_ce_ghash_finish( &prv->gctx, trash );
    return ret;
}

int arm_ce_gcm_start( arm_ce_gcm_ctx_t *ctx,
                      int32_t op,
                      const uint8_t *iv,
                      uint64_t iv_len )
{
    #define MAX_IV_LEN      ((1ULL << 61) - 1)
    int ret = 0;
    uint8_t ghash_iv[ARM_CE_SCA_BLK_SIZE] = {0};
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) ||
         ((DBH_GCM_ENCRYPT != op) && (DBH_GCM_DECRYPT != op)) ||
          (0 == iv_len) || (NULL == iv) || (iv_len  > MAX_IV_LEN) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    ret = ce_gcm_format_j0( ctx, prv->j0, iv, iv_len );
    if ( 0 != ret ) {
        return ret;
    }

    ret = arm_ce_ghash_start( &prv->gctx, ghash_iv );
    if ( 0 != ret ) {
        return ret;
    }

    pal_memcpy( prv->ctr, prv->j0, ARM_CE_SCA_BLK_SIZE );
    COUNTER_INCREASE(prv->ctr, ARM_CE_SCA_BLK_SIZE, GTR_SIZE, 1);
    prv->aadlen = 0;
    prv->mlen = 0;
    prv->strpos = 0;
    prv->op = ( (op == DBH_GCM_ENCRYPT) ? ARM_CE_SCA_OPERATION_ENCRYPT :
                                          ARM_CE_SCA_OPERATION_DECRYPT );
    pal_memset( prv->stream, 0x00, ARM_CE_SCA_BLK_SIZE );
    prv->state = CE_GCM_STATE_START;
    return ret;
}

int arm_ce_gcm_update_aad( arm_ce_gcm_ctx_t *ctx,
                           size_t aadlen,
                           const uint8_t *aad )
{
    int ret = 0;
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || ((NULL == aad) && (aadlen > 0)) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_GCM_STATE_START:
    case CE_GCM_STATE_UPDATE_AAD:
        break;
    case CE_GCM_STATE_UPDATE_PAYLOAD:
    case CE_GCM_STATE_RAW:
    case CE_GCM_STATE_INIT:
    case CE_GCM_STATE_READY:
    default:
        return DBH_GCM_ERR_BAD_STATE;
    }

    if ( 0 == aadlen ) {
        prv->state = CE_GCM_STATE_UPDATE_AAD;
        return 0;
    }
    /**check aadlen overflow*/
    if ( ((prv->aadlen + aadlen) < prv->aadlen) ||
         ((prv->aadlen + aadlen) < aadlen) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    ret = arm_ce_ghash_update( &prv->gctx, aadlen, aad );
    if ( 0 != ret ) {
        return ret;
    }
    prv->aadlen += aadlen;
    prv->state = CE_GCM_STATE_UPDATE_AAD;
    return ret;
}

static inline int ce_gcm_finish_update_aad( arm_ce_gcm_ctx_t *ctx )
{
#define MAX_AAD_LEN     ((1ULL << 61) - 1)
    int ret = 0;
    uint8_t padding[ARM_CE_SCA_BLK_SIZE] = {0};
    size_t padding_sz = 0;
    dbh_gcm_ctx_t *prv = NULL;

    if ( NULL == ctx ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    if ( prv->aadlen > MAX_AAD_LEN ) {
        return DBH_GCM_ERR_BAD_INPUT_LENGTH;
    }
    /**do padding stuff*/
    if ( (prv->aadlen > 0) && (prv->aadlen % ARM_CE_SCA_BLK_SIZE != 0) ) {
        padding_sz = ARM_CE_SCA_BLK_SIZE -
                        (prv->aadlen % ARM_CE_SCA_BLK_SIZE);
        ret = arm_ce_ghash_update( &prv->gctx, padding_sz, padding );
    }

    return ret;
}

int arm_ce_gcm_update( arm_ce_gcm_ctx_t *ctx,
                       size_t inlen,
                       const uint8_t *in,
                       uint8_t *out )
{
#define GTR_MAX     (0xFFFFFFFFUL)
#define PROCLEN_MAX (0xFFFFFFFFUL)
#define MAX_PAYLOAD_LEN     ((1ULL < 39) - 256)
    int ret = 0;
    size_t proclen = 0;
    size_t ctr_left = 0;
    size_t in_left = inlen;
    bool cy = false;
    uint8_t gtr[ARM_CE_SCA_BLK_SIZE] = {0};
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || ((inlen > 0) && ((NULL == in) || (NULL == out))) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GCM_GET_PRV_CTX( ctx );
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_GCM_STATE_START:
        prv->state = CE_GCM_STATE_UPDATE_PAYLOAD;
        break;
    case CE_GCM_STATE_UPDATE_AAD:
        /** first update of payload, should do some aad padding stuff */
        if ( 0 == prv->mlen ) {
            ret = ce_gcm_finish_update_aad(ctx);
            if ( 0 != ret ) {
                goto err_fin_upaad;
            }
        }
        prv->state = CE_GCM_STATE_UPDATE_PAYLOAD;
        break;
    case CE_GCM_STATE_UPDATE_PAYLOAD:
        break;
    case CE_GCM_STATE_RAW:
    case CE_GCM_STATE_INIT:
    case CE_GCM_STATE_READY:
    default:
        return DBH_GCM_ERR_BAD_STATE;
    }
    if ( 0 == inlen ) {
        return 0;
    }
    /** overflow detect */
    if ( ((prv->mlen + inlen) < prv->mlen) ||
         ((prv->mlen + inlen) < inlen) ||
         ((prv->mlen + inlen) > MAX_PAYLOAD_LEN)) {
        return DBH_GCM_ERR_BAD_INPUT_LENGTH;
    }

    while ( in_left > 0 ) {
        ctr_left = GTR_MAX - CE_GCM_READ_COUNTER( prv->ctr ) + 1;
        if ( ctr_left < ( ( PROCLEN_MAX / ARM_CE_SCA_BLK_SIZE ) + 1 ) ) {
            proclen = ctr_left * ARM_CE_SCA_BLK_SIZE;
        } else {
            /* set proclen to MAX_SIZE - BLK_SIZE when it overflow */
            proclen = PROCLEN_MAX & ( ~( ARM_CE_SCA_BLK_SIZE - 1 ) );
        }
        /* ctr overflow */
        if ( ctr_left == 0 ) {
            cy = true;
        } else {
            cy = false;
        }
        if ( prv->strpos > 0 ) {
            proclen += ( ARM_CE_SCA_BLK_SIZE - prv->strpos );
        }
        if ( ( proclen > in_left ) || cy ) {
            proclen = in_left;
        }
        pal_memcpy( gtr, prv->ctr, ARM_CE_SCA_BLK_SIZE );
        if ( ARM_CE_SCA_OPERATION_ENCRYPT == prv->op ) {
            ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                                    CE_GCM_GET_CTR_ALG(prv), ARM_CE_NONE_MAC,
                                    proclen, &prv->strpos, gtr, prv->stream,
                                    in + (inlen - in_left),
                                    out + (inlen - in_left) );
            if ( 0 != ret ) {
                return ret;
            }
            ret = arm_ce_ghash_update( &prv->gctx, proclen,
                                       out + (inlen - in_left) );
        } else { /*!< DECRYPT */
            ret = arm_ce_ghash_update( &prv->gctx, proclen,
                                       in + (inlen - in_left) );
            if ( 0 != ret ) {
                return ret;
            }
            ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                                    CE_GCM_GET_CTR_ALG(prv), ARM_CE_NONE_MAC,
                                    proclen, &prv->strpos, gtr, prv->stream,
                                    in + (inlen - in_left), out + (inlen - in_left) );
        }

        if ( 0 != ret ) {
            return ret;
        }
        in_left -= proclen;
        if ( 0 == in_left ) {
            pal_memcpy( prv->ctr + IV_THRESHOLD, gtr + IV_THRESHOLD, GTR_SIZE );
        } else {
            pal_memset( prv->ctr + IV_THRESHOLD, 0x00, GTR_SIZE );
        }
    }

    prv->mlen += inlen;
    prv->state = CE_GCM_STATE_UPDATE_PAYLOAD;
    return 0;
err_fin_upaad:
    return ret;
}

int arm_ce_gcm_finish( arm_ce_gcm_ctx_t *ctx,
                       uint8_t *tag,
                       size_t taglen )
{
    int ret = 0;
    uint8_t padding[ARM_CE_SCA_BLK_SIZE * 2] = {0}; /*< one block padding plus one block length*/
    size_t padding_sz = 0;
    uint8_t stream[ARM_CE_SCA_BLK_SIZE] = {0};
    size_t strpos = 0;
    dbh_gcm_ctx_t *prv = NULL;

    if ( (NULL == ctx) || (NULL == tag) ||
         (taglen < 4) || (taglen > ARM_CE_SCA_BLK_SIZE) ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }

    prv = GCM_GET_PRV_CTX(ctx);
    if ( NULL == prv ) {
        return DBH_SCA_PARAM_INVAL_ERR;
    }
    switch (prv->state)
    {
    case CE_GCM_STATE_START:
    case CE_GCM_STATE_UPDATE_AAD:
    case CE_GCM_STATE_UPDATE_PAYLOAD:
        break;
    case CE_GCM_STATE_RAW:
    case CE_GCM_STATE_INIT:
    case CE_GCM_STATE_READY:
    default:
        return DBH_GCM_ERR_BAD_STATE;
    }

    /** check if need padding and handle padding */
    if ( 0 != prv->mlen % ARM_CE_SCA_BLK_SIZE ) {
        padding_sz = ARM_CE_SCA_BLK_SIZE - (prv->mlen % ARM_CE_SCA_BLK_SIZE);
        pal_memset( padding, 0x00, padding_sz );
    }
    /** pad aad len and payload len  */
    padding_sz += 8;
    ce_gcm_read_size( padding, padding_sz - 1, prv->aadlen * 8 );
    padding_sz += 8;
    ce_gcm_read_size( padding, padding_sz - 1, prv->mlen * 8 );
    ret = arm_ce_ghash_update( &prv->gctx, padding_sz, padding );
    if ( 0 != ret ) {
        goto err_up_padding;
    }
    ret = arm_ce_ghash_finish( &prv->gctx, padding );
    if ( 0 != ret ) {
        goto err_ghash_fin;
    }
    ret = arm_ce_sca_crypt( &prv->cctx, ARM_CE_SCA_OPERATION_ENCRYPT,
                            CE_GCM_GET_CTR_ALG(prv), ARM_CE_NONE_MAC,
                            ARM_CE_SCA_BLK_SIZE, &strpos, prv->j0, stream,
                            padding, padding );
    if ( 0 != ret ) {
        goto err_sca_crypt_tag;
    }
    pal_memcpy(tag, padding, taglen);
    prv->state = CE_GCM_STATE_READY;
    return 0;
err_up_padding:
err_sca_crypt_tag:
err_ghash_fin:
    return ret;
}
