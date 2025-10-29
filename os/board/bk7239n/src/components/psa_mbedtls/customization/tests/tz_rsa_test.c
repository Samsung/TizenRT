/*
 *  RSA simple data encryption program
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#include <string.h>
#include <driver/flash.h>
#include "common/bk_typedef.h"
#include "FreeRTOS.h"
#include "task.h"
#include <mbedtls/platform.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>
#include <mbedtls/aes.h>
#include <mbedtls/x509_csr.h>
#include "mbedtls/entropy.h"
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/pk.h>
#if defined(CONFIG_INT_WDT)
#include <driver/wdt.h>
#include <bk_wdt.h>
#endif
static uint32_t s_count = 0;
static volatile uint32_t s_delay_time = 3;

extern int my_random(void *p_rng, unsigned char *output, size_t output_len);

extern int tz_mbedtls_pk_parse_key(mbedtls_pk_context *pk,
                    char *key, size_t keylen,
                    const unsigned char *pwd, size_t pwdlen);

extern int tz_mbedtls_pk_decrypt(mbedtls_pk_context *ctx,
                    const unsigned char *input, size_t ilen,
                    unsigned char *output, size_t *olen, size_t osize,
                    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);

extern void tz_mbedtls_pk_free(mbedtls_pk_context *ctx);

extern int tz_set_private_key( int index, unsigned char * key );
extern char*  key_mgt_get_client_key(int key_idx);

extern int tz_mbedtls_asymmetric_keygen(mbedtls_pk_context *pk_key, int key_idx, int type);

static int psa_pk_test_init(mbedtls_pk_context* key_gen_ctx,unsigned char* prv_pem_buf)
{
	int ret  = 0;

	mbedtls_pk_init( key_gen_ctx );

    if( ( ret = mbedtls_pk_setup( key_gen_ctx,mbedtls_pk_info_from_type( MBEDTLS_PK_RSA ) ) ) != 0 )
    {
        bk_printf( "\n ERROR: mbedtls_pk_setup returned -0x%04x \n", -ret );
    }
    else
    {
        bk_printf("\n mbedtls_pk_setup succeed. \n");
    }

 	bk_printf("\n Generating rsa key...  \n");
    if( (ret = mbedtls_rsa_gen_key( mbedtls_pk_rsa( *key_gen_ctx ), my_random, NULL, 1024 , 65537 )) != 0 )
    {
        bk_printf( "\n ERROR: mbedtls_rsa_gen_key returned -0x%04x \n", -ret );
    }
    else
    {
        bk_printf("\n mbedtls_rsa_gen_key succeed. \n");
    }

    if( ( ret = mbedtls_pk_write_key_pem( key_gen_ctx, prv_pem_buf, 4096 ) ) != 0 )
    {
        bk_printf( "\n ERROR: mbedtls_pk_write_key_pem returned -0x%04x \n", -ret );
    }
    else
    {
        bk_printf("\n mbedtls_pk_write_key_pem succeed. \n");
    }

	return ret;
}
static void psa_rsa_encrypt_decrypt_test(void *arg)
{
	int ret  = 0;
	size_t olen = 0;
	uint32_t key_idx = 0;
	int i = 0;

    char *test_client_key = NULL;
	mbedtls_pk_context pk_ctx;
    mbedtls_pk_context key_gen_ctx;
    unsigned char prv_pem_buf[4096] = {0};
    unsigned char plain_text[512] = "This is a test.";
    unsigned char enc_text[1024] = {0};
    unsigned char dec_text[1024] = {0};

	psa_pk_test_init(&key_gen_ctx,prv_pem_buf);

    key_idx = 8;
    tz_set_private_key(key_idx, prv_pem_buf);

    test_client_key = key_mgt_get_client_key( key_idx );
    if( test_client_key == NULL )
    {
        bk_printf("\n ERROR: key_mgt_get_client_key failed. \n");
		goto exit;
    }
    else
    {
        bk_printf("\n key_mgt_get_client_key succeed. \n");
    }

	while(1)
	{
		s_count++;
		bk_printf("pk_encrypt_decrypt_test  g_count = %d\r\n", s_count);

#if defined(CONFIG_INT_WDT)
		bk_wdt_feed();
#endif
#if defined(CONFIG_TASK_WDT)
		extern void bk_task_wdt_feed(void);
		bk_task_wdt_feed();
#endif

		//RSA encryption
	    if( ( ret = mbedtls_pk_encrypt( &key_gen_ctx, plain_text, strlen( (char*)plain_text ),enc_text, &olen, sizeof(enc_text),my_random, NULL ) ) != 0 )
	        bk_printf( "\n ERROR: mbedtls_pk_encrypt returned -0x%04x. \n\n", -ret );
	    else
	        bk_printf("\n mbedtls_pk_encrypt succeed. \n");

	    //mbedtls_pk_free( &key_gen_ctx );
#if defined(CONFIG_INT_WDT)
		bk_wdt_feed();
#endif
#if defined(CONFIG_TASK_WDT)
		extern void bk_task_wdt_feed(void);
		bk_task_wdt_feed();
#endif

		//RSA decryption

	    memset( dec_text, 0, sizeof(dec_text) );
	    if( ( ret = tz_mbedtls_pk_parse_key( &pk_ctx, test_client_key, strlen((char*)test_client_key)+1 , NULL, 0 )) != 0)
	    {
	        bk_printf("\n ERROR: tz_mbedtls_pk_parse_key failed returned -0x%04x. \n", -ret);
			continue;
			//goto exit;
	    }
	    else
	    {
	        bk_printf("\n tz_mbedtls_pk_parse_key succeed. \n");
	    }

	    if((ret = tz_mbedtls_pk_decrypt( &pk_ctx, enc_text, olen, dec_text, &olen, sizeof(dec_text), my_random, NULL)) != 0)
	    {
	        bk_printf("\n ERROR: tz_mbedtls_pk_decrypt failed returned -0x%04x. \n", -ret);
			continue;
			//goto exit;
	    }
	    else
	    {
	        bk_printf("\n tz_mbedtls_pk_decrypt succeed. \n");
	    }

	    for(i=0; i< strlen((char*)plain_text); i++)
		{
	        if( dec_text[i] != plain_text[i] )
			{
	            bk_printf("\n ERROR: tz_mbedtls_pk_decrypt result failed. \n");
	            break;
	        }
	    }

	    if(i == strlen((char*)plain_text))
	    {
	        bk_printf("\n tz_mbedtls_pk_decrypt result compare succeed. \n");
	    }

	    //tz_mbedtls_pk_free( &pk_ctx );

		extern int bk_rand(void);
		rtos_delay_milliseconds(((uint32_t)bk_rand()%s_delay_time));
	}

 exit:
 	tz_mbedtls_pk_free( &pk_ctx );
	mbedtls_pk_free( &key_gen_ctx );

}

int psa_rsa_test_main(void)
{
	int ret  = 0;
    rtos_create_thread(NULL, 4,
		"psa_rsa_test",
		(beken_thread_function_t)psa_rsa_encrypt_decrypt_test,
		20*1024,
		(beken_thread_arg_t)0);

	return ret;
}
