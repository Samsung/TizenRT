/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_environ.c

/// @brief Test Case Example for Environ API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include "tc_internal.h"

#define ENV_TEST_MAX_NUM 10
#define ENV_TEST_LEN 10

/**
* @fn                   :TC_environ_setenv_getenv_unsetenv
* @brief                :Set, get and unset environment of name value pair.
* @scenario             :Set environment
*                        Get environment
*                        Unset environment
* @API's covered        :setenv, getenv, unsetenv
* @total_passcase       :When setenv function returns zero value and getenv return same value which was set and unsetenv function returns zero value.
* @failcase             :When setenv function returns non zero value or getenv return different value which was set or unsetenv function returns non zero value.
* @Preconditions        :NA
*/
static void tc_environ_setenv_getenv_unsetenv(void)
{
	int ret_chk = ERROR;
	const char *psz_name = "abc";
	const char *psz_value = "xyz";
	const char *pusz_name = "arv";
	char *psz_getvalue = NULL;

	ret_chk = setenv(psz_name, psz_value, 1);
	TC_ASSERT_EQ_CLEANUP("setenv", ret_chk, OK, clearenv());

	psz_getvalue = getenv(NULL);
	TC_ASSERT_EQ("getenv", psz_getvalue, NULL);

	psz_getvalue = getenv(psz_name);
	TC_ASSERT_NEQ_CLEANUP("getenv", psz_getvalue, NULL, clearenv());
	TC_ASSERT_EQ_CLEANUP("getenv", strcmp(psz_getvalue, psz_value), 0, clearenv());

	/* with overwrite_num = 0, psz_value should not be updated */

	psz_value = "pqr";
	ret_chk = setenv(psz_name, psz_value, 0);
	TC_ASSERT_EQ_CLEANUP("setenv", ret_chk, OK, clearenv());

	/* set and get value should not be equal as overwrite is 0 */

	psz_getvalue = getenv(psz_name);
	TC_ASSERT_NEQ_CLEANUP("getenv", psz_getvalue, NULL, clearenv());
	TC_ASSERT_NEQ_CLEANUP("getenv", strcmp(psz_getvalue, psz_value), 0, clearenv());

	/* random value, getenv should fail */

	psz_getvalue = getenv("arv");
	TC_ASSERT_EQ("getenv", psz_getvalue, NULL);

	/* random value, unsetenv should not fail */

	ret_chk = unsetenv(pusz_name);
	TC_ASSERT_EQ("unsetenv", ret_chk, OK);

	/* getvalue should be null */

	psz_getvalue = getenv("arv");
	TC_ASSERT_EQ("getenv", psz_getvalue, NULL);

	ret_chk = setenv(NULL, psz_value, 0);
	TC_ASSERT_EQ("setenv", ret_chk, ERROR);

	ret_chk = setenv(psz_name, NULL, 1);
	TC_ASSERT_EQ("setenv", ret_chk, OK);

	ret_chk = setenv(psz_name, NULL, 0);
	TC_ASSERT_EQ("setenv", ret_chk, OK);

	clearenv();
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :TC_clearenv
* @brief                :Clear environment of name value pair.
* @scenario             :Set environment
*                        Clear environment
* @API's covered        :setenv, clearenv
* @total_passcase       :When setenv function returns zero value and and clearenv function returns zero value.
* @failcase             :When setenv function returns non zero value or clearenv function returns non zero value.
* @Preconditions        :setenv
*/
static void tc_environ_clearenv(void)
{
	int ret_chk = ERROR;
	const char *psz_name = "abc";
	const char *psz_value = "xyz";
	char *psz_getvalue = NULL;
	int overwrite_num = 1;

	ret_chk = setenv(psz_name, psz_value, overwrite_num);
	TC_ASSERT_EQ("setenv", ret_chk, OK);

	ret_chk = clearenv();
	TC_ASSERT_EQ("clearenv", ret_chk, OK);

	psz_getvalue = getenv(psz_name);
	TC_ASSERT_EQ("getenv", psz_getvalue, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :TC_putenv
* @brief                :Add/change environment variable.
* @scenario             :Add environment
*                        Clear environment
* @API's covered        :putenv, clearenv
* @total_passcase       :When putenv function returns zero value.
* @failcase             :When putenv function returns non zero value.
* @Preconditions        :NA
*/
static void tc_environ_putenv(void)
{
	int ret_chk;
	char *psz_getvalue = NULL;

	/* adding enviroment variable */

	ret_chk = putenv("PATH=C:");
	TC_ASSERT_EQ("putenv", ret_chk, OK);

	psz_getvalue = getenv("PATH");
	TC_ASSERT_NEQ_CLEANUP("getenv", psz_getvalue, NULL, clearenv());
	TC_ASSERT_EQ_CLEANUP("getenv", strcmp(psz_getvalue, "C:"), 0, clearenv());

	/* Changing the value of already existing PATH variable */
	ret_chk = putenv("PATH=D:");
	TC_ASSERT_EQ("putenv", ret_chk, OK);

	psz_getvalue = getenv("PATH");
	TC_ASSERT_NEQ_CLEANUP("getenv", psz_getvalue, NULL, clearenv());
	TC_ASSERT_EQ_CLEANUP("getenv", strcmp(psz_getvalue, "D:"), 0, clearenv());

	ret_chk = putenv(NULL);
	TC_ASSERT_EQ("putenv", ret_chk, ERROR);

	clearenv();
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :TC_environ_get_environ_ptr
* @brief                :Add and check the environment variable through ptr
* @scenario             :Add environment variable
*                        Check environment variable whether matched or not.
* @API's covered        :setenv, get_environ_ptr
* @total_passcase       :All put env variables are matched when get_envirion_ptr
* @failcase             :some variables are not matched.
* @Preconditions        :NA
*/
static void tc_environ_get_environ_ptr(void)
{
	char env_arr[ENV_TEST_MAX_NUM][ENV_TEST_LEN * 2];
	size_t env_size;
	size_t cmp_size = 0;
	char *env_ptr;
	int env_idx;
	int ret_chk;
	char env_name[ENV_TEST_LEN];
	char env_val[ENV_TEST_LEN];

	for (env_idx = 0; env_idx < ENV_TEST_MAX_NUM; env_idx++) {
		snprintf(env_name, ENV_TEST_LEN, "%s%d", "ENV", env_idx);
		snprintf(env_val, ENV_TEST_LEN, "%s%d", "VAL", env_idx);
		cmp_size += strlen(env_name) + strlen(env_val) + 2;
		setenv(env_name, env_val, TRUE);
		snprintf(env_arr[env_idx], ENV_TEST_LEN * 2, "%s=%s", env_name, getenv(env_name));
	}

	env_ptr = get_environ_ptr(&env_size);
	TC_ASSERT_NEQ("get_environ_ptr", env_ptr, NULL);

	/* env_size is set to length of all env name and val pair */

	TC_ASSERT_EQ("get_environ_ptr", env_size, cmp_size);

	for (env_idx = 0; env_idx < ENV_TEST_MAX_NUM; env_idx++) {
		TC_ASSERT_EQ("get_environ_ptr", strcmp(env_arr[env_idx], env_ptr), 0);
		env_ptr += strlen(env_ptr) + 1;
	}

	ret_chk = clearenv();
	TC_ASSERT_EQ("clearenv", ret_chk, OK);

	env_ptr = get_environ_ptr(&env_size);
	TC_ASSERT_EQ("get_environ_ptr", env_ptr, NULL);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: environ
 ****************************************************************************/

int environ_main(void)
{
	tc_environ_setenv_getenv_unsetenv();
	tc_environ_clearenv();
	tc_environ_putenv();
	tc_environ_get_environ_ptr();

	return 0;
}
