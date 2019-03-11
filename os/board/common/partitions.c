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

#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>

void configure_partitions(void)
{
#if defined(CONFIG_FLASH_PARTITION)
	int partno;
	int partoffset;
	const char *parts = CONFIG_FLASH_PART_SIZE;
	const char *types = CONFIG_FLASH_PART_TYPE;
#if defined(CONFIG_MTD_PARTITION_NAMES)
	const char *names = CONFIG_FLASH_PART_NAME;
#endif
	FAR struct mtd_dev_s *mtd;
	FAR struct mtd_geometry_s geo;

#ifdef CONFIG_MTD_PROGMEM
	mtd = progmem_initialize();
	if (!mtd) {
		lldbg("ERROR: progmem_initialize failed\n");
		return;
	}

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		lldbg("ERROR: mtd->ioctl failed\n");
		return;
	}
#else
	mtd = up_flashinitialize();
	if (!mtd) {
		lldbg("ERROR : up_flashinitializ failed\n");
		return;
	}

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		lldbg("ERROR: mtd->ioctl failed\n");
		return;
	}
#endif
	partno = 0;
	partoffset = 0;

	while (*parts) {
		FAR struct mtd_dev_s *mtd_part;
		int partsize;

		partsize = strtoul(parts, NULL, 0) << 10;

		if (partsize < geo.erasesize) {
			lldbg("ERROR: Partition size is lesser than erasesize\n");
			return;
		}

		if (partsize % geo.erasesize != 0) {
			lldbg("ERROR: Partition size is not multiple of erasesize\n");
			return;
		}

		mtd_part = mtd_partition(mtd, partoffset, partsize / geo.blocksize, partno);
		partoffset += partsize / geo.blocksize;

		if (!mtd_part) {
			lldbg("ERROR: failed to create partition.\n");
			return;
		}
#ifdef CONFIG_MTD_FTL
		if (!strncmp(types, "ftl,", 4)
#ifdef CONFIG_FS_ROMFS
		|| !strncmp(types, "romfs,", 6)
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
			smart_initialize(CONFIG_FLASH_MINOR, mtd_part, partref);
		} else
#endif
		{
		}

#if defined(CONFIG_MTD_PARTITION_NAMES)
		if (*names) {
			mtd_setpartitionname(mtd_part, names);
		}

		while (*names) {
			if (*names == ',') {
				names++;
				break;
			}
			names++;
		}
#endif

		while (*parts) {
			if (*parts == ',') {
				parts++;
				break;
			}
			parts++;
		}

		while (*types) {
			if (*types == ',') {
				types++;
				break;
			}
			types++;
		}

		partno++;
	}
#endif /* CONFIG_FLASH_PARTITION */
}
