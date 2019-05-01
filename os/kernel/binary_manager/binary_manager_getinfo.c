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
#include <mqueue.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
extern int g_bin_count;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/* Get index in binary table with binary id */
int binary_manager_get_index_with_binid(int bin_id)
{
	int bin_idx;
	int bin_count;

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
	int bin_count;

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
int binary_manager_get_info_with_name(char *bin_name, char *q_name)
{
	int bin_idx;
	mqd_t private_mq;
	binmgr_response_t response_msg;

	struct mq_attr attr;
	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(binmgr_response_t);
	attr.mq_flags = 0;

	private_mq = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);

	if (private_mq == (mqd_t)ERROR) {
		bmdbg("mq_open failed! %d\n", errno);
		return ERROR;
	}

	memset((void *)&response_msg, 0, sizeof(binmgr_response_t));
	response_msg.cmd = BINMGR_RESPONSE_INVALID;

	for (bin_idx = 0; bin_idx < g_bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, BIN_NAME_MAX)) {
			response_msg.cmd = BINMGR_RESPONSE_DONE;
			response_msg.part_size = BIN_PARTSIZE(bin_idx);
			strncpy(response_msg.name, BIN_NAME(bin_idx) , BIN_NAME_MAX);
			strncpy(response_msg.version, BIN_VER(bin_idx), BIN_VER_MAX);
			snprintf(response_msg.dev_path, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, (BIN_USEIDX(bin_idx) ^ 1)));
			break;
		}
	}

	if (mq_send(private_mq, (const char *)&response_msg, sizeof(binmgr_response_t), BINMGR_NORMAL_PRIO) == ERROR) {
		bmdbg("send ERROR errno %d\n", errno);
		mq_close(private_mq);
		return ERROR;
	}

	mq_close(private_mq);
	return OK;
}

/* Get info of all registered binaries */
int binary_manager_get_info_all(char *q_name)
{
	mqd_t private_mq;
	binmgr_response_t response_msg;

	struct mq_attr attr;
	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(response_msg);
	attr.mq_flags = 0;

	private_mq = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);

	memset((void *)&response_msg, 0, sizeof(response_msg));

	response_msg.cmd = BINMGR_RESPONSE_CONTINUE;
	for (int index = 0; index < g_bin_count; ++index) {
		if (index == g_bin_count - 1) {
			response_msg.cmd = BINMGR_RESPONSE_DONE;
		}

		strncpy(response_msg.name, BIN_NAME(index), BIN_NAME_MAX);
		response_msg.part_size = BIN_PARTSIZE(index);
		strncpy(response_msg.version, BIN_VER(index), BIN_VER_MAX);
		snprintf(response_msg.dev_path, BINMGR_DEVNAME_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(index, (BIN_USEIDX(index) ^ 1)));
		
		if (mq_send(private_mq, (const char *)&response_msg, sizeof(response_msg), 0) == ERROR) {
			bmdbg("send ERROR errno %d\n", errno);
			mq_close(private_mq);
			return ERROR;
		}
	}

	mq_close(private_mq);
	return OK;
}
