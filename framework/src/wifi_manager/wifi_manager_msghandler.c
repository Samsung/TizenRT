/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <stdint.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <wifi_manager/wifi_manager.h>
#include <tinyara/wifi/wifi_manager.h>
#include <tinyara/net/netlog.h>
#include <tinyara/net/if/wifi.h>
#include "wifi_manager_dhcp.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include "wifi_manager_message.h"
#include "wifi_manager_utils.h"
#include "wifi_manager_lwnl_listener.h"

#define TAG "[WM]"

/*
 * Global variable
 */
static handler_queue g_wifi_message_queue;

/*
 * External functions
 */
extern wifi_manager_result_e wifimgr_handle_request(wifimgr_msg_s *msg);
extern int wifimgr_create_msgqueue(handler_queue *queue);
extern int wifimgr_message_out(handler_msg *msg, handler_queue *queue);

static int _process_msg(int argc, char *argv[])
{
	if (wifimgr_create_msgqueue(&g_wifi_message_queue) < 0) {
		return -1;
	}

	while (1) {
		handler_msg hmsg;
		int res = wifimgr_message_out(&hmsg, &g_wifi_message_queue);
		if (res < 0) {
			NET_LOGE(TAG, "wifimgr msg out fail %d\n", res);
			return -1;
		} else if (res == 1) {
			continue;
		}
	}
	return 0;
}

/**
 * Public
 */
int wifimgr_run_msghandler(void)
{
	int tid = net_task_create("wifi msg handler", 100, 4096, (main_t)_process_msg, NULL);
	if (tid == -1) {
		NET_LOGE(TAG, "wifi msg handler task create %d\n", errno);
		return -1;
	}

	return 0;
}

int wifimgr_post_message(wifimgr_msg_s *msg)
{
	handler_msg hmsg;
	sem_t sem;
	int res = sem_init(&sem, 0, 0);
	if (res != 0) {
		return -1;
	}
	hmsg.signal = &sem;
	hmsg.msg = (void *)msg;

	res = wifimgr_message_in(&hmsg, &g_wifi_message_queue);
	if (res < 0) {
		NET_LOGE(TAG, "wifimgr msg in fail %d\n", res);
		return -1;
	}

	WIFIMGR_WAIT_SIG(hmsg.signal);
	sem_destroy(hmsg.signal);

	return 0;
}
