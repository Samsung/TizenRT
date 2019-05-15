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
#include <sys/types.h>

#include <stdio.h>
#include <pthread.h>

#include <security/security_common.h>
#include <security/security_auth.h>
#include <security/security_keymgr.h>
#include <security/security_crypto.h>
#include <security/security_ss.h>

#include "tc_common.h"

security_handle g_hnd;

static void utc_security_init_n(void)
{
	security_error ret = security_init(NULL);

	TC_ASSERT_NEQ("security_init", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

static void utc_security_init_p(void)
{
	security_error ret = security_init(&g_hnd);

	TC_ASSERT_EQ("security_init", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

static void utc_security_free_data_n(void)
{
	security_error ret = security_free_data(NULL);

	TC_ASSERT_NEQ("security_free_data", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

static void utc_security_free_data_p(void)
{
	security_data data;
	data.data = (unsigned char *)malloc(SECURITY_MAX_BUF);
	data.length = SECURITY_MAX_BUF;

	security_error ret = security_free_data(&data);

	TC_ASSERT_EQ("security_free_data", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

static void utc_security_get_status_n(void)
{
	security_error ret = security_get_status(NULL);

	TC_ASSERT_NEQ("security_get_status", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

static void utc_security_get_status_p(void)
{
	int status = -1;
	security_error ret = security_get_status(&status);

	TC_ASSERT_EQ("security_get_status", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

static void utc_security_deinit_n(void)
{
	security_error ret = security_deinit(NULL);

	TC_ASSERT_NEQ("security_deinit", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

static void utc_security_deinit_p(void)
{
	security_error ret = security_deinit(g_hnd);

	TC_ASSERT_EQ("security_deinit", ret, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

void utc_common_main(void)
{
	utc_security_init_n();
	utc_security_init_p();
	utc_security_free_data_n();
	utc_security_free_data_p();
	utc_security_get_status_n();
	utc_security_get_status_p();
	utc_security_deinit_n();
	utc_security_deinit_p();
}
