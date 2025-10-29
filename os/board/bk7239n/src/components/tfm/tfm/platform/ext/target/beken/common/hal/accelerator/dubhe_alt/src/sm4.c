#if !defined( MBEDTLS_CONFIG_FILE )
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined( MBEDTLS_SM4_C )
#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */

#if defined( ARM_CE_DUBHE_SCA )

#include <string.h>
#include "mbedtls/sm4.h"

void mbedtls_sm4_init( mbedtls_sm4_context *ctx )
{
    int ret = 0;

    if ( ctx != NULL )
    {
        arm_ce_sca_context_t *sca_ctx = (arm_ce_sca_context_t *) mbedtls_calloc(
            1, sizeof( arm_ce_sca_context_t ) );
        if ( sca_ctx != NULL )
        {
            memset( sca_ctx, 0, sizeof( arm_ce_sca_context_t ) );
            ret = arm_ce_sca_init( sca_ctx );
            if ( ret < 0 )
            {
                mbedtls_printf( "init sca context failed\n" );
                mbedtls_free( sca_ctx );
                return;
            }

            sca_ctx->alg_type = ARM_CE_SM4;
        }
        else
        {
            mbedtls_printf( "calloc memory for sca context failed\n" );
            return;
        }
        ctx->sca_ctx = sca_ctx;
    }
    else
        mbedtls_printf( "ctx is NULL" );
    return;
}

void mbedtls_sm4_free( mbedtls_sm4_context *ctx )
{
    if ( ctx == NULL || ctx->sca_ctx == NULL )
        return;

    arm_ce_sca_free( ctx->sca_ctx, NULL );
    mbedtls_free( ctx->sca_ctx );
    ctx->sca_ctx = NULL;
}

/*
 * SM4 key schedule (encryption)
 */
int mbedtls_sm4_setkey_enc( mbedtls_sm4_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    int ret = 0;
    if ( ctx == NULL || ctx->sca_ctx == NULL )
        return ( -1 );

    ret                         = arm_ce_sca_set_key(
        ctx->sca_ctx, ARM_CE_SCA_EXTERNAL_KEY, key, NULL, NULL, keybits, 0 );
    switch ( ret )
    {
    case DBH_SCA_KEYBITS_INVAL_ERR:
        ret = MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH;
        break;
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
        ret = MBEDTLS_ERR_SM4_INVALID_PARAM;
        break;
    default:
        ret = MBEDTLS_ERR_SM4_GENERIC;
        break;
    }

    return ret;
}

/*
 * SM4 key schedule (decryption)
 */
int mbedtls_sm4_setkey_dec( mbedtls_sm4_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    int ret = 0;

    if ( ctx == NULL || ctx->sca_ctx == NULL )
        return ( MBEDTLS_ERR_SM4_INVALID_PARAM );
    ret                         = arm_ce_sca_set_key(
        ctx->sca_ctx, ARM_CE_SCA_EXTERNAL_KEY, key, NULL, NULL, keybits, 0 );
    switch ( ret )
    {
    case DBH_SCA_KEYBITS_INVAL_ERR:
        ret = MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH;
        break;
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
        ret = MBEDTLS_ERR_SM4_INVALID_PARAM;
        break;
    default:
        ret = MBEDTLS_ERR_SM4_GENERIC;
        break;
    }

    return ret;
}

int mbedtls_sm4_set_derived_key( mbedtls_sm4_context *ctx,
                                 int key_type,
                                 int mode,
                                 const unsigned char *ek1,
                                 const unsigned char *ek2,
                                 const unsigned char *ek3,
                                 unsigned int ek1bits )
{
    int ret = -1;
    if ( ctx == NULL )
        return ( MBEDTLS_ERR_SM4_INVALID_PARAM );
    if ( key_type == MBEDTLS_SM4_DERIVED_MODEL_KEY )
        ret = arm_ce_sca_set_key(
            ctx->sca_ctx, ARM_CE_SCA_MODEL_KEY, ek1, ek2, ek3, ek1bits, 1 );
    else if ( key_type == MBEDTLS_SM4_DERIVED_DEVICE_ROOT_KEY )
        ret = arm_ce_sca_set_key( ctx->sca_ctx,
                                  ARM_CE_SCA_DEVICE_ROOT_KEY,
                                  ek1,
                                  ek2,
                                  ek3,
                                  ek1bits,
                                  1 );
    else
    {
        mbedtls_printf( "sm4 set derived key failed\n" );
        return ( MBEDTLS_ERR_SM4_INVALID_PARAM );
    }

    switch ( ret )
    {
    case DBH_SCA_KEYBITS_INVAL_ERR:
        ret = MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH;
        break;
    case 0:
        break;
    case DBH_SCA_PARAM_INVAL_ERR:
        ret = MBEDTLS_ERR_SM4_INVALID_PARAM;
        break;
    default:
        ret = MBEDTLS_ERR_SM4_GENERIC;
        break;
    }

    return ret;
}

/*
 * SM4-ECB block encryption/decryption
 */
int mbedtls_sm4_crypt_ecb( mbedtls_sm4_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    if ( ctx == NULL || ctx->sca_ctx == NULL )
        return ( -1 );

    return arm_ce_sca_crypt( ctx->sca_ctx,
                             ( ( MBEDTLS_SM4_ENCRYPT == mode ) ?
                               ARM_CE_SCA_OPERATION_ENCRYPT :
                               ARM_CE_SCA_OPERATION_DECRYPT ),
                             ARM_CE_SM4_ECB,
                             0,
                             ARM_CE_SCA_BLK_SIZE,
                             0,
                             NULL,
                             NULL,
                             input,
                             output );
}

/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_sm4_crypt_cbc( mbedtls_sm4_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    if ( ctx != NULL )
    {
        if ( length % 16 )
            return ( MBEDTLS_ERR_SM4_INVALID_INPUT_LENGTH );

        return arm_ce_sca_crypt( ctx->sca_ctx,
                                 ( ( MBEDTLS_SM4_DECRYPT == mode ) ?
                                   ARM_CE_SCA_OPERATION_DECRYPT :
                                   ARM_CE_SCA_OPERATION_ENCRYPT ),
                                 ARM_CE_SM4_CBC,
                                 0,
                                 length,
                                 0,
                                 iv,
                                 NULL,
                                 input,
                                 output );
    }
    else
        return ( -1 );
}

/*
 * SM4-CTR buffer encryption/decryption
 */
int mbedtls_sm4_crypt_ctr( mbedtls_sm4_context *ctx,
                           size_t length,
                           size_t *nc_off,
                           unsigned char nonce_counter[16],
                           unsigned char stream_block[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    if ( ctx == NULL || ctx->sca_ctx == NULL )
        return ( -1 );

    return arm_ce_sca_crypt( ctx->sca_ctx,
                             ARM_CE_SCA_OPERATION_ENCRYPT,
                             ARM_CE_SM4_CTR,
                             0,
                             length,
                             nc_off,
                             nonce_counter,
                             stream_block,
                             input,
                             output );
}
#endif /* ARM_CE_DUBHE_SCA */
#endif /* MBEDTLS_SM4_C */
