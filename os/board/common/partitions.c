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

#undef  CONFIG_DEBUG
#undef  CONFIG_DEBUG_ERROR
#undef  CONFIG_DEBUG_WARN
#undef  CONFIG_DEBUG_VERBOSE
#undef  CONFIG_ARCH_LOWPUTC

#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_ERROR 1
#define CONFIG_DEBUG_WARN 1
#define CONFIG_DEBUG_VERBOSE 1
#define CONFIG_ARCH_LOWPUTC 1

#include <debug.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(CONFIG_AUTOMOUNT_USERFS) || defined(CONFIG_AUTOMOUNT_ROMFS) || defined(CONFIG_LIBC_ZONEINFO_ROMFS)
#include <sys/mount.h>
#include <tinyara/fs/mksmartfs.h>
#endif

#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_BINARY_MANAGER
#include <tinyara/binary_manager.h>
#endif
#include "common.h"

#define FS_PATH_MAX 16

#ifdef CONFIG_AUTOMOUNT_USERFS
#ifdef CONFIG_USERFS_MNTPT
#define USERFS_MNTPT           CONFIG_USERFS_MNTPT
#else
#define USERFS_MNTPT           "/mnt"
#endif

#ifdef CONFIG_USERFS_EXT_MNTPT
#define USERFS_EXT_MNTPT       CONFIG_USERFS_EXT_MNTPT
#else
#define USERFS_EXT_MNTPT       "/ext"
#endif
#endif /* CONFIG_AUTOMOUNT_USERFS */

#ifdef CONFIG_FLASH_PARTITION

struct partition_data_s {
	uint32_t start_addr;
	char *types;
	char *sizes;
	int minor;
#ifdef CONFIG_MTD_PARTITION_NAMES
	char *names;
#endif
};

struct partition_data_s g_flash_part_data = {
	CONFIG_FLASH_START_ADDR,
	CONFIG_FLASH_PART_TYPE,
	CONFIG_FLASH_PART_SIZE,
	CONFIG_FLASH_MINOR,
#ifdef CONFIG_MTD_PARTITION_NAMES
	CONFIG_FLASH_PART_NAME
#endif
};

#ifdef CONFIG_SECOND_FLASH_PARTITION
#if defined(CONFIG_BINARY_MANAGER) && !defined(CONFIG_SECOND_FLASH_START_ADDR)
#error "Please define a start address of second flash, CONFIG_SECOND_FLASH_START_ADDR."
#endif

struct partition_data_s g_second_flash_part_data = {
	CONFIG_SECOND_FLASH_START_ADDR,
	CONFIG_SECOND_FLASH_PART_TYPE,
	CONFIG_SECOND_FLASH_PART_SIZE,
	CONFIG_SECOND_FLASH_MINOR,
#ifdef CONFIG_MTD_PARTITION_NAMES
	CONFIG_SECOND_FLASH_PART_NAME
#endif
};
#endif

static int g_partno;

FAR struct mtd_dev_s *mtd_initialize(void)
{
	FAR struct mtd_dev_s *mtd;
	g_partno = 0;
#ifdef CONFIG_MTD_PROGMEM
	mtd = progmem_initialize();
	if (!mtd) {
		printf("ERROR: progmem_initialize failed\n");
		return NULL;
	}
#else
	mtd = up_flashinitialize();
	if (!mtd) {
		printf("ERROR : up_flashinitializ failed\n");
		return NULL;
	}
#endif
	return mtd;
}

static int type_specific_initialize(int minor, FAR struct mtd_dev_s *mtd_part, const char *types, partition_info_t *partinfo)
{
	int tagno = MTD_NONE;
#ifdef CONFIG_FS_ROMFS
	bool save_romfs_partno = false;
#endif
#ifdef CONFIG_LIBC_ZONEINFO_ROMFS
	bool save_timezone_partno = false;
#endif

	if (partinfo == NULL) {
		printf("ERROR: partinfo is NULL\n");
		return ERROR;
	}

#ifdef CONFIG_MTD_FTL
	bool do_ftlinit = false;
	if (!strncmp(types, "ftl,", 4)) {
		tagno = MTD_FTL;
		do_ftlinit = true;
	}
#ifdef CONFIG_BINARY_MANAGER
	else if (!strncmp(types, "kernel,", 7)
	|| !strncmp(types, "bootparam,", 10)
#ifdef CONFIG_APP_BINARY_SEPARATION
	|| !strncmp(types, "bin,", 4)
#endif
	) {
		do_ftlinit = true;
	}
#endif /* CONFIG_BINARY_MANAGER */

#ifdef CONFIG_FS_ROMFS
	else if (!strncmp(types, "romfs,", 6)) {
		do_ftlinit = true;
		tagno = MTD_ROMFS;
		save_romfs_partno = true;
	}
#endif
#ifdef CONFIG_LIBC_ZONEINFO_ROMFS
	else if (!strncmp(types, "timezone,", 9)) {
		do_ftlinit = true;
		tagno = MTD_ROMFS;
		save_timezone_partno = true;
	}
#endif
#ifdef CONFIG_RESOURCE_FS
	else if (!strncmp(types, "resource,", 9)) {
		do_ftlinit = true;
		tagno = MTD_FTL;
	}
#endif
#endif

#ifdef CONFIG_MTD_CONFIG
	else if (!strncmp(types, "config,", 7)) {
		mtdconfig_register(mtd_part);
	}
#endif

#if defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS)
	else if (!strncmp(types, "smartfs,", 8)) {
		char partref[4];

		snprintf(partref, sizeof(partref), "p%d", g_partno);
		smart_initialize(minor, mtd_part, partref);
		partinfo->smartfs_partno = g_partno;
		mtd_setpartitiontagno(mtd_part, MTD_FS);
	}
#endif

#ifdef CONFIG_MTD_FTL
	if (do_ftlinit) {
		if (ftl_initialize(g_partno, mtd_part)) {
			printf("ERROR: failed to initialise mtd ftl errno :%d\n", errno);
			return ERROR;
		}
		mtd_setpartitiontagno(mtd_part, tagno);
#ifdef CONFIG_FS_ROMFS
		if (save_romfs_partno) {
			partinfo->romfs_partno = g_partno;
			save_romfs_partno = false;
		}
#endif

#ifdef CONFIG_LIBC_ZONEINFO_ROMFS
		if (save_timezone_partno) {
			partinfo->timezone_partno = g_partno;
			save_timezone_partno = false;
		}
#endif
	}
#endif /* CONFIG_MTD_FTL */
	partinfo->minor = minor;
	return OK;
}

static void move_to_next_part(const char **par)
{
	/* Move to next part information. */
	while (*(*par)++ != ',');
}

int get_partition_num(char *part)
{
	int partno = 0;
	int name_len = strlen(part);

	char *partname = CONFIG_FLASH_PART_NAME;
	while (*partname) {
		if (!strncmp(partname, part, name_len)) {
			if (*(partname + name_len) == ',') {
				return partno;
			}
		}
		partno++;
		move_to_next_part((const char **)&partname);
	}
	return ERROR;
}

#ifdef CONFIG_MTD_PARTITION_NAMES
static void configure_partition_name(FAR struct mtd_dev_s *mtd_part, const char **names, int *index, char *part_name)
{
	if (*(*names)) {
		while (*(*names)) {
			if (*(*names) == ',') {
				part_name[*index] = '\0';
				*index = 0;
				(*names)++;
				break;
			}
			if (*index < MTD_PARTNAME_LEN) {
				part_name[(*index)++] = *(*names)++;
			} else {
				part_name[*index] = '\0';
				*index = 0;
				printf("ERROR: Partition name is so long. Please make it smaller than %d\n", MTD_PARTNAME_LEN);
				/* Move to next part name information. */
				move_to_next_part(names);
				break;
			}
		}
		mtd_setpartitionname(mtd_part, part_name);
	}
}
#endif

#ifdef CONFIG_RESOURCE_FS
static int make_resource_mtd_partition(struct mtd_dev_s *mtd, off_t partoffset, off_t nblocks, uint16_t partno)
{
	FAR struct mtd_dev_s *mtd_part;
	uint16_t resource_partno;

	/* Set part num to avoid duplication */
	resource_partno = partno + RESOURCE_DEVNUM_OFFSET;

	mtd_part = mtd_partition(mtd, partoffset, nblocks, resource_partno);

	if (!mtd_part) {
		printf("ERROR: failed to create partition.\n");
		return ERROR;
	}

	if (ftl_initialize(resource_partno, mtd_part)) {
		printf("ERROR: failed to initialise mtd ftl errno :%d\n", errno);
		return ERROR;
	}

	return OK;
}
#endif

int configure_mtd_partitions(struct mtd_dev_s *mtd, int minor, partition_info_t *partinfo)
{
	int ret;
	int partoffset;
	char *types;
	char *sizes;
#ifdef CONFIG_MTD_PARTITION_NAMES
	char part_name[MTD_PARTNAME_LEN + 1];
	int index = 0;
	char *names;
#endif
	FAR struct mtd_geometry_s geo;
	struct partition_data_s part_data;

	if (minor == 0) {
		part_data = g_flash_part_data;
	} else {
#ifdef CONFIG_SECOND_FLASH_PARTITION	
		part_data = g_second_flash_part_data;
#else
		printf("ERROR: Invalid minor\n");
		return ERROR;
#endif
	}

	if (!mtd || !part_data.types || !part_data.sizes || !partinfo) {
		printf("ERROR: Invalid partition data is NULL\n");
		return ERROR;
	}
#ifdef CONFIG_MTD_PARTITION_NAMES
	else if (!part_data.names) {
		printf("ERROR: Invalid partition data is NULL\n");
		return ERROR;
	}
#endif

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		printf("ERROR: mtd->ioctl failed\n");
		return ERROR;
	}

	/* Initialize partinfo data */
	partinfo->smartfs_partno = -1;
	partinfo->romfs_partno = -1;
	partinfo->timezone_partno = -1;

	partoffset = 0;
	types = part_data.types;
	sizes = part_data.sizes;
#ifdef CONFIG_MTD_PARTITION_NAMES
	names = part_data.names;
#endif

	while (*sizes) {
		FAR struct mtd_dev_s *mtd_part;
		int partsize;
		partsize = strtoul(sizes, NULL, 0) << 10;

		if (partsize < geo.erasesize) {
			printf("ERROR: Partition size is lesser than erasesize partsize : %d erasesize : %d\n", partsize, geo.erasesize);
			return ERROR;
		}

		if (partsize % geo.erasesize != 0) {
			printf("ERROR: Partition size is not multiple of erasesize partsize : %d erasesize : %d\n", partsize, geo.erasesize);
			return ERROR;
		}
		mtd_part = mtd_partition(mtd, partoffset, partsize / geo.blocksize, g_partno);

		if (!mtd_part) {
			printf("ERROR: failed to create partition.\n");
			return ERROR;
		}

		ret = type_specific_initialize(minor, mtd_part, types, partinfo);
		if (ret != OK) {
			printf("ERROR: fail to initialize type specific mtd part.\n");
			return ERROR;
		}

#ifdef CONFIG_BINARY_MANAGER
		if (!strncmp(types, "kernel,", 7)) {
			binary_manager_register_kpart(g_partno, partsize, part_data.start_addr + partoffset * geo.blocksize);
#ifdef CONFIG_USE_BP
		} else if (!strncmp(types, "bootparam,", 10)) {
			binary_manager_register_bppart(g_partno, partsize);
#endif
		}
#endif
#ifdef CONFIG_RESOURCE_FS
		if (!strncmp(types, "resource,", 9)) {
			int nblocks = geo.erasesize / geo.blocksize;
			/* Make mtd dev to access resource fs. It starts from offset + erasesize (4K). */
			ret = make_resource_mtd_partition(mtd, partoffset + (geo.erasesize / 1024), partsize / geo.blocksize - nblocks, g_partno);
			if (ret != OK) {
				printf("ERROR: fail to make resource mtd part.\n");
			}
		}
#endif
#ifdef CONFIG_MTD_PARTITION_NAMES
		configure_partition_name(mtd_part, (const char **)&names, &index, part_name);
#ifdef CONFIG_BINARY_MANAGER
#ifdef CONFIG_APP_BINARY_SEPARATION
		if (!strncmp(types, "bin,", 4)) {
			binary_manager_register_upart(part_name, g_partno, partsize, part_data.start_addr + partoffset * geo.blocksize);
		}
#endif
#ifdef CONFIG_RESOURCE_FS
		if (!strncmp(types, "resource,", 9)) {
			binary_manager_register_respart(g_partno, partsize, part_data.start_addr + partoffset * geo.blocksize);
		}
#endif
#endif
#endif
		partoffset += partsize / geo.blocksize;

		move_to_next_part((const char **)&sizes);
		move_to_next_part((const char **)&types);
		g_partno++;
	}
	return OK;
}

void automount_fs_partition(partition_info_t *partinfo)
{
#if defined(CONFIG_AUTOMOUNT_USERFS) || defined(CONFIG_AUTOMOUNT_ROMFS) || defined(CONFIG_LIBC_ZONEINFO_ROMFS)
	int ret;
	char fs_devname[FS_PATH_MAX];

	if (partinfo == NULL) {
		printf("ERROR : partinfo is NULL.\n");
		return;
	}
#ifdef CONFIG_AUTOMOUNT_USERFS
	/* Initialize and mount user partition (if we have) */
	if (partinfo->smartfs_partno != -1) {
		snprintf(fs_devname, FS_PATH_MAX, "/dev/smart%dp%d", partinfo->minor, partinfo->smartfs_partno);
	#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
		ret = mksmartfs(fs_devname, 1, false);
	#else
		ret = mksmartfs(fs_devname, false);
	#endif
		if (ret != OK) {
			printf("ERROR: mksmartfs on %s failed errno : %d\n", fs_devname, errno);
		} else {
			char *mountpath;
			if (partinfo->minor == 0) {
				mountpath = USERFS_MNTPT;
			} else {
				mountpath = USERFS_EXT_MNTPT;
			}
			ret = mount(fs_devname, mountpath, "smartfs", 0, NULL);
			if (ret != OK) {
				printf("ERROR: mounting '%s' failed, errno %d\n", fs_devname, get_errno());
			} else {
				printf("%s is mounted successfully @ %s \n", fs_devname, mountpath);
			}
		}
	}
#endif

#ifdef CONFIG_AUTOMOUNT_ROMFS
	if (partinfo->romfs_partno != -1) {
		snprintf(fs_devname, FS_PATH_MAX, "/dev/mtdblock%d", partinfo->romfs_partno);
		ret = mount(fs_devname, "/rom", "romfs", 0, NULL);
		if (ret != OK) {
			printf("ERROR: mounting '%s'(ROMFS) failed, errno %d\n", fs_devname, get_errno());
		} else {
			printf("%s is mounted successfully @ %s \n", fs_devname, "/rom");
		}
	}
#endif /* CONFIG_AUTOMOUNT_ROMFS */

#ifdef CONFIG_LIBC_ZONEINFO_ROMFS
	if (partinfo->romfs_partno != -1) {
		snprintf(fs_devname, FS_PATH_MAX, "/dev/mtdblock%d", partinfo->timezone_partno);
		ret = mount(fs_devname, CONFIG_LIBC_TZDIR, "romfs", MS_RDONLY, NULL);
		if (ret != OK) {
			printf("ROMFS ERROR: timezone mount failed, errno %d\n", get_errno());
		} else {
			printf("%s is mounted successfully @ %s \n", fs_devname, CONFIG_LIBC_TZDIR);
		}
	}
#endif	/* CONFIG_LIBC_ZONEINFO_ROMFS */
#endif /* CONFIG_AUTOMOUNT_USERFS, CONFIG_AUTOMOUNT_ROMFS, CONFIG_LIBC_ZONEINFO_ROMFS */
}
#endif /* CONFIG_FLASH_PARTITION */
