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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/statfs.h>

#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
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
		bmdbg("Invalid binary name, NULL\n");
		return ERROR;
	}

	bin_count = binary_manager_get_ucount();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name) + 1)) {
			bmvdbg("FIND binary %s idx = %d\n", bin_name, bin_idx);
			return bin_idx;
		}
	}

	return ERROR;
}

int binary_manager_get_available_size(int bin_idx)
{
	DIR *dirp;
	int size;
	int name_len;
	char *bin_name;
	struct stat file_buf;
	struct statfs fs_buf;
	char filepath[BINARY_PATH_LEN];
	char running_file[NAME_MAX];

	snprintf(running_file, NAME_MAX, "%s_%d", BIN_NAME(bin_idx), BIN_LOADVER(bin_idx));
	bin_name = BIN_NAME(bin_idx);
	name_len = strlen(bin_name);
	size = 0;

	/* Stat for Available bytes per block */
	if (stat(BINARY_MNT_PATH, &file_buf) != OK) {
		bmdbg("Failed to stat %s, errno %d\n", BINARY_MNT_PATH, errno);
		return ERROR;
	}

	/* Statfs for Free blocks */
	if (statfs(BINARY_MNT_PATH, &fs_buf) != OK) {
		bmdbg("Failed to stat fs %s, errno %d\n", BINARY_MNT_PATH, errno);
		return ERROR;
	}

	/* Get available size on file system */
	size = file_buf.st_blksize * fs_buf.f_bavail;

	/* Open a directory for user binaries, BINARY_DIR_PATH */
	dirp = opendir(BINARY_DIR_PATH);
	if (dirp) {
		/* Read each directory entry */
		for (;;) {
			struct dirent *entryp = readdir(dirp);
			if (!entryp) {
				/* Finished with this directory */
				break;
			}
			/* Calculate size of old binary files to be removed when creating new file */
			if (DIRENT_ISFILE(entryp->d_type) && !strncmp(entryp->d_name, bin_name, name_len) \
				&& entryp->d_name[name_len] == '_' && strncmp(entryp->d_name, running_file, strlen(running_file))) {
				snprintf(filepath, BINARY_PATH_LEN, "%s/%s", BINARY_DIR_PATH, entryp->d_name);
				if (stat(filepath, &file_buf) == OK) {
					bmvdbg("filepath %s size %d\n", filepath, file_buf.st_size);
					size += file_buf.st_size;
				}
			}
		}
		closedir(dirp);
	} else if (errno != ENOENT) {
		bmdbg("Failed to open a directory, %s\n", BINARY_DIR_PATH);
		return ERROR;
	}
	bmvdbg("Available size %d in fs \n", size);

	return size;
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
		bmdbg("Invalid data pid %d name %s\n", requester_pid, bin_name);
		return;
	}
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);

	memset((void *)&response_msg, 0, sizeof(binmgr_getstate_response_t));
	response_msg.result = BINMGR_NOT_FOUND;

	bin_count = binary_manager_get_ucount();
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name) + 1)) {
			response_msg.result = BINMGR_OK;
			response_msg.state = BIN_STATE(bin_idx);
			break;
		}
	}

	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getstate_response_t));
}

/****************************************************************************
 * Name: binary_manager_get_info_with_name
 *
 * Description:
 *	 This function gets binary info with binary name.
 *
 ****************************************************************************/
void binary_manager_get_info_with_name(int requester_pid, char *bin_name)
{
	int size;
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

	bin_count = binary_manager_get_ucount();
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, BIN_NAME_MAX)) {
			size = binary_manager_get_available_size(bin_idx);
			if (size < 0) {
				response_msg.result = BINMGR_OPERATION_FAIL;
			} else {
				response_msg.result = BINMGR_OK;
				response_msg.data.available_size = size;
				strncpy(response_msg.data.name, BIN_NAME(bin_idx) , BIN_NAME_MAX);
				response_msg.data.version = (double)BIN_LOADVER(bin_idx);
			}
			break;
		}
	}

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
	int size;
	int bin_idx;
	int result_idx;
	uint32_t bin_count;
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

	/* Kernel data */
	kerinfo = binary_manager_get_kdata();
	strncpy(response_msg.data.bin_info[result_idx].name, kerinfo->name , BIN_NAME_MAX);
	response_msg.data.bin_info[result_idx].version = kerinfo->version;
	if (kerinfo->part_count > 1) {
		response_msg.data.bin_info[result_idx].available_size = kerinfo->part_info[kerinfo->inuse_idx ^ 1].part_size;
	}
	result_idx++;

	/* User binaries data */
	bin_count = binary_manager_get_ucount();
	if (bin_count > 0) {
		for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
			size = binary_manager_get_available_size(bin_idx);
			if (size < 0) {
				response_msg.result = BINMGR_OPERATION_FAIL;
				break;
			}
			response_msg.result = BINMGR_OK;
			response_msg.data.bin_info[result_idx].available_size = size;
			strncpy(response_msg.data.bin_info[result_idx].name, BIN_NAME(bin_idx) , BIN_NAME_MAX);
			response_msg.data.bin_info[result_idx].version = (double)BIN_LOADVER(bin_idx);
			result_idx++;
		}
	}

	if (response_msg.result == BINMGR_OK) {
		response_msg.data.bin_count = bin_count + 1;
	}

	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_getinfo_all_response_t));
}
