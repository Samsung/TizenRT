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

#ifndef __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_BLK_H
#define __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_BLK_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tinyara/semaphore.h>
#include "virtio_mmio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Virtio Block Device specific registers */

#define VIRTIO_BLK_CONFIG_CAPACITY	0x00
#define VIRTIO_BLK_CONFIG_SIZE_MAX	0x08
#define VIRTIO_BLK_CONFIG_SEG_MAX	0x0c
#define VIRTIO_BLK_CONFIG_GEOMETRY	0x10
#define VIRTIO_BLK_CONFIG_BLK_SIZE	0x14
#define VIRTIO_BLK_CONFIG_TOPOLOGY	0x18
#define VIRTIO_BLK_CONFIG_WRITEBACK	0x20
#define VIRTIO_BLK_CONFIG_UNUSED	0x21
#define VIRTIO_BLK_CONFIG_NUM_QUEUES	0x22
#define VIRTIO_BLK_CONFIG_MAX_DISCARD_SECTORS	0x24
#define VIRTIO_BLK_CONFIG_MAX_DISCARD_SEG	0x28
#define VIRTIO_BLK_CONFIG_DISCARD_SECTOR_ALIGNMENT	0x2a
#define VIRTIO_BLK_CONFIG_MAX_WRITE_ZEROES_SECTORS	0x2c
#define VIRTIO_BLK_CONFIG_MAX_WRITE_ZEROES_SEG	0x30
#define VIRTIO_BLK_CONFIG_WRITE_ZEROES_MAY_UNMAP	0x32

/* Feature bits */

#define VIRTIO_BLK_F_SIZE_MAX		(1 << 1)	/* Maximum size of any single segment */
#define VIRTIO_BLK_F_SEG_MAX		(1 << 2)	/* Maximum number of segments in a request */
#define VIRTIO_BLK_F_GEOMETRY		(1 << 4)	/* Disk-style geometry specified */
#define VIRTIO_BLK_F_RO			(1 << 5)	/* Device is read-only */
#define VIRTIO_BLK_F_BLK_SIZE		(1 << 6)	/* Block size of disk is available */
#define VIRTIO_BLK_F_FLUSH		(1 << 9)	/* Cache flush command support */
#define VIRTIO_BLK_F_TOPOLOGY		(1 << 10)	/* Topology information is available */
#define VIRTIO_BLK_F_CONFIG_WCE		(1 << 11)	/* Writeback mode available in config */
#define VIRTIO_BLK_F_MQ			(1 << 12)	/* Device supports multiqueue */
#define VIRTIO_BLK_F_DISCARD		(1 << 13)	/* Device supports discard command */
#define VIRTIO_BLK_F_WRITE_ZEROS	(1 << 14)	/* Device supports write zeroes command */

/* Command types */

#define VIRTIO_BLK_T_IN			0	/* Read */
#define VIRTIO_BLK_T_OUT		1	/* Write */
#define VIRTIO_BLK_T_FLUSH		4	/* Flush */
#define VIRTIO_BLK_T_DISCARD		11	/* Discard */
#define VIRTIO_BLK_T_WRITE_ZEROES	13	/* Write zeroes */

/* Status codes */

#define VIRTIO_BLK_S_OK			0	/* Success */
#define VIRTIO_BLK_S_IOERR		1	/* Device or driver error */
#define VIRTIO_BLK_S_UNSUPP		2	/* Supports request */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Block device configuration structure */

struct virtio_blk_config_s {
	uint64_t capacity;		/* Number of sectors */
	uint32_t size_max;		/* Maximum segment size */
	uint32_t seg_max;		/* Maximum number of segments */
	struct virtio_blk_geometry_s {
		uint16_t cylinders;
		uint8_t heads;
		uint8_t sectors;
	} geometry;
	uint32_t blk_size;		/* Block size of device */
	struct virtio_blk_topology_s {
		uint8_t physical_block_exp;
		uint8_t alignment_offset;
		uint16_t min_io_size;
		uint32_t opt_io_size;
	} topology;
	uint8_t writeback;
	uint8_t unused;
	uint16_t num_queues;
	uint32_t max_discard_sectors;
	uint32_t max_discard_seg;
	uint32_t discard_sector_alignment;
	uint32_t max_write_zeroes_sectors;
	uint32_t max_write_zeroes_seg;
	uint8_t write_zeroes_may_unmap;
} __attribute__((packed));

struct virtio_blk_dev_s {
	virtio_mmio_dev_t mmio_dev;	/* Base virtio-mmio device */
	struct virtio_blk_config_s config;	/* Block device configuration */
	uint32_t features;		/* Negotiated features */
	bool ready;			/* Device ready status */
	virtq_t vq;			/* Virtqueue for block operations */
	int irq;			/* IRQ number for this device */
	volatile bool completion_received; /* Completion flag for interrupt handling */
	sem_t completion_sem;		/* Semaphore for completion synchronization */
};

/* Block request header */

struct virtio_blk_req_hdr_s {
	uint32_t type;			/* Request type */
	uint32_t reserved;
	uint64_t sector;		/* Sector number */
} __attribute__((packed));

/* Block request footer */

struct virtio_blk_req_footer_s {
	uint8_t status;			/* Status of request */
} __attribute__((packed));

typedef struct virtio_blk_dev_s virtio_blk_dev_t;

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

int virtio_blk_init(virtio_blk_dev_t *dev, uint32_t device_num);
void virtio_blk_deinit(virtio_blk_dev_t *dev);

/* Block device operations */

int virtio_blk_read(virtio_blk_dev_t *dev, uint64_t sector, void *buffer, size_t nsectors);
int virtio_blk_write(virtio_blk_dev_t *dev, uint64_t sector, const void *buffer, size_t nsectors);
int virtio_blk_flush(virtio_blk_dev_t *dev);

/* Configuration access */

uint64_t virtio_blk_get_capacity(virtio_blk_dev_t *dev);
uint32_t virtio_blk_get_block_size(virtio_blk_dev_t *dev);

/* qemu-virt convenience helpers */

int qemu_virtio_blk_initialize(void);
int test_virtio_blk(int argc, char *argv[]);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_BLK_H */
