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
	app_info_list_t *action_manager_info;

	char *msg1 = (char *)malloc(sizeof(char) * 9);
	char *msg2 = (char *)malloc(sizeof(char) * 12);
	char *msg3 = (char *)malloc(sizeof(char) * 13);	
	char *msg4 = (char *)malloc(sizeof(char) * 14);
	char *msg5 = (char *)malloc(sizeof(char) * 10);
	strcpy(msg1, "alarm_on");
	strcpy(msg2, "alarm_pause");
	strcpy(msg3, "alarm_resume");
	strcpy(msg4, "alarm_restart");
	strcpy(msg5, "alarm_off");

	action_manager_info = (app_info_list_t *)task_manager_getinfo_with_name("action_manager", TM_RESPONSE_WAIT_INF);
	if (action_manager_info != NULL) {
		handle_actionmanager = action_manager_info->task.handle;
	}
	task_manager_clean_infolist(&action_manager_info);
	
	printf("\nUser App sends Alarm On message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, msg1, strlen(msg1) + 1, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm On message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Pause message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, msg2, strlen(msg2) + 1, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Pause message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Resume message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, msg3, strlen(msg3) + 1, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Resume message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Restart message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, msg4, strlen(msg4) + 1, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Restart message is successfully sended!\n");
	}

	sleep(3);
	printf("\nUser App sends Alarm Off message to Action Manager\n");
	ret = task_manager_unicast(handle_actionmanager, msg5, strlen(msg5) + 1, TM_RESPONSE_WAIT_INF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("Alarm Off message is successfully sended!\n");
	}

	free(msg1);
	free(msg2);
	free(msg3);
	free(msg4);
	free(msg5);

	sleep(1);
	printf("\nUser App broadcasts WIFI_ON message\n");
	ret = task_manager_broadcast(TM_BROADCAST_WIFI_ON);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("WIFI_ON message is successfully broadcasted!\n");
	}

	sleep(1);
	printf("\nUser App broadcasts WIFI_OFF message\n");
	ret = task_manager_broadcast(TM_BROADCAST_WIFI_OFF);
	if (ret != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret);
	} else {
		printf("WIFI_OFF message is successfully broadcasted!\n");
	}

	return 0;
}
