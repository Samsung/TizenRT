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

#define SECLINK_SS_TEST_TRIAL SECLINK_TEST_TRIAL
#define SECLINK_SS_TEST_LIMIT_TIME SECLINK_TEST_LIMIT
#define SECLINK_SS_TEST_MEM_SIZE SECLINK_TEST_MEM_SIZE

#define SL_TEST_MAX_SLOT_INDEX 3
#define SL_TEST_START_INDEX 0
#define SL_TEST_MAX_DATA 8192
#define SL_SS_TEST_LIMIT_TIME 1000000
#define SL_SS_TEST_TRIAL 1

/*  Each storage has different size */
static sl_ctx g_hnd;
static uint32_t g_size_arr[SL_TEST_MAX_SLOT_INDEX];
static hal_data g_input[SL_TEST_MAX_SLOT_INDEX];
static hal_data g_output[SL_TEST_MAX_SLOT_INDEX];
static unsigned char *g_ss_data[SL_TEST_MAX_SLOT_INDEX] = {NULL,};

/*
 * Desc: Write data in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-securestorage-test_8c-example.html
 */
TEST_SETUP(write_storage)
{
	ST_START_TEST;

	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		g_ss_data[i] = (unsigned char *)malloc(g_size_arr[i]);
		memset(g_ss_data[i], i + 1, g_size_arr[i]);
		g_input[i].data = g_ss_data[i];
		g_input[i].data_len = g_size_arr[i];
	}

	ST_END_TEST;
}

TEST_TEARDOWN(write_storage)
{
	ST_START_TEST;
	hal_result_e hres;
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, i, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
	}

	for (int i = 0; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		free(g_ss_data[i]);
	}

	ST_END_TEST;
}

TEST_F(write_storage)
{
	ST_START_TEST;
	hal_result_e hres;
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, i, &g_input[i], &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
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

	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		g_ss_data[i] = (unsigned char *)malloc(g_size_arr[i]);
		memset(g_ss_data[i], i + 1, g_size_arr[i]);
		g_input[i].data = g_ss_data[i];
		g_input[i].data_len = g_size_arr[i];
	}

	hal_result_e hres;
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, i, &g_input[i], &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
	}

	ST_END_TEST;
}

TEST_TEARDOWN(read_storage)
{
	ST_START_TEST;

	hal_result_e hres;
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, i, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
		sl_test_free_buffer(&g_output[i]);
	}

	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		free(g_ss_data[i]);
	}

	ST_END_TEST;
}

TEST_F(read_storage)
{
	ST_START_TEST;
	hal_result_e hres;
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_output[i], SL_TEST_MAX_DATA));
		ST_EXPECT_EQ(SECLINK_OK, sl_read_storage(g_hnd, i, &g_output[i], &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);

		char message[32] = {0,};
		snprintf(message, 32, "read storage %u", i);
		sl_test_print_buffer(g_output[i].data, g_output[i].data_len, message);
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

	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		g_ss_data[i] = (unsigned char *)malloc(g_size_arr[i]);
		memset(g_ss_data[i], i + 1, g_size_arr[i]);
		g_input[i].data = g_ss_data[i];
		g_input[i].data_len = g_size_arr[i];
	}

	hal_result_e hres;
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, i, &g_input[i], &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
	}

	ST_END_TEST;
}

TEST_TEARDOWN(delete_storage)
{
	ST_START_TEST;

	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		free(g_ss_data[i]);
	}

	ST_END_TEST;
}

TEST_F(delete_storage)
{
	ST_START_TEST;

	hal_result_e hres;
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, i, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
	}

	ST_END_TEST;
}

static void _sl_init_sstest(void)
{
	g_size_arr[0] = SL_TEST_MAX_DATA;
	g_size_arr[1] = 300;
	g_size_arr[2] = 300;

	int res = sl_init(&g_hnd);
	if (res != SECLINK_OK) {
		printf("initialize error\n");
	}
}

static void _sl_deinit_sstest(void)
{
	int res = sl_deinit(g_hnd);
	if (res != SECLINK_OK) {
		printf("deinitialize error\n");
	}
}

void sl_ss_test(void)
{
	_sl_init_sstest();

	ST_SET_PACK(sl_ss);

	ST_SET_SMOKE(sl_ss, SL_SS_TEST_TRIAL, SL_SS_TEST_LIMIT_TIME, "Write data in secure storage", write_storage);
	ST_SET_SMOKE(sl_ss, SL_SS_TEST_TRIAL, SL_SS_TEST_LIMIT_TIME, "Read data from secure storage", read_storage);
	ST_SET_SMOKE(sl_ss, SL_SS_TEST_TRIAL, SL_SS_TEST_LIMIT_TIME, "Delete data of secure storage", delete_storage);

	ST_RUN_TEST(sl_ss);
	ST_RESULT_TEST(sl_ss);

	_sl_deinit_sstest();
}
