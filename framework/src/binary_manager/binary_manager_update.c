/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <sched.h>
#include <fcntl.h>
#include <string.h>
#include <tinyara/binary_manager.h>
#include <binary_manager/binary_manager.h>
#include "binary_manager_internal.h"

binmgr_result_type_e binary_manager_update_binary(char *binary_name)
{
	binmgr_result_type_e ret;
	binmgr_request_t request_msg;

	if (binary_name == NULL || strlen(binary_name) > BIN_NAME_MAX - 1) {
		bmdbg("load_new_binary failed : invalid binary name\n");
		return BINMGR_INVALID_PARAM;
	}

	ret = binary_manager_set_request(&request_msg, BINMGR_UPDATE, binary_name);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_send_request(&request_msg);
	if (ret != BINMGR_OK) {
		return ret;
	}

	return BINMGR_OK;
}

binmgr_result_type_e binary_manager_get_update_info(char *binary_name, binary_update_info_t *binary_info)
{
	binmgr_result_type_e ret;
	binmgr_request_t request_msg;
	binmgr_getinfo_response_t response_msg;

	if (binary_name == NULL || binary_info == NULL || strlen(binary_name) > BIN_NAME_MAX - 1) {
		bmdbg("get_binary_info failed : invalid param.\n");
		return BINMGR_INVALID_PARAM;
	}

	ret = binary_manager_set_request(&request_msg, BINMGR_GET_INFO, binary_name);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_send_request(&request_msg);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_receive_response(&response_msg, sizeof(binmgr_getinfo_response_t));
	if (ret != BINMGR_OK) {
		return ret;
	}

	if (response_msg.result == BINMGR_OK) {
		/* Copy binary info data */
		memset(binary_info, 0, sizeof(binary_update_info_t));
		memcpy(binary_info, &response_msg.data, sizeof(binary_update_info_t));
	} else {
		bmdbg("Binary manager getinfo FAIL %d\n", response_msg.result);
	}

	return response_msg.result;
}

binmgr_result_type_e binary_manager_get_update_info_all(binary_update_info_list_t *binary_info_list)
{
	binmgr_result_type_e ret;
	binmgr_request_t request_msg;
	binmgr_getinfo_all_response_t response_msg;

	ret = binary_manager_set_request(&request_msg, BINMGR_GET_INFO_ALL, NULL);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_send_request(&request_msg);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_receive_response(&response_msg, sizeof(binmgr_getinfo_all_response_t));
	if (ret != BINMGR_OK) {
		return ret;
	}

	if (response_msg.result == BINMGR_OK) {
		/* Copy binary info list data */
		memset(binary_info_list, 0, sizeof(binary_update_info_list_t));
		memcpy(binary_info_list, &response_msg.data, sizeof(binary_update_info_list_t));
	} else {
		bmdbg("Binary manager getinfo_all FAIL %d\n", response_msg.result);
	}

	return response_msg.result;
}

binmgr_result_type_e binary_manager_get_download_path(char *binary_name, uint32_t version, char *download_path)
{
	binmgr_result_type_e ret;
	binmgr_update_bin_t data;
	binmgr_request_t request_msg;
	binmgr_createbin_response_t response_msg;

	if (binary_name == NULL || version == 0) {
		bmdbg("Invalid parameter\n");
		return BINMGR_INVALID_PARAM;
	}

	strncpy(data.bin_name, binary_name, BIN_NAME_MAX - 1);
	data.bin_name[BIN_NAME_MAX - 1] = '\0';
	data.version = version;

	ret = binary_manager_set_request(&request_msg, BINMGR_CREATE_BIN, &data);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_send_request(&request_msg);
	if (ret != BINMGR_OK) {
		return ret;
	}

	ret = binary_manager_receive_response(&response_msg, sizeof(binmgr_createbin_response_t));
	if (ret != BINMGR_OK) {
		return ret;
	}

	if (response_msg.result == BINMGR_OK) {
		bmvdbg("Create file path : %s\n", response_msg.binpath);
		strncpy(download_path, response_msg.binpath, strlen(response_msg.binpath) + 1);
	}

	return response_msg.result;
}
