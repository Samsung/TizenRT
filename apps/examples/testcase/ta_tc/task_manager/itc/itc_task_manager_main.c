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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "tc_common.h"

#define TM_SAMPLE_NAME "itc_tm_sample"
#define TM_NOPERM_NAME "tm_noperm"
#define TM_SAMPLE_MSG  "test_msg"
#define TM_INVALID_HANDLE      -2
#define TM_UNREGISTERED_HANDLE 99
#define TM_INVALID_TIMEOUT     -2
#define TM_INVALID_PERMISSION  -2
#define TM_INVALID_UPPER_RANGE_PERMISSION 3
#define TM_INVALID_UPPER_RANGE_HANDLE 33

static int tm_sample_handle;
static bool flag;
static task_info_t *sample_info;
static task_info_t *second_sample_info;

static void test_handler(void *info)
{
	flag = !strncmp(info, TM_SAMPLE_MSG, strlen(TM_SAMPLE_MSG));
}

int itc_sample_main(int argc, char *argv[])
{
	int ret;


	ret = task_manager_set_unicast_cb(test_handler);
	if (ret != OK) {
		printf("ERROR : fail to set handler\n");
	}

	while (1) {
		usleep(1);
	}       // This will be dead at utc_task_manager_terminate_p
	return ret;
}

/**
 * @testcase         itc_task_manager_clean_info_p
 * @brief            clean the task manager task info
 * @scenario         Get task_manager_getinfo_with_handle and then clean the task manager task info
 * @apicovered       task_manager_getinfo_with_handle, task_manager_clean_info
 * @precondition     NA
 * @postcondition    NA
 */
static void itc_task_manager_clean_info_p(void)
{
	task_manager_clean_info(&sample_info);
	TC_ASSERT_NEQ("task_manager_clean_info", (task_info_t *)&sample_info, NULL);

	task_manager_clean_info(&sample_info);
	TC_ASSERT_NEQ("task_manager_clean_info", (task_info_t *)&sample_info, NULL);

	task_manager_clean_info(&second_sample_info);
	TC_ASSERT_NEQ("task_manager_clean_info", (task_info_t *)&second_sample_info, NULL);

	TC_SUCCESS_RESULT();
}


/**
 * @testcase         itc_task_manager_register_n
 * @brief            check the api task_manager_register with different invalid parameter
 * @scenario         Call API task_manager_register with all invalid parameter and check with TM_INVALID_PARAM enum
 * @apicovered       task_manager_register
 * @precondition     NA
 * @postcondition    NA
 */

static void itc_task_manager_register_n(void)
{
	int ret;
	ret = task_manager_register(NULL, TM_TASK_PERMISSION_DEDICATE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_register_n", ret, TM_INVALID_PARAM);

	ret = task_manager_register(TM_SAMPLE_NAME, TM_INVALID_PERMISSION, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_register_n", ret, TM_INVALID_PARAM);

	ret = task_manager_register(TM_SAMPLE_NAME, TM_TASK_PERMISSION_GROUP, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_register_n", ret, TM_INVALID_PARAM);

	ret = task_manager_register(TM_SAMPLE_NAME, TM_INVALID_UPPER_RANGE_PERMISSION, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_register_n", ret, TM_INVALID_PARAM);

	ret = task_manager_register(TM_SAMPLE_NAME, TM_TASK_PERMISSION_GROUP, TM_NO_RESPONSE);
	TC_ASSERT_NEQ("task_manager_register_n", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}
/**
 * @testcase         itc_task_manager_register_p
 * @brief            Register a new task which will managed by task manager
 * @scenario         Register a new task with invalid and valid parameter
 * @apicovered       task_manager_register
 * @precondition     NA
 * @postcondition    NA
 */
static void itc_task_manager_register_p(void)
{

	tm_sample_handle = task_manager_register("invalid", TM_TASK_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_register_p", tm_sample_handle, TM_OPERATION_FAIL);

	tm_sample_handle = task_manager_register(TM_SAMPLE_NAME, TM_TASK_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_p", tm_sample_handle, 0);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_task_manager_start_n
 * @brief            Start a task with different parameter managed by task manager
 * @scenario         Start a task with different invalid parameter
 * @apicovered       task_manager_start
 * @precondition     NA
 * @postcondition    NA
 */
static void itc_task_manager_start_n(void)
{
	int ret;

	ret = task_manager_start(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_start", ret, TM_INVALID_PARAM);

	ret = task_manager_start(TM_UNREGISTERED_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_start", ret, TM_INVALID_PARAM);

	ret = task_manager_start(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_start", ret, TM_INVALID_PARAM);

	ret = task_manager_start(TM_INVALID_HANDLE, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_start", ret, TM_INVALID_PARAM);

	ret = task_manager_start(TM_UNREGISTERED_HANDLE, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_start", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}
/**
 * @testcase         itc_task_manager_getinfo_with_handle_twice_p
 * @brief            Get task_manager_getinfo_with_handle with valid parameter twice
 * @scenario         Get task_manager_set_handler with valid parameter twice
 * @apicovered       task_manager_getinfo_with_handle
 * @precondition     NA
 * @postcondition    NA
 */
static void itc_task_manager_getinfo_with_handle_twice_p(void)
{
	sample_info = (task_info_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle_p", sample_info, NULL);

	second_sample_info = (task_info_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle_p", second_sample_info, NULL);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle_p", sample_info, second_sample_info);

	TC_SUCCESS_RESULT();
}
/**
 * @testcase         itc_task_manager_getinfo_with_name_n
 * @brief            Get task info with name with invalid parameter
 * @scenario         Get task info with name  with invalid parameter
 * @apicovered       task_manager_getinfo_with_name
 * @precondition     NA
 * @postcondition    NA
 */
static void itc_task_manager_getinfo_with_name_n(void)
{
	task_info_list_t *ret;
	ret = (task_info_list_t *)task_manager_getinfo_with_name(NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_name_n", ret, NULL);

	ret = (task_info_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_getinfo_with_name_n", ret, NULL);

	ret = (task_info_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_name_n", ret, NULL);

	ret = (task_info_list_t *)task_manager_getinfo_with_name(NULL, -1);
	TC_ASSERT_EQ("task_manager_getinfo_with_name_n", ret, NULL);

	ret = (task_info_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, -1);
	TC_ASSERT_NEQ("task_manager_getinfo_with_name_n", ret, NULL);

	TC_SUCCESS_RESULT();
}



/**
 * @testcase         itc_task_manager_unregister_n
 * @brief            Check task_manager_unregister api with different parameter
 * @scenario         Check task_manager_unregister api
 * @apicovered       task_manager_unregister
 * @precondition     NA
 * @postcondition    NA
 */

static void itc_task_manager_unregister_n(void)
{
	int ret;

	ret = task_manager_unregister(TM_INVALID_HANDLE, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_INVALID_PARAM);

	ret = task_manager_unregister(TM_UNREGISTERED_HANDLE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_INVALID_PARAM);

	ret = task_manager_unregister(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_INVALID_PARAM);

	ret = task_manager_unregister(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_task_manager_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "TaskManager ITC") == ERROR) {
		return ERROR;
	}

	itc_task_manager_register_n();

	itc_task_manager_register_p();

	itc_task_manager_start_n();

	itc_task_manager_getinfo_with_handle_twice_p();

	itc_task_manager_getinfo_with_name_n();

	itc_task_manager_clean_info_p();

	itc_task_manager_unregister_n();

	(void)tc_handler(TC_END, "TaskManager ITC");

	return 0;
}

