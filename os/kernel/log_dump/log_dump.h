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

#ifndef __LOG_DUMP_LOG_DUMP_H
#define __LOG_DUMP_LOG_DUMP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <queue.h>
#include <mqueue.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#define LOG_DUMP_OK_HEAPCHK		1	/* To check heap available after success return */
#define LOG_DUMP_STRLEN_FREEHEAP	15	/* String size to display Free heap size */
#define LOG_DUMP_COMPRESS_NODESZ	5	/* use 4 char to store compressed node size */
#define LOG_DUMP_COMPRESS_FAIL	-1
#define LOG_DUMP_MSGQ_FAIL	-2
#define LOG_DUMP_MEM_FAIL	-3

//save
#define SAVE_START() (can_save=true)
#define SAVE_STOP() (can_save=false)
#define IS_POSSIBLE_TO_SAVE() (can_save==true)

//uncomp ring buf
#define UNCOMP_TAIL_PTR (g_uncomp_queue.tail)
#define UNCOMP_HEAD_PTR (g_uncomp_queue.head)

#define UNCOMP_BUF(i) (g_uncomp_queue.item[i])
#define UNCOMP_SPARE_BUF (g_uncomp_queue.spare_buf)

#define UNCOMP_TAIL (UNCOMP_BUF(UNCOMP_TAIL_PTR))
#define UNCOMP_HEAD (UNCOMP_BUF(UNCOMP_HEAD_PTR))

#define UNCOMP_NEXT(idx) ((idx + 1) % CONFIG_LOG_DUMP_UNCOMP_RING_SIZE)
#define UNCOMP_TAIL_NEXT() ((UNCOMP_TAIL_PTR) = (UNCOMP_NEXT(UNCOMP_TAIL_PTR)))
#define UNCOMP_HEAD_NEXT() ((UNCOMP_HEAD_PTR) = (UNCOMP_NEXT(UNCOMP_HEAD_PTR)))

#define IS_UNCOMP_RING_FULL() ((UNCOMP_NEXT(UNCOMP_TAIL_PTR))==(UNCOMP_HEAD_PTR))
#define IS_UNCOMP_RING_EMPTY() ((UNCOMP_TAIL_PTR) == (UNCOMP_HEAD_PTR))
#define IS_UNCOMP_NODE_FULL(idx) ((UNCOMP_BUF(idx).curbytes) == (CONFIG_LOG_DUMP_UNCOMP_BUF_SIZE))
#define IS_UNCOMP_TAIL_FULL() (IS_UNCOMP_NODE_FULL(UNCOMP_TAIL_PTR))
#define IS_UNCOMP_HEAD_FULL() (IS_UNCOMP_NODE_FULL(UNCOMP_HEAD_PTR))
#define IS_UNCOMP_FULL() (IS_UNCOMP_RING_FULL() && IS_UNCOMP_TAIL_FULL())

#define UNCOMP_TAIL_WRITE(ch) ((UNCOMP_TAIL.arr[(UNCOMP_TAIL.curbytes)++]) = ch)

// comp_chunk_utils
#define comp_chunk_alloc(size) ((log_dump_chunk_t *)kmm_malloc(size))
#define comp_chunk_peek() ((log_dump_chunk_t *)sq_peek(&(g_comp_queue.log_dump_chunks)))
#define comp_chunk_tail() ((log_dump_chunk_t *)sq_tail(&(g_comp_queue.log_dump_chunks)))
#define comp_chunk_next(node) ((log_dump_chunk_t *)sq_next((sq_entry_t *)(node)))
#define comp_chunk_append(node) (sq_addlast(((sq_entry_t *)(node)), &(g_comp_queue.log_dump_chunks)))
#define comp_chunk_pop() ((log_dump_chunk_t *)sq_remfirst(&(g_comp_queue.log_dump_chunks)))

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct log_dump_msg_s {
	int uncomp_idx;
};
typedef struct log_dump_msg_s log_dump_msg_t;

struct uncomp_node_s {
	char arr[CONFIG_LOG_DUMP_UNCOMP_BUF_SIZE];
	int curbytes;
	int free_size;
	sem_t buf_sem;	
};
typedef struct uncomp_node_s uncomp_node_t;

struct spare_node_s {
	char arr[CONFIG_LOG_DUMP_SPARE_BUF_SIZE];
	int sparebytes;
};
typedef struct spare_node_s spare_node_t;

/* emergency buf must be less than uncomp buf size*/
struct log_dump_uncomp_ring_buf_s {
	uncomp_node_t item[CONFIG_LOG_DUMP_UNCOMP_RING_SIZE];
	spare_node_t spare_buf;
	int head;
	int tail;
};
typedef struct log_dump_uncomp_ring_buf_s log_dump_uncomp_ring_buf_t;

/* log_dump_chunk is the compressed data node */
struct log_dump_chunk_s {
	struct log_dump_chunk_s *flink;
	char arr[CONFIG_LOG_DUMP_CHUNK_SIZE];
};
typedef struct log_dump_chunk_s log_dump_chunk_t;

struct log_dump_comp_queue_s {
	sq_queue_t log_dump_chunks;
	int curbytes;
	int size_of_chunks;
};
typedef struct log_dump_comp_queue_s log_dump_comp_queue_t;

struct log_dump_read_manager_s {
	log_dump_chunk_t *read_node;
	sem_t read_sem;
	int readbytes;
};
typedef struct log_dump_read_manager_s log_dump_read_manager_t;

#define LOG_CHUNK_SIZE			sizeof(log_dump_chunk_t)

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern bool can_save;

extern char *g_out_buf;

extern log_dump_comp_queue_t g_comp_queue;
extern log_dump_uncomp_ring_buf_t g_uncomp_queue;

extern log_dump_read_manager_t g_read_manager;

extern mqd_t mq_fd;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* Functions contained in log_dump_set_mode.c ***********************************/
int log_dump_set_mode(FAR const char *buffer, size_t buflen);

/* Functions contained in log_dump_read.c ***********************************/
size_t log_dump_read(FAR char *buffer, size_t buflen);

/* Functions contained in log_dump_save.c ***********************************/
int log_dump_compress_and_move_tail(void);

/* Functions contained in log_dump_initialize.c ***********************************/
int log_dump_initialize(void);

/* Functions contained in log_dump_compress.c ***********************************/
int log_dump_compress(char *out_buf, char *in_buf, long unsigned int in_buf_size);

/* Functions contained in log_dump_mem_check.c ***********************************/
void log_dump_mem_check(size_t max_size);

/* Functions contained in log_dump_append.c ***********************************/
size_t log_dump_get_free_size(void);

/* Functions contained in log_dump_append.c ***********************************/
int log_dump_append(const char *compressed_buffer, const int compressed_size);

#endif		/* __LOG_DUMP_LOG_DUMP_H */
