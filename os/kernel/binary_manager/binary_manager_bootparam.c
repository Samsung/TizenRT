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
#include <stdint.h>
#include <tinyara/config.h>
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#include <tinyara/binary_manager.h>
#include <tinyara/reboot_reason.h>

#include "binary_manager_internal.h"

/* Data for Boot parameters */
static binmgr_bpinfo_t g_bp_info;

#define BP_SEEK_OFFSET(index)   (index == 0 ? 0 : BOOTPARAM_PARTSIZE / 2)
#define FACTORY_KBIN_VERSION    101

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
		bmdbg("Invalid BP partition : num %d, size %d\n", part_num, part_size);
		return;
	}

	g_bp_info.part_num = part_num;

	bmvdbg("[BOOTPARAM] part num %d\n", part_num);
}

bool is_valid_bootparam(char *bootparam)
{
	binmgr_bpdata_head_t *bp_head = (binmgr_bpdata_head_t *)bootparam;
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint32_t max_app_count;
#endif

	if (!bp_head) {
		bmdbg("Boot param is NULL\n");
		return false;
	} else if ((bp_head->format_ver < BOOTPARAM_FORMAT_VERSION_1 || bp_head->format_ver > BOOTPARAM_FORMAT_VERSION_LATEST) || bp_head->active_idx >= binary_manager_get_kdata()->part_count) {
		bmdbg("Invalid data. ver: %u, active index: %u, addresses: %x, %x\n", bp_head->version, bp_head->active_idx, bp_head->address[0], bp_head->address[1]);
		return false;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	max_app_count = sizeof(bp_head->app_data) / sizeof(bp_head->app_data[0]);
	if (bp_head->app_count > max_app_count) {
		bmdbg("Invalid app count %u, max %u\n", bp_head->app_count, max_app_count);
		return false;
	}
#endif

	if (bp_head->crc_hash != crc32((uint8_t *)bootparam + CHECKSUM_SIZE, BOOTPARAM_SIZE - CHECKSUM_SIZE)) {
		bmdbg("Invalid crc32 value, crc32 %u, ver: %u, active index: %u, addresses: %x, %x\n", bp_head->crc_hash, bp_head->version, bp_head->active_idx, bp_head->address[0], bp_head->address[1]);
		return false;
	}

	return true;
}

static int binary_manager_open_bootparam(void)
{
	int fd;
	char bp_devpath[BINARY_PATH_LEN];

	if (g_bp_info.part_num < 0) {
		bmdbg("Invalid BP partition Num : %d\n", g_bp_info.part_num);
		return BINMGR_INVALID_PARAM;
	}

	/* Open dev to access boot parameters */
	snprintf(bp_devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, g_bp_info.part_num);
	fd = open(bp_devpath, O_RDWR, 0666);
	if (fd < 0) {
		bmdbg("Fail to get a fd for BP, errno %d\n", errno);
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
	size_t update_reason_offset;
	binmgr_bpdata_t scanned_bp_data;
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint32_t bp_app_idx;
	uint32_t max_app_count;
#endif

	if (bp_info == NULL) {
		bmdbg("Invalid input bp_info\n");
		return BINMGR_INVALID_PARAM;
	}

	fd = binary_manager_open_bootparam();
	if (fd < 0) {
		return BINMGR_OPERATION_FAIL;
	}

	bootparam = (char *)kmm_malloc(BOOTPARAM_SIZE);
	if (!bootparam) {
		close(fd);
		bmdbg("Fail to malloc to read BP\n");
		return BINMGR_OUT_OF_MEMORY;
	}

	for (bp_idx = 0; bp_idx < BOOTPARAM_COUNT; bp_idx++) {
		bmdbg("Checking BP%d start\n", bp_idx);
		ret = lseek(fd, BP_SEEK_OFFSET(bp_idx), SEEK_SET);
		if (ret < 0) {
			bmdbg("Fail to seek to read BP, offset %d errno %d\n", BP_SEEK_OFFSET(bp_idx), errno);
			kmm_free(bootparam);
			close(fd);
			return BINMGR_OPERATION_FAIL;
		}

		/* Read bootparam data */
		ret = read(fd, (FAR uint8_t *)bootparam, BOOTPARAM_SIZE);
		if (ret != BOOTPARAM_SIZE) {
			bmdbg("Fail to read BP, errno %d\n", errno);
			continue;
		} else if (!is_valid_bootparam(bootparam)) {
			bmdbg("BP%d is invalid\n", bp_idx);
			continue;
		}

		memcpy(&scanned_bp_data.head, bootparam, sizeof(scanned_bp_data.head));

		/* Version 1 has no update reason and leaves the last byte as reserved. */
		update_reason_offset = BOOTPARAM_SIZE - sizeof(scanned_bp_data.tail.bp_update_reason);
		if (scanned_bp_data.head.format_ver < BOOTPARAM_FORMAT_VERSION_2 || ((uint8_t *)bootparam)[update_reason_offset] > BP_UPDATE_UNKNOWN) {
			scanned_bp_data.tail.bp_update_reason = BP_UPDATE_UNKNOWN;
		} else {
			scanned_bp_data.tail.bp_update_reason = ((uint8_t *)bootparam)[update_reason_offset];
		}

		bmdbg("BP%d is valid. crc %u, version %u, format %u, reason %u, active idx %u, address[0] 0x%x, address[1] 0x%x\n", bp_idx, scanned_bp_data.head.crc_hash, scanned_bp_data.head.version, scanned_bp_data.head.format_ver, scanned_bp_data.tail.bp_update_reason, scanned_bp_data.head.active_idx, scanned_bp_data.head.address[0], scanned_bp_data.head.address[1]);

#ifdef CONFIG_APP_BINARY_SEPARATION
		max_app_count = sizeof(scanned_bp_data.head.app_data) / sizeof(scanned_bp_data.head.app_data[0]);
		bmdbg("BP%d: app count %u, max %u\n", bp_idx, scanned_bp_data.head.app_count, max_app_count);
		for (bp_app_idx = 0; bp_app_idx < scanned_bp_data.head.app_count && bp_app_idx < max_app_count; bp_app_idx++) {
			bmdbg("BP%d: app[%u] name %.*s, useidx %u\n", bp_idx, bp_app_idx, BIN_NAME_MAX, scanned_bp_data.head.app_data[bp_app_idx].name, scanned_bp_data.head.app_data[bp_app_idx].useidx);
		}
#endif

#ifdef CONFIG_RESOURCE_FS
		bmdbg("BP%d: resource active idx %u\n", bp_idx, scanned_bp_data.head.resource_active_idx);
#endif

		/* Update the latest version and index */
		if (latest_ver < scanned_bp_data.head.version) {
			latest_ver = scanned_bp_data.head.version;
			/* Update bootparam data */
			bp_info->inuse_idx = bp_idx;
			memcpy(&bp_info->bp_data, &scanned_bp_data, sizeof(binmgr_bpdata_t));
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
		bmvdbg("BP[%d] ver: %u, format: %u, reason: %u, active index: %u, addresses: %x, %x\n", g_bp_info.inuse_idx, g_bp_info.bp_data.head.version, g_bp_info.bp_data.head.format_ver, g_bp_info.bp_data.tail.bp_update_reason, g_bp_info.bp_data.head.active_idx, g_bp_info.bp_data.head.address[0], g_bp_info.bp_data.head.address[1]);
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
	uint8_t inuse_idx;
	char *bootparam;
	size_t tail_offset;

	if (!bp_data) {
		bmdbg("ERROR: Input bp data is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	if (g_bp_info.inuse_idx >= BOOTPARAM_COUNT) {
		bmdbg("ERROR: Invalid g_bp_info, inuse idx %u\n", g_bp_info.inuse_idx);
		return BINMGR_INVALID_PARAM;
	}

	bootparam = (char *)kmm_malloc(BOOTPARAM_SIZE);
	if (!bootparam) {
		bmdbg("Fail to malloc to write BP\n");
		return BINMGR_OUT_OF_MEMORY;
	}
	memset(bootparam, 0xff, BOOTPARAM_SIZE);

	fd = binary_manager_open_bootparam();
	if (fd < 0) {
		kmm_free(bootparam);
		return BINMGR_OPERATION_FAIL;
	}

	/* Update bootparam data : format version and bp tail */
	bp_data->head.format_ver = BOOTPARAM_FORMAT_VERSION_LATEST;
	tail_offset = BOOTPARAM_SIZE - sizeof(bp_data->tail);
	memcpy(bootparam, &bp_data->head, sizeof(bp_data->head));
	memcpy(&bootparam[tail_offset], &bp_data->tail, sizeof(bp_data->tail));

	/* Update bootparam data : CRC */
	bp_data->head.crc_hash = crc32((uint8_t *)bootparam + CHECKSUM_SIZE, BOOTPARAM_SIZE - CHECKSUM_SIZE);
	((binmgr_bpdata_head_t *)bootparam)->crc_hash = bp_data->head.crc_hash;
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
	kmm_free(bootparam);

	return BINMGR_OK;
errout_with_fd:
	close(fd);
	kmm_free(bootparam);
	return BINMGR_OPERATION_FAIL;
}

static bool binary_manager_is_set_mismatch(binmgr_bpdata_t *bp_data)
{
	uint8_t active_idx;
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint8_t app_idx;
	uint32_t bp_app_idx;
	uint32_t max_app_count;
#endif

	if (!bp_data) {
		bmdbg("BP data is NULL\n");
		return true;
	}

	active_idx = bp_data->head.active_idx;
	if (active_idx >= PARTS_PER_BIN) {
		bmdbg("Invalid BP kernel active idx %u\n", active_idx);
		return true;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	max_app_count = sizeof(bp_data->head.app_data) / sizeof(bp_data->head.app_data[0]);
	if (bp_data->head.app_count > max_app_count) {
		bmdbg("Invalid BP app count %u, max %u\n", bp_data->head.app_count, max_app_count);
		return true;
	}

	for (bp_app_idx = 0; bp_app_idx < bp_data->head.app_count; bp_app_idx++) {
		app_idx = bp_data->head.app_data[bp_app_idx].useidx;
		if (app_idx != active_idx) {
			bmdbg("BP set mismatch: kernel %u, app[%u:%s] %u\n", active_idx, bp_app_idx, bp_data->head.app_data[bp_app_idx].name, app_idx);
			return true;
		}
	}
#endif

#ifdef CONFIG_RESOURCE_FS
	if (bp_data->head.resource_active_idx != active_idx) {
		bmdbg("BP set mismatch: kernel %u, resource %u\n", active_idx, bp_data->head.resource_active_idx);
		return true;
	}
#endif

	return false;
}

static int binary_manager_get_kbin_version(uint8_t part_idx, uint32_t *version)
{
	int ret;
	char filepath[BINARY_PATH_LEN];
	kernel_binary_header_t header_data;
	binmgr_kinfo_t *kdata;

	if (!version) {
		bmdbg("Kernel version output is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	kdata = binary_manager_get_kdata();
	if (part_idx >= kdata->part_count) {
		bmdbg("Invalid kernel part idx %u, part count %u\n", part_idx, kdata->part_count);
		return BINMGR_INVALID_PARAM;
	}

	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kdata->part_info[part_idx].devnum);
	ret = binary_manager_read_header(BINARY_KERNEL, filepath, &header_data, true);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to read kernel version, part idx %u, devpath %s, ret %d\n", part_idx, filepath, ret);
		return ret;
	}

	*version = header_data.version;
	return BINMGR_OK;
}

static int binary_manager_validate_set(binmgr_bpdata_t *bp_data, uint8_t set_idx, struct binmgr_set_validation_s *set_info)
{
	int ret;
#ifdef CONFIG_APP_BINARY_SEPARATION
	int bin_idx;
	uint32_t bp_app_idx;
	uint32_t max_app_count;
#endif

	if (!bp_data || !set_info || set_idx >= PARTS_PER_BIN) {
		bmdbg("Invalid set validation parameter, set idx %u\n", set_idx);
		return BINMGR_INVALID_PARAM;
	}

	ret = binary_manager_verify_kbin(set_idx);
	set_info->kbin_valid = (ret == BINMGR_OK);
	set_info->kbin_version = 0;
	if (set_info->kbin_valid) {
		ret = binary_manager_get_kbin_version(set_idx, &set_info->kbin_version);
		if (ret != BINMGR_OK) {
			set_info->kbin_valid = false;
		}
	}

	set_info->ubin_valid = true;
#ifdef CONFIG_APP_BINARY_SEPARATION
	max_app_count = sizeof(bp_data->head.app_data) / sizeof(bp_data->head.app_data[0]);
	if (bp_data->head.app_count > max_app_count) {
		bmdbg("Invalid BP app count %u, max %u\n", bp_data->head.app_count, max_app_count);
		set_info->ubin_valid = false;
	} else {
		for (bp_app_idx = 0; bp_app_idx < bp_data->head.app_count; bp_app_idx++) {
			bin_idx = binary_manager_get_index_with_name(bp_data->head.app_data[bp_app_idx].name);
			if (bin_idx < 0) {
				bmdbg("Fail to find matched binary %s in binary table\n", bp_data->head.app_data[bp_app_idx].name);
				set_info->ubin_valid = false;
				continue;
			}

			ret = binary_manager_verify_ubin(bin_idx, set_idx);
			if (ret != BINMGR_OK) {
				bmdbg("Set %s app[%u:%s] validation FAIL, ret %d\n", GET_PARTNAME(set_idx), bp_app_idx, bp_data->head.app_data[bp_app_idx].name, ret);
				set_info->ubin_valid = false;
				continue;
			}
			bmvdbg("Set %s app[%u:%s] validation PASS\n", GET_PARTNAME(set_idx), bp_app_idx, bp_data->head.app_data[bp_app_idx].name);
		}
	}
#endif

	set_info->resource_valid = true;
#ifdef CONFIG_RESOURCE_FS
	ret = binary_manager_verify_resource(set_idx);
	set_info->resource_valid = (ret == BINMGR_OK);
	if (!set_info->resource_valid) {
		bmdbg("Set %s resource validation FAIL, ret %d\n", GET_PARTNAME(set_idx), ret);
	} else {
		bmvdbg("Set %s resource validation PASS\n", GET_PARTNAME(set_idx));
	}
#endif

	bmdbg("Set %s validation: kbin %d version %u, apps %d, resource %d\n", GET_PARTNAME(set_idx), set_info->kbin_valid, set_info->kbin_version, set_info->ubin_valid, set_info->resource_valid);

	return BINMGR_OK;
}

static int binary_manager_select_recovery_set(uint8_t active_set, struct binmgr_set_validation_s *set_a, struct binmgr_set_validation_s *set_b, uint8_t *target_set)
{
	bool set_a_valid;
	bool set_b_valid;
	struct binmgr_set_validation_s *target_info;

	if (!set_a || !set_b || !target_set || active_set >= PARTS_PER_BIN) {
		bmdbg("Invalid recovery set select parameter\n");
		return BINMGR_INVALID_PARAM;
	}

	set_a_valid = set_a->kbin_valid && set_a->ubin_valid && set_a->resource_valid;
	set_b_valid = set_b->kbin_valid && set_b->ubin_valid && set_b->resource_valid;

	if (set_a_valid && set_b_valid) {
		*target_set = active_set;
	} else if (set_a_valid) {
		*target_set = 0;
	} else if (set_b_valid) {
		*target_set = 1;
	} else {
		return BINMGR_NOT_FOUND;
	}

	if (*target_set != active_set) {
		target_info = (*target_set == 0) ? set_a : set_b;
		if (target_info->kbin_version == FACTORY_KBIN_VERSION) {
			bmdbg("Do not switch to set %s because kernel version %u is factory binary\n", GET_PARTNAME(*target_set), target_info->kbin_version);
			return BINMGR_NOT_FOUND;
		}
	}

	return BINMGR_OK;
}

int binary_manager_check_bootparam_set(void)
{
	int ret;
	binmgr_bpdata_t *bp_data;

	ret = binary_manager_update_bpinfo();
	if (ret != BINMGR_OK) {
		bmdbg("Fail to update BP info before set-mismatch recovery, ret %d\n", ret);
		return ret;
	}

	bp_data = binary_manager_get_bpdata();
	if (!bp_data) {
		bmdbg("BP data is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	if (binary_manager_is_set_mismatch(bp_data)) {
		return BINMGR_OPERATION_FAIL;
	}

	bmdbg("BP set is already aligned, active idx %u\n", bp_data->head.active_idx);

	return BINMGR_OK;
}

int binary_manager_recover_bootparam_set(void)
{
	int ret;
	uint8_t active_set;
	uint8_t target_set;
	binmgr_bpdata_t update_bp_data;
	binmgr_bpdata_t *bp_data;
	struct binmgr_set_validation_s set_a;
	struct binmgr_set_validation_s set_b;
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint32_t bp_app_idx;
#endif

	bp_data = binary_manager_get_bpdata();
	if (!bp_data) {
		bmdbg("BP data is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	active_set = bp_data->head.active_idx;
	bmdbg("BP set mismatch detected. active idx %u, BP version %u\n", active_set, bp_data->head.version);

	ret = binary_manager_validate_set(bp_data, 0, &set_a);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to validate set A, ret %d\n", ret);
		binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
		return ret;
	}

	ret = binary_manager_validate_set(bp_data, 1, &set_b);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to validate set B, ret %d\n", ret);
		binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
		return ret;
	}

	ret = binary_manager_select_recovery_set(active_set, &set_a, &set_b, &target_set);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to select recoverable set. Reboot as recovery fail\n");
		binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
		return ret;
	}

	bmdbg("Select set %s for BP set alignment. A valid kbin %d apps %d resource %d, B valid kbin %d apps %d resource %d\n", GET_PARTNAME(target_set), set_a.kbin_valid, set_a.ubin_valid, set_a.resource_valid, set_b.kbin_valid, set_b.ubin_valid, set_b.resource_valid);

	memcpy(&update_bp_data, bp_data, sizeof(binmgr_bpdata_t));
	update_bp_data.head.version++;
	update_bp_data.head.active_idx = target_set;
#ifdef CONFIG_APP_BINARY_SEPARATION
	for (bp_app_idx = 0; bp_app_idx < update_bp_data.head.app_count; bp_app_idx++) {
		update_bp_data.head.app_data[bp_app_idx].useidx = target_set;
	}
#endif
#ifdef CONFIG_RESOURCE_FS
	update_bp_data.head.resource_active_idx = target_set;
#endif
	update_bp_data.head.format_ver = BOOTPARAM_FORMAT_VERSION_LATEST;
	update_bp_data.tail.bp_update_reason = BP_UPDATE_BINARY_MANAGER_SET_ALIGNMENT;

	ret = binary_manager_write_bootparam(&update_bp_data);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to write aligned BP, ret %d. Reboot as recovery fail\n", ret);
		binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
		return ret;
	}

	bmdbg("Aligned BP to set %s, new version %u. Reboot for binary update\n", GET_PARTNAME(target_set), update_bp_data.head.version);
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_UPDATE);

	return BINMGR_OPERATION_FAIL;
}

void binary_manager_update_bootparam(int requester_pid, uint8_t type)
{
	int ret;
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

	response_msg.result = BINMGR_OK;
	is_all_updatable = true;

	/* Get current bootparam data and update version */
	memcpy(&update_bp_data, binary_manager_get_bpdata(), sizeof(binmgr_bpdata_t));
	update_bp_data.head.version++;
	update_bp_data.head.format_ver = BOOTPARAM_FORMAT_VERSION_LATEST;
	update_bp_data.tail.bp_update_reason = BP_UPDATE_BINARY_MANAGER_UPDATE;

	if (BM_CHECK_GROUP(type, BINARY_KERNEL)) {
		/* Update bootparam and Reboot if new kernel binary exists */
		ret = binary_manager_check_kernel_update(true);
		if (ret > 0) {
			/* Update index for inactive partition */
			update_bp_data.head.active_idx ^= 1;
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
		ret = binary_manager_check_resource_update(true);
		if (ret > 0) {
			/* Update index for inactive partition */
			update_bp_data.head.resource_active_idx ^= 1;
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
			ret = binary_manager_check_user_update(bin_idx, true);
			if (ret > 0) {
				/* Update index for inactive partition */
				update_bp_data.head.app_data[BIN_BPIDX(bin_idx)].useidx ^= 1;
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
		ret = binary_manager_check_user_update(BM_CMNLIB_IDX, true);
		if (ret > 0) {
			/* Update index for inactive partition */
			update_bp_data.head.app_data[BIN_BPIDX(BM_CMNLIB_IDX)].useidx ^= 1;
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
		ret = binary_manager_write_bootparam(&update_bp_data);
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

void binary_manager_swap_bootparam(int requester_pid)
{
	int ret;
	bool is_all_updatable;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_bpdata_t update_bp_data;
	binmgr_response_t response_msg;
#ifdef CONFIG_APP_BINARY_SEPARATION
	int bin_idx;
	uint32_t bin_count;
#endif

	memset((void *)&response_msg, 0, sizeof(binmgr_response_t));

	if (requester_pid < 0) {
		bmdbg("Invalid requester pid %d\n", requester_pid);
		return;
	}

	response_msg.result = BINMGR_OK;
	is_all_updatable = true;

	/* Get current bootparam data */
	memcpy(&update_bp_data, binary_manager_get_bpdata(), sizeof(binmgr_bpdata_t));
	update_bp_data.head.version++;
	update_bp_data.head.format_ver = BOOTPARAM_FORMAT_VERSION_LATEST;
	update_bp_data.tail.bp_update_reason = BP_UPDATE_BINARY_MANAGER_SWAP;

	/* Update bootparam and Reboot if valid kernel binary exists */
	ret = binary_manager_check_kernel_update(false);
	if (ret < 0) {
		bmdbg("Fail to find valid kernel binary, %d\n", ret);
		goto send_response;
	}
	update_bp_data.head.active_idx ^= 1;

#ifdef CONFIG_RESOURCE_FS
	/* Update bootparam if valid resource binary exists */
	ret = binary_manager_check_resource_update(false);
	if (ret < 0) {
		bmdbg("Fail to find valid resource binary, %d\n", ret);
		goto send_response;
	}
	update_bp_data.head.resource_active_idx ^= 1;
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
	bin_count = binary_manager_get_ucount();
	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		/* Scan binary files */
		ret = binary_manager_check_user_update(bin_idx, false);
		if (ret < 0) {
			bmdbg("Fail to find valid user binary, %d\n", ret);
			goto send_response;
		}
		update_bp_data.head.app_data[BIN_BPIDX(bin_idx)].useidx ^= 1;
	}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	ret = binary_manager_check_user_update(BM_CMNLIB_IDX, false);
	if (ret < 0) {
		bmdbg("Fail to find valid common binary, %d\n", ret);
		goto send_response;
	}
	update_bp_data.head.app_data[BIN_BPIDX(BM_CMNLIB_IDX)].useidx ^= 1;
#endif
#endif

	/* Then, Write bootparam with updated bootparam data */
	ret = binary_manager_write_bootparam(&update_bp_data);
	if (ret == BINMGR_OK) {
		bmvdbg("Update BP SUCCESS\n");
	} else {
		bmdbg("Fail to update BP, %d\n", ret);
	}

send_response:
	response_msg.result = ret;
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, requester_pid);
	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_response_t));
}
