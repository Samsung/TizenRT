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

#ifndef AMEBA_UTILS_RINGBUFFER_H
#define AMEBA_UTILS_RINGBUFFER_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RingBuffer RingBuffer;

RingBuffer *RingBuffer_Create(void *data, uint32_t size, int32_t owns);
int32_t RingBuffer_Destroy(RingBuffer *rb);

uint32_t RingBuffer_Size(RingBuffer *rb);
uint32_t RingBuffer_Space(RingBuffer *rb);
uint32_t RingBuffer_Available(RingBuffer *rb);

int32_t RingBuffer_Write(RingBuffer *rb, uint8_t *buffer, uint32_t count);
int32_t RingBuffer_Read(RingBuffer *rb, uint8_t *buffer, uint32_t count);

void RingBuffer_Reset(RingBuffer *rb);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_UTILS_RINGBUFFER_H
