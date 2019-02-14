#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <security_hal.h>
#include <stress_tool/st_perf.h>
#include "hal_test_utils.h"

/*  Configuration */
#define HAL_CRYPTO_TEST_TRIAL 5
#define HAL_CRYPTO_TEST_LIMIT_TIME 100000000

#define HAL_TEST_KEY_LEN 32
#define HAL_TEST_KEY_SLOT 1
#define HAL_TEST_AES_IV_LEN 16
#define HAL_TEST_AES_DATA_LEN 16
#define HAL_TEST_RSA_DATA_LEN 256

/*
 * Desc: Encrypt data using AES
 * Refered https://developer.artik.io/documentation/security-api/see-encdec-test_8c-example.html
 */
static hal_data g_aes_key;
static hal_aes_param g_aes_param;
static hal_data g_aes_input;
static hal_data g_aes_output;
const static unsigned char test_aes[] = "1234567890123456";
TEST_SETUP(aes_encrypt)
{
	ST_START_TEST;

	g_aes_key.data = (unsigned char *)malloc(HAL_TEST_KEY_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	g_aes_key.data_len = HAL_TEST_KEY_LEN;
	memset(g_aes_key.data, 0xa5, HAL_TEST_KEY_LEN);
	ST_EXPECT(0, hal_set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key, NULL));

	g_aes_param.mode = HAL_AES_ECB_NOPAD;
	g_aes_param.iv = (unsigned char *)malloc(HAL_TEST_AES_IV_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_param.iv);
	g_aes_param.iv_len = HAL_TEST_AES_IV_LEN;
	memset(g_aes_param.iv, 0xa5, HAL_TEST_AES_IV_LEN);

	g_aes_input.data = (unsigned char *)malloc(HAL_TEST_AES_DATA_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_input.data);
	g_aes_input.data_len = HAL_TEST_AES_DATA_LEN;
	memcpy(g_aes_input.data, test_aes, HAL_TEST_AES_DATA_LEN);

	ST_END_TEST;
}

TEST_TEARDOWN(aes_encrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));
	hal_test_free_buffer(&g_aes_key);
	hal_test_free_buffer(&g_aes_input);
	hal_free_data(&g_aes_output);
	free(g_aes_param.iv);

	ST_END_TEST;
}

TEST_F(aes_encrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_aes_encrypt(&g_aes_input, &g_aes_param, HAL_TEST_KEY_SLOT, &g_aes_output));

	ST_END_TEST;
}

/*
 * Desc: Decrypt data using AES
 * Refered https://developer.artik.io/documentation/security-api/see-encdec-test_8c-example.html
 */
static hal_data g_aes_final;
TEST_SETUP(aes_decrypt)
{
	ST_START_TEST;

	g_aes_key.data = (unsigned char *)malloc(HAL_TEST_KEY_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	g_aes_key.data_len = HAL_TEST_KEY_LEN;
	memset(g_aes_key.data, 0xa5, HAL_TEST_KEY_LEN);
	ST_EXPECT(0, hal_set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key, NULL));

	g_aes_param.mode = HAL_AES_ECB_NOPAD;
	g_aes_param.iv = (unsigned char *)malloc(HAL_TEST_AES_IV_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_param.iv);
	g_aes_param.iv_len = HAL_TEST_AES_IV_LEN;
	memset(g_aes_param.iv, 0xa5, HAL_TEST_AES_IV_LEN);

	g_aes_input.data = (unsigned char *)malloc(HAL_TEST_AES_DATA_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_input.data);
	g_aes_input.data_len = HAL_TEST_AES_DATA_LEN;
	memcpy(g_aes_input.data, test_aes, HAL_TEST_AES_DATA_LEN);
	ST_EXPECT(0, hal_aes_encrypt(&g_aes_input, &g_aes_param, HAL_TEST_KEY_SLOT, &g_aes_output));

	ST_END_TEST;
}

TEST_TEARDOWN(aes_decrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));
	hal_test_free_buffer(&g_aes_key);
	hal_test_free_buffer(&g_aes_input);
	hal_free_data(&g_aes_output);
	hal_free_data(&g_aes_final);
	free(g_aes_param.iv);

	ST_END_TEST;
}

TEST_F(aes_decrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_aes_decrypt(&g_aes_output, &g_aes_param, HAL_TEST_KEY_SLOT, &g_aes_final));
	ST_EXPECT(0, memcmp(g_aes_input.data, g_aes_final.data, g_aes_final.data_len));

	ST_END_TEST;
}

/*
 * Desc: Encrypt data using RSA
 * Refered https://developer.artik.io/documentation/security-api/see-encdec-test_8c-example.html
 */
static hal_data g_rsa_input;
static hal_data g_rsa_output;
TEST_SETUP(rsa_encrypt)
{
	ST_START_TEST;
	
	ST_EXPECT(0, hal_generate_key(HAL_KEY_RSA_1024, HAL_TEST_KEY_SLOT));
	g_rsa_input.data = (unsigned char *)malloc(HAL_TEST_RSA_DATA_LEN);
	ST_EXPECT_NEQ(NULL, g_rsa_input.data);
	memset(g_rsa_input.data, 1, HAL_TEST_RSA_DATA_LEN);
	g_rsa_input.data_len = HAL_TEST_RSA_DATA_LEN;

	ST_END_TEST;
}

TEST_TEARDOWN(rsa_encrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_RSA_1024, HAL_TEST_KEY_SLOT));
	hal_test_free_buffer(&g_rsa_input);
	hal_free_data(&g_rsa_output);

	ST_END_TEST;
}

TEST_F(rsa_encrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_rsa_encrypt(&g_rsa_input, HAL_TEST_KEY_SLOT, &g_rsa_output));

	ST_END_TEST;
}

/*
 * Desc: Decrypt data using RSA
 * Refered https://developer.artik.io/documentation/security-api/see-encdec-test_8c-example.html
 */
static hal_data g_rsa_final;
TEST_SETUP(rsa_decrypt)
{
	ST_START_TEST;
	
	ST_EXPECT(0, hal_generate_key(HAL_KEY_RSA_1024, HAL_TEST_KEY_SLOT));
	g_rsa_input.data = (unsigned char *)malloc(HAL_TEST_RSA_DATA_LEN);
	ST_EXPECT_NEQ(NULL, g_rsa_input.data);
	memset(g_rsa_input.data, 1, HAL_TEST_RSA_DATA_LEN);
	g_rsa_input.data_len = HAL_TEST_RSA_DATA_LEN;
	ST_EXPECT(0, hal_rsa_encrypt(&g_rsa_input, HAL_TEST_KEY_SLOT, &g_rsa_output));

	ST_END_TEST;
}

TEST_TEARDOWN(rsa_decrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_RSA_1024, HAL_TEST_KEY_SLOT));
	hal_test_free_buffer(&g_rsa_input);
	hal_free_data(&g_rsa_output);
	hal_free_data(&g_rsa_final);

	ST_END_TEST;
}

TEST_F(rsa_decrypt)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_rsa_decrypt(&g_rsa_output, HAL_TEST_KEY_SLOT, &g_rsa_final));
	ST_EXPECT(0, memcmp(g_rsa_input.data, g_rsa_final.data, g_rsa_final.data_len));

	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(HAL_CRYPTO_TEST_TRIAL, HAL_CRYPTO_TEST_LIMIT_TIME, "Encrypt data using AES", aes_encrypt);
ST_SET_SMOKE(HAL_CRYPTO_TEST_TRIAL, HAL_CRYPTO_TEST_LIMIT_TIME, "Decrypt data using AES", aes_decrypt, aes_encrypt);
ST_SET_SMOKE(HAL_CRYPTO_TEST_TRIAL, HAL_CRYPTO_TEST_LIMIT_TIME, "Encrypt data using RSA", rsa_encrypt, aes_decrypt);
ST_SET_SMOKE(HAL_CRYPTO_TEST_TRIAL, HAL_CRYPTO_TEST_LIMIT_TIME, "Decrypt data using RSA", rsa_decrypt, rsa_encrypt);
ST_SET_PACK(hal_crypto, rsa_decrypt);

int hal_crypto_test(void)
{
	ST_RUN_TEST(hal_crypto);
	ST_RESULT_TEST(hal_crypto);

	return 0;
}
