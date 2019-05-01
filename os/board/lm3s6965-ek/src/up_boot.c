/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/************************************************************************************
 * board/lm3s6965-ek/src/up_boot.c
 *
 *   Copyright (C) 2010 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <debug.h>

#include <arch/board/board.h>

#ifdef CONFIG_MTD
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>
#endif

#include "up_arch.h"
#include "up_internal.h"
#include "lm3s6965ek_internal.h"

/************************************************************************************
 * Definitions
 ************************************************************************************/

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: tiva_boardinitialize
 *
 * Description:
 *   All Stellaris architectures must provide the following entry point.  This entry
 *   point is called early in the intitialization -- after all memory has been
 *   configured and mapped but before any devices have been initialized.
 *
 ************************************************************************************/

void tiva_boardinitialize(void)
{
	/* Configure SPI chip selects if 1) SSI is not disabled, and 2) the weak function
	 * lm_ssiinitialize() has been brought into the link.
	 */

	/* The LM3S6965 Eval Kit microSD CS and OLED are on SSI0 (Duh! There is no SSI1) */

#if defined(CONFIG_TIVA_SSI0)	/* || defined(CONFIG_TIVA_SSI1) */
	if (lm_ssiinitialize) {
		lm_ssiinitialize();
	}
#endif

	/* Configure on-board LEDs if LED support has been selected. */

#ifdef CONFIG_ARCH_LEDS
	board_led_initialize();
#endif
}


#ifdef CONFIG_BOARD_INITIALIZE

#if defined(CONFIG_QEMU_SRAM) || defined (CONFIG_QEMU_SDRAM)

#define 	QEMU_SMARTFS_PARTITION_START	(0)
#define 	QEMU_SMARTFS_PARTITION_SIZE	(1024 * 1024) /* 1MB */
#define 	QEMU_SMARTFS_PARTITION_PARTNO	(0)
#define 	QEMU_SMARTFS_PARTITION_MINORNO	(0)
#define 	QEMU_SMARTFS_MOUNT_POINT	"/mnt"

/************************************************************************************
 * Name: board_initialize
 *
 * Description:
 *	Board specific intialization after os app start
 *
 ************************************************************************************/

int board_initialize(void)
{
	int ret;
	char partref[4];
	char devname[16];
#ifdef CONFIG_MTD
	FAR struct mtd_dev_s *mtd;
	FAR struct mtd_geometry_s geo;
	FAR struct mtd_dev_s *mtd_part;
#endif
	int partno = QEMU_SMARTFS_PARTITION_PARTNO;
	int minorno = QEMU_SMARTFS_PARTITION_MINORNO;
	int partoffset = QEMU_SMARTFS_PARTITION_START;
	int partsize = QEMU_SMARTFS_PARTITION_SIZE;

#ifdef CONFIG_MTD
	mtd = up_flashinitialize();

	if (!mtd) {
		lldbg("ERROR : up_flashinitializ failed\n");
		return ERROR;
	}

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		lldbg("ERROR: mtd->ioctl failed\n");
		return ERROR;
	}

#ifdef CONFIG_MTD_PARTITION
	mtd_part = mtd_partition(mtd, partoffset, partsize / geo.blocksize, partno);
	if (!mtd_part) {
		lldbg("ERROR: failed to create partition.\n");
		return ERROR;
	}
#endif /* CONFIG_MTD_PARTITION */

	ret = snprintf(partref, sizeof(partref), "p%d", partno);
	if (ret < 0) {
		lldbg("ERROR: snprintf failed while constructing partref, ret = %d\n", ret);
		return ERROR;
	}

	if (ret >= sizeof(partref)) {
		lldbg("WARNING: snprintf output might have truncated, ret = %d\n", ret);
	}

	ret = snprintf(devname, sizeof(devname), "/dev/smart%d%s", minorno, partref);
	if (ret < 0) {
		lldbg("ERROR: snprintf failed while constructing devname, ret = %d\n", ret);
		return ERROR;
	}

	if (ret >= sizeof(devname)) {
		lldbg("WARNING: snprintf output might have truncated, ret = %d\n", ret);
	}


#if defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS)
	smart_initialize(minorno, mtd_part, partref);
#endif /* defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS) */

#endif /* CONFIG_MTD */

	ret = mksmartfs(devname, false);

	if (ret != OK) {
		lldbg("ERROR: mksmartfs on %s failed, ret = %d\n", devname, ret);
	} else {
		ret = mount(devname, QEMU_SMARTFS_MOUNT_POINT, "smartfs", 0, NULL);
		if (ret != OK) {
			lldbg("ERROR: mounting '%s' failed, ret = %d\n", devname, ret);
		} else {
			lldbg("%s is mounted successfully at %s \n", devname, QEMU_SMARTFS_MOUNT_POINT);
		}
	}

	return OK;
}
#endif /* defined(CONFIG_QEMU_SRAM) || defined (CONFIG_QEMU_SDRAM) */
#endif /* CONFIG_BOARD_INITIALIZE */
