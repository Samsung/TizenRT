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
#include <task_manager/task_manager.h>

/****************************************************************************
 * tm_sample_main
 ****************************************************************************/
extern int remocon_task(int argc, char *argv[]);
extern int aircon_task(int argc, char *argv[]);

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tm_main(int argc, char *argv[])
#endif
{
	int ret;
	int remocon_handle;
	int aircon_handle;
	printf("Hello, tm_sample!!\n");

	remocon_handle = task_manager_register_task("remocon", 100, 1024, remocon_task, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (remocon_handle < 0) {
		printf("ERROR : register task, handle %d, Line %d\n", remocon_handle, __LINE__);
		return -1;
	}

	aircon_handle = task_manager_register_task("aircon", 100, 1024, aircon_task, NULL, TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (aircon_handle < 0) {
		printf("ERROR : register task, handle %d, Line %d\n", aircon_handle, __LINE__);
		return -1;
	}

	printf("Start Remocon %d\n", remocon_handle);
	ret = task_manager_start(remocon_handle, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("ERROR : start task, ret %d, Line %d\n", ret, __LINE__);
		return -1;
	}

	printf("Start Aircon %d\n", aircon_handle);
	ret = task_manager_start(aircon_handle, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("ERROR : start task, ret %d, Line %d\n", ret, __LINE__);
		return -1;
	}

	sleep(1); //wait for start another tasks

	ret = task_manager_broadcast(TM_BROADCAST_WIFI_ON, NULL);
	if (ret < 0) {
		printf("ERROR : broadcast, ret %d, Line %d\n", ret, __LINE__);
		return -1;
	}
	printf("\nTurn On the WIFI.\n");

	sleep(3);
	task_manager_unregister(aircon_handle, TM_RESPONSE_WAIT_INF);
	task_manager_unregister(remocon_handle, TM_RESPONSE_WAIT_INF);
	printf("\nFinish Main\n");
	return 0;
}
