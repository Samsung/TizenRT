/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef __MM_ASAN_H
#define __MM_ASAN_H
#include <tinyara/config.h>
#include <stdbool.h>

#ifdef CONFIG_MM_ASAN_RT

#define ASAN_SHADOW_OFFSET         (unsigned long) CONFIG_MM_ASAN_SHADOW_OFFSET
#define ASAN_SHADOW_SCALE_SHIFT    3
#define ASAN_SHADOW_SCALE_SIZE     (1UL << ASAN_SHADOW_SCALE_SHIFT)
#define ASAN_SHADOW_MASK           (ASAN_SHADOW_SCALE_SIZE - 1)
#define _RET_IP_		   (unsigned long)__builtin_return_address(0)
#define __alias(symbol)		   __attribute__((alias(#symbol)))
#define BITS_PER_LONG              sizeof(long) * 8
#define __round_mask(x, y)         ((__typeof__(x))((y)-1))
#define round_up(x, y)             ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y)           ((x) & ~__round_mask(x, y))

#define ASAN_MALLOC_REDZONE        0xFC	/* redzone inside heap object */
#define ASAN_MALLOC_FREE           0xFB	/* object was freed */

struct asan_access_info {
	const void *access_addr;
	const void *first_bad_addr;
	size_t access_size;
	bool is_write;
	unsigned long ip;
};

static inline void *asan_mem_to_shadow(const void *addr)
{
	return (void *)(((unsigned long)addr - CONFIG_RAM_START) >> ASAN_SHADOW_SCALE_SHIFT)
		   + ASAN_SHADOW_OFFSET;
}

static inline const void *asan_shadow_to_mem(const void *shadow_addr)
{
	return (void *)((((unsigned long)shadow_addr - ASAN_SHADOW_OFFSET)
					 << ASAN_SHADOW_SCALE_SHIFT) + CONFIG_RAM_START);
}

void asan_report_error(struct asan_access_info *info);
void asan_report_user_access(struct asan_access_info *info);
void asan_unpoison_shadow(const void *address, size_t size);

void asan_poison_heap(const void *address, size_t size);

#endif							/* CONFIG_MM_ASAN_RT */

#endif							/* __MM_ASAN_H */
