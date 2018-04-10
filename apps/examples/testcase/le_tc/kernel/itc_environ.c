/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#define VAR_STR "var"
#define VALUE_STR "value"
#define BUFF_LEN 20
#define LOOP_SIZE CONFIG_ITC_ENV_LOOP_SIZE

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

	TC_ASSERT_EQ("setenv", setenv(VAR_STR, VALUE_STR, OVERWRITE), OK);
	result = getenv(VAR_STR);
	TC_ASSERT_NEQ_CLEANUP("getenv", result, NULL, clearenv());
	TC_ASSERT_EQ_CLEANUP("getenv", strcmp(result, VALUE_STR), 0, clearenv());

	TC_ASSERT_EQ_CLEANUP("setenv", setenv(VAR_STR, buffer, OVERWRITE), OK, clearenv());
	result = getenv(VAR_STR);
	TC_ASSERT_EQ_CLEANUP("getenv", result, NULL, clearenv());

	TC_ASSERT_EQ_CLEANUP("setenv", setenv(VAR_STR, NULL, OVERWRITE), OK, clearenv());
	result = getenv(VAR_STR);
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
	int exec_index;
	char var_buf[BUFF_LEN];
	char value_buf[BUFF_LEN];
	char *result;

	for (exec_index = 0; exec_index < LOOP_SIZE; exec_index++) {
		snprintf(var_buf, BUFF_LEN, "%s%d", VAR_STR, exec_index);
		snprintf(value_buf, BUFF_LEN, "%s%d", VALUE_STR, exec_index);
		TC_ASSERT_EQ_CLEANUP("setenv", setenv(var_buf, value_buf, OVERWRITE), OK, clearenv());
	}

	memset(var_buf, 0, BUFF_LEN);
	snprintf(var_buf, BUFF_LEN, "%s%d=%s%d", VAR_STR, 0, VALUE_STR, 0);

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
	itc_environ_setenv_p();
	itc_environ_getenv_n();

	return 0;
}
