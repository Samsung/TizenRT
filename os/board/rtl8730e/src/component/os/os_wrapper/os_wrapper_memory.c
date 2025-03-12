/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "ameba.h"
#include "os_wrapper.h"

/* For Smart, need to align malloc to 64bytes (cache line size of AP) for cache operations */
#define ALIGN 64
#define ALIGN_MASK 0x003f

void *rtos_mem_malloc(uint32_t size)
{
	void *pbuf = NULL;
	if ((size & ALIGN_MASK) != 0x00) {
		size += (ALIGN - (size & ALIGN_MASK));
	}	
	pbuf = kmm_memalign(ALIGN, size);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	if (pbuf){
		DEBUG_SET_CALLER_ADDR(pbuf);
	}
#endif
	return pbuf;
}

void *rtos_mem_zmalloc(uint32_t size)
{
	void *pbuf = NULL;

	if ((size & ALIGN_MASK) != 0x00) {
		size += (ALIGN - (size & ALIGN_MASK));
	}
	pbuf = kmm_memalign(ALIGN, size);
	if (pbuf){
		memset(pbuf, 0, size);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(pbuf);
#endif
	}
	return pbuf;
}

void *rtos_mem_calloc(uint32_t elementNum, uint32_t elementSize)
{
	u32 sz = elementNum * elementSize;
	if ((sz & ALIGN_MASK) != 0x00) {
		sz += (ALIGN - (sz & ALIGN_MASK));
	}
	void *pbuf = kmm_memalign(ALIGN, sz);
	if (pbuf){
		memset(pbuf, 0, sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(pbuf);
#endif
	}
	return pbuf;
}

void *rtos_mem_realloc(void *pbuf, uint32_t size)
{
	return kmm_realloc(pbuf, size);
}

void rtos_mem_free(void *pbuf)
{
	if (pbuf == NULL) {
		return;
	}
	kmm_free(pbuf);
}

uint32_t rtos_mem_get_free_heap_size(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return 0;
}

uint32_t rtos_mem_get_minimum_ever_free_heap_size(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return 0;
}