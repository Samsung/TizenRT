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

#define TM_START_MSG       "start"
#define TM_STOP_MSG       "stop"
#define TM_SYNC_SEND_MSG    "send_sync"
#define TM_SYNC_RECV_MSG    "recv_sync"
#define TM_BROAD_START           "BROAD_START"
#define TM_BROAD_END           "BROAD_END"
#define EXIT_CB_CHECK_STR               "exit_cb_chk"

static int tm_sample_handle;
static int pid_tm_itc;
static int pid_task6;
static int handle_tm_itc;
static int tm_broadcast_msg;
static bool unregister_task1_flag;
static bool unregister_task5_flag;
static bool broadcast_data_flag;

static int not_builtin_task_1(int argc, char *argv[])
{

	while (1) {
		usleep(1);
	}
	return OK;
}

static int not_builtin_task(int argc, char *argv[])
{
	pid_task6 = getpid();
	while (1) {
		usleep(1);
	}
	return OK;
}

void unregister_tasks(void)
{
	int ret;

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	int tm_sample_handle2;

	tm_appinfo_list_t *sample_list_info;

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("itc_task_manager_user1", TM_RESPONSE_WAIT_INF));
	TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&sample_list_info, NULL);

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

}

void unregister_tasks_3(int tm_sample_handle2)
{
	int ret;

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);
}

void unregister_tasks_4(int gid)
{
	tm_appinfo_list_t *group_list_info;
	tm_appinfo_t *task_info;
	int ret;

	group_list_info = (tm_appinfo_list_t *)task_manager_getinfo_with_group(gid, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_group", group_list_info, NULL);

	while (group_list_info != NULL) {
		task_info = &(group_list_info->task);
		ret = task_manager_unregister(task_info->handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ("task_manager_unregister", ret, OK);

		task_manager_clean_info(&task_info);
		TC_ASSERT_NEQ("task_manager_clean_info", (tm_appinfo_t *)&task_info, NULL);

		group_list_info = group_list_info->next;
	}

	task_manager_clean_infolist(&group_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&group_list_info, NULL);
}

void itc_task_manager_check_dedicate_permission_p(void)
{
	int tm_sample_handle2;

	tm_sample_handle = task_manager_register_builtin("tm_sample", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_builtin", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_builtin("itc_task_manager_user1", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_builtin", tm_sample_handle2, 0);

	int ret;
	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks());

	usleep(100);

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, unregister_tasks_3(tm_sample_handle2));

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}

void itc_task_manager_user1_main(void)
{
	int ret;

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, TM_NO_PERMISSION);

}

void cleanup_task1(void)
{
	unregister_task1_flag = true;
}

void unregister_tasks_1(void)
{
	int ret;

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	int tm_sample_handle2;

	tm_appinfo_list_t *sample_list_info;

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task2", TM_RESPONSE_WAIT_INF));
	TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&sample_list_info, NULL);

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

}

void itc_task_manager_pause_task1(void)
{
	int ret;

	ret = task_manager_pause(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_pause", ret, OK, cleanup_task1());

	ret = task_manager_resume(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_resume", ret, OK, cleanup_task1());

	ret = task_manager_restart(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_restart", ret, OK, cleanup_task1());

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, cleanup_task1());

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, cleanup_task1());

}

int itc_task_manager_pause_task1_main(int argc, char *argv[])
{
	itc_task_manager_pause_task1();
	return 0;
}

void itc_task_manager_check_group_permission_p(void)
{
	int tm_sample_handle2;

	tm_sample_handle = task_manager_register_task("task1", 100, 1024, not_builtin_task_1, NULL, TM_APP_PERMISSION_GROUP, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_task("task2", 100, 1024, itc_task_manager_pause_task1_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	int ret;

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_1());

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_1());

	unregister_task1_flag = false;
	usleep(100);

	if (unregister_task1_flag) {
		ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, unregister_tasks_3(tm_sample_handle2));
	}

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}

void cleanup_task5(void)
{
	unregister_task5_flag = true;
}

void itc_task_manager_task3(void)
{
	int ret;
	int tm_sample_handle4;
	tm_appinfo_list_t *sample_list_info;

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task5", TM_RESPONSE_WAIT_INF));
	TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

	tm_sample_handle4 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&sample_list_info, NULL);

	ret = task_manager_pause(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_pause", ret, OK, cleanup_task5());

	ret = task_manager_resume(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_resume", ret, OK, cleanup_task5());

	ret = task_manager_restart(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_restart", ret, OK, cleanup_task5());

	ret = task_manager_stop(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, cleanup_task5());

	ret = task_manager_unregister(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, cleanup_task5());

}

int itc_task_manager_task3_main(int argc, char *argv[])
{
	itc_task_manager_task3();
	return 0;
}

void itc_task_manager_task4(void)
{
	int tm_sample_handle4;

	tm_sample_handle4 = task_manager_register_task("task5", 100, 1024, not_builtin_task_1, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle4, 0);

	unregister_task5_flag = false;
	int ret;

	ret = task_manager_start(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_start", ret, OK);

	while (1) {
		usleep(1);
		if (unregister_task5_flag) {
			ret = task_manager_unregister(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
			TC_ASSERT_EQ("task_manager_unregister", ret, OK);
		}
	}
}

int itc_task_manager_task4_main(int argc, char *argv[])
{
	itc_task_manager_task4();
	return 0;
}

void unregister_tasks_2(void)
{
	int ret;

	int tm_sample_handle2;

	tm_appinfo_list_t *sample_list_info;
	if (unregister_task5_flag) {
		sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task5", TM_RESPONSE_WAIT_INF));
		TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

		tm_sample_handle2 = sample_list_info->task.handle;
		task_manager_clean_infolist(&sample_list_info);
		TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&sample_list_info, NULL);

		ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ("task_manager_unregister", ret, OK);
	}

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task4", TM_RESPONSE_WAIT_INF));
	TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&sample_list_info, NULL);

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task3", TM_RESPONSE_WAIT_INF));
	TC_ASSERT_NEQ("task_manager_getinfo_with_name", sample_list_info, NULL);

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&sample_list_info, NULL);

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

}

void itc_task_manager_check_all_permission_p(void)
{
	int ret;
	int tm_sample_handle3;
	int tm_sample_handle2;

	tm_sample_handle3 = task_manager_register_task("task4", 100, 1024, itc_task_manager_task4_main, NULL, TM_APP_PERMISSION_GROUP, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle3, 0);

	tm_sample_handle2 = task_manager_register_task("task3", 100, 1024, itc_task_manager_task3_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	ret = task_manager_start(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_2());

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_2());

	usleep(100);

	ret = task_manager_stop(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_2());

	ret = task_manager_unregister(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, unregister_tasks_3(tm_sample_handle2));

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}

void itc_unicast_handler_2(tm_msg_t *info)
{
	tm_msg_t reply_msg;
	if (!strncmp((char *)info->msg, TM_SYNC_SEND_MSG, strlen(TM_SYNC_SEND_MSG))) {
		reply_msg.msg = malloc(strlen(TM_SYNC_RECV_MSG) + 1);
		if (reply_msg.msg != NULL) {
			reply_msg.msg_size = strlen(TM_SYNC_RECV_MSG) + 1;
			memcpy(reply_msg.msg, TM_SYNC_RECV_MSG, reply_msg.msg_size);
			task_manager_reply_unicast(&reply_msg);
		}
	}
}

void itc_unicast_handler(tm_msg_t *info)
{
	int tm_sample_handle2;
	int ret;

	if (!strncmp((char *)info->msg, TM_START_MSG, strlen(TM_START_MSG))) {

		tm_sample_handle2 = task_manager_register_task("task6", 100, 1024, not_builtin_task, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

		ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_3(tm_sample_handle2));

		ret = task_manager_pause(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ_CLEANUP("task_manager_pause", ret, OK, unregister_tasks_3(tm_sample_handle2));

		ret = task_manager_resume(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ_CLEANUP("task_manager_resume", ret, OK, unregister_tasks_3(tm_sample_handle2));

		ret = task_manager_restart(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ_CLEANUP("task_manager_restart", ret, OK, unregister_tasks_3(tm_sample_handle2));

	} else if (!strncmp((char *)info->msg, TM_STOP_MSG, strlen(TM_STOP_MSG))) {
		tm_appinfo_t *task_info;

		task_info = ((tm_appinfo_t *)task_manager_getinfo_with_pid(pid_task6, TM_RESPONSE_WAIT_INF));
		TC_ASSERT_NEQ("task_manager_getinfo_with_name", task_info, NULL);

		tm_sample_handle2 = task_info->handle;
		task_manager_clean_info(&task_info);
		TC_ASSERT_NEQ("task_manager_clean_infolist", (tm_appinfo_list_t *)&task_info, NULL);

		ret = task_manager_stop(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_3(tm_sample_handle2));

		ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, unregister_tasks_3(tm_sample_handle2));

		ret = task_manager_set_unicast_cb(itc_unicast_handler_2);
		TC_ASSERT_EQ("task_manager_set_unicast_cb", ret, OK);
	}
}

void *itc_task_manager_pthread(void *param)
{
	int ret;

	ret = task_manager_set_unicast_cb(itc_unicast_handler);
	if (ret != OK) {
		printf("ERROR : fail to set unicast callback handler\n");
	}
	while (1) {
		usleep(1);
	}
}

void itc_task_manager_unicast_thrice_p(void)
{
	int tm_pthread_handle;
	int ret;

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	tm_pthread_handle = task_manager_register_pthread("tm_pthread", &attr, itc_task_manager_pthread, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_pthread", tm_pthread_handle, 0);

	ret = task_manager_start(tm_pthread_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_3(tm_pthread_handle));

	usleep(100);

	tm_msg_t send_msg;
	tm_msg_t reply_msg;

	send_msg.msg_size = strlen(TM_START_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_START_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_pthread_handle, &send_msg, NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, OK);
	usleep(100);

	send_msg.msg_size = strlen(TM_STOP_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_STOP_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_pthread_handle, &send_msg, NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, OK);

	usleep(100);

	send_msg.msg_size = strlen(TM_SYNC_SEND_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_SYNC_SEND_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_pthread_handle, &send_msg, &reply_msg, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unicast", ret, OK, free(send_msg.msg));

	if (strncmp((char *)reply_msg.msg, (char *)TM_SYNC_RECV_MSG, strlen(TM_SYNC_RECV_MSG)) != 0) {
		printf("Callback failed");
	}
	ret = task_manager_stop(tm_pthread_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_3(tm_pthread_handle));

	ret = task_manager_unregister(tm_pthread_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}


void itc_broadcast_handler(tm_msg_t *user_data, tm_msg_t *info)
{
	int ret;

	if (strncmp((char *)info->msg, TM_BROAD_START, strlen(TM_BROAD_START) + 1) == 0) {

		ret = task_manager_unset_broadcast_cb(tm_broadcast_msg, TM_NO_RESPONSE);
		if (ret != OK) {
			printf("ERROR : fail to unset broadcast callback ERR: %d\n", ret);
		}

		broadcast_data_flag = true;
		tm_msg_t data;

		data.msg_size = strlen(TM_BROAD_END) + 1;
		data.msg = malloc(data.msg_size);
		memcpy(data.msg, TM_BROAD_END, data.msg_size);
		ret = task_manager_set_broadcast_cb(tm_broadcast_msg, itc_broadcast_handler, &data);
		free(data.msg);
		if (ret != OK) {
			printf("ERROR : fail to set broadcast callback ERR: %d\n", ret);
		}
	} else if (strncmp((char *)info->msg, TM_BROAD_END, strlen(TM_BROAD_END) + 1) == 0) {
		broadcast_data_flag = false;
		ret = task_manager_unset_broadcast_cb(tm_broadcast_msg, TM_NO_RESPONSE);
		if (ret != OK) {
			printf("ERROR : fail to unset broadcast callback ERR: %d\n", ret);
		}
	}
}

void itc_task_manager_task7(void)
{
	int ret;
	tm_msg_t data;

	data.msg_size = strlen(TM_BROAD_START) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_START, data.msg_size);
	ret = task_manager_set_broadcast_cb(tm_broadcast_msg, itc_broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set broadcast callback ERR: %d\n", ret);
	}
	while (1) {
		usleep(1);
	}
}

int itc_task_manager_task7_main(int argc, char *argv[])
{
	itc_task_manager_task7();
	return 0;
}

void itc_task_manager_broadcast_twice_p(void)
{
	int ret;

	tm_broadcast_msg = task_manager_alloc_broadcast_msg();
	TC_ASSERT_GEQ("task_manager_alloc_broadcast_msg", tm_broadcast_msg, 0);

	tm_sample_handle = task_manager_register_task("task7", 100, 1024, itc_task_manager_task7_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_3(tm_sample_handle));

	tm_msg_t user_data;

	user_data.msg_size = strlen("START") + 1;
	user_data.msg = malloc(user_data.msg_size);
	strncpy(user_data.msg, "START", user_data.msg_size);

	int sleep_cnt = 0;

	usleep(100);

	(void)task_manager_broadcast(tm_broadcast_msg, &user_data, TM_NO_RESPONSE);
	while (1) {
		sleep(1);
		if (broadcast_data_flag) {
			break;
		}
		TC_ASSERT_LEQ_CLEANUP("task_manager_broadcast", sleep_cnt, 1, unregister_tasks_3(tm_sample_handle));
		sleep_cnt++;
	}
	user_data.msg_size = strlen("END") + 1;
	user_data.msg = malloc(user_data.msg_size);
	strncpy(user_data.msg, "END", user_data.msg_size);

	sleep_cnt = 0;

	usleep(100);

	(void)task_manager_broadcast(tm_broadcast_msg, &user_data, TM_NO_RESPONSE);
	while (1) {
		sleep(1);
		if (!broadcast_data_flag) {
			break;
		}
		TC_ASSERT_LEQ_CLEANUP("task_manager_broadcast", sleep_cnt, 1, unregister_tasks_3(tm_sample_handle));
		sleep_cnt++;
	}

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_3(tm_sample_handle));

	unregister_tasks_3(tm_sample_handle);

	ret = task_manager_dealloc_broadcast_msg(tm_broadcast_msg, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_dealloc_broadcast_msg", ret, OK);

	TC_SUCCESS_RESULT();
}

int itc_task_manager_task9_main(int argc, char *argv[])
{
	int ret;

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : fail to stop task8 ERR: %d\n", ret);
	}
	return 0;
}

void itc_task_manager_group_unregister_p(void)
{
	int tm_sample_handle2;
	int tm_sample_handle3;

	tm_sample_handle = task_manager_register_task("task8", 100, 1024, not_builtin_task_1, NULL, TM_APP_PERMISSION_GROUP, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle3 = task_manager_register_task("task10", 100, 1024, not_builtin_task_1, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_task("task9", 100, 1024, itc_task_manager_task9_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	tm_appinfo_t *sample_info;

	sample_info = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);

	int ret;

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	ret = task_manager_start(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	usleep(100);

	unregister_tasks_4(sample_info->tm_gid);

	TC_SUCCESS_RESULT();
}

void itc_exit_handler(void *info)
{
	if (strncmp((char *)info, EXIT_CB_CHECK_STR, strlen(EXIT_CB_CHECK_STR) + 1) == 0) {
		int ret;

		ret = task_manager_pause(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ("task_manager_pause", ret, OK);
	}
}

void itc_stop_cb(void *data)
{
	int ret;

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_3(tm_sample_handle));

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);
}

void itc_task_manager_task12(void)
{
	int ret;

	ret = task_manager_set_stop_cb(itc_stop_cb, NULL);
	if (ret != OK) {
		printf("ERROR : fail to set stop callback ERR: %d\n", ret);
	}

	tm_msg_t data;

	data.msg_size = strlen(EXIT_CB_CHECK_STR) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, EXIT_CB_CHECK_STR, data.msg_size);

	ret = task_manager_set_exit_cb(itc_exit_handler, &data);
	TC_ASSERT_EQ_CLEANUP("task_manager_set_exit_cb", ret, OK, free(data.msg));

	free(data.msg);


	while (1) {
		usleep(1);
	}
}

int itc_task_manager_task12_main(int argc, char *argv[])
{
	itc_task_manager_task12();
	return 0;
}

void itc_task_manager_stop_exit_p(void)
{
	int tm_sample_handle2;

	tm_sample_handle = task_manager_register_task("task11", 100, 1024, not_builtin_task_1, NULL, TM_APP_PERMISSION_GROUP, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_task("task12", 100, 1024, itc_task_manager_task12_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	tm_appinfo_t *sample_info;

	sample_info = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);

	int ret;

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	usleep(100);

	ret = task_manager_stop(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}

void tm_itc_main(void)
{
	pid_tm_itc = getpid();

	itc_task_manager_check_dedicate_permission_p();

	itc_task_manager_check_group_permission_p();

	itc_task_manager_check_all_permission_p();

	itc_task_manager_unicast_thrice_p();

	itc_task_manager_broadcast_twice_p();

	itc_task_manager_group_unregister_p();

	itc_task_manager_stop_exit_p();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_task_manager_main(int argc, char *argv[])
#endif
{
	int status;

	if (tc_handler(TC_START, "TaskManager ITC") == ERROR) {
		return ERROR;
	}

	handle_tm_itc = task_manager_register_builtin("tm_itc", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	(void)task_manager_start(handle_tm_itc, TM_NO_RESPONSE);
	sleep(3);	//wait for starting tm_itc

	(void)waitpid(pid_tm_itc, &status, 0);

	(void)task_manager_unregister(handle_tm_itc, TM_NO_RESPONSE);

	(void)tc_handler(TC_END, "TaskManager itc");

	return 0;
}

