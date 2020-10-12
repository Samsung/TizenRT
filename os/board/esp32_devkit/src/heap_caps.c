/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <tinyara/mm/mm.h>
#include <tinyara/kmalloc.h>

#include "esp_attr.h"
#include "esp_heap_caps.h"

/*
Routine to allocate a bit of memory with certain capabilities. caps is a bitfield of MALLOC_CAP_* bits.
*/
IRAM_ATTR void *heap_caps_malloc(size_t size, uint32_t caps)
{
	if (caps & MALLOC_CAP_EXEC) {
		//MALLOC_CAP_EXEC forces an alloc from IRAM. There is a region which has both this as well as the following
		//caps, but the following caps are not possible for IRAM.  Thus, the combination is impossible and we return
		//NULL directly, even although our heap capabilities (based on soc_memory_tags & soc_memory_regions) would
		//indicate there is a tag for this.
		if ((caps & MALLOC_CAP_8BIT) || (caps & MALLOC_CAP_DMA)) {
			return NULL;
		}
		caps |= MALLOC_CAP_32BIT; // IRAM is 32-bit accessible RAM
	}

	if (caps & MALLOC_CAP_32BIT) {
		/* 32-bit accessible RAM should allocated in 4 byte aligned sizes
		 * (Future versions of ESP-IDF should possibly fail if an invalid size is requested)
		 */
		size = (size + 3) & (~3);
	}

	int index = 0;
#ifdef CONFIG_RAM_MALLOC_PRIOR_INDEX
	index = CONFIG_RAM_MALLOC_PRIOR_INDEX;
#endif
	if (caps & MALLOC_CAP_EXEC || caps & MALLOC_CAP_DMA || MALLOC_CAP_INTERNAL) {
		index = 0;
	}

	return kmm_malloc_at(index, size);;
}

IRAM_ATTR void heap_caps_free(void *ptr)
{
	if (ptr == NULL) {
		return;
	}

	kmm_free(ptr);
}

IRAM_ATTR void *heap_caps_realloc(void *ptr, size_t size, int caps)
{
	if (ptr == NULL) {
		return heap_caps_malloc(size, caps);
	}

	if (size == 0) {
		heap_caps_free(ptr);
		return NULL;
	}

	return kmm_realloc(ptr, size);
}

IRAM_ATTR void *heap_caps_calloc(size_t n, size_t size, uint32_t caps)
{
	void *result = NULL;
	size_t size_bytes = n * size;

	if (n > 0 && size > 0) {
		result = heap_caps_malloc(size_bytes, caps);
		if (result != NULL) {
			bzero(result, size_bytes);
		}
	}
	return result;
}
