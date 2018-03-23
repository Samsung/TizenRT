/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _THINGS_COMMON_H_
#define _THINGS_COMMON_H_

#include "ocstack.h"

typedef OCEntityHandlerResult(*request_handler_cb)(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entity_handler_request, void *cb);

typedef enum {
	RST_COMPLETE = 0,
	RST_EASY_SETUP_DISABLE = 1,
	RST_CLOUD_MODULE_DISABLE = (1 << 1),
	RST_CONTROL_MODULE_DISABLE = (1 << 2),
	RST_CONTROL_QUEUE_EMPTY = (1 << 3),
	RST_NOTI_MODULE_DISABLE = (1 << 4),
	RST_DELETE_CLOUD_DEVICE_SUCCESS = (1 << 5),
	RST_DELETE_CLOUD_DEVICE_FAILURE = (1 << 6),
	RST_USER_CONFIRM_COMPLETED = (1 << 7),
	RST_ALL_FLAG = 0xFFFFFFFF
} rst_state_e;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
bool things_get_reset_mask(rst_state_e value);

#endif							// _THINGS_COMMON_H_
