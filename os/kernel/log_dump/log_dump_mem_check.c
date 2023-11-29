/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "log_dump/log_dump.h"

#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/
/****************************************************************************
 * Public Variables
 ****************************************************************************/
/****************************************************************************
 * Private Variables
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/

void log_dump_mem_check(size_t max_size)
{
	/* If kernel heap is locked, then we cannot perform any
		* mem free opeartions. So we just return here. The mem
		* free will be done when the kernel heap is released
		*/
	if (IS_KMM_LOCKED()) {
		return;
	}
		

	if (g_comp_queue.size_of_chunks > max_size) {
		/* remove extra chunks from start */
		size_t extra_chunk_size = g_comp_queue.size_of_chunks - max_size;
		int extra_chunks_count = extra_chunk_size / LOG_CHUNK_SIZE;
		log_dump_chunk_t *next_chunk;

		for (int i = 0; i < extra_chunks_count; i++) {
			next_chunk = comp_chunk_pop();
			kmm_free(next_chunk);
		}
	}
}

