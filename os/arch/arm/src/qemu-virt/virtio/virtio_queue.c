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
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "virtio_queue.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define VIRTQ_PAGE_SIZE 4096

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static uintptr_t virtq_align_up(uintptr_t value, uintptr_t align)
{
	return (value + align - 1) & ~(align - 1);
}

static int virtq_alloc_memory(virtq_t *vq)
{
	size_t desc_size;
	size_t avail_size;
	size_t used_offset;
	size_t used_size;
	size_t total_size;
	void *raw_mem;
	uintptr_t aligned_base;

	desc_size = sizeof(struct virtq_desc) * vq->num;
	avail_size = sizeof(uint16_t) * (3 + vq->num);
	used_offset = virtq_align_up(desc_size + avail_size, VIRTQ_PAGE_SIZE);
	used_size = sizeof(uint16_t) * 3 + sizeof(struct virtq_used_elem) * vq->num;
	total_size = used_offset + used_size;

	raw_mem = kmm_malloc(total_size + VIRTQ_PAGE_SIZE);
	if (!raw_mem) {
		return -ENOMEM;
	}

	aligned_base = virtq_align_up((uintptr_t)raw_mem, VIRTQ_PAGE_SIZE);
	memset((void *)aligned_base, 0, total_size);

	vq->queue_mem = raw_mem;
	vq->queue_paddr = aligned_base;
	vq->desc = (struct virtq_desc *)aligned_base;
	vq->avail = (struct virtq_avail *)(aligned_base + desc_size);
	vq->used = (struct virtq_used *)(aligned_base + used_offset);

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int virtq_init(virtq_t *vq, uint16_t num)
{
	int ret;

	if (!vq || num == 0 || (num & (num - 1)) != 0) {
		return -EINVAL;
	}

	memset(vq, 0, sizeof(virtq_t));
	vq->num = num;
	vq->num_mask = num - 1;

	ret = virtq_alloc_memory(vq);
	if (ret != OK) {
		return ret;
	}

	vq->ready = true;
	return OK;
}

void virtq_deinit(virtq_t *vq)
{
	if (!vq || !vq->ready) {
		return;
	}

	if (vq->queue_mem) {
		kmm_free(vq->queue_mem);
	}

	memset(vq, 0, sizeof(virtq_t));
}

int virtq_add_buffer(virtq_t *vq, struct virtq_desc *desc, uint16_t out_num, uint16_t in_num, void *data)
{
	uint16_t total_desc;
	uint16_t head;
	uint16_t ring_index;
	uint16_t i;

	(void)data;

	if (!vq || !vq->ready || !desc) {
		return -EINVAL;
	}

	total_desc = out_num + in_num;
	if (total_desc == 0 || total_desc > vq->num) {
		return -EINVAL;
	}

	head = vq->next_desc_idx;
	for (i = 0; i < total_desc; i++) {
		uint16_t idx = (head + i) & vq->num_mask;
		uint16_t next = (idx + 1) & vq->num_mask;

		vq->desc[idx] = desc[i];
		if (i + 1 < total_desc) {
			vq->desc[idx].flags |= VIRTQ_DESC_F_NEXT;
			vq->desc[idx].next = next;
		} else {
			vq->desc[idx].flags &= ~VIRTQ_DESC_F_NEXT;
			vq->desc[idx].next = 0;
		}
	}

	ring_index = vq->avail->idx & vq->num_mask;
	vq->avail->ring[ring_index] = head;
	vq->avail->idx++;
	vq->avail_idx = vq->avail->idx;
	vq->next_desc_idx = (head + total_desc) & vq->num_mask;

	return OK;
}

int virtq_get_buffer(virtq_t *vq, uint32_t *len)
{
	uint16_t used_index;

	if (!vq || !vq->ready) {
		return -EINVAL;
	}

	if (vq->last_used_idx == vq->used->idx) {
		return -EAGAIN;
	}

	used_index = vq->last_used_idx & vq->num_mask;
	if (len) {
		*len = vq->used->ring[used_index].len;
	}

	vq->last_used_idx++;
	return OK;
}

void virtq_kick(virtq_t *vq)
{
	(void)vq;
}
