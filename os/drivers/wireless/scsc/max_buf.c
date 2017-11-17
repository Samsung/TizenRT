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
#include "utils_misc.h"
#include "utils_scsc.h"
#include "max_buf.h"
#include "debug_scsc.h"

/* This function resets all the members of the max_buff */
void mbuf_reset(struct max_buff *mbuf)
{
	u8 *buffer;
	u32 buffer_len;

	if (mbuf == NULL) {
		return;
	}

	buffer = mbuf->buffer;
	buffer_len = mbuf->buffer_len;

	memset(mbuf, 0, sizeof(struct max_buff));

	mbuf->buffer = buffer;
	mbuf->buffer_len = buffer_len;
}

/* This function allocates a max_buff. This function retuns NULL if the memory allocation fails. */
struct max_buff *mbuf_alloc(unsigned int size)
{
	struct max_buff *mbuf;

	/* Allocate the max_buff structure */
	mbuf = kmm_malloc(sizeof(struct max_buff));
	if (!mbuf) {
		goto out;
	}

	memset(mbuf, 0, sizeof(struct max_buff));

	/* Allocate the buffer for max_buff */
	mbuf->buffer = kmm_malloc(size);
	if (!mbuf->buffer) {
		kmm_free(mbuf);
		mbuf = NULL;
		goto  out;
	}

	mbuf->buffer_len = size;
out:
	return mbuf;
}

/* This function releases memory allocated for the max_buff */
void mbuf_free(struct max_buff *mbuf)
{
	if (!mbuf) {
		return;
	}

	/* Free the buffer */
	if (mbuf->buffer) {
		kmm_free(mbuf->buffer);
	}

	kmm_free(mbuf);
}

/**
 * This function increases the actual data area of the max_buff by given number of bytes.
 * If it exceeds the total buffer size then error message will be printed.
 * A pointer to the first byte of the extra data is returned.
 */
unsigned char *mbuf_put(struct max_buff *mbuf, unsigned int len)
{
	unsigned char *tail = &mbuf->buffer[mbuf->data_offset + mbuf->data_len];

	if ((mbuf->data_offset + mbuf->data_len + len) > mbuf->buffer_len) {
		SLSI_WARN_NODEV("Not enough room at tail: mbuf->data_len:%d len:%d mbuf->buffer:%p, mbuf->data_offset: %d\n",
			mbuf->data_len, len, mbuf->buffer, mbuf->data_offset);
		return NULL;
	}

	mbuf->data_len += len;

	return tail;
}

/**
 * This function moves the data area of the buffer at the start of the buffer.
 * If it exceeds the total headroom then error message will be printed.
 * A pointer to the first byte of the extra data is returned.
 */
unsigned char *mbuf_push(struct max_buff *mbuf, unsigned int len)
{
	if (mbuf->data_offset < len) {
		SLSI_WARN_NODEV("Not enough room at head: mbuf->data_len:%d len:%d mbuf->buffer:%p mbuf->data_offset: %d\n",
			mbuf->data_len, len, mbuf->buffer, mbuf->data_offset);
		return NULL;
	}

	mbuf->data_offset -= len;
	mbuf->data_len += len;

	return slsi_mbuf_get_data(mbuf);
}

/**
 * This function removes given number of bytes from the actual data
 * A pointer to the next data in the buffer is returned.
 */
unsigned char *mbuf_pull(struct max_buff *mbuf, unsigned int len)
{
	if (len > mbuf->data_len)
		return NULL;

	mbuf->data_offset += len;
	mbuf->data_len -= len;

	return slsi_mbuf_get_data(mbuf);
}

/**
 * This function copies the max_buff header and it's buffer
 * Returns NULL if memory allocation fails else pointer to new max_buff
 */
struct max_buff *mbuf_copy(const struct max_buff *mbuf)
{
	struct max_buff *new_mbuf = mbuf_alloc(mbuf->buffer_len);

	if (!new_mbuf) {
		return NULL;
	}

	/* Set the data pointer */
	mbuf_reserve_headroom(new_mbuf, mbuf_headroom(mbuf));

	/* Set the tail pointer and length */
	mbuf_put(new_mbuf, mbuf->data_len);

	/* Copy the max_buff header */
	memcpy(&new_mbuf->fapi, &mbuf->fapi, sizeof(struct slsi_mbuf_fapi));
	new_mbuf->colour = mbuf->colour;
	new_mbuf->ac_queue = mbuf->ac_queue;
	new_mbuf->user_priority = mbuf->user_priority;
	new_mbuf->mac_header = mbuf->mac_header;

	/* Copy the mbuf data */
	memcpy(slsi_mbuf_get_data(new_mbuf), slsi_mbuf_get_data(mbuf), mbuf->data_len);

	return new_mbuf;
}

/**
 * This function returns the mbuf that is at the head of the list. This operation
 * is protected by list->lock. This function returns NULL if the list is empty.
 */
struct max_buff *mbuf_dequeue(struct max_buff_head *list)
{
	struct max_buff *mbuf;
	struct max_buff *prev;
	struct max_buff *next;

	if (!list) {
		return NULL;
	}

	SLSI_MUTEX_LOCK(list->lock);

	/* Check if the list is empty */
	if (list->next == (struct max_buff *)list) {
		SLSI_MUTEX_UNLOCK(list->lock);
		return NULL;
	}

	mbuf = list->next;
	next = mbuf->next;
	prev = mbuf->prev;
	mbuf->next = mbuf->prev = NULL;
	next->prev = prev;
	prev->next = next;
	list->queue_len--;
	SLSI_MUTEX_UNLOCK(list->lock);

	return mbuf;
}

/* Delete all buffers on an max_buff_head list. This operation is protected by list->lock */
void mbuf_queue_purge(struct max_buff_head *list)
{
	struct max_buff *mbuf = mbuf_dequeue(list);

	while (mbuf != NULL) {
		mbuf_free(mbuf);
		mbuf = mbuf_dequeue(list);
	}
}

/* Queue a buffer at the start of the list */
void mbuf_queue_head(struct max_buff_head *list, struct max_buff *mbuf)
{
	struct max_buff *prev;
	struct max_buff *next;

	SLSI_MUTEX_LOCK(list->lock);
	prev = (struct max_buff *)list;
	next = list->next;

	mbuf->next = next;
	mbuf->prev = prev;
	next->prev  = prev->next = mbuf;
	list->queue_len++;
	SLSI_MUTEX_UNLOCK(list->lock);
}

/* Queue a buffer at the tail of the list. */
void mbuf_queue_tail(struct max_buff_head *list, struct max_buff *mbuf)
{
	struct max_buff *prev;
	struct max_buff *next;

	SLSI_MUTEX_LOCK(list->lock);
	prev = list->prev;
	next = (struct max_buff *)list;

	mbuf->next = next;
	mbuf->prev = prev;
	next->prev  = prev->next = mbuf;
	list->queue_len++;
	SLSI_MUTEX_UNLOCK(list->lock);
}
