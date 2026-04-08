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
#include <tinyara/kmalloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "virtio_blk.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

static virtio_blk_dev_t g_blk_dev;
static bool g_blk_ready;

static int qemu_virtio_blk_fail(int ret, const char *message)
{
	printf("VIRTIO_BLK_TEST: FAIL");
	if (message) {
		printf(" (%s", message);
		if (ret < 0) {
			printf(": %d", ret);
		}
		printf(")");
	}
	printf("\n");
	return ret < 0 ? ret : ERROR;
}

int qemu_virtio_blk_initialize(void)
{
	int ret;

	if (g_blk_ready) {
		return OK;
	}

	memset(&g_blk_dev, 0, sizeof(g_blk_dev));
	ret = virtio_blk_init(&g_blk_dev, 0);
	if (ret != OK) {
		printf("virtio-blk init failed: %d\n", ret);
		return ret;
	}

	g_blk_ready = true;
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: test_virtio_blk
 *
 * Description:
 *   Test function for virtio block device
 *
 ****************************************************************************/

int test_virtio_blk(int argc, char *argv[])
{
	int ret;
	uint64_t capacity;
	uint32_t block_size;
	uint8_t *write_buffer;
	uint8_t *read_buffer;
	uint64_t test_sector = 1;
	size_t test_sectors = 1;
	size_t buffer_len;
	uint32_t index;

	ret = qemu_virtio_blk_initialize();
	if (ret != OK) {
		return qemu_virtio_blk_fail(ret, "init");
	}

	capacity = virtio_blk_get_capacity(&g_blk_dev);
	block_size = virtio_blk_get_block_size(&g_blk_dev);
	if (block_size == 0) {
		block_size = 512;
	}

	if (capacity <= test_sector) {
		return qemu_virtio_blk_fail(-ENOSPC, "capacity");
	}

	buffer_len = block_size * test_sectors;
	write_buffer = kmm_malloc(buffer_len);
	read_buffer = kmm_malloc(buffer_len);
	if (!write_buffer || !read_buffer) {
		if (write_buffer) {
			kmm_free(write_buffer);
		}
		if (read_buffer) {
			kmm_free(read_buffer);
		}
		return qemu_virtio_blk_fail(-ENOMEM, "alloc");
	}

	for (index = 0; index < buffer_len; index++) {
		write_buffer[index] = (uint8_t)(0xA5 ^ (index & 0xFF));
	}
	memset(read_buffer, 0, buffer_len);

	ret = virtio_blk_write(&g_blk_dev, test_sector, write_buffer, test_sectors);
	if (ret != OK) {
		kmm_free(write_buffer);
		kmm_free(read_buffer);
		return qemu_virtio_blk_fail(ret, "write");
	}

	ret = virtio_blk_flush(&g_blk_dev);
	if (ret != OK) {
		kmm_free(write_buffer);
		kmm_free(read_buffer);
		return qemu_virtio_blk_fail(ret, "flush");
	}

	ret = virtio_blk_read(&g_blk_dev, test_sector, read_buffer, test_sectors);
	if (ret != OK) {
		kmm_free(write_buffer);
		kmm_free(read_buffer);
		return qemu_virtio_blk_fail(ret, "read");
	}

	ret = memcmp(write_buffer, read_buffer, buffer_len);
	kmm_free(write_buffer);
	kmm_free(read_buffer);
	if (ret != 0) {
		return qemu_virtio_blk_fail(-EIO, "verify");
	}

	printf("VIRTIO_BLK_TEST: PASS\n");
	return OK;
}
