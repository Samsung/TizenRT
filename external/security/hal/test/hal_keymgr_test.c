#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <security_hal.h>
#include <stress_tool/st_perf.h>
#include "hal_test_utils.h"


/*  Configuration */
#define HAL_KEYMGR_TEST_TRIAL 5
#define HAL_KEYMGR_TEST_LIMIT_TIME 100000000

/*
 * Desc: Set key
 */
#define HAL_TEST_KEY_LEN 32
#define HAL_TEST_KEY_SLOT 1
static hal_data g_aes_key;
TEST_SETUP(set_key)
{
	ST_START_TEST;

	g_aes_key.data = (unsigned char *)malloc(HAL_TEST_KEY_LEN);

	ST_EXPECT_NEQ(NULL, g_aes_key.data);

	g_aes_key.data_len = HAL_TEST_KEY_LEN;

	memset(g_aes_key.data, 0xa5, HAL_TEST_KEY_LEN);

	ST_END_TEST;
}

TEST_TEARDOWN(set_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));
	hal_test_free_buffer(&g_aes_key);
	ST_END_TEST;
}

TEST_F(set_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key, NULL));
	ST_END_TEST;
}

/*
 * Desc: Get key
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(get_key)
{
	ST_START_TEST;

	g_aes_key.data = (unsigned char *)malloc(HAL_TEST_KEY_LEN);

	ST_EXPECT_NEQ(NULL, g_aes_key.data);

	g_aes_key.data_len = HAL_TEST_KEY_LEN;

	memset(g_aes_key.data, 0xa5, HAL_TEST_KEY_LEN);

	ST_EXPECT(0, hal_set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key, NULL));

	hal_test_free_buffer(&g_aes_key);

	ST_END_TEST;
}

TEST_TEARDOWN(get_key)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_free_data(&g_aes_key));

	ST_EXPECT(0, hal_remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));

	ST_END_TEST;
}

TEST_F(get_key)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_get_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key));

	ST_END_TEST;
}

/*
 * Desc: Remove key in Secure Storage
 */
TEST_SETUP(remove_key)
{
	ST_START_TEST;
	g_aes_key.data = (unsigned char *)malloc(HAL_TEST_KEY_LEN);

	ST_EXPECT_NEQ(NULL, g_aes_key.data);

	g_aes_key.data_len = HAL_TEST_KEY_LEN;

	memset(g_aes_key.data, 0xa5, HAL_TEST_KEY_LEN);

	ST_EXPECT(0, hal_set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key, NULL));

	ST_END_TEST;
}

TEST_TEARDOWN(remove_key)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_aes_key);

	ST_END_TEST;
}

TEST_F(remove_key)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_remove_key(HAL_KEY_AES_128, HAL_TEST_KEY_SLOT));
	ST_END_TEST;
}

/*
 * Desc: Generate key
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(generate_key)
{
	ST_START_TEST;

	ST_END_TEST;
}

TEST_TEARDOWN(generate_key)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));

	ST_END_TEST;
}

TEST_F(generate_key)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_generate_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));

	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Set key", set_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Get key", get_key, set_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Remove key", remove_key, get_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Generate key", generate_key, remove_key);
ST_SET_PACK(hal_keymgr, generate_key);

int hal_keymgr_test(void)
{
	ST_RUN_TEST(hal_keymgr);
	ST_RESULT_TEST(hal_keymgr);

	return 0;
}
