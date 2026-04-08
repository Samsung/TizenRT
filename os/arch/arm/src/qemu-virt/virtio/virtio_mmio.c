/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include "virtio_mmio.h"
#include "up_arch.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: virtio_mmio_get_base_addr
 *
 * Description:
 *   Calculate the base address for a virtio-mmio device
 *
 ****************************************************************************/

static inline uint32_t virtio_mmio_get_base_addr(uint32_t device_num)
{
	DEBUGASSERT(device_num < VIRTIO_MMIO_MAX_DEVICES);
	return VIRTIO_MMIO_BASE_ADDR + (device_num * VIRTIO_MMIO_DEVICE_SPACING);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: virtio_mmio_init
 *
 * Description:
 *   Initialize a virtio-mmio device
 *
 ****************************************************************************/

int virtio_mmio_init(virtio_mmio_dev_t *dev, uint32_t device_num)
{
	uint32_t magic;
	uint32_t base_addr;

	/* Validate input parameters */

	if (!dev || device_num >= VIRTIO_MMIO_MAX_DEVICES) {
		return -EINVAL;
	}

	/* Initialize device structure */

	memset(dev, 0, sizeof(virtio_mmio_dev_t));
	base_addr = virtio_mmio_get_base_addr(device_num);
	dev->base_addr = base_addr;

	/* Check if device exists by reading magic value */
	/* Direct register access during initialization */

	magic = getreg32(base_addr + VIRTIO_MMIO_MAGIC_VALUE);
	if (magic != 0x74726976) {	/* "virt" in little-endian */
		return -ENODEV;
	}

	/* Read device information */

	dev->version = getreg32(base_addr + VIRTIO_MMIO_VERSION);
	dev->device_id = getreg32(base_addr + VIRTIO_MMIO_DEVICE_ID);
	dev->vendor_id = getreg32(base_addr + VIRTIO_MMIO_VENDOR_ID);

	/* Reset device */

	putreg32(VIRTIO_CONFIG_STATUS_RESET, base_addr + VIRTIO_MMIO_STATUS);
	up_udelay(100);	/* Wait for reset to complete */

	dev->initialized = true;
	return OK;
}

/****************************************************************************
 * Name: virtio_mmio_deinit
 *
 * Description:
 *   Deinitialize a virtio-mmio device
 *
 ****************************************************************************/

void virtio_mmio_deinit(virtio_mmio_dev_t *dev)
{
	if (!dev || !dev->initialized) {
		return;
	}

	/* Reset device */

	virtio_mmio_set_status(dev, VIRTIO_CONFIG_STATUS_RESET);
	dev->initialized = false;
}

/****************************************************************************
 * Name: virtio_mmio_read32
 *
 * Description:
 *   Read a 32-bit value from a virtio-mmio register
 *
 ****************************************************************************/

uint32_t virtio_mmio_read32(virtio_mmio_dev_t *dev, uint32_t offset)
{
	DEBUGASSERT(dev && dev->initialized);
	return getreg32(dev->base_addr + offset);
}

/****************************************************************************
 * Name: virtio_mmio_write32
 *
 * Description:
 *   Write a 32-bit value to a virtio-mmio register
 *
 ****************************************************************************/

void virtio_mmio_write32(virtio_mmio_dev_t *dev, uint32_t offset, uint32_t value)
{
	DEBUGASSERT(dev && dev->initialized);
	putreg32(value, dev->base_addr + offset);
}

/****************************************************************************
 * Name: virtio_mmio_read8
 *
 * Description:
 *   Read an 8-bit value from a virtio-mmio register
 *
 ****************************************************************************/

uint8_t virtio_mmio_read8(virtio_mmio_dev_t *dev, uint32_t offset)
{
	DEBUGASSERT(dev && dev->initialized);
	return getreg8(dev->base_addr + offset);
}

/****************************************************************************
 * Name: virtio_mmio_write8
 *
 * Description:
 *   Write an 8-bit value to a virtio-mmio register
 *
 ****************************************************************************/

void virtio_mmio_write8(virtio_mmio_dev_t *dev, uint32_t offset, uint8_t value)
{
	DEBUGASSERT(dev && dev->initialized);
	putreg8(value, dev->base_addr + offset);
}

/****************************************************************************
 * Name: virtio_mmio_get_device_features
 *
 * Description:
 *   Get device features
 *
 ****************************************************************************/

uint32_t virtio_mmio_get_device_features(virtio_mmio_dev_t *dev)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_DEVICE_FEATURES_SEL, 0);
	return virtio_mmio_read32(dev, VIRTIO_MMIO_DEVICE_FEATURES);
}

/****************************************************************************
 * Name: virtio_mmio_set_driver_features
 *
 * Description:
 *   Set driver features
 *
 ****************************************************************************/

int virtio_mmio_set_driver_features(virtio_mmio_dev_t *dev, uint32_t features)
{
	DEBUGASSERT(dev && dev->initialized);

	virtio_mmio_write32(dev, VIRTIO_MMIO_DRIVER_FEATURES_SEL, 0);
	virtio_mmio_write32(dev, VIRTIO_MMIO_DRIVER_FEATURES, features);

	return OK;
}

/****************************************************************************
 * Name: virtio_mmio_get_status
 *
 * Description:
 *   Get device status
 *
 ****************************************************************************/

uint8_t virtio_mmio_get_status(virtio_mmio_dev_t *dev)
{
	DEBUGASSERT(dev && dev->initialized);
	return (uint8_t)virtio_mmio_read32(dev, VIRTIO_MMIO_STATUS);
}

/****************************************************************************
 * Name: virtio_mmio_set_status
 *
 * Description:
 *   Set device status
 *
 ****************************************************************************/

void virtio_mmio_set_status(virtio_mmio_dev_t *dev, uint8_t status)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_STATUS, status);
}

/****************************************************************************
 * Name: virtio_mmio_select_queue
 *
 * Description:
 *   Select a queue for operations
 *
 ****************************************************************************/

void virtio_mmio_select_queue(virtio_mmio_dev_t *dev, uint32_t queue_sel)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_QUEUE_SEL, queue_sel);
}

/****************************************************************************
 * Name: virtio_mmio_get_queue_num_max
 *
 * Description:
 *   Get maximum queue size
 *
 ****************************************************************************/

uint32_t virtio_mmio_get_queue_num_max(virtio_mmio_dev_t *dev)
{
	DEBUGASSERT(dev && dev->initialized);
	return virtio_mmio_read32(dev, VIRTIO_MMIO_QUEUE_NUM_MAX);
}

/****************************************************************************
 * Name: virtio_mmio_set_queue_num
 *
 * Description:
 *   Set queue size
 *
 ****************************************************************************/

void virtio_mmio_set_queue_num(virtio_mmio_dev_t *dev, uint32_t queue_num)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_QUEUE_NUM, queue_num);
}

/****************************************************************************
 * Name: virtio_mmio_set_queue_align
 *
 * Description:
 *   Set queue alignment
 *
 ****************************************************************************/

void virtio_mmio_set_queue_align(virtio_mmio_dev_t *dev, uint32_t align)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_QUEUE_ALIGN, align);
}

/****************************************************************************
 * Name: virtio_mmio_set_queue_pfn
 *
 * Description:
 *   Set queue page frame number
 *
 ****************************************************************************/

void virtio_mmio_set_queue_pfn(virtio_mmio_dev_t *dev, uint32_t pfn)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_QUEUE_PFN, pfn);
}

/****************************************************************************
 * Name: virtio_mmio_queue_notify
 *
 * Description:
 *   Notify device about queue activity
 *
 ****************************************************************************/

void virtio_mmio_queue_notify(virtio_mmio_dev_t *dev, uint32_t queue_num)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_QUEUE_NOTIFY, queue_num);
}

/****************************************************************************
 * Name: virtio_mmio_get_interrupt_status
 *
 * Description:
 *   Get interrupt status
 *
 ****************************************************************************/

uint32_t virtio_mmio_get_interrupt_status(virtio_mmio_dev_t *dev)
{
	DEBUGASSERT(dev && dev->initialized);
	return virtio_mmio_read32(dev, VIRTIO_MMIO_INTERRUPT_STATUS);
}

/****************************************************************************
 * Name: virtio_mmio_interrupt_ack
 *
 * Description:
 *   Acknowledge interrupts
 *
 ****************************************************************************/

void virtio_mmio_interrupt_ack(virtio_mmio_dev_t *dev, uint32_t int_status)
{
	DEBUGASSERT(dev && dev->initialized);
	virtio_mmio_write32(dev, VIRTIO_MMIO_INTERRUPT_ACK, int_status);
}
