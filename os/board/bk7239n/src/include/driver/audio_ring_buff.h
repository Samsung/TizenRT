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

#ifndef _AUDIO_RING_BUFF_H_
#define _AUDIO_RING_BUFF_H_

#include <common/bk_include.h>
#include <driver/dma.h>

/**
 * @brief DMA type definition
 */
typedef enum
{
    RB_DMA_TYPE_NULL  = 0,
    RB_DMA_TYPE_READ  = 1,
    RB_DMA_TYPE_WRITE = 2,
}RB_DMA_TYPE;

/**
 * @brief ring buffer context definition
 */
typedef struct _RingBufferContext
{
    uint8_t* address;     /**< memory base address      */
    uint32_t capacity;    /**< memory capacity in bytes */
    uint32_t wp;          /**< write point in bytes     */
    uint32_t rp;          /**< read point in bytes      */
    //void*    dma;
    dma_id_t dma_id;
    uint32_t dma_type;
}RingBufferContext;

/**
 * @brief ring buffer initialization
 * @param rb   ring buffer context pointer
 * @param addr ring buffer start address
 * @param capacity ring buffer capacity
 * @param dma dma handler/pointer
 * @param dma_type dma tyepe, @see RB_DMA_TYPE
 */
void ring_buffer_init(RingBufferContext* rb, uint8_t* addr, uint32_t capacity, dma_id_t dma_id, uint32_t dma_type);

/**
 * @brief ring buffer clear
 * @param rb   ring buffer context pointer
 */
void ring_buffer_clear(RingBufferContext* rb);

/**
 * @brief ring buffer read
 * @param rb     ring buffer context pointer
 * @param buffer buffer to be read into
 * @param size   read size
 * @return actually read size
 */
uint32_t ring_buffer_read(RingBufferContext* rb, uint8_t* buffer, uint32_t size);

/**
 * @brief ring buffer write
 * @param rb     ring buffer context pointer
 * @param buffer buffer to be write from
 * @param size   write size
 * @return actually write size
 */
uint32_t ring_buffer_write(RingBufferContext* rb, uint8_t* buffer, uint32_t size);

/**
 * @brief get filled size of ring buffer
 * @param rb     ring buffer context pointer
 * @return the filled size of ring buffer
 */
uint32_t ring_buffer_get_fill_size(RingBufferContext* rb);

/**
 * @brief get free size of ring buffer
 * @param rb     ring buffer context pointer
 * @return the free size of ring buffer
 */
uint32_t ring_buffer_get_free_size(RingBufferContext* rb);

/**
 * @brief ring buffer abort read
 * @param rb     ring buffer context pointer
 * @param size   abort read size
 * @return actually abort read size
 */
uint32_t ring_buffer_read_abort(RingBufferContext* rb, uint32_t size);

#endif
