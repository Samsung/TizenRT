#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <security_hal.h>
#include <stress_tool/st_perf.h>

void hal_test_free_buffer(hal_data *data)
{
	if (data->data) {
		free(data->data);
		data->data = NULL;
	}
	data->data_len = 0;
}

/*  Configuration */
#define HAL_KEYMGR_TEST_TRIAL 1
#define HAL_KEYMGR_TEST_LIMIT_TIME 100000000

/*
 * Desc: Set key
 */
#define HAL_TEST_KEY_LEN 16
#define HAL_TEST_KEY_SLOT 1
static hal_data g_aes_key;

TEST_SETUP(set_key)
{
	ST_START_TEST;
	g_aes_key.data_len = HAL_TEST_KEY_LEN;
	g_aes_key.data = (char *)malloc(HAL_TEST_KEY_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	ST_END_TEST;
}

TEST_TEARDOWN(set_key)
{
	ST_START_TEST;
	free(g_aes_key.data);
	ST_END_TEST;
}

TEST_F(set_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_set_key(&g_aes_key, HAL_KEY_AES_128, HAL_TEST_KEY_SLOT));
	ST_END_TEST;
}

/*
 * Desc: Remove key in Secure Storage
 */
TEST_SETUP(remove_key)
{
	ST_START_TEST;
	g_aes_key.data_len = HAL_TEST_KEY_LEN;
	g_aes_key.data = (char *)malloc(HAL_TEST_KEY_LEN);
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	ST_EXPECT(0, hal_set_key(&g_aes_key, HAL_KEY_AES_128, HAL_TEST_KEY_SLOT));
	ST_END_TEST;
}

TEST_TEARDOWN(remove_key)
{
	ST_START_TEST;
	free(g_aes_key.data);
	ST_END_TEST;
}

TEST_F(remove_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_remove_key(HAL_KEY_AES_128, HAL_TEST_KEY_SLOT));
	ST_END_TEST;
}

/*
 * Desc: Generate hmac key
 */
#define HAL_TEST_HMAC_KEY_LEN 32
TEST_SETUP(gen_hmac_key)
{
	ST_START_TEST;
	g_aes_key.data_len = HAL_TEST_HMAC_KEY_LEN;
	g_aes_key.data = (char *)malloc(sizeof(g_aes_key.data_len));
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	ST_END_TEST;
}

TEST_TEARDOWN(gen_hmac_key)
{
	ST_START_TEST;
	hal_test_free_buffer(&g_aes_key);
	ST_END_TEST;
}

TEST_F(gen_hmac_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_hmac_generate_key(HAL_KEY_HMAC_SHA256, HAL_TEST_KEY_SLOT, &g_aes_key));
	ST_END_TEST;
}

/*
 * Desc: Generate RSA key
 */
#define HAL_TEST_RSA_KEY_LEN (1024/8)
TEST_SETUP(gen_rsa_key)
{
	ST_START_TEST;
	g_aes_key.data_len = HAL_TEST_RSA_KEY_LEN;
	g_aes_key.data = (char *)malloc(sizeof(g_aes_key.data_len));
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	ST_END_TEST;
}

TEST_TEARDOWN(gen_rsa_key)
{
	ST_START_TEST;
	hal_test_free_buffer(&g_aes_key);
	ST_END_TEST;
}

TEST_F(gen_rsa_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_rsa_generate_key(HAL_KEY_RSA_1024, HAL_TEST_KEY_SLOT, &g_aes_key));
	ST_END_TEST;
}

/*
 * Desc: Generate ECC Key
 */

#define HAL_TEST_MAX_KEY_SIZE 1280

TEST_SETUP(gen_ecc_key)
{
	ST_START_TEST;
	g_aes_key.data_len = HAL_TEST_MAX_KEY_SIZE;
	g_aes_key.data = (char *)malloc(sizeof(g_aes_key.data_len));
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	ST_END_TEST;
}

TEST_TEARDOWN(gen_ecc_key)
{
	ST_START_TEST;
	hal_test_free_buffer(&g_aes_key);
	ST_END_TEST;
}

TEST_F(gen_ecc_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_ecc_generate_key(HAL_ECP_DP_SECP192R1, HAL_TEST_KEY_SLOT, &g_aes_key));
	ST_END_TEST;
}

/*
 * Desc: Get ECDSA public key
 */
static hal_ecc_key g_ecc_key;
#define HAL_TEST_MAX_ECP_KEY_SIZE (68)

TEST_SETUP(get_ecdsa_key)
{
	ST_START_TEST;

	g_aes_key.data_len = HAL_TEST_MAX_KEY_SIZE;
	g_aes_key.data = (char *)malloc(sizeof(g_aes_key.data_len));
	ST_EXPECT_NEQ(NULL, g_aes_key.data);
	ST_EXPECT(0, hal_ecc_generate_key(HAL_ECP_DP_SECP192R1, HAL_TEST_KEY_SLOT, &g_aes_key));

	g_ecc_key.publickey_x = (char *)malloc(sizeof(HAL_TEST_MAX_ECP_KEY_SIZE));
	ST_EXPECT_NEQ(NULL, g_ecc_key.publickey_x);

	g_ecc_key.publickey_y = (char *)malloc(sizeof(HAL_TEST_MAX_ECP_KEY_SIZE));
	ST_EXPECT_NEQ(NULL, g_ecc_key.privatekey);

	ST_END_TEST;
}

TEST_TEARDOWN(get_ecdsa_key)
{
	ST_START_TEST;
	hal_test_free_buffer(&g_aes_key);

	free(g_ecc_key.publickey_x);
	free(g_ecc_key.publickey_y);

	ST_END_TEST;
}

TEST_F(get_ecdsa_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_ecdsa_get_pubkey(HAL_ECP_DP_SECP192R1, HAL_TEST_KEY_SLOT, &g_ecc_key));
	ST_END_TEST;
}

/*
 * Desc: Get RSA key from Secure Storage
 */
static hal_rsa_key g_rsa_key;
TEST_SETUP(get_rsa_key)
{
	ST_START_TEST;

	g_rsa_key.publickey = (char *)malloc(sizeof(HAL_TEST_MAX_KEY_SIZE));
	ST_EXPECT_NEQ(NULL, g_rsa_key.publickey);
	g_rsa_key.privatekey = (char *)malloc(sizeof(HAL_TEST_MAX_KEY_SIZE));
	ST_EXPECT_NEQ(NULL, g_rsa_key.privatekey);

	ST_END_TEST;
}

TEST_TEARDOWN(get_rsa_key)
{
	ST_START_TEST;
	free(g_rsa_key.publickey);
	free(g_rsa_key.privatekey);
	ST_END_TEST;
}

TEST_F(get_rsa_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_rsa_get_pubkey(HAL_TEST_KEY_SLOT, &g_rsa_key));
	ST_END_TEST;
}


ST_SET_SMOKE_TAIL(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Set key", set_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Remove key", remove_key, set_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Generate HMAC key", gen_hmac_key, remove_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Generate RSA key", gen_rsa_key, gen_hmac_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Generate ECC key", gen_ecc_key, gen_rsa_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Generate ECDSA key", get_ecdsa_key, gen_ecc_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Get RSA key", get_rsa_key, get_ecdsa_key);
ST_SET_PACK(hal_keymgr, get_rsa_key);


int hal_keymgr_test(void)
{
	ST_RUN_TEST(hal_keymgr);
	ST_RESULT_TEST(hal_keymgr);

	return 0;
}
