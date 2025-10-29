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

#include <string.h>
#include <stdio.h>
#include <os/mem.h>
#include "dubhe_sca.h"

#include "otp_s.h"

#include "tfm_secure_api.h"
#include "tfm_dubhe_key_ladder_nsc.h"

//#include <components/log.h>

#define LADDER_TAG "LADDER"

#define LADDER_LOGI(...) BK_LOGI(LADDER_TAG, ##__VA_ARGS__)
#define LADDER_LOGW(...) BK_LOGW(LADDER_TAG, ##__VA_ARGS__)
#define LADDER_LOGE(...) BK_LOGE(LADDER_TAG, ##__VA_ARGS__)
#define LADDER_LOGD(...) BK_LOGD(LADDER_TAG, ##__VA_ARGS__)

#if 0//CONFIG_KEY_LADDER_DEBUG//_g_ek1/2/3 for test
static uint8_t _g_ek3[] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
                           0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66};

static uint8_t _g_ek2[] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
                           0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66};

static uint8_t _g_ek1[] = {0xAA, 0x98, 0x42, 0x70, 0x6E, 0x74, 0x19, 0xDD,
	                       0xD4, 0x4C, 0x22, 0x9A, 0x21, 0xB2, 0x77, 0xBD,
	                       0x21, 0xB2, 0xAC, 0x0E, 0xF9, 0x1F, 0x1D, 0xF9,
	                       0xCC, 0xC2, 0xCE, 0x89, 0xE0, 0x62, 0x93, 0xC5};
#endif
#define MBEDTLS_ERR_AES_INVALID_KEY_LENGTH                -0x0020  /**< Invalid key length. */
#define MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH              -0x0022  /**< Invalid data input length. */
#define MBEDTLS_ERR_AES_MALLOC                            -0x0023
#define MBEDTLS_ERR_AES_HW_INIT                           -0x0024
#define MBEDTLS_ERR_AES_INVALID_PARAM                     -0x002F
#define MBEDTLS_ERR_AES_GENERIC                           -0x0030

#define LADDER_DUBHE_BASE_S     (0x4b110000)
#if CONFIG_KEY_LADDER_DEBUG
static arm_ce_sca_context_t g_sca_context;
#endif
void psa_key_ladder_nsc_stub(void)
{
	return;
}

extern  int otp_active();
extern void otp_sleep();
extern bk_err_t otp_read(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size);

#define OTP_ACTIVE() do {\
	if(otp_active() == -1){ \
		otp_sleep(); \
		return BK_FAIL; \
	}\
} while(0)

#define DUBHE_KEY_LADDER_SIZE_BIT (256)
static int dubhe_aes_setkey_id( arm_ce_sca_context_t *ctx, uint32_t item )
{
	int ret = 0;
#if CONFIG_OTP_NSC
	//for test, need get ek123 by otp id
	uint8_t ek3[16];     /**< encrypted key3 (fixed to 128-bit) */
	uint8_t ek2[16];     /**< encrypted key2 (fixed to 128-bit) */
	uint8_t ek1[32];     /**< encrypted key1 */
	uint32_t size;

	//read ek1/2/3 from OTP
	OTP_ACTIVE();
	size = otp_map_2[OTP_EK_3].allocated_size;
	ret = otp_read(2, OTP_EK_3, ek3, size);
	if(ret < 0)
		goto err_exit;
	size = otp_map_2[OTP_EK_2].allocated_size;
	ret = otp_read(2, OTP_EK_2, ek2, size);
	if(ret < 0)
		goto err_exit;
	size = otp_map_2[item].allocated_size;
	ret = otp_read(2, item, ek1, size);
	if(ret < 0)
		goto err_exit;

#if CONFIG_KEY_LADDER_DEBUG
	printf("\r\n ek3 text \r\n");
	for(int i = 0; i < sizeof(ek3); i++)
		{
		if(i%8==0)
			printf("\r\n");

		printf("0x%02x, ",ek3[i]);
	}
	printf("\r\n ek2 text \r\n");
	for(int i = 0; i < sizeof(ek2); i++)
		{
		if(i%8==0)
			printf("\r\n");

		printf("0x%02x, ",ek2[i]);
	}
	printf("\r\n ek1 text index =%d\r\n", (int)item);
	for(int i = 0; i < sizeof(ek1); i++)
		{
		if(i%8==0)
			printf("\r\n");

		printf("0x%02x, ",ek1[i]);
	}
#endif
#if 0//CONFIG_KEY_LADDER_DEBUG
	memcpy(ek3, _g_ek3, sizeof(_g_ek3));
	memcpy(ek2, _g_ek2, sizeof(_g_ek2));
	memcpy(ek1, _g_ek1, DUBHE_KEY_LADDER_SIZE_BIT/8);
#endif

	ret = arm_ce_sca_set_key(ctx, ARM_CE_SCA_MODEL_KEY,
		ek1, ek2, ek3, DUBHE_KEY_LADDER_SIZE_BIT, 1 );

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
#endif //#if CONFIG_OTP_NSC

err_exit:
	otp_sleep();
	return ret;
}

/* Implementation that should never be optimized out by the compiler */
static void dubhe_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = (unsigned char *) v;
    while ( n-- )
        *p++ = 0;
}

static inline void buff_free(void *p)
{
	if (p != NULL)
	{
		os_free(p);
          	// cppcheck-suppress uselessAssignmentPtrArg
		p = NULL;
	}
}
__tfm_psa_secure_gateway_no_naked_attributes__  int dubhe_key_ladder_crypt_aes_cbc( key_ladder_context_t *ex_ctx, size_t length,
                         const unsigned char *input,
                         unsigned char *output )
{
	arm_ce_sca_context_t ctx;
	const unsigned char  *in_temp = NULL;
	unsigned char        *out_temp = NULL;
	unsigned char        iv[16];
	int                  ret = 0;

    if ( ex_ctx == NULL || input == NULL || output == NULL )
		return ( MBEDTLS_ERR_AES_INVALID_PARAM );

	//if(ex_ctx->key_id > xxx)  need to do ,check otp id

	//dubhe init
	ret = arm_ce_sca_init( &ctx );
    if ( ret < 0 )
    {
        LADDER_LOGE( "init sca context failed,ret=%d \r\n", ret);
        return MBEDTLS_ERR_AES_HW_INIT;
    }
    ctx.alg_type = ARM_CE_AES;

	//dubhe aes ladder set key and iv
	ret = dubhe_aes_setkey_id( &ctx, ex_ctx->key_id );
	if ( ret < 0 )
    {
        LADDER_LOGE( "key ladder set key failed,ret=%d \r\n", ret);
		goto clearup;
    }
	memcpy(iv, ex_ctx->iv, 0x10);
	//dubhe aes ladder encrypt
	in_temp  = os_malloc( length );
	out_temp = os_malloc( length );
	if( in_temp ==NULL || out_temp == NULL )
	{
		ret = MBEDTLS_ERR_AES_MALLOC;
		goto clearup;
	}
	memcpy((void*) in_temp, (const void *)input, length );
    ret = arm_ce_sca_crypt( &ctx,
                             ( ( AES_OPERATION_ENCRYPT == ex_ctx->mode ) ?
                                 ARM_CE_SCA_OPERATION_ENCRYPT :
                                 ARM_CE_SCA_OPERATION_DECRYPT ),
                             ARM_CE_AES_CBC,
                             0,
                             length,
                             NULL,
                             iv,
                             NULL,
                             in_temp,
                             out_temp );

	if(ret < 0)
	{
		LADDER_LOGE( "key ladder encrypt failed,ret=%d \r\n", ret );
		goto clearup;
	}

#if CONFIG_KEY_LADDER_DEBUG
	printf("\r\n input text,mode = %d \r\n",ex_ctx->mode);
	for(int i = 0; i < length; i++)
		{
		if(i%8==0)
			printf("\r\n");

		printf("0x%02x, ",in_temp[i]);
	}
	printf("\r\n output text,mode = %d \r\n",ex_ctx->mode);
	for(int i = 0; i < length; i++)
		{
		if(i%8==0)
			printf("\r\n");

		printf("0x%02x, ",out_temp[i]);
	}
#endif
	memcpy( output, out_temp, length );
clearup:
	buff_free((void *) in_temp );
	buff_free(out_temp );
	arm_ce_sca_free( &ctx, NULL );
	dubhe_zeroize( &ctx, sizeof( arm_ce_sca_context_t ) );

	return ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int dubhe_key_ladder_crypt_aes_ecb( key_ladder_context_t *ex_ctx, size_t length,
                         const unsigned char *input,
                         unsigned char *output )
{
	arm_ce_sca_context_t ctx;
	const unsigned char  *in_temp = NULL;
	unsigned char        *out_temp = NULL;
	int                  ret = 0;

    if ( ex_ctx == NULL || input == NULL || output == NULL)
		return ( MBEDTLS_ERR_AES_INVALID_PARAM );

	//if(ex_ctx->key_id > xxx)  need to do ,check otp id
	//dubhe init
	ret = arm_ce_sca_init( &ctx );
    if ( ret < 0 )
    {
        //printf( "init sca context failed\n" );
        return MBEDTLS_ERR_AES_HW_INIT;
    }
    ctx.alg_type = ARM_CE_AES;

	//dubhe aes ladder set key and iv
	ret = dubhe_aes_setkey_id( &ctx, ex_ctx->key_id );
	if ( ret < 0 )
    {
        LADDER_LOGE( "key ladder set key failed,ret=%d \r\n", ret );
		goto clearup;
    }
	//dubhe aes ladder encrypt
	in_temp  = os_malloc( length );
	out_temp = os_malloc( length );
	if( in_temp ==NULL || out_temp == NULL )
	{
		ret = MBEDTLS_ERR_AES_MALLOC;
		goto clearup;
	}
	memcpy( (void *)in_temp, input, length );
    ret = arm_ce_sca_crypt( &ctx,
                             ( ( AES_OPERATION_ENCRYPT == ex_ctx->mode ) ?
                                 ARM_CE_SCA_OPERATION_ENCRYPT :
                                 ARM_CE_SCA_OPERATION_DECRYPT ),
                             ARM_CE_AES_ECB,
                             0,
                             length,
                             NULL,
                             NULL,
                             NULL,
                             in_temp,
                             out_temp );
	if(ret < 0)
	{
		LADDER_LOGE( "key ladder encrypt failed,ret=%d \r\n", ret);
		goto clearup;
	}
	memcpy( output, out_temp, length );
clearup:
	//dubhe_aes_ladder_free
	buff_free((void *) in_temp );
	buff_free( out_temp );
	arm_ce_sca_free( &ctx, NULL );
	dubhe_zeroize( &ctx, sizeof( arm_ce_sca_context_t ) );

	return ret;
}


#if CONFIG_KEY_LADDER_DEBUG
/* Implementation that should never be optimized out by the compiler */
static void dubhe_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = (unsigned char *) v;
    while ( n-- )
        *p++ = 0;
}
__tfm_psa_secure_gateway_no_naked_attributes__ void dubhe_aes_ladder_init( void )
{
    int ret = 0;
	arm_ce_sca_context_t *ctx = &g_sca_context;

    ret = arm_ce_sca_init( ctx );
    if ( ret < 0 )
    {
        printf( "init sca context failed\n" );
        return;
    }
    ctx->alg_type = ARM_CE_AES;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void dubhe_aes_ladder_free( void )
{
	arm_ce_sca_context_t *ctx = &g_sca_context;
    if ( ctx == NULL )
        return;

    arm_ce_sca_free( ctx, NULL );
    dubhe_zeroize( ctx, sizeof( arm_ce_sca_context_t ) );
	//dubhe_driver_cleanup();
}

__tfm_psa_secure_gateway_no_naked_attributes__ int dubhe_aes_ladder_set_model_key( unsigned int ek1bits )
{
	arm_ce_sca_context_t *ctx = &g_sca_context;
	int ret = -1;
	uint8_t ek3[16];     /**< encrypted key3 (fixed to 128-bit) */
	uint8_t ek2[16];     /**< encrypted key2 (fixed to 128-bit) */
	uint8_t ek1[32];     /**< encrypted key1 */

	memcpy(ek3, _g_ek3, sizeof(_g_ek3));
	memcpy(ek2, _g_ek2, sizeof(_g_ek2));
	memcpy(ek1, _g_ek1, ek1bits/8);

	ret = arm_ce_sca_set_key( ctx, ARM_CE_SCA_MODEL_KEY, ek1, ek2, ek3, ek1bits, 1 );

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

__tfm_psa_secure_gateway_no_naked_attributes__ int dubhe_aes_ladder_set_iv(unsigned char iv[16] )
{
	arm_ce_sca_context_t *ctx = &g_sca_context;
	if ( ctx == NULL )
		return ( MBEDTLS_ERR_AES_INVALID_PARAM );

	memcpy( ctx->inter_value.last_blk, iv, 0x10 );

	return 0;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int dubhe_aes_ladder_encrypt_cbc( size_t length,
                         const unsigned char *input,
                         unsigned char *output )
{
	int ret = -1;

	arm_ce_sca_context_t *ctx = &g_sca_context;
    if ( ctx == NULL )
        return ( MBEDTLS_ERR_AES_INVALID_PARAM );
    if ( length % 16 )
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );

	const unsigned char * input_temp = os_malloc(length);
	unsigned char * output_temp = os_malloc(length);
	memcpy(input_temp, input, length);

    ret = arm_ce_sca_crypt( ctx,
                             1, //encrypt
                             ARM_CE_AES_CBC,
                             0,
                             length,
                             NULL,
                             ctx->inter_value.last_blk,
                             NULL,
                             input_temp,
                             output_temp );
	if(ret < 0){
		os_free(input_temp);
		os_free(output_temp);
		return ret;
	}

	printf("\r\n ciper text \r\n");
	for(int i = 0; i < length; i++)
		{
		if(i%8==0)
			printf("\r\n");

		printf("0x%02x, ",output_temp[i]);
	}

	memcpy(output, output_temp,length);
	os_free(input_temp);
	os_free(output_temp);

	return 0;
}
__tfm_psa_secure_gateway_no_naked_attributes__ int dubhe_aes_ladder_decrypt_cbc( size_t length,
                         const unsigned char *input,
                         unsigned char *output )
{
	int ret =-1;
	arm_ce_sca_context_t *ctx = &g_sca_context;

    if ( length % 16 )
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );

	const unsigned char * input_temp = os_malloc(length);
	unsigned char * output_temp = os_malloc(length);
	memcpy(input_temp, input, length);

    ret = arm_ce_sca_crypt( ctx,
                             0, //decrypt
                             ARM_CE_AES_CBC,
                             0,
                             length,
                             NULL,
                             ctx->inter_value.ctr_value.cur_iv,
                             NULL,
                             input_temp,
                             output_temp );

	if(ret < 0)
	{
		os_free(input_temp);
		os_free(output_temp);
		return ret;
	}

	printf("\r\n input ciper text \r\n");
	for(int i = 0; i < length; i++)
		{
		if(i%8==0)
			printf("\r\n");

		printf("0x%02x, ",input_temp[i]);
	}

	memcpy(output, output_temp,length);
	os_free(input_temp);
	os_free(output_temp);

	return ret;
}

#endif