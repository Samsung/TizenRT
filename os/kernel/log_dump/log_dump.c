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
#include <tinyara/compression.h>

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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef MIN
#define MIN(a, b)		   (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)		   (((a) > (b)) ? (a) : (b))
#endif

#define LOG_DUMP_MSGQ	"logdump_s"

#define LOG_DUMP_OK			0	/* Used for successful return check */
#define LOG_DUMP_OK_HEAPCHK		1	/* To check heap available after success return */
#define LOG_DUMP_MEM_FAIL		-1
#define LOG_DUMP_COMPHEAD_OFFSET	-1
#define LOG_DUMP_OPT_FAIL		-2
#define LOG_DUMP_FAIL			-99
#define LOG_DUMP_STRLEN_FREEHEAP		15	/* String size to display Free heap size */
#define LOG_DUMP_COMPRESS_NODESZ	5	/* use 4 char to store compressed node size */

#define LOG_CHUNK_SIZE			sizeof(struct log_dump_chunk_s)

static bool is_started_to_save;

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

struct log_dump_msg_s {
      bool comp;
};

/* log_dump_chunk_s is the compressed data node */
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
static mqd_t mq_fd;
static sq_queue_t log_dump_chunks;	/* queue for holding the logs */
static int compress_curbytes = 0;	/* number of bytes in the current chunk that were filled */
static int compress_rdptr = 0;		/* current pointer for compressed data read */
static size_t log_dump_size = 0;
static struct log_dump_chunk_s *read_node;
static signed char uncomp_buf[CONFIG_LOG_DUMP_NUMBUFS][CONFIG_LOG_DUMP_CHUNK_SIZE];	/* initial uncompressed log dump data */
static bool uncomp_buf_full[CONFIG_LOG_DUMP_NUMBUFS];
static int uncomp_idx = 0;
static int comp_idx = 0;
static int uncomp_curbytes = 0;
static char comp_size[LOG_DUMP_COMPRESS_NODESZ];
static bool set_comp_head = false;
static size_t writesize = CONFIG_LOG_DUMP_CHUNK_SIZE;
static unsigned char *out_buf;
static bool compress_full_block;	/* indicate completion of complete block compression */
static int compress_ret;

/* Structures used to compress last partially filled block */
static unsigned char *last_comp_block;
static size_t last_comp_block_size;
static int last_comp_block_ptr;
static bool compress_last_block;

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
		lldbg("memory allocation failure\n");
		return LOG_DUMP_MEM_FAIL;
	}

	/* The buffers allocated below are required for log-dump till the 
	 * device is shut down or restarted. So these buffers MUST NEVER
	 * BE FREED. 
	 */
	out_buf = allocate_compress_buffer(0, CONFIG_LOG_DUMP_CHUNK_SIZE);
	if (out_buf == NULL) {
		lldbg("memory allocation failure\n");
		goto exit_with_node;
	}

	last_comp_block = allocate_compress_buffer(4, CONFIG_LOG_DUMP_CHUNK_SIZE);
	if (last_comp_block == NULL) {
		lldbg("memory allocation failure\n");
		goto exit_with_outbuf;
	}


	node->comp_head = 0;		/* since first node */

	for (int i = 0; i < CONFIG_LOG_DUMP_NUMBUFS; i++) {
		uncomp_buf_full[i] = false;
	}

	uncomp_idx = 0;
	comp_idx = 0;
	compress_curbytes = 0;
	compress_rdptr = 0;
	sq_addfirst((sq_entry_t *)node, &log_dump_chunks);
	/* set the size of chunks to 1 chunk */
	log_dump_size = LOG_CHUNK_SIZE;
	/* Initialize to save the log by default */
	is_started_to_save = true;

	return LOG_DUMP_OK;

exit_with_outbuf:
	kmm_free(out_buf);
exit_with_node:
	kmm_free(node);

	return LOG_DUMP_MEM_FAIL;
}

/* This is used to wake up the compression thread to compress current log data */
int log_dump_read_wake(void)
{
	compress_last_block = true;
	int ret;
	int msg_compress = true;
	ret = mq_send(mq_fd, (const char *)&msg_compress, sizeof(int), 100);
	if (ret < 0) {
		lldbg("failed mq_send ret = %d\n", ret);
		return ERROR;
	}

	/* wait for the completion of the partially filled block compression */
	while (compress_last_block) ;
	return 0;
}

/* This function is called during read and compress the last partially filled block */
int log_dump_compress_lastblock(void)
{
	read_node = (struct log_dump_chunk_s *)sq_peek(&log_dump_chunks);	/* reset the read node to head */

	if (read_node == NULL) {
		lldbg("empty read node\n");
		return LOG_DUMP_MEM_FAIL;
	}

	if (read_node->comp_head < 0) {
		compress_rdptr = compress_curbytes;
	} else {
		compress_rdptr = read_node->comp_head;
	}

	last_comp_block_size = CONFIG_LOG_DUMP_CHUNK_SIZE;

	compress_ret = compress_block(&last_comp_block[4], &last_comp_block_size, uncomp_buf[uncomp_idx], uncomp_curbytes);

	if (compress_ret != LOG_DUMP_OK) {
		lldbg("Fail to compress compress_ret = %d\n", compress_ret);
		return -compress_ret;
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
	/* If kernel heap is locked, then we cannot perform any
	 * mem free opeartions. So we just return here. The mem
	 * free will be done when the kernel heap is released
	 */
	if (IS_KMM_LOCKED()) {
		return;
	}

	if (log_dump_size > max_size) {
		/* remove extra chunks from start */
		size_t extra_chunk_size = log_dump_size - max_size;
		int extra_chunks_count = extra_chunk_size / LOG_CHUNK_SIZE;
		struct log_dump_chunk_s *next_chunk;

		for (int i = 0; i < extra_chunks_count; i++) {
			next_chunk = (struct log_dump_chunk_s *)sq_remfirst(&log_dump_chunks);
			compress_rdptr = 0;	/* reset the read pointer as the head is removed */
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

	if (compress_curbytes == CONFIG_LOG_DUMP_CHUNK_SIZE) {	/* last chunk is full */

		if (log_dump_size + LOG_CHUNK_SIZE < max_size) {
			/* memory available, so fetch a new buffer */

			struct log_dump_chunk_s *node = (struct log_dump_chunk_s *)kmm_malloc(LOG_CHUNK_SIZE);

			if (node == NULL) {
				return LOG_DUMP_MEM_FAIL;
			}

			compress_curbytes = 1;
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
			compress_curbytes = 1;
			compress_rdptr = 0;	/* reset the read pointer as head is modified */
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
		log_dump_tail->arr[compress_curbytes] = ch;
		compress_curbytes += 1;
		if (set_comp_head) {	/* new node, so compressed block starts at 0 */
			if (log_dump_tail->comp_head < 0) {	/* compressed node not present */
				log_dump_tail->comp_head = compress_curbytes - 1;
			}
			set_comp_head = false;
		}
		if (compress_curbytes % CONFIG_LOG_DUMP_MEMCHECK_SIZE == 0) {
			log_dump_mem_check(max_size);
			return LOG_DUMP_OK_HEAPCHK;
		}
	}
	return LOG_DUMP_OK;
}

static int compress_full_bufs(void)
{
	int ret = 0;
	int i = uncomp_idx + 1;
	if (i == CONFIG_LOG_DUMP_NUMBUFS) {
		i = 0;
	}

	while (i != uncomp_idx) {

		if (!uncomp_buf_full[i]) {
			i++;
			if (i == CONFIG_LOG_DUMP_NUMBUFS) {
				i = 0;
			}
			continue;
		}
		size_t free_size = 0;
		int msg_compress = false;
		/* compress the block, add it to the nodes, reset the uncomp_buf */
		set_comp_head = true;
		compress_full_block = true;
		comp_idx = i;

		mq_send(mq_fd, (const char *)&msg_compress, sizeof(int), 100);

		/* wait for completion of the current full block compression */
		while (compress_full_block) ;

		if (compress_ret < 0) {
			lldbg("Fail to compress ret = %d\n", compress_ret);
			return compress_ret;
		}
		snprintf(comp_size, LOG_DUMP_COMPRESS_NODESZ, "%04d", writesize);
		for (int m = 0; m < LOG_DUMP_COMPRESS_NODESZ - 1; m++) {
			ret = log_dump_tobuffer(comp_size[m], &free_size);
			if (ret < 0) {
				return ret;
			} else if (ret == LOG_DUMP_OK_HEAPCHK) {
				char fsz_array[LOG_DUMP_STRLEN_FREEHEAP];	/* For Free heap memory string */
				snprintf(fsz_array, sizeof(size_t), "<fsz:%08d>", free_size);
				for (int n = 0; n < sizeof(fsz_array); n++) {
					uncomp_buf[uncomp_idx][uncomp_curbytes++] = fsz_array[n];
				}
			}
		}

		for (int write_idx = 0; write_idx < writesize; write_idx++) {
			ret = log_dump_tobuffer(out_buf[write_idx], &free_size);
			if (ret < 0) {
				return ret;
			}
			if (ret == LOG_DUMP_OK_HEAPCHK) {
				char fsz_array[LOG_DUMP_STRLEN_FREEHEAP];	/* For Free heap memory string */
				snprintf(fsz_array, sizeof(size_t), "<fsz:%08d>", free_size);
				for (int fsz_idx = 0; fsz_idx < sizeof(fsz_array); fsz_idx++) {
					uncomp_buf[uncomp_idx][uncomp_curbytes++] = fsz_array[fsz_idx];
				}
			}
		}

		uncomp_buf_full[i] = false;
		i++;
		if (i == CONFIG_LOG_DUMP_NUMBUFS) {
			i = 0;
		}
	}
	return ret;
}

int log_dump_save(char ch)
{
	if (is_started_to_save == false) {
		return LOG_DUMP_OK;
	}

	/* If the uncompress buffer is already full and not released because
	 * the compression is blocked by kernel heap lock, then we will not
	 * accept any more data into the uncompress buffer.
	 */
	if (!uncomp_buf_full[uncomp_idx]) {
		uncomp_buf[uncomp_idx][uncomp_curbytes] = ch;
		uncomp_curbytes++;
		if (uncomp_curbytes == CONFIG_LOG_DUMP_CHUNK_SIZE) {
			uncomp_curbytes = 0;	/* reset */
			uncomp_buf_full[uncomp_idx] = true;
			int temp = uncomp_idx++;
			if (uncomp_idx == CONFIG_LOG_DUMP_NUMBUFS) {
				uncomp_idx = 0;
			}
			if (uncomp_buf_full[uncomp_idx]) {
				uncomp_idx = temp;
			}

			/* If kernel heap is locked we cannot allocate new buffers.
			 * Also, we cannot perform compress operation, since the compression
		 	 * code internally tries to allocate memory. So, we will just ignore
			 * the log data until the kernel heap lock is released.
		 	 */
			if (IS_KMM_LOCKED()) {
				return LOG_DUMP_MEM_FAIL;
			}

			/* If the current thread priority is greater than the compress thread
			 * priority, then there will be a lockup since both threads cannot proceed. 
			 * So, in this case, we will return without performing the compression.
			 */
			if (sched_self()->sched_priority > CONFIG_LOG_DUMP_PRIO) {
				return LOG_DUMP_FAIL;
			}

			return compress_full_bufs();
		}
	}

	return LOG_DUMP_OK;
}

size_t log_dump_read(FAR char *buffer, size_t buflen)
{
	size_t ret = 0;

	sched_lock();	/* to ensure that the read is not disturbed by add_char */

	while (ret < buflen) {
		if (read_node == (struct log_dump_chunk_s *)sq_tail(&log_dump_chunks)) {	/* last chunk */
			if (compress_rdptr == compress_curbytes) {	/* all data has been read */
				if (last_comp_block_ptr < last_comp_block_size) {
					buffer[ret] = last_comp_block[last_comp_block_ptr];
					ret++;
					last_comp_block_ptr++;
				} else {
					break;
				}
			} else {
				buffer[ret] = read_node->arr[compress_rdptr];
				compress_rdptr++;
				ret++;
			}
		} else {	/* reading a filled chunk */

			buffer[ret] = read_node->arr[compress_rdptr];
			compress_rdptr++;
			ret++;
			if (compress_rdptr == CONFIG_LOG_DUMP_CHUNK_SIZE) {	/* completed reading a chunk */
				compress_rdptr = 0;
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
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags = 0;

	/* Create log dump message queue */
	mq_fd = mq_open(LOG_DUMP_MSGQ, O_RDWR | O_CREAT, 0666, &attr);
	if (mq_fd == (mqd_t)ERROR) {
		lldbg("Fail to open mq, errno %d. EXIT!\n", errno);
		return 0;
	}

	if (log_dump_init() != LOG_DUMP_OK) {
		lldbg("Fail to init log dump\n");
		mq_close(mq_fd);
		return 0;
	}

	int msg_compress;
	int nbytes;

	while (true) {
		nbytes = mq_receive(mq_fd, (char *)&msg_compress, sizeof(int), NULL);
		if (nbytes <= 0) {
			continue;
		}
		if (msg_compress) {
			/* To compress last partially filled block during app read*/
			log_dump_compress_lastblock();
			compress_last_block = false;
		} else {
			/* compress the completely filled block */
			writesize = CONFIG_LOG_DUMP_CHUNK_SIZE;
			compress_ret = compress_block(out_buf, &writesize, uncomp_buf[comp_idx], CONFIG_LOG_DUMP_CHUNK_SIZE);
			if (compress_ret != LOG_DUMP_OK) {
				lldbg("Fail to compress compress_ret = %d\n", compress_ret);
			}
			compress_full_block = false;
		}
	}
}
