#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <security/hal/security_hal.h>
#include <stress_tool/st_perf.h>
#include "hal_test_utils.h"

/*  Configuration */
#define HAL_SS_TEST_TRIAL 10
#define HAL_SS_TEST_LIMIT_TIME 100000000

#define HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX 8
#define HAL_TEST_SECURE_STORAGE_MAX_DATA 64

/*
 * Desc: Write data in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-securestorage-test_8c-example.html
 */
static hal_data g_input;
static hal_data g_output[HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX];
TEST_SETUP(write_storage)
{
	ST_START_TEST;

	unsigned char input[HAL_TEST_SECURE_STORAGE_MAX_DATA];
	memset(input, 1, HAL_TEST_SECURE_STORAGE_MAX_DATA);
	g_input.data = input;
	g_input.data_len = sizeof(input);

	ST_END_TEST;
}

TEST_TEARDOWN(write_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_2(HAL_SUCCESS, HAL_NOT_SUPPORTED, hal_delete_storage(i));
	}

	ST_END_TEST;
}

TEST_F(write_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_2(HAL_SUCCESS, HAL_NOT_SUPPORTED, hal_write_storage(i, &g_input));
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

	unsigned char input[HAL_TEST_SECURE_STORAGE_MAX_DATA];
	memset(input, 1, HAL_TEST_SECURE_STORAGE_MAX_DATA);
	g_input.data = input;
	g_input.data_len = HAL_TEST_SECURE_STORAGE_MAX_DATA;
	for (uint32_t i = 0; i < HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_2(HAL_SUCCESS, HAL_NOT_SUPPORTED, hal_write_storage(i, &g_input));
	}

	ST_END_TEST;
}

TEST_TEARDOWN(read_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_2(HAL_SUCCESS, HAL_NOT_SUPPORTED, hal_delete_storage(i));
		hal_free_data(&g_output[i]);
	}

	ST_END_TEST;
}

TEST_F(read_storage)
{
	ST_START_TEST;

	for (uint32_t i = 0; i < HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_2(HAL_SUCCESS, HAL_NOT_SUPPORTED, hal_read_storage(i, &g_output[i]));
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

	unsigned char input[HAL_TEST_SECURE_STORAGE_MAX_DATA];
	memset(input, 1, HAL_TEST_SECURE_STORAGE_MAX_DATA);
	g_input.data = input;
	g_input.data_len = HAL_TEST_SECURE_STORAGE_MAX_DATA;;
	for (uint32_t i = 0; i < HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_2(HAL_SUCCESS, HAL_NOT_SUPPORTED, hal_write_storage(1, &g_input));
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

	for (uint32_t i = 0; i < HAL_TEST_SECURE_STORAGE_MAX_SLOT_INDEX; i++) {
		ST_EXPECT_2(HAL_SUCCESS, HAL_NOT_SUPPORTED, hal_delete_storage(i));
	}

	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(HAL_SS_TEST_TRIAL, HAL_SS_TEST_LIMIT_TIME, "Write data in secure storage", write_storage);
ST_SET_SMOKE(HAL_SS_TEST_TRIAL, HAL_SS_TEST_LIMIT_TIME, "Read data from secure storage", read_storage, write_storage);
ST_SET_SMOKE(HAL_SS_TEST_TRIAL, HAL_SS_TEST_LIMIT_TIME, "Delete data of secure storage", delete_storage, read_storage);
ST_SET_PACK(hal_ss, delete_storage);

int hal_ss_test(void)
{
	ST_RUN_TEST(hal_ss);
	ST_RESULT_TEST(hal_ss);

	return 0;
}
