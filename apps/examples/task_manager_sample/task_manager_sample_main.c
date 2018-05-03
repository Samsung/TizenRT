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
#include <unistd.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int task_manager_sample_main(int argc, char *argv[])
#endif
{
	int tid_actionmanager;
	int tid_user;
	int ret_start_actionmanager;
	int ret_start_user;

	printf("Task Manager Sample is started\n");
	printf("Register Action Manager\n");

	tid_actionmanager = task_manager_register("action_manager", TM_TASK_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (tid_actionmanager < 0) {
		printf("FAIL TO REGISTER Action Manager, %d\n", tid_actionmanager);
	} else if (tid_actionmanager >= 0) {
		printf("Action Manager is succeefully registered and its tid is %d\n", tid_actionmanager);
	}

	printf("\nRegister User App\n");
	tid_user = task_manager_register("user", TM_TASK_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (tid_user < 0) {
		printf("FAIL TO REGISTER User App, %d\n", tid_user);
	} else if (tid_user > 0) {
		printf("User App is succeefully registered and its tid is %d\n", tid_user);
	}

	printf("\nAction Manager starts\n");
	ret_start_actionmanager = task_manager_start(tid_actionmanager, TM_RESPONSE_WAIT_INF);

	if (ret_start_actionmanager < 0) {
		printf("FAIL TO START Action Manager, %d\n", ret_start_actionmanager);
	} else if (ret_start_actionmanager == OK) {
		printf("Action Manager is successfully started!\n");
	}

	sleep(3);
	printf("\nUser App starts\n");
	ret_start_user = task_manager_start(tid_user, TM_RESPONSE_WAIT_INF);

	if (ret_start_user < 0) {
		printf("FAIL TO START User App, %d\n", ret_start_user);
	} else if (ret_start_user == OK) {
		printf("User App is successfully started!\n");
	}

	return 0;
}
