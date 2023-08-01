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

#include <tinyara/log_dump/log_dump.h>
#include <tinyara/mm/mm.h>
#include <tinyara/config.h>

#include <stdlib.h>

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

static int tobuffer(const char ch)
{
	/* need to check if the current chunks size is over max_log_size or greater than x% of free heap */
	size_t max_size;

	size_t free_size = log_dump_get_free_size();
	size_t percent_free_size = (free_size * CONFIG_LOG_DUMP_MAX_FREE_HEAP) / 100;

	if (percent_free_size < CONFIG_LOG_DUMP_MAX_SIZE) {
		max_size = LOG_CHUNK_SIZE;	/* setting the lower limit */
	} else {
		max_size = CONFIG_LOG_DUMP_MAX_SIZE;	/* setting the upper limit */
	}

	if (g_comp_queue.curbytes == CONFIG_LOG_DUMP_CHUNK_SIZE) {	/* last chunk is full */

		if (g_comp_queue.size_of_chunks + LOG_CHUNK_SIZE < max_size) {
			/* memory available, so fetch a new buffer */

			log_dump_chunk_t *node = comp_chunk_alloc(LOG_CHUNK_SIZE);

			if (node == NULL) {
				return LOG_DUMP_MEM_FAIL;
			}

			g_comp_queue.curbytes = 0;
			node->arr[g_comp_queue.curbytes++] = ch;
			comp_chunk_append(node);
			g_comp_queue.size_of_chunks += LOG_CHUNK_SIZE;

		} else {
			/* logs reached memory limit, reuse the head and free extra logs */
			log_dump_chunk_t *node = comp_chunk_pop();
			
			g_comp_queue.curbytes = 0;
			node->arr[g_comp_queue.curbytes++] = ch;
			comp_chunk_append(node);
			log_dump_mem_check(max_size);
		}

	} else {					/* there is place in tail */
		log_dump_chunk_t *log_dump_tail = comp_chunk_tail();
		log_dump_tail->arr[g_comp_queue.curbytes++] = ch;
	}
	return LOG_DUMP_OK;
}

static int write_buffer(const char *buf, const int bufsize)
{
    int ret = 0;

    for (int i = 0; i < bufsize; i++) {
        ret = tobuffer(buf[i]);
        if (ret < 0)
            break;
    }

    return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

size_t log_dump_get_free_size()
{
    size_t free_size;

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    free_size = kmm_get_heap_free_size();
#else
    struct mallinfo mem;
#ifdef CONFIG_CAN_PASS_STRUCTS
    mem = kmm_mallinfo();
#else
    (void)kmm_mallinfo(&mem);
#endif
    free_size = mem.fordblks;
#endif

    return free_size;
}

int log_dump_append(const char *compressed_buffer, const int compressed_size)
{
    char comp_size[LOG_DUMP_COMPRESS_NODESZ];
    int ret;

    snprintf(comp_size, LOG_DUMP_COMPRESS_NODESZ, "%04d", compressed_size);
    ret = write_buffer(comp_size, LOG_DUMP_COMPRESS_NODESZ - 1);
    if (ret < 0)
		return ret;
    ret = write_buffer(compressed_buffer, compressed_size);

    return ret;
}
