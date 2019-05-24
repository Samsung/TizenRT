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
#include <security/security_api.h>
#include "tc_common.h"
#include "utc_security.h"

static security_handle g_hnd = NULL;

/**
 * @testcase         utc_ss_read_secure_storage_p
 * @brief            Read Secure Storage
 * @scenario         Read Secure Storage
 * @apicovered       ss_read_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_read_secure_storage_p(void)
{
	security_data data = {NULL, 0};
	security_error res = ss_read_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME, 0, &data);

	TC_ASSERT_EQ("ss_read_secure_storage_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_read_secure_storage_hnd_n
 * @brief            Read Secure Storage
 * @scenario         Read Secure Storage
 * @apicovered       ss_read_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_read_secure_storage_hnd_n(void)
{
	security_data data = {NULL, 0};
	security_error res = ss_read_secure_storage(NULL, UTC_CRYPTO_KEY_NAME, 0, &data);

	TC_ASSERT_EQ("ss_read_secure_storage_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_read_secure_storage_name_n
 * @brief            Read Secure Storage
 * @scenario         Read Secure Storage
 * @apicovered       ss_read_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_read_secure_storage_name_n(void)
{
	security_data data = {NULL, 0};
	security_error res = ss_read_secure_storage(g_hnd, NULL, 0, &data);

	TC_ASSERT_EQ("ss_read_secure_storage_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_read_secure_storage_input_n
 * @brief            Read Secure Storage
 * @scenario         Read Secure Storage
 * @apicovered       ss_read_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_read_secure_storage_input_n(void)
{
	security_error res = ss_read_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME, 0, NULL);

	TC_ASSERT_EQ("ss_read_secure_storage_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_write_secure_storage_p
 * @brief            Write secure storage
 * @scenario         Write secure storage
 * @apicovered       ss_write_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_write_secure_storage_p(void)
{
	US_DEFINE_DATA(ss_data, "SS Data");
	security_error res = ss_write_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME, 0, &ss_data);

	TC_ASSERT_EQ("ss_write_secure_storage_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_write_secure_storage_hnd_n
 * @brief            Write secure storage
 * @scenario         Write secure storage
 * @apicovered       ss_write_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_write_secure_storage_hnd_n_(void)
{
	US_DEFINE_DATA(ss_data, "SS Data");
	security_error res = ss_write_secure_storage(NULL, UTC_CRYPTO_KEY_NAME, 0, &ss_data);

	TC_ASSERT_EQ("ss_write_secure_storage", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_write_secure_storage_name_n
 * @brief            Write secure storage
 * @scenario         Write secure storage
 * @apicovered       ss_write_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_write_secure_storage_name_n(void)
{
	US_DEFINE_DATA(ss_data, "SS Data");
	security_error res = ss_write_secure_storage(g_hnd, NULL, 0, &ss_data);

	TC_ASSERT_EQ("ss_write_secure_storage_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_write_secure_storage_input_n
 * @brief            Write secure storage
 * @scenario         Write secure storage
 * @apicovered       ss_write_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_write_secure_storage_input_n(void)
{
	security_error res = ss_write_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME, 0, NULL);

	TC_ASSERT_EQ("ss_write_secure_storage_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_delete_secure_storage_p
 * @brief            Delete secure storage
 * @scenario         Delete secure storage
 * @apicovered       ss_delete_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_delete_secure_storage_p(void)
{
	US_DEFINE_DATA(ss_data, "SS Data");

	security_error res = ss_write_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME, 0, &ss_data);
	TC_ASSERT_EQ("ss_delete_secure_storage_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();

	res = ss_delete_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME);
	TC_ASSERT_EQ("ss_delete_secure_storage_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_delete_secure_storage_hnd_n
 * @brief            Delete secure storage
 * @scenario         Delete secure storage
 * @apicovered       ss_delete_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_delete_secure_storage_hnd_n(void)
{
	security_error res = ss_delete_secure_storage(NULL, UTC_CRYPTO_KEY_NAME);

	TC_ASSERT_EQ("ss_delete_secure_storage_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_delete_secure_storage_name_n
 * @brief            Delete secure storage
 * @scenario         Delete secure storage
 * @apicovered       ss_delete_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_delete_secure_storage_name_n(void)
{
	security_error res = ss_delete_secure_storage(g_hnd, NULL);

	TC_ASSERT_EQ("ss_delete_secure_storage_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_get_size_secure_storage_p
 * @brief            Get size of secure storage
 * @scenario         Get size of secure storage
 * @apicovered       ss_get_size_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_get_size_secure_storage_p(void)
{
	unsigned int size = 0;
	security_error res = ss_get_size_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME, &size);

	TC_ASSERT_EQ("ss_get_size_secure_storage_p", res, SECURITY_NOT_SUPPORT);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_get_size_secure_storage_hnd_n
 * @brief            Get size of secure storage
 * @scenario         Get size of secure storage
 * @apicovered       ss_get_size_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_get_size_secure_storage_hnd_n(void)
{
	unsigned int size = 0;
	security_error res = ss_get_size_secure_storage(NULL, UTC_CRYPTO_KEY_NAME, &size);

	TC_ASSERT_EQ("ss_get_size_secure_storage_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_get_size_secure_storage_name_n
 * @brief            Get size of secure storage
 * @scenario         Get size of secure storage
 * @apicovered       ss_get_size_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_get_size_secure_storage_name_n(void)
{
	unsigned int size = 0;
	security_error res = ss_get_size_secure_storage(g_hnd, NULL, &size);

	TC_ASSERT_EQ("ss_get_size_secure_storage_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_get_size_secure_storage_input_n
 * @brief            Get size of secure storage
 * @scenario         Get size of secure storage
 * @apicovered       ss_get_size_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_get_size_secure_storage_input_n(void)
{
	security_error res = ss_get_size_secure_storage(g_hnd, UTC_CRYPTO_KEY_NAME, NULL);

	TC_ASSERT_EQ("ss_get_size_secure_storage_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_get_list_secure_storage_hnd_n
 * @brief            Get list of secure storage
 * @scenario         Get list of secure storage
 * @apicovered       ss_get_list_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_get_list_secure_storage_hnd_n(void)
{
	security_storage_list slist;
	security_error res = ss_get_list_secure_storage(NULL, &slist);

	TC_ASSERT_EQ("ss_get_list_secure_storage_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_ss_get_list_secure_storage_input_n
 * @brief            Get list of secure storage
 * @scenario         Get list of secure storage
 * @apicovered       ss_get_list_secure_storage
 * @precondition     none
 * @postcondition    none
 */
static void utc_ss_get_list_secure_storage_input_n(void)
{
	security_error res = ss_get_list_secure_storage(g_hnd, NULL);

	TC_ASSERT_EQ("ss_get_list_secure_storage_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

void utc_ss_main(void)
{
	security_error res = security_init(&g_hnd);
	if (res < 0) {
		US_ERROR;
	}

	utc_ss_read_secure_storage_p();
	utc_ss_read_secure_storage_hnd_n();
	utc_ss_read_secure_storage_name_n();
	utc_ss_read_secure_storage_input_n();

	utc_ss_write_secure_storage_p();
	utc_ss_write_secure_storage_hnd_n_();
	utc_ss_write_secure_storage_name_n();
	utc_ss_write_secure_storage_input_n();

	utc_ss_delete_secure_storage_p();
	utc_ss_delete_secure_storage_hnd_n();
	utc_ss_delete_secure_storage_name_n();

	utc_ss_get_size_secure_storage_p();
	utc_ss_get_size_secure_storage_hnd_n();
	utc_ss_get_size_secure_storage_name_n();
	utc_ss_get_size_secure_storage_input_n();

	utc_ss_get_list_secure_storage_hnd_n();
	utc_ss_get_list_secure_storage_input_n();

	res = security_deinit(g_hnd);
	if (res != SECURITY_OK) {
		US_ERROR;
	}
}
