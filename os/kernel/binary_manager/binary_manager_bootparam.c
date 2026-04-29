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
static binmgr_bp_recovery_info_t g_bp_recovery_info;

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
 * Name: binary_manager_write_bootparam_to_slot
 *
 * Description:
 *	 This function writes input bootparam data, bp_data, to the requested BP slot.
 *
 ********************************************************************************/
static int binary_manager_write_bootparam_to_slot(binmgr_bpdata_t *bp_data, uint8_t bp_idx)
{
	int fd;
	int ret;
	char *bootparam;
	size_t tail_offset;

	if (!bp_data) {
		bmdbg("ERROR: Input bp data is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	if (bp_idx >= BOOTPARAM_COUNT) {
		bmdbg("ERROR: Invalid BP index %u\n", bp_idx);
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

	ret = lseek(fd, BP_SEEK_OFFSET(bp_idx), SEEK_SET);
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

/*********************************************************************************
 * Name: binary_manager_write_bootparam
 *
 * Description:
 *	 This function writes input bootparam data, bp_data, to the inactive BP slot.
 *
 ********************************************************************************/
int binary_manager_write_bootparam(binmgr_bpdata_t *bp_data)
{
	if (g_bp_info.inuse_idx >= BOOTPARAM_COUNT) {
		bmdbg("ERROR: Invalid g_bp_info, inuse idx %u\n", g_bp_info.inuse_idx);
		return BINMGR_INVALID_PARAM;
	}

	return binary_manager_write_bootparam_to_slot(bp_data, g_bp_info.inuse_idx ^ 1);
}

/****************************************************************************
 * Name: binary_manager_is_set_mismatch
 *
 * Description:
 *	 This function checks whether all BP indices match the active kernel set.
 *   Only AAAA or BBBB can be set as active set.
 *
 ****************************************************************************/
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

/****************************************************************************
 * Name: binary_manager_make_bootparam_from_partitions
 *
 * Description:
 *	 This function rebuilds bootparam data for given set_idx as active_idx from registered partitions.
 *   bp_data is updated with the new version and active set.
 *
 ****************************************************************************/
static int binary_manager_make_bootparam_from_partitions(binmgr_bpdata_t *bp_data, uint8_t set_idx, uint32_t version)
{
	uint32_t part_idx;
	binmgr_kinfo_t *kdata;
#ifdef CONFIG_APP_BINARY_SEPARATION
	int ret;
	int bin_idx;
	uint32_t app_count;
	uint32_t max_app_count;
	uint32_t bin_count;
	char devpath[BINARY_PATH_LEN];
	user_binary_header_t user_header_data;
#endif

	if (!bp_data || set_idx >= PARTS_PER_BIN) {
		bmdbg("Invalid bootparam make parameter, set idx %u\n", set_idx);
		return BINMGR_INVALID_PARAM;
	}

	memset(bp_data, 0, sizeof(binmgr_bpdata_t));
	bp_data->head.version = version;
	bp_data->head.format_ver = BOOTPARAM_FORMAT_VERSION_LATEST;
	bp_data->head.active_idx = set_idx;
	bp_data->tail.bp_update_reason = BP_UPDATE_BINARY_MANAGER_SET_ALIGNMENT;

	kdata = binary_manager_get_kdata();
	if (!kdata) {
		bmdbg("kernel data is not found\n");
		return BINMGR_INVALID_PARAM;
	}

	if (kdata->part_count > BOOTPARAM_COUNT) {
		bmdbg("Invalid kernel part count %u\n", kdata->part_count);
		return BINMGR_INVALID_PARAM;
	}

	for (part_idx = 0; part_idx < kdata->part_count; part_idx++) {
		bp_data->head.address[part_idx] = kdata->part_info[part_idx].address;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	max_app_count = sizeof(bp_data->head.app_data) / sizeof(bp_data->head.app_data[0]);
	bin_count = binary_manager_get_ucount();
	app_count = 0;
	for (bin_idx = 0; bin_idx <= bin_count; bin_idx++) {
		if (BIN_COUNT(bin_idx) == 0) {
			continue;
		}

		if (app_count >= max_app_count) {
			bmdbg("App count exceeds BP max app count %u\n", max_app_count);
			return BINMGR_INVALID_PARAM;
		}

		if (set_idx >= BIN_COUNT(bin_idx)) {
			bmdbg("Invalid %s part idx %u, part count %u\n", BIN_NAME(bin_idx), set_idx, BIN_COUNT(bin_idx));
			return BINMGR_INVALID_PARAM;
		}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
		if (bin_idx == BM_CMNLIB_IDX) {
			strncpy(bp_data->head.app_data[app_count].name, BM_CMNLIB_NAME, BIN_NAME_MAX - 1);
		} else
#endif
		{
			snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, BIN_PARTNUM(bin_idx, set_idx));
			ret = binary_manager_read_header(BINARY_USERAPP, devpath, &user_header_data, false);
			if (ret != BINMGR_OK) {
				bmdbg("Fail to read app header, name %s, set %s, ret %d\n", BIN_NAME(bin_idx), GET_PARTNAME(set_idx), ret);
				return ret;
			}
			strncpy(bp_data->head.app_data[app_count].name, user_header_data.bin_name, BIN_NAME_MAX - 1);
		}
		bp_data->head.app_data[app_count].name[BIN_NAME_MAX - 1] = '\0';
		bp_data->head.app_data[app_count].useidx = set_idx;
		app_count++;
	}
	bp_data->head.app_count = app_count;
#endif

#ifdef CONFIG_RESOURCE_FS
	bp_data->head.resource_active_idx = set_idx;
#endif

	return BINMGR_OK;
}

/****************************************************************************
 * Name: binary_manager_validate_set
 *
 * Description:
 *	 This function validates all binaries in given set_idx and returns false on the
 *	 first invalid binary.
 *
 ****************************************************************************/
static bool binary_manager_validate_set(uint8_t set_idx)
{
	int ret;
#ifdef CONFIG_APP_BINARY_SEPARATION
	int bin_idx;
	uint32_t bin_count;
#endif

	if (set_idx >= PARTS_PER_BIN) {
		bmdbg("Invalid set validation parameter, set idx %u\n", set_idx);
		return false;
	}

	ret = binary_manager_verify_kbin(set_idx);
	if (ret != BINMGR_OK) {
		bmdbg("Set %s kernel validation FAIL, ret %d\n", GET_PARTNAME(set_idx), ret);
		return false;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	bin_count = binary_manager_get_ucount();
	for (bin_idx = 0; bin_idx <= bin_count; bin_idx++) {
		if (BIN_COUNT(bin_idx) == 0) {
			continue;
		}

		if (set_idx >= BIN_COUNT(bin_idx)) {
			bmdbg("Set %s app[%d:%s] has no partition, part count %u\n", GET_PARTNAME(set_idx), bin_idx, BIN_NAME(bin_idx), BIN_COUNT(bin_idx));
			return false;
		}

		ret = binary_manager_verify_ubin(bin_idx, set_idx);
		if (ret != BINMGR_OK) {
			bmdbg("Set %s app[%d:%s] validation FAIL, ret %d\n", GET_PARTNAME(set_idx), bin_idx, BIN_NAME(bin_idx), ret);
			return false;
		}
		bmvdbg("Set %s app[%d:%s] validation PASS\n", GET_PARTNAME(set_idx), bin_idx, BIN_NAME(bin_idx));
	}
#endif

#ifdef CONFIG_RESOURCE_FS
	ret = binary_manager_verify_resource(set_idx);
	if (ret != BINMGR_OK) {
		bmdbg("Set %s resource validation FAIL, ret %d\n", GET_PARTNAME(set_idx), ret);
		return false;
	}
	bmvdbg("Set %s resource validation PASS\n", GET_PARTNAME(set_idx));
#endif

	bmdbg("Set %s validation PASS\n", GET_PARTNAME(set_idx));

	return true;
}

/****************************************************************************
 * Name: binary_manager_get_kbin_version
 *
 * Description:
 *	 This function reads the kernel binary version from the given set.
 *	 It returns 0 if any step fails.
 *
 ****************************************************************************/
static uint32_t binary_manager_get_kbin_version(uint8_t set_idx)
{
	int ret;
	binmgr_kinfo_t *kdata;
	kernel_binary_header_t header_data;
	char filepath[BINARY_PATH_LEN];

	kdata = binary_manager_get_kdata();
	if (!kdata || set_idx >= kdata->part_count) {
		bmdbg("Invalid kernel version request, set idx %u\n", set_idx);
		return 0;
	}

	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kdata->part_info[set_idx].devnum);
	ret = binary_manager_read_header(BINARY_KERNEL, filepath, &header_data, true);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to read kernel header, set %s, devpath %s, ret %d\n", GET_PARTNAME(set_idx), filepath, ret);
		return 0;
	}

	return header_data.version;
}

/****************************************************************************
 * Name: binary_manager_check_bootparam_set
 *
 * Description:
 *	 This function checks whether bootparam set recovery is required.
 *	 It returns BINMGR_OPERATION_FAIL to enter the recovery path.
 *
 ****************************************************************************/
int binary_manager_check_bootparam_set(void)
{
	int ret;
	binmgr_bpdata_t *bp_data;

	g_bp_recovery_info.has_valid_bp = false;
	g_bp_recovery_info.inuse_idx = -1;
	g_bp_recovery_info.active_set = -1;
	g_bp_recovery_info.bp_version = -1;

	ret = binary_manager_update_bpinfo();
	if (ret != BINMGR_OK) {
		bmdbg("No valid BP. Run BP recovery from partition information\n");
		return BINMGR_OPERATION_FAIL;
	}

	bp_data = binary_manager_get_bpdata();
	if (!bp_data) {
		bmdbg("BP data is NULL. Run BP recovery from partition information\n");
		return BINMGR_OPERATION_FAIL;
	}

	g_bp_recovery_info.has_valid_bp = true;
	g_bp_recovery_info.inuse_idx = g_bp_info.inuse_idx;
	g_bp_recovery_info.bp_version = bp_data->head.version;
	g_bp_recovery_info.active_set = bp_data->head.active_idx;

	if (binary_manager_is_set_mismatch(bp_data)) {
		bmdbg("BP set mismatch detected. Run BP recovery from partition information\n");
		return BINMGR_OPERATION_FAIL;
	}

	bmdbg("BP set is already aligned and active set %s is valid\n", GET_PARTNAME(bp_data->head.active_idx));
	return BINMGR_OK;
}

/****************************************************************************
 * Name: binary_manager_recover_bootparam_set
 *
 * Description:
 *	 This function selects a valid binary set, writes an aligned BP, and reboots.
 *
 ****************************************************************************/
int binary_manager_recover_bootparam_set(void)
{
	int ret;
	uint8_t target_set;
	uint8_t write_bp_idx;
	uint32_t new_version;
	uint32_t set_a_version;
	uint32_t set_b_version;
	uint32_t target_version;
	binmgr_bpdata_t update_bp_data;
	bool set_a_valid;
	bool set_b_valid;

	bmdbg("BP set recovery required. valid BP %d, active idx %d, BP version %u\n", g_bp_recovery_info.has_valid_bp, g_bp_recovery_info.active_set, g_bp_recovery_info.bp_version);

	set_a_valid = binary_manager_validate_set(0);
	set_b_valid = binary_manager_validate_set(1);
	set_a_version = set_a_valid ? binary_manager_get_kbin_version(0) : 0;
	set_b_version = set_b_valid ? binary_manager_get_kbin_version(1) : 0;

	bmdbg("Set A valid %d, version %u, Set B valid %d, version %u\n", set_a_valid, set_a_version, set_b_valid, set_b_version);

	if (set_a_valid && set_b_valid) {
		if (g_bp_recovery_info.has_valid_bp) {
			target_set = g_bp_recovery_info.active_set;
		} else {
			target_set = set_a_version > set_b_version ? 0 : 1;
		}
	} else {
		target_set = set_a_valid ? 0 : 1;
	}

	target_version = target_set == 0 ? set_a_version : set_b_version;
	if (target_version == 0 || target_version == FACTORY_KBIN_VERSION) {
		bmdbg("Invalid target kernel version, set %s, version %u. Reboot as recovery fail\n", GET_PARTNAME(target_set), target_version);
		binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
		return BINMGR_OPERATION_FAIL;
	}

	bmdbg("Select set %s for BP set alignment.\n", GET_PARTNAME(target_set));

	new_version = g_bp_recovery_info.has_valid_bp ? g_bp_recovery_info.bp_version + 1 : 1;
	ret = binary_manager_make_bootparam_from_partitions(&update_bp_data, target_set, new_version);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to make recovery BP, ret %d. Reboot as recovery fail\n", ret);
		binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
		return ret;
	}

	write_bp_idx = g_bp_recovery_info.has_valid_bp ? (g_bp_recovery_info.inuse_idx ^ 1) : 0;
	ret = binary_manager_write_bootparam_to_slot(&update_bp_data, write_bp_idx);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to write aligned BP, ret %d. Reboot as recovery fail\n", ret);
		binary_manager_reset_board(REBOOT_SYSTEM_BINARY_RECOVERYFAIL);
		return ret;
	}

	bmdbg("Aligned BP to set %s, BP slot %u, new version %u. Reboot for binary update\n", GET_PARTNAME(target_set), write_bp_idx, update_bp_data.head.version);
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_UPDATE);

	return BINMGR_OK;
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
