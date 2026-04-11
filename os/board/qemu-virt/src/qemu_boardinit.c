/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * os/board/qemu-virt/src/qemu_boardinit.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES, OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/arch.h>
#include <tinyara/binary_manager.h>
#include <tinyara/board.h>
#include <tinyara/config.h>
#include <tinyara/fs/mtd.h>
#include <errno.h>
#include <sys/boardctl.h>
#include <debug.h>
#include <stdint.h>

#include "common.h"

#ifdef CONFIG_BOARDCTL_IOCTL
#include <arch/board/boardctl.h>
#endif

#include "qemu_cfi.h"
#include "qemu_mtd_cfi.h"

#ifdef CONFIG_QEMU_VIRT_VIRTIO_BLK
#include "virtio/virtio_blk.h"
#endif

#ifdef CONFIG_FTL_ENABLED
extern void app_ftl_init(void);
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void qemu_mount_partitions(void);

static void qemu_virt_profile_mount_storage(void)
{
#ifdef CONFIG_QEMU_VIRT_MOUNT_FLASH_PARTITIONS
	qemu_mount_partitions();
#endif
}

static void qemu_virt_profile_init_ftl(void)
{
#if defined(CONFIG_FTL_ENABLED) && defined(CONFIG_QEMU_VIRT_INIT_FTL)
	app_ftl_init();
#endif
}

static void qemu_virt_profile_init_block_runtime(void)
{
#if defined(CONFIG_QEMU_VIRT_VIRTIO_BLK) && defined(CONFIG_QEMU_VIRT_INIT_VIRTIO_BLK_AT_BOOT)
	if (qemu_virtio_blk_initialize() != OK) {
		lldbg("ERROR: failed to initialize virtio-blk\n");
	}
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

static void qemu_mount_partitions(void)
{
	int ret;
	struct mtd_dev_s *mtd;
	partition_info_t partinfo;

	mtd = cfi_initialize(CONFIG_FLASH_START_ADDR, CONFIG_FLASH_START_ADDR + CONFIG_FLASH_SIZE, 0x04);
	if (!mtd)
	{
		lldbg("ERROR: mtd_initialize failed\n");
		return;
	}

	/* Configure mtd partitions */
	ret = configure_mtd_partitions(mtd, 0, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions for primary flash failed\n");
		return;
	}

#ifdef CONFIG_SECOND_FLASH_PARTITION
	mtd = cfi_initialize(CONFIG_SECOND_FLASH_START_ADDR, CONFIG_SECOND_FLASH_START_ADDR + CONFIG_SECOND_FLASH_SIZE, 0x04);
	if (mtd == NULL) {
		lldbg("CFI Init failed\n");
		return;
	}

	ret = configure_mtd_partitions(mtd, 1, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions for secondary flash failed\n");
		return;
	}
#endif

#ifdef CONFIG_AUTOMOUNT
	automount_fs_partition(&partinfo);
#endif

#ifdef CONFIG_RESOURCE_FS
	if (binary_manager_mount_resource() != OK) {
		lldbg("ERROR: Failed to mount resource\n");
	}
#endif
}

/****************************************************************************
 * Name: board_initialize
 *
 * Description:
 *   If CONFIG_BOARD_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_initialize().  board_initialize() will be
 *   called immediately after up_initialize() is called and just before the
 *   initial application is started.  This additional initialization phase
 *   may be used, for example, to initialize board-specific device drivers.
 *
 ****************************************************************************/

#ifdef CONFIG_BOARD_INITIALIZE

void board_initialize(void)
{

	/* init console */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	shell_init_rom(0, 0);
#endif
	qemu_virt_profile_mount_storage();
	qemu_virt_profile_init_ftl();
	qemu_virt_profile_init_block_runtime();

}
#else
#error "CONFIG_BOARD_INITIALIZE MUST ENABLE"
#endif

int board_app_initialize(void)
{
	return OK;
}

#ifdef CONFIG_BOARDCTL_IOCTL
int board_ioctl(unsigned int cmd, uintptr_t arg)
{
	(void)arg;

	switch (cmd) {
#ifdef CONFIG_QEMU_VIRT_VIRTIO_BLK
	case BIOC_QEMU_VIRTIO_BLK_TEST:
		return test_virtio_blk(0, NULL);
#endif
	default:
		return -EINVAL;
	}
}
#endif
