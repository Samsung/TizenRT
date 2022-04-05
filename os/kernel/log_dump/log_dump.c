/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <queue.h>

#include <tinyara/log_dump/log_dump.h>
#include <tinyara/log_dump/log_dump_internal.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef MIN
#define MIN(a, b)		   (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)		   (((a) > (b)) ? (a) : (b))
#endif

#define LOG_DUMP_OK			 0
#define LOG_DUMP_OK			0
#define LOG_DUMP_OK_FSZ		1
#define LOG_DUMP_MEM_FAIL		-1
#define LOG_DUMP_HEAP_FAIL		-2
#define LOG_DUMP_SIZE_FAIL		-3
#define LOG_DUMP_OPT_FAIL		-4
#define LOG_DUMP_STR_FREEMEM	14

#define LOG_CHUNK_SIZE			sizeof(struct log_dump_chunk_s)

static bool is_started_to_save;

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

struct log_dump_chunk_s {
	struct log_dump_chunk_s *flink;
	char arr[CONFIG_LOG_DUMP_CHUNK_SIZE];
};

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static sq_queue_t log_dump_chunks;	/* queue for holding the logs */
static int curbytes = 0;		/* number of bytes in the current chunk that were filled */
static int current_ptr = 0;		/* current pointer for read */
static size_t log_dump_size = 0;
static struct log_dump_chunk_s *read_node;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int log_dump_init(void)
{
	sq_init(&log_dump_chunks);
	/* no chunks are present so we allocate new chunk */
	struct log_dump_chunk_s *node = (struct log_dump_chunk_s *)kmm_malloc(LOG_CHUNK_SIZE);

	if (node == NULL) {
		return LOG_DUMP_MEM_FAIL;
	}

	curbytes = 0;
	current_ptr = 0;
	sq_addfirst((sq_entry_t *)node, &log_dump_chunks);
	/* set the size of chunks to 1 chunk */
	log_dump_size = LOG_CHUNK_SIZE;
	/* Initialize to save the log by default */
	is_started_to_save = true;

	return LOG_DUMP_OK;
}

void log_dump_read_init(void)
{
	read_node = (struct log_dump_chunk_s *)sq_peek(&log_dump_chunks);	/* reset the read node to head */
	current_ptr = 0;							/* reset the read pointer */
}

static void log_dump_mem_check(size_t max_size)
{
	if (log_dump_size > max_size) {
		/* remove extra chunks from start */
		size_t extra_chunk_size = log_dump_size - max_size;
		int extra_chunks_count = extra_chunk_size / LOG_CHUNK_SIZE;
		struct log_dump_chunk_s *next_chunk;
		
		for (int i = 0; i < extra_chunks_count; i++) {
			next_chunk = (struct log_dump_chunk_s *)sq_remfirst(&log_dump_chunks);
			current_ptr = 0; /* reset the read pointer as the head is removed */
			kmm_free(next_chunk);
		}
	}
}

int log_dump_set(FAR const char *buffer, size_t buflen)
{
	(void)buflen;

	if (strncmp(buffer, LOGDUMP_SAVE_START, strlen(LOGDUMP_SAVE_START) + 1) == 0) {
		is_started_to_save = true;
	} else if (strncmp(buffer, LOGDUMP_SAVE_STOP, strlen(LOGDUMP_SAVE_STOP) + 1) == 0) {
		is_started_to_save = false;
	} else {
		return LOG_DUMP_OPT_FAIL;
	}
	return LOG_DUMP_OK;
}

int log_dump_tobuffer(char ch, size_t *free_size)
{
	/* need to check if the current chunks size is over max_log_size or greater than x% of free heap */

	size_t max_size;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	*free_size = kmm_get_heap_free_size();
#else
	struct mallinfo mem;
#ifdef CONFIG_CAN_PASS_STRUCTS
	mem = kmm_mallinfo();
#else
	(void)kmm_mallinfo(&mem);
#endif
	*free_size = mem.fordblks;
#endif
	size_t percent_free_size = (*free_size * CONFIG_LOG_DUMP_MAX_FREE_HEAP) / 100;

	if(percent_free_size < CONFIG_LOG_DUMP_MAX_SIZE) {
		max_size = LOG_CHUNK_SIZE;
	} else {
		max_size = MIN(percent_free_size, CONFIG_LOG_DUMP_MAX_SIZE);			/* setting the upper limit */
		max_size = MAX(max_size, CONFIG_LOG_DUMP_MIN_SIZE);						/* checking the lower limit */
	}

	if (curbytes == CONFIG_LOG_DUMP_CHUNK_SIZE) { /* last chunk is full */

		if (log_dump_size + LOG_CHUNK_SIZE < max_size) {	
			/* memory available, so fetch a new buffer */
			struct log_dump_chunk_s *node = (struct log_dump_chunk_s *)kmm_malloc(LOG_CHUNK_SIZE);

			if (node == NULL) {
				return LOG_DUMP_MEM_FAIL;
			}

			curbytes = 1;
			node->arr[0] = ch;
			sq_addlast((sq_entry_t *)node, &log_dump_chunks);
			log_dump_size += LOG_CHUNK_SIZE;

		} else {
			/* logs reached memory limit, reuse the head and free extra logs */
			struct log_dump_chunk_s *node = (struct log_dump_chunk_s *)sq_remfirst(&log_dump_chunks);
			curbytes = 1;
			current_ptr = 0; /* reset the read pointer as head is modified */
			node->arr[0] = ch;
			sq_addlast((sq_entry_t *)node, &log_dump_chunks);
			log_dump_mem_check(max_size);
		}

	} else { /* there is place in tail */
		struct log_dump_chunk_s *log_dump_tail = (struct log_dump_chunk_s *)sq_tail(&log_dump_chunks);
		log_dump_tail->arr[curbytes] = ch;
		curbytes += 1;
		if (curbytes % CONFIG_LOG_DUMP_MEMCHECK_SIZE == 0) {
			log_dump_mem_check(max_size);
			return LOG_DUMP_OK_FSZ;
		}
	}

	return LOG_DUMP_OK;
}

int log_dump_save(char ch)
{
	if (is_started_to_save == false) {
		return LOG_DUMP_OK;
	}

	size_t free_size = 0;
	int ret = 0;
	char fsz_array[LOG_DUMP_STR_FREEMEM];

	ret = log_dump_tobuffer(ch, &free_size);
	if (ret == LOG_DUMP_OK_FSZ)
	{
		snprintf(fsz_array, sizeof(size_t), "<fsz:%d>", free_size);
		for(int i = 0; i < sizeof(fsz_array); i++)
		{
			ret = log_dump_tobuffer(fsz_array[i], &free_size);
			if(ret < 0)
				return ret;
		}
	}
	return ret;
}

size_t log_dump_read(FAR char *buffer, size_t buflen)
{
	size_t ret = 0;

	sched_lock(); /* to ensure that the read is not disturbed by add_char */

	while (ret < buflen) {
		if (read_node == (struct log_dump_chunk_s *)sq_tail(&log_dump_chunks)) { /* last chunk */
			if (current_ptr == curbytes) { /* all data has been read */
				break;
			} else {
				buffer[ret] = read_node->arr[current_ptr];
				current_ptr++;
				ret++;
			}
		} else { /* reading a filled chunk */

			buffer[ret] = read_node->arr[current_ptr];
			current_ptr++;
			ret++;
			if (current_ptr == CONFIG_LOG_DUMP_CHUNK_SIZE) { /* completed reading a chunk */
				current_ptr = 0;
				read_node = (struct log_dump_chunk_s *)sq_next((sq_entry_t *)read_node);
			}
		}
	}

	sched_unlock();

	return ret;
}
