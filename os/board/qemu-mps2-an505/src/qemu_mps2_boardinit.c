/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <tinyara/board.h>
#include <tinyara/fs/mtd.h>
#include <debug.h>
#include <errno.h>

#include <arch/chip/chip.h>
#include "common.h"

#ifdef CONFIG_QEMU_MPS2_AN505_LOADABLE_LOADER
extern int qemu_mps2_loader_initialize(void);
#endif
extern struct mtd_dev_s *qemu_mps2_flash_initialize(uintptr_t start, size_t size);
extern int get_partition_num(char *part);

static void qemu_mps2_debug_probe_bootparam(void)
{
#if defined(CONFIG_BINARY_MANAGER) && defined(CONFIG_USE_BP)
	char devpath[32];
	int part_num;
	int fd;

	part_num = get_partition_num("bootparam");
	lldbg("qemu-mps2: bootparam partition probe part_num=%d\n", part_num);
	if (part_num < 0) {
		return;
	}

	snprintf(devpath, sizeof(devpath), "/dev/mtdblock%d", part_num);
	fd = open(devpath, O_RDWR);
	if (fd < 0) {
		lldbg("qemu-mps2: bootparam probe open failed path=%s errno=%d\n",
		      devpath, errno);
		return;
	}

	lldbg("qemu-mps2: bootparam probe open success path=%s fd=%d\n",
	      devpath, fd);
	close(fd);
#endif
}

static void qemu_mps2_profile_init_flash_partitions(void)
{
#ifdef CONFIG_FLASH_PARTITION
	static bool g_partitions_ready;
	struct mtd_dev_s *mtd;
	partition_info_t partinfo;

	if (g_partitions_ready) {
		lldbg("qemu-mps2: flash partitions already initialized\n");
		return;
	}

	lldbg("qemu-mps2: flash init start base=%p size=%u\n",
	      (FAR void *)QEMU_MPS2_AN505_FLASH_IMAGE_BASE,
	      (unsigned int)CONFIG_FLASH_SIZE);
	mtd = qemu_mps2_flash_initialize(QEMU_MPS2_AN505_FLASH_IMAGE_BASE,
					 CONFIG_FLASH_SIZE);
	if (!mtd) {
		lldbg("ERROR: failed to initialize qemu-mps2 flash image\n");
		return;
	}
	lldbg("qemu-mps2: flash init done\n");

	lldbg("qemu-mps2: configure_mtd_partitions start\n");
	if (configure_mtd_partitions(mtd, 0, &partinfo) != OK) {
		lldbg("ERROR: failed to configure qemu-mps2-an505 flash partitions\n");
		return;
	}
	lldbg("qemu-mps2: configure_mtd_partitions done smartfs=%d romfs=%d littlefs=%d timezone=%d minor=%d\n",
	      partinfo.smartfs_partno, partinfo.romfs_partno,
	      partinfo.littlefs_partno, partinfo.timezone_partno, partinfo.minor);

#if defined(CONFIG_AUTOMOUNT) && defined(CONFIG_AUTOMOUNT_USERFS)
	lldbg("qemu-mps2: automount_fs_partition start\n");
	automount_fs_partition(&partinfo);
	lldbg("qemu-mps2: automount_fs_partition done\n");
#endif

	g_partitions_ready = true;
	lldbg("qemu-mps2: flash partitions ready\n");
	qemu_mps2_debug_probe_bootparam();
#endif
}

static void qemu_mps2_profile_init_loader(void)
{
#ifdef CONFIG_QEMU_MPS2_AN505_LOADABLE_LOADER
	lldbg("qemu-mps2: loader init start\n");
	if (qemu_mps2_loader_initialize() != OK) {
		lldbg("ERROR: qemu-mps2-an505 loader init failed\n");
	} else {
		lldbg("qemu-mps2: loader init done\n");
	}
#endif
}

#ifdef CONFIG_BOARD_INITIALIZE
void board_initialize(void)
{
	qemu_mps2_profile_init_flash_partitions();
	qemu_mps2_profile_init_loader();
}
#else
#error "CONFIG_BOARD_INITIALIZE must be enabled for qemu-mps2-an505"
#endif

int board_app_initialize(void)
{
	return OK;
}

#ifdef CONFIG_BOARDCTL_IOCTL
int board_ioctl(unsigned int cmd, uintptr_t arg)
{
	UNUSED(cmd);
	UNUSED(arg);
	return -ENOTTY;
}
#endif
