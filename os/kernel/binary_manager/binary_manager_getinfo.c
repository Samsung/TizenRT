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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/* Get index in binary table with binary id */
int binary_manager_get_index_with_binid(int bin_id)
{
	int bin_idx;
	uint32_t bin_count;

	if (bin_id <= 0) {
		bmdbg("Invalid bin_id %d\n", bin_id);
		return ERROR;
	}

	bin_count = binary_manager_get_binary_count();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (BIN_ID(bin_idx) == bin_id) {
			bmvdbg("FIND binary %d idx = %d\n", bin_id, bin_idx);
			return bin_idx;
		}
	}

	return ERROR;
}

/* Get index in binary table with binary id */
int binary_manager_get_index_with_name(char *bin_name)
{
	int bin_idx;
	uint32_t bin_count;

	if (bin_name == NULL) {
		bmdbg("Invalid binary name, NULL\n");
		return ERROR;
	}

	bin_count = binary_manager_get_binary_count();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name) + 1)) {
			bmvdbg("FIND binary %s idx = %d\n", bin_name, bin_idx);
			return bin_idx;
		}
	}

	return ERROR;
}

/* Get binary info with binary name */
void binary_manager_get_info_with_name(int requester_pid, char *bin_name)
{
	int bin_idx;
	uint32_t bin_count;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_getinfo_response_t response_msg;

	if (requester_pid < 0 || bin_name == NULL) {
		bmdbg("Invalid data pid %d name %s\n", requester_pid, bin_name);
		return;
	}
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);

	memset((void *)&response_msg, 0, sizeof(binmgr_getinfo_response_t));
	response_msg.result = BINMGR_NOT_FOUND;

	bin_count = binary_manager_get_binary_count();
	for (bin_idx = 0; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, BIN_NAME_MAX)) {
			response_msg.result = BINMGR_OK;
			response_msg.data.inactive_partsize = BIN_PARTSIZE(bin_idx, (BIN_USEIDX(bin_idx) ^ 1));
			strncpy(response_msg.data.name, BIN_NAME(bin_idx) , BIN_NAME_MAX);
			strncpy(response_msg.data.active_ver, BIN_VER(bin_idx), BIN_VER_MAX);
			snprintf(response_msg.data.active_dev, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, BIN_USEIDX(bin_idx)));
			if (BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx) ^ 1)) != -1) {
				snprintf(response_msg.data.inactive_dev, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx) ^ 1)));
			}
			break;
		}
	}

	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getinfo_response_t));
}

/* Get info of all registered binaries */
void binary_manager_get_info_all(int requester_pid)
{
	int bin_idx;
	uint32_t bin_count;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_getinfo_all_response_t response_msg;

	if (requester_pid < 0) {
		bmdbg("Invalid requester pid %d\n", requester_pid);
		return;
	}
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);

	memset((void *)&response_msg, 0, sizeof(binmgr_getinfo_all_response_t));

	bin_count = binary_manager_get_binary_count();
	if (bin_count > 0) {
		for (bin_idx = 0; bin_idx < bin_count + 1; bin_idx++) {
			response_msg.data.bin_info[bin_idx].inactive_partsize = BIN_PARTSIZE(bin_idx, (BIN_USEIDX(bin_idx) ^ 1));
			strncpy(response_msg.data.bin_info[bin_idx].name, BIN_NAME(bin_idx) , BIN_NAME_MAX);
			strncpy(response_msg.data.bin_info[bin_idx].active_ver, BIN_VER(bin_idx), BIN_VER_MAX);
			snprintf(response_msg.data.bin_info[bin_idx].active_dev, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, BIN_USEIDX(bin_idx)));
			if (BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx) ^ 1)) != -1) {
				snprintf(response_msg.data.bin_info[bin_idx].inactive_dev, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx) ^ 1)));
			}
		}
		response_msg.data.bin_count = bin_count + 1;
		response_msg.result = BINMGR_OK;
	} else {
		response_msg.result = BINMGR_NOT_FOUND;
	}

	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getinfo_all_response_t));
}
