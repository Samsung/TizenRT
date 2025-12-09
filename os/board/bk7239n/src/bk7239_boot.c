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
#include "_otp.h"
#endif
#include <arch/board/board.h>
#include "armino_i2c.h"
#include "armino_spi.h"
#ifdef CONFIG_FLASH_PARTITION
#include "../common/common.h"
#endif

#include <unistd.h>
#include "hal_gpio_types.h"
#include "hal_timer_types.h"
#include <driver/uart.h>
#include <modules/pm.h>

#ifdef CONFIG_WATCHDOG
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/watchdog.h>
#endif

#if defined(CONFIG_TFM_OTP_NSC)
#include "tfm_otp_nsc.h"
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
extern FAR struct gpio_lowerhalf_s *armino_gpio_lowerhalf(u32 pinname, u32 pinmode, u32 pinpull);
#if defined(CONFIG_AUDIO_I2SCHAR) && defined(CONFIG_ARMINO_I2S)
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
	uint32_t prod_disable = 0;
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
    bk_otp_apb_read(OTP_EFUSE, &prod_disable, sizeof(prod_disable));
    if (prod_disable & (1 << 31)) {
		return OK;
	}
	return ERROR;
#else
	bool swd_enabled = true;
	bk_err_t ret = bk_otp_read_swd_nsc(&swd_enabled);
	if (ret != BK_OK) {
		return ERROR;
	}
	if (!swd_enabled) {
		return OK;
	}
	return ERROR;
#endif
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
#if defined(CONFIG_AUDIO_I2SCHAR) && defined(CONFIG_AMRINO_I2S)
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

	extern void bk_iomx_gpio_isr_attach(void);
	bk_iomx_gpio_isr_attach();
#if CONFIG_GPIO

	int i;
	struct gpio_lowerhalf_s *lower;

	struct {
		u32 pin;
		u32 pinmode;
		u32 pinpull;
	} pins[] = {
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
		{
			GPIO_0, GPIO_OUTPUT_ENABLE, GPIO_PULL_DISABLE
		}, {
			GPIO_1, GPIO_OUTPUT_ENABLE, GPIO_PULL_DISABLE
		}, {
			GPIO_2, GPIO_OUTPUT_ENABLE, GPIO_PULL_DISABLE
		},
#endif
		{
			GPIO_5, GPIO_INPUT_ENABLE, GPIO_PULL_UP_EN
		}, {
			GPIO_6, GPIO_INPUT_ENABLE, GPIO_PULL_UP_EN
		},
	};

	for (i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		lower = armino_gpio_lowerhalf(pins[i].pin, pins[i].pinmode, pins[i].pinpull);
		gpio_register(pins[i].pin, lower);
	}
#endif

}

void armino_mount_partitions(void)
{
#ifdef CONFIG_FLASH_PARTITION
	int ret;
	struct mtd_dev_s *mtd;
	partition_info_t partinfo;

	mtd = (FAR struct mtd_dev_s *)mtd_initialize();

	//TODO:need open this funciton
	/* Configure mtd partitions */
	ret = configure_mtd_partitions(mtd, 0, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions failed\n");
		return;
	}

#ifdef CONFIG_AUTOMOUNT
	automount_fs_partition(&partinfo);
#endif

#endif /* CONFIG_FLASH_PARTITION */
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
static int board_pm_pre_init(void)
{
	#ifdef CONFIG_PM
	extern void arm_pminitialize(void);
	arm_pminitialize();
	#endif

	#if defined(CONFIG_ROSC_CALIB_SW)
	bk_rosc_32k_calib();
	#endif
	return 0;
}

static int board_pm_init(void)
{
	pm_cpu_freq_e cpu_freq_default = (pm_cpu_freq_e)CONFIG_PM_CPU_FRQ_DEFAULT;
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,cpu_freq_default);

	#if defined(CONFIG_PM_LV_WDT_PROTECTION)
	//pm_wifi_event_init();
	#endif
	#if defined(CONFIG_ANA_GPIO)
	ana_gpio_wakeup_init();
	#endif
	/* pm demo thread */
	pm_thread_main();
	return 0;
}
extern int32_t ns_interface_lock_init(void);
void board_initialize(void)
{
    entry_main();
#if (CONFIG_SPE == 0)
    ns_interface_lock_init();
#endif
    
#if defined(CONFIG_CLI)
	beken_commands_line_init(1, NULL);
#endif

#if defined(CONFIG_ASYNC_IRQ_LOG)
	extern int async_irq_log_init(void);
	async_irq_log_init();
#endif

#if defined(CONFIG_WATCHDOG)
	armino_wdg_initialize(CONFIG_WATCHDOG_DEVPATH);
#endif

	board_gpio_initialize();

	armino_mount_partitions();
#ifdef CONFIG_I2C
	board_i2c_initialize();
#endif
#ifdef CONFIG_SPI
	board_spi_initialize();
#endif

#ifdef CONFIG_TIMER
        int i;
        char path[CONFIG_PATH_MAX];
        for (i = 0; i < TIMER_ID_MAX; i++) {
                snprintf(path, sizeof(path), "/dev/timer%d", i);
                armino_timer_initialize(path, i);
        }
#endif

#ifdef CONFIG_RTC_DRIVER
	struct rtc_lowerhalf_s *rtclower;
	int ret;

	rtclower = armino_rtc_lowerhalf();
	if (rtclower) {
		ret = rtc_initialize(0, rtclower);
		if (ret < 0) {
			lldbg("Failed to register the RTC driver: %d\n", ret);
		}
	}
#endif
	/*pm pre init*/
	board_pm_pre_init();

	extern void bk_phy_init(void);
	bk_phy_init();

	/*pm init*/
	board_pm_init();

}
#else
#error "CONFIG_BOARD_INITIALIZE MUST ENABLE"
#endif

int board_app_initialize(void)
{
	return OK;
}
