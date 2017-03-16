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
#include <tinyara/spi/spi.h>
#include <tinyara/mmcsd.h>
#include <tinyara/rtc.h>
#include <time.h>
#include <chip.h>
#include <tinyara/kmalloc.h>

#include "s5j_rtc.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

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
extern void qspi_register(void);
extern void sdio_drv_register(void);
extern void pwmdrv_register(void);
extern void chipid_register(void);
extern void s5j_i2c_register(int bus);
extern uint32_t _vector_start;
extern void wlbt_if_booting(void);
extern void pdma_init(void);

char *s5j_get_binary_version(uint32_t baddr)
{
	static char version[13];
	version[12] = '\0';
	strncpy(version, (char *)baddr, sizeof(version) - 1);
	return version;

}

#ifdef CONFIG_SCSC_WLAN
static void scsc_wpa_ctrl_iface_init(void)
{
	int ret;

	ret = mkfifo("/dev/wpa_ctrl_req", 666);
	if(ret != 0 && ret != -EEXIST) {
		lldbg("mkfifo error ret:%d\n", ret);
		return;
	}

	ret = mkfifo("/dev/wpa_ctrl_cfm", 666);
	if(ret != 0 && ret != -EEXIST) {
		lldbg("mkfifo error ret:%d\n", ret);
		return;
	}

	ret = mkfifo("/dev/wpa_monitor", 666);
	if(ret != 0 && ret != -EEXIST) {
		lldbg("mkfifo error ret:%d\n", ret);
		return;
	}

	return;
}
#endif

int board_app_initialize(void)
{
#ifdef S5J_DISPLAY_MAC_ADDR
	char mac_buf[6];
#endif
#ifdef S5J_PERFORMANCE_TEST
	uint32_t src;
	uint32_t dst;
	uint32_t t1;
	uint32_t t2;
#endif
#if defined(CONFIG_RTC) && defined(CONFIG_RTC_DRIVER) && defined(CONFIG_S5J_RTC)
	/* FAR struct rtc_lowerhalf_s *lower; */
	FAR struct tm tp;
#endif

#ifdef CONFIG_S5J_SFLASH
	s5j_qspi_init();
#endif

#ifdef CONFIG_S5J_ADC
	adcdrv_register();
#endif

#ifdef CONFIG_S5J_EFUSE
	efusedrv_register();
#endif

#ifdef CONFIG_S5J_CHIPID
	chipid_register();
#endif

#ifdef CONFIG_S5J_I2C
	s5j_i2c_register(0);
	s5j_i2c_register(1);
#endif

	up_timer_initialize();

	/* clk_print_info_all(-1); */

#ifdef CONFIG_S5J_PWM
	pwmdrv_register();
#endif

#ifdef CONFIG_S5J_SDIO_SLAVE
	sdio_drv_register();
#endif

#ifdef CONFIG_S5J_LEDCTRLBLK
	ledctrlblk_drv_init();
#endif

#ifdef CONFIG_S5J_WATCHDOG
	up_wdginitialize();
#ifdef CONFIG_S5J_WATCHDOG_RESET
	s5j_wdg_set_reset(WDT_CPUCL_BASE, 1);
#endif
#endif

#if defined(CONFIG_RTC) && defined(CONFIG_RTC_DRIVER) && defined(CONFIG_S5J_RTC)
	up_rtc_getdatetime(&tp);
	lldbg("RTC getdatetime %d/%d/%d/%d/%d/%d\n", tp.tm_year + CONFIG_RTC_BASE_YEAR, tp.tm_mon + 1, tp.tm_mday, tp.tm_hour, tp.tm_min, tp.tm_sec);
	lldbg("Version Info :\n");
	lldbg("tinyARA %s\n", __TIMESTAMP__);
#endif

#ifdef CONFIG_S5J_WLBTBLK
	wlbt_if_booting();
#endif

#ifdef CONFIG_EXAMPLES_LDO_TEST
	register_ldo_drver();
#endif

#ifdef CONFIG_S5J_TICK_COUNTER
	s5j_tickcnt_initialize();
#endif

#ifdef CONFIG_S5J_DMA
	pdma_init();
#endif

#ifdef CONFIG_SCSC_WLAN
	scsc_wpa_ctrl_iface_init();
#endif

	lldbg("SIDK S5JT200 boot from 0x%x\n", &_vector_start);
	return OK;
}
