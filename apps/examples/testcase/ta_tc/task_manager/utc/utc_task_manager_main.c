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
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "tc_common.h"

#define TM_SAMPLE_NAME      "tm_sample"
#define TM_BROADCAST1_NAME  "tm_broadcast1"
#define TM_BROADCAST2_NAME  "tm_broadcast2"
#define TM_BROADCAST3_NAME  "tm_broadcast3"
#define TM_NOPERM_NAME      "tm_noperm"
#define TM_SAMPLE_MSG       "test_msg"
#define TM_INVALID_HANDLE         -2
#define TM_UNREGISTERED_HANDLE    (CONFIG_MAX_TASKS - 2)
#define TM_INVALID_TIMEOUT        -2
#define TM_INVALID_PERMISSION     -2
#define TM_INVALID_MSG_MASK       -2
#define TM_INVALID_BROADCAST_MSG  -2

static int tm_sample_handle;
static int tm_broadcast_handle1;
static int tm_broadcast_handle2;
static int tm_broadcast_handle3;
static int tm_noperm_handle;
static bool flag;
static task_info_t *sample_info;
static task_info_list_t *group_list_info;
static task_info_list_t *sample_list_info;
static int *addr;
static int *addr2;
static int broad_cnt;

static void test_handler(int sig, tm_msg_t *info)
{
	flag = !strncmp(info->si_value.sival_ptr, TM_SAMPLE_MSG, strlen(TM_SAMPLE_MSG));
}

static void test_broadcast_handler(int sig, tm_msg_t *info)
{
	int recv_msg = info->si_value.sival_int;
	if (recv_msg == TM_BROADCAST_WIFI_ON) {
		broad_cnt++;
	} else if (recv_msg == TM_BROADCAST_WIFI_OFF) {
		broad_cnt++;
	} else {
		return;
	}
}

static void test_broadcast_handler1(int sig, tm_msg_t *info)
{
	int recv_msg = info->si_value.sival_int;
	if (recv_msg == TM_BROADCAST_WIFI_ON) {
		broad_cnt++;
	} else if (recv_msg == TM_BROADCAST_WIFI_OFF) {
		broad_cnt++;
	} else {
		return;
	}
}

static void test_broadcast_handler2(int sig, tm_msg_t *info)
{
	int recv_msg = info->si_value.sival_int;
	if (recv_msg == TM_BROADCAST_WIFI_ON) {
		broad_cnt++;
	} else if (recv_msg == TM_BROADCAST_WIFI_OFF) {
		broad_cnt++;
	} else {
		return;
	}
}

void free_handler(void)
{
	free(addr);
	free(addr2);
}

int tm_sample_main(int argc, char *argv[])
{
	int ret;

	tm_noperm_handle = task_manager_register(TM_NOPERM_NAME, TM_TASK_PERMISSION_DEDICATE, 100);

	ret = task_manager_set_handler(test_handler);
	if (ret != OK) {
		printf("ERROR : fail to set handler\n");
	}
	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_terminate_p
}

int tm_broadcast1_main(int argc, char *argv[])
{
	int ret;

	ret = task_manager_set_broadcast_handler((TM_BROADCAST_WIFI_ON | TM_BROADCAST_WIFI_OFF), test_broadcast_handler);
	if (ret != OK) {
		printf("ERROR : fail to set handler\n");
	}
	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_terminate_p
}

int tm_broadcast2_main(int argc, char *argv[])
{
	int ret;

	ret = task_manager_set_broadcast_handler((TM_BROADCAST_WIFI_ON), test_broadcast_handler1);
	if (ret != OK) {
		printf("ERROR : fail to set handler\n");
	}
	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_terminate_p
}

int tm_broadcast3_main(int argc, char *argv[])
{
	int ret;

	ret = task_manager_set_broadcast_handler((TM_BROADCAST_WIFI_OFF), test_broadcast_handler2);
	if (ret != OK) {
		printf("ERROR : fail to set handler\n");
	}
	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_terminate_p
}

static void utc_task_manager_register_n(void)
{
	int ret;
	ret = task_manager_register(NULL, TM_TASK_PERMISSION_DEDICATE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_register", ret, TM_INVALID_PARAM);

	ret = task_manager_register(TM_SAMPLE_NAME, TM_INVALID_PERMISSION, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_register", ret, TM_INVALID_PARAM);

	ret = task_manager_register(TM_SAMPLE_NAME, TM_TASK_PERMISSION_GROUP, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_register", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_register_p(void)
{
	tm_sample_handle = task_manager_register("invalid", TM_TASK_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_register", tm_sample_handle, TM_FAIL_REGISTER);

	tm_sample_handle = task_manager_register(TM_SAMPLE_NAME, TM_TASK_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register", tm_sample_handle, 0);

	tm_broadcast_handle1 = task_manager_register(TM_BROADCAST1_NAME, TM_TASK_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register", tm_broadcast_handle1, 0);

	tm_broadcast_handle2 = task_manager_register(TM_BROADCAST2_NAME, TM_TASK_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register", tm_broadcast_handle2, 0);

	tm_broadcast_handle3 = task_manager_register(TM_BROADCAST3_NAME, TM_TASK_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register", tm_broadcast_handle3, 0);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_start_n(void)
{
	int ret;
	ret = task_manager_start(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_start", ret, TM_INVALID_PARAM);

	ret = task_manager_start(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_start", ret, TM_INVALID_PARAM);

	ret = task_manager_start(TM_UNREGISTERED_HANDLE, 100);
	TC_ASSERT_EQ("task_manager_start", ret, TM_FAIL_UNREGISTERED_TASK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_start_p(void)
{
	int ret;

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, TM_FAIL_ALREADY_STARTED_TASK);

	ret = task_manager_start(tm_broadcast_handle1, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	ret = task_manager_start(tm_broadcast_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	ret = task_manager_start(tm_broadcast_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_handler_n(void)
{
	int ret;
	ret = task_manager_set_handler(NULL);
	TC_ASSERT_EQ("task_manager_set_handler", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_handler_p(void)
{
	int ret;
	ret = task_manager_set_handler(test_handler);
	TC_ASSERT_EQ("task_manager_set_handler", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_broadcast_handler_n(void)
{
	int ret;
	ret = task_manager_set_broadcast_handler(TM_INVALID_MSG_MASK, test_broadcast_handler);
	TC_ASSERT_EQ("task_manager_set_broadcast_handler", ret, TM_INVALID_PARAM);

	ret = task_manager_set_broadcast_handler(TM_BROADCAST_WIFI_ON, NULL);
	TC_ASSERT_EQ("task_manager_set_broadcast_handler", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_broadcast_handler_p(void)
{
	int ret;
	ret = task_manager_set_broadcast_handler(TM_BROADCAST_WIFI_ON, test_broadcast_handler);
	TC_ASSERT_EQ("task_manager_set_broadcast_handler", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_termination_cb_n(void)
{
	int ret;
	ret = task_manager_set_termination_cb(NULL);
	TC_ASSERT_EQ("task_manager_set_termination_cb", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_termination_cb_p(void)
{
	int ret;
	/* No meaningful malloc for testing resource collection handler */
	addr = (int *)malloc(123);
	addr2 = (int *)malloc(456);

	ret = task_manager_set_termination_cb(free_handler);
	TC_ASSERT_EQ("task_manager_set_termination_cb", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unicast_n(void)
{
	int ret;
	ret = task_manager_unicast(tm_sample_handle, TM_SAMPLE_MSG, strlen(TM_SAMPLE_MSG), TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_unicast", ret, TM_INVALID_PARAM);

	ret = task_manager_unicast(TM_INVALID_HANDLE, TM_SAMPLE_MSG, strlen(TM_SAMPLE_MSG), TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, TM_INVALID_PARAM);

	ret = task_manager_unicast(tm_sample_handle, TM_SAMPLE_MSG, 0, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unicast_p(void)
{
	int ret;
	int sleep_cnt = 0;
	ret = task_manager_unicast(tm_sample_handle, TM_SAMPLE_MSG, strlen(TM_SAMPLE_MSG), TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, OK);
	while (1) {
		sleep(1);
		if (flag) {
			break;
		}
		TC_ASSERT_LEQ("task_manager_unicast", sleep_cnt, 10);
		sleep_cnt++;
	}

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_broadcast_n(void)
{
	int ret;
	ret = task_manager_broadcast(TM_INVALID_BROADCAST_MSG);
	TC_ASSERT_EQ("task_manager_broadcast", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_broadcast_p(void)
{
	broad_cnt = 0;
	(void)task_manager_broadcast(TM_BROADCAST_WIFI_ON);
	TC_ASSERT_EQ("task_manager_broadcast", broad_cnt, 2);
	while (1) {
		usleep(1);
		if (broad_cnt == 2) {
			break;
		}
	}

	broad_cnt = 0;
	(void)task_manager_broadcast(TM_BROADCAST_WIFI_OFF);
	TC_ASSERT_EQ("task_manager_broadcast", broad_cnt, 2);
	while (1) {
		usleep(1);
		if (broad_cnt == 2) {
			break;
		}
	}

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_pause_n(void)
{
	int ret;
	ret = task_manager_pause(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_pause", ret, TM_INVALID_PARAM);

	ret = task_manager_pause(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_pause", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_pause_p(void)
{
	int ret;
	ret = task_manager_pause(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_pause", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_resume_n(void)
{
	int ret;
	ret = task_manager_resume(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_resume", ret, TM_INVALID_PARAM);

	ret = task_manager_resume(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_resume", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_resume_p(void)
{
	int ret;
	ret = task_manager_resume(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_resume", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_name_n(void)
{
	task_info_list_t *ret;
	ret = (task_info_list_t *)task_manager_getinfo_with_name(NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_name", ret, NULL);

	ret = (task_info_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_getinfo_with_name", ret, NULL);

	ret = (task_info_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_name", ret, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_name_p(void)
{
	sample_list_info = (task_info_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_handle_n(void)
{
	task_info_t *ret;
	ret = (task_info_t *)task_manager_getinfo_with_handle(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_handle", ret, NULL);

	ret = (task_info_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_getinfo_with_handle", ret, NULL);

	ret = (task_info_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_handle", ret, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_handle_p(void)
{
	sample_info = (task_info_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_group_n(void)
{
	task_info_list_t *ret;
	ret = (task_info_list_t *)task_manager_getinfo_with_group(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_group", ret, NULL);

	if (!sample_info) {
		sample_info = (task_info_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);
	}

	ret = (task_info_list_t *)task_manager_getinfo_with_group(sample_info->tm_gid, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_getinfo_with_group", ret, NULL);

	ret = (task_info_list_t *)task_manager_getinfo_with_group(sample_info->tm_gid, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_group", ret, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_group_p(void)
{
	if (!sample_info) {
		sample_info = (task_info_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);
	}

	group_list_info = (task_info_list_t *)task_manager_getinfo_with_group(sample_info->tm_gid, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_group", group_list_info, NULL);

	task_manager_clean_infolist(&group_list_info);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_clean_infolist_p(void)
{
	task_manager_clean_infolist(&sample_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (task_info_list_t *)&sample_list_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_clean_info_p(void)
{
	task_manager_clean_info(&sample_info);
	TC_ASSERT_NEQ("task_manager_clean_info", (task_info_t *)&sample_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_terminate_n(void)
{
	int ret;
	ret = task_manager_terminate(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_terminate", ret, TM_INVALID_PARAM);

	ret = task_manager_terminate(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_terminate", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_terminate_p(void)
{
	int ret;
	ret = task_manager_terminate(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_terminate", ret, OK);

	ret = task_manager_terminate(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_terminate", ret, TM_FAIL_ALREADY_STOPPED_TASK);

	ret = task_manager_terminate(tm_broadcast_handle1, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_terminate", ret, OK);

	ret = task_manager_terminate(tm_broadcast_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_terminate", ret, OK);

	ret = task_manager_terminate(tm_broadcast_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_terminate", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_restart_n(void)
{
	int ret;
	ret = task_manager_restart(TM_INVALID_HANDLE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_restart", ret, TM_INVALID_PARAM);

	ret = task_manager_restart(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_restart", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_restart_p(void)
{
	int ret;
	ret = task_manager_restart(TM_UNREGISTERED_HANDLE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_restart", ret, TM_FAIL_UNREGISTERED_TASK);

	ret = task_manager_restart(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_restart", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unregister_n(void)
{
	int ret;
	ret = task_manager_unregister(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_INVALID_PARAM);

	ret = task_manager_unregister(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unregister_p(void)
{
	int ret;
	ret = task_manager_unregister(TM_UNREGISTERED_HANDLE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_FAIL_UNREGISTERED_TASK);

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_broadcast_handle1, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_broadcast_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_broadcast_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_task_manager_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "TaskManager UTC") == ERROR) {
		return ERROR;
	}

	utc_task_manager_register_n();
	utc_task_manager_register_p();

	utc_task_manager_start_n();
	utc_task_manager_start_p();

	utc_task_manager_set_handler_n();
	utc_task_manager_set_handler_p();

	utc_task_manager_set_broadcast_handler_n();
	utc_task_manager_set_broadcast_handler_p();

	utc_task_manager_set_termination_cb_n();
	utc_task_manager_set_termination_cb_p();

	utc_task_manager_unicast_n();
	utc_task_manager_unicast_p();

	utc_task_manager_broadcast_n();
	utc_task_manager_broadcast_p();

	utc_task_manager_pause_n();
	utc_task_manager_pause_p();

	utc_task_manager_resume_n();
	utc_task_manager_resume_p();

	utc_task_manager_getinfo_with_name_n();
	utc_task_manager_getinfo_with_name_p();

	utc_task_manager_getinfo_with_handle_n();
	utc_task_manager_getinfo_with_handle_p();

	utc_task_manager_getinfo_with_group_n();
	utc_task_manager_getinfo_with_group_p();

	utc_task_manager_clean_infolist_p();
	utc_task_manager_clean_info_p();

	utc_task_manager_restart_n();
	utc_task_manager_restart_p();

	utc_task_manager_terminate_n();
	utc_task_manager_terminate_p();

	utc_task_manager_unregister_n();
	utc_task_manager_unregister_p();

	(void)tc_handler(TC_END, "TaskManager UTC");

	return 0;
}
