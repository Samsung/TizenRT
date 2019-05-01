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
 * Desc: Write data in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-securestorage-test_8c-example.html
 */
static hal_data g_input;
static hal_data g_output[HAL_TEST_MAX_SLOT_INDEX];
TEST_SETUP(write_storage)
{
	ST_START_TEST;

	unsigned char input[HAL_TEST_MAX_DATA];
	memset(input, 1, HAL_TEST_MAX_DATA);
	g_input.data = input;
	g_input.data_len = sizeof(input);

	ST_END_TEST;
}

TEST_TEARDOWN(write_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->delete_storage(i));
	}

	ST_END_TEST;
}

TEST_F(write_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->write_storage(i, &g_input));
	}

	ST_END_TEST;
}

/*
 * Desc: Read data in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-securestorage-test_8c-example.html
 */
TEST_SETUP(read_storage)
{
	ST_START_TEST;

	unsigned char input[HAL_TEST_MAX_DATA];
	memset(input, 1, HAL_TEST_MAX_DATA);
	g_input.data = input;
	g_input.data_len = HAL_TEST_MAX_DATA;
	for (uint32_t i = 0; i < HAL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->write_storage(i, &g_input));
	}

	ST_END_TEST;
}

TEST_TEARDOWN(read_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->delete_storage(i));
		hal_test_free_buffer(&g_output[i]);
	}

	ST_END_TEST;
}

TEST_F(read_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_output[i], HAL_SS_TEST_MEM_SIZE)); 
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->read_storage(i, &g_output[i]));
	}

	ST_END_TEST;
}

/*
 * Desc: Delete data in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-securestorage-test_8c-example.html
 */
TEST_SETUP(delete_storage)
{
	ST_START_TEST;

	unsigned char input[HAL_TEST_MAX_DATA];
	memset(input, 1, HAL_TEST_MAX_DATA);
	g_input.data = input;
	g_input.data_len = HAL_TEST_MAX_DATA;;
	for (uint32_t i = 0; i < HAL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->write_storage(1, &g_input));
	}

	ST_END_TEST;
}

TEST_TEARDOWN(delete_storage)
{
	ST_START_TEST;

	ST_END_TEST;
}

TEST_F(delete_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->delete_storage(i));
	}

	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(HAL_SS_TEST_TRIAL, HAL_SS_TEST_LIMIT_TIME, "Write data in secure storage", write_storage);
ST_SET_SMOKE(HAL_SS_TEST_TRIAL, HAL_SS_TEST_LIMIT_TIME, "Read data from secure storage", read_storage, write_storage);
ST_SET_SMOKE(HAL_SS_TEST_TRIAL, HAL_SS_TEST_LIMIT_TIME, "Delete data of secure storage", delete_storage, read_storage);
ST_SET_PACK(hal_ss, delete_storage);

pthread_addr_t hal_ss_test(void)
{
	g_se = se_get_device();

	ST_RUN_TEST(hal_ss);
	ST_RESULT_TEST(hal_ss);

	return 0;
}
