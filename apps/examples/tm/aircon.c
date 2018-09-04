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
#include <stdbool.h>
#include <string.h>
#include <task_manager/task_manager.h>

/****************************************************************************
 * aircon_task
 ****************************************************************************/
static bool finish_available;
static bool wifi_on;

void aircon_broadcast_cb(void *user_data, void *cb_data)
{
	printf("\n[AIRCON] - WIFI ON received.\n");
	wifi_on = true;
}

void aircon_unicast_cb(tm_msg_t *data)
{
	if (strncmp(data->msg, "TEMP_UP", data->msg_size) == 0) {
		printf("\n[AIRCON] Temperature UP!\n");
		finish_available = true;
	}
	free(data->msg);
}

int aircon_task(int argc, char *argv[])
{
	int ret;

	printf("\n[Aircon] Enter Aircon Task\n");

	wifi_on = finish_available = false;

	ret = task_manager_set_broadcast_cb(TM_BROADCAST_WIFI_ON, aircon_broadcast_cb, NULL);
	if (ret < 0) {
		printf("ERROR : set broadcast cb, ret %d, LINE %d\n", ret, __LINE__);
		return -1;
	}

	ret = task_manager_set_unicast_cb(aircon_unicast_cb);
	if (ret < 0) {
		printf("ERROR : set unicast cb, ret %d, LINE %d\n", ret, __LINE__);
		return -1;
	}

	while (1) {
		if (wifi_on == true)
			break;
		printf("\n[Aircon] waits for turning on the WIFI...\n");
		sleep(1);
	}

	while (1) {
		if (finish_available)
			break;
		sleep(1);
	}

	ret = task_manager_unset_broadcast_cb(TM_BROADCAST_WIFI_ON, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("ERROR : unset broadcast cb, ret %d, LINE %d\n", ret, __LINE__);
		return -1;
	}

	printf("\n[Aircon] Finish\n");
	return 0;
}
