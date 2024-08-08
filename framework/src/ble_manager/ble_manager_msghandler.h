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
#pragma once

#include <semaphore.h>
#include <ble_manager/ble_manager.h>

#define BLE_MANAGER_MAX_PARAM 4

struct blemgr_msg {
	blemgr_req_e event;
	ble_result_e result;
	void *param;
	sem_t *signal;
	union {
		void *ptr;
		int32_t val;
	} ret;
};
typedef struct blemgr_msg blemgr_msg_s;

typedef struct _blemgr_msg_params {
	uint8_t count;
	void *param[BLE_MANAGER_MAX_PARAM];
	blemgr_req_e evt;
} blemgr_msg_params;

// application doesn't need to get messages from queue
// int blemgr_get_message(struct blemgr_msg *msg, int flag);
int blemgr_post_message(blemgr_msg_s *msg);
