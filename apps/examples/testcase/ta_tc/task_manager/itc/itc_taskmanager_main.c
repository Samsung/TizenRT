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
static int pid_tm_user1;
static int pid_tm_task2;
static int pid_tm_task4;
static int pid_tm_task9;
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
	int tm_sample_handle2;
	tm_appinfo_list_t *sample_list_info;

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks ERR: %d\n", ret);
	}

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("itc_taskmanager_user1", TM_RESPONSE_WAIT_INF));
	if (sample_list_info == NULL) {
		printf("ERROR: fail to task_manager_getinfo_with_name in unregister_tasks\n");
		return;
	}

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	if (sample_list_info != NULL) {
		printf("ERROR: fail to task_manager_clean_infolist in unregister_tasks\n");
		task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		return;
	}

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks ERR: %d\n", ret);
	}

	return;
}

void unregister_tasks_3(int tm_sample_handle2)
{
	int ret;

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregsiter in unregister_tasks_3 ERR: %d\n", ret);
	}
}

void unregister_tasks_4(int gid)
{
	tm_appinfo_list_t *group_list_info;
	tm_appinfo_t *task_info;
	int ret;

	group_list_info = (tm_appinfo_list_t *)task_manager_getinfo_with_group(gid, TM_RESPONSE_WAIT_INF);
	if (group_list_info == NULL) {
		printf("ERROR: fail to task_manager_getinfo_with_group in unregister_tasks_4\n");
		return;
	}

	while (group_list_info != NULL) {

		task_info = &(group_list_info->task);
		ret = task_manager_stop(task_info->handle, TM_RESPONSE_WAIT_INF);
		if (ret != OK && ret != TM_ALREADY_STOPPED_APP) {
			printf("ERROR: fail to task_manager_stop in unregister_tasks_4 ERR: %d\n", ret);
		}

		ret = task_manager_unregister(task_info->handle, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_unregister in unregister_tasks_4 ERR: %d\n", ret);
		}

		task_manager_clean_info(&task_info);
		if (task_info != NULL) {
			printf("ERROR: fail to task_manager_clean_info in unregister_tasks_4\n");
		}

		group_list_info = group_list_info->next;
	}

	task_manager_clean_infolist(&group_list_info);
	if (group_list_info != NULL) {
		printf("ERROR: fail to task_manager_clean_infolist in unregister_tasks_4\n");
		return;
	}
}

void unregister_tasks_5(int tm_sample_handle2, int tm_sample_handle3)
{
	int ret;

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks_5 ERR: %d\n", ret);
	}

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks_5 ERR: %d\n", ret);
	}

	ret = task_manager_unregister(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks_5 ERR: %d\n", ret);
	}
}

void itc_taskmanager_check_dedicate_permission_p(void)
{
	int tm_sample_handle2;
	int status;
	int ret;

	tm_sample_handle = task_manager_register_builtin("tm_sample_itc", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_builtin", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_builtin("start_itc_sampl", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_builtin", tm_sample_handle2, 0);

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks());

	usleep(10);

	waitpid(pid_tm_user1, &status, 0);

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, unregister_tasks_3(tm_sample_handle2));

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}

void taskmanager_sample_main(void)
{
	while (1) {
		usleep(1);
	}	
}

void taskmanager_start_sample_main(void)
{
	int ret;
	pid_tm_user1 = getpid();

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != TM_NO_PERMISSION) {
		printf("ERROR: fail to task_manager_start in taskmanager_start_task0_main ERR: %d\n", ret);
	}

}

void cleanup_task1(void)
{
	unregister_tasks_3(tm_sample_handle);
}

void unregister_tasks_1(void)
{
	int ret;
	int tm_sample_handle2;
	tm_appinfo_list_t *sample_list_info;

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks_1 ERR: %d\n", ret);
	}

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task2", TM_RESPONSE_WAIT_INF));
	if (sample_list_info == NULL) {
		printf("ERROR: fail to task_manager_getinfo_with_name in unregister_tasks_1\n");
		return;
	}

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	if (sample_list_info != NULL) {
		printf("ERROR: fail to task_manager_clean_infolist in unregister_tasks_1\n");
		return;
	}
	
	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks_1 ERR: %d\n", ret);
	}

}

void taskmanager_pause_task1(void)
{
	int ret;

	ret = task_manager_pause(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_pause in taskmanager_pause_task1 ERR: %d\n", ret);
		unregister_task1_flag = true;
		return;
	}
	ret = task_manager_resume(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_resume in taskmanager_pause_task1 ERR: %d\n", ret);
		unregister_task1_flag = true;
		return;
	}

	ret = task_manager_restart(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_restart in taskmanager_pause_task1 ERR: %d\n", ret);
		unregister_task1_flag = true;
		return;
	}

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_stop in taskmanager_pause_task1 ERR: %d\n", ret);
		unregister_task1_flag = true;
		return;
	}
	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in taskmanager_pause_task1 ERR: %d\n", ret);
		unregister_task1_flag = true;
		return;
	}

}

int taskmanager_pause_task1_main(int argc, char *argv[])
{

	pid_tm_task2 = getpid();

	taskmanager_pause_task1();
	return 0;
}

void itc_taskmanager_check_group_permission_p(void)
{
	int tm_sample_handle2;
	int ret;
	int status;

	tm_sample_handle = task_manager_register_task("task1", 100, 1024, not_builtin_task_1, NULL, TM_APP_PERMISSION_GROUP, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_task("task2", 100, 1024, taskmanager_pause_task1_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_1());

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_1());

	unregister_task1_flag = false;
	usleep(1);

	waitpid(pid_tm_task2, &status, 0);

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	if (unregister_task1_flag) {
		ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ("task_manager_stop", ret, OK);

		ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
		TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	}
	
	TC_ASSERT_EQ("task_manager_stop", unregister_task1_flag, false);

	TC_SUCCESS_RESULT();
}

void cleanup_task5(void)
{
	unregister_task5_flag = true;
}

void taskmanager_task3(void)
{
	int ret;
	int tm_sample_handle4;
	tm_appinfo_list_t *sample_list_info;

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task5", TM_RESPONSE_WAIT_INF));
	if (sample_list_info == NULL) {
		printf("ERROR: fail to task_manager_getinfo_with_name in taskmanager_task3\n");
		cleanup_task5();
		return;
	}

	tm_sample_handle4 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	if (sample_list_info != NULL) {
		printf("ERROR: fail to task_manager_clean_infolist in taskmanager_task3\n");
		cleanup_task5();
		return;
	}

	ret = task_manager_pause(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_pause in taskmanager_task3 ERR: %d\n", ret);
		cleanup_task5();
		return;
	}

	ret = task_manager_resume(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_resume in taskmanager_task3 ERR: %d\n", ret);
		cleanup_task5();
		return;
	}

	ret = task_manager_restart(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_restart in taskmanager_task3 ERR: %d\n", ret);
		cleanup_task5();
		return;
	}

	ret = task_manager_stop(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_stop in taskmanager_task3 ERR: %d\n", ret);
		cleanup_task5();
		return;
	}

	ret = task_manager_unregister(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in taskmanager_task3 ERR: %d\n", ret);
		cleanup_task5();
		return;
	}

}

int taskmanager_task3_main(int argc, char *argv[])
{
	taskmanager_task3();
	return 0;
}

void taskmanager_task4(void)
{
	int tm_sample_handle4;
	int ret;

	tm_sample_handle4 = task_manager_register_task("task5", 100, 1024, not_builtin_task_1, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (tm_sample_handle4 < 0) {
		printf("ERROR: fail to task_manager_register_task in taskmanager_task4 ERR: %d\n", tm_sample_handle4);
		return;
	}

	unregister_task5_flag = false;

	ret = task_manager_start(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_start in taskmanager_task4 ERR: %d\n", ret);
		unregister_tasks_3(tm_sample_handle4);
		return;
	}

	while (1) {
		usleep(1);
		if (unregister_task5_flag) {
			ret = task_manager_unregister(tm_sample_handle4, TM_RESPONSE_WAIT_INF);
			if (ret != OK) {
				printf("ERROR: fail to task_manager_unregister in taskmanager_task4 ERR: %d\n", ret);
			}
		}
	}
}

int taskmanager_task4_main(int argc, char *argv[])
{
	pid_tm_task4 = getpid();

	taskmanager_task4();
	return 0;
}

void unregister_tasks_2(void)
{
	int ret;
	int tm_sample_handle2;
	tm_appinfo_list_t *sample_list_info;

	if (unregister_task5_flag) {
		sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task5", TM_RESPONSE_WAIT_INF));
		if (sample_list_info == NULL) {
			printf("ERROR: fail to task_manager_getinfo_with_name in unregister_tasks_2\n");
			return;
		}

		tm_sample_handle2 = sample_list_info->task.handle;
		task_manager_clean_infolist(&sample_list_info);
		if (sample_list_info != NULL) {
			printf("ERROR: fail to task_manager_clean_infolist in unregister_tasks_2\n");
			task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
			return;
		}

		ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_unregister in unregister_tasks_2 ERR: %d\n", ret);
			return;
			
		}
	}

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task4", TM_RESPONSE_WAIT_INF));
	if (sample_list_info == NULL) {
		printf("ERROR: fail to task_manager_getinfo_with_name in unregister_tasks_2\n");

		return;
	}

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	if (sample_list_info != NULL) {
		printf("ERROR: fail to task_manager_clean_infolist in unregister_tasks_2\n");
		task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		return;
	}

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks_2 ERR: %d\n", ret);
	}

	sample_list_info = ((tm_appinfo_list_t *)task_manager_getinfo_with_name("task3", TM_RESPONSE_WAIT_INF));
	if (sample_list_info == NULL) {
		printf("ERROR: fail to task_manager_getinfo_with_name in unregister_tasks_2\n");
		return;
	}

	tm_sample_handle2 = sample_list_info->task.handle;
	task_manager_clean_infolist(&sample_list_info);
	if (sample_list_info != NULL) {
		printf("ERROR: fail to task_manager_clean_infolist in unregister_tasks_2\n");
		task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		return;
	}

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR: fail to task_manager_unregister in unregister_tasks_2 ERR: %d\n", ret);
	}

}

void itc_taskmanager_check_all_permission_p(void)
{
	int ret;
	int tm_sample_handle3;
	int tm_sample_handle2;
	int status;

	tm_sample_handle3 = task_manager_register_task("task4", 100, 1024, taskmanager_task4_main, NULL, TM_APP_PERMISSION_GROUP, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle3, 0);

	tm_sample_handle2 = task_manager_register_task("task3", 100, 1024, taskmanager_task3_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	ret = task_manager_start(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_2());

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_2());

	usleep(1);

	waitpid(pid_tm_task4, &status, 0);

	ret = task_manager_stop(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_2());

	ret = task_manager_unregister(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unregister", ret, OK, unregister_tasks_3(tm_sample_handle2));

	ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}

void unicast_handler_2(tm_msg_t *info)
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

void unicast_handler(tm_msg_t *info)
{
	int tm_sample_handle2;
	int ret;
	tm_appinfo_t *task_info;

	if (!strncmp((char *)info->msg, TM_START_MSG, strlen(TM_START_MSG))) {

		tm_sample_handle2 = task_manager_register_task("task6", 100, 2048, not_builtin_task, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
		if (tm_sample_handle2 < 0) {
			printf("ERROR: fail to task_manager_register_task in unicast_handler ERR: %d\n", tm_sample_handle2);
			return;
		}

		ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_start in unicast_handler ERR: %d\n", ret);
			unregister_tasks_3(tm_sample_handle2);
			return;
		}

		ret = task_manager_pause(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_pause in unicast_handler ERR: %d\n", ret);
			unregister_tasks_3(tm_sample_handle2);
			return;
		}

		ret = task_manager_resume(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_resume in unicast_handler ERR: %d\n", ret);
			unregister_tasks_3(tm_sample_handle2);
			return;
		}

		ret = task_manager_restart(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_restart in unicast_handler ERR: %d\n", ret);
			unregister_tasks_3(tm_sample_handle2);
			return;
		}

	} else if (!strncmp((char *)info->msg, TM_STOP_MSG, strlen(TM_STOP_MSG))) {

		task_info = ((tm_appinfo_t *)task_manager_getinfo_with_pid(pid_task6, TM_RESPONSE_WAIT_INF));
		if (task_info == NULL) {
			printf("ERROR: fail to task_manager_getinfo_with_pid in unicast_handler\n");
			return;
		}

		tm_sample_handle2 = task_info->handle;
		task_manager_clean_info(&task_info);
		if (task_info != NULL) {
			printf("ERROR: fail to task_manager_clean_info in unicast_handler\n");
			return;
		}

		ret = task_manager_stop(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_stop in unicast_handler ERR: %d\n", ret);
			unregister_tasks_3(tm_sample_handle2);
			return;
		}

		ret = task_manager_unregister(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_unregister in unicast_handler ERR: %d\n", ret);
			unregister_tasks_3(tm_sample_handle2);
			return;
		}

		ret = task_manager_set_unicast_cb(unicast_handler_2);
		if (ret != OK) {
			printf("ERROR: fail to task_manager_set_unicast_cb in unicast_handler ERR: %d\n", ret);
		}
	}
}

void *taskmanager_pthread(void *param)
{
	int ret;

	ret = task_manager_set_unicast_cb(unicast_handler);
	if (ret != OK) {
		printf("ERROR : fail to set unicast callback handler\n");
	}
	while (1) {
		usleep(1);
	}
}

void itc_taskmanager_unicast_thrice_p(void)
{
	int tm_pthread_handle;
	int ret;
	tm_msg_t send_msg;
	tm_msg_t reply_msg;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	tm_pthread_handle = task_manager_register_pthread("tm_pthread", &attr, taskmanager_pthread, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_pthread", tm_pthread_handle, 0);

	ret = task_manager_start(tm_pthread_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_3(tm_pthread_handle));

	usleep(1);

	send_msg.msg_size = strlen(TM_START_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_START_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_pthread_handle, &send_msg, NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, OK);
	usleep(1);

	send_msg.msg_size = strlen(TM_STOP_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_STOP_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_pthread_handle, &send_msg, NULL, TM_NO_RESPONSE);
	TC_ASSERT_EQ("task_manager_unicast", ret, OK);

	usleep(1);

	send_msg.msg_size = strlen(TM_SYNC_SEND_MSG) + 1;
	send_msg.msg = malloc(send_msg.msg_size);
	TC_ASSERT_NEQ("task_manager_unicast", send_msg.msg, NULL);

	strncpy(send_msg.msg, TM_SYNC_SEND_MSG, send_msg.msg_size);

	ret = task_manager_unicast(tm_pthread_handle, &send_msg, &reply_msg, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_unicast", ret, OK, free(send_msg.msg));

	if (strncmp((char *)reply_msg.msg, (char *)TM_SYNC_RECV_MSG, strlen(TM_SYNC_RECV_MSG)) != 0) {
		printf("Callback fail to");
	}
	ret = task_manager_stop(tm_pthread_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_3(tm_pthread_handle));

	ret = task_manager_unregister(tm_pthread_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ("task_manager_unregister", ret, OK);

	TC_SUCCESS_RESULT();
}


void broadcast_handler(tm_msg_t *user_data, tm_msg_t *info)
{
	int ret;
	tm_msg_t data;

	if (strncmp((char *)info->msg, TM_BROAD_START, strlen(TM_BROAD_START) + 1) == 0) {

		ret = task_manager_unset_broadcast_cb(tm_broadcast_msg, TM_NO_RESPONSE);
		if (ret != OK) {
			printf("ERROR : fail to unset broadcast callback in broadcast_handler ERR: %d\n", ret);
			return;
		}

		broadcast_data_flag = true;

		data.msg_size = strlen(TM_BROAD_END) + 1;
		data.msg = malloc(data.msg_size);
		memcpy(data.msg, TM_BROAD_END, data.msg_size);
		ret = task_manager_set_broadcast_cb(tm_broadcast_msg, broadcast_handler, &data);
		free(data.msg);
		if (ret != OK) {
			printf("ERROR : fail to set broadcast callback in broadcast_handler ERR: %d\n", ret);
		}
	} else if (strncmp((char *)info->msg, TM_BROAD_END, strlen(TM_BROAD_END) + 1) == 0) {
		broadcast_data_flag = false;
		ret = task_manager_unset_broadcast_cb(tm_broadcast_msg, TM_NO_RESPONSE);
		if (ret != OK) {
			printf("ERROR : fail to unset broadcast callback in broadcast_handler ERR: %d\n", ret);
		}
	}
}

void taskmanager_task7(void)
{
	int ret;
	tm_msg_t data;

	data.msg_size = strlen(TM_BROAD_START) + 1;
	data.msg = malloc(data.msg_size);
	memcpy(data.msg, TM_BROAD_START, data.msg_size);
	ret = task_manager_set_broadcast_cb(tm_broadcast_msg, broadcast_handler, &data);
	free(data.msg);
	if (ret != OK) {
		printf("ERROR : fail to set broadcast callback in taskmanager_task7 ERR: %d\n", ret);
		return;
	}
	while (1) {
		usleep(1);
	}
}

int taskmanager_task7_main(int argc, char *argv[])
{
	taskmanager_task7();
	return 0;
}

void itc_taskmanager_broadcast_twice_p(void)
{
	int ret;
	tm_msg_t user_data;
	int sleep_cnt = 0;

	tm_broadcast_msg = task_manager_alloc_broadcast_msg();
	TC_ASSERT_GEQ("task_manager_alloc_broadcast_msg", tm_broadcast_msg, 0);

	tm_sample_handle = task_manager_register_task("task7", 100, 2048, taskmanager_task7_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_3(tm_sample_handle));

	user_data.msg_size = strlen("START") + 1;
	user_data.msg = malloc(user_data.msg_size);
	strncpy(user_data.msg, "START", user_data.msg_size);

	usleep(1);

	(void)task_manager_broadcast(tm_broadcast_msg, &user_data, TM_NO_RESPONSE);
	while (1) {
		usleep(1);
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

	usleep(1);

	(void)task_manager_broadcast(tm_broadcast_msg, &user_data, TM_NO_RESPONSE);
	while (1) {
		usleep(1);
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

int taskmanager_task9_main(int argc, char *argv[])
{
	int ret;

	pid_tm_task9 = getpid();

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : fail to stop task8\n");
	}
	return 0;
}

void itc_taskmanager_group_unregister_p(void)
{
	int tm_sample_handle2;
	int tm_sample_handle3;
	int ret;
	int status;
	tm_appinfo_t *sample_info;

	tm_sample_handle = task_manager_register_task("task8", 100, 2048, not_builtin_task, NULL, TM_APP_PERMISSION_GROUP, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle3 = task_manager_register_task("task10", 100, 2048, not_builtin_task, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_task("task9", 100, 2048, taskmanager_task9_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	sample_info = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	ret = task_manager_start(tm_sample_handle3, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	usleep(1);

	waitpid(pid_tm_task9, &status, 0);

	unregister_tasks_4(sample_info->tm_gid);

	TC_SUCCESS_RESULT();
}

void taskmanager_stop_cb(void *data)
{
	int ret;

	ret = task_manager_stop(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : fail to stop taskmanager handle ERR: %d\n", ret);
		unregister_tasks_3(tm_sample_handle);
		return;
	}

	ret = task_manager_unregister(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : fail to unregister taskmanager handle ERR: %d\n", ret);
	}
}

void taskmanager_task12(void)
{
	int ret;

	ret = task_manager_set_stop_cb(taskmanager_stop_cb, NULL);
	if (ret != OK) {
		printf("ERROR : fail to set stop callback ERR: %d\n", ret);
		return;
	}

	while (1) {
		usleep(1);
	}
}

int taskmanager_task12_main(int argc, char *argv[])
{
	taskmanager_task12();
	return 0;
}

void itc_taskmanager_stop_exit_p(void)
{
	int tm_sample_handle2;
	int ret;
	tm_appinfo_t *sample_info;

	tm_sample_handle = task_manager_register_task("task11", 100, 2048, not_builtin_task_1, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle, 0);

	tm_sample_handle2 = task_manager_register_task("task12", 100, 2048, taskmanager_task12_main, NULL, TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_GEQ("task_manager_register_task", tm_sample_handle2, 0);

	sample_info = (tm_appinfo_t *)task_manager_getinfo_with_handle(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_NEQ("task_manager_getinfo_with_handle", sample_info, NULL);

	ret = task_manager_start(tm_sample_handle, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	ret = task_manager_start(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_start", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	usleep(1);

	ret = task_manager_stop(tm_sample_handle2, TM_RESPONSE_WAIT_INF);
	TC_ASSERT_EQ_CLEANUP("task_manager_stop", ret, OK, unregister_tasks_4(sample_info->tm_gid));

	usleep(1);

	unregister_tasks_4(sample_info->tm_gid);

	TC_SUCCESS_RESULT();
}

void tm_itc_main(void)
{
	pid_tm_itc = getpid();

	itc_taskmanager_check_dedicate_permission_p();

	itc_taskmanager_check_group_permission_p();

	itc_taskmanager_check_all_permission_p();

	itc_taskmanager_unicast_thrice_p();

	itc_taskmanager_broadcast_twice_p();

	itc_taskmanager_group_unregister_p();

	itc_taskmanager_stop_exit_p();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_taskmanager_main(int argc, char *argv[])
#endif
{
	int status;

	if (testcase_state_handler(TC_START, "TaskManager ITC") == ERROR) {
		return ERROR;
	}

	handle_tm_itc = task_manager_register_builtin("tm_itc", TM_APP_PERMISSION_DEDICATE, TM_RESPONSE_WAIT_INF);
	(void)task_manager_start(handle_tm_itc, TM_NO_RESPONSE);
	sleep(1);	//wait for starting tm_itc

	(void)waitpid(pid_tm_itc, &status, 0);

	(void)task_manager_unregister(handle_tm_itc, TM_NO_RESPONSE);

	(void)testcase_state_handler(TC_END, "TaskManager itc");

	return 0;
}

