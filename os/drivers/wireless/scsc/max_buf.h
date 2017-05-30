/******************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 *****************************************************************************/

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

	__u32 qlen;
	pthread_mutex_t lock;
};

struct max_buff;

/* Maximum number of packets to store in RX data mbuf queue */
#define SCSC_MAX_RX_MBUF_QUEUE_LEN (32)

/* WorqQueue to be used within SCSC driver for all MLME and internal works */
#define SCSC_WORK           HPWORK

/* Donot use LPWORK other than HIP4_WQ processing as this can lead into race condition if the other tasks
 * are blocked for waiting for a semaphore e.g <supplicant>_req takes vif_mutex and waits for a CFM.
 * If any code running in LPWORK also takes this semaphore than it will go into racecondition and hip4_wq will be in deadlock*/
#define SLSI_HIP_WORK_QID   LPWORK

/**
 *	struct max_buff - socket buffer
 *	@next: Next maxwell buffer in list
 *	@prev: Previous maxwell buffer in list
 *	@cb: Control buffer. FAPI signal details are placed in this
 *	@len: Length of actual data
 *	@data_len: Data length
 *	@queue_mapping: Queue mapping for multiqueue devices
 *	@priority: Packet queueing priority
 *	@protocol: Packet protocol from driver
 *	@mac_header: Link layer header
 *	@tail: Tail pointer of data
 *	@end: End pointer of max_buff
 *	@head: Head of max_buff buffer
 *	@data: Data head pointer
 */

struct max_buff {
	struct max_buff *next;
	struct max_buff *prev;
	struct slsi_mbuf_cb cb;
	unsigned int len, data_len;
	__u16 queue_mapping;
	__u16 priority;
	__be16 protocol;
	__u16 mac_header;
	unsigned char *head;
	unsigned char *data;
	unsigned char *tail;
	unsigned char *end;
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
static inline unsigned int mbuf_headroom(const struct max_buff *mbuf);
struct max_buff *__alloc_mbuf(unsigned int size);
struct max_buff *mbuf_dequeue(struct max_buff_head *list);
void mbuf_queue_head(struct max_buff_head *list, struct max_buff *newsk);
void mbuf_queue_purge(struct max_buff_head *list);
void kfree_mbuf(struct max_buff *mbuf);
void mbuf_queue_tail(struct max_buff_head *list, struct max_buff *newsk);
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

/*
 *	Insert an max_buff on a list.
 *
 */
static inline void __mbuf_insert(struct max_buff *newsk, struct max_buff *prev, struct max_buff *next, struct max_buff_head *list)
{
	newsk->next = next;
	newsk->prev = prev;
	next->prev = prev->next = newsk;
	list->qlen++;
}

/**
 *	mbuf_reserve - adjust headroom
 *	@mbuf: buffer to alter
 *	@len: bytes to move
 *
 *	Increase the headroom of an empty &max_buff by reducing the tail
 *	room. This is only allowed for an empty buffer.
 */
static inline void mbuf_reserve(struct max_buff *mbuf, int len)
{
	mbuf->data += len;
	mbuf->tail += len;
}

static inline struct max_buff *alloc_mbuf(unsigned int size)
{
	return __alloc_mbuf(size);
}

static inline unsigned char *__mbuf_pull(struct max_buff *mbuf, unsigned int len)
{
	mbuf->len -= len;
	WARN_ON(mbuf->len < mbuf->data_len);
	return mbuf->data += len;
}

static inline unsigned char *mbuf_pull_inline(struct max_buff *mbuf, unsigned int len)
{
	return (len > mbuf->len) ? NULL : __mbuf_pull(mbuf, len);
}

/**
 *	__mbuf_queue_head_init - initialize  max_buff_head
 *	@list: queue to initialize
 *
 *	This initializes only the list and queue length aspects of
 *	an max_buff_head object. It can also be used for on-stack max_buff_head
 *	objects where the spinlock is known to not be used.
 */
static inline void __mbuf_queue_head_init(struct max_buff_head *list)
{
	list->prev = list->next = (struct max_buff *)list;
	list->qlen = 0;
}

/*
 * This function initializes qhead and related locks
 */
static inline void mbuf_queue_head_init(struct max_buff_head *list)
{
	SLSI_MUTEX_INIT(list->lock);
	__mbuf_queue_head_init(list);
}

/**
 *	mbuf_headroom - bytes at buffer head
 *	@mbuf: buffer to check
 *
 *	Return the number of bytes of free space at the head of an &max_buff.
 */
static inline unsigned int mbuf_headroom(const struct max_buff *mbuf)
{
	return mbuf->data - mbuf->head;
}

static inline bool mbuf_is_nonlinear(const struct max_buff *mbuf)
{
	return mbuf->data_len;
}

/**
 *	mbuf_tailroom - bytes at buffer end
 *	@mbuf: buffer to check
 *
 *	Return the number of bytes of free space at the tail of an max_buff
 */
static inline int mbuf_tailroom(const struct max_buff *mbuf)
{
	return mbuf_is_nonlinear(mbuf) ? 0 : mbuf->end - mbuf->tail;
}

/**
 *	mbuf_queue_len	- get queue length
 *	@list_: list to measure
 *
 *	Return the length of an &max_buff queue.
 */
static inline __u32 mbuf_queue_len(const struct max_buff_head *list_)
{
	return list_->qlen;
}

static inline void mbuf_set_tail_pointer(struct max_buff *mbuf, unsigned int offset)
{
	mbuf->tail = mbuf->data + offset;
}

static inline unsigned int mbuf_end_offset(const struct max_buff *mbuf)
{
	return mbuf->end - mbuf->head;
}

/*
 * remove max_buff from list. _Must_ be called atomically, and with
 * the list known..
 */
static inline void __mbuf_unlink(struct max_buff *mbuf, struct max_buff_head *list)
{
	struct max_buff *next, *prev;

	list->qlen--;
	next = mbuf->next;
	prev = mbuf->prev;
	mbuf->next = mbuf->prev = NULL;
	next->prev = prev;
	prev->next = next;
}

/**
 *	mbuf_peek - peek at the head of an &max_buff_head
 *	@list_: list to peek at
 *
 *	Peek an &max_buff. Unlike most other operations you _MUST_
 *	be careful with this one. A peek leaves the buffer on the
 *	list and someone else may run off with it. You must hold
 *	the appropriate locks or have a private queue to do this.
 *
 *	Returns %NULL for an empty list or a pointer to the head element.
 *	The reference count is not incremented and the reference is therefore
 *	volatile. Use with caution.
 */
static inline struct max_buff *mbuf_peek(const struct max_buff_head *list_)
{
	struct max_buff *mbuf = list_->next;

	if (mbuf == (struct max_buff *)list_) {
		mbuf = NULL;
	}
	return mbuf;
}

/**
 *	mbuf_peek_tail - peek at the tail of an &max_buff_head
 *	@list_: list to peek at
 *
 *	Peek an &max_buff. Unlike most other operations you _MUST_
 *	be careful with this one. A peek leaves the buffer on the
 *	list and someone else may run off with it. You must hold
 *	the appropriate locks or have a private queue to do this.
 *
 *	Returns %NULL for an empty list or a pointer to the tail element.
 *	The reference count is not incremented and the reference is therefore
 *	volatile. Use with caution.
 */
static inline struct max_buff *mbuf_peek_tail(const struct max_buff_head *list_)
{
	struct max_buff *mbuf = list_->prev;

	if (mbuf == (struct max_buff *)list_) {
		mbuf = NULL;
	}
	return mbuf;
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
		mbuf_reserve(mbuf, SLSI_NETIF_MBUF_HEADROOM);
	}
	return mbuf;
}

#define slsi_alloc_mbuf(size_)              slsi_alloc_mbuf_f(size_, __FILE__, __LINE__)
#define slsi_mbuf_copy(mbuf_)               mbuf_copy(mbuf_)
#define slsi_kfree_mbuf(mbuf_)              kfree_mbuf(mbuf_)
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

	if (work->queue.qlen == 0) {
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
