/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef SLSI_MAX_BUF_H__
#define SLSI_MAX_BUF_H__

#include <sys/types.h>
#include <arpa/inet.h>
#include "utils_scsc.h"
#include "utils_misc.h"

struct slsi_mbuf_fapi {
	u32 sig_length;
	u32 data_length;
	u32 frame_format;
};

struct max_buff_head {
	/* These two members must be first. */
	struct max_buff *next;
	struct max_buff *prev;

	unsigned int    queue_len;
	pthread_mutex_t lock;
};

/* Maximum number of packets to store in RX data mbuf queue */
#define SCSC_MAX_RX_MBUF_QUEUE_LEN (32)

/* WorqQueue to be used within SCSC driver for all MLME and internal works */
#define SCSC_WORK           HPWORK

/* Donot use LPWORK other than HIP4_WQ processing as this can lead into race condition if the other tasks
 * are blocked for waiting for a semaphore e.g <supplicant>_req takes vif_mutex and waits for a CFM.
 * If any code running in LPWORK also takes this semaphore than it will go into racecondition and hip4_wq will be in deadlock*/
#define SLSI_HIP_WORK_QID   LPWORK

/**
 * struct max_buff
 * next: Next max_buff in the list
 * prev: Previous max_buff in the list
 * buffer: buffer pointer of max_buff
 * buffer_len: Total size of the buffer
 * data_offset: Points to the starting of actual data in the buffer
 * data_len: Length of actual data in the buffer
 * fapi: Data related to FAPI
 * colour: HIP4 data
 * ac_queue: Queue number for the max_buff
 * mac_header: Starting offset of the MAC header
 * user_priority: TID priority of max_buff
 */
struct max_buff {
	struct max_buff       *next;
	struct max_buff       *prev;

	u8                    *buffer;
	u32                   buffer_len;
	u32                   data_offset;
	u32                   data_len;

	struct slsi_mbuf_fapi fapi;
	u32                   colour;
	u16                   ac_queue;
	u16                   mac_header;
	u16                   user_priority;
};

struct slsi_mbuf_work {
	struct slsi_dev      *sdev;
	struct netif         *dev; /* This can be NULL */
	struct work_s        work;
	struct max_buff_head queue;
	void                 *sync_ptr;
};

struct max_buff *mbuf_copy(const struct max_buff *mbuf);
unsigned char *mbuf_put(struct max_buff *mbuf, unsigned int len);
unsigned char *mbuf_push(struct max_buff *mbuf, unsigned int len);
unsigned char *mbuf_pull(struct max_buff *mbuf, unsigned int len);
struct max_buff *mbuf_alloc(unsigned int size);
struct max_buff *mbuf_dequeue(struct max_buff_head *list);
void mbuf_queue_head(struct max_buff_head *list, struct max_buff *mbuf);
void mbuf_queue_purge(struct max_buff_head *list);
void mbuf_free(struct max_buff *mbuf);
void mbuf_queue_tail(struct max_buff_head *list, struct max_buff *mbuf);
void mbuf_reset(struct max_buff *mbuf);

/**
 * Increase the headroom of an empty max_buff by moving the data_offset.
 * This is only allowed for an empty buffer.
 */
static inline void mbuf_reserve_headroom(struct max_buff *mbuf, int len)
{
	mbuf->data_offset += len;
}

/* This function initializes max_buff_head and the list lock */
static inline void mbuf_queue_head_init(struct max_buff_head *list)
{
	SLSI_MUTEX_INIT(list->lock);
	list->prev = list->next = (struct max_buff *)list;
	list->queue_len = 0;
}

/* This function returns the number of bytes of free space at the head of an max_buff */
static inline unsigned int mbuf_headroom(const struct max_buff *mbuf)
{
	return mbuf->data_offset;
}


static inline struct ethhdr *eth_hdr(const struct max_buff *mbuf)
{
	return (struct ethhdr *)(mbuf->buffer + mbuf->mac_header);
}

static inline void mbuf_set_mac_header(struct max_buff *mbuf, const int offset)
{
	mbuf->mac_header = mbuf->data_offset + offset;
}

static inline struct max_buff *slsi_mbuf_alloc(unsigned int size)
{
	struct max_buff *mbuf = mbuf_alloc(SLSI_NETIF_MBUF_HEADROOM + SLSI_NETIF_MBUF_TAILROOM + size);

	if (mbuf) {
		mbuf_reserve_headroom(mbuf, SLSI_NETIF_MBUF_HEADROOM);
	}
	return mbuf;
}

#define slsi_mbuf_get_data(mbuf_)           (&mbuf_->buffer[mbuf_->data_offset])
#define slsi_mbuf_copy(mbuf_)               mbuf_copy(mbuf_)
#define slsi_kfree_mbuf(mbuf_)              mbuf_free(mbuf_)
#define slsi_mbuf_queue_tail(list_, mbuf_)  mbuf_queue_tail(list_, mbuf_)
#define slsi_mbuf_queue_head(list_, mbuf_)  mbuf_queue_head(list_, mbuf_)
#define slsi_mbuf_dequeue(list_)            mbuf_dequeue(list_)
#define slsi_mbuf_queue_purge(list_)        mbuf_queue_purge(list_)

static inline int slsi_mbuf_work_init(struct slsi_dev *sdev, struct netif *dev, struct slsi_mbuf_work *work, const char *name, void (*func)(FAR void *arg))
{
	work->sync_ptr = (void *)sdev;
	work->sdev = sdev;
	work->dev = dev;
	mbuf_queue_head_init(&work->queue);
	work->work.worker = func;
	work->work.arg = (void *)&work;

	return 0;
}

static inline void slsi_mbuf_schedule_work(struct slsi_mbuf_work *work, void *func)
{
	work_queue(SCSC_WORK, &work->work, func, (FAR void *)work, 0);
}

static inline void slsi_mbuf_work_enqueue_l(struct slsi_mbuf_work *work, struct max_buff *mbuf, void *func)
{
	void *sync_ptr;

	sync_ptr = work->sync_ptr;

	if (WARN_ON(!sync_ptr)) {
		slsi_kfree_mbuf(mbuf);
		return;
	}

	if (work->queue.queue_len == 0) {
		mbuf_queue_tail(&work->queue, mbuf);
		slsi_mbuf_schedule_work(work, func);
	} else {
		mbuf_queue_tail(&work->queue, mbuf);
	}
}

static inline struct max_buff *slsi_mbuf_work_dequeue_l(struct slsi_mbuf_work *work)
{
	return mbuf_dequeue(&work->queue);
}

static inline void slsi_mbuf_work_deinit(struct slsi_mbuf_work *work)
{
	if (WARN_ON(!work->sync_ptr)) {
		return;
	}

	work->sync_ptr = NULL;

	work_cancel(SCSC_WORK, &work->work);
	slsi_mbuf_queue_purge(&work->queue);
}

#define slsi_mbuf_work_enqueue(work_, mbuf_, func_) slsi_mbuf_work_enqueue_l(work_, mbuf_, func_)
#define slsi_mbuf_work_dequeue(work_) slsi_mbuf_work_dequeue_l(work_)

#endif							/* SLSI_MAX_BUF_H__ */
