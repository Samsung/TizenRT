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
	int tid_actionmanager = 0;
	int ret_send_msg1;
	int ret_send_msg2;
	task_info_list_t *action_manager_info;

	char *msg1 = (char *)malloc(sizeof(char) * 9);
	char *msg2 = (char *)malloc(sizeof(char) * 10);
	strcpy(msg1, "alarm_on");
	strcpy(msg2, "alarm_off");

	action_manager_info = (task_info_list_t *)task_manager_getinfo_with_name("action_manager", TM_RESPONSE_WAIT_INF);
	if (action_manager_info != NULL) {
		tid_actionmanager = action_manager_info->task.handle;
	}

	printf("\nUser App sends Alarm On message to Action Manager\n");
	ret_send_msg1 = task_manager_unicast(tid_actionmanager, msg1, strlen(msg1) + 1, TM_RESPONSE_WAIT_INF);
	if (ret_send_msg1 != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret_send_msg1);
	} else if (ret_send_msg1 == OK) {
		printf("Alarm On message is successfully sended!\n");
	}

	sleep(5);
	printf("\n");
	printf("User App sends Alarm Off message to Action Manager\n");
	ret_send_msg2 = task_manager_unicast(tid_actionmanager, msg2, strlen(msg2) + 1, TM_RESPONSE_WAIT_INF);
	if (ret_send_msg2 != OK) {
		printf("ERROR : SEND SIGNAL %d\n", ret_send_msg2);
	} else if (ret_send_msg2 == OK) {
		printf("Alarm Off message is successfully sended!\n");
	}

	free(msg1);
	free(msg2);

	return 0;
}
