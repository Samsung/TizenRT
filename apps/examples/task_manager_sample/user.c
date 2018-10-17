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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>

/****************************************************************************
 * user_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int user_main(int argc, char *argv[])
#endif
{
	int handle_actionmanager = 0;
	int ret;
	tm_appinfo_list_t *action_manager_info;
	tm_msg_t uni_msg1;
	tm_msg_t uni_msg2;
	tm_msg_t uni_msg3;
	tm_msg_t uni_msg4;
	tm_msg_t uni_msg5;
	tm_msg_t broad_msg1;
	tm_msg_t broad_msg2;
	char *alarm_on_msg = "alarm_on";
	char *alarm_pause_msg = "alarm_pause";
	char *alarm_resume_msg = "alarm_resume";
	char *alarm_restart_msg = "alarm_restart";
	char *alarm_off_msg = "alarm_off";
	char *wifi_on_msg = "WIFI On";
	char *wifi_off_msg = "WIFI Off";

	uni_msg1.msg = (void *)alarm_on_msg;
	uni_msg1.msg_size = strlen(alarm_on_msg) + 1;
	uni_msg2.msg = (void *)alarm_pause_msg;
	uni_msg2.msg_size = strlen(alarm_pause_msg) + 1;
	uni_msg3.msg = (void *)alarm_resume_msg;
	uni_msg3.msg_size = strlen(alarm_resume_msg) + 1;
	uni_msg4.msg = (void *)alarm_restart_msg;
	uni_msg4.msg_size = strlen(alarm_restart_msg) + 1;
	uni_msg5.msg = (void *)alarm_off_msg;
	uni_msg5.msg_size = strlen(alarm_off_msg) + 1;

	action_manager_info = task_manager_getinfo_with_name("action_manager", TM_RESPONSE_WAIT_INF);
	if (action_manager_info != NULL) {
		handle_actionmanager = action_manager_info->task.handle;
	}
	task_manager_clean_infolist(&action_manager_info);
	
	printf("\nUser App sends Alarm On message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, &uni_msg1, NULL, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm On message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Pause message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, &uni_msg2, NULL, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Pause message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Resume message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, &uni_msg3, NULL, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Resume message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Restart message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, &uni_msg4, NULL, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Restart message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Off message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, &uni_msg5, NULL, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Off message is successfully sended!\n");
	}

	broad_msg1.msg = (void *)wifi_on_msg;
	broad_msg1.msg_size = strlen(wifi_on_msg) + 1;
	sleep(1);
	printf("\nUser App broadcasts WIFI_ON message\n");
	ret = task_manager_broadcast(TM_BROADCAST_WIFI_ON, &broad_msg1, TM_NO_RESPONSE);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("WIFI_ON message is successfully broadcasted!\n");
	}

	broad_msg2.msg = (void *)wifi_off_msg;
	broad_msg2.msg_size = strlen(wifi_off_msg) + 1;
	sleep(1);
	printf("\nUser App broadcasts WIFI_OFF message\n");
	ret = task_manager_broadcast(TM_BROADCAST_WIFI_OFF, &broad_msg2, TM_NO_RESPONSE);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("WIFI_OFF message is successfully broadcasted!\n");
	}

	return 0;
}
