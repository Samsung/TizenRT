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
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <task_manager/task_manager.h>
#ifdef CONFIG_TASK_MANAGER_USER_SPECIFIC_BROADCAST
#include <task_manager/task_manager_broadcast_list.h>
#endif
#include "tc_common.h"

#define TM_SAMPLE_NAME      "tm_sample"
#define TM_BROADCAST1_NAME  "tm_broadcast1"
#define TM_BROADCAST2_NAME  "tm_broadcast2"
#define TM_BROADCAST3_NAME  "tm_broadcast3"
#define TM_NOPERM_NAME      "tm_noperm"
#define TM_SAMPLE_MSG       "test_msg"
#define TM_SYNC_SEND_MSG    "send_sync"
#define TM_SYNC_RECV_MSG    "recv_sync"
#define TM_INVALID_HANDLE               -2
#define TM_INVALID_TIMEOUT              -2
#define TM_INVALID_PERMISSION           -2
#define TM_INVALID_MSG_MASK             -2
#define TM_INVALID_BROAD_MSG            -2
#define TM_INVALID_PID                  -2
#define TM_BROAD_TASK_NUM               2
#define TM_BROAD_UNDEF_NUM              3
#define TM_BROAD_WIFI_ON_DATA           "BROAD_WIFI_ON"
#define TM_BROAD_WIFI_OFF_DATA          "BROAD_WIFI_OFF"
#define TM_BROAD_UNDEFINED_MSG_DATA     "BROAD_UNDEFINED"
#define TM_BROAD_UNDEFINED_MSG_NOT_USED (TM_BROADCAST_MSG_MAX + 2)
#define EXIT_CB_CHECK_STR               "exit_cb_chk"

static int tm_sample_handle;
static int tm_not_builtin_handle;
static int tm_pthread_handle;
static int tm_broadcast_handle1;
static int tm_broadcast_handle2;
static int tm_broadcast_handle3;
static int tm_broadcast_undefined_msg;
static int tm_noperm_handle;
static bool flag;
static tm_appinfo_t *sample_info;
static tm_appinfo_list_t *group_list_info;
static tm_appinfo_list_t *sample_list_info;
static int broad_wifi_on_cnt;
static int broad_wifi_off_cnt;
static int broad_undefined_cnt;
static int pid_tm_utc;
static int handle_tm_utc;
static int broadcast_data_flag;
static bool cb_flag;

static sem_t tm_broad_sem;

static void sync_test_cb(tm_msg_t *info)
{
	tm_msg_t reply_msg;

	if (strncmp(info->msg, TM_SYNC_SEND_MSG, info->msg_size) == 0) {
		reply_msg.msg = malloc(strlen(TM_SYNC_RECV_MSG) + 1);
		if (reply_msg.msg != NULL) {
			reply_msg.msg_size = strlen(TM_SYNC_RECV_MSG) + 1;
			memcpy(reply_msg.msg, TM_SYNC_RECV_MSG, reply_msg.msg_size);
			task_manager_reply_unicast(&reply_msg);
		}
	}
	if (reply_msg.msg != NULL) {
		free(reply_msg.msg);
	}
}

static int not_builtin_task(int argc, char *argv[])
{
	(void)task_manager_set_unicast_cb(sync_test_cb);

	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_stop_p
	return OK;
}

static void *tm_pthread(void *param)
{
	return NULL;
}

static void test_unicast_handler(tm_msg_t *info)
{
	flag = !strncmp((char *)info->msg, TM_SAMPLE_MSG, strlen(TM_SAMPLE_MSG));
}

static void test_broadcast_handler(tm_msg_t *user_data, tm_msg_t *info)
{
	if (info->msg != NULL) {
		if (strncmp((char *)info->msg, TM_BROAD_WIFI_ON_DATA, info->msg_size) == 0) {
			if (user_data->msg != NULL) {
				broadcast_data_flag = strncmp((char *)user_data->msg, "WIFI_ON", user_data->msg_size);
			}
			sem_wait(&tm_broad_sem);
			broad_wifi_on_cnt++;
			sem_post(&tm_broad_sem);
			(void)task_manager_unset_broadcast_cb(TM_BROADCAST_WIFI_ON, TM_NO_RESPONSE);
		} else if (strncmp((char *)info->msg, TM_BROAD_WIFI_OFF_DATA, info->msg_size) == 0) {
			sem_wait(&tm_broad_sem);
			broad_wifi_off_cnt++;
			sem_post(&tm_broad_sem);
			(void)task_manager_unset_broadcast_cb(TM_BROADCAST_WIFI_OFF, TM_NO_RESPONSE);
		} else if (strncmp((char *)info->msg, TM_BROAD_UNDEFINED_MSG_DATA, info->msg_size) == 0) {
			sem_wait(&tm_broad_sem);
			broad_undefined_cnt++;
			sem_post(&tm_broad_sem);
			(void)task_manager_unset_broadcast_cb(tm_broadcast_undefined_msg, TM_NO_RESPONSE);
		}
	}
}

void exit_handler(void *info)
{
	if (strncmp((char *)info, EXIT_CB_CHECK_STR, strlen(EXIT_CB_CHECK_STR) + 1) != 0) {
		TC_ASSERT("task_manager_set_exit_cb", false);
	}
}

static void test_stop_cb(void *data)
{
	cb_flag = true;
}

int tm_sample_main(int argc, char *argv[])
{
	int ret;

	tm_noperm_handle = task_manager_register_builtin(TM_NOPERM_NAME, TM_APP_PERMISSION_DEDICATE, 100);

	ret = task_manager_set_stop_cb(test_stop_cb, NULL);
	if (ret != OK) {
		cb_flag = false;
	}
	ret = task_manager_set_unicast_cb(test_unicast_handler);
	if (ret != OK) {
		printf("ERROR : fail to set handler\n");
	}

	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_stop_p
	return ret;
}

int tm_broadcast1_main(int argc, char *argv[])
{
	int ret;
	tm_msg_t data;

	data.msg_size = strlen(TM_BROAD_WIFI_ON_DATA) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_WIFI_ON_DATA, data.msg_size);
	ret = task_manager_set_broadcast_cb(TM_BROADCAST_WIFI_ON, test_broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set callback ERR: %d\n", ret);
	}

	data.msg_size = strlen(TM_BROAD_WIFI_OFF_DATA) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_WIFI_OFF_DATA, data.msg_size);
	ret = task_manager_set_broadcast_cb(TM_BROADCAST_WIFI_OFF, test_broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set callback ERR: %d\n", ret);
	}

	data.msg_size = strlen(TM_BROAD_UNDEFINED_MSG_DATA) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_UNDEFINED_MSG_DATA, data.msg_size);
	ret = task_manager_set_broadcast_cb(tm_broadcast_undefined_msg, test_broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set callback ERR: %d\n", ret);
	}
	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_stop_p
	return ret;
}

int tm_broadcast2_main(int argc, char *argv[])
{
	int ret;
	tm_msg_t data;

	data.msg_size = strlen(TM_BROAD_WIFI_ON_DATA) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_WIFI_ON_DATA, data.msg_size);
	ret = task_manager_set_broadcast_cb(TM_BROADCAST_WIFI_ON, test_broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set callback ERR: %d\n", ret);
	}

	data.msg_size = strlen(TM_BROAD_WIFI_OFF_DATA) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_WIFI_OFF_DATA, data.msg_size);
	ret = task_manager_set_broadcast_cb(TM_BROADCAST_WIFI_OFF, test_broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set callback ERR: %d\n", ret);
	}
	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_stop_p
	return ret;
}

int tm_broadcast3_main(int argc, char *argv[])
{
	int ret;
	tm_msg_t data;

	data.msg_size = strlen(TM_BROAD_UNDEFINED_MSG_DATA) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_UNDEFINED_MSG_DATA, data.msg_size);
	ret = task_manager_set_broadcast_cb(tm_broadcast_undefined_msg, test_broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set callback ERR: %d\n", ret);
	}
	while (1) {
		usleep(1);
	}	// This will be dead at utc_task_manager_stop_p
	return ret;
}

static void utc_task_manager_register_n(void)
{
	int ret;
	ret = task_manager_register_builtin(NULL, TM_APP_PERMISSION_DEDICATE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_register", ret, TM_INVALID_PARAM);

	ret = task_manager_register_builtin(TM_SAMPLE_NAME, TM_INVALID_PERMISSION, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_register", ret, TM_INVALID_PARAM);

	ret = task_manager_register_builtin(TM_SAMPLE_NAME, TM_APP_PERMISSION_GROUP, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_register", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_register_p(void)
{
	tm_sample_handle = task_manager_register_builtin("invalid", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_register", tm_sample_handle, TM_OPERATION_FAIL);

	tm_sample_handle = task_manager_register_builtin(TM_SAMPLE_NAME, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register", tm_sample_handle, 0);

	tm_broadcast_handle1 = task_manager_register_builtin(TM_BROADCAST1_NAME, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register", tm_broadcast_handle1, 0);

	tm_broadcast_handle2 = task_manager_register_builtin(TM_BROADCAST2_NAME, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register", tm_broadcast_handle2, 0);

	tm_broadcast_handle3 = task_manager_register_builtin(TM_BROADCAST3_NAME, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
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

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_start_p(void)
{
	int ret;

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, TM_ALREADY_STARTED_APP);

	ret = task_manager_start(tm_broadcast_handle1, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	ret = task_manager_start(tm_broadcast_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	ret = task_manager_start(tm_broadcast_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	ret = task_manager_start(tm_not_builtin_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_unicast_cb_n(void)
{
	int ret;
	ret = task_manager_set_unicast_cb(NULL);
	TC_ASSERT_EQ("task_manager_set_unicast_cb", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_unicast_cb_p(void)
{
	int ret;
	ret = task_manager_set_unicast_cb(test_unicast_handler);
	TC_ASSERT_EQ("task_manager_set_unicast_cb", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_broadcast_cb_n(void)
{
	int ret;
	ret = task_manager_set_broadcast_cb(TM_INVALID_BROAD_MSG, test_broadcast_handler, (void *)NULL);
	TC_ASSERT_EQ("task_manager_set_broadcast_cb", ret, TM_INVALID_PARAM);

	ret = task_manager_set_broadcast_cb(TM_BROADCAST_WIFI_ON, NULL, (void *)NULL);
	TC_ASSERT_EQ("task_manager_set_broadcast_cb", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_broadcast_cb_p(void)
{
	int ret;
	tm_msg_t data;

	ret = task_manager_set_broadcast_cb(TM_BROAD_UNDEFINED_MSG_NOT_USED, test_broadcast_handler, (void *)NULL);
	TC_ASSERT_EQ("task_manager_set_broadcast_cb", ret, TM_UNREGISTERED_MSG);

	data.msg_size = strlen(TM_BROAD_UNDEFINED_MSG_DATA) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_UNDEFINED_MSG_DATA, data.msg_size);
	ret = task_manager_set_broadcast_cb(tm_broadcast_undefined_msg, test_broadcast_handler, &data);
	TC_ASSERT_EQ_CLEANUP("task_manager_set_broadcast_cb", ret, OK, free(data.msg));

	free(data.msg);
	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_exit_cb_n(void)
{
	int ret;
	ret = task_manager_set_exit_cb(NULL, (void *)NULL);
	TC_ASSERT_EQ("task_manager_set_exit_cb", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_exit_cb_p(void)
{
	int ret;
	tm_msg_t data;

	data.msg_size = strlen(EXIT_CB_CHECK_STR) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, EXIT_CB_CHECK_STR, data.msg_size);

	ret = task_manager_set_exit_cb(exit_handler, &data);
	TC_ASSERT_EQ_CLEANUP("task_manager_set_exit_cb", ret, OK, free(data.msg));

	free(data.msg);
	TC_SUCCESS_RESULT();
}

static void utc_task_manager_alloc_broadcast_msg_p(void)
{
	tm_broadcast_undefined_msg = task_manager_alloc_broadcast_msg();
	TC_ASSERT_GEQ("task_manager_alloc_broadcast_msg", tm_broadcast_undefined_msg, 0);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unicast_n(void)
{
	int ret;
	tm_msg_t send_msg;
	send_msg.msg_size = strlen(TM_SAMPLE_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_SAMPLE_MSG, send_msg.msg_size);
	
	ret = task_manager_unicast(tm_sample_handle, &send_msg, NULL, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ_CLEANUP("task_manager_unicast", ret, TM_INVALID_PARAM, free(send_msg.msg));

	ret = task_manager_unicast(TM_INVALID_HANDLE, &send_msg, NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ_CLEANUP("task_manager_unicast", ret, TM_INVALID_PARAM, free(send_msg.msg));

	send_msg.msg_size = 0;
	ret = task_manager_unicast(tm_sample_handle, &send_msg, NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ_CLEANUP("task_manager_unicast", ret, TM_INVALID_PARAM, free(send_msg.msg));

	free(send_msg.msg);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unicast_p(void)
{
	int ret;
	int sleep_cnt = 0;
	tm_msg_t reply_msg;
	tm_msg_t send_msg;

	cb_flag = false;

	send_msg.msg_size = strlen(TM_SAMPLE_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_SAMPLE_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_sample_handle, &send_msg, NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, OK);
	while (1) {
		sleep(1);
		if (flag) {
			break;
		}
		TC_ASSERT_LEQ_CLEANUP("task_manager_unicast", sleep_cnt, 10, free(send_msg.msg));
		sleep_cnt++;
	}

	free(send_msg.msg);

	send_msg.msg_size = strlen(TM_SYNC_SEND_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_SYNC_SEND_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_not_builtin_handle, &send_msg, &reply_msg, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unicast", ret, OK, free(send_msg.msg));
	if (strncmp((char *)reply_msg.msg, (char *)TM_SYNC_RECV_MSG, strlen(TM_SYNC_RECV_MSG)) == 0) {
		cb_flag = true;
	} else {
		cb_flag = false;
	}

	free(send_msg.msg);
	free(reply_msg.msg);
	TC_SUCCESS_RESULT();
}

static void utc_task_manager_broadcast_n(void)
{
	int ret;
	ret = task_manager_broadcast(TM_INVALID_BROAD_MSG, NULL, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_broadcast", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_broadcast_p(void)
{
	int sleep_cnt = 0;
	tm_msg_t user_data;

	sem_init(&tm_broad_sem, 0, 1);
	broad_wifi_on_cnt = 0;
	broad_wifi_off_cnt = 0;
	broad_undefined_cnt = 0;
	broadcast_data_flag = -1;

	user_data.msg_size = strlen("WIFI_ON") + 1;
	user_data.msg = malloc(user_data.msg_size);
	strncpy(user_data.msg, "WIFI_ON", user_data.msg_size);

	(void)task_manager_broadcast(TM_BROADCAST_WIFI_ON, &user_data, TM_NO_RESPONSE);
	while (1) {
		sleep(1);
		if (broad_wifi_on_cnt == TM_BROAD_TASK_NUM) {
			break;
		}
		TC_ASSERT_LEQ_CLEANUP("task_manager_broadcast", sleep_cnt, 10, sem_destroy(&tm_broad_sem));
		sleep_cnt++;
	}
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_wifi_on_cnt, TM_BROAD_TASK_NUM, sem_destroy(&tm_broad_sem); free(user_data.msg));
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_wifi_off_cnt, 0, sem_destroy(&tm_broad_sem); free(user_data.msg));
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_undefined_cnt, 0, sem_destroy(&tm_broad_sem); free(user_data.msg));
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broadcast_data_flag, 0, sem_destroy(&tm_broad_sem); free(user_data.msg));
	free(user_data.msg);

	broad_wifi_on_cnt = 0;
	broad_wifi_off_cnt = 0;
	broad_undefined_cnt = 0;
	sleep_cnt = 0;
	(void)task_manager_broadcast(TM_BROADCAST_WIFI_OFF, NULL, TM_NO_RESPONSE);
	while (1) {
		usleep(500);
		if (broad_wifi_off_cnt == TM_BROAD_TASK_NUM) {
			break;
		}
		TC_ASSERT_LEQ_CLEANUP("task_manager_broadcast", sleep_cnt, 10, sem_destroy(&tm_broad_sem));
		sleep_cnt++;
	}
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_wifi_on_cnt, 0, sem_destroy(&tm_broad_sem));
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_wifi_off_cnt, TM_BROAD_TASK_NUM, sem_destroy(&tm_broad_sem));
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_undefined_cnt, 0, sem_destroy(&tm_broad_sem));

	broad_wifi_on_cnt = 0;
	broad_wifi_off_cnt = 0;
	broad_undefined_cnt = 0;
	sleep_cnt = 0;
	(void)task_manager_broadcast(tm_broadcast_undefined_msg, NULL, TM_NO_RESPONSE);
	while (1) {
		usleep(500);
		if (broad_undefined_cnt == TM_BROAD_UNDEF_NUM) {
			break;
		}
		TC_ASSERT_LEQ_CLEANUP("task_manager_broadcast", sleep_cnt, 10, sem_destroy(&tm_broad_sem));
		sleep_cnt++;
	}
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_wifi_on_cnt, 0, sem_destroy(&tm_broad_sem));
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_wifi_off_cnt, 0, sem_destroy(&tm_broad_sem));
	TC_ASSERT_EQ_CLEANUP("task_manager_broadcast", broad_undefined_cnt, TM_BROAD_UNDEF_NUM, sem_destroy(&tm_broad_sem));

	sem_destroy(&tm_broad_sem);
	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unset_broadcast_cb_n(void)
{
	int ret;

	ret = task_manager_unset_broadcast_cb(TM_INVALID_BROAD_MSG, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unset_broadcast_cb", ret, TM_INVALID_PARAM);

	ret = task_manager_unset_broadcast_cb(TM_BROADCAST_WIFI_ON, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_unset_broadcast_cb", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_unset_broadcast_cb_p(void)
{
	int ret;

	ret = task_manager_unset_broadcast_cb(TM_BROADCAST_WIFI_ON, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unset_broadcast_cb", ret, TM_UNREGISTERED_MSG);

	ret = task_manager_unset_broadcast_cb(tm_broadcast_undefined_msg, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unset_broadcast_cb", ret, TM_UNREGISTERED_MSG);

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
	tm_appinfo_list_t *ret;
	ret = (tm_appinfo_list_t *)task_manager_getinfo_with_name(NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_name", ret, NULL);

	ret = (tm_appinfo_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_getinfo_with_name", ret, NULL);

	ret = (tm_appinfo_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_name", ret, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_name_p(void)
{
	sample_list_info = (tm_appinfo_list_t *)task_manager_getinfo_with_name(TM_SAMPLE_NAME, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_handle_n(void)
{
	tm_appinfo_t *ret;
	ret = (tm_appinfo_t *)task_manager_getinfo_with_handle(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_handle", ret, NULL);

	ret = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_getinfo_with_handle", ret, NULL);

	ret = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_handle", ret, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_handle_p(void)
{
	sample_info = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_group_n(void)
{
	tm_appinfo_list_t *ret;
	ret = (tm_appinfo_list_t *)task_manager_getinfo_with_group(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_group", ret, NULL);

	if (!sample_info) {
		sample_info = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);
	}

	ret = (tm_appinfo_list_t *)task_manager_getinfo_with_group(sample_info->tm_gid, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_getinfo_with_group", ret, NULL);

	ret = (tm_appinfo_list_t *)task_manager_getinfo_with_group(sample_info->tm_gid, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_group", ret, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_group_p(void)
{
	if (!sample_info) {
		sample_info = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);
	}

	group_list_info = (tm_appinfo_list_t *)task_manager_getinfo_with_group(sample_info->tm_gid, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_group", group_list_info, NULL);

	task_manager_clean_infolist(&group_list_info);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_clean_infolist_p(void)
{
	task_manager_clean_infolist(&sample_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&sample_list_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_clean_info_p(void)
{
	task_manager_clean_info(&sample_info);
	TC_ASSERT_NEQ("task_manager_clean_info", (tm_appinfo_t *)&sample_info, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_stop_n(void)
{
	int ret;
	ret = task_manager_stop(tm_sample_handle, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_stop", ret, TM_INVALID_PARAM);

	ret = task_manager_stop(TM_INVALID_HANDLE, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_stop", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_stop_p(void)
{
	int ret;

	sleep(1);
	cb_flag = false;
	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_stop", ret, OK);

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_LEQ("task_manager_stop", ret, 0);

	ret = task_manager_stop(tm_broadcast_handle1, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_stop", ret, OK);

	ret = task_manager_stop(tm_broadcast_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_stop", ret, OK);

	ret = task_manager_stop(tm_broadcast_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_stop", ret, OK);

	ret = task_manager_stop(tm_not_builtin_handle, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_stop", ret, OK);

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

	ret = task_manager_unregister(tm_not_builtin_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_pthread_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_broadcast_handle1, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_broadcast_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	ret = task_manager_unregister(tm_broadcast_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	/* Wait for unregistering */
	sleep(1);
	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, TM_UNREGISTERED_APP);

	ret = task_manager_restart(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_restart", ret, TM_UNREGISTERED_APP);

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, TM_UNREGISTERED_APP);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_register_task_n(void)
{
	int ret;

	ret = task_manager_register_task(NULL, 100, 1024, not_builtin_task, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_register_task", ret, TM_INVALID_PARAM);

	ret = task_manager_register_task("not_builtin", 100, 1024, not_builtin_task, NULL, TM_INVALID_PERMISSION, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_register_task", ret, TM_INVALID_PARAM);

	ret = task_manager_register_task("not_builtin", 100, 1024, not_builtin_task, NULL, TM_APP_PERMISSION_DEDICATE, TM_INVALID_PARAM);
	TC_ASSERT_EQ("task_manager_register_task", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_register_task_p(void)
{
	tm_not_builtin_handle = task_manager_register_task("not_builtin", 100, 1024, not_builtin_task, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_not_builtin_handle, 0);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_register_pthread_n(void)
{
	int ret;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	ret = task_manager_register_pthread(NULL, &attr, tm_pthread, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_register_pthread", ret, TM_INVALID_PARAM);

	ret = task_manager_register_pthread("tm_pthread", &attr, tm_pthread, NULL, TM_INVALID_PERMISSION, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_register_pthread", ret, TM_INVALID_PARAM);

	ret = task_manager_register_pthread("tm_pthread", &attr, tm_pthread, NULL, TM_APP_PERMISSION_DEDICATE, TM_INVALID_PARAM);
	TC_ASSERT_EQ("task_manager_register_pthread", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_register_pthread_p(void)
{
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	tm_pthread_handle = task_manager_register_pthread("tm_pthread", &attr, tm_pthread, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_pthread", tm_pthread_handle, 0);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_dealloc_broadcast_msg_n(void)
{
	int ret;

	ret = task_manager_dealloc_broadcast_msg(TM_INVALID_BROAD_MSG, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_dealloc_broadcast_msg", ret, TM_INVALID_PARAM);

	ret = task_manager_dealloc_broadcast_msg(TM_BROADCAST_WIFI_ON, TM_INVALID_TIMEOUT);
	TC_ASSERT_EQ("task_manager_dealloc_broadcast_msg", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_dealloc_broadcast_msg_p(void)
{
	int ret;

	ret = task_manager_dealloc_broadcast_msg(tm_broadcast_undefined_msg, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_dealloc_broadcast_msg", ret, OK);

	ret = task_manager_dealloc_broadcast_msg(tm_broadcast_undefined_msg, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_dealloc_broadcast_msg", ret, TM_UNREGISTERED_MSG);

	TC_SUCCESS_RESULT();
}

static void utc_task_manager_getinfo_with_pid_n(void)
{
	tm_appinfo_t *ret;

	ret = task_manager_getinfo_with_pid(pid_tm_utc, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_getinfo_with_pid", ret, NULL);

	ret = task_manager_getinfo_with_pid(TM_INVALID_PID, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_getinfo_with_pid", ret, NULL);

	TC_SUCCESS_RESULT();	
}

static void utc_task_manager_getinfo_with_pid_p(void)
{
	tm_appinfo_t *ret;

	ret = task_manager_getinfo_with_pid(pid_tm_utc, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ_CLEANUP("task_manager_getinfo_with_pid", ret, NULL, task_manager_clean_info(&ret));
	TC_ASSERT_EQ_CLEANUP("task_manager_getinfo_with_pid", ret->handle, handle_tm_utc, task_manager_clean_info(&ret));

	(void)task_manager_clean_info(&ret);
	TC_SUCCESS_RESULT();
}

static void utc_task_manager_set_stop_cb_n(void)
{
	int ret;
	ret = task_manager_set_stop_cb(NULL, NULL);
	TC_ASSERT_EQ("task_manager_set_stop_cb", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();	
}

static void utc_task_manager_set_stop_cb_p(void)
{
	TC_ASSERT_EQ("task_manager_set_stop_cb", cb_flag, true);

	TC_SUCCESS_RESULT();	
}

static void utc_task_manager_reply_unicast_n(void)
{
	int ret;
	ret = task_manager_reply_unicast(NULL);
	TC_ASSERT_EQ("task_manager_reply_unicast", ret, TM_INVALID_PARAM);

	TC_SUCCESS_RESULT();	
}

static void utc_task_manager_reply_unicast_p(void)
{
	TC_ASSERT_EQ("task_manager_reply_unicast", cb_flag, true);

	TC_SUCCESS_RESULT();	
}

void tm_utc_main(void)
{
	pid_tm_utc = getpid();
	cb_flag = false;
	
	utc_task_manager_alloc_broadcast_msg_p();

	utc_task_manager_register_n();
	utc_task_manager_register_p();

	utc_task_manager_register_task_n();
	utc_task_manager_register_task_p();

	utc_task_manager_register_pthread_n();
	utc_task_manager_register_pthread_p();

	utc_task_manager_start_n();
	utc_task_manager_start_p();

	utc_task_manager_getinfo_with_pid_n();
	utc_task_manager_getinfo_with_pid_p();

	utc_task_manager_set_unicast_cb_n();
	utc_task_manager_set_unicast_cb_p();

	utc_task_manager_set_broadcast_cb_n();
	utc_task_manager_set_broadcast_cb_p();

	utc_task_manager_set_exit_cb_n();
	utc_task_manager_set_exit_cb_p();

	utc_task_manager_unicast_n();
	utc_task_manager_unicast_p();

	utc_task_manager_reply_unicast_n();
	utc_task_manager_reply_unicast_p();

	utc_task_manager_broadcast_n();
	utc_task_manager_broadcast_p();

	utc_task_manager_pause_n();
	utc_task_manager_pause_p();

	utc_task_manager_resume_n();
	utc_task_manager_resume_p();

	utc_task_manager_unset_broadcast_cb_n();
	utc_task_manager_unset_broadcast_cb_p();

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

	utc_task_manager_stop_n();
	utc_task_manager_stop_p();

	utc_task_manager_unregister_n();
	utc_task_manager_unregister_p();

	utc_task_manager_set_stop_cb_n();
	utc_task_manager_set_stop_cb_p();

	utc_task_manager_dealloc_broadcast_msg_n();
	utc_task_manager_dealloc_broadcast_msg_p();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_task_manager_main(int argc, char *argv[])
#endif
{
	int status;

	if (tc_handler(TC_START, "TaskManager UTC") == ERROR) {
		return ERROR;
	}

	handle_tm_utc = task_manager_register_builtin("tm_utc", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	(void)task_manager_start(handle_tm_utc, TM_NO_RESPONSE);
	sleep(5);	//wait for starting tm_utc

	(void)waitpid(pid_tm_utc, &status, 0);

	(void)task_manager_unregister(handle_tm_utc, TM_NO_RESPONSE);

	(void)tc_handler(TC_END, "TaskManager UTC");

	return 0;
}
