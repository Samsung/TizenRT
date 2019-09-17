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
#include <unistd.h>
#include <sys/types.h>
#include "tc_common.h"

#include <binary_manager/binary_manager.h>

#define APP_NAME "micom"

/* The number of User binaries */
#ifdef CONFIG_NUM_APPS
#define USER_BIN_COUNT                   CONFIG_NUM_APPS
#else
#define USER_BIN_COUNT                   2
#endif
#define KERNEL_BIN_COUNT                 1

#define BINARY_COUNT                     (USER_BIN_COUNT + KERNEL_BIN_COUNT)

static void utc_binary_manager_update_binary_p(void)
{
	int ret;

	ret = binary_manager_update_binary(APP_NAME);
	TC_ASSERT_EQ("binary_manager_update_binary", ret, OK);

	/* Wait for APP reload */
	sleep(1);
	
	TC_SUCCESS_RESULT();
}

static void utc_binary_manager_update_binary_n(void)
{
	int ret;

	ret = binary_manager_update_binary(NULL);
	TC_ASSERT_NEQ("binary_manager_update_binary", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_binary_manager_get_update_info_p(void)
{
	int ret;
	binary_update_info_t binary_info;

	memset(&binary_info, 0, sizeof(binary_update_info_t));

	ret = binary_manager_get_update_info(APP_NAME, &binary_info);
	TC_ASSERT_EQ("binary_manager_get_update_info", ret, OK);
	TC_ASSERT_EQ("binary_manager_get_update_info", strncmp(binary_info.name, APP_NAME, BIN_NAME_MAX), OK);

	TC_SUCCESS_RESULT();
}

static void utc_binary_manager_get_update_info_n(void)
{
	int ret;
	binary_update_info_t binary_info;

	memset(&binary_info, 0, sizeof(binary_update_info_t));

	ret = binary_manager_get_update_info(NULL, &binary_info);
	TC_ASSERT_NEQ("binary_manager_get_update_info", ret, OK);
	TC_ASSERT_NEQ("binary_manager_get_update_info", strncmp(binary_info.name, APP_NAME, BIN_NAME_MAX), OK);

	ret = binary_manager_get_update_info(APP_NAME, NULL);
	TC_ASSERT_NEQ("binary_manager_get_update_info", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_binary_manager_get_update_info_all_p(void)
{
	int ret;
	binary_update_info_list_t binary_info_list;

	memset(&binary_info_list, 0, sizeof(binary_info_list));

	ret = binary_manager_get_update_info_all(&binary_info_list);
	TC_ASSERT_EQ("binary_manager_get_update_info_all", ret, OK);
	TC_ASSERT_EQ("binary_manager_get_update_info_all", binary_info_list.bin_count, BINARY_COUNT);

	TC_SUCCESS_RESULT();
}

static void utc_binary_manager_get_update_info_all_n(void)
{
	int ret;

	ret = binary_manager_get_update_info_all(NULL);
	TC_ASSERT_NEQ("binary_manager_get_update_info_all", ret, OK);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_binary_manager_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "Binary Manager UTC") == ERROR) {
		return ERROR;
	}

	utc_binary_manager_update_binary_n();
	utc_binary_manager_update_binary_p();

	utc_binary_manager_get_update_info_n();
	utc_binary_manager_get_update_info_p();

	utc_binary_manager_get_update_info_all_n();
	utc_binary_manager_get_update_info_all_p();

	(void)tc_handler(TC_END, "Binary Manager UTC");

	return 0;
}
