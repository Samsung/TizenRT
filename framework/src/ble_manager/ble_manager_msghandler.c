/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <ble_manager/ble_manager.h>
#include <semaphore.h>
#include "ble_manager_log.h"
#include "ble_manager_event.h"
#include "ble_manager_message.h"
#include "ble_manager_msghandler.h"
/*
 * Global variable
 */
static ble_handler_queue g_ble_message_queue;

/*
 * External functions
 */
extern ble_result_e blemgr_handle_request(blemgr_msg_s *msg);

static int _process_msg(int argc, char *argv[])
{
	if (blemgr_create_msgqueue(&g_ble_message_queue) < 0) {
		return -1;
	}

	while (1) {
		ble_handler_msg hmsg;
		int res = blemgr_message_out(&hmsg, &g_ble_message_queue);
		if (res < 0) {
			BLE_ERR;
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
int blemgr_run_msghandler(void)
{
	int tid = task_create("ble msg handler", 100, 4096, (main_t)_process_msg, NULL);
	if (tid < 0) {
		BLE_ERR;
		return -1;
	}

	return 0;
}

int blemgr_post_message(blemgr_msg_s *msg)
{
	ble_handler_msg hmsg;
	sem_t sem;
	int res = sem_init(&sem, 0, 0);
	if (res != 0) {
		return -1;
	}
	hmsg.signal = &sem;
	hmsg.msg = (void *)msg;

	res = blemgr_message_in(&hmsg, &g_ble_message_queue);
	if (res < 0) {
		sem_destroy(hmsg.signal);
		BLE_ERR;
		return -1;
	}

	while (sem_wait(hmsg.signal) < 0) {
		DEBUGASSERT(get_errno() == EINTR);
	}

	sem_destroy(hmsg.signal);

	return 0;
}
