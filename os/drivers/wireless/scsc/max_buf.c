/******************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 *****************************************************************************/
#include "utils_misc.h"
#include "utils_scsc.h"
#include "max_buf.h"
#include "debug_scsc.h"

/* This function resets all the members of the max_buff */
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
}

/* This function allocates a max_buff. This function retuns NULL if the memory allocation fails. */
struct max_buff *alloc_mbuf(unsigned int size)
{
	struct max_buff *mbuf = NULL;
	u8 *data;

	/* Allocate the max_buff structure */
	mbuf = kmm_malloc(sizeof(struct max_buff));
	if (!mbuf) {
		goto out;
	}

	/* Allocate the data for max_buff */
	data = kmm_malloc(size);
	if (!data) {
		kmm_free(mbuf);
		mbuf = NULL;
		goto  out;
	}

	memset(mbuf, 0, sizeof(struct max_buff));

	mbuf->head = data;
	mbuf->data = data;
	mbuf->tail = mbuf->data;
	mbuf->end = mbuf->tail + size;
out:
	return mbuf;
}

/* This function releases memory allocated for the max_buff */
void free_mbuf(struct max_buff *mbuf)
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
 * This function moves the tail pointer and increases the length of the max_buff
 * by given number of bytes.
 * If it exceeds the total buffer size then error message will be printed.
 * A pointer to the first byte of the extra data is returned.
 */
unsigned char *mbuf_put(struct max_buff *mbuf, unsigned int len)
{
	unsigned char *tmp = mbuf->tail;

	mbuf->tail += len;
	mbuf->len  += len;
	if (mbuf->tail > mbuf->end) {
		SLSI_WARN_NODEV("%s: over_panic: mbuf->len:%d len:%d head:%p data:%p tail:%#lx end:%#lx\n",
			mbuf->len, len, mbuf->head, mbuf->data,
			(unsigned long)mbuf->tail, (unsigned long)mbuf->end);
	}

	return tmp;
}

/**
 * This function moves the data area of the buffer at the start of the buffer.
 * If it exceeds the total headroom then error message will be printed.
 * A pointer to the first byte of the extra data is returned.
 */
unsigned char *mbuf_push(struct max_buff *mbuf, unsigned int len)
{
	mbuf->data -= len;
	mbuf->len  += len;
	if (mbuf->data < mbuf->head) {
		SLSI_WARN_NODEV("%s: under_panic: mbuf->len:%d len:%d head:%p data:%p tail:%#lx end:%#lx\n",
			mbuf->len, len, mbuf->head, mbuf->data,
			(unsigned long)mbuf->tail, (unsigned long)mbuf->end);
	}

	return mbuf->data;
}

/**
 * This function removes data from the start of a buffer.
 * A pointer to the next data in the buffer is returned.
 */
unsigned char *mbuf_pull(struct max_buff *mbuf, unsigned int len)
{
	if (len > mbuf->len)
		return NULL;

	mbuf->len -= len;

	return mbuf->data += len;
}

/**
 * This function copies the max_buff header and it's data
 * Returns NULL if memory allocation fails else pointer to new max_buff
 */
struct max_buff *mbuf_copy(const struct max_buff *mbuf)
{
	unsigned int    mbuf_size = mbuf->end - mbuf->head;
	struct max_buff *new_mbuf = alloc_mbuf(mbuf_size);

	if (!new_mbuf) {
		return NULL;
	}

	/* Set the data pointer */
	mbuf_reserve(new_mbuf, mbuf_headroom(mbuf));

	/* Set the tail pointer and length */
	mbuf_put(new_mbuf, mbuf->len);

	/* Copy the max_buff header */
	memcpy(&new_mbuf->cb, &mbuf->cb, sizeof(struct slsi_mbuf_cb));
	new_mbuf->queue_mapping = mbuf->queue_mapping;
	new_mbuf->priority = mbuf->priority;
	new_mbuf->protocol = mbuf->protocol;
	new_mbuf->mac_header = mbuf->mac_header;

	/* Copy the mbuf->data */
	memcpy(new_mbuf->data, mbuf->data, mbuf->len);

	return new_mbuf;
}

/**
 * This function returns the mbuf that is at the head of the list. This operation
 * is protected by list->lock. This function returns NULL if the list is empty.
 */
struct max_buff *mbuf_dequeue(struct max_buff_head *list)
{
	struct max_buff *mbuf;

	SLSI_MUTEX_LOCK(list->lock);
	mbuf = list->next;
	if (mbuf == (struct max_buff *)list)
		mbuf = NULL;

	if (mbuf) {
		mbuf_unlink(mbuf, list);
	}
	SLSI_MUTEX_UNLOCK(list->lock);

	return mbuf;
}

/* Delete all buffers on an max_buff_head list. This operation is protected by list->lock */
void mbuf_queue_purge(struct max_buff_head *list)
{
	struct max_buff *mbuf;

	mbuf = mbuf_dequeue(list);
	while (mbuf != NULL) {
		free_mbuf(mbuf);
		mbuf = mbuf_dequeue(list);
	}
}

/* Queue a buffer at the start of the list */
void mbuf_queue_head(struct max_buff_head *list, struct max_buff *mbuf)
{
	SLSI_MUTEX_LOCK(list->lock);
	mbuf_insert(mbuf, (struct max_buff *)list, list->next, list);
	SLSI_MUTEX_UNLOCK(list->lock);
}

/* Queue a buffer at the tail of the list. */
void mbuf_queue_tail(struct max_buff_head *list, struct max_buff *mbuf)
{
	SLSI_MUTEX_LOCK(list->lock);
	mbuf_insert(mbuf, list->prev, (struct max_buff *)list, list);
	SLSI_MUTEX_UNLOCK(list->lock);
}
