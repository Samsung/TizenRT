/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#define SL_TEST_ECDH_KEY_SLOT_A 32
#define SL_TEST_ECDH_KEY_SLOT_B 33
#define SL_TEST_AUTH_MEM_SIZE 4096

static sl_ctx g_hnd;
static hal_data g_key_a;
static hal_data g_key_b;
static hal_data g_shared_secret_a;
static hal_data g_shared_secret_b;
static hal_ecdh_data ecdh_a;
static hal_ecdh_data ecdh_b;

TESTCASE_SETUP(ecdh_compute)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_b, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_b, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &g_key_a, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &g_key_b, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ecdh_a.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_a.pubkey_x);
	ecdh_a.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_a.pubkey_y);
	ecdh_b.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_b.pubkey_x);
	ecdh_b.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_b.pubkey_y);

	//data: pubkey_x, priv: pubkey_y
	ecdh_a.curve = HAL_ECDSA_SEC_P256R1;
	ecdh_a.pubkey_x->data = g_key_a.data;
	ecdh_a.pubkey_x->data_len = g_key_a.data_len;
	ecdh_a.pubkey_y->data = g_key_a.priv;
	ecdh_a.pubkey_y->data_len = g_key_a.priv_len;

	//data: pubkey_x, priv: pubkey_y
	ecdh_b.curve = HAL_ECDSA_SEC_P256R1;
	ecdh_b.pubkey_x->data = g_key_b.data;
	ecdh_b.pubkey_x->data_len = g_key_b.data_len;
	ecdh_b.pubkey_y->data = g_key_b.priv;
	ecdh_b.pubkey_y->data_len = g_key_b.priv_len;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_b, SL_TEST_AUTH_MEM_SIZE));
} END_TEST_F

TESTCASE_TEARDOWN(ecdh_compute)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	sl_test_free_buffer(&g_key_a);
	sl_test_free_buffer(&g_key_b);
	free(ecdh_a.pubkey_x);
	free(ecdh_a.pubkey_y);
	free(ecdh_b.pubkey_x);
	free(ecdh_b.pubkey_y);
	sl_test_free_buffer(&g_shared_secret_a);
	sl_test_free_buffer(&g_shared_secret_b);
} END_TEST_F

START_TEST_F(ecdh_compute)
{
	hal_result_e hres = HAL_FAIL;
	// A<--B, B<--A
	if (ecdh_a.pubkey_x != NULL && ecdh_b.pubkey_x != NULL) {
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &ecdh_b, SL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &ecdh_a, SL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);

		ST_EXPECT_EQ(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
		ST_EXPECT_EQ(0, memcmp(g_shared_secret_a.data, g_shared_secret_b.data, g_shared_secret_a.data_len));
	}
} END_TEST_F

void sl_handle_auth_ecdh(sl_options *opt)
{
	ST_SET_PACK(sl_auth);
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ecdh compute", ecdh_compute);
	ST_RUN_TEST(sl_auth);
	ST_RESULT_TEST(sl_auth);
}
