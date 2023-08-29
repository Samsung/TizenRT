/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "RingBuffer"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ringbuffer.h"

#define RB_LOGV(x, ...) printf("[%s][%s] " x, LOG_TAG, __func__, ##__VA_ARGS__)
#define RB_LOGD(x, ...) printf("[%s][%s] " x, LOG_TAG, __func__, ##__VA_ARGS__)
#define RB_LOGI(x, ...) printf("[%s][%s] " x, LOG_TAG, __func__, ##__VA_ARGS__)
#define RB_LOGW(x, ...) printf("[%s][%s] warn: " x, LOG_TAG, __func__, ##__VA_ARGS__)
#define RB_LOGE(x, ...) printf("[%s][%s] error: " x, LOG_TAG, __func__, ##__VA_ARGS__)

struct RingBuffer {
	uint8_t *start;
	uint32_t size;
	int32_t owns;
	uint8_t *wptr;
	uint8_t *rptr;
};

RingBuffer *RingBuffer_Create(void *data, uint32_t size, int32_t owns)
{
	RingBuffer *rb = (RingBuffer *)malloc(sizeof(RingBuffer));
	if (!rb) {
		RB_LOGE("alloc struct fail.\n");
		return NULL;
	}
	if (owns) {
		rb->start = malloc(size);
		if (!rb->start) {
			RB_LOGE("alloc data fail.\n");
			free(rb);
			return NULL;
		}
	} else {
		rb->start = (uint8_t *)data;
	}

	rb->size = size;
	rb->wptr = rb->start;
	rb->rptr = rb->start;
	rb->owns = owns;
	return rb;
}

int32_t RingBuffer_Destroy(RingBuffer *rb)
{
	if (rb->owns) {
		if (rb->start) {
			free(rb->start);
		}
	}
	free(rb);

	return 0;
}

uint32_t RingBuffer_Size(RingBuffer *rb)
{
	return rb->size;
}

uint32_t RingBuffer_Space(RingBuffer *rb)
{
	return ((rb->size - 1 + rb->rptr - rb->wptr) % (rb->size));
}

uint32_t RingBuffer_Available(RingBuffer *rb)
{
	return ((rb->size + rb->wptr - rb->rptr) % (rb->size));
}

int32_t RingBuffer_Write(RingBuffer *rb, uint8_t *buffer, uint32_t count)
{
	if (!count || !buffer) {
		RB_LOGW("try to write from empty buffer.\n");
		return -1;
	}

	if (rb->wptr < rb->rptr) {
		memcpy(rb->wptr, buffer, count);
		rb->wptr += count;
		if (rb->wptr == rb->start + rb->size) {
			rb->wptr = rb->start;
		}
	} else {
		uint32_t size = (uint32_t)(rb->start + rb->size - rb->wptr);
		if (size >= count) {
			size = count;
		}
		memcpy(rb->wptr, buffer, size);
		rb->wptr += size;
		if (rb->wptr == rb->start + rb->size) {
			rb->wptr = rb->start;
		}

		if (size != count) {
			memcpy(rb->wptr, buffer + size, count - size);
			rb->wptr += count - size;
			if (rb->wptr == rb->start + rb->size) {
				rb->wptr = rb->start;
			}
		}
	}
	return 0;
}

int32_t RingBuffer_Read(RingBuffer *rb, uint8_t *buffer, uint32_t count)
{
	if (!count || !buffer) {
		RB_LOGW("try to read to empty buffer.\n");
		return -1;
	}

	if (rb->wptr > rb->rptr) {
		memcpy(buffer, rb->rptr, count);
		rb->rptr += count;
		if (rb->rptr == rb->start + rb->size) {
			rb->rptr = rb->start;
		}
	} else {
		uint32_t size = (uint32_t)(rb->start + rb->size - rb->rptr);
		if (size >= count) {
			size = count;
		}
		memcpy(buffer, rb->rptr, size);

		rb->rptr += size;
		if (rb->rptr == rb->start + rb->size) {
			rb->rptr = rb->start;
		}

		if (size != count) {
			memcpy(buffer + size, rb->rptr, count - size);

			rb->rptr += count - size;
			if (rb->rptr == rb->start + rb->size) {
				rb->rptr = rb->start;
			}
		}
	}
	return 0;
}

uint8_t *RingBuffer_GetWritePoint(RingBuffer *rb)
{
	return rb->wptr;
}

uint8_t *RingBuffer_GetReadPoint(RingBuffer *rb)
{
	return rb->rptr;
}

void RingBuffer_Reset(RingBuffer *rb)
{
	rb->wptr = rb->rptr = rb->start;
}
