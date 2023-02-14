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
#include <string.h>
#ifdef CONFIG_APP_BINARY_SEPARATION
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#endif

#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_APP_BINARY_SEPARATION
/****************************************************************************
 * Name: binary_manager_get_index_with_name
 *
 * Description:
 *	 This function gets index in binary table with binary id.
 *
 ****************************************************************************/
int binary_manager_get_index_with_name(char *bin_name)
{
	int bin_idx;
	uint32_t bin_count;

	if (bin_name == NULL) {
		bmdbg("binary name is NULL\n");
		return ERROR;
	}

	bin_count = binary_manager_get_ucount();

	for (bin_idx = 0; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name) + 1)) {
			bmvdbg("FIND binary %s idx = %d\n", bin_name, bin_idx);
			return bin_idx;
		}
	}

	return ERROR;
}

/****************************************************************************
 * Name: binary_manager_get_state_with_name
 *
 * Description:
 *	 This function gets binary state with binary name.
 *
 ****************************************************************************/
void binary_manager_get_state_with_name(int requester_pid, char *bin_name)
{
	int bin_idx;
	uint32_t bin_count;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_getstate_response_t response_msg;

	if (requester_pid < 0 || bin_name == NULL) {
		bmdbg("Invalid pid %d name %s\n", requester_pid, bin_name);
		return;
	}
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);

	memset((void *)&response_msg, 0, sizeof(binmgr_getstate_response_t));
	response_msg.result = BINMGR_NOT_FOUND;

	bin_count = binary_manager_get_ucount();
	for (bin_idx = 0; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name) + 1)) {
			response_msg.result = BINMGR_OK;
			response_msg.state = BIN_STATE(bin_idx);
			break;
		}
	}

	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getstate_response_t));
}
#endif

/****************************************************************************
 * Name: binary_manager_get_info_with_name
 *
 * Description:
 *	 This function gets binary info with binary name.
 *
 ****************************************************************************/
void binary_manager_get_info_with_name(int requester_pid, char *bin_name)
{
#ifdef CONFIG_APP_BINARY_SEPARATION
	int bin_idx;
	uint32_t bin_count;
#endif
	binmgr_kinfo_t *kerinfo;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_getinfo_response_t response_msg;

	if (requester_pid < 0 || bin_name == NULL) {
		bmdbg("Invalid pid %d name %s\n", requester_pid, bin_name);
		return;
	}
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);

	memset((void *)&response_msg, 0, sizeof(binmgr_getinfo_response_t));
	response_msg.result = BINMGR_NOT_FOUND;

	if (!strncmp("kernel", bin_name, BIN_NAME_MAX)) {
		kerinfo = binary_manager_get_kdata();
		strncpy(response_msg.data.name, "kernel", BIN_NAME_MAX);
		response_msg.data.version = kerinfo->version;
		if (kerinfo->part_count > 1) {
			response_msg.data.available_size = kerinfo->part_info[kerinfo->inuse_idx ^ 1].size;
		} else {
			response_msg.data.available_size = -1;
		}
		response_msg.result = BINMGR_OK;
	}
#ifdef CONFIG_APP_BINARY_SEPARATION
	else {
		bin_count = binary_manager_get_ucount();
		for (bin_idx = 0; bin_idx <= bin_count; bin_idx++) {
			if (!strncmp(BIN_NAME(bin_idx), bin_name, BIN_NAME_MAX)) {
				response_msg.result = BINMGR_OK;
				response_msg.data.available_size = BIN_PARTSIZE(bin_idx, (BIN_USEIDX(bin_idx) ^ 1));
				strncpy(response_msg.data.name, BIN_NAME(bin_idx) , BIN_NAME_MAX);
				response_msg.data.version = BIN_VER(bin_idx, BIN_USEIDX(bin_idx));
				break;
			}
		}
	}
#endif
	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getinfo_response_t));
}

/****************************************************************************
 * Name: binary_manager_get_info_all
 *
 * Description:
 *	 This function gets info of all registered binaries.
 *
 ****************************************************************************/
void binary_manager_get_info_all(int requester_pid)
{
#ifdef CONFIG_APP_BINARY_SEPARATION
	int bin_idx;	
	uint32_t bin_count;
#endif
	int result_idx;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_kinfo_t *kerinfo;
	binmgr_getinfo_all_response_t response_msg;

	if (requester_pid < 0) {
		bmdbg("Invalid requester pid %d\n", requester_pid);
		return;
	}
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);

	result_idx = 0;
	memset((void *)&response_msg, 0, sizeof(binmgr_getinfo_all_response_t));
	response_msg.result = BINMGR_OK;

	/* Get kernel data */
	kerinfo = binary_manager_get_kdata();
	strncpy(response_msg.data.bin_info[result_idx].name, "kernel", BIN_NAME_MAX);
	response_msg.data.bin_info[result_idx].version = kerinfo->version;
	if (kerinfo->part_count > 1) {
		response_msg.data.bin_info[result_idx].available_size = kerinfo->part_info[kerinfo->inuse_idx ^ 1].size;
	} else {
		response_msg.data.bin_info[result_idx].available_size = -1;
	}
	result_idx++;

#ifdef CONFIG_APP_BINARY_SEPARATION
	/* User binaries data */
	bin_count = binary_manager_get_ucount();
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	for (bin_idx = 0; bin_idx <= bin_count; bin_idx++) {
#else
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
#endif
		response_msg.result = BINMGR_OK;
		response_msg.data.bin_info[result_idx].available_size = BIN_PARTSIZE(bin_idx, (BIN_USEIDX(bin_idx) ^ 1));
		strncpy(response_msg.data.bin_info[result_idx].name, BIN_NAME(bin_idx) , BIN_NAME_MAX);
		response_msg.data.bin_info[result_idx].version = BIN_VER(bin_idx, BIN_USEIDX(bin_idx));
		result_idx++;
	}
#endif
	if (response_msg.result == BINMGR_OK) {
		response_msg.data.bin_count = result_idx;
	}

	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getinfo_all_response_t));
}

static int binary_manager_get_path(int requester_pid, char *bin_name, bool inactive)
{
#ifdef CONFIG_APP_BINARY_SEPARATION
	int bin_idx;
#endif
	binmgr_kinfo_t *kerinfo;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_getpath_response_t response_msg;

	if (requester_pid < 0 || bin_name == NULL) {
		bmdbg("Invalid param : pid %d name %s \n", requester_pid, bin_name);
		response_msg.result = BINMGR_INVALID_PARAM;
		goto send_result;
	}

	/* If it is kernel, Return the devname of inactive kernel partition */
	if (!strncmp("kernel", bin_name, BIN_NAME_MAX)) {
		kerinfo = binary_manager_get_kdata();
		if (kerinfo->part_count > 1) {
			response_msg.result = BINMGR_OK;
			snprintf(response_msg.binpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kerinfo->part_info[kerinfo->inuse_idx ^ inactive].devnum);
		} else {
			response_msg.result = BINMGR_NOT_FOUND;
		}
		goto send_result;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	bin_idx = binary_manager_get_index_with_name(bin_name);
	if (bin_idx < 0) {
		bmdbg("Binary %s is not registered.\n", bin_name);
		response_msg.result = BINMGR_NOT_FOUND;
	} else if (BIN_PARTSIZE(bin_idx, (BIN_USEIDX(bin_idx) ^ 1)) == 0) {
		bmvdbg("Partition to download binary is not existing\n");
		response_msg.result = BINMGR_OPERATION_FAIL;
	} else {
		/* Return devpath for partition to download binary on success */
		response_msg.result = BINMGR_OK;
		snprintf(response_msg.binpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx) ^ inactive)));
	}
#endif

send_result:
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);
	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getpath_response_t));

	return response_msg.result;
}

int binary_manager_get_active_path(int requester_pid, char *bin_name)
{
	return binary_manager_get_path(requester_pid, bin_name, false);
}

int binary_manager_get_inactive_path(int requester_pid, char *bin_name)
{
	return binary_manager_get_path(requester_pid, bin_name, true);
}
