/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
#include "ameba_i2c.h"
#include "ameba_spi.h"
#ifdef CONFIG_FLASH_PARTITION
#include "common.h"
#endif

#include "up_internal.h"
#include "amebasmart_boot.h"
#include "ameba_soc.h"
#include "platform_opts_bt.h"

#ifdef CONFIG_AMEBASMART_BOR
#include "ameba_bor.h"
#endif
/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
extern FAR struct gpio_lowerhalf_s *amebasmart_gpio_lowerhalf(u32 pinname, u32 pinmode, u32 pinpull);
#ifdef CONFIG_PRODCONFIG
extern u32 OTP_Read8(u32 Addr, u8 *Data);
#endif
#if defined(CONFIG_AUDIO_I2SCHAR) && defined(CONFIG_AMEBASMART_I2S)
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
	OTP_Read8(SEC_CFG1, &prod_disable);
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
	u8 hw_download, sw_download;
	OTP_Read8(SEC_CFG2, &hw_download);
	OTP_Read8(0x369, &sw_download);
	if ((hw_download & 1) && (sw_download & 0x08)) {
		return OK;
	}
	return ERROR;
}
#endif

#ifdef CONFIG_AMEBASMART_BOR
static void board_initialize_bor(void)
{
	BOR_ThresholdSet(CONFIG_AMEBASMART_THRESHOLD_FALL,CONFIG_AMEBASMART_THRESHOLD_RISE);
	BOR_ModeSet(BOR_RESET);
	BOR_Enable(ENABLE);
	DelayUs(100);
	RCC_PeriphClockCmd(APBPeriph_BOR, APBPeriph_CLOCK_NULL, ENABLE);
	dbg("Brownout reset enabled\n");
}
#endif

int up_check_iwdg(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_FEN);

	if ((Temp & APBPeriph_IWDG) == 0) {
		dbg("IWDG is disabled\n");
	} else {
		dbg("IWDG is enabled\n");
	}
}

void board_i2s_initialize(void)
{
#ifdef CONFIG_AMEBASMART_I2S
#ifdef CONFIG_AUDIO_I2SCHAR
	i2schar_devinit();
#endif
#ifdef CONFIG_PM
	i2s_pminitialize();
#endif
#endif
}

void board_spi_initialize(void)
{
#ifdef CONFIG_SPI
	struct spi_dev_s *spi;
#ifdef CONFIG_AMEBASMART_SPI0
	spi = up_spiinitialize(0);
#ifdef CONFIG_SPI_USERIO
	if (spi_uioregister(0, spi) < 0) {
		lldbg("Failed to register SPI0\n");
	}
#endif
#endif

#if defined(CONFIG_AMEBASMART_SPI1) && !defined(CONFIG_SECOND_FLASH_PARTITION)
	spi = up_spiinitialize(1);
#ifdef CONFIG_SPI_USERIO
	if (spi_uioregister(1, spi) < 0) {
		lldbg("Failed to register SPI1\n");
	}
#endif
#endif
#ifdef CONFIG_PM
	spi_pminitialize();
#endif
#endif /* CONFIG_SPI */
}

void board_i2c_initialize(void)
{
#ifdef CONFIG_I2C
	FAR struct i2c_dev_s *i2c;
	int bus = 0;
	int ret;
	char path[16];

#ifdef CONFIG_AMEBASMART_I2C0
	bus = 0;
	snprintf(path, sizeof(path), "/dev/i2c-%d", bus);
	i2c = (struct i2c_dev_s *)up_i2cinitialize(bus);
#ifdef CONFIG_I2C_USERIO
	if (i2c != NULL) {
		ret = i2c_uioregister(path, i2c);
		if (ret < 0) {
			up_i2cuninitialize(i2c);
		}
	} else {
		lldbg("Failed to register I2C\n");
	}
#endif
#endif

#ifdef CONFIG_AMEBASMART_I2C1
	bus = 1;
	snprintf(path, sizeof(path), "/dev/i2c-%d", bus);
	i2c = up_i2cinitialize(bus);
#ifdef CONFIG_I2C_USERIO
	if (i2c != NULL) {
		ret = i2c_uioregister(path, i2c);
		if (ret < 0) {
			up_i2cuninitialize(i2c);
		}
	} else {
		lldbg("Failed to register I2C\n");
	}
#endif
#endif

#ifdef CONFIG_AMEBASMART_I2C2
	bus = 2;
	snprintf(path, sizeof(path), "/dev/i2c-%d", bus);
	i2c = up_i2cinitialize(bus);
#ifdef CONFIG_I2C_USERIO
	if (i2c != NULL) {
		ret = i2c_uioregister(path, i2c);
		if (ret < 0) {
			up_i2cuninitialize(i2c);
		}
	} else {
		lldbg("Failed to register I2C\n");
	}
#endif
#endif
#ifdef CONFIG_PM
	i2c_pminitialize();
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
				{PA_23, PIN_INPUT, PullDown},
				/* PB_20 is gpio pin number for LED */
				{PB_20, PIN_OUTPUT, PullDown},
				{PB_22, PIN_INPUT, PullUp},
		/* NOTE: Do not open pins not for GPIO usage. E.g uart,SPI pins
		Loguart pins
		*/
		/*		{PA_0, PIN_OUTPUT, PullNone},
				{PA_1, PIN_OUTPUT, PullNone},
				{PA_2, PIN_OUTPUT, PullNone},
				{PA_3, PIN_OUTPUT, PullNone},
				{PA_4, PIN_OUTPUT,  PullNone},
				{PA_5, PIN_OUTPUT, PullNone},
				{PA_6, PIN_OUTPUT, PullNone},
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
				//PA23,PA_24 LOGUART PINS
				{PB_25, PIN_OUTPUT, PullNone},
				{PB_26, PIN_OUTPUT, PullNone},
				{PB_27, PIN_OUTPUT, PullNone},
				{PB_28, PIN_OUTPUT, PullNone},
				{PB_29, PIN_OUTPUT, PullNone},
				{PB_30, PIN_OUTPUT, PullNone},
				{PB_31, PIN_OUTPUT, PullNone}, */
	};

	for (i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		lower = amebasmart_gpio_lowerhalf(pins[i].pinname, pins[i].pinmode, pins[i].pinpull);
		gpio_register(pins[i].pinname, lower);
	}
#ifdef CONFIG_PM
	gpio_pminitialize();
#endif
#endif
}

void amebasmart_mount_partitions(void)
{
	int ret;
	struct mtd_dev_s *mtd;
	partition_info_t partinfo;

	mtd = (FAR struct mtd_dev_s *)mtd_initialize();
	/* Configure mtd partitions */
	ret = configure_mtd_partitions(mtd, 0, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions for primary flash failed\n");
		return;
	}

#ifdef CONFIG_AUTOMOUNT
	automount_fs_partition(&partinfo);
#endif

#ifdef CONFIG_SECOND_FLASH_PARTITION
	struct spi_dev_s *spi = up_spiinitialize(1);

#ifdef CONFIG_MTD_JEDEC
	mtd = jedec_initialize(spi);
	if (mtd == NULL) {
		lldbg("Jedec Init failed\n");
		return;
	}

#elif defined(CONFIG_MTD_W25)
	mtd = w25_initialize(spi);
	if (mtd == NULL) {
		lldbg("w25 Init failed\n");
		return;
	}
#endif
	ret = configure_mtd_partitions(mtd, 1, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions for secondary flash failed\n");
		return;
	}
#ifdef CONFIG_AUTOMOUNT
	automount_fs_partition(&partinfo);
#endif
#endif /* end of CONFIG_SECOND_FLASH_PARTITION */

#ifdef CONFIG_RESOURCE_FS
	if (binary_manager_mount_resource() != OK) {
		lldbg("ERROR: Failed to mount resource\n");
	}
#endif
}

#ifdef CONFIG_FTL_ENABLED
extern u8 ftl_phy_page_num;
extern u32 ftl_phy_page_start_addr;
#include "ftl_int.h"
extern void flash_get_layout_info(u32 type, u32 *start, u32 *end);

void app_ftl_init(void)
{
	u32 ftl_start_addr, ftl_end_addr;

	flash_get_layout_info(FTL, &ftl_start_addr, &ftl_end_addr);

	ftl_phy_page_start_addr = ftl_start_addr - SPI_FLASH_BASE;
	ftl_phy_page_num = (ftl_end_addr - ftl_start_addr +1) / PAGE_SIZE_4K;

	if (SYSCFG_BootFromNor()) {
		ftl_init(ftl_phy_page_start_addr, ftl_phy_page_num);
	}
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

static gtimer_t g_timer_np_lp;

static void np_lp_status_timer_hdl(void)
{
	/* Added to inspect the value of BKUP_REG2 */
	u32 boot_reason_reg2 = BKUP_Read(BKUP_REG2);
	if (boot_reason_reg2 != 0x0) {
		lldbg("boot_reason_reg2 0x%x \n", boot_reason_reg2);
		ASSERT(boot_reason_reg2 == 0x0);
	}
}

static void init_np_lp_status_timer(void)
{
	gtimer_init(&g_timer_np_lp, TIMER2);
	gtimer_start_periodical(&g_timer_np_lp, CONFIG_AMEBASMART_NP_LP_CHECK_INTERVAL, (void *)np_lp_status_timer_hdl, (uint32_t)&g_timer_np_lp);
}

void board_initialize(void)
{

	/* Create task here */
	InterruptRegister(IPC_INTHandler, IPC_AP_IRQ, (u32)IPCAP_DEV, INT_PRI_MIDDLE);
	InterruptEn(IPC_AP_IRQ, INT_PRI_MIDDLE);

#if 0  //def CONFIG_MBED_TLS_ENABLED
	app_mbedtls_rom_init();
#endif

	ipc_table_init(IPCAP_DEV);

#ifdef CONFIG_AMEBASMART_USBDEVICE
	usb_initialize();
#endif
	rtl8730e_km4_logtask_initialize();

	/* init console */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	shell_init_rom(0, 0);
#endif
	amebasmart_mount_partitions();
	board_gpio_initialize();
	board_i2c_initialize();
	board_spi_initialize();
	board_i2s_initialize();

#ifdef CONFIG_LCD_ST7789
	rtl8730_st7789_initialize();
#endif

#if defined(CONFIG_LCD_ST7785) || defined(CONFIG_LCD_ST7701) || defined(CONFIG_LCD_ST7701SN)
	rtl8730e_lcdc_initialize();
#endif

#if defined(CONFIG_TOUCH_IST415)
	rtl8730e_ist415_initialize();
#endif

#ifdef CONFIG_WATCHDOG
	amebasmart_wdg_initialize(CONFIG_WATCHDOG_DEVPATH, 5000);
#endif
#ifdef CONFIG_TIMER
	int i;
	char path[CONFIG_PATH_MAX];
	for (i = 0; i < GTIMER_MAX; i++) {
		snprintf(path, sizeof(path), "/dev/timer%d", i);
		amebasmart_timer_initialize(path, i);
	}
#endif

#ifdef CONFIG_RTC_DRIVER
	struct rtc_lowerhalf_s *rtclower;
	int ret;

	rtclower = (FAR struct rtc_lowerhalf_s *)amebasmart_rtc_lowerhalf();
	if (rtclower) {
		ret = rtc_initialize(0, rtclower);
		if (ret < 0) {
			lldbg("Failed to register the RTC driver: %d\n", ret);
		}
	}
#endif

#ifdef CONFIG_FTL_ENABLED
	app_ftl_init();
#endif

	/* Start timer to check KM4/KM0 status */
	init_np_lp_status_timer();
#ifdef CONFIG_AMEBASMART_WIFI
	wlan_initialize();
#endif
	/* Enable IPC buffered print */
	inic_ipc_buffered_printf_set_np_enable(1);

#ifdef CONFIG_AUDIO_ALC1019
	if (rtl8730e_alc1019_initialize(0) != 0) {
		lldbg("ALC1019 initialization failed\n");
	}
#endif

#ifdef CONFIG_AUDIO_SYU645B
	if (rtl8730e_syu645b_initialize(0) != 0) {
		lldbg("syu645b initialization failed\n");
	}
#endif

#ifdef CONFIG_AUDIO_NDP120
	if (rtl8730e_ndp120_initialize(0) != 0) {
		lldbg("NDP120 initialization failed\n");
	}
 #endif
 
 #ifdef CONFIG_AMEBASMART_BOR
	board_initialize_bor();
 #endif
	IPC_MSG_STRUCT ipc_msg_loguart;

	ipc_msg_loguart.msg_type = IPC_USER_POINT;
	ipc_msg_loguart.msg = 0;
	ipc_msg_loguart.msg_len = 1;
	ipc_msg_loguart.rsvd = 0; /* for coverity init issue */
	ipc_send_message(IPC_AP_TO_LP, IPC_A2L_DISLOGUART, &ipc_msg_loguart);
	
	up_check_iwdg();
}
#else
#error "CONFIG_BOARD_INITIALIZE MUST ENABLE"
#endif

int board_app_initialize(void)
{
	return OK;
}

