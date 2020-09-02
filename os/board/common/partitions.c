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

#include <debug.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>
#ifdef CONFIG_BINARY_MANAGER
#include <tinyara/binary_manager.h>
#endif
#include "common.h"

#ifdef CONFIG_FLASH_PARTITION
struct partition_data_s g_flash_part_data = {
	CONFIG_FLASH_PART_TYPE,
	CONFIG_FLASH_PART_SIZE,
	CONFIG_FLASH_MINOR,
#ifdef CONFIG_MTD_PARTITION_NAMES
	CONFIG_FLASH_PART_NAME
#endif
};
#endif

#ifdef CONFIG_SECOND_FLASH_PARTITION
struct partition_data_s g_second_flash_part_data = {
	CONFIG_SECOND_FLASH_PART_TYPE,
	CONFIG_SECOND_FLASH_PART_SIZE,
	CONFIG_SECOND_FLASH_MINOR,
#ifdef CONFIG_MTD_PARTITION_NAMES
	CONFIG_SECOND_FLASH_PART_NAME
#endif
};
#endif

#if defined(CONFIG_FLASH_PARTITION) || defined(CONFIG_SECOND_FLASH_PARTITION)
FAR struct mtd_dev_s *mtd_initialize(void)
{
	FAR struct mtd_dev_s *mtd;
#ifdef CONFIG_MTD_PROGMEM
	mtd = progmem_initialize();
	if (!mtd) {
		lldbg("ERROR: progmem_initialize failed\n");
		return NULL;
	}
#else
	mtd = up_flashinitialize();
	if (!mtd) {
		lldbg("ERROR : up_flashinitializ failed\n");
		return NULL;
	}
#endif
	return mtd;
}

static void type_specific_initialize(int minor, FAR struct mtd_dev_s *mtd_part, int partno, const char *types)
{
#ifdef CONFIG_MTD_FTL
		if (!strncmp(types, "ftl,", 4)
#ifdef CONFIG_FS_ROMFS
		|| !strncmp(types, "romfs,", 6)
#endif
#ifdef CONFIG_BINARY_MANAGER
		|| !strncmp(types, "kernel,", 7) || !strncmp(types, "bin,", 4)
#endif
		) {
			if (ftl_initialize(partno, mtd_part)) {
				lldbg("ERROR: failed to initialise mtd ftl errno :%d\n", errno);
				return;
			}
		} else
#endif

#ifdef CONFIG_MTD_CONFIG
		if (!strncmp(types, "config,", 7)) {
			mtdconfig_register(mtd_part);
		} else
#endif
#if defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS)
		if (!strncmp(types, "smartfs,", 8)) {
			char partref[4];

			snprintf(partref, sizeof(partref), "p%d", partno);
			smart_initialize(minor, mtd_part, partref);
		} else
#endif
		{

		}
}

static void move_to_next_part(const char **par)
{
		/* Move to next part information. */
		while (*(*par)++ != ',');
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
				lldbg("ERROR: Partition name is so long. Please make it smaller than %d\n", MTD_PARTNAME_LEN);
				/* Move to next part name information. */
				move_to_next_part(names);
				break;
			}
		}
		mtd_setpartitionname(mtd_part, part_name);
	}
}
#endif

int configure_mtd_partitions(struct mtd_dev_s *mtd, struct partition_data_s *part_data)
{
	int partno;
	int partoffset;
	char *types;
	char *sizes;
	int minor;
#ifdef CONFIG_MTD_PARTITION_NAMES
	char part_name[MTD_PARTNAME_LEN + 1];
	int index = 0;
	char *names;
#endif
	FAR struct mtd_geometry_s geo;

	if (!mtd || !part_data || !part_data->types || !part_data->sizes) {
		lldbg("ERROR: Invalid partition data is NULL\n");
		return ERROR;
	}
#ifdef CONFIG_MTD_PARTITION_NAMES
	else if (!part_data->names) {
		lldbg("ERROR: Invalid partition data is NULL\n");
		return ERROR;
	}
#endif

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		lldbg("ERROR: mtd->ioctl failed\n");
		return ERROR;
	}

	partno = 0;
	partoffset = 0;
	types = part_data->types;
	sizes = part_data->sizes;
	minor = part_data->minor;
#ifdef CONFIG_MTD_PARTITION_NAMES
	names = part_data->names;
#endif

	while (*sizes) {
		FAR struct mtd_dev_s *mtd_part;
		int partsize;

		partsize = strtoul(sizes, NULL, 0) << 10;

		if (partsize < geo.erasesize) {
			lldbg("ERROR: Partition size is lesser than erasesize\n");
			return ERROR;
		}

		if (partsize % geo.erasesize != 0) {
			lldbg("ERROR: Partition size is not multiple of erasesize\n");
			return ERROR;
		}

		mtd_part = mtd_partition(mtd, partoffset, partsize / geo.blocksize, partno);
		partoffset += partsize / geo.blocksize;

		if (!mtd_part) {
			lldbg("ERROR: failed to create partition.\n");
			return ERROR;
		}

		type_specific_initialize(minor, mtd_part, partno, types);

#ifdef CONFIG_MTD_PARTITION_NAMES
		configure_partition_name(mtd_part, (const char **)&names, &index, part_name);
#ifdef CONFIG_BINARY_MANAGER
		if (!strncmp(types, "kernel,", 7)) {
			binary_manager_register_kpart(partno, partsize);
		}
#endif
#endif
		move_to_next_part((const char **)&sizes);
		move_to_next_part((const char **)&types);
		partno++;
	}

	return OK;
}
#endif // defined(CONFIG_FLASH_PARTITION) || defined(CONFIG_SECOND_FLASH_PARTITION)
