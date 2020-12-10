/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
 * os/board/imxrt1020-evk/src/imxrt_bringup.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name TinyARA nor the names of its contributors may be
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

#include <debug.h>

#include <tinyara/configdata.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mksmartfs.h>

#include <syslog.h>
#include <tinyara/i2c.h>
#include <tinyara/pwm.h>
#include "imxrt_lpi2c.h"
#include "imxrt_pwm.h"
#include "imxrt_log.h"

#if defined(CONFIG_FLASH_PARTITION)
#include "common.h"
#endif
#include "imxrt1020-evk.h"
#include "imxrt_usbhost.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
#define IMXRT_AUTOMOUNT_USERFS_DEVNAME  CONFIG_IMXRT_AUTOMOUNT_USERFS_DEVNAME"d1"
#else
#define IMXRT_AUTOMOUNT_USERFS_DEVNAME  CONFIG_IMXRT_AUTOMOUNT_USERFS_DEVNAME
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(CONFIG_I2C)
static void imxrt_i2c_register(int bus)
{
	FAR struct i2c_master_s *i2c;
	int ret;
	char path[16];

	snprintf(path, sizeof(path), "/dev/i2c-%d", bus - 1);
	i2c = up_i2cinitialize(bus);
	if (i2c == NULL) {
		syslog("ERROR: Failed to get I2C%d interface\n", bus - 1);
	} else {
		ret = i2c_uioregister(path, i2c);
		if (ret < 0) {
			syslog("ERROR: Failed to register I2C%d driver: %d\n", bus - 1, ret);
			up_i2cuninitialize(i2c);
		}
	}
}
#endif

/****************************************************************************
 * Name: board_pwmm_initialize
 *
 * Description:
 *   PWM intialization for imxrt
 *
 ****************************************************************************/
static void imxrt_pwm_register(int ch)
{
#ifdef CONFIG_PWM
	struct pwm_lowerhalf_s *pwm_ch;
	char path[16];
	int ret;

	/* The last two channels(15, 16) multiplex the PINs with LPUART1,
	 * so for debugging they are not be set to PWM PINs */
	pwm_ch = imxrt_pwminitialize(ch);
	if (!pwm_ch) {
		dbg("Failed to get imxrt PWM Channel lower half\n");
		return;
	}

	/* Register the PWM channel driver at "/dev/pwmx" */
	snprintf(path, sizeof(path), "/dev/pwm%d", ch);
	ret = pwm_register(path, pwm_ch);
	if (ret < 0) {
		dbg("Imxrt PWM registeration failure: %d\n", ret);
	}
#endif
	return;
}

void imxrt_filesystem_initialize(void)
{
#ifdef CONFIG_FLASH_PARTITION
	int ret;
	struct mtd_dev_s *mtd;
#if defined(CONFIG_IMXRT_AUTOMOUNT) && defined(CONFIG_RAMMTD) && defined(CONFIG_FS_SMARTFS)
	int bufsize;
	static uint8_t *rambuf;
#endif							/* CONFIG_RAMMTD */

	IMXLOG("imxrt_bringup");

	mtd = (FAR struct mtd_dev_s *)mtd_initialize();
	/* Configure mtd partitions */
	ret = configure_mtd_partitions(mtd, &g_flash_part_data);
	if (ret != OK) {
		IMXLOG("configure_mtd_partitions failed");
	} else {
		IMXLOG("SUCCESS: configure_mtd_partitions");
	}

#ifdef CONFIG_IMXRT_AUTOMOUNT
#ifdef CONFIG_IMXRT_AUTOMOUNT_USERFS
	/* Initialize and mount user partition (if we have) */
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = mksmartfs(IMXRT_AUTOMOUNT_USERFS_DEVNAME, 1, false);
#else
	ret = mksmartfs(IMXRT_AUTOMOUNT_USERFS_DEVNAME, false);
#endif
	if (ret != OK) {
		IMXLOG("USERFS ERROR: mksmartfs failed");
	} else {
		IMXLOG("SUCCESS: mksmartfs");
		ret = mount(IMXRT_AUTOMOUNT_USERFS_DEVNAME, CONFIG_IMXRT_AUTOMOUNT_USERFS_MOUNTPOINT, "smartfs", 0, NULL);
		if (ret != OK) {
			IMXLOG("USERFS ERROR: mounting failed");
		}
	}
#endif							/* CONFIG_IMXRT_AUTOMOUNT_USERFS */

#ifdef CONFIG_IMXRT_AUTOMOUNT_SSSRW
	/* Initialize and mount secure storage partition (if we have) */
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	ret = mksmartfs(CONFIG_IMXRT_AUTOMOUNT_SSSRW_DEVNAME, 1, false);
#else
	ret = mksmartfs(CONFIG_IMXRT_AUTOMOUNT_SSSRW_DEVNAME, false);
#endif
	if (ret != OK) {
		IMXLOG("SSSRW ERROR: mksmartfs failed");
	} else {
		ret = mount(CONFIG_IMXRT_AUTOMOUNT_SSSRW_DEVNAME, CONFIG_IMXRT_AUTOMOUNT_SSSRW_MOUNTPOINT, "smartfs", 0, NULL);
		if (ret != OK) {
			IMXLOG("SSSRW ERROR: mounting failed");
		}
	}
#endif							/* CONFIG_IMXRT_AUTOMOUNT_SSSRW */

#ifdef CONFIG_IMXRT_AUTOMOUNT_ROMFS
	ret = mount(CONFIG_IMXRT_AUTOMOUNT_ROMFS_DEVNAME, CONFIG_IMXRT_AUTOMOUNT_ROMFS_MOUNTPOINT, "romfs", 0, NULL);

	if (ret != OK) {
		IMXLOG("ROMFS ERROR: mounting failed");
	}
#endif

#if defined(CONFIG_RAMMTD) && defined(CONFIG_FS_SMARTFS)
	bufsize = CONFIG_RAMMTD_ERASESIZE * CONFIG_IMXRT_RAMMTD_NEBLOCKS;

	rambuf = (uint8_t *)kmm_malloc(bufsize);
	if (!rambuf) {
		IMXLOG("SMARTFS ERROR: kmm_malloc failed");
	} else {
		mtd = rammtd_initialize(rambuf, bufsize);
		if (!mtd) {
			IMXLOG("SMARTFS ERROR: FAILED TO CREATE RAM MTD INSTANCE");
			kmm_free(rambuf);
		} else {
			ret = smart_initialize(CONFIG_IMXRT_RAMMTD_DEV_NUMBER, mtd, NULL);
			if (ret < 0) {
				IMXLOG("SMARTFS ERROR: smart_initialize failed");
				kmm_free(rambuf);
			} else {
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
				ret = mksmartfs(CONFIG_IMXRT_RAMMTD_DEV_POINT, 1, false);
#else
				ret = mksmartfs(CONFIG_IMXRT_RAMMTD_DEV_POINT, false);
#endif
				if (ret != OK) {
					IMXLOG("SMARTFS ERROR: mksmartfs failed");
					kmm_free(rambuf);
				} else {
					ret = mount(CONFIG_IMXRT_RAMMTD_DEV_POINT, CONFIG_IMXRT_RAMMTD_MOUNT_POINT, "smartfs", 0, NULL);
					if (ret < 0) {
						IMXLOG("SMARTFS ERROR: mount the SMART volume failed");
						kmm_free(rambuf);
					}
				}
			}
		}
	}
#endif							/* CONFIG_RAMMTD */
#ifdef CONFIG_LIBC_ZONEINFO_ROMFS
	ret = mount(CONFIG_IMXRT_AUTOMOUNT_TZDEVNAME, CONFIG_LIBC_TZDIR, "romfs", MS_RDONLY, NULL);

	if (ret != OK) {
		IMXLOG("ROMFS ERROR: mounting failed");
	}
#endif							/* CONFIG_LIBC_ZONEINFO_ROMFS */
#endif							/* CONFIG_IMXRT_AUTOMOUNT */
#endif							/* CONFIG_FLASH_PARTITION */
}

/************************************************************************************
 * Name: imxrt_spidev_initialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the imxrt1020-evk board.
 *
 ************************************************************************************/

void weak_function imxrt_spidev_initialize(void)
{
#if 0							//TO-DO
#ifdef CONFIG_IMXRT_LPSPI1
	(void)imxrt_config_gpio(GPIO_LPSPI1_CS);	/* LPSPI1 chip select */
	(void)imxrt_config_gpio(GPIO_MMCSD_EN);
#endif
#ifdef CONFIG_IMXRT_LPSPI3
	(void)imxrt_config_gpio(GPIO_LPSPI3_CS);	/* LPSPI3 chip select */
#endif
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_bringup
 *
 * Description:
 *   Bring up board features
 *
 ****************************************************************************/
int imxrt_bringup(void)
{
	int ret;

	/* If we got here then perhaps not all initialization was successful, but
	 * at least enough succeeded to bring-up NSH with perhaps reduced
	 * capabilities.
	 */

	imxrt_filesystem_initialize();

#ifdef CONFIG_FS_PROCFS
	/* Mount the procfs file system */

	ret = mount(NULL, "/proc", "procfs", 0, NULL);
	if (ret < 0) {
		syslog(LOG_ERR, "ERROR: Failed to mount procfs at /proc: %d\n", ret);
	}
#endif

#if defined(CONFIG_I2C) && defined(CONFIG_IMXRT_LPI2C1)
	imxrt_i2c_register(1);
#endif
#if defined(CONFIG_I2C) && defined(CONFIG_IMXRT_LPI2C2)
	imxrt_i2c_register(2);
#endif
#if defined(CONFIG_I2C) && defined(CONFIG_IMXRT_LPI2C3)
	imxrt_i2c_register(3);
#endif
#if defined(CONFIG_I2C) && defined(CONFIG_IMXRT_LPI2C4)
	imxrt_i2c_register(4);
#endif

#ifdef CONFIG_PWM
#if defined(CONFIG_IMXRT_PWM_CHANNEL1)
	imxrt_pwm_register(1);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL2)
	imxrt_pwm_register(2);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL3)
	imxrt_pwm_register(3);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL4)
	imxrt_pwm_register(4);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL5)
	imxrt_pwm_register(5);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL6)
	imxrt_pwm_register(6);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL7)
	imxrt_pwm_register(7);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL8)
	imxrt_pwm_register(8);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL9)
	imxrt_pwm_register(9);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL10)
	imxrt_pwm_register(10);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL11)
	imxrt_pwm_register(11);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL12)
	imxrt_pwm_register(12);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL13)
	imxrt_pwm_register(13);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL14)
	imxrt_pwm_register(14);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL15)
	imxrt_pwm_register(15);
#endif
#if defined(CONFIG_IMXRT_PWM_CHANNEL16)
	imxrt_pwm_register(16);
#endif
#endif

#ifdef CONFIG_USBHOST
	/* Initialize USB host operation.  imxrt_usbhost_initialize() starts a thread
	 * will monitor for USB connection and disconnection events.
	 */
#ifdef IMXRT_USB
	uvdbg("IMXRT USB host initialized!\n");
	imxrt_usbhost_service_initialize();
#else
	IMXLOG("Start USB host services\n");
	ret = imxrt_usbhost_initialize();
	if (ret != OK) {
		IMXLOG("ERROR: Failed to start USB host services\n");
		return ret;
	}
#endif
#endif

#ifdef CONFIG_MMCSD_SPI
	/* Initialize SPI-based MMC/SD card support */

	imxrt_spidev_initialize();

#if 0							//TO-DO
	ret = imxrt_mmcsd_spi_initialize(MMCSD_MINOR);
	if (ret < 0) {
		syslog(LOG_ERR, "ERROR: Failed to initialize SD slot %d: %d\n", ret);
	}
#endif
#endif

	UNUSED(ret);
	return OK;
}
