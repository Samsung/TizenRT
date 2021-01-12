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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <debug.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_clear_binfile
 *
 * Description:
 *	 This function clears binary files with old version.
 *
 ****************************************************************************/
static int binary_manager_clear_binfile(int bin_idx)
{
	DIR *dirp;
	int name_len;
	char *bin_name;
	char filepath[BINARY_PATH_LEN];
	char running_file[NAME_MAX];

	snprintf(running_file, NAME_MAX, "%s_%d", BIN_NAME(bin_idx), BIN_LOADVER(bin_idx));
	bin_name = BIN_NAME(bin_idx);
	name_len = strlen(bin_name);

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
			/* Remove binary file which is not running */
			if (DIRENT_ISFILE(entryp->d_type) && !strncmp(entryp->d_name, bin_name, name_len) \
				&& entryp->d_name[name_len] == '_' && strncmp(entryp->d_name, running_file, strlen(running_file))) { 
				snprintf(filepath, BINARY_PATH_LEN, "%s/%s", BINARY_DIR_PATH, entryp->d_name);
				bmvdbg("unlink %s\n", entryp->d_name);
				unlink(filepath);
			}
		}
		closedir(dirp);
	} else if (errno != ENOENT) {
		bmdbg("Failed to open a directory, %s\n", BINARY_DIR_PATH);
		return BINMGR_OPERATION_FAIL;
	}

	return BINMGR_OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_scan_ubin_all
 *
 * Description:
 *	 This function scans all binary files in a directory for user binaries.
 *   And it registers them to binary table.
 *
 ****************************************************************************/
void binary_manager_scan_ubin_all(void)
{
	int ret;
	DIR *dirp;
	int bin_idx;
	binary_header_t header_data;
	char filepath[BINARY_PATH_LEN];

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
			/* Remove binary file which is not running */
			if (DIRENT_ISFILE(entryp->d_type)) {
				snprintf(filepath, BINARY_PATH_LEN, "%s/%s", BINARY_DIR_PATH, entryp->d_name);
				ret = binary_manager_read_header(filepath, &header_data, false);
				if (ret < 0) {
					continue;
				}
				bin_idx = binary_manager_get_index_with_name(header_data.bin_name);
				if (bin_idx < 0) {
					/* If binary is not registered, register it */
					(void)binary_manager_register_ubin(header_data.bin_name, header_data.bin_ver, header_data.loading_priority);
				} else {
					/* Already registered */
					BIN_FILECNT(bin_idx)++;
					BIN_VER(bin_idx, 1) = header_data.bin_ver;
					BIN_LOAD_PRIORITY(bin_idx, 1) = header_data.loading_priority;
					if (header_data.bin_ver > BIN_VER(bin_idx, 0)) {
						BIN_USEIDX(bin_idx) = 1;
					}
				}
			}
		}
		closedir(dirp);
	} else if (errno != ENOENT) {
		bmdbg("Failed to open a directory, %s\n", BINARY_DIR_PATH);
	}
}

/****************************************************************************
 * Name: binary_manager_scan_ubin
 *
 * Description:
 *	 This function scans binary files of binary with specific index.
 *
 ****************************************************************************/
int binary_manager_scan_ubin(int bin_idx)
{
	int ret;
	DIR *dirp;
	int file_idx;
	int name_len;
	int latest_ver;
	int latest_idx;
	char *bin_name;
	binary_header_t header_data;
	char filepath[BINARY_PATH_LEN];

	bmvdbg("Open a directory, %s\n", BINARY_DIR_PATH);

	/* Open a directory for user binaries, BINARY_DIR_PATH */
	dirp = opendir(BINARY_DIR_PATH);
	if (!dirp) {
		bmdbg("Failed to open a directory, %s\n", BINARY_DIR_PATH);
		return ERROR;
	}

	file_idx = 0;
	latest_ver = 0;
	latest_idx = -1;
	bin_name = BIN_NAME(bin_idx);
	name_len = strlen(bin_name);

	BIN_FILECNT(bin_idx) = 0;

	/* Read each directory entry */
	for (;;) {
		struct dirent *entryp = readdir(dirp);
		if (!entryp) {
			/* Finished with this directory */
			break;
		}
		/* Read and Verify a binary file */
		if (DIRENT_ISFILE(entryp->d_type) \
			&& !strncmp(entryp->d_name, bin_name, name_len) && entryp->d_name[name_len] == '_') {
			snprintf(filepath, BINARY_PATH_LEN, "%s/%s", BINARY_DIR_PATH, entryp->d_name);
			ret = binary_manager_read_header(filepath, &header_data, false);
			if (ret == OK) {
				BIN_FILECNT(bin_idx)++;
				BIN_VER(bin_idx, file_idx) = header_data.bin_ver;
				BIN_LOAD_PRIORITY(bin_idx, file_idx) = header_data.loading_priority;
				bmvdbg("Found valid header with version %d\n", header_data.bin_ver);
				if (header_data.bin_ver > latest_ver) {
					latest_ver = header_data.bin_ver;
					latest_idx = file_idx;
					BIN_USEIDX(bin_idx) = latest_idx;
					bmvdbg("Latest version %d, idx %d\n", latest_ver, latest_idx);
				}
			}
			file_idx++;
		}
	}
	closedir(dirp);

	if (BIN_FILECNT(bin_idx) <= 0) {
		bmdbg("Failed to find valid binary file, %s\n", BIN_NAME(bin_idx));
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: binary_manager_create_entry
 *
 * Description:
 *	 This function creates new empty file for requested binary name.
 *
 ****************************************************************************/
int binary_manager_create_entry(int requester_pid, char *bin_name, int version)
{
	int fd;
	int ret;
	int bin_idx;
	binmgr_kinfo_t *kerinfo;
	char q_name[BIN_PRIVMQ_LEN];
	char filepath[BINARY_PATH_LEN];
	binmgr_createbin_response_t response_msg;

	if (requester_pid < 0 || bin_name == NULL || version < 0) {
		bmdbg("Invalid data : pid %d name %s version %d\n", requester_pid, bin_name, version);
		response_msg.result = BINMGR_INVALID_PARAM;
		goto send_result;
	}

	/* If it is kernel, Return the devname of inactive kernel partition */
	if (!strncmp("kernel", bin_name, BIN_NAME_MAX)) {
		kerinfo = binary_manager_get_kdata();
		if (kerinfo->part_count > 1) {
			response_msg.result = BINMGR_OK;
			snprintf(response_msg.binpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kerinfo->part_info[kerinfo->inuse_idx ^ 1].part_num, BINARY_PATH_LEN);
		} else {
			response_msg.result = BINMGR_NOT_FOUND;
		}
		goto send_result;
	}

	/* Else, Create a new file for user binary and Return filepath */
	bin_idx = binary_manager_get_index_with_name(bin_name);
	if (bin_idx >= 0) {
		/* Check version */
		if (BIN_LOADVER(bin_idx) == version) {
			bmvdbg("Already existing version %d\n", version);
			response_msg.result = BINMGR_ALREADY_UPDATED;
			goto send_result;
		}
		/* Remove old binary files to get space in fs */
		ret = binary_manager_clear_binfile(bin_idx);
		if (ret < 0) {
			response_msg.result = ret;
			goto send_result;
		}
	} else {
		/* If it is not registered, register it */
		ret = binary_manager_register_ubin(bin_name, version, SCHED_PRIORITY_MIN);
		if (ret < 0) {
			response_msg.result = BINMGR_OPERATION_FAIL;
			goto send_result;
		}
	}
	response_msg.result = BINMGR_OPERATION_FAIL;

	/* Create a new file */
	snprintf(filepath, BINARY_PATH_LEN, "%s/%s_%d", BINARY_DIR_PATH, bin_name, version);
	fd = open(filepath, O_RDWR | O_CREAT, 0666);
	if (fd > 0) {
		bmvdbg("Created file '%s' for binary %s\n", filepath, bin_name);
		strncpy(response_msg.binpath, filepath, strlen(filepath) + 1);
		response_msg.result = BINMGR_OK;
		close(fd);
	} else if (errno == ENOENT) {
		/* A directory for binary is not existing, then Create it */
		ret = mkdir(BINARY_DIR_PATH, 0777);
		if (ret == OK) {
			fd = open(filepath, O_RDWR | O_CREAT, 0666);
			if (fd > 0) {
				bmvdbg("Created file '%s' for binary %s\n", filepath, bin_name);
				strncpy(response_msg.binpath, filepath, strlen(filepath) + 1);
				response_msg.result = BINMGR_OK;
				close(fd);
			}
		}
	}
send_result:
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);
	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_createbin_response_t));

	return response_msg.result;
}
