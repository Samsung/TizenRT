// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_include.h>
#include <common/bk_generic.h>
#include <common/bk_compiler.h>
#include <os/mem.h>
#include <os/os.h>

typedef struct kfifo
{
	unsigned int in;
	unsigned int out;

	unsigned int mask;

	unsigned int size;
	unsigned char *buffer;
} kfifo_t, *kfifo_ptr_t;

/**
 * kfifo_init - allocates a new FIFO using a preallocated buffer
 * @buffer: the preallocated buffer to be used.
 * @size: the size of the internal buffer, this have to be a power of 2.
 * @gfp_mask: get_free_pages mask, passed to ke_malloc()
 * @lock: the lock to be used to protect the fifo buffer
 *
 * Do NOT pass the kfifo to kfifo_free() after use ! Simply free the
 * struct kfifo with ke_free().
 */
__BK_INLINE struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size)
{
	struct kfifo *fifo;

	/* size must be a power of 2 */
	//BK_ASSERT(size & (size - 1));

	fifo = os_malloc(sizeof(struct kfifo));
	if (!fifo)
		return NULLPTR;

	fifo->buffer = buffer;
	fifo->size = size;
	fifo->in = 0;
	fifo->out = 0;
	fifo->mask = fifo->size - 1;

	return fifo;
}

/**
 * kfifo_alloc - allocates a new FIFO and its internal buffer
 * @size: the size of the internal buffer to be allocated.
 * @gfp_mask: get_free_pages mask, passed to ke_malloc()
 * @lock: the lock to be used to protect the fifo buffer
 *
 * The size will be rounded-up to a power of 2.
 */
__BK_INLINE struct kfifo *kfifo_alloc(unsigned int size)
{
	unsigned char *buffer;
	struct kfifo *ret;

	buffer = os_malloc(size);
	if (!buffer)
		return 0;
	os_memset(buffer, 0, size);

	ret = kfifo_init(buffer, size);

	if (!(ret))
		os_free(buffer);

	return ret;
}

/**
 * kfifo_free - frees the FIFO
 * @fifo: the fifo to be freed.
 */
__BK_INLINE void kfifo_free(struct kfifo *fifo)
{
	os_free(fifo->buffer);
	fifo->buffer = 0;

	os_free(fifo);
}

/**
 * __kfifo_put - puts some data into the FIFO, no locking version
 * @fifo: the fifo to be used.
 * @buffer: the data to be added.
 * @len: the length of the data to be added.
 *
 * This function copies at most 'len' bytes from the 'buffer' into
 * the FIFO depending on the free space, and returns the number of
 * bytes copied.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these functions.
 */
__BK_INLINE unsigned int kfifo_put(struct kfifo *fifo,
			 unsigned char *buffer, unsigned int len)
{
	unsigned int l;
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
	len = min(len, fifo->size - fifo->in + fifo->out);

	/* first put the data starting from fifo->in to buffer end */
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	os_memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);

	/* then put the rest (if any) at the beginning of the buffer */
	os_memcpy(fifo->buffer, buffer + l, len - l);

	fifo->in += len;
    GLOBAL_INT_RESTORE();

	return len;
}

/**
 * __kfifo_get - gets some data from the FIFO, no locking version
 * @fifo: the fifo to be used.
 * @buffer: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most 'len' bytes from the FIFO into the
 * 'buffer' and returns the number of copied bytes.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these functions.
 */
__BK_INLINE unsigned int kfifo_get(struct kfifo *fifo,
			 unsigned char *buffer, unsigned int len)
{
	unsigned int l;
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
	len = min(len, fifo->in - fifo->out);

	/* first get the data from fifo->out until the end of the buffer */
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
	os_memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);

	/* then get the rest (if any) from the beginning of the buffer */
	os_memcpy(buffer + l, fifo->buffer, len - l);

	fifo->out += len;

	if (fifo->out == fifo->in) {
		fifo->in = 0;
		fifo->out = 0;
	}
    GLOBAL_INT_RESTORE();

	return len;
}

__BK_INLINE unsigned int kfifo_data_size(struct kfifo *fifo)
{
	return (fifo->in - fifo->out);
}

__BK_INLINE unsigned int kfifo_unused(struct kfifo *fifo)
{
	return (fifo->mask + 1) - (fifo->in - fifo->out);
}

__BK_INLINE void kfifo_copy_out(struct kfifo *fifo, void *dst,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int l;

	off &= fifo->mask;

	l = min(len, size - off);

	os_memcpy(dst, (void *)(fifo->buffer + off), l);
	os_memcpy((void *)((unsigned int)dst + l), (void *)fifo->buffer, len - l);
	/*
	 * make sure that the data is copied before
	 * incrementing the fifo->out index counter
	 */
}

__BK_INLINE unsigned int kfifo_out_peek(struct kfifo *fifo,
			 unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)
		len = l;

	kfifo_copy_out(fifo, buffer, len, fifo->out);

	return len;
}

#ifdef __cplusplus
}
#endif
