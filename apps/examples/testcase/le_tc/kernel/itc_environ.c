/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file itc_environ.c
/// @brief Scenario Test Case for Environ API

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

#define OVERWRITE 1
#define VAR "var"
#define VALUE "value"
#define VAR_COUNT 5
#define BUFF_LEN 20

/**
* @fn                   :itc_environ_env_dup_env_findvar_p
* @brief                :Copies the environment structure of a task, and finds the address of a environment variable by name
* @scenario             :Find environment variable before and after duplicating
* API's covered         :env_dup, env_findvar, setenv, env_release, clearenv
* Preconditions         :none
* Postconditions        :none
*/
static void itc_environ_env_dup_env_findvar_p(void)
{
	struct task_group_s group;
	int i;
	char var_buf[BUFF_LEN];
	char value_buf[BUFF_LEN];
	char *result;

	clearenv();
	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		result = (char *)env_findvar(&group, var_buf);
		TC_ASSERT_EQ("env_findvar", result, NULL);
	}

	TC_ASSERT_EQ("env_dup", env_dup(&group), OK);
	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		result = (char *)env_findvar(&group, var_buf);
		TC_ASSERT_EQ("env_findvar", result, NULL);
	}

	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		snprintf(value_buf, BUFF_LEN, "%s%d", VALUE, i);
		TC_ASSERT_EQ_CLEANUP("setenv", setenv(var_buf, value_buf, OVERWRITE), OK, clearenv());
	}

	TC_ASSERT_EQ_CLEANUP("env_dup", env_dup(&group), OK, clearenv());
	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		result = (char *)env_findvar(&group, var_buf);
		TC_ASSERT_NEQ_CLEANUP("env_findvar", result, NULL, env_release(&group); clearenv());
		snprintf(value_buf, BUFF_LEN, "%s%d=%s%d", VAR, i, VALUE, i);
		TC_ASSERT_EQ_CLEANUP("env_findvar", strcmp(result, value_buf), 0, env_release(&group); clearenv());
	}

	env_release(&group);
	clearenv();
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_environ_env_release_p
* @brief                :Deletes all environment variables.
* @scenario             :Find the environment variable after releasing
* API's covered         :env_release, env_dup, env_findvar, setenv, clearenv
* Preconditions         :none
* Postconditions        :none
*/
static void itc_environ_env_release_p(void)
{
	struct task_group_s group;
	int i;
	char var_buf[BUFF_LEN];
	char value_buf[BUFF_LEN];
	char *result;

	clearenv();

	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		snprintf(value_buf, BUFF_LEN, "%s%d", VALUE, i);
		TC_ASSERT_EQ_CLEANUP("setenv", setenv(var_buf, value_buf, OVERWRITE), OK, clearenv());
	}

	TC_ASSERT_EQ_CLEANUP("env_dup", env_dup(&group), OK, clearenv());
	env_release(&group);
	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		result = (char *)env_findvar(&group, var_buf);
		TC_ASSERT_EQ_CLEANUP("env_findvar", result, NULL, clearenv());
	}

	clearenv();
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_environ_env_removevar_p
* @brief                :Removes a variable
* @scenario             :Remove a variable and try to find that variable
* API's covered         :env_removevar, env_findvar, env_dup, setenv, env_release, clearenv
* Preconditions         :none
* Postconditions        :none
*/
static void itc_environ_env_removevar_p(void)
{
	struct task_group_s group;
	int i;
	char var_buf[BUFF_LEN];
	char value_buf[BUFF_LEN];
	char *result;

	clearenv();

	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		snprintf(value_buf, BUFF_LEN, "%s%d", VALUE, i);
		TC_ASSERT_EQ_CLEANUP("setenv", setenv(var_buf, value_buf, OVERWRITE), OK, clearenv());
	}

	TC_ASSERT_EQ_CLEANUP("env_dup", env_dup(&group), OK, clearenv());
	for (i = 0; i <= VAR_COUNT / 2; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		TC_ASSERT_EQ_CLEANUP("env_removevar", env_removevar(&group, env_findvar(&group, var_buf)), OK, env_release(&group); clearenv());
	}
	for (i = 0; i <= VAR_COUNT / 2; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		result = (char *)env_findvar(&group, var_buf);
		TC_ASSERT_EQ_CLEANUP("env_findvar", result, NULL, env_release(&group); clearenv());
	}
	for (i = VAR_COUNT / 2 + 1; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		result = (char *)env_findvar(&group, var_buf);
		TC_ASSERT_NEQ_CLEANUP("env_findvar", result, NULL, env_release(&group); clearenv());
		snprintf(value_buf, BUFF_LEN, "%s%d=%s%d", VAR, i, VALUE, i);
		TC_ASSERT_EQ_CLEANUP("env_findvar", strcmp(result, value_buf), 0, env_release(&group); clearenv());
	}

	env_release(&group);
	clearenv();
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_environ_setenv_p
* @brief                :Stores variable name and its corresponding value
* @scenario             :Strore with value null or empty string to unset the variable
* API's covered         :setenv, getenv, clearenv
* Preconditions         :none
* Postconditions        :none
*/
static void itc_environ_setenv_p(void)
{
	char *buffer = "";
	char *result;

	TC_ASSERT_EQ("setenv", setenv(VAR, VALUE, OVERWRITE), OK);
	result = getenv(VAR);
	TC_ASSERT_NEQ_CLEANUP("getenv", result, NULL, clearenv());
	TC_ASSERT_EQ_CLEANUP("getenv", strcmp(result, VALUE), 0, clearenv());

	TC_ASSERT_EQ_CLEANUP("setenv", setenv(VAR, buffer, OVERWRITE), OK, clearenv());
	result = getenv(VAR);
	TC_ASSERT_EQ_CLEANUP("getenv", result, NULL, clearenv());

	TC_ASSERT_EQ("setenv", setenv(VAR, VALUE, OVERWRITE), OK);
	result = getenv(VAR);
	TC_ASSERT_NEQ_CLEANUP("getenv", result, NULL, clearenv());
	TC_ASSERT_EQ_CLEANUP("getenv", strcmp(result, VALUE), 0, clearenv());

	TC_ASSERT_EQ_CLEANUP("setenv", setenv(VAR, NULL, OVERWRITE), OK, clearenv());
	result = getenv(VAR);
	TC_ASSERT_EQ_CLEANUP("getenv", result, NULL, clearenv());

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_environ_getenv_n
* @brief                :Get the stored value of a variable by its name
* @scenario             :Get value for an invalid variable
* API's covered         :getenv, setenv, clearenv
* Preconditions         :none
* Postconditions        :none
*/
static void itc_environ_getenv_n(void)
{
	int i;
	char var_buf[BUFF_LEN];
	char value_buf[BUFF_LEN];
	char *result;

	for (i = 0; i < VAR_COUNT; i++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR, i);
		snprintf(value_buf, BUFF_LEN, "%s%d", VALUE, i);
		TC_ASSERT_EQ_CLEANUP("setenv", setenv(var_buf, value_buf, OVERWRITE), OK, clearenv());
	}

	memset(var_buf, 0, BUFF_LEN);
	snprintf(var_buf, BUFF_LEN, "%s%d=%s%d", VAR, 0, VALUE, 0);
	snprintf(var_buf + strlen(var_buf) + 1, BUFF_LEN, "%s%d", VAR, 1);

	result = getenv(var_buf);
	TC_ASSERT_EQ_CLEANUP("getenv", result, NULL, clearenv());

	clearenv();
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: itc_environ_main
 ****************************************************************************/

int itc_environ_main(void)
{
	itc_environ_env_dup_env_findvar_p();
	itc_environ_env_release_p();
	itc_environ_env_removevar_p();
	itc_environ_setenv_p();
	itc_environ_getenv_n();

	return 0;
}
