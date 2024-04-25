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
/****************************************************************************
 * board/artik05x/src/artik05x_tash.c
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
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <tinyara/board.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mksmartfs.h>
#include <tinyara/rtc.h>
#include <tinyara/analog/adc.h>
#include <tinyara/configdata.h>
#include <chip.h>

#include "s5j_adc.h"
#include "s5j_rtc.h"
#include "s5j_mct.h"
#include "up_internal.h"

#include <apps/shell/tash.h>

#include "artik05x.h"
#include "common.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
#define ARTIK05X_AUTOMOUNT_USERFS_DEVNAME  CONFIG_ARTIK05X_AUTOMOUNT_USERFS_DEVNAME"d1"
#else
#define ARTIK05X_AUTOMOUNT_USERFS_DEVNAME  CONFIG_ARTIK05X_AUTOMOUNT_USERFS_DEVNAME
#endif
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: artik05x_adc_setup
 *
 * Description:
 *   Initialize ADC and register the ADC driver.
 *
 ****************************************************************************/
int artik05x_adc_setup(void)
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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_app_initialize
 *
 * Description:
 *   Perform architecture specific initialization
 *
 ****************************************************************************/
int board_app_initialize(void)
{
	int ret;
#ifdef CONFIG_FLASH_PARTITION
	struct mtd_dev_s *mtd;
	partition_info_t partinfo;
#if defined(CONFIG_AUTOMOUNT) && defined(CONFIG_RAMMTD) && defined(CONFIG_FS_SMARTFS)
	int bufsize;
	static uint8_t *rambuf;
#endif /* CONFIG_RAMMTD */

	mtd = (FAR struct mtd_dev_s *)mtd_initialize();
	/* Configure mtd partitions */
	ret = configure_mtd_partitions(mtd, 0, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions failed.\n");
		return ERROR;
	}

#ifdef CONFIG_AUTOMOUNT
	automount_fs_partition(&partinfo);

#ifdef CONFIG_ARTIK05X_AUTOMOUNT_SSSRW
	/* Initialize and mount secure storage partition (if we have) */
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = mksmartfs(CONFIG_ARTIK05X_AUTOMOUNT_SSSRW_DEVNAME, 1, false);
#else
	ret = mksmartfs(CONFIG_ARTIK05X_AUTOMOUNT_SSSRW_DEVNAME, false);
#endif
	if (ret != OK) {
		lldbg("ERROR: mksmartfs on %s failed\n",
				CONFIG_ARTIK05X_AUTOMOUNT_SSSRW_DEVNAME);
	} else {
		ret = mount(CONFIG_ARTIK05X_AUTOMOUNT_SSSRW_DEVNAME,
				CONFIG_ARTIK05X_AUTOMOUNT_SSSRW_MOUNTPOINT,
				"smartfs", 0, NULL);
		if (ret != OK) {
			lldbg("ERROR: mounting '%s' failed\n",
					CONFIG_ARTIK05X_AUTOMOUNT_SSSRW_DEVNAME);
		}
	}
#endif /* CONFIG_ARTIK05X_AUTOMOUNT_SSSRW */

#if defined(CONFIG_RAMMTD) && defined(CONFIG_FS_SMARTFS)
	bufsize = CONFIG_RAMMTD_ERASESIZE * CONFIG_ARTIK05X_RAMMTD_NEBLOCKS;
	rambuf = (uint8_t *)kmm_malloc(bufsize);
	if (!rambuf) {
		lldbg("ERROR: FAILED TO allocate RAMMTD\n");
	} else {
		mtd = rammtd_initialize(rambuf, bufsize);
		if (!mtd) {
			lldbg("ERROR: FAILED TO CREATE RAM MTD INSTANCE\n");
			kmm_free(rambuf);
		} else {
			ret = smart_initialize(CONFIG_ARTIK05X_RAMMTD_DEV_NUMBER, mtd, NULL);
			if (ret < 0) {
				lldbg("ERROR: FAILED TO smart_initialize\n");
				kmm_free(rambuf);
			} else {
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
				ret = mksmartfs(CONFIG_ARTIK05X_RAMMTD_DEV_POINT, 1, false);
#else
				ret = mksmartfs(CONFIG_ARTIK05X_RAMMTD_DEV_POINT, false);
#endif
				if (ret != OK) {
					lldbg("ERROR: mksmartfs on %s failed\n", CONFIG_ARTIK05X_RAMMTD_DEV_POINT);
					kmm_free(rambuf);
				} else {
					ret = mount(CONFIG_ARTIK05X_RAMMTD_DEV_POINT, CONFIG_ARTIK05X_RAMMTD_MOUNT_POINT, "smartfs", 0, NULL);
					if (ret < 0) {
						lldbg("ERROR: Failed to mount the SMART volume: %d\n", errno);
						kmm_free(rambuf);
					}
				}
			}
		}
	}
#endif /* CONFIG_RAMMTD */
#endif /* CONFIG_AUTOMOUNT */
#endif /* CONFIG_FLASH_PARTITION */

#if defined(CONFIG_RTC_DRIVER)
	{
		struct rtc_lowerhalf_s *rtclower;

		rtclower = s5j_rtc_lowerhalf();
		if (rtclower) {
			ret = rtc_initialize(0, rtclower);
			if (ret < 0) {
				lldbg("Failed to register the RTC driver: %d\n", ret);
			}
		}
	}
#endif /* CONFIG_RTC_DRIVER */

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

	artik05x_adc_setup();

	scsc_wpa_ctrl_iface_init();

	UNUSED(ret);

	return OK;
}
