/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/sidk_s5jt200/src/s5jt200_tash.c
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <syslog.h>
#include <errno.h>

#include <tinyara/board.h>
#include <tinyara/rtc.h>
#include <time.h>
#include <chip.h>

#include "s5j_rtc.h"
#include "up_internal.h"

#include <apps/shell/tash.h>

#include <tinyara/fs/mtd.h>

#include "sidk_s5jt200.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
extern void s5j_i2c_register(int bus);

char *s5j_get_binary_version(uint32_t baddr)
{
	static char version[13];
	version[12] = '\0';
	strncpy(version, (char *)baddr, sizeof(version) - 1);

	return version;
}

#ifdef CONFIG_TASH
#ifdef CONFIG_EXAMPLES_SLSIWIFI
int slsi_wifi_main(int argc, char *argv[]);
#endif

const static tash_cmdlist_t tash_s5j_cmds[] = {
#ifdef CONFIG_EXAMPLES_SLSIWIFI
	{ "artikwifi",		slsi_wifi_main,		TASH_EXECMD_SYNC },
#endif
	{ NULL,			NULL,			0 }
};
#endif

static void scsc_wpa_ctrl_iface_init(void)
{
#ifdef CONFIG_SCSC_WLAN
	int ret;

	ret = mkfifo("/dev/wpa_ctrl_req", 666);
	if (ret != 0 && ret != -EEXIST) {
		lldbg("mkfifo error ret:%d\n", ret);
		return;
	}

	ret = mkfifo("/dev/wpa_ctrl_cfm", 666);
	if (ret != 0 && ret != -EEXIST) {
		lldbg("mkfifo error ret:%d\n", ret);
		return;
	}

	ret = mkfifo("/dev/wpa_monitor", 666);
	if (ret != 0 && ret != -EEXIST) {
		lldbg("mkfifo error ret:%d\n", ret);
		return;
	}
#endif
}

static void sidk_s5jt200_configure_partitions(void)
{
#if defined(CONFIG_SIDK_S5JT200_FLASH_PART)
	int partno;
	int partoffset;
	const char *parts = CONFIG_SIDK_S5JT200_FLASH_PART_LIST;
	const char *types = CONFIG_SIDK_S5JT200_FLASH_PART_TYPE;
#if defined(CONFIG_MTD_PARTITION_NAMES)
	const char *names = CONFIG_SIDK_S5JT200_FLASH_PART_NAME;
#endif
	FAR struct mtd_dev_s *mtd;
	FAR struct mtd_geometry_s geo;

	mtd = progmem_initialize();
	if (!mtd) {
		lldbg("ERROR: progmem_initialize failed\n");
		return;
	}

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		lldbg("ERROR: mtd->ioctl failed\n");
		return;
	}

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

		mtd_part = mtd_partition(mtd, partoffset,
					partsize / geo.erasesize, 0);
		partoffset += partsize / geo.erasesize;

		if (!mtd_part) {
			lldbg("ERROR: failed to create partition.\n");
			return;
		}

#if defined(CONFIG_MTD_FTL)
		if (!strncmp(types, "ftl,", 4)) {
			if (ftl_initialize(partno, mtd_part)) {
				lldbg("ERROR: failed to initialise mtd ftl errno :%d\n", errno);
			}
		} else
#endif
#if defined(CONFIG_MTD_CONFIG)
		if (!strncmp(types, "config,", 7)) {
			mtdconfig_register(mtd_part);
		} else
#endif
#if defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS)
		if (!strncmp(types, "smartfs,", 8)) {
			char partref[4];
			sprintf(partref, "p%d", partno);
			smart_initialize(CONFIG_SIDK_S5JT200_FLASH_MINOR,
					mtd_part, partref);
		} else
#endif
		{
		}

#if defined(CONFIG_MTD_PARTITION_NAMES)
		if (strcmp(names, ""))
			mtd_setpartitionname(mtd_part, names);

		while (*names != ',' && *names) names++;
		if (*names == ',') names++;
#endif

		while (*parts != ',' && *parts) parts++;
		if (*parts == ',') parts++;

		while (*types != ',' && *types) types++;
		if (*types == ',') types++;

		partno++;
	}
#endif /* CONFIG_SIDK_S5JT200_FLASH_PART */
}

/****************************************************************************
 * Name: board_app_initialize
 *
 * Description:
 *   Perform application specific initialization. THis function is never
 *   called directly from application code, but only indirectly via the
 *   (non-standard) boardctl() interface using the command BOARDIOC_INIT.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_EEPROM_TEST
int ee_test_main(int argc, char **args);
#endif

int board_app_initialize(void)
{
	int ret;

	sidk_s5jt200_configure_partitions();

#ifdef CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME
	/* Initialize and mount user partition (if we have) */
	ret = mksmartfs(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME, false);
	if (ret != OK) {
		lldbg("ERROR: mksmartfs on %s failed",
				CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME);
	} else {
		ret = mount(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME,
				CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_MOUNTPOINT,
				"smartfs", 0, NULL);
		if (ret != OK)
			lldbg("ERROR: mounting '%s' failed\n",
				CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME);
	}
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME */

#ifdef CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME
	/* Initialize and mount sssrw partition (if we have) */
	ret = mksmartfs(CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME, false);
	if (ret != OK) {
		lldbg("ERROR: mksmartfs on %s failed",
				CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME);
	} else {
		ret = mount(CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME,
				CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_MOUNTPOINT,
				"smartfs", 0, NULL);
		if (ret != OK)
			lldbg("ERROR: mounting '%s' failed\n",
				CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME);
	}
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME */

#ifdef CONFIG_FS_PROCFS
	/* Mount the procfs file system */
	ret = mount(NULL, SIDK_S5JT200_PROCFS_MOUNTPOINT, "procfs", 0, NULL);
	if (ret < 0) {
		lldbg("Failed to mount procfs at %s: %d\n",
				SIDK_S5JT200_PROCFS_MOUNTPOINT, ret);
	}
#endif

#ifdef CONFIG_S5J_I2C
	s5j_i2c_register(0);
	s5j_i2c_register(1);
#endif

#if defined(CONFIG_RTC)
	{
		struct tm tp;

		up_rtc_getdatetime(&tp);
		lldbg("RTC getdatetime %d/%d/%d/%d/%d/%d\n",
				tp.tm_year + 1900, tp.tm_mon + 1,
				tp.tm_mday, tp.tm_hour, tp.tm_min, tp.tm_sec);
		lldbg("Version Info :\n");
		lldbg("tinyARA %s\n", __TIMESTAMP__);
	}
#if defined(CONFIG_RTC_DRIVER)
	{
		struct rtc_lowerhalf_s *rtclower;

		rtclower = s5j_rtc_lowerhalf();
		if (rtclower) {
			ret = rtc_initialize(0, rtclower);
			if (ret < 0) {
				lldbg("Failed to register the RTC driver: %d\n",
						ret);
			}
		}
	}
#endif /* CONFIG_RTC_DRIVER */
#endif /* CONFIG_RTC */

	scsc_wpa_ctrl_iface_init();

#ifdef CONFIG_TASH
	tash_cmdlist_install(tash_s5j_cmds);
#endif

#ifdef CONFIG_EXAMPLES_EEPROM_TEST
        ee_test_main(0, NULL);
#endif

	/* to suppress a compiler warning */
	UNUSED(ret);

	return OK;
}
