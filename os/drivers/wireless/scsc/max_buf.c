/******************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 *****************************************************************************/
#include "utils_misc.h"
#include "utils_scsc.h"
#include "max_buf.h"
#include "debug_scsc.h"

/**
 *	mbuf_panic
 *	@mbuf:	buffer
 *	@sz:	size
 *	@addr:	address
 *	@msg:	mbuf_over_panic or mbuf_under_panic
 *
 *	Out-of-line support for mbuf_put() and mbuf_push().
 *	Called via the wrapper mbuf_over_panic() or mbuf_under_panic().
 */
static void mbuf_panic(struct max_buff *mbuf, unsigned int sz, void *addr, const char msg[])
{
	SLSI_WARN_NODEV("%s: text:%p len:%d put:%d head:%p data:%p tail:%#lx end:%#lx \n", msg, addr, mbuf->len, sz, mbuf->head, mbuf->data, (unsigned long)mbuf->tail, (unsigned long)mbuf->end);
}

static void mbuf_over_panic(struct max_buff *mbuf, unsigned int sz, void *addr)
{
	mbuf_panic(mbuf, sz, addr, __func__);
}

static void mbuf_under_panic(struct max_buff *mbuf, unsigned int sz, void *addr)
{
	mbuf_panic(mbuf, sz, addr, __func__);
}

void mbuf_reset(struct max_buff *mbuf)
{
	unsigned char *head;
	unsigned char *end;

	if (mbuf == NULL) {
		return;
	}

	head = mbuf->head;
	end = mbuf->end;

	memset(mbuf, 0, sizeof(struct max_buff));
	mbuf->head = mbuf->data = mbuf->tail = head;
	mbuf->end = end;
	mbuf->mac_header = (typeof(mbuf->mac_header)) ~ 0U;
}

/**
 *	__alloc_mbuf	-	allocate a maxwell buffer
 *	@size: size to allocate
 *
 *	Allocate a new &max_buff. The returned buffer has no headroom and a
 *	tail room of at least size bytes with reference count of one.
 *
 */
struct max_buff *__alloc_mbuf(unsigned int size)
{
	struct max_buff *mbuf = NULL;
	u8 *data;

	/* Get the HEAD */
	mbuf = kmm_malloc(sizeof(struct max_buff));
	if (!mbuf) {
		goto out;
	}

	data = kmm_malloc(size);

	if (!data) {
		goto nodata;
	}

	memset(mbuf, 0, sizeof(struct max_buff));

	mbuf->head = data;
	mbuf->data = data;
	mbuf->tail = mbuf->data;
	mbuf->end = mbuf->tail + size;
	mbuf->mac_header = (typeof(mbuf->mac_header)) ~ 0U;

out:
	return mbuf;
nodata:
	kmm_free(mbuf);
	mbuf = NULL;
	goto out;
}

/**
 *	kfree_mbuf - free an max_buff
 *	@mbuf: buffer to free
 *
 *	Drop a reference to the buffer and free it if the usage count has
 *	hit zero.
 */
void kfree_mbuf(struct max_buff *mbuf)
{
	if (!mbuf) {
		return;
	}

	/* Free the data: mbuf->head and mbuf->data are same */
	if (mbuf->head) {
		kmm_free(mbuf->head);
	}

	kmm_free(mbuf);
}

/**
 *	mbuf_put - add data to a buffer
 *	@mbuf:  buffer  pointer
 *	@len: lenght of data to add
 *
 *	This function writes the data at the end of the data and updates length appropriately.
 *	If it exceeds the total buffer size then panic will be raised. A pointer to the
 *	first byte of the extra data is returned.
 */
unsigned char *mbuf_put(struct max_buff *mbuf, unsigned int len)
{
	unsigned char *tmp = mbuf->tail;

	mbuf->tail += len;
	mbuf->len += len;
	if (mbuf->tail > mbuf->end) {
		mbuf_over_panic(mbuf, len, __builtin_return_address(0));
	}
	return tmp;
}

/**
 *	mbuf_push - add data at the start of a buffer
 *	@mbuf: buffer pointer
 *	@len: length of data to add
 *
 *	This function moves the data area of the buffer at the buffer
 *	start. If this would exceed the total headroom then panic will be raised.
 *    A pointer to the first byte of the extra data is returned.
 */
unsigned char *mbuf_push(struct max_buff *mbuf, unsigned int len)
{
	mbuf->data -= len;
	mbuf->len += len;
	if (mbuf->data < mbuf->head) {
		mbuf_under_panic(mbuf, len, __builtin_return_address(0));
	}
	return mbuf->data;
}

/**
 *	mbuf_pull - remove data from the start of a buffer
 *	@mbuf: buffer pointer
 *	@len: amount of data to remove
 *
 *	This function removes data from the start of a buffer, returning
 *	the memory to the headroom. A pointer to the next data in the buffer
 *	is returned. Once the data has been pulled future pushes will overwrite
 *	the old data.
 */
unsigned char *mbuf_pull(struct max_buff *mbuf, unsigned int len)
{
	return mbuf_pull_inline(mbuf, len);
}

/**
 *	mbuf_copy	-	create private copy of an max_buff
 *	@mbuf: buffer to copy
 *	@gfp_mask: allocation priority
 *
 *	Make a copy of both an &max_buff and its data. This is used when the
 *	caller wishes to modify the data and needs a private copy of the
 *	data to alter. Returns %NULL on failure or the pointer to the buffer
 *	on success. The returned buffer has a reference count of 1.
 *
 *	As by-product this function converts non-linear &max_buff to linear
 *	one, so that &max_buff becomes completely private and caller is allowed
 *	to modify all the data of returned buffer. This means that this
 *	function is not recommended for use in circumstances when only
 *	header is going to be modified. Use pmbuf_copy() instead.
 */
struct max_buff *mbuf_copy(const struct max_buff *mbuf)
{
	int headerlen = mbuf_headroom(mbuf);
	unsigned int size = mbuf_end_offset(mbuf) + mbuf->data_len;
	struct max_buff *n = __alloc_mbuf(size);

	if (!n) {
		return NULL;
	}

	/* Set the data pointer */
	mbuf_reserve(n, headerlen);
	/* Set the tail pointer and length */
	mbuf_put(n, mbuf->len);

	return n;
}

/**
 *	__mbuf_queue_after - queue a buffer at the list head
 *	@list: list to use
 *	@prev: place after this buffer
 *	@newsk: buffer to queue
 *
 *	Queue a buffer int the middle of a list. This function takes no locks
 *	and you must therefore hold required locks before calling it.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
static inline void __mbuf_queue_after(struct max_buff_head *list, struct max_buff *prev, struct max_buff *newsk)
{
	__mbuf_insert(newsk, prev, prev->next, list);
}

static inline void __mbuf_queue_before(struct max_buff_head *list, struct max_buff *next, struct max_buff *newsk)
{
	__mbuf_insert(newsk, next->prev, next, list);
}

static inline struct max_buff *__mbuf_dequeue(struct max_buff_head *list)
{
	struct max_buff *mbuf = mbuf_peek(list);

	if (mbuf) {
		__mbuf_unlink(mbuf, list);
	}
	return mbuf;
}

/**
 *	mbuf_dequeue - remove from the head of the queue
 *	@list: list to dequeue from
 *
 *	Remove the head of the list. The list lock is taken so the function
 *	may be used safely with other locking list functions. The head item is
 *	returned or %NULL if the list is empty.
 */
struct max_buff *mbuf_dequeue(struct max_buff_head *list)
{
	struct max_buff *result;

	SLSI_MUTEX_LOCK(list->lock);
	result = __mbuf_dequeue(list);
	SLSI_MUTEX_UNLOCK(list->lock);
	return result;
}

/**
 *	__mbuf_dequeue_tail - remove from the tail of the queue
 *	@list: list to dequeue from
 *
 *	Remove the tail of the list. This function does not take any locks
 *	so must be used with appropriate locks held only. The tail item is
 *	returned or %NULL if the list is empty.
 */
static inline struct max_buff *__mbuf_dequeue_tail(struct max_buff_head *list)
{
	struct max_buff *mbuf = mbuf_peek_tail(list);

	if (mbuf) {
		__mbuf_unlink(mbuf, list);
	}
	return mbuf;
}

/**
 *	mbuf_dequeue_tail - remove from the tail of the queue
 *	@list: list to dequeue from
 *
 *	Remove the tail of the list. The list lock is taken so the function
 *	may be used safely with other locking list functions. The tail item is
 *	returned or %NULL if the list is empty.
 */
struct max_buff *mbuf_dequeue_tail(struct max_buff_head *list)
{
	struct max_buff *result;

	SLSI_MUTEX_LOCK(list->lock);
	result = __mbuf_dequeue_tail(list);
	SLSI_MUTEX_UNLOCK(list->lock);
	return result;
}

/**
 *	mbuf_queue_purge - empty a list
 *	@list: list to empty
 *
 *	Delete all buffers on an &max_buff list. Each buffer is removed from
 *	the list and one reference dropped. This function takes the list
 *	lock and is atomic with respect to other list locking functions.
 */
void mbuf_queue_purge(struct max_buff_head *list)
{
	struct max_buff *mbuf;

	while ((mbuf = mbuf_dequeue(list)) != NULL) {
		kfree_mbuf(mbuf);
	}
}

/**
 *	mbuf_queue_head - queue a buffer at the list head
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the start of the list. This function takes the
 *	list lock and can be used safely with other locking &max_buff functions
 *	safely.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
void mbuf_queue_head(struct max_buff_head *list, struct max_buff *newsk)
{
	SLSI_MUTEX_LOCK(list->lock);
	__mbuf_queue_after(list, (struct max_buff *)list, newsk);
	SLSI_MUTEX_UNLOCK(list->lock);
}

/**
 *	mbuf_queue_tail - queue a buffer at the list tail
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the tail of the list. This function takes the
 *	list lock and can be used safely with other locking &max_buff functions
 *	safely.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
void mbuf_queue_tail(struct max_buff_head *list, struct max_buff *newsk)
{
	SLSI_MUTEX_LOCK(list->lock);
	__mbuf_queue_before(list, (struct max_buff *)list, newsk);
	SLSI_MUTEX_UNLOCK(list->lock);
}
