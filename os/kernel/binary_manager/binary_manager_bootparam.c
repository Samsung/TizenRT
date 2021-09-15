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
		bmdbg("ERROR: Invalid part info : num %d, size %d\n", part_num, part_size);
		return;
	}

	g_bp_info.part_num = part_num;

	bmvdbg("[BOOTPARAM] part num %d\n", part_num);
}

bool is_valid_bootparam(binmgr_bpdata_t *bp_data)
{
	int bpdata_size;

	if (!bp_data) {
		bmdbg("Boot param is NULL\n");
		return false;
	} else if (bp_data->format_ver == 0 || bp_data->active_idx >= binary_manager_get_kdata()->part_count) {
		bmdbg("Invalid data. ver: %u, active index: %u, addresses: %x, %x\n", bp_data->version, bp_data->active_idx, bp_data->address[0], bp_data->address[1]);
		return false;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
#if 0 //FIX after bootloader modification
	bpdata_size = sizeof(binmgr_bpdata_t) - CHECKSUM_SIZE - (((CONFIG_NUM_APPS + 1 - bp_data->app_count) * sizeof(binmgr_userbp_t)) + 1);
#endif
	bpdata_size = sizeof(binmgr_bpdata_t) - CHECKSUM_SIZE - (sizeof(bp_data->app_data) + 1);
#else
	bpdata_size = sizeof(binmgr_bpdata_t) - CHECKSUM_SIZE;
#endif
	if (bp_data->crc_hash != crc32((uint8_t *)bp_data + CHECKSUM_SIZE, bpdata_size)) {
		bmdbg("Invalid crc32 value, crc32 %u, ver: %u, active index: %u, addresses: %x, %x\n", bp_data->crc_hash, bp_data->version, bp_data->active_idx, bp_data->address[0], bp_data->address[1]);
		return false;
	}

	return true;
}

static int binary_manager_open_bootparam(void)
{
	int fd;
	char bp_devpath[BINARY_PATH_LEN];

	if (g_bp_info.part_num < 0) {
		bmdbg("Invalid Bootparam partition Num : %d\n", g_bp_info.part_num);
		return BINMGR_INVALID_PARAM;
	}

	/* Open dev to access boot parameters */
	snprintf(bp_devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, g_bp_info.part_num);
	fd = open(bp_devpath, O_RDWR, 0666);
	if (fd < 0) {
		bmdbg("ERROR: Get a fd of bootparam, errno %d\n", errno);
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
	int latest_idx = -1;
	uint32_t latest_ver = 0;
	binmgr_bpdata_t bp_data[BOOTPARAM_COUNT];

	if (bp_info == NULL) {
		bmdbg("Invalid input bp_info\n");
		return BINMGR_INVALID_PARAM;
	}

	fd = binary_manager_open_bootparam();
	if (fd < 0) {
		return BINMGR_OPERATION_FAIL;
	}

	for (bp_idx = 0; bp_idx < BOOTPARAM_COUNT; bp_idx++) {
		ret = lseek(fd, BP_SEEK_OFFSET(bp_idx), SEEK_SET);
		if (ret < 0) {
			bmdbg("ERROR: Seek Failed, errno %d\n", errno);
			close(fd);
			return BINMGR_OPERATION_FAIL;
		}
		/* Read bootparam data */
		ret = read(fd, (FAR uint8_t *)&bp_data[bp_idx], sizeof(binmgr_bpdata_t));
		if (ret != sizeof(binmgr_bpdata_t)) {
			continue;
		} else if (!is_valid_bootparam(&bp_data[bp_idx])) {
			continue;
		}

		/* Update the latest version and index */
		if (latest_ver < bp_data[bp_idx].version) {
			latest_ver = bp_data[bp_idx].version;
			latest_idx = bp_idx;
			bmvdbg("Latest Version %d, index %d\n", latest_ver, latest_idx);
		}
	}
	close(fd);

	if (latest_ver == 0 || latest_idx < 0) {
		bmdbg("Failed to find valid bootparam\n");
		return BINMGR_NOT_FOUND;
	}

	bp_info->inuse_idx = latest_idx;
	bp_info->bp_data = bp_data[latest_idx];

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
		bmvdbg("Bootparam[%d] ver: %u, active index: %u, addresses: %x, %x\n", g_bp_info.inuse_idx, g_bp_info.bp_data.version, g_bp_info.bp_data.active_idx, g_bp_info.bp_data.address[0], g_bp_info.bp_data.address[1]);
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
int binary_manager_write_bootparam(binmgr_bpdata_t *bp_data)
{
	int fd;
	int ret;	
	int bpdata_size;
	uint8_t inuse_idx;

	if (!bp_data) {
		bmdbg("ERROR: Input bp data is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	if (g_bp_info.inuse_idx >= BOOTPARAM_COUNT) {
		bmdbg("ERROR: Invalid bootparam data, inuse idx %u\n", g_bp_info.inuse_idx);
		return BINMGR_INVALID_PARAM;
	}

	fd = binary_manager_open_bootparam();
	if (fd < 0) {
		return BINMGR_OPERATION_FAIL;
	}

	/* Update bootparam data : CRC */
#ifdef CONFIG_APP_BINARY_SEPARATION
#if 0 //FIX after bootloader modification
	bpdata_size = sizeof(binmgr_bpdata_t) - CHECKSUM_SIZE - (((CONFIG_NUM_APPS + 1 - bp_data->app_count) * sizeof(binmgr_userbp_t)) + 1);
#endif
	bpdata_size = sizeof(binmgr_bpdata_t) - CHECKSUM_SIZE - (sizeof(bp_data->app_data) + 1);
#else
	bpdata_size = sizeof(binmgr_bpdata_t) - CHECKSUM_SIZE;
#endif
	bp_data->crc_hash = crc32((uint8_t *)bp_data + CHECKSUM_SIZE, bpdata_size);
	inuse_idx = g_bp_info.inuse_idx ^ 1;

	ret = lseek(fd, BP_SEEK_OFFSET(inuse_idx), SEEK_SET);
	if (ret < 0) {
		bmdbg("ERROR: Seek Failed, errno %d\n", errno);
		goto errout_with_fd;
	}

	/* Write bootparam data */
	ret = write(fd, (FAR uint8_t *)bp_data, sizeof(binmgr_bpdata_t));
	if (ret != sizeof(binmgr_bpdata_t)) {
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
	int bin_idx;
	bool need_update;
	bool check_update;
	uint32_t bin_count;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_bpdata_t update_bp_data;
	binmgr_setbp_response_t response_msg;

	memset((void *)&response_msg, 0, sizeof(binmgr_setbp_response_t));

	if (requester_pid < 0) {
		bmdbg("Invalid requester pid %d\n", requester_pid);
		return;
	}

	/* Get current bootparam data and update version */
	memcpy(&update_bp_data, binary_manager_get_bpdata(), sizeof(binmgr_bpdata_t));
	update_bp_data.version++;

	if (BM_CHECK_GROUP(type, BINARY_KERNEL)) {
		/* Update bootparam and Reboot if new kernel binary exists */
		ret = binary_manager_check_kernel_update(&check_update);
		if (ret != BINMGR_OK) {
			bmdbg("Failed to check kernel update, %d\n", ret);
			goto send_response;
		} else if (!check_update) {
			bmdbg("No binary to update\n");
			goto send_response;
		}		
		/* Update index for inactive partition */
		update_bp_data.active_idx ^= 1;
	}

	need_update = false;

	if (BM_CHECK_GROUP(type, BINARY_USERAPP)) {
		bin_count = binary_manager_get_ucount();
		/* Reload binaries if new binary is scanned */
		for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
			/* Scan binary files */
			ret = binary_manager_check_user_update(bin_idx, &check_update);
			if (ret == OK && check_update) {
				/* Update index for inactive partition */
				update_bp_data.app_data[BIN_BPIDX(bin_idx)].useidx ^= 1;
				need_update = true;
			}
		}
		if (!need_update) {
			bmdbg("No binary to update\n");
			goto send_response;
		}
	}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (BM_CHECK_GROUP(type, BINARY_COMMON)) {
		ret = binary_manager_check_user_update(BM_CMNLIB_IDX, &check_update);
		if (ret != BINMGR_OK) {
			bmdbg("Failed to check kernel update, %d\n", ret);
			goto send_response;
		} else if (!check_update) {
			bmdbg("No binary to update\n");
			goto send_response;
		}		
		/* Update index for inactive partition */
		update_bp_data.app_data[BIN_BPIDX(BM_CMNLIB_IDX)].useidx ^= 1;
	}
#endif

	/* Then, Write bootparam with updated bootparam data */
	ret = binary_manager_write_bootparam(&update_bp_data);
	if (ret == BINMGR_OK) {
		bmvdbg("Update bootparam SUCCESS\n");
	} else {
		bmdbg("Failed to update bootparam, %d\n", ret);
	}

send_response:
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
