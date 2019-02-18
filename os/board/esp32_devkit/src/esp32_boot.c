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
#include <tinyara/gpio.h>
#ifdef CONFIG_SPIRAM_USE_CAPS_ALLOC
#include <esp_heap_caps.h>
#endif

#if defined(CONFIG_ADC)
#include "esp32_adc.h"
#include <tinyara/analog/adc.h>
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

#if defined(CONFIG_ESP32_FLASH_PART) && (CONFIG_ESP32_FLASH_PART == 1)
void esp32_devKit_config_partions(void)
{
	int partno;
	int partoffset;
	const char *parts = CONFIG_ESP32_FLASH_PART_LIST;
	const char *types = CONFIG_ESP32_FLASH_PART_TYPE;
	const char *names = CONFIG_ESP32_FLASH_PART_NAME;
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
	partoffset = CONFIG_ESP_FLASH_BASE / geo.blocksize;

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

		mtd_part = mtd_partition(mtd, partoffset, partsize / geo.blocksize, partno);
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
					smart_initialize(CONFIG_ESP32_FLASH_MINOR, mtd_part, partref);
				} else
#endif
				{
				}

#if defined(CONFIG_MTD_PARTITION_NAMES)
		if (strcmp(names, "")) {
			mtd_setpartitionname(mtd_part, names);
		}

		while (*names != ',' && *names) {
			names++;
		}
		if (*names == ',') {
			names++;
		}
#endif

		while (*parts != ',' && *parts) {
			parts++;
		}
		if (*parts == ',') {
			parts++;
		}

		while (*types != ',' && *types) {
			types++;
		}
		if (*types == ',') {
			types++;
		}

		partno++;
	}
}

void esp32_devKit_mount_partions(void)
{
	int ret;
	/* Initialize and mount user partition (if we have) */
	ret = mksmartfs(CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME, false);
	if (ret != OK) {
		lldbg("ERROR: mksmartfs on %s failed", CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME);
	} else {
		ret = mount(CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME, CONFIG_ESP32_AUTOMOUNT_USERFS_MOUNTPOINT, "smartfs", 0, NULL);
		if (ret != OK) {
			lldbg("ERROR: mounting '%s' failed\n", CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME);
		}
	}

#ifdef CONFIG_FS_PROCFS
	/* Mount the procfs file system */
	ret = mount(NULL, CONFIG_ESP32_PROCFS_MOUNTPOINT, "procfs", 0, NULL);
	if (ret < 0) {
		lldbg("Failed to mount procfs at %s: %d\n", CONFIG_ESP32_PROCFS_MOUNTPOINT, ret);
	}
#endif

}
#endif

void esp32_board_initialize(void)
{
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
#if defined(CONFIG_ESP32_FLASH_PART) && (CONFIG_ESP32_FLASH_PART == 1)
	esp32_devKit_config_partions();
	esp32_devKit_mount_partions();
#endif
	board_gpio_initialize();
	board_i2c_initialize();
	board_ledc_setup();

#if defined(CONFIG_ADC)
	board_adc_initialize();
#endif

	/*Init SPI RAM*/
#ifdef CONFIG_SPIRAM_SUPPORT
	esp_spiram_init_cache();
	esp_spiram_init();
#endif

	/*no-os heap configure*/
#ifdef CONFIG_SPIRAM_USE_CAPS_ALLOC
	esp_spiram_add_to_heapalloc();
	//Assgin DMA memory for drivers*/
	uint8_t *dma_heap = (uint8_t *)malloc(CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL);
	if (!dma_heap) {
		return;
	}
	uint32_t caps[] = { MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL, 0, MALLOC_CAP_8BIT | MALLOC_CAP_32BIT };
	heap_caps_add_region_with_caps(caps, (intptr_t) dma_heap, (intptr_t) dma_heap + CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL - 1);

	/*enable heaps */
	heap_caps_enable_nonos_stack_heaps();
#endif
}
#endif
