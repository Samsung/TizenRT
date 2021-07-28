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

#define SL_TEST_ECC_KEY_SLOT 32
#define SL_TEST_ECC_HASH_LEN 32
#define SL_TEST_AUTH_MEM_SIZE 4096

static sl_ctx g_hnd;
static hal_data g_ecdsa_signature;
static hal_data g_ecdsa_hash;
static hal_ecdsa_mode g_ecdsa_mode;

TESTCASE_SETUP(ecdsa_sign)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_signature, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_hash, SL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = SL_TEST_ECC_HASH_LEN;
	g_ecdsa_mode.hash_t = HAL_HASH_SHA256;
} END_TEST_F

TESTCASE_TEARDOWN(ecdsa_sign)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	sl_test_free_buffer(&g_ecdsa_hash);
	sl_test_free_buffer(&g_ecdsa_signature);
} END_TEST_F

START_TEST_F(ecdsa_sign)
{
	hal_result_e hres = HAL_FAIL;

	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_sign_md(g_hnd, g_ecdsa_mode,
											  &g_ecdsa_hash,
											  SL_TEST_ECC_KEY_SLOT,
											  &g_ecdsa_signature, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
} END_TEST_F

void sl_handle_auth_ecdsa_signature(sl_options *opt)
{
	ST_SET_PACK(sl_auth);
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ecdsa sign", ecdsa_sign);
	ST_RUN_TEST(sl_auth);
	ST_RESULT_TEST(sl_auth);

	return;
}
