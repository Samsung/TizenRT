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

#include <mqueue.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <queue.h>

#include <tinyara/log_dump/log_dump.h>
#include <tinyara/log_dump/log_dump_internal.h>
#if defined(CONFIG_LIB_MINIZ)
#include "miniz/miniz.h"
#endif
#if defined(CONFIG_LIB_LZMA)
#include <tinyara/lzma/LzmaLib.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef MIN
#define MIN(a, b)		   (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)		   (((a) > (b)) ? (a) : (b))
#endif

#define LOG_DUMP_OK			0	/* Used for successful return check */
#define LOG_DUMP_OK_HEAPCHK		1	/* To check heap available after success return */
#define LOG_DUMP_MEM_FAIL		-1
#define LOG_DUMP_COMPHEAD_OFFSET	-1
#define LOG_DUMP_OPT_FAIL		-2
#define LOG_DUMP_STR_FREEHEAP		15	/* String size to display Free heap size */
#define LOG_DUMP_COMPRESS_NODESZ	5	/* use 4 char to store compressed node size */

#define LOG_CHUNK_SIZE			sizeof(struct log_dump_chunk_s)

static bool is_started_to_save;

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

struct log_dump_chunk_s {
	struct log_dump_chunk_s *flink;
	char arr[CONFIG_LOG_DUMP_CHUNK_SIZE];	/* to store the compressed log */
	int comp_head;				/* starting index of the 1st compressed block in the chunk */
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
static signed char *uncomp_array;
static int uncomp_curbytes = 0;
static char comp_size[LOG_DUMP_COMPRESS_NODESZ];
#if defined(CONFIG_LIB_LZMA)
static unsigned int propsSize = LZMA_PROPS_SIZE;
#endif
static bool set_comp_head = false;
static mqd_t g_logdump_mq_fd;
static size_t writesize = CONFIG_LOG_DUMP_CHUNK_SIZE;
static unsigned char *out_buf;
static bool current_compression;
static int compress_ret;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

mqd_t log_dump_get_mqfd(void)
{
	return g_logdump_mq_fd;
}

int log_dump_init(void)
{
	sq_init(&log_dump_chunks);
	/* no chunks are present so we allocate new chunk */
	struct log_dump_chunk_s *node = (struct log_dump_chunk_s *)kmm_malloc(LOG_CHUNK_SIZE);

	if (node == NULL) {
		lldbg("memory allocation failure\n");
		return LOG_DUMP_MEM_FAIL;
	}
	uncomp_array = (signed char *)kmm_malloc(CONFIG_LOG_DUMP_CHUNK_SIZE);
	if (uncomp_array == NULL) {
		lldbg("memory allocation failure\n");
		kmm_free(node);
		return LOG_DUMP_MEM_FAIL;
	}
	node->comp_head = 0;		/* since first node */

	curbytes = 0;
	current_ptr = 0;
	sq_addfirst((sq_entry_t *)node, &log_dump_chunks);
	/* set the size of chunks to 1 chunk */
	log_dump_size = LOG_CHUNK_SIZE;
	/* Initialize to save the log by default */
	is_started_to_save = true;

	return LOG_DUMP_OK;
}

unsigned char *last_comp_block;
size_t last_comp_block_size;
int last_comp_block_ptr;
bool last_block_compression;

int log_dump_read_wake(void)
{
	last_block_compression = true;
	struct log_dump_msg_s msg;
	int ret;

	msg.comp = true;
	msg.ch = 'c';
	ret = mq_send(log_dump_get_mqfd(), (const char *)&msg, sizeof(struct log_dump_msg_s), 100);
	if (ret < 0) {
		lldbg("failed mq_send ret = %d\n", ret);
		return ERROR;
	}

	while (last_block_compression) ;
	return 0;
}

int log_dump_read_init(void)
{
	read_node = (struct log_dump_chunk_s *)sq_peek(&log_dump_chunks);	/* reset the read node to head */

	if (read_node == NULL) {
		lldbg("empty read node\n");
		return LOG_DUMP_MEM_FAIL;
	}

	if (read_node->comp_head < 0) {
		current_ptr = curbytes;
	} else {
		current_ptr = read_node->comp_head;
	}

	last_comp_block_size = CONFIG_LOG_DUMP_CHUNK_SIZE;

#if defined(CONFIG_LIB_LZMA)
	last_comp_block = (unsigned char *)kmm_malloc(4 + CONFIG_LOG_DUMP_CHUNK_SIZE + LZMA_PROPS_SIZE);
#elif defined(CONFIG_LIB_MINIZ)
	last_comp_block = (unsigned char *)kmm_malloc(4 + CONFIG_LOG_DUMP_CHUNK_SIZE);
#endif
	if (last_comp_block == NULL) {
		lldbg("memory allocation failure\n");
		return LOG_DUMP_MEM_FAIL;
	}

#if defined(CONFIG_LIB_LZMA)
	int ret = LzmaCompress(&last_comp_block[4 + LZMA_PROPS_SIZE], &last_comp_block_size, uncomp_array, uncomp_curbytes, &last_comp_block[4], &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);
#elif defined(CONFIG_LIB_MINIZ)
	int ret = mz_compress(&last_comp_block[4], &last_comp_block_size, uncomp_array, uncomp_curbytes);
#endif
	if (ret != Z_OK) {
		lldbg("Fail to compress ret = %d\n", ret);
		kmm_free(last_comp_block);
		return -ret;
	}

	snprintf(comp_size, LOG_DUMP_COMPRESS_NODESZ, "%04d", last_comp_block_size);
	for (int i = 0; i < LOG_DUMP_COMPRESS_NODESZ - 1; i++) {
		last_comp_block[i] = comp_size[i];
	}

	last_comp_block_size += 4;
	last_comp_block_ptr = 0;
	return 0;
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
			current_ptr = 0;	/* reset the read pointer as the head is removed */
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

static int log_dump_tobuffer(char ch, size_t *free_size)
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

	if (percent_free_size < CONFIG_LOG_DUMP_MAX_SIZE) {
		max_size = LOG_CHUNK_SIZE;	/* setting the lower limit */
	} else {
		max_size = CONFIG_LOG_DUMP_MAX_SIZE;	/* setting the upper limit */
	}

	if (curbytes == CONFIG_LOG_DUMP_CHUNK_SIZE) {	/* last chunk is full */

		if (log_dump_size + LOG_CHUNK_SIZE < max_size) {
			/* memory available, so fetch a new buffer */
			struct log_dump_chunk_s *node = (struct log_dump_chunk_s *)kmm_malloc(LOG_CHUNK_SIZE);

			if (node == NULL) {
				return LOG_DUMP_MEM_FAIL;
			}

			curbytes = 1;
			node->arr[0] = ch;
			sq_addlast((sq_entry_t *) node, &log_dump_chunks);
			if (set_comp_head) {	/* new node, so compressed block starts at 0 */
				node->comp_head = 0;
				set_comp_head = false;
			} else {
				node->comp_head = LOG_DUMP_COMPHEAD_OFFSET;
				set_comp_head = false;
			}
			log_dump_size += LOG_CHUNK_SIZE;

		} else {
			/* logs reached memory limit, reuse the head and free extra logs */
			struct log_dump_chunk_s *node = (struct log_dump_chunk_s *)sq_remfirst(&log_dump_chunks);
			curbytes = 1;
			current_ptr = 0;	/* reset the read pointer as head is modified */
			node->arr[0] = ch;
			sq_addlast((sq_entry_t *) node, &log_dump_chunks);
			if (set_comp_head) {	/* new node, so compressed block starts at 0 */
				node->comp_head = 0;
				set_comp_head = false;
			} else {
				node->comp_head = LOG_DUMP_COMPHEAD_OFFSET;
				set_comp_head = false;
			}
			log_dump_mem_check(max_size);
		}

	} else {					/* there is place in tail */
		struct log_dump_chunk_s *log_dump_tail = (struct log_dump_chunk_s *)sq_tail(&log_dump_chunks);
		log_dump_tail->arr[curbytes] = ch;
		curbytes += 1;
		if (set_comp_head) {	/* new node, so compressed block starts at 0 */
			if (log_dump_tail->comp_head < 0) {	/* compressed node not present */
				log_dump_tail->comp_head = curbytes - 1;
			}
			set_comp_head = false;
		}
		if (curbytes % CONFIG_LOG_DUMP_MEMCHECK_SIZE == 0) {
			log_dump_mem_check(max_size);
			return LOG_DUMP_OK_HEAPCHK;
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
	uncomp_array[uncomp_curbytes] = ch;
	uncomp_curbytes++;
	if (uncomp_curbytes == CONFIG_LOG_DUMP_CHUNK_SIZE) {
		/* compress the block, add it to the nodes, reset the uncomp_array */
		set_comp_head = true;
#if defined(CONFIG_LIB_LZMA)
		out_buf = (unsigned char *)kmm_malloc(CONFIG_LOG_DUMP_CHUNK_SIZE + LZMA_PROPS_SIZE);
#elif defined(CONFIG_LIB_MINIZ)
		out_buf = (unsigned char *)kmm_malloc(CONFIG_LOG_DUMP_CHUNK_SIZE);
#endif
		if (out_buf == NULL) {
			lldbg("memory allocation failure\n");
			return LOG_DUMP_MEM_FAIL;
		}

		current_compression = true;

		struct log_dump_msg_s msg;
		msg.comp = false;
		msg.ch = 'c';
		mq_send(log_dump_get_mqfd(), (const char *)&msg, sizeof(struct log_dump_msg_s), 100);

		while (current_compression) ;

		if (compress_ret != Z_OK) {
			lldbg("Fail to compress ret = %d\n", compress_ret);
			kmm_free(out_buf);
			return -compress_ret;
		}
		uncomp_curbytes = 0;	/* reset */
		snprintf(comp_size, LOG_DUMP_COMPRESS_NODESZ, "%04d", writesize);
		for (int i = 0; i < LOG_DUMP_COMPRESS_NODESZ - 1; i++) {
			ret = log_dump_tobuffer(comp_size[i], &free_size);
			if (ret < 0) {
				kmm_free(out_buf);
				return ret;
			}
		}

		for (int i = 0; i < writesize; i++) {
			ret = log_dump_tobuffer(out_buf[i], &free_size);
			if (ret < 0) {
				kmm_free(out_buf);
				return ret;
			}
		}
		kmm_free(out_buf);
		ret = LOG_DUMP_OK_HEAPCHK;

	} else {
		ret = LOG_DUMP_OK;
	}

	if (ret == LOG_DUMP_OK_HEAPCHK) {
		char fsz_array[LOG_DUMP_STR_FREEHEAP];	/* For Free heap memory string */
		snprintf(fsz_array, sizeof(size_t), "<fsz:%08d>", free_size);
		for (int i = 0; i < sizeof(fsz_array); i++) {
			uncomp_array[uncomp_curbytes++] = fsz_array[i];
		}
	}
	return ret;
}

size_t log_dump_read(FAR char *buffer, size_t buflen)
{
	size_t ret = 0;

	sched_lock();	/* to ensure that the read is not disturbed by add_char */

	while (ret < buflen) {
		if (read_node == (struct log_dump_chunk_s *)sq_tail(&log_dump_chunks)) {	/* last chunk */
			if (current_ptr == curbytes) {	/* all data has been read */
				if (last_comp_block_ptr < last_comp_block_size) {
					buffer[ret] = last_comp_block[last_comp_block_ptr];
					ret++;
					last_comp_block_ptr++;
				} else {
					break;
				}
			} else {
				buffer[ret] = read_node->arr[current_ptr];
				current_ptr++;
				ret++;
			}
		} else {	/* reading a filled chunk */

			buffer[ret] = read_node->arr[current_ptr];
			current_ptr++;
			ret++;
			if (current_ptr == CONFIG_LOG_DUMP_CHUNK_SIZE) {	/* completed reading a chunk */
				current_ptr = 0;
				read_node = (struct log_dump_chunk_s *)sq_next((sq_entry_t *)read_node);
			}
		}
	}

	sched_unlock();

	return ret;
}

int log_dump(int argc, char *argv[])
{
	struct mq_attr attr;

	attr.mq_maxmsg = 32;
	attr.mq_msgsize = sizeof(struct log_dump_msg_s);
	attr.mq_flags = 0;

	/* Create log dump message queue */
	g_logdump_mq_fd = mq_open("logdump_s", O_RDWR | O_CREAT, 0666, &attr);
	if (g_logdump_mq_fd == (mqd_t) ERROR) {
		lldbg("Fail to open mq, errno %d. EXIT!\n", errno);
		return 0;
	}

	log_dump_init();

	struct log_dump_msg_s msg;
	int nbytes;

	while (true) {
		nbytes = mq_receive(g_logdump_mq_fd, (char *)&msg, sizeof(struct log_dump_msg_s), NULL);
		if (nbytes <= 0) {
			continue;
		}
		if (msg.comp) {
			log_dump_read_init();
			last_block_compression = false;
		} else {
#if defined(CONFIG_LIB_LZMA)
			compress_ret = LzmaCompress(&out_buf[LZMA_PROPS_SIZE], &writesize, uncomp_array, CONFIG_LOG_DUMP_CHUNK_SIZE, out_buf, &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);
#elif defined(CONFIG_LIB_MINIZ)
			compress_ret = mz_compress(out_buf, &writesize, uncomp_array, CONFIG_LOG_DUMP_CHUNK_SIZE);
#endif
			current_compression = false;
		}
	}
}
