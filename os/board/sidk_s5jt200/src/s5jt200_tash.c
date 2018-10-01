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
 * board/sidk_s5jt200/src/s5jt200_tash.c
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
#include "s5j_adc.h"
#include "up_internal.h"

#include <apps/shell/tash.h>

#include <tinyara/fs/mtd.h>

#include "sidk_s5jt200.h"
#include "s5j_mct.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
#define SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME"d1"
#else
#define SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME
#endif
/****************************************************************************
 * Private Variables
 ****************************************************************************/
#if defined(CONFIG_FS_ROMFS)
static uint16_t	rommtd_partno;
static bool	rommtd_device_exist = false;
#endif

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
					partsize / geo.blocksize, partno);
		partoffset += partsize / geo.blocksize;

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
			snprintf(partref, sizeof(partref), "p%d", partno);
			smart_initialize(CONFIG_SIDK_S5JT200_FLASH_MINOR,
					mtd_part, partref);
		} else
#endif
#if defined(CONFIG_MTD_FTL) && defined(CONFIG_FS_ROMFS)
		if (!strncmp(types, "romfs,", 6)) {
			if (ftl_initialize(partno, mtd_part)) {
				lldbg("ERROR: failed to initialise mtd ftl errno :%d\n", errno);
			} else {
				rommtd_partno = partno;
				rommtd_device_exist = true;
			}
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
 * Name: sidk_s5jt200_adc_setup
 *
 * Description:
 *   Initialize ADC and register the ADC driver.
 *
 ****************************************************************************/
int sidk_s5jt200_adc_setup(void)
{
#ifdef CONFIG_S5J_ADC
	int ret;
	struct adc_dev_s *adc;
	uint8_t chanlist[] = {
		adc_channel_0,
		adc_channel_1,
		adc_channel_2,
		adc_channel_3,
	};

	/* Get an instance of the ADC interface */
	adc = s5j_adc_initialize(chanlist, sizeof(chanlist));
	if (adc == NULL) {
		return -ENODEV;
	}

	/* Register the ADC driver at "/dev/adc0" */
	ret = adc_register("/dev/adc0", adc);
	if (ret < 0) {
		return ret;
	}
#endif /* CONFIG_S5J_ADC */

	return OK;
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
#ifdef CONFIG_SIDK_S5JT200_AUTOMOUNT
#if defined(CONFIG_RAMMTD) && defined(CONFIG_FS_SMARTFS)
	int bufsize = CONFIG_RAMMTD_ERASESIZE * CONFIG_SIDK_S5JT200_RAMMTD_NEBLOCKS;
	static uint8_t *rambuf;
	struct mtd_dev_s *mtd;
#endif /* CONFIG_RAMMTD */
#if defined(CONFIG_FS_ROMFS)
	char rommtd_devname[16];
#endif
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT */

	sidk_s5jt200_configure_partitions();

#ifdef CONFIG_SIDK_S5JT200_AUTOMOUNT
#if defined(CONFIG_SIDK_S5JT200_AUTOMOUNT_ROMFS)
	if (rommtd_device_exist) {
		snprintf(rommtd_devname, 16, "/dev/mtdblock%d", rommtd_partno);

		ret = mount(rommtd_devname,
				CONFIG_SIDK_S5JT200_AUTOMOUNT_ROMFS_MOUNTPOINT,
				"romfs", 0, NULL);

		if (ret != OK) {
			lldbg("ERROR: mounting '%s'(ROMFS) failed\n",
				rommtd_devname);
		}
	}
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT_ROMFS */

#ifdef CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS
	/* Initialize and mount user partition (if we have) */
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = mksmartfs(SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME, 1, false);
#else
	ret = mksmartfs(SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME, false);
#endif
	if (ret != OK) {
		lldbg("ERROR: mksmartfs on %s failed",
				SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME);
	} else {
		ret = mount(SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME,
				CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_MOUNTPOINT,
				"smartfs", 0, NULL);
		if (ret != OK)
			lldbg("ERROR: mounting '%s' failed\n",
				SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME);
	}
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS */

#ifdef CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW
	/* Initialize and mount sssrw partition (if we have) */
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = mksmartfs(CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME, 1, false);
#else
	ret = mksmartfs(CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW_DEVNAME, false);
#endif
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
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT_SSSRW */

#if defined(CONFIG_RAMMTD) && defined(CONFIG_FS_SMARTFS)
	rambuf = (uint8_t *)malloc(bufsize);

	mtd = rammtd_initialize(rambuf, bufsize);
	if (!mtd) {
		lldbg("ERROR: FAILED TO CREATE RAM MTD INSTANCE\n");
		free(rambuf);
	} else {
		if (smart_initialize(CONFIG_SIDK_S5JT200_RAMMTD_DEV_NUMBER, mtd, NULL) < 0) {
			lldbg("ERROR: FAILED TO smart_initialize\n");
			free(rambuf);
		} else {
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
			ret = mksmartfs(CONFIG_SIDK_S5JT200_RAMMTD_DEV_POINT, 1, false);
#else
			ret = mksmartfs(CONFIG_SIDK_S5JT200_RAMMTD_DEV_POINT, false);
#endif
			if (ret != OK) {
				lldbg("ERROR: mksmartfs on %s failed", CONFIG_SIDK_S5JT200_RAMMTD_DEV_POINT);
				free(rambuf);
			} else {
				ret = mount(CONFIG_SIDK_S5JT200_RAMMTD_DEV_POINT, CONFIG_SIDK_S5JT200_RAMMTD_MOUNT_POINT, "smartfs", 0, NULL);
				if (ret < 0) {
					lldbg("ERROR: Failed to mount the SMART volume: %d\n", errno);
					free(rambuf);
				}
			}
		}
	}
#endif /* CONFIG_RAMMTD */
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT */

#ifdef CONFIG_S5J_I2C
	s5j_i2c_register(0);
	s5j_i2c_register(1);
#endif

#if defined(CONFIG_RTC)
	{
		struct tm tp;

		up_rtc_getdatetime(&tp);
		lldbg("RTC getdatetime %d/%d/%d/%d/%d/%d\n",
				tp.tm_year + TM_YEAR_BASE, tp.tm_mon + 1,
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

	sidk_s5jt200_adc_setup();

	scsc_wpa_ctrl_iface_init();

#ifdef CONFIG_TIMER
	{
		int  i;
		char path[CONFIG_PATH_MAX];

		for (i = 0; i < CONFIG_S5J_MCT_NUM; i++) {
			snprintf(path, sizeof(path), "/dev/timer%d", i);
			s5j_timer_initialize(path, i);
		}
	}
#endif

#ifdef CONFIG_EXAMPLES_EEPROM_TEST
	ee_test_main(0, NULL);
#endif

	/* to suppress a compiler warning */
	UNUSED(ret);

	return OK;
}
