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
#include <tinyara/fs/fs.h>
#include <tinyara/fs/mtd.h>

#ifdef CONFIG_RESOURCE_BINARY_SIGNING
#include <tinyara/signature.h>
#endif

#include "binary_manager_internal.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
#define RESOURCE_MOUNTPT "/res"
#define RESOURCE_DEC_DEVNAME_FMT "/dev/mtdblock%d_dec"
#define RESOURCE_DEC_PATH_LEN 24
#define RESOURCE_DEC_SECTOR_SIZE 256

/* Data for Resource partitions */
static binmgr_resinfo_t resource_info;

#ifdef CONFIG_BINMGR_READ_DECRYPTED_BINARY 
struct resource_decblk_s {
	bool is_registered;
	uint32_t part_addr;
	uint32_t part_size;
	char devpath[RESOURCE_DEC_PATH_LEN];
};

static struct resource_decblk_s g_resource_decblk[RESOURCE_BIN_COUNT];

static ssize_t resource_decblk_read(FAR struct inode *inode, FAR unsigned char *buffer, size_t start_sector, unsigned int nsectors)
{
	ssize_t ret;
	size_t offset;
	size_t size;
	struct resource_decblk_s *dev;

	if (!inode || !inode->i_private || !buffer) {
		return -EINVAL;
	}

	dev = (struct resource_decblk_s *)inode->i_private;
	offset = start_sector * RESOURCE_DEC_SECTOR_SIZE;
	size = nsectors * RESOURCE_DEC_SECTOR_SIZE;

	if (nsectors == 0) {
		return 0;
	}

	if (start_sector != offset / RESOURCE_DEC_SECTOR_SIZE ||
		nsectors != size / RESOURCE_DEC_SECTOR_SIZE ||
		offset > dev->part_size ||
		size > dev->part_size - offset) {
		return -EINVAL;
	}

	ret = up_read_decrypted_flash(dev->part_addr + offset, (FAR void *)buffer, size);
	if (ret != OK) {
		return ret < 0 ? ret : -EIO;
	}

	return nsectors;
}

static int resource_decblk_geometry(FAR struct inode *inode, FAR struct geometry *geometry)
{
	struct resource_decblk_s *dev;

	if (!inode || !inode->i_private || !geometry) {
		return -EINVAL;
	}

	dev = (struct resource_decblk_s *)inode->i_private;
	geometry->geo_available = true;
	geometry->geo_mediachanged = false;
	geometry->geo_writeenabled = false;
	geometry->geo_nsectors = dev->part_size / RESOURCE_DEC_SECTOR_SIZE;
	geometry->geo_sectorsize = RESOURCE_DEC_SECTOR_SIZE;
	geometry->geo_model[0] = '\0';

	return OK;
}

static const struct block_operations g_resource_decblk_ops = {
	NULL,
	NULL,
	resource_decblk_read,
	NULL,
	resource_decblk_geometry,
	NULL
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	, 0
#endif
};

static int resource_register_decblk(int part_idx, FAR char *devpath, size_t devpath_len)
{
	int ret;
	struct resource_decblk_s *dev;

	if (part_idx < 0 || part_idx >= resource_info.part_count || !devpath || devpath_len == 0) {
		return -EINVAL;
	}

	dev = &g_resource_decblk[part_idx];
	ret = snprintf(devpath, devpath_len, RESOURCE_DEC_DEVNAME_FMT, resource_info.part_info[part_idx].devnum);
	if (ret < 0 || ret >= (int)devpath_len) {
		return -ENAMETOOLONG;
	}

	if (dev->is_registered) {
		return OK;
	}

	dev->part_addr = resource_info.part_info[part_idx].address;
	dev->part_size = resource_info.part_info[part_idx].size;
	snprintf(dev->devpath, sizeof(dev->devpath), "%s", devpath);

	ret = register_blockdriver(dev->devpath, &g_resource_decblk_ops, 0, dev);
	if (ret < 0) {
		bmdbg("Fail to register decrypted resource block %s, ret %d\n", dev->devpath, ret);
		return ret;
	}

	dev->is_registered = true;
	return OK;
}

static void resource_unregister_decblk(int part_idx)
{
	int ret;
	struct resource_decblk_s *dev;

	if (part_idx < 0 || part_idx >= resource_info.part_count) {
		return;
	}

	dev = &g_resource_decblk[part_idx];
	if (!dev->is_registered) {
		return;
	}

	ret = unregister_blockdriver(dev->devpath);
	if (ret < 0) {
		bmdbg("Fail to unregister decrypted resource block %s, ret %d\n", dev->devpath, ret);
		return;
	}

	dev->is_registered = false;
	dev->devpath[0] = '\0';
}
#endif

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
 * Name: binary_manager_verify_resource
 *
 * Description:
 *	 This function verifies the resource binary in part_idx.
 *
 ****************************************************************************/
int binary_manager_verify_resource(uint8_t part_idx)
{
	int ret;
	char filepath[BINARY_PATH_LEN];
	resource_binary_header_t header_data;
#ifdef CONFIG_RESOURCE_BINARY_SIGNING
	uint32_t resource_start_address;
#endif

	if (part_idx >= resource_info.part_count) {
		bmdbg("Invalid resource part idx %u, part count %u\n", part_idx, resource_info.part_count);
		return BINMGR_INVALID_PARAM;
	}

#ifdef CONFIG_RESOURCE_BINARY_SIGNING
	resource_start_address = resource_info.part_info[part_idx].address;
	ret = up_verify_usersignature(resource_start_address);
	if (ret != SIGNATURE_VAILD) {
		bmdbg("Invalid Resource Signature, part idx %u, address : 0x%x\n", part_idx, resource_start_address);
		return BINMGR_NOT_FOUND;
	}
	bmvdbg("Resource Signature Checking Success, part idx %u, address : 0x%x\n", part_idx, resource_start_address);
#endif

	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[part_idx].devnum);
	ret = binary_manager_read_header(BINARY_RESOURCE, filepath, resource_info.part_info[part_idx].address, (void *)&header_data, true);
	if (ret != BINMGR_OK) {
		bmdbg("Invalid resource candidate, part idx %u, devpath %s, ret %d\n", part_idx, filepath, ret);
		return ret;
	}

	bmvdbg("Valid resource candidate [%s], dev %d, version %u\n", GET_PARTNAME(part_idx), resource_info.part_info[part_idx].devnum, header_data.version);

	return BINMGR_OK;
}

/****************************************************************************
 * Name: binary_manager_get_resource_version
 *
 * Description:
 *	 This function reads the resource binary version from part_idx.
 *
 ****************************************************************************/
uint32_t binary_manager_get_resource_version(uint8_t part_idx)
{
	int ret;
	char filepath[BINARY_PATH_LEN];
	resource_binary_header_t header_data;

	if (part_idx >= resource_info.part_count) {
		bmdbg("Invalid resource part idx %u, part count %u\n", part_idx, resource_info.part_count);
		return 0;
	}

	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[part_idx].devnum);
	ret = binary_manager_read_header(BINARY_RESOURCE, filepath, resource_info.part_info[part_idx].address, &header_data, false);
	if (ret != BINMGR_OK) {
		bmdbg("Fail to read resource header, set %s, devpath %s, ret %d\n", GET_PARTNAME(part_idx), filepath, ret);
		return 0;
	}

	return header_data.version;
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
	char devpath[BINARY_PATH_LEN];
	char fs_devpath[RESOURCE_DEC_PATH_LEN];
	resource_binary_header_t header_data;
#ifdef CONFIG_RESOURCE_BINARY_SIGNING
	uint32_t resource_start_address = 0;
#endif

	if (resource_info.is_mounted) {
		bmvdbg("RESOURCEFS is already mounted\n");
		return OK;
	}

#ifdef CONFIG_USE_BP
	binmgr_bpdata_t *bp_data;
	ret = binary_manager_update_bpinfo();
	if (ret != BINMGR_OK) {
		bmdbg("Failed to update bpinfo %d\n", ret);
		return ERROR;
	}

	bp_data = binary_manager_get_bpdata();
	inuse_idx = bp_data->head.resource_active_idx;
#else
	uint32_t latest_ver = 0;
	int latest_partidx = -1;

	for (int part_idx = 0; part_idx < resource_info.part_count; part_idx++) {
#ifdef CONFIG_RESOURCE_BINARY_SIGNING
		/* Check signature first at the start address */
		resource_start_address = resource_info.part_info[part_idx].address;
		ret = up_verify_usersignature(resource_start_address);
		if (ret == SIGNATURE_VAILD) {
			bmdbg("Resource Signature Checking Success, address : 0x%x\n", resource_start_address);
		} else {
			bmdbg("Invalid Resource Signature, address : 0x%x\n", resource_start_address);
			/* Skip this partition and check next partition */
			continue;
		}
#endif
		snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[part_idx].devnum);
		ret = binary_manager_read_header(BINARY_RESOURCE, devpath, resource_info.part_info[part_idx].address, &header_data, true);
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
#ifdef CONFIG_RESOURCE_BINARY_SIGNING
		/* Check signature */
		resource_start_address = resource_info.part_info[inuse_idx].address;
		ret = up_verify_usersignature(resource_start_address);
		if (ret == SIGNATURE_VAILD) {
			bmdbg("Resource Signature Checking Success, address : 0x%x\n", resource_start_address);
		} else {
			bmdbg("Invalid Resource Signature, address : 0x%x\n", resource_start_address);

#ifdef CONFIG_USE_BP
			/* If resource binary is invalid */
			/* Scan all binaries and set valid binary set in bootparam */
			ret = binary_manager_recover_bootparam_set();
			if (ret != BINMGR_OK) {
				bmdbg("Failed to recover bootparam set mismatch, ret %d\n", ret);
				return ERROR;
			}
#endif

			/* Check if a binary exists in another partition. */
			if (--bin_count > 0) {
				inuse_idx ^= 1;
				bmdbg("Try to read another partition %d\n", resource_info.part_info[inuse_idx].devnum);
				continue;
			} else {
				bmdbg("No valid Resource binary\n");
				return ERROR;
			}
		}
#endif
		/* Read and verify header data */
		snprintf(devpath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[inuse_idx].devnum);
		bmdbg("Checking resource in partition [%d], path %s\n", inuse_idx, devpath);
		ret = binary_manager_read_header(BINARY_RESOURCE, devpath, resource_info.part_info[inuse_idx].address, &header_data, false);
		if (ret == BINMGR_OK) {
			bmvdbg("Resource [%d] Header Checking Success.\n", inuse_idx);
#ifdef CONFIG_BINMGR_READ_DECRYPTED_BINARY 
			ret = resource_register_decblk(inuse_idx, fs_devpath, sizeof(fs_devpath));
			if (ret != OK) {
				bmdbg("Fail to register decrypted resource block, part idx %d, ret %d\n", inuse_idx, ret);
				return ERROR;
			}
#else
			snprintf(fs_devpath, sizeof(fs_devpath), BINMGR_DEVNAME_FMT, resource_info.part_info[inuse_idx].devnum);
#endif
#ifdef CONFIG_RESOURCE_BINARY_SIGNING
			ret = mount(fs_devpath, RESOURCE_MOUNTPT, "romfs", MS_RDONLY, RESOURCE_HEADER_SIZE + USER_SIGN_PREPEND_SIZE);
#else
			ret = mount(fs_devpath, RESOURCE_MOUNTPT, "romfs", MS_RDONLY, RESOURCE_HEADER_SIZE);
#endif
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
#ifdef CONFIG_BINMGR_READ_DECRYPTED_BINARY 
				resource_unregister_decblk(inuse_idx);
#endif
			}
		}

		bmdbg("Invalid Resource header[%d] devpath : %s\n", inuse_idx, devpath);

#ifdef CONFIG_USE_BP
		/* If resource binary is invalid */
		/* Scan all binaries and set valid binary set in bootparam */
		ret = binary_manager_recover_bootparam_set();
		if (ret != BINMGR_OK) {
			bmdbg("Failed to recover bootparam set mismatch, ret %d\n", ret);
			return ERROR;
		}
#endif

		/* Check if a binary exists in another partition. */
		if (--bin_count > 0) {
			inuse_idx ^= 1;
			bmdbg("Try to read another partition %d\n", resource_info.part_info[inuse_idx].devnum);
			continue;
		} else {
			bmdbg("No valid Resource binary\n");
			return ERROR;
		}
	} while (bin_count > 0);

	return OK;
}

/****************************************************************************
 * Name: binary_manager_umount_resource
 *
 * Description:
 *	 This function unmounts resourcefs.
 *
 ****************************************************************************/
int binary_manager_umount_resource(void)
{
	int ret;

	/* Unmount resourcefs */
	ret = umount(RESOURCE_MOUNTPT);
	if (ret != OK) {
		bmdbg("ERROR: resourcefs umount failed, errno %d\n", get_errno());
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
 *   This function checks that new user binary exists in their own inactive partition.
 *  If check_updatable is true, validate whole binary by checking CRC
 *  and it checks version by comparing running version with version of binary in inactive partition.
 *  Otherwise, it just checks header data of binary.
 *
 *******************************************************************************************/
int binary_manager_check_resource_update(bool check_updatable)
{
	int ret;
	int inactive_partidx;
	char filepath[BINARY_PATH_LEN];
	resource_binary_header_t header_data;
#ifdef CONFIG_RESOURCE_BINARY_SIGNING
	uint32_t resource_start_address;
#endif

	inactive_partidx = resource_info.inuse_idx ^ 1;

#ifdef CONFIG_RESOURCE_BINARY_SIGNING
	/* Check signature */
	resource_start_address = resource_info.part_info[inactive_partidx].address;
	ret = up_verify_usersignature(resource_start_address);
	if (ret == SIGNATURE_VAILD) {
		bmvdbg("Resource Signature Checking Success, address : 0x%x\n", resource_start_address);
	} else {
		bmdbg("Invalid Resource Signature, address : 0x%x\n", resource_start_address);
		return BINMGR_NOT_FOUND;
	}
#endif

	/* Verify resource binary on the partition without running binary */
	snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, resource_info.part_info[inactive_partidx].devnum);
	ret = binary_manager_read_header(BINARY_RESOURCE, filepath, resource_info.part_info[inactive_partidx].address, (void *)&header_data, check_updatable);
	if (ret == BINMGR_OK) {
		if (!check_updatable) {
			bmvdbg("Found valid resource binary in inactive partition %d\n", resource_info.part_info[inactive_partidx].devnum);
			return header_data.version;
		}
#ifdef CONFIG_BINMGR_UPDATE_SAME_VERSION
		if (resource_info.version <= header_data.version) {
#else
		if (resource_info.version < header_data.version) {
#endif
			/* Need to update bootparam and reboot */
			bmvdbg("Found new resource binary in inactive partition %d\n", resource_info.part_info[inactive_partidx].devnum);
			return header_data.version;
		}

		/* Running version is the latest */
		bmdbg("Latest version is running, version %d\n", resource_info.version);
		return BINMGR_ALREADY_UPDATED;
	}
	bmdbg("No valid resource binary in inactive partition\n");
	return ret;
}
