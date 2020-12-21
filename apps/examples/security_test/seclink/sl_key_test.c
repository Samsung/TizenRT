/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include <stress_tool/st_perf.h>
#include "sl_test.h"

#define SL_TEST_KEY_MEM_SIZE 4096
#define SL_TEST_KEY_TRIAL 1
#define SL_TEST_KEY_LIMIT_TIME 1000000

// ToDo: key length will be fixed. it doesn't consider real key size.
#define SL_TEST_SYMKEY_LEN 32
#define SL_TEST_PUBKEY_LEN 32
#define SL_TEST_PRIKEY_LEN 32

#define SL_TEST_KEY_SLOT 1

static sl_ctx g_hnd;

/**
 * Description: Set key(Symmetric)
 */
static hal_data g_aes_key_in;
static hal_data g_aes_key_out;
TEST_SETUP(set_sym_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_aes_key_in, SL_TEST_SYMKEY_LEN));
	g_aes_key_in.data_len = SL_TEST_SYMKEY_LEN;
	memset(g_aes_key_in.data, 0xa5, SL_TEST_SYMKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_aes_key_out, SL_TEST_SYMKEY_LEN));
	g_aes_key_out.data_len = SL_TEST_SYMKEY_LEN;

	ST_END_TEST;
}

TEST_TEARDOWN(set_sym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	// hal layer should not return symmetric key
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &g_aes_key_out, &hres));
	ST_EXPECT_EQ(HAL_FAIL, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_aes_key_in);
	sl_test_free_buffer(&g_aes_key_out);

	ST_END_TEST;
}

TEST_F(set_sym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &g_aes_key_in, NULL, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}


/**
 * Description: Set keys (Asymmetric)
 */
static hal_data g_pubkey_in;
static hal_data g_prikey_in;
static hal_data g_pubkey_out;
static hal_data g_prikey_out;

TEST_SETUP(set_asym_pubprikey)
{
	ST_START_TEST;

	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_in, SL_TEST_PUBKEY_LEN));
	g_pubkey_in.data_len = SL_TEST_PUBKEY_LEN;
	memset(g_pubkey_in.data, 0xa5, SL_TEST_PUBKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_prikey_in, SL_TEST_PRIKEY_LEN));
	g_prikey_in.data_len = SL_TEST_PRIKEY_LEN;
	memset(g_prikey_in.data, 0xa6, SL_TEST_PRIKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_out, SL_TEST_PUBKEY_LEN));
	g_pubkey_out.data_len = SL_TEST_PUBKEY_LEN;

	ST_END_TEST;
}

TEST_TEARDOWN(set_asym_pubprikey)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_out, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	// SE can only return public key.
	ST_EXPECT_NEQ(0, g_pubkey_out.data_len);
	ST_EXPECT_EQ(0, g_pubkey_out.priv_len);
	ST_EXPECT_EQ(g_pubkey_in.data_len, g_pubkey_out.data_len);
	ST_EXPECT_EQ(0, memcmp(g_pubkey_in.data, g_pubkey_out.data, g_pubkey_out.data_len));

	sl_test_print_buffer(g_pubkey_out.data, g_pubkey_out.data_len, "Set public and private key");

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_pubkey_in);
	sl_test_free_buffer(&g_pubkey_out);
	sl_test_free_buffer(&g_prikey_in);

	ST_END_TEST;
}

TEST_F(set_asym_pubprikey)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_in, &g_prikey_in, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}


/**
 * Description: Set public key only (Asymmetric)
 */
TEST_SETUP(set_asym_pubkey)
{
	ST_START_TEST;

	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_in, SL_TEST_PUBKEY_LEN));
	g_pubkey_in.data_len = SL_TEST_PUBKEY_LEN;
	memset(g_pubkey_in.data, 0xa5, SL_TEST_PUBKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_out, SL_TEST_PUBKEY_LEN));
	g_pubkey_out.data_len = SL_TEST_PUBKEY_LEN;

	ST_END_TEST;
}

TEST_TEARDOWN(set_asym_pubkey)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_out, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	// SE can only return public key.
	ST_EXPECT_NEQ(0, g_pubkey_out.data_len);
	ST_EXPECT_EQ(0, g_pubkey_out.priv_len);
	ST_EXPECT_EQ(g_pubkey_in.data_len, g_pubkey_out.data_len);
	ST_EXPECT_EQ(0, memcmp(g_pubkey_in.data, g_pubkey_out.data, g_pubkey_out.data_len));

	sl_test_print_buffer(g_pubkey_out.data, g_pubkey_out.data_len, "Set public key");

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_pubkey_in);
	sl_test_free_buffer(&g_pubkey_out);

	ST_END_TEST;
}

TEST_F(set_asym_pubkey)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_in, NULL, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}

/**
 * Description: Set private key only (Asymmetric)
 */
TEST_SETUP(set_asym_prikey)
{
	ST_START_TEST;

	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_prikey_in, SL_TEST_PRIKEY_LEN));
	g_pubkey_in.data_len = SL_TEST_PRIKEY_LEN;
	memset(g_prikey_in.data, 0xa6, SL_TEST_PRIKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_prikey_out, SL_TEST_PRIKEY_LEN));
	g_prikey_out.data_len = SL_TEST_PRIKEY_LEN;

	ST_END_TEST;
}

TEST_TEARDOWN(set_asym_prikey)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_prikey_out, &hres));
	ST_EXPECT_EQ(HAL_BAD_KEY, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_prikey_in);
	sl_test_free_buffer(&g_prikey_out);

	ST_END_TEST;
}

TEST_F(set_asym_prikey)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, NULL, &g_prikey_in, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}


/**
 * Description: Get key (Symmetric)
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_aes_key_out;

TEST_SETUP(get_sym_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_aes_key_in, SL_TEST_KEY_MEM_SIZE));
	g_aes_key_in.data_len = SL_TEST_SYMKEY_LEN;
	memset(g_aes_key_in.data, 0xa5, SL_TEST_SYMKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_aes_key_out, SL_TEST_KEY_MEM_SIZE));

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &g_aes_key_in, NULL, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}

TEST_TEARDOWN(get_sym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_aes_key_in);
	sl_test_free_buffer(&g_aes_key_out);

	ST_END_TEST;
}

TEST_F(get_sym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &g_aes_key_out, &hres));
	ST_EXPECT_EQ(HAL_BAD_KEY, hres);

	ST_END_TEST;
}


/**
 * Description: Get public key
 */
#define SL_TEST_PUBKEY_LEN 128

TEST_SETUP(get_pub_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_in, SL_TEST_PUBKEY_LEN));
	g_pubkey_in.data_len = SL_TEST_PUBKEY_LEN;
	memset(g_pubkey_in.data, 0xa5, SL_TEST_PUBKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_out, SL_TEST_PUBKEY_LEN));
	g_pubkey_out.data_len = SL_TEST_PUBKEY_LEN;

	hal_result_e hres = HAL_FAIL;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_in, NULL, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}

TEST_TEARDOWN(get_pub_key)
{
	ST_START_TEST;

	// SE can only return public key.
	ST_EXPECT_NEQ(0, g_pubkey_out.data_len);
	ST_EXPECT_EQ(0, g_pubkey_out.priv_len);
	ST_EXPECT_EQ(g_pubkey_in.data_len, g_pubkey_out.data_len);
	ST_EXPECT_EQ(0, memcmp(g_pubkey_in.data, g_pubkey_out.data, g_pubkey_out.data_len));

	sl_test_print_buffer(g_pubkey_out.data, g_pubkey_out.data_len, "Get public key");

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_pubkey_in);
	sl_test_free_buffer(&g_pubkey_out);
	sl_test_free_buffer(&g_prikey_in);

	ST_END_TEST;
}

TEST_F(get_pub_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_out, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}


/**
 * Description: Remove key in Secure Storage
 */
TEST_SETUP(remove_sym_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_aes_key_in, SL_TEST_KEY_MEM_SIZE));
	g_aes_key_in.data_len = SL_TEST_SYMKEY_LEN;
	memset(g_aes_key_in.data, 0xa5, SL_TEST_SYMKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_aes_key_out, SL_TEST_KEY_MEM_SIZE));
	g_aes_key_out.data_len = SL_TEST_SYMKEY_LEN;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &g_aes_key_in, NULL, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}

TEST_TEARDOWN(remove_sym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &g_aes_key_out, &hres));
	ST_EXPECT_EQ(HAL_FAIL, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_aes_key_in);
	sl_test_free_buffer(&g_aes_key_out);

	ST_END_TEST;
}

TEST_F(remove_sym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}


/**
 * Description: Remove key in Secure Storage
 */
TEST_SETUP(remove_asym_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_in, SL_TEST_KEY_MEM_SIZE));
	g_pubkey_in.data_len = SL_TEST_PUBKEY_LEN;
	memset(g_pubkey_in.data, 0xa5, SL_TEST_PUBKEY_LEN);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_pubkey_out, SL_TEST_KEY_MEM_SIZE));
	g_pubkey_out.data_len = SL_TEST_PUBKEY_LEN;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_in, NULL, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}

TEST_TEARDOWN(remove_asym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &g_pubkey_out, &hres));
	ST_EXPECT_EQ(HAL_FAIL, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	sl_test_free_buffer(&g_pubkey_in);
	sl_test_free_buffer(&g_pubkey_out);

	ST_END_TEST;
}

TEST_F(remove_asym_key)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}

/**
 * Description: Generate key (Positive case)
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static int g_key_length[] = {SL_TEST_KEY_MEM_SIZE, SL_TEST_KEY_MEM_SIZE};
static int g_key_index[] = {32, 33};
static hal_key_type g_key_type[] = {HAL_KEY_AES_256, HAL_KEY_ECC_BRAINPOOL_P256R1};
static hal_data g_key_out[] = {HAL_DATA_INITIALIZER, HAL_DATA_INITIALIZER};
static g_key_arr_size = 0;

TEST_SETUP(generate_key_p)
{
	ST_START_TEST;
	g_key_arr_size = sizeof(g_key_length)/sizeof(int);

	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));

	for (int i = 0; i < g_key_arr_size; i++) {
		ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_out[i], g_key_length[i]));
		g_key_out[i].data_len = g_key_length[i];
	}

	ST_END_TEST;
}

TEST_TEARDOWN(generate_key_p)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;

	for (int i = 0; i < g_key_arr_size) {
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, g_key_type[i], g_key_index[i], &g_key_out[i], &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
		ST_EXPECT_NEQ(0, g_key_out[i].data_len);
		ST_EXPECT_EQ(0, g_key_out[i].priv_len);

		sl_test_print_buffer(g_key_out[i].data, g_key_out[i].data_len, "Generate key");

		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, g_key_type[i], g_key_index[i], &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
		sl_test_free_buffer(&g_key_out[i]);
	}

	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));

	ST_END_TEST;
}

TEST_F(generate_key_p)
{
	ST_START_TEST;

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_AES_256, SL_TEST_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set symmetric key", set_sym_key);
ST_SET_SMOKE(SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set asymmetric public private key", set_asym_pubprikey, set_sym_key);
ST_SET_SMOKE(SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set asymmetric public key", set_asym_pubkey, set_asym_pubprikey);
ST_SET_SMOKE(SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set asymmetric private key", set_asym_prikey, set_asym_pubkey);
ST_SET_SMOKE(SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Get key", get_sym_key, set_asym_prikey);
ST_SET_SMOKE(SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Remove key", remove_sym_key, get_sym_key);
ST_SET_SMOKE(SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Generate key", generate_key_p, remove_sym_key);
ST_SET_PACK(sl_keymgr, generate_key_p);


void sl_keymgr_test(void)
{
	ST_RUN_TEST(sl_keymgr);
	ST_RESULT_TEST(sl_keymgr);
}

