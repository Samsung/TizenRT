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
#include <stdbool.h>
#include <string.h>
#include <task_manager/task_manager.h>

static bool is_wifi_on;

/****************************************************************************
 * remocon_task
 ****************************************************************************/
void remocon_broadcast_cb(void *user_data, void *cb_data)
{
	printf("\n[REMOCON] - WIFI ON received.\n");
	is_wifi_on = true;
}

int remocon_task(int argc, char *argv[])
{
	int ret;
	tm_appinfo_list_t *info;
	int aircon_handle;
	tm_msg_t send_msg;

	is_wifi_on = false;

	printf("\n[Remocon] Enter Remocon Task\n");

	ret = task_manager_set_broadcast_cb(TM_BROADCAST_WIFI_ON, remocon_broadcast_cb, NULL);
	if (ret < 0) {
		printf("ERROR : set broadcast cb, ret %d, LINE %d\n", ret, __LINE__);
		return -1;
	}

	while (1) {
		if (is_wifi_on == true)
			break;
		printf("\n[Remocon] waits for turning on the WIFI...\n");
		sleep(1);
	}

	info = task_manager_getinfo_with_name("aircon", TM_RESPONSE_WAIT_INF);
	if (info == NULL) {
		printf("ERROR : getinfo, LINE %d\n", __LINE__);
		return -1;
	}

	do {
		aircon_handle = info->task.handle;
	} while (info->next);

	(void)task_manager_clean_infolist(&info);

	send_msg.msg_size = strlen("TEMP_UP") + 1;
	send_msg.msg = "TEMP_UP";

	sleep(1);
	ret = task_manager_unicast(aircon_handle, &send_msg, NULL, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("ERROR : unicast, ret %d, LINE %d\n", ret, __LINE__);
		return -1;
	}
	printf("\n[Remocon] Send TEMP_UP msg\n");

	ret = task_manager_unset_broadcast_cb(TM_BROADCAST_WIFI_ON, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("ERROR : unset broadcast cb, ret %d, LINE %d\n", ret, __LINE__);
		return -1;
	}

	sleep(1);
	printf("\n[Remocon] Finish\n");
	
	return 0;
}
