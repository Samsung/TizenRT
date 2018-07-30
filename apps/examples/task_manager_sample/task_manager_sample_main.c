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
#include <semaphore.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>

#include "task_manager_sample.h"

/****************************************************************************
 * task_manager_sample_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int task_manager_sample_main(int argc, char *argv[])
#endif
{
	int handle_actionmanager;
	int handle_user;
	int ret;
	int ret_unregister_actionmanager;
	int ret_unregister_user;

	sem_init(&tm_sem, 0, 0);

	printf("Task Manager Sample is started\nRegister Action Manager\n");

	handle_actionmanager = task_manager_register_builtin("action_manager", TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (handle_actionmanager < 0) {
		printf("FAIL TO REGISTER ACTION MANAGER, %d\n", handle_actionmanager);
	} else if (handle_actionmanager >= 0) {
		printf("Action Manager is succeefully registered and its handle is %d\n", handle_actionmanager);
	}

	printf("\nRegister User App\n");
	handle_user = task_manager_register_builtin("user", TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (handle_user < 0) {
		printf("FAIL TO REGISTER USER APP, %d\n", handle_user);
	} else if (handle_user >= 0) {
		printf("User App is succeefully registered and its handle is %d\n", handle_user);
	}

	printf("\nStart Action Manager\n");
	ret = task_manager_start(handle_actionmanager, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("FAIL TO START ACTION MANAGER, %d\n", ret);
	} else if (ret == OK) {
		printf("Action Manager is successfully started!\n");
	}

	sem_wait(&tm_sem);
	printf("\nStart User App\n");
	ret = task_manager_start(handle_user, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("FAIL TO START USER APP, %d\n", ret);
	} else if (ret == OK) {
		printf("User App is successfully started!\n");
	}

	sem_wait(&tm_sem);
	printf("\nUnregister Action Manager\n");
	ret_unregister_actionmanager = task_manager_unregister(handle_actionmanager, TM_RESPONSE_WAIT_INF);
	if (ret_unregister_actionmanager < 0) {
		printf("FAIL TO UNREGISTER ACTION MANAGER, %d\n", handle_actionmanager);
	} else if (ret_unregister_actionmanager == OK) {
		printf("Action Manager is succeefully unregistered\n");
	}

	printf("\nUnregister User App\n");
	ret_unregister_user = task_manager_unregister(handle_user, TM_RESPONSE_WAIT_INF);
	if (ret_unregister_user < 0) {
		printf("FAIL TO UNREGISTER USER APP, %d\n", handle_user);
	} else if (ret_unregister_user == OK) {
		printf("User App is succeefully unregistered\n");
	}


	if ((ret_unregister_actionmanager == OK) && (ret_unregister_user == OK)) {
		printf("\n\nTask Manager Samaple is successfully Ended!\n");
	} else {
		printf("\n\nRunning Task Manager Samaple is Failed.\n");
	}

	return 0;
}
