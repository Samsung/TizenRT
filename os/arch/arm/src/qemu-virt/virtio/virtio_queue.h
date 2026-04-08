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

#ifndef __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_QUEUE_H
#define __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_QUEUE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Virtqueue related definitions */

#define VIRTQ_DESC_F_NEXT	1	/* This marks a buffer as continuing via the next field */
#define VIRTQ_DESC_F_WRITE	2	/* This marks a buffer as write-only (otherwise read-only) */
#define VIRTQ_DESC_F_INDIRECT	4	/* This means the buffer contains a list of buffer descriptors */

#define VIRTQ_AVAIL_F_NO_INTERRUPT	1	/* Do not send interrupt when buffer is used */

#define VIRTQ_USED_F_NO_NOTIFY	1	/* Do not send notify when buffer is added to avail ring */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Virtqueue descriptor */
struct virtq_desc {
	uint64_t addr;			/* Address (guest-physical) */
	uint32_t len;			/* Length */
	uint16_t flags;			/* The flags as indicated above */
	uint16_t next;			/* We chain unused descriptors via this, too */
};

/* Virtqueue available ring */
struct virtq_avail {
	uint16_t flags;			/* Always zero */
	uint16_t idx;			/* Index of the next available descriptor */
	uint16_t ring[];		/* Ring of descriptors */
	/* Only if VIRTIO_F_EVENT_IDX */
	// uint16_t used_event;		/* Only if VIRTIO_F_EVENT_IDX */
};

/* Virtqueue used element */
struct virtq_used_elem {
	uint32_t id;			/* Index of start of used descriptor chain */
	uint32_t len;			/* Total length of the descriptor chain which was used */
};

/* Virtqueue used ring */
struct virtq_used {
	uint16_t flags;			/* Always zero */
	volatile uint16_t idx;		/* Index of the next used descriptor */
	struct virtq_used_elem ring[];	/* Ring of used descriptors */
	/* Only if VIRTIO_F_EVENT_IDX */
	// uint16_t avail_event;		/* Only if VIRTIO_F_EVENT_IDX */
};

/* Virtqueue structure */
struct virtq {
	struct virtq_desc *desc;	/* Descriptor table */
	struct virtq_avail *avail;	/* Available ring */
	struct virtq_used *used;	/* Used ring */
	void *queue_mem;			/* Raw queue allocation */
	uintptr_t queue_paddr;		/* Queue base address */
	uint16_t num;			/* Number of descriptors */
	uint16_t num_mask;		/* Number of descriptors mask */
	uint16_t last_used_idx;		/* Last used index */
	uint16_t avail_idx;		/* Available index */
	uint16_t next_desc_idx;		/* Next descriptor head */
	bool ready;			/* Queue ready status */
};

typedef struct virtq virtq_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Virtqueue management */

int virtq_init(virtq_t *vq, uint16_t num);
void virtq_deinit(virtq_t *vq);
int virtq_add_buffer(virtq_t *vq, struct virtq_desc *desc, uint16_t out_num, uint16_t in_num, void *data);
int virtq_get_buffer(virtq_t *vq, uint32_t *len);
void virtq_kick(virtq_t *vq);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __ARCH_ARM_SRC_QEMU_VIRT_VIRTIO_QUEUE_H */
