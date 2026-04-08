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
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/semaphore.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "virtio_blk.h"
#include "up_arch.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define VIRTIO_BLK_QUEUE_NUM 8
#define VIRTIO_MMIO_IRQ_BASE 48

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int virtio_blk_interrupt(int irq, FAR void *context, FAR void *arg)
{
	virtio_blk_dev_t *dev = (virtio_blk_dev_t *)arg;
	uint32_t int_status;

	(void)irq;
	(void)context;

	int_status = virtio_mmio_get_interrupt_status(&dev->mmio_dev);
	virtio_mmio_interrupt_ack(&dev->mmio_dev, int_status);

	if (int_status & VIRTIO_MMIO_INT_VRING) {
		dev->completion_received = true;
		sem_post(&dev->completion_sem);
	}

	return OK;
}

static void virtio_blk_read_config(virtio_blk_dev_t *dev)
{
	uint32_t i;
	uint32_t *config_ptr = (uint32_t *)&dev->config;
	uint32_t config_size = sizeof(struct virtio_blk_config_s);

	for (i = 0; i < (config_size + 3) / 4; i++) {
		config_ptr[i] = virtio_mmio_read32(&dev->mmio_dev, VIRTIO_MMIO_CONFIG + (i * 4));
	}

	if (dev->config.blk_size == 0) {
		dev->config.blk_size = 512;
	}
}

static int virtio_blk_setup_queue(virtio_blk_dev_t *dev)
{
	uint32_t queue_num_max;
	uint32_t queue_num;
	uintptr_t desc_addr;
	uintptr_t avail_addr;
	uintptr_t used_addr;

	virtio_mmio_select_queue(&dev->mmio_dev, 0);
	queue_num_max = virtio_mmio_get_queue_num_max(&dev->mmio_dev);
	if (queue_num_max == 0) {
		return -ENODEV;
	}

	queue_num = queue_num_max < dev->vq.num ? queue_num_max : dev->vq.num;
	if (queue_num == 0 || (queue_num & (queue_num - 1)) != 0) {
		return -ENODEV;
	}
	dev->vq.num = (uint16_t)queue_num;
	dev->vq.num_mask = (uint16_t)(queue_num - 1);
	virtio_mmio_set_queue_num(&dev->mmio_dev, queue_num);

	desc_addr = (uintptr_t)dev->vq.desc;
	avail_addr = (uintptr_t)dev->vq.avail;
	used_addr = (uintptr_t)dev->vq.used;

	if (dev->mmio_dev.version <= 1) {
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_GUEST_PAGE_SIZE, 4096);
		virtio_mmio_set_queue_align(&dev->mmio_dev, 4096);
		virtio_mmio_set_queue_pfn(&dev->mmio_dev, (uint32_t)(dev->vq.queue_paddr / 4096));
	} else {
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_QUEUE_DESC_LOW, (uint32_t)desc_addr);
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_QUEUE_DESC_HIGH, (uint32_t)(desc_addr >> 32));
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_QUEUE_AVAIL_LOW, (uint32_t)avail_addr);
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_QUEUE_AVAIL_HIGH, (uint32_t)(avail_addr >> 32));
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_QUEUE_USED_LOW, (uint32_t)used_addr);
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_QUEUE_USED_HIGH, (uint32_t)(used_addr >> 32));
		virtio_mmio_write32(&dev->mmio_dev, VIRTIO_MMIO_QUEUE_READY, 1);
	}

	return OK;
}

static int virtio_blk_wait_for_completion(virtio_blk_dev_t *dev)
{
	int ret;

	while (sem_trywait(&dev->completion_sem) == OK) {
	}

	dev->completion_received = false;
	for (;;) {
		ret = sem_wait(&dev->completion_sem);
		if (ret == OK) {
			break;
		}

		if (errno != EINTR) {
			return -EIO;
		}
	}

	if (virtq_get_buffer(&dev->vq, NULL) == -EAGAIN) {
		return -EIO;
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int virtio_blk_init(virtio_blk_dev_t *dev, uint32_t device_num)
{
	uint32_t device_features;
	uint32_t driver_features = 0;
	uint8_t status;
	int ret;

	if (!dev) {
		return -EINVAL;
	}

	memset(dev, 0, sizeof(*dev));
	dev->irq = VIRTIO_MMIO_IRQ_BASE + device_num;

	if (sem_init(&dev->completion_sem, 0, 0) != OK) {
		return -errno;
	}

	ret = virtio_mmio_init(&dev->mmio_dev, device_num);
	if (ret != OK) {
		sem_destroy(&dev->completion_sem);
		return ret;
	}

	if (dev->mmio_dev.device_id != 2) {
		ret = -ENODEV;
		goto err_out;
	}

	status = VIRTIO_CONFIG_STATUS_ACK;
	virtio_mmio_set_status(&dev->mmio_dev, status);

	status |= VIRTIO_CONFIG_STATUS_DRIVER;
	virtio_mmio_set_status(&dev->mmio_dev, status);

	device_features = virtio_mmio_get_device_features(&dev->mmio_dev);
	dev->features = device_features;

	if (device_features & VIRTIO_BLK_F_BLK_SIZE) {
		driver_features |= VIRTIO_BLK_F_BLK_SIZE;
	}
	if (device_features & VIRTIO_BLK_F_FLUSH) {
		driver_features |= VIRTIO_BLK_F_FLUSH;
	}

	ret = virtio_mmio_set_driver_features(&dev->mmio_dev, driver_features);
	if (ret != OK) {
		goto err_out;
	}

	status |= VIRTIO_CONFIG_STATUS_FEATURES_OK;
	virtio_mmio_set_status(&dev->mmio_dev, status);
	if ((virtio_mmio_get_status(&dev->mmio_dev) & VIRTIO_CONFIG_STATUS_FEATURES_OK) == 0) {
		ret = -ENOTSUP;
		goto err_out;
	}

	virtio_blk_read_config(dev);

	ret = virtq_init(&dev->vq, VIRTIO_BLK_QUEUE_NUM);
	if (ret != OK) {
		goto err_out;
	}

	ret = virtio_blk_setup_queue(dev);
	if (ret != OK) {
		goto err_queue;
	}

	ret = irq_attach(dev->irq, virtio_blk_interrupt, dev);
	if (ret != OK) {
		goto err_queue;
	}
	up_enable_irq(dev->irq);

	status |= VIRTIO_CONFIG_STATUS_DRIVER_OK;
	virtio_mmio_set_status(&dev->mmio_dev, status);

	dev->ready = true;
	return OK;

err_queue:
	virtq_deinit(&dev->vq);
err_out:
	virtio_mmio_deinit(&dev->mmio_dev);
	sem_destroy(&dev->completion_sem);
	return ret;
}

void virtio_blk_deinit(virtio_blk_dev_t *dev)
{
	if (!dev || !dev->ready) {
		return;
	}

	up_disable_irq(dev->irq);
	virtio_mmio_set_status(&dev->mmio_dev, VIRTIO_CONFIG_STATUS_RESET);
	virtq_deinit(&dev->vq);
	virtio_mmio_deinit(&dev->mmio_dev);
	sem_destroy(&dev->completion_sem);
	dev->ready = false;
}

int virtio_blk_read(virtio_blk_dev_t *dev, uint64_t sector, void *buffer, size_t nsectors)
{
	struct virtq_desc desc[3];
	struct virtio_blk_req_hdr_s hdr;
	struct virtio_blk_req_footer_s footer;
	int ret;

	if (!dev || !dev->ready || !buffer || nsectors == 0) {
		return -EINVAL;
	}

	memset(&footer, 0xff, sizeof(footer));
	hdr.type = VIRTIO_BLK_T_IN;
	hdr.reserved = 0;
	hdr.sector = sector;

	desc[0].addr = (uint64_t)(uintptr_t)&hdr;
	desc[0].len = sizeof(hdr);
	desc[0].flags = 0;
	desc[0].next = 0;

	desc[1].addr = (uint64_t)(uintptr_t)buffer;
	desc[1].len = nsectors * dev->config.blk_size;
	desc[1].flags = VIRTQ_DESC_F_WRITE;
	desc[1].next = 0;

	desc[2].addr = (uint64_t)(uintptr_t)&footer;
	desc[2].len = sizeof(footer);
	desc[2].flags = VIRTQ_DESC_F_WRITE;
	desc[2].next = 0;

	ret = virtq_add_buffer(&dev->vq, desc, 1, 2, NULL);
	if (ret != OK) {
		return ret;
	}

	virtio_mmio_queue_notify(&dev->mmio_dev, 0);

	ret = virtio_blk_wait_for_completion(dev);
	if (ret != OK) {
		return ret;
	}

	return footer.status == VIRTIO_BLK_S_OK ? OK : -EIO;
}

int virtio_blk_write(virtio_blk_dev_t *dev, uint64_t sector, const void *buffer, size_t nsectors)
{
	struct virtq_desc desc[3];
	struct virtio_blk_req_hdr_s hdr;
	struct virtio_blk_req_footer_s footer;
	int ret;

	if (!dev || !dev->ready || !buffer || nsectors == 0) {
		return -EINVAL;
	}

	memset(&footer, 0xff, sizeof(footer));
	hdr.type = VIRTIO_BLK_T_OUT;
	hdr.reserved = 0;
	hdr.sector = sector;

	desc[0].addr = (uint64_t)(uintptr_t)&hdr;
	desc[0].len = sizeof(hdr);
	desc[0].flags = 0;
	desc[0].next = 0;

	desc[1].addr = (uint64_t)(uintptr_t)buffer;
	desc[1].len = nsectors * dev->config.blk_size;
	desc[1].flags = 0;
	desc[1].next = 0;

	desc[2].addr = (uint64_t)(uintptr_t)&footer;
	desc[2].len = sizeof(footer);
	desc[2].flags = VIRTQ_DESC_F_WRITE;
	desc[2].next = 0;

	ret = virtq_add_buffer(&dev->vq, desc, 2, 1, NULL);
	if (ret != OK) {
		return ret;
	}

	virtio_mmio_queue_notify(&dev->mmio_dev, 0);

	ret = virtio_blk_wait_for_completion(dev);
	if (ret != OK) {
		return ret;
	}

	return footer.status == VIRTIO_BLK_S_OK ? OK : -EIO;
}

int virtio_blk_flush(virtio_blk_dev_t *dev)
{
	struct virtq_desc desc[2];
	struct virtio_blk_req_hdr_s hdr;
	struct virtio_blk_req_footer_s footer;
	int ret;

	if (!dev || !dev->ready) {
		return -EINVAL;
	}

	if ((dev->features & VIRTIO_BLK_F_FLUSH) == 0) {
		return OK;
	}

	memset(&footer, 0xff, sizeof(footer));
	hdr.type = VIRTIO_BLK_T_FLUSH;
	hdr.reserved = 0;
	hdr.sector = 0;

	desc[0].addr = (uint64_t)(uintptr_t)&hdr;
	desc[0].len = sizeof(hdr);
	desc[0].flags = 0;
	desc[0].next = 0;

	desc[1].addr = (uint64_t)(uintptr_t)&footer;
	desc[1].len = sizeof(footer);
	desc[1].flags = VIRTQ_DESC_F_WRITE;
	desc[1].next = 0;

	ret = virtq_add_buffer(&dev->vq, desc, 1, 1, NULL);
	if (ret != OK) {
		return ret;
	}

	virtio_mmio_queue_notify(&dev->mmio_dev, 0);

	ret = virtio_blk_wait_for_completion(dev);
	if (ret != OK) {
		return ret;
	}

	return footer.status == VIRTIO_BLK_S_OK ? OK : -EIO;
}

uint64_t virtio_blk_get_capacity(virtio_blk_dev_t *dev)
{
	if (!dev || !dev->ready) {
		return 0;
	}

	return dev->config.capacity;
}

uint32_t virtio_blk_get_block_size(virtio_blk_dev_t *dev)
{
	if (!dev || !dev->ready) {
		return 0;
	}

	return dev->config.blk_size;
}
