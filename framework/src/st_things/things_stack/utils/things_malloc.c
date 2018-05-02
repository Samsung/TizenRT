/****************************************************************************
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

#include <stdlib.h>
#include "things_malloc.h"

#include "logging/things_logger.h"
#include "things_util.h"

#ifdef ENABLE_MALLOC_DEBUG
#define TAG "[things_malloc]"
#endif

#ifdef ENABLE_MALLOC_DEBUG
static uint32_t count;
#endif

#ifdef ENABLE_THINGS_MALLOC
void *things_malloc(size_t size)
{
	RET_NULL_IF_PARAM_EXPR_IS_TRUE(TAG, size < 1);

	/* Allocating Memory for given size using malloc API
	 */
#ifdef ENABLE_MALLOC_DEBUG
	void *ptr = malloc(size);
	count++;
	THINGS_LOG_D(TAG, "malloc: ptr=%p, size=%u, count=%u", ptr, size, count);
	return ptr;
#else
	return malloc(size);
#endif
}

void *things_calloc(size_t num, size_t size)
{
	RET_NULL_IF_PARAM_EXPR_IS_TRUE(TAG, num < 1);
	RET_NULL_IF_PARAM_EXPR_IS_TRUE(TAG, size < 1);

	/* Allocating Memory for given size using calloc API
	 */
#ifdef ENABLE_MALLOC_DEBUG
	void *ptr = calloc(num, size);
	THINGS_LOG_D(TAG, "calloc: ptr=%p, num=%u, size=%u", ptr, num, size);
	return ptr;
#else
	return calloc(num, size);
#endif
}

void *things_realloc(void *ptr, size_t size)
{
	// Override realloc() behavior for NULL pointer which normally would
	// work as per malloc(), however we suppress the behavior of possibly
	// returning a non-null unique pointer.
	if (ptr == NULL) {
		return things_malloc(size);
	}
	// Otherwise leave the behavior up to realloc() itself:

#ifdef ENABLE_MALLOC_DEBUG
	void *newptr = realloc(ptr, size);
	THINGS_LOG_D(TAG, "realloc: ptr=%p, newptr=%p, size=%u", ptr, newptr, size);
	// Very important to return the correct pointer here, as it only *somtimes*
	// differs and thus can be hard to notice/test:
	return newptr;
#else
	return realloc(ptr, size);
#endif
}

void things_free(void *ptr)
{
#ifdef ENABLE_MALLOC_DEBUG
	// Since things_malloc() did not increment count if it returned NULL,
	// guard the decrement:
	if (ptr) {
		count--;
	}
	THINGS_LOG_D(TAG, "free: ptr=%p, count=%u", ptr, count);
#endif

	free(ptr);
	ptr = NULL;
}
#endif

