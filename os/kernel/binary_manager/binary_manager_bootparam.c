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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#include <tinyara/binary_manager.h>
#include <tinyara/common_logs/common_logs.h>

#include "binary_manager.h"

/* Data for Boot parameters */
static binmgr_bpinfo_t g_bp_info;

#define BP_SEEK_OFFSET(index)   (index == 0 ? 0 : BOOTPARAM_PARTSIZE / 2)

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_register_bppart
 *
 * Description:
 *	 This function registers the partition of boot parameters.
 *
 ****************************************************************************/
void binary_manager_register_bppart(int part_num, int part_size)
{
	if (part_num < 0 || part_size != BOOTPARAM_PARTSIZE) {
		bmdbg("%s BP partition : num %d, size %d\n", clog_message_str[CMN_LOG_INVALID_VAL], part_num, part_size);
		return;
	}

	g_bp_info.part_num = part_num;

	bmvdbg("[BOOTPARAM] part num %d\n", part_num);
}

bool is_valid_bootparam(char *bootparam)
{
	binmgr_bpdata_t *bp_data = (binmgr_bpdata_t *)bootparam;

	if (!bp_data) {
		bmdbg("%s bootparam\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
		return false;
	} else if (bp_data->format_ver == 0 || bp_data->active_idx >= binary_manager_get_kdata()->part_count) {
		bmdbg("%s ver: %u, active index: %u, addresses: %x, %x\n", clog_message_str[CMN_LOG_INVALID_VAL], bp_data->version, bp_data->active_idx, bp_data->address[0], bp_data->address[1]);
		return false;
	}

	if (bp_data->crc_hash != crc32((uint8_t *)bootparam + CHECKSUM_SIZE, BOOTPARAM_SIZE - CHECKSUM_SIZE)) {
		bmdbg("%s crc32, crc32 %u, ver: %u, active index: %u, addresses: %x, %x\n", clog_message_str[CMN_LOG_INVALID_VAL], bp_data->crc_hash, bp_data->version, bp_data->active_idx, bp_data->address[0], bp_data->address[1]);
		return false;
	}

	return true;
}

static int binary_manager_open_bootparam(void)
{
	int fd;
	char bp_devpath[BINARY_PATH_LEN];

	if (g_bp_info.part_num < 0) {
		bmdbg("%s BP partition Num : %d\n", clog_message_str[CMN_LOG_INVALID_VAL], g_bp_info.part_num);
		return BINMGR_INVALID_PARAM;
	}

	/* Open dev to access boot parameters */
	snprintf(bp_devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, g_bp_info.part_num);
	fd = open(bp_devpath, O_RDWR, 0666);
	if (fd < 0) {
		bmdbg("%s bp, errno %d\n", clog_message_str[CMN_LOG_FILE_OPEN_ERROR], errno);
		return BINMGR_OPERATION_FAIL;
	}
	return fd;
}

/*****************************************************************************************************
 * Name: binary_manager_scan_bootparam
 *
 * Description:
 *	 This function checks the latest boot parameter information by scanning all boot parameters.
 *
 *****************************************************************************************************/
int binary_manager_scan_bootparam(binmgr_bpinfo_t *bp_info)
{
	int fd;
	int ret;
	int bp_idx;
	uint32_t latest_ver = 0;
	char *bootparam;

	if (bp_info == NULL) {
		bmdbg("%s bpinfo\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
		return BINMGR_INVALID_PARAM;
	}

	fd = binary_manager_open_bootparam();
	if (fd < 0) {
		return BINMGR_OPERATION_FAIL;
	}

	bootparam = (char *)kmm_malloc(BOOTPARAM_SIZE);
	if (!bootparam) {
		close(fd);
		bmdbg("%s BP\n", clog_message_str[CMN_LOG_ALLOC_FAIL]);
		return BINMGR_OUT_OF_MEMORY;
	}

	for (bp_idx = 0; bp_idx < BOOTPARAM_COUNT; bp_idx++) {
		bmdbg("Checking BP%d start\n", bp_idx);
		ret = lseek(fd, BP_SEEK_OFFSET(bp_idx), SEEK_SET);
		if (ret < 0) {
			bmdbg("%s BP, offset %d errno %d\n", clog_message_str[CMN_LOG_FILE_SEEK_ERROR], BP_SEEK_OFFSET(bp_idx), errno);
			kmm_free(bootparam);
			close(fd);
			return BINMGR_OPERATION_FAIL;
		}

		/* Read bootparam data */
		ret = read(fd, (FAR uint8_t *)bootparam, BOOTPARAM_SIZE);
		if (ret != BOOTPARAM_SIZE) {
			bmdbg("%s BP, errno %d\n", clog_message_str[CMN_LOG_FILE_READ_ERROR], errno);
			continue;
		} else if (!is_valid_bootparam(bootparam)) {
			bmdbg("%s BP%d\n", clog_message_str[CMN_LOG_INVALID_DATA], bp_idx);
			continue;
		}

		bmdbg("%s BP%d\n", clog_message_str[CMN_LOG_VALID_DATA], bp_idx);

		/* Update the latest version and index */
		if (latest_ver < ((binmgr_bpdata_t *)bootparam)->version) {
			latest_ver = ((binmgr_bpdata_t *)bootparam)->version;
			/* Update bootparam data */
			bp_info->inuse_idx = bp_idx;
			memcpy(&bp_info->bp_data, bootparam, sizeof(binmgr_bpdata_t));
		}
	}
	close(fd);
	kmm_free(bootparam);

	if (latest_ver == 0) {
		bmdbg("Fail to find valid BP\n");
		return BINMGR_NOT_FOUND;
	}
	bmdbg("BP USE index %d, version %d\n", bp_info->inuse_idx, latest_ver);

	return BINMGR_OK;
}

/******************************************************************************
* Name: binary_manager_update_bpinfo
*
* Description:
*	 This function saves the latest boot parameter information to g_bp_info.
*
******************************************************************************/
int binary_manager_update_bpinfo(void)
{
	int ret;
	binmgr_bpinfo_t bp_info;

	ret = binary_manager_scan_bootparam(&bp_info);
	if (ret == BINMGR_OK) {	
		/* Set scanned bootparam data to g_bp_info */
		g_bp_info.inuse_idx = bp_info.inuse_idx;
		g_bp_info.bp_data = bp_info.bp_data;
		bmvdbg("BP[%d] ver: %u, active index: %u, addresses: %x, %x\n", g_bp_info.inuse_idx, g_bp_info.bp_data.version, g_bp_info.bp_data.active_idx, g_bp_info.bp_data.address[0], g_bp_info.bp_data.address[1]);
	}

	return ret;
}

/*********************************************************************************
* Name: binary_manager_update_bootparam
*
* Description:
*	 This function updates input bootparam data, bp_data to inactive bootparam partition.
*
********************************************************************************/
int binary_manager_write_bootparam(char *bootparam)
{
	int fd;
	int ret;	
	uint8_t inuse_idx;

	if (!bootparam) {
		bmdbg("ERROR: Input bp data is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	if (g_bp_info.inuse_idx >= BOOTPARAM_COUNT) {
		bmdbg("ERROR: Invalid g_bp_info, inuse idx %u\n", g_bp_info.inuse_idx);
		return BINMGR_INVALID_PARAM;
	}

	fd = binary_manager_open_bootparam();
	if (fd < 0) {
		return BINMGR_OPERATION_FAIL;
	}

	/* Update bootparam data : CRC */
	((binmgr_bpdata_t *)bootparam)->crc_hash = crc32((uint8_t *)bootparam + CHECKSUM_SIZE, BOOTPARAM_SIZE - CHECKSUM_SIZE);
	inuse_idx = g_bp_info.inuse_idx ^ 1;

	ret = lseek(fd, BP_SEEK_OFFSET(inuse_idx), SEEK_SET);
	if (ret < 0) {
		bmdbg("ERROR: Seek Failed, errno %d\n", errno);
		goto errout_with_fd;
	}

	/* Write bootparam data */
	ret = write(fd, (FAR uint8_t *)bootparam, BOOTPARAM_SIZE);
	if (ret != BOOTPARAM_SIZE) {
		bmdbg("ERROR: Write Failed, errno %d\n", errno);
		goto errout_with_fd;
	}
	close(fd);

	return BINMGR_OK;
errout_with_fd:
	close(fd);
	return BINMGR_OPERATION_FAIL;
}

void binary_manager_update_bootparam(int requester_pid, uint8_t type)
{
	int ret;
	char *bootparam;
	bool is_all_updatable;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_bpdata_t update_bp_data;
	binmgr_setbp_response_t response_msg;
#ifdef CONFIG_APP_BINARY_SEPARATION
	int bin_idx;
	bool need_update;
	uint32_t bin_count;
#endif

	memset((void *)&response_msg, 0, sizeof(binmgr_setbp_response_t));

	if (requester_pid < 0) {
		bmdbg("Invalid requester pid %d\n", requester_pid);
		return;
	}

	bootparam = (char *)kmm_malloc(BOOTPARAM_SIZE);
	if (!bootparam) {
		bmdbg("Fail to malloc to read BP\n");
		ret = BINMGR_OUT_OF_MEMORY;
		goto send_response;
	}
	memset(bootparam, 0xff, BOOTPARAM_SIZE);

	response_msg.result = BINMGR_OK;
	is_all_updatable = true;

	/* Get current bootparam data and update version */
	memcpy(&update_bp_data, binary_manager_get_bpdata(), sizeof(binmgr_bpdata_t));
	update_bp_data.version++;

	if (BM_CHECK_GROUP(type, BINARY_KERNEL)) {
		/* Update bootparam and Reboot if new kernel binary exists */
		ret = binary_manager_check_kernel_update();
		if (ret == BINMGR_OK) {
			/* Update index for inactive partition */
			update_bp_data.active_idx ^= 1;
		} else if (ret == BINMGR_ALREADY_UPDATED || ret == BINMGR_NOT_FOUND) {
			bmdbg("No kernel binary to update\n");
			is_all_updatable = false;
			response_msg.data.result[BINARY_KERNEL] = BINMGR_ALREADY_UPDATED;
		} else {
			bmdbg("Fail to check kernel update, %d\n", ret);
			goto send_response;
		}
	}

#ifdef CONFIG_RESOURCE_FS
	if (BM_CHECK_GROUP(type, BINARY_RESOURCE)) {
		/* Update bootparam if new resource binary exists */
		ret = binary_manager_check_resource_update();
		if (ret == BINMGR_OK) {
			/* Update index for inactive partition */
			update_bp_data.resource_active_idx ^= 1;
		} else if (ret == BINMGR_ALREADY_UPDATED || ret == BINMGR_NOT_FOUND) {
			bmdbg("No resource binary to update\n");
			is_all_updatable = false;
			response_msg.data.result[BINARY_RESOURCE] = BINMGR_ALREADY_UPDATED;
		} else {
			bmdbg("Fail to check resource update, %d\n", ret);
			goto send_response;
		}
	}
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
	need_update = false;

	if (BM_CHECK_GROUP(type, BINARY_USERAPP)) {
		bin_count = binary_manager_get_ucount();
		/* Reload binaries if new binary is scanned */
		for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
			/* Scan binary files */
			ret = binary_manager_check_user_update(bin_idx);
			if (ret == BINMGR_OK) {
				/* Update index for inactive partition */
				update_bp_data.app_data[BIN_BPIDX(bin_idx)].useidx ^= 1;
				need_update = true;
			} else if (ret == BINMGR_ALREADY_UPDATED || ret == BINMGR_NOT_FOUND) {
				bmdbg("No user binary to update: bin_idx %d, ret %d\n", bin_idx, ret);
			} else {
				bmdbg("Fail to check user update: bin_idx %d, ret %d\n", bin_idx, ret);
				goto send_response;
			}
		}
		if (!need_update) {
			bmdbg("No App binaries to update\n");
			is_all_updatable = false;
			response_msg.data.result[BINARY_USERAPP] = BINMGR_ALREADY_UPDATED;
		}
	}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (BM_CHECK_GROUP(type, BINARY_COMMON)) {
		ret = binary_manager_check_user_update(BM_CMNLIB_IDX);
		if (ret == BINMGR_OK) {
			/* Update index for inactive partition */
			update_bp_data.app_data[BIN_BPIDX(BM_CMNLIB_IDX)].useidx ^= 1;
		} else if (ret == BINMGR_ALREADY_UPDATED || ret == BINMGR_NOT_FOUND) {
			bmdbg("No common binary to update\n");
			is_all_updatable = false;
			response_msg.data.result[BINARY_COMMON] = BINMGR_ALREADY_UPDATED;
		} else {
			bmdbg("Fail to check common update, %d\n", ret);
			goto send_response;
		}
	}
#endif
#endif

	if (is_all_updatable) {
		/* Then, Write bootparam with updated bootparam data */
		memcpy(bootparam, &update_bp_data, sizeof(binmgr_bpdata_t));
		ret = binary_manager_write_bootparam(bootparam);
		if (ret == BINMGR_OK) {
			bmvdbg("Update BP SUCCESS\n");
		} else {
			bmdbg("Fail to update BP, %d\n", ret);
		}
	} else {
		ret = BINMGR_ALREADY_UPDATED;
		bmdbg("Fail to update BP because some binaries are not updated\n");
	}

send_response:
	kmm_free(bootparam);
	response_msg.result = ret;
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);
	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_setbp_response_t));
}

/****************************************************************************
 * Name: binary_manager_get_bpdata
 *
 * Description:
 *	 This function gets boot parameter data.
 *
 ****************************************************************************/
binmgr_bpdata_t *binary_manager_get_bpdata(void)
{
	return &g_bp_info.bp_data;
}

/****************************************************************************
 * Name: binary_manager_set_bpdata
 *
 * Description:
 *	 This function set boot parameter data to g_bp_info.
 *
 ****************************************************************************/
int binary_manager_set_bpdata(binmgr_bpdata_t *bp_data)
{
	if (bp_data == NULL) {
		return ERROR;
	}
	memcpy(&g_bp_info.bp_data, bp_data, sizeof(binmgr_bpdata_t));

	return OK;
}

/****************************************************************************
 * Name: binary_manager_set_bp_index
 *
 * Description:
 *	 This function set boot parameter index to g_bp_info.
 *
 ****************************************************************************/
void binary_manager_set_bpidx(uint8_t index)
{
	g_bp_info.inuse_idx = index;
}
