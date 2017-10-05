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

#include <arpa/inet.h>
#include "utils_scsc.h"
#include "utils_misc.h"

struct slsi_mbuf_cb {
	u32 sig_length;
	u32 data_length;
	u32 frame_format;
	u32 colour;
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
 * head: Head of max_buff buffer
 * data: Data pointer of max_buff
 * tail: Tail pointer of max_buff
 * end: End pointer of max_buff
 * len: Length of actual data in max_buff
 * cb: FAPI signal details are placed in this
 * queue_mapping: Queue mapping for for the max_buff
 * mac_header: Starting offset of the MAC header
 * priority: TID priority of max_buff
 * protocol: Ether protocol of the max_buff
 */
struct max_buff {
	struct max_buff     *next;
	struct max_buff     *prev;
	unsigned char       *head;
	unsigned char       *data;
	unsigned char       *tail;
	unsigned char       *end;
	unsigned int        len;
	struct slsi_mbuf_cb cb;
	unsigned short      queue_mapping;
	unsigned short      mac_header;
	unsigned short      priority;
	unsigned short      protocol;
};

struct slsi_mbuf_work {
	struct slsi_dev *sdev;
	struct netif *dev;			/* This can be NULL */
	struct work_s work;
	struct max_buff_head queue;
	void *sync_ptr;
};

struct max_buff *mbuf_copy(const struct max_buff *mbuf);
unsigned char *mbuf_put(struct max_buff *mbuf, unsigned int len);
unsigned char *mbuf_push(struct max_buff *mbuf, unsigned int len);
unsigned char *mbuf_pull(struct max_buff *mbuf, unsigned int len);
struct max_buff *alloc_mbuf(unsigned int size);
struct max_buff *mbuf_dequeue(struct max_buff_head *list);
void mbuf_queue_head(struct max_buff_head *list, struct max_buff *mbuf);
void mbuf_queue_purge(struct max_buff_head *list);
void free_mbuf(struct max_buff *mbuf);
void mbuf_queue_tail(struct max_buff_head *list, struct max_buff *mbuf);
void mbuf_reset(struct max_buff *mbuf);

static inline struct slsi_mbuf_cb *slsi_mbuf_cb_get(struct max_buff *mbuf)
{
	return (struct slsi_mbuf_cb *)&mbuf->cb;
}

static inline struct slsi_mbuf_cb *slsi_mbuf_cb_init(struct max_buff *mbuf)
{
	memset(&mbuf->cb, 0, sizeof(struct slsi_mbuf_cb));
	return slsi_mbuf_cb_get(mbuf);
}

/* Insert an max_buff on a list */
static inline void mbuf_insert(struct max_buff *mbuf, struct max_buff *prev,
			       struct max_buff *next, struct max_buff_head *list)
{
	mbuf->next = next;
	mbuf->prev = prev;
	next->prev  = prev->next = mbuf;
	list->queue_len++;
}

/**
 * Increase the headroom of an empty max_buff by moving the data and tail pointers.
 * This is only allowed for an empty buffer.
 */
static inline void mbuf_reserve(struct max_buff *mbuf, int len)
{
	mbuf->data += len;
	mbuf->tail += len;
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
	return mbuf->data - mbuf->head;
}

/* This function returns the number of bytes of free space at the tail of an max_buff */
static inline int mbuf_tailroom(const struct max_buff *mbuf)
{
	return mbuf->end - mbuf->tail;
}

/* Returns the length of a max_buff_head queue */
static inline __u32 mbuf_queue_len(const struct max_buff_head *list)
{
	return list->queue_len;
}

/* Removes max_buff from list. _Must_ be called atomically */
static inline void mbuf_unlink(struct max_buff *mbuf, struct max_buff_head *list)
{
	struct max_buff *next, *prev;

	list->queue_len--;
	next = mbuf->next;
	prev = mbuf->prev;
	mbuf->next = mbuf->prev = NULL;
	next->prev = prev;
	prev->next = next;
}

static inline unsigned char *mbuf_mac_header(const struct max_buff *mbuf)
{
	return mbuf->head + mbuf->mac_header;
}

static inline struct ethhdr *eth_hdr(const struct max_buff *mbuf)
{
	return (struct ethhdr *)mbuf_mac_header(mbuf);
}

static inline void mbuf_reset_mac_header(struct max_buff *mbuf)
{
	mbuf->mac_header = mbuf->data - mbuf->head;
}

static inline void mbuf_set_mac_header(struct max_buff *mbuf, const int offset)
{
	mbuf_reset_mac_header(mbuf);
	mbuf->mac_header += offset;
}

static inline struct max_buff *slsi_alloc_mbuf_f(unsigned int size, const char *file, int line)
{
	struct max_buff *mbuf = alloc_mbuf(SLSI_NETIF_MBUF_HEADROOM + SLSI_NETIF_MBUF_TAILROOM + size);

	SLSI_UNUSED_PARAMETER(file);
	SLSI_UNUSED_PARAMETER(line);

	if (mbuf) {
		mbuf_reserve(mbuf, SLSI_NETIF_MBUF_HEADROOM - SLSI_MBUF_GET_ALIGNMENT_OFFSET(mbuf));
	}
	return mbuf;
}

#define slsi_alloc_mbuf(size_)              slsi_alloc_mbuf_f(size_, __FILE__, __LINE__)
#define slsi_mbuf_copy(mbuf_)               mbuf_copy(mbuf_)
#define slsi_kfree_mbuf(mbuf_)              free_mbuf(mbuf_)
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
