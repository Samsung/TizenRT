/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

/************************************************************************************
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
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
#include <tinyara/fs/mtd.h>
#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mount.h>

#include <tinyara/fs/mksmartfs.h>
#include <tinyara/board.h>
#include <arch/board/board.h>
#include "esp32_gpio.h"
#include "esp32_core.h"
#include "esp32_i2c.h"
#include "common.h"
#include <tinyara/gpio.h>
#ifdef CONFIG_FLASH_PARTITION
#include <tinyara/fs/mtd.h>
#endif

#ifdef CONFIG_PRODCONFIG
#include <tinyara/prodconfig.h>
#include <sys/types.h>
#endif

#include <../xtensa/xtensa.h>

#if defined(CONFIG_ADC)
#include "esp32_adc.h"
#include <tinyara/analog/adc.h>
#endif
#if defined(CONFIG_ESP32_SPIFLASH)
extern void esp_partition_initialize();
#endif
#ifdef CONFIG_SPIRAM_SUPPORT
extern void esp_spiram_init_cache();
extern int esp_spiram_init();
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#ifdef CONFIG_PRODCONFIG
int up_check_prodswd(void)
{
	return OK;
}
int up_check_proddownload(void)
{
	return OK;
}
#endif

/************************************************************************************
 * Name: esp32_board_initialize
 *
 * Description:
 *   All STM32 architectures must provide the following entry point.  This entry point
 *   is called early in the initialization -- after all memory has been configured
 *   and mapped but before any devices have been initialized.
 *
 ************************************************************************************/
extern int board_ledc_setup(void);

void esp32_devKit_mount_partitions(void)
{
#ifdef CONFIG_FLASH_PARTITION
	int ret;
	struct mtd_dev_s *mtd;
	partition_info_t partinfo;

	mtd = (FAR struct mtd_dev_s *)mtd_initialize();
	/* Configure mtd partitions */
	ret = configure_mtd_partitions(mtd, 0, &partinfo);
	if (ret != OK) {
		lldbg("ERROR: configure_mtd_partitions failed");
		return;
	}
#ifdef CONFIG_AUTOMOUNT
	automount_fs_partition(&partinfo);
#endif

#endif /* CONFIG_FLASH_PARTITION */
}

void esp32_board_initialize(void)
{
	/*Init SPI RAM*/
#ifdef CONFIG_SPIRAM_SUPPORT
	esp_spiram_init_cache();
	esp_spiram_init();
#endif
}

static void board_gpio_initialize(void)
{
#ifdef CONFIG_GPIO
	int i;
	struct gpio_lowerhalf_s *lower;

	struct {
		uint8_t minor;
		uint16_t pincfg;
	} pins[] = {
		{
			5, INPUT_PULLDOWN
		}, {
			15, INPUT_PULLDOWN
		}, {
			18, INPUT_PULLDOWN
		}, {
			19, INPUT_PULLDOWN
		}, {
			21, INPUT_PULLDOWN
		},
	};

	for (i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		lower = esp32_gpio_lowerhalf(pins[i].minor, pins[i].pincfg);
		gpio_register(pins[i].minor, lower);
	}
#endif
}

static void board_i2c_initialize(void)
{
#if defined(CONFIG_I2C)
#if defined(CONFIG_ESP32_I2C0) && (1 == CONFIG_ESP32_I2C0)
	esp32_i2c_register(0);
#endif

#if defined(CONFIG_ESP32_I2C1) && (1 == CONFIG_ESP32_I2C1)
	esp32_i2c_register(1);
#endif
#endif
}

#if defined(CONFIG_ADC)
static void board_adc_initialize(void)
{
	static bool adc_initialized;
	struct adc_dev_s *adc;
	const adc_channel_t chanlist[] = {
		ADC_CHANNEL_4,
		ADC_CHANNEL_5,
		ADC_CHANNEL_6,
		ADC_CHANNEL_7
	};

	/* Have we already initialized? */
	if (!adc_initialized) {
		adc = esp32_adc_initialize(chanlist, sizeof(chanlist) / sizeof(adc_channel_t));
		if (NULL != adc) {
			lldbg("ERROR: Failed to get the ESP32 ADC driver\n");
		} else {
			lldbg("ERROR: up_spiinitialize failed\n");
		}

		/* Register the ADC driver at "/dev/adc0" */
		int ret = adc_register("/dev/adc0", adc);
		if (ret < 0) {
			return;
		}

		/* Now we are initialized */
		adc_initialized = true;
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
void board_initialize(void)
{
	/* Perform board-specific initialization */
	(void)esp32_bringup();
#ifdef CONFIG_ESP32_SPIFLASH
	esp_partition_initialize();
#endif
	esp32_devKit_mount_partitions();
	board_gpio_initialize();
	board_i2c_initialize();
	board_ledc_setup();

#if defined(CONFIG_ADC)
	board_adc_initialize();
#endif

}
#endif
