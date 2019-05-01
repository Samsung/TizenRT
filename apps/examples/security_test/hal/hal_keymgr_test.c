/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/seclink_drv.h>
#include <tinyara/security_hal.h>
#include <stress_tool/st_perf.h>
#include "hal_test_utils.h"
#include "hal_test.h"

extern struct sec_lowerhalf_s *se_get_device(void);

static struct sec_lowerhalf_s *g_se = NULL;

/*
 * Desc: Set key
 */
static hal_data g_aes_key_in;
TEST_SETUP(set_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_aes_key_in, HAL_KEYMGR_TEST_MEM_SIZE));
	g_aes_key_in.data_len = HAL_TEST_KEY_LEN;
	memset(g_aes_key_in.data, 0xa5, HAL_TEST_KEY_LEN);

	ST_END_TEST;
}

TEST_TEARDOWN(set_key)
{
	ST_START_TEST;
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));
	hal_test_free_buffer(&g_aes_key_in);
	ST_END_TEST;
}

TEST_F(set_key)
{
	ST_START_TEST;
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key_in, NULL));
	ST_END_TEST;
}

/*
 * Desc: Get key
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_aes_key_out;
TEST_SETUP(get_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_aes_key_in, HAL_KEYMGR_TEST_MEM_SIZE));
	g_aes_key_in.data_len = HAL_TEST_KEY_LEN;
	memset(g_aes_key_in.data, 0xa5, HAL_TEST_KEY_LEN);

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_aes_key_out, HAL_KEYMGR_TEST_MEM_SIZE));

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key_in, NULL));

	ST_END_TEST;
}

TEST_TEARDOWN(get_key)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_aes_key_in);
	hal_test_free_buffer(&g_aes_key_out);

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));

	ST_END_TEST;
}

TEST_F(get_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key_out));

	ST_END_TEST;
}

/*
 * Desc: Remove key in Secure Storage
 */
TEST_SETUP(remove_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_aes_key_in, HAL_KEYMGR_TEST_MEM_SIZE));
	g_aes_key_in.data_len = HAL_TEST_KEY_LEN;
	memset(g_aes_key_in.data, 0xa5, HAL_TEST_KEY_LEN);

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->set_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT, &g_aes_key_in, NULL));

	ST_END_TEST;
}

TEST_TEARDOWN(remove_key)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_aes_key_in);

	ST_END_TEST;
}

TEST_F(remove_key)
{
	ST_START_TEST;
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));
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

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));

	ST_END_TEST;
}

TEST_F(generate_key)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_AES_256, HAL_TEST_KEY_SLOT));

	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Set key", set_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Get key", get_key, set_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Remove key", remove_key, get_key);
ST_SET_SMOKE(HAL_KEYMGR_TEST_TRIAL, HAL_KEYMGR_TEST_LIMIT_TIME, "Generate key", generate_key, remove_key);
ST_SET_PACK(hal_keymgr, generate_key);


pthread_addr_t hal_keymgr_test(void)
{
	g_se = se_get_device();

	ST_RUN_TEST(hal_keymgr);
	ST_RESULT_TEST(hal_keymgr);

	return 0;
}
