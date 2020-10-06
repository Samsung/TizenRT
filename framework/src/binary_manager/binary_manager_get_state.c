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

/***************************************************************************
 * Included Files
 ***************************************************************************/

#include <debug.h>
#include <string.h>
#include <tinyara/binary_manager.h>
#include <binary_manager/binary_manager.h>
#include "binary_manager_internal.h"

binmgr_result_type_e binary_manager_get_state(char *binary_name, int *state)
{
	binmgr_result_type_e ret;
	binmgr_request_t request_msg;
	binmgr_getstate_response_t response_msg;

	if (binary_name == NULL || strlen(binary_name) > BIN_NAME_MAX - 1) {
		bmdbg("get_binary_info failed : invalid param.\n");
		return BINMGR_INVALID_PARAM;
	}

	ret = binary_manager_set_request(&request_msg, BINMGR_GET_STATE, binary_name);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_send_request(&request_msg);
	if (ret != BINMGR_OK) {
		bmdbg("Failed to send request msg %d\n", ret);
		return ret;
	}

	ret = binary_manager_receive_response(&response_msg, sizeof(binmgr_getstate_response_t));
	if (ret != BINMGR_OK) {
		bmdbg("Failed to receive response msg %d\n", ret);
		return ret;
	}

	if (response_msg.result == BINMGR_OK) {
		*state = response_msg.state;
	} else {
		bmdbg("Binary manager getstate FAIL %d\n", response_msg.result);
	}

	return response_msg.result;
}
