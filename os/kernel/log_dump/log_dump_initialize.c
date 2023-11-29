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
#include <tinyara/sched.h>
#include <tinyara/config.h>

#include <semaphore.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LOG_DUMP_MSGQ	"logdump_s"

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/
/****************************************************************************
 * Public Variables
 ****************************************************************************/

log_dump_comp_queue_t g_comp_queue;
log_dump_uncomp_ring_buf_t g_uncomp_queue; 

char *g_out_buf;

log_dump_read_manager_t g_read_manager;
bool can_save;

mqd_t mq_fd;

/****************************************************************************
 * Private Variables
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/

static mqd_t init_message_queue(void)
{
	struct mq_attr attr;
	mqd_t _mq_fd;

	attr.mq_maxmsg = 32;
	attr.mq_msgsize = sizeof(log_dump_msg_t);
	attr.mq_flags = 0;

	_mq_fd = mq_open(LOG_DUMP_MSGQ, O_RDWR | O_CREAT, 0666, &attr);
	return _mq_fd;
}

static void init_comp_queue(log_dump_comp_queue_t *cq)
{
	sq_init(&(cq->log_dump_chunks));
	cq->curbytes = 0;
	cq->size_of_chunks = 0;
}

static void init_uncomp_node(uncomp_node_t *uncn)
{
	sem_init(&uncn->buf_sem, 0, 1);
	uncn->curbytes = 0;
	uncn->free_size = -1;
}

static void init_spare_node(spare_node_t *spn)
{
	spn->sparebytes = 0;
}

static void init_uncomp_queue(log_dump_uncomp_ring_buf_t *ring)
{
	ring->tail = ring->head = 0;
	for (int i = 0; i < CONFIG_LOG_DUMP_UNCOMP_RING_SIZE; i++) {
		init_uncomp_node(&ring->item[i]);
	}

	init_spare_node(&ring->spare_buf);
}

static void init_read_manager(log_dump_read_manager_t *rm)
{
	rm->read_node = NULL;
	sem_init(&rm->read_sem, 0, 1);
	rm->readbytes = 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int log_dump_initialize(void)
{
	init_comp_queue(&g_comp_queue);
	init_uncomp_queue(&g_uncomp_queue);
	init_read_manager(&g_read_manager);

	mq_fd = init_message_queue();
	if (mq_fd == (mqd_t)ERROR) {
		lldbg("Fail to open mq, errno %d. EXIT!\n", errno);
		mq_close(mq_fd);
		return LOG_DUMP_MSGQ_FAIL;
	}

	log_dump_chunk_t *node = comp_chunk_alloc(LOG_CHUNK_SIZE);

	if (node == NULL) {
		lldbg("memory allocation failure\n");
		return LOG_DUMP_MEM_FAIL;
	}

	g_out_buf = allocate_compress_buffer(0, CONFIG_LOG_DUMP_CHUNK_SIZE);
	if (g_out_buf == NULL) {
		lldbg("memory allocation failure\n");
		goto exit_with_node;
	}

	comp_chunk_append(node);

	g_comp_queue.size_of_chunks = LOG_CHUNK_SIZE;
	SAVE_START();

	return LOG_DUMP_OK;

exit_with_node:
	kmm_free(node);

	return LOG_DUMP_MEM_FAIL;
}
