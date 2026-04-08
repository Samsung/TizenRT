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

#ifndef __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_MMIO_H
#define __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_MMIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>
#include "virtio_queue.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Virtio MMIO registers */

#define VIRTIO_MMIO_MAGIC_VALUE		0x000
#define VIRTIO_MMIO_VERSION		0x004
#define VIRTIO_MMIO_DEVICE_ID		0x008
#define VIRTIO_MMIO_VENDOR_ID		0x00c
#define VIRTIO_MMIO_DEVICE_FEATURES	0x010
#define VIRTIO_MMIO_DEVICE_FEATURES_SEL	0x014
#define VIRTIO_MMIO_DRIVER_FEATURES	0x020
#define VIRTIO_MMIO_DRIVER_FEATURES_SEL	0x024
#define VIRTIO_MMIO_GUEST_PAGE_SIZE	0x028
#define VIRTIO_MMIO_QUEUE_SEL		0x030
#define VIRTIO_MMIO_QUEUE_NUM_MAX	0x034
#define VIRTIO_MMIO_QUEUE_NUM		0x038
#define VIRTIO_MMIO_QUEUE_ALIGN		0x03c
#define VIRTIO_MMIO_QUEUE_PFN		0x040
#define VIRTIO_MMIO_QUEUE_READY		0x044
#define VIRTIO_MMIO_QUEUE_NOTIFY	0x050
#define VIRTIO_MMIO_INTERRUPT_STATUS	0x060
#define VIRTIO_MMIO_INTERRUPT_ACK	0x064
#define VIRTIO_MMIO_STATUS		0x070
#define VIRTIO_MMIO_QUEUE_DESC_LOW	0x080
#define VIRTIO_MMIO_QUEUE_DESC_HIGH	0x084
#define VIRTIO_MMIO_QUEUE_AVAIL_LOW	0x090
#define VIRTIO_MMIO_QUEUE_AVAIL_HIGH	0x094
#define VIRTIO_MMIO_QUEUE_USED_LOW	0x0a0
#define VIRTIO_MMIO_QUEUE_USED_HIGH	0x0a4
#define VIRTIO_MMIO_CONFIG_GENERATION	0x0fc
#define VIRTIO_MMIO_CONFIG		0x100

/* Status byte for guest to report progress */

#define VIRTIO_CONFIG_STATUS_RESET	0x00
#define VIRTIO_CONFIG_STATUS_ACK	0x01
#define VIRTIO_CONFIG_STATUS_DRIVER	0x02
#define VIRTIO_CONFIG_STATUS_DRIVER_OK	0x04
#define VIRTIO_CONFIG_STATUS_FEATURES_OK 0x08
#define VIRTIO_CONFIG_STATUS_FAILED	0x80

/* Interrupt flags */

#define VIRTIO_MMIO_INT_VRING		(1 << 0)
#define VIRTIO_MMIO_INT_CONFIG		(1 << 1)

/* Virtio MMIO base address and device spacing */

#define VIRTIO_MMIO_BASE_ADDR		0x0A000000
#define VIRTIO_MMIO_DEVICE_SPACING	0x200
#define VIRTIO_MMIO_MAX_DEVICES		32

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct virtio_mmio_dev_s {
	uint32_t base_addr;		/* Base address of the device */
	uint32_t device_id;		/* Device ID */
	uint32_t vendor_id;		/* Vendor ID */
	uint32_t version;		/* Device version */
	bool initialized;		/* Device initialization status */
};

typedef struct virtio_mmio_dev_s virtio_mmio_dev_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Device initialization and management */

int virtio_mmio_init(virtio_mmio_dev_t *dev, uint32_t device_num);
void virtio_mmio_deinit(virtio_mmio_dev_t *dev);

/* Register access functions */

uint32_t virtio_mmio_read32(virtio_mmio_dev_t *dev, uint32_t offset);
void virtio_mmio_write32(virtio_mmio_dev_t *dev, uint32_t offset, uint32_t value);
uint8_t virtio_mmio_read8(virtio_mmio_dev_t *dev, uint32_t offset);
void virtio_mmio_write8(virtio_mmio_dev_t *dev, uint32_t offset, uint8_t value);

/* Device status and feature management */

uint32_t virtio_mmio_get_device_features(virtio_mmio_dev_t *dev);
int virtio_mmio_set_driver_features(virtio_mmio_dev_t *dev, uint32_t features);
uint8_t virtio_mmio_get_status(virtio_mmio_dev_t *dev);
void virtio_mmio_set_status(virtio_mmio_dev_t *dev, uint8_t status);

/* Queue management */

void virtio_mmio_select_queue(virtio_mmio_dev_t *dev, uint32_t queue_sel);
uint32_t virtio_mmio_get_queue_num_max(virtio_mmio_dev_t *dev);
void virtio_mmio_set_queue_num(virtio_mmio_dev_t *dev, uint32_t queue_num);
void virtio_mmio_set_queue_align(virtio_mmio_dev_t *dev, uint32_t align);
void virtio_mmio_set_queue_pfn(virtio_mmio_dev_t *dev, uint32_t pfn);
void virtio_mmio_queue_notify(virtio_mmio_dev_t *dev, uint32_t queue_num);

/* Interrupt handling */

uint32_t virtio_mmio_get_interrupt_status(virtio_mmio_dev_t *dev);
void virtio_mmio_interrupt_ack(virtio_mmio_dev_t *dev, uint32_t int_status);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_MMIO_H */
