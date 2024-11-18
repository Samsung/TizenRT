/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <errno.h>
#include <sys/mount.h>

#include <tinyara/binary_manager.h>

#include "binary_manager/binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
#define RESOURCE_MOUNTPT "/res"

/* Data for Resource partitions */
static binmgr_resinfo_t resource_info;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************	
 * Name: binary_manager_get_resdata	
 *	
 * Description:	
 *	 This function gets a resource data.	
 *	
 ****************************************************************************/	
binmgr_resinfo_t *binary_manager_get_resdata(void)	
{	
	return &resource_info;	
}

/****************************************************************************
 * Name: binary_manager_register_resource
 *
 * Description:
 *	 This function registers a partition of resource.
 *
 ****************************************************************************/
void binary_manager_register_respart(int part_num, int part_size, uint32_t part_addr)
{
	if (part_num < 0 || part_size <= 0 || part_addr == 0 || resource_info.part_count >= RESOURCE_BIN_COUNT) {
		bmdbg("Invalid resource partition : num %d, size %d, addr 0x%x\n", part_num, part_size, part_addr);
		return;
	}

	resource_info.is_mounted = false;
	resource_info.part_info[resource_info.part_count].size = part_size;
	resource_info.part_info[resource_info.part_count].devnum = part_num;
	resource_info.part_info[resource_info.part_count].address = part_addr;

	bmdbg("[RESOURCE %d] part num %d size %d, address 0x%x\n", resource_info.part_count, part_num, part_size, part_addr);

	resource_info.part_count++;
}

/****************************************************************************
 * Name: binary_manager_mount_resource
 *
 * Description:
 *	 This function mounts resourcefs.
 *
 ****************************************************************************/
int binary_manager_mount_resource(void)
{
	int ret;
	int inuse_idx;
	int bin_count;
	bool need_update_bp = false;
	char devpath[BINARY_PATH_LEN];
	char fs_devpath[BINARY_PATH_LEN];
	resource_binary_header_t header_data;

	if (resource_info.is_mounted) {
		bmvdbg("RESOURCEFS is already mounted\n");
		return OK;
	}

#ifdef CONFIG_USE_BP
	binmgr_bpdata_t *bp_data;
	if (binary_manager_update_bpinfo() != BINMGR_OK) {
		bmdbg("Failed to update bpinfo %d\n", ret);
		return ERROR;
	}

	bp_data = binary_manager_get_bpdata();
	inuse_idx = bp_data->resource_active_idx;
#else
	uint32_t latest_ver = 0;
	int latest_partidx = -1;

	for (int part_idx = 0; part_idx < resource_info.part_count; part_idx++) {
		snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[part_idx].devnum);
		ret = binary_manager_read_header(BINARY_RESOURCE, devpath, &header_data, true);
		if (ret == OK && latest_ver < header_data.version) {
			/* Update latest version and inuse index */
			latest_partidx = part_idx;
			latest_ver = header_data.version;
		}
	}
	inuse_idx = latest_partidx;
#endif
	if (inuse_idx < 0 || inuse_idx >= resource_info.part_count) {
		bmdbg("Failed to find valid resource binary. %d\n", inuse_idx);
		return ERROR;
	}

	bin_count = resource_info.part_count;
	do {
		/* Read and verify header data */
		snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[inuse_idx].devnum);
		ret = binary_manager_read_header(BINARY_RESOURCE, devpath, &header_data, false);
		if (ret == BINMGR_OK) {
			bmvdbg("Resource [%d] Header Checking Success.\n", inuse_idx);
			snprintf(fs_devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[inuse_idx].devnum + RESOURCE_DEVNUM_OFFSET);
			ret = mount(fs_devpath, RESOURCE_MOUNTPT, "romfs", MS_RDONLY, NULL);
			if (ret == OK) {
				/* Update inuse index and resource version */
				resource_info.inuse_idx = inuse_idx;
				resource_info.version = header_data.version;
				resource_info.is_mounted = true;
				printf("%s is mounted successfully @ %s \n", fs_devpath, RESOURCE_MOUNTPT);
				bmdbg("Mount resource success! [Version: %d] [Partition: %s] \n", resource_info.version, GET_PARTNAME(inuse_idx));
				break;
			} else {
				printf("ROMFS ERROR: resourcefs %s mount failed, errno %d\n", fs_devpath, get_errno());
			}
		}

		bmdbg("Invalid Resource header[%d] devpath : %s\n", inuse_idx, devpath);

		/* Check if a binary exists in another partition. */
		if (--bin_count > 0) {
			inuse_idx ^= 1;
#ifdef CONFIG_USE_BP
			need_update_bp = true;
#endif
			bmdbg("Try to read another partition %d\n", resource_info.part_info[inuse_idx].devnum);
			continue;
		} else {
			bmvdbg("No valid Resource binary\n");
			return ERROR;
		}
	} while (bin_count > 0);

#ifdef CONFIG_USE_BP
	if (need_update_bp) {
		/* Update boot param data because the binary not written to bootparam is loaded */
		binmgr_bpdata_t update_bp_data;
		memcpy(&update_bp_data, binary_manager_get_bpdata(), sizeof(binmgr_bpdata_t));
		update_bp_data.version++;
		update_bp_data.resource_active_idx = inuse_idx;
		ret = binary_manager_write_bootparam(&update_bp_data);
		if (ret == BINMGR_OK) {
			binary_manager_set_bpdata(&update_bp_data);
			bmvdbg("Update bootparam SUCCESS. Resource index to %d\n", inuse_idx);
		} else {
			bmdbg("Fail to update bootparam to recover resource. %d\n", ret);
			return ERROR;
		}
	}
#endif
	return OK;
}

/****************************************************************************
 * Name: binary_manager_unmount_resource
 *
 * Description:
 *	 This function unmounts resourcefs.
 *
 ****************************************************************************/
int binary_manager_unmount_resource(void)
{
	int ret;

	/* Unmont current resource */
	ret = unmount(RESOURCE_MOUNTPT);
	if (ret != OK) {
		bmdbg("ERROR: resourcefs unmount failed, errno %d\n", get_errno());
		return ERROR;
	}

	resource_info.is_mounted = false;
	bmvdbg("%s is unmounted successfully \n", RESOURCE_MOUNTPT);

	return OK;
}

/*************************************************************************************
* Name: binary_manager_check_resource_update
*
* Description:
*   This function checks that new resource binary exists on inactive partition
*  and verifies the update is needed by comparing running version with new version.
*
*************************************************************************************/
int binary_manager_check_resource_update(void)
{
	int ret;
	int inactive_partidx;
	char filepath[BINARY_PATH_LEN];
	resource_binary_header_t header_data;

	inactive_partidx = resource_info.inuse_idx ^ 1;

	/* Verify resource binary on the partition without running binary */
	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[inactive_partidx].devnum);
	ret = binary_manager_read_header(BINARY_RESOURCE, filepath, (void *)&header_data, true);
	if (ret == BINMGR_OK) {
#ifdef CONFIG_BINMGR_UPDATE_SAME_VERSION
		if (resource_info.version <= header_data.version) {
#else
		if (resource_info.version < header_data.version) {
#endif
			/* Need to update bootparam and reboot */
			bmvdbg("Found new resource binary in inactive partition %d\n", resource_info.part_info[inactive_partidx].devnum);
			return BINMGR_OK;
		} else {
			bmdbg("Latest version is running, version %d\n", resource_info.version);
			return BINMGR_ALREADY_UPDATED;
		}
	}
	return ret;
}
