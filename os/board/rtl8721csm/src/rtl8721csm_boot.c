/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/fs/mtd.h>
#include <../include/debug.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mount.h>

#include <tinyara/fs/mksmartfs.h>
#include <tinyara/board.h>
#ifdef CONFIG_FLASH_PARTITION
#include <tinyara/fs/mtd.h>
#endif
#ifdef CONFIG_PRODCONFIG
#include <tinyara/prodconfig.h>
#include <sys/types.h>
#endif
#include <arch/board/board.h>
#include "gpio_api.h"
#include "timer_api.h"
#include "amebad_i2c.h"
#include "amebad_spi.h"
#ifdef CONFIG_FLASH_PARTITION
#include "common.h"
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
extern FAR struct gpio_lowerhalf_s *amebad_gpio_lowerhalf(u32 pinname, u32 pinmode, u32 pinpull);
#ifdef CONFIG_PRODCONFIG
extern u32 EFUSERead8(u32 CtrlSetting, u32 Addr, u8 *Data, u8 L25OutVoltage);
#endif
#if defined(CONFIG_AUDIO_I2SCHAR) && defined(CONFIG_AMEBAD_I2S)
extern int i2schar_devinit(void);
#endif

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/
#ifdef CONFIG_PRODCONFIG
static int up_check_prod(void)
{
	u8 prod_disable;
	EFUSERead8(0, EFUSE_SEC_CONFIG_ADDR1, &prod_disable, L25EOUTVOLTAGE);
	if (prod_disable == 0xff) {
		return OK;
	}
	return ERROR;
}
int up_check_prodswd(void)
{
	return up_check_prod();
}
int up_check_proddownload(void)
{
	return up_check_prod();
}
#endif

void board_i2s_initialize(void)
{
#if defined(CONFIG_AUDIO_I2SCHAR) && defined(CONFIG_AMEBAD_I2S)
	i2schar_devinit();
#endif
}

void board_spi_initialize(void)
{
#ifdef CONFIG_SPI
	struct spi_dev_s *spi;
	spi = up_spiinitialize(1);

#ifdef CONFIG_SPI_USERIO
	if (spi_uioregister(1, spi) < 0) {
		lldbg("Failed to register SPI1\n");
	}
#endif
#endif
}

void board_i2c_initialize(void)
{
#ifdef CONFIG_I2C
	FAR struct i2c_dev_s *i2c;
	int bus = 0;
	int ret;
	char path[16];

	snprintf(path, sizeof(path), "/dev/i2c-%d", bus);
	i2c = up_i2cinitialize(bus);
#ifdef CONFIG_I2C_USERIO
	if (i2c) {
		ret = i2c_uioregister(path, i2c);
		if (ret < 0) {
			up_i2cuninitialize(i2c);
		}
	}
#endif
#endif
}

void board_gpio_initialize(void)
{
#ifdef CONFIG_GPIO
	int i;
	struct gpio_lowerhalf_s *lower;

	struct {
		u32 pinname;
		u32 pinmode;
		u32 pinpull;
	} pins[] = {
		{
			PA_0, PIN_OUTPUT, PullNone
		}, {
			PA_1, PIN_OUTPUT, PullNone
		}, {
			PA_2, PIN_OUTPUT, PullNone
		}, {
			PA_3, PIN_OUTPUT, PullNone
		}, {
			PB_5, PIN_OUTPUT, PullNone
		},
		/*		{PA_6, PIN_OUTPUT, PullNone},
				{PA_7, PIN_OUTPUT, PullNone},
				{PA_8, PIN_OUTPUT, PullNone},
				{PA_9, PIN_OUTPUT, PullNone},
				{PA_10, PIN_OUTPUT, PullNone},
				{PA_11, PIN_OUTPUT, PullNone},
				{PA_12, PIN_OUTPUT, PullNone},
				{PA_13, PIN_OUTPUT, PullNone},
				{PA_14, PIN_OUTPUT, PullNone},
				{PA_15, PIN_OUTPUT, PullNone},
				{PA_16, PIN_OUTPUT, PullNone},
				{PA_17, PIN_OUTPUT, PullNone},
				{PA_18, PIN_OUTPUT, PullNone},
				{PA_19, PIN_OUTPUT, PullNone},
				{PA_20, PIN_OUTPUT, PullNone},
				{PA_21, PIN_OUTPUT, PullNone},
				{PA_22, PIN_OUTPUT, PullNone},
				{PA_23, PIN_OUTPUT, PullNone},
				{PA_24, PIN_OUTPUT, PullNone},
				{PA_25, PIN_OUTPUT, PullNone},
				{PA_26, PIN_OUTPUT, PullNone},
				{PB_27, PIN_OUTPUT, PullNone},
				{PA_28, PIN_OUTPUT, PullNone},
				{PA_29, PIN_OUTPUT, PullNone},
				{PA_30, PIN_OUTPUT, PullNone},
				{PA_31, PIN_OUTPUT, PullNone},
				{PB_0, PIN_OUTPUT, PullNone},
				{PB_1, PIN_OUTPUT, PullNone},
				{PB_2, PIN_OUTPUT, PullNone},
				{PB_3, PIN_OUTPUT, PullNone},
				{PB_4, PIN_OUTPUT, PullNone},
				{PB_5, PIN_OUTPUT, PullNone},
				{PB_6, PIN_OUTPUT, PullNone},
				{PB_7, PIN_OUTPUT, PullNone},
				{PB_8, PIN_OUTPUT, PullNone},
				{PB_9, PIN_OUTPUT, PullNone},
				{PB_10, PIN_OUTPUT, PullNone},
				{PB_11, PIN_OUTPUT, PullNone},
				{PB_12, PIN_OUTPUT, PullNone},
				{PB_13, PIN_OUTPUT, PullNone},
				{PB_14, PIN_OUTPUT, PullNone},
				{PB_15, PIN_OUTPUT, PullNone},
				{PB_16, PIN_OUTPUT, PullNone},
				{PB_17, PIN_OUTPUT, PullNone},
				{PB_18, PIN_OUTPUT, PullNone},
				{PB_19, PIN_OUTPUT, PullNone},
				{PB_20, PIN_OUTPUT, PullNone},
				{PB_21, PIN_OUTPUT, PullNone},
				{PB_22, PIN_OUTPUT, PullNone},
				{PB_23, PIN_OUTPUT, PullNone},
				{PB_24, PIN_OUTPUT, PullNone},
				{PB_25, PIN_OUTPUT, PullNone},
				{PB_26, PIN_OUTPUT, PullNone},
				{PB_27, PIN_OUTPUT, PullNone},
				{PB_28, PIN_OUTPUT, PullNone},
				{PB_29, PIN_OUTPUT, PullNone},
				{PB_30, PIN_OUTPUT, PullNone},
				{PB_31, PIN_OUTPUT, PullNone}, */
	};

	for (i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		lower = amebad_gpio_lowerhalf(pins[i].pinname, pins[i].pinmode, pins[i].pinpull);
		gpio_register(pins[i].pinname, lower);
	}
#endif
}

void amebad_mount_partitions(void)
{
#ifdef CONFIG_FLASH_PARTITION
	int ret;
	struct mtd_dev_s *mtd;
	partition_info_t partinfo;

	mtd = (FAR struct mtd_dev_s *)mtd_initialize();
	/* Configure mtd partitions */
	ret = configure_mtd_partitions(mtd, &g_flash_part_data, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions failed\n");
		return;
	}

#ifdef CONFIG_AUTOMOUNT
	automount_fs_partition(&partinfo);
#endif

#endif /* CONFIG_FLASH_PARTITION */
}

#ifdef CONFIG_FTL_ENABLED
extern const u8 ftl_phy_page_num;
extern const u32 ftl_phy_page_start_addr;
static void app_ftl_init(void)
{
	ftl_init(ftl_phy_page_start_addr, ftl_phy_page_num);
}
#endif

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
	SOCPS_InitSYSIRQ_HP();
	memcpy_gdma_init();
	if (wifi_config.wifi_ultra_low_power &&
		wifi_config.wifi_app_ctrl_tdma == FALSE) {
		SystemSetCpuClk(CLK_KM4_100M);
	}

	InterruptRegister(IPC_INTHandler, IPC_IRQ, (u32)IPCM0_DEV, 5);
	InterruptEn(IPC_IRQ, 5);
	/* init console */
	shell_recv_all_data_onetime = 1;
	shell_init_rom(0, 0);
	//shell_init_ram();
	ipc_table_init();
#ifdef CONFIG_FTL_ENABLED
	app_ftl_init();
#endif
	amebad_mount_partitions();
	board_gpio_initialize();
	board_i2c_initialize();
	board_spi_initialize();
	board_i2s_initialize();
#ifdef CONFIG_WATCHDOG
	amebad_wdg_initialize(CONFIG_WATCHDOG_DEVPATH, 5000);
#endif
#ifdef CONFIG_TIMER
	int i;
	char path[CONFIG_PATH_MAX];
	for (i = 0; i < GTIMER_MAX; i++) {
		snprintf(path, sizeof(path), "/dev/timer%d", i);
		amebad_timer_initialize(path, i);
	}
#endif

#ifdef CONFIG_RTC_DRIVER
	struct rtc_lowerhalf_s *rtclower;
	int ret;

	rtclower = amebad_rtc_lowerhalf();
	if (rtclower) {
		ret = rtc_initialize(0, rtclower);
		if (ret < 0) {
			lldbg("Failed to register the RTC driver: %d\n", ret);
		}
	}
#endif
}
#else
#error "CONFIG_BOARD_INITIALIZE MUST ENABLE"
#endif

int board_app_initialize(void)
{
	return OK;
}
