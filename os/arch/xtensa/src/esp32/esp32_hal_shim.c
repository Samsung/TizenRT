/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * os/arch/xtensa/src/esp32/esp32_hal_shim.c
 *
 * Description:
 *   Minimal glue for the symbols that the pinned esp-hal-3rdparty
 *   (b9472bd, NuttX 12.13.0) does not provide: the esp_partition
 *   component and the SPI bus lock driver (esp_driver_spi) are not part
 *   of the pinned HAL repository, while the ESP32 boot path
 *   (esp32_start.c) unconditionally pulls the HAL flash initialization
 *   code that references them.  The SPI flash is the only SPI bus user
 *   in the flat configurations, so the bus lock operations are no-ops.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <tinyara/config.h>

#include "esp_attr.h"
#include "esp_err.h"
#include "bootloader_init.h"
#include "esp_private/spi_share_hw_ctrl.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SPI_BUS_LOCK_MAX_HOSTS	4
#define SPI_BUS_LOCK_MAX_DEVS	4

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct spi_bus_lock_t {
	int host_id;
};

struct spi_bus_lock_dev_t {
	int dev_id;
	bool in_use;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct spi_bus_lock_t g_spi_bus_locks[SPI_BUS_LOCK_MAX_HOSTS];
static struct spi_bus_lock_dev_t g_spi_bus_lock_devs[SPI_BUS_LOCK_MAX_DEVS];
static int g_spi_bus_lock_next_dev_id;

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Defined in bootloader_init.c in the HAL; that file is only built for
 * the simple-boot configuration, so provide the storage here instead.
 */

#ifndef CONFIG_ESPRESSIF_SIMPLE_BOOT
esp_image_header_t WORD_ALIGNED_ATTR bootloader_image_hdr;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: esp_partition_main_flash_region_safe
 *
 * Description:
 *   Without the esp_partition component there is no partition table to
 *   check against; the flat image never writes the flash at runtime, so
 *   every region is reported safe.
 *
 ****************************************************************************/

bool esp_partition_main_flash_region_safe(size_t addr, size_t size)
{
	return true;
}

/****************************************************************************
 * Name: spi_bus_lock_get_by_id
 ****************************************************************************/

spi_bus_lock_handle_t spi_bus_lock_get_by_id(spi_host_device_t host_id)
{
	if (host_id < 0 || host_id >= SPI_BUS_LOCK_MAX_HOSTS) {
		return NULL;
	}

	g_spi_bus_locks[host_id].host_id = host_id;
	return &g_spi_bus_locks[host_id];
}

/****************************************************************************
 * Name: spi_bus_lock_register_dev
 ****************************************************************************/

esp_err_t spi_bus_lock_register_dev(spi_bus_lock_handle_t lock,
									spi_bus_lock_dev_config_t *config,
									spi_bus_lock_dev_handle_t *out_dev_handle)
{
	int i;

	if (lock == NULL || out_dev_handle == NULL) {
		return ESP_ERR_INVALID_ARG;
	}

	for (i = 0; i < SPI_BUS_LOCK_MAX_DEVS; i++) {
		if (!g_spi_bus_lock_devs[i].in_use) {
			g_spi_bus_lock_devs[i].in_use = true;
			g_spi_bus_lock_devs[i].dev_id = g_spi_bus_lock_next_dev_id++;
			*out_dev_handle = &g_spi_bus_lock_devs[i];
			return ESP_OK;
		}
	}

	return ESP_ERR_NO_MEM;
}

/****************************************************************************
 * Name: spi_bus_lock_unregister_dev
 ****************************************************************************/

void spi_bus_lock_unregister_dev(spi_bus_lock_dev_handle_t dev_handle)
{
	if (dev_handle != NULL) {
		dev_handle->in_use = false;
	}
}

/****************************************************************************
 * Name: spi_bus_lock_get_dev_id
 ****************************************************************************/

int spi_bus_lock_get_dev_id(spi_bus_lock_dev_handle_t dev_handle)
{
	if (dev_handle == NULL) {
		return -1;
	}

	return dev_handle->dev_id;
}

/****************************************************************************
 * Name: spi_bus_lock_touch
 ****************************************************************************/

bool spi_bus_lock_touch(spi_bus_lock_dev_handle_t dev_handle)
{
	return dev_handle != NULL;
}

/****************************************************************************
 * Name: spi_bus_lock_acquire_start
 ****************************************************************************/

esp_err_t spi_bus_lock_acquire_start(spi_bus_lock_dev_handle_t dev_handle,
									 TickType_t wait)
{
	return dev_handle != NULL ? ESP_OK : ESP_ERR_INVALID_ARG;
}

/****************************************************************************
 * Name: spi_bus_lock_acquire_end
 ****************************************************************************/

esp_err_t spi_bus_lock_acquire_end(spi_bus_lock_dev_handle_t dev_handle)
{
	return dev_handle != NULL ? ESP_OK : ESP_ERR_INVALID_ARG;
}
