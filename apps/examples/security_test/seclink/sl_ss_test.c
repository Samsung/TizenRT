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
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include <stress_tool/st_perf.h>
#include "sl_test.h"
#include "sl_test_usage.h"

static char *g_command[] = {
#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) command,
#include "sl_ss_table.h"
};

#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) \
	extern void func(sl_options *opt);
#include "sl_ss_table.h"

static sl_test_func g_func_list[] = {
#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) func,
#include "sl_ss_table.h"
};

typedef enum {
#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) type,
#include "sl_ss_table.h"
	SL_SS_TYPE_MAX,
	SL_SS_TYPE_ERR = -1
} sl_ss_type_e;

#define SL_TEST_START_INDEX 0
#define SL_TEST_MAX_SLOT_INDEX 3
#define SL_TEST_START_INDEX 0
#define SL_TEST_MAX_DATA 8192

/*  Each storage has different size */
static sl_ctx g_hnd;
static uint32_t g_size_arr[SL_TEST_MAX_SLOT_INDEX];
static hal_data g_input[SL_TEST_MAX_SLOT_INDEX];
static hal_data g_output[SL_TEST_MAX_SLOT_INDEX];
static unsigned char *g_ss_data[SL_TEST_MAX_SLOT_INDEX] = {
	NULL,
};

ST_SET_PACK_GLOBAL(sl_ss);

/*
 * Desc: Write data in secure storage
 */
TESTCASE_SETUP(write_storage)
{
	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		g_ss_data[i] = (unsigned char *)malloc(g_size_arr[i]);
		memset(g_ss_data[i], i + 1, g_size_arr[i]);
		g_input[i].data = g_ss_data[i];
		g_input[i].data_len = g_size_arr[i];
	}
}
END_TEST_F

TESTCASE_TEARDOWN(write_storage)
{

	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, i));
	}

	for (int i = 0; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		free(g_ss_data[i]);
	}
}
END_TEST_F

START_TEST_F(write_storage)
{
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, i, &g_input[i]));
	}
}
END_TEST_F

/*
 * Desc: Read data in secure storage
 */
TESTCASE_SETUP(read_storage)
{
	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		g_ss_data[i] = (unsigned char *)malloc(g_size_arr[i]);
		memset(g_ss_data[i], i + 1, g_size_arr[i]);
		g_input[i].data = g_ss_data[i];
		g_input[i].data_len = g_size_arr[i];
	}

	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, i, &g_input[i]));
	}
}
END_TEST_F

TESTCASE_TEARDOWN(read_storage)
{
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, i));

		sl_test_free_buffer(&g_output[i]);
	}

	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		free(g_ss_data[i]);
	}
}
END_TEST_F

START_TEST_F(read_storage)
{
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_output[i], SL_TEST_MAX_DATA));
		ST_EXPECT_EQ(SECLINK_OK, sl_read_storage(g_hnd, i, &g_output[i]));

		char message[32] = {
			0,
		};
		snprintf(message, 32, "read storage slot %u", i);
		sl_test_print_buffer(g_output[i].data, g_output[i].data_len, message);
	}
}
END_TEST_F

/*
 * Desc: Delete data in secure storage
 */
TESTCASE_SETUP(delete_storage)
{
	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		g_ss_data[i] = (unsigned char *)malloc(g_size_arr[i]);
		memset(g_ss_data[i], i + 1, g_size_arr[i]);
		g_input[i].data = g_ss_data[i];
		g_input[i].data_len = g_size_arr[i];
	}

	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, i, &g_input[i]));
	}
}
END_TEST_F

TESTCASE_TEARDOWN(delete_storage)
{
	for (int i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		free(g_ss_data[i]);
	}
}
END_TEST_F

START_TEST_F(delete_storage)
{
	for (uint32_t i = SL_TEST_START_INDEX; i < SL_TEST_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, i));
	}
}
END_TEST_F

TESTCASE_SETUP(sl_ss_global)
{
	g_size_arr[0] = SL_TEST_MAX_DATA;
	g_size_arr[1] = 300;
	g_size_arr[2] = 300;
	ST_ASSERT_EQ(SECLINK_OK, sl_init(&g_hnd));
}
END_TESTCASE

TESTCASE_TEARDOWN(sl_ss_global)
{
	ST_ASSERT_EQ(SECLINK_OK, sl_deinit(g_hnd));
}
END_TESTCASE

void sl_handle_ss_write(sl_options *opt)
{
	ST_SET_SMOKE(sl_ss, opt->count, 0, "write storage", write_storage);
}

void sl_handle_ss_read(sl_options *opt)
{
	ST_SET_SMOKE(sl_ss, opt->count, 0, "read storage", read_storage);
}

void sl_handle_ss_delete(sl_options *opt)
{
	ST_SET_SMOKE(sl_ss, opt->count, 0, "delete storage", delete_storage);
}

void sl_handle_ss(sl_options *opt)
{
	ST_TC_SET_GLOBAL(sl_ss, sl_ss_global);

	SL_PARSE_MESSAGE(opt, g_command, sl_ss_type_e,
					 g_func_list, SL_SS_TYPE_MAX, SL_SS_TYPE_ERR);
	ST_RUN_TEST(sl_ss);
	ST_RESULT_TEST(sl_ss);
}
